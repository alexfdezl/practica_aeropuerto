#include "CRestauracio6Object.h"
#include "simulator.h"
#include "simulationobject.h"
#include "simulationevent.h"
#include "passenger.h"
#include "structs.h"
#include "entity.h"
#include <string>
#include <random>
#include <iostream>
#include <iomanip>
#include <array>
#include <map>
#include <queue>

using namespace std;

CRestauracio6Object::CRestauracio6Object(CSimulator* simulator, int category, int id, string nom, int capacitatMAX) :CSimulationObject(simulator, category, id, nom)
{
	setState(SERVICE);
    capacitat = capacitatMAX;
}

bool CRestauracio6Object::taula_disponible() {
    if (nClients < 50) return true;
    return false;
}

void CRestauracio6Object::ocupa_taula() {
    nClients++;
}

void CRestauracio6Object::desocupa_taula() {
    nClients--;
}

piecewise_linear_distribution<double> CRestauracio6Object::triangular_distribution(double min, double peak, double max)
{
    array<double, 3> i{ min, peak, max };
    array<double, 3> w{ 0, 1, 0 };
    return piecewise_linear_distribution<double>{i.begin(), i.end(), w.begin()};
}

void CRestauracio6Object::temps_estada() {
    random_device rd;
    // create a mersenne twister PRNG seeded from some implementation-defined random source
    mt19937 gen(rd());
    g = gen;
    dist = triangular_distribution(5, 30, 15);
}

float CRestauracio6Object::delay() {
    float tempsEvent = dist(g);
    return tempsEvent;
}

//Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
void CRestauracio6Object::showStatistics() {
    cout << getName() << " No tinc estadístics \n";
};

bool CRestauracio6Object::AcceptEntity(CSimulationObject* emissor) {
    int quantitat = nClients + cola_in.size();
    if (quantitat < capacitat) return true;
    else {
        listaPendents.push_back(emissor);
        return false;
    }
}
//És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
bool CRestauracio6Object::SendMeNow(CSimulationObject* tincEspai) {
    itMap = mapaRebutjats.find(tincEspai);
    if (itMap != mapaRebutjats.end()) {
        CEntity* entitat = itMap->second.front();
        CPassenger* pax = (CPassenger*)entitat;
        if (pax->HaslostFlight() && pax->takeFlight()) {
            //pop, destruir i return false
            std::list<CEntity*>::iterator itList = itMap->second.begin();
            itMap->second.erase(itList);
            m_Simulator->deleteEntity(entitat);
            return false;
        }
        else {
            //pop, push i return true
            std::list<CEntity*>::iterator itList = itMap->second.begin();
            itMap->second.erase(itList);
            int temps = m_Simulator->time() + m_Simulator->timeTo(tincEspai, pax);
            CSimulationEvent* eventPush = new CSimulationEvent(temps, this, this, entitat, ePUSH);
            m_Simulator->scheduleEvent(eventPush);
            std::cout << " i programo un event service per a l'entitat " + to_string(entitat->getId()) + "\n";
            return true;
        }
    }
    return false;
};

//Processar un esdeveniment de simulació, funció pura que us toca implementar
void CRestauracio6Object::processEvent(CSimulationEvent* event) {
    if (event->getEntity() == NULL)
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i no rebo entitat.";
    else
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i  rebo entitat " + to_string(event->getEntity()->getId());

    float tempsEvent = 0;
    CPassenger* pax = (CPassenger*)event->getEntity();
    if (event->getEventType() == ePUSH) {
        if (getState() == IDLE) {
            if (pax->takeFlight()) {        //agafa vol
                float hora_vuelo = pax->m_departureTime;
                int idFinger = pax->getNumberFinger();
                CSimulationObject* finger = m_Simulator->getFinger(idFinger);
                float temps = m_Simulator->timeTo(finger, pax);
                if (m_Simulator->time() < hora_vuelo - 45 - temps) {            //tinc temps
                    ocupa_taula();
                    tempsEvent = delay();
                    CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                    m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {                  //gestio de no tinc temps
                    CSimulationEvent* eventGoToFinger = new CSimulationEvent(temps + m_Simulator->time(), this, finger, event->getEntity(), ePUSH);
                    m_Simulator->scheduleEvent(eventGoToFinger);
                    std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
            }
            else {              //no agafa vol
                ocupa_taula();
                tempsEvent = delay();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                m_Simulator->scheduleEvent(eventPush);
                std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
            }
            setState(SERVICE);
        }
        if (getState() == SERVICE) {
            if (pax->takeFlight()) {            //vola
                float hora_vuelo = pax->m_departureTime;
                int idFinger = pax->getNumberFinger();
                CSimulationObject* finger = m_Simulator->getFinger(idFinger);
                float temps = m_Simulator->timeTo(finger, pax);
                if (m_Simulator->time() < hora_vuelo - 45 - temps) {            //tinc temps
                    if (!taula_disponible()) {              //no hi ha taula
                        cola_in.push_back(event);
                        CSimulationEvent* eventFiCua = new CSimulationEvent(hora_vuelo-45, this, this, event->getEntity(), eFICUA);
                        m_Simulator->scheduleEvent(eventFiCua);
                        std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                    }
                    else {                  //hi ha taula
                        ocupa_taula();
                        tempsEvent = delay() + event->getTime();
                        CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                        m_Simulator->scheduleEvent(eventPush);
                        std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                    }
                }
                else {          //no te temps
                    CSimulationEvent* eventGoToFinger = new CSimulationEvent(temps+m_Simulator->time(), this, finger, event->getEntity(), ePUSH);
                    m_Simulator->scheduleEvent(eventGoToFinger);
                    std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
            }
            else {          //no vola
                if (!taula_disponible()) {              //no hi ha taula
                    cola_in.push_back(event);
                }
                else {                  //hi ha taula
                    ocupa_taula();
                    tempsEvent = delay();
                    CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                    m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
            }
        }
    }
    if (event->getEventType() == eSERVICE) {
        if (m_category > 0)
        {
            cola_out.push_back(event);
            if (cola_in.size() > 0) {           //tinc cua
                tempsEvent = delay();
                tempsEvent += event->getTime();
                CSimulationEvent* nextEvent = cola_in.front();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, nextEvent->getEntity(), eSERVICE);
                cola_in.pop_front();
                m_Simulator->scheduleEvent(eventPush);
                std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
            }
            else {                  //no tinc cua
                desocupa_taula();
                if (nClients == 0) setState(IDLE);
            }
            //busca desti
            if (pax->takeFlight()) {
                int idFinger = pax->getNumberFinger();
                CSimulationObject* finger = m_Simulator->getFinger(idFinger);
                if (finger->AcceptEntity(this)) {       //accepten l'entitat
                    float temps = m_Simulator->timeTo(finger, pax) + event->getTime();
                    CSimulationEvent* eventService = new CSimulationEvent(temps, this, finger, event->getEntity(), ePUSH);
                    m_Simulator->scheduleEvent(eventService);
                    std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {      //no accepten l'entitat
                    itMap = mapaRebutjats.find(finger);
                    if (itMap != mapaRebutjats.end()) {     //lo tengo
                        std::list<CEntity*> aux = itMap->second;
                        aux.push_back(event->getEntity());
                        mapaRebutjats.erase(itMap);
                        mapaRebutjats.insert(make_pair(finger, aux));
                    }
                    else {          //no lo tengo
                        std::list<CEntity*> aux;
                        aux.push_back(event->getEntity());
                        mapaRebutjats.insert(make_pair(finger, aux));
                    }
                }
            }
            else {
                std::list<struct__route> destins;
                destins = m_Simulator->nextObject(event->getEntity(), this);
                int mida = destins.size();
                CSimulationEvent* eventService;
                int temps;
                if (pax->carryBaggage()) {      //l'envio a l'hipodrom
                    //categoria 17
                    for (int i = 0; i < mida; i++) {
                        struct__route candidat = destins.front();
                        if (candidat.destination->getCategory() == 17) {
                            temps = candidat.time + m_Simulator->time();
                            if (candidat.destination->AcceptEntity(this)) {         //accepten
                                eventService = new CSimulationEvent(temps, this, candidat.destination, event->getEntity(), ePUSH);
                                m_Simulator->scheduleEvent(eventService);
                                std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                            }
                            else {          //no accepten
                                itMap = mapaRebutjats.find(candidat.destination);
                                if (itMap != mapaRebutjats.end()) {     //lo tengo
                                    std::list<CEntity*> aux = itMap->second;
                                    aux.push_back(event->getEntity());
                                    mapaRebutjats.erase(itMap);
                                    mapaRebutjats.insert(make_pair(candidat.destination, aux));
                                }
                                else {          //no lo tengo
                                    std::list<CEntity*> aux;
                                    aux.push_back(event->getEntity());
                                    mapaRebutjats.insert(make_pair(candidat.destination, aux));
                                }
                            }
                        }
                        destins.pop_front();
                    }
                }
                else {          //l'envio a duanes
                    //categoria 18
                    for (int i = 0; i < mida; i++) {
                        struct__route candidat = destins.front();
                        if (candidat.destination->getCategory() == 18) {
                            temps = candidat.time + m_Simulator->time();
                            if (candidat.destination->AcceptEntity(this)) {     //accepten
                                eventService = new CSimulationEvent(temps, this, candidat.destination, event->getEntity(), ePUSH);
                                m_Simulator->scheduleEvent(eventService);
                                std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                            }
                            else {      //no accepten
                                itMap = mapaRebutjats.find(candidat.destination);
                                if (itMap != mapaRebutjats.end()) {     //lo tengo
                                    std::list<CEntity*> aux = itMap->second;
                                    aux.push_back(event->getEntity());
                                    mapaRebutjats.erase(itMap);
                                    mapaRebutjats.insert(make_pair(candidat.destination, aux));
                                }
                                else {          //no lo tengo
                                    std::list<CEntity*> aux;
                                    aux.push_back(event->getEntity());
                                    mapaRebutjats.insert(make_pair(candidat.destination, aux));
                                }
                            }
                        }
                        destins.pop_front();
                    }
                }
            }
            gestioSendMeNow();
        }
    }
    if (event->getEventType() == eFICUA) {
        if (getState() == SERVICE) {
            deque <CSimulationEvent*>::iterator it = find(cola_in.begin(),cola_in.end(),event);
            if (it != cola_in.end()) {
                int idFinger = pax->getNumberFinger();
                CSimulationObject* finger = m_Simulator->getFinger(idFinger);                
                if (finger->AcceptEntity(this)) {           //accepten
                    float temps = m_Simulator->timeTo(finger, pax) + event->getTime();
                    CSimulationEvent* eventFiCua = new CSimulationEvent(temps, this, finger, event->getEntity(), ePUSH);
                    m_Simulator->scheduleEvent(eventFiCua);
                    std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {      //no accepten
                    itMap = mapaRebutjats.find(finger);
                    if (itMap != mapaRebutjats.end()) {     //lo tengo
                        std::list<CEntity*> aux = itMap->second;
                        aux.push_back(event->getEntity());
                        mapaRebutjats.erase(itMap);
                        mapaRebutjats.insert(make_pair(finger, aux));
                    }
                    else {          //no lo tengo
                        std::list<CEntity*> aux;
                        aux.push_back(event->getEntity());
                        mapaRebutjats.insert(make_pair(finger, aux));
                    }
                }
            }
            gestioSendMeNow();
        }
    }
}

void CRestauracio6Object::gestioSendMeNow() {
    if (!listaPendents.empty()) {
        CSimulationObject* primer = listaPendents.front();
        bool b = primer->SendMeNow(this);
        listaPendents.pop_front();
    }
}

//Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
void CRestauracio6Object::simulationStart() {
    setState(IDLE);
    temps_estada();
    cout << getName() << " inicialitzat \n";
}
//Métode que el simulador us pot invocar a la finalització de l'estudi
void CRestauracio6Object::simulationEnd() {
    setState(IDLE);
}