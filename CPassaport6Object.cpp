#include "CPassaport6Object.h"
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

CPassaport6Object::CPassaport6Object(CSimulator* simulator, int category, int id, string nom, int cap) :CSimulationObject(simulator, category, id, nom)
{
    setState(SERVICE);
    capacitat = cap;
}

piecewise_linear_distribution<double> CPassaport6Object::triangular_distribution(double min, double peak, double max)
{
    array<double, 3> i{ min, peak, max };
    array<double, 3> w{ 0, 1, 0 };
    return piecewise_linear_distribution<double>{i.begin(), i.end(), w.begin()};
}

void CPassaport6Object::temps_incidents() {
    random_device rd;
    // create a mersenne twister PRNG seeded from some implementation-defined random source
    mt19937 gen(rd());
    g = gen;
    // create a triangular distribution with a minimum of 0, a peak at 20, and a maximum of 30
    dist1 = triangular_distribution(10, 25, 15); //no_incidents
    dist2 = triangular_distribution(25, 120, 50); //minor_incidents(random control or minor_incident)
    dist3 = triangular_distribution(120, 1800, 600); //serious_incidents
}

//Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
void CPassaport6Object::showStatistics() {
    std::cout << " Han entrat " << passengersIn << " passengers i han sortit " << passengersOut << " passengers \n";
    std::cout << " De tots els passengers que han passat, un " << float((contadorPMR/passengersIn)*100) << "% son PMR \n";
};

bool CPassaport6Object::AcceptEntity(CSimulationObject* emissor) {
    if (cola_in.size() + cola_in_PMR.size() < capacitat) return true;
    else {
        pendingAcceptList.push_back(emissor);
        return false;
    }
}
//És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
bool CPassaport6Object::SendMeNow(CSimulationObject* tincEspai) {
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

float CPassaport6Object::delay() {
    int probabilitat = rand() & 100;
    float tempsEvent;
    if (probabilitat < 80) {            //passa el control sense cap incident
        tempsEvent = dist1(g);
    }
    else if (probabilitat >= 80 && probabilitat < 97) {             //control aleatori i incident greu
        tempsEvent = dist2(g);
    }
    else if (probabilitat >= 97 && probabilitat < 100) {                //incident greu
        tempsEvent = dist3(g);
    }
    return tempsEvent;
}

//Processar un esdeveniment de simulació, funció pura que us toca implementar
void CPassaport6Object::processEvent(CSimulationEvent* event) {
    if (event->getEntity() == NULL)
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i no rebo entitat.";
    else
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i  rebo entitat " + to_string(event->getEntity()->getId());

    float tempsEvent = 0;
    CPassenger* pax = (CPassenger*)event->getEntity();
    if (event->getEventType() == ePUSH) {
        ++passengersIn;
        if (getState() == IDLE) {
            if (pax->isPMR()) {            //PMR
                PMRprocess = true;
                ++contadorPMR;
            }
            else noPMRprocess = true;
            tempsEvent = delay() + event->getTime();
            CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
            m_Simulator->scheduleEvent(eventPush);
            std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
            setState(SERVICE);
        }
        if (getState() == SERVICE) {
            if (pax->isPMR()) {            //PMR
                ++contadorPMR;
                if (PMRprocess) {
                    cola_in_PMR.push(event);
                }
                else {
                    PMRprocess = true;
                    tempsEvent = delay() + event->getTime();
                    CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                    m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
            }
            else {                      //noPMR
                if (noPMRprocess) {
                    cola_in.push(event);
                }
                else {
                    noPMRprocess = true;
                    tempsEvent = delay() + event->getTime();
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
            if (pax->isPMR()) {         //PMR
                cola_out_PMR.push(event);
                if (cola_in_PMR.size() > 0) {
                    tempsEvent = delay() + event->getTime();
                    CSimulationEvent* nextEvent = cola_in_PMR.front();
                    CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, nextEvent->getEntity(), eSERVICE);
                    cola_in_PMR.pop();                    
                    m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {
                    PMRprocess = false;
                    if (!noPMRprocess) setState(IDLE);
                }
            }
            else {                      //noPMR
                cola_out.push(event);
                if (cola_in.size() > 0) {
                    tempsEvent = delay() + event->getTime();
                    CSimulationEvent* nextEvent = cola_in.front();
                    CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, nextEvent->getEntity(), eSERVICE);
                    cola_in.pop();
                    m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {
                    noPMRprocess = false;
                    if (!PMRprocess) setState(IDLE);
                }
            }
            //buscar desti
            std::list<struct__route> destins;
            destins = m_Simulator->nextObject(event->getEntity(), this);
            int mida = destins.size();
            int temps;
            CSimulationEvent* eventService;
            if (pax->isSchengen()) {        //l'envio a la cinta
                //categoria 11
                for (int i = 0; i < mida; i++) {
                    struct__route candidat = destins.front();
                    if (candidat.destination->getCategory() == 11) {
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
            else {      //no l'envio a la cinta, no es Schengen
                int idFinger = pax->getNumberFinger();
                CSimulationObject* finger = m_Simulator->getFinger(idFinger);
                float timeToFinger = m_Simulator->timeTo(finger, pax);
                if (m_Simulator->time() < pax->m_departureTime - 45 - timeToFinger) {
                    //categoria 13
                    for (int i = 0; i < mida; i++) {            //l'envio a restauracio
                        struct__route candidat = destins.front();
                        if (candidat.destination->getCategory() == 13) {
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
                else {          //l'envio al finger
                    //categoria 15
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
            }
            gestioSendMeNow();
        }
    }
}

void CPassaport6Object::gestioSendMeNow() {
    bool b = false;
    while (!pendingAcceptList.empty() && !b) {
        CSimulationObject* primer = pendingAcceptList.front();
        b = primer->SendMeNow(this);
        pendingAcceptList.pop_front();
    }
}

//Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
void CPassaport6Object::simulationStart() {
    setState(IDLE);
    temps_incidents();
    cout << getName() << " inicialitzat \n";
}
//Métode que el simulador us pot invocar a la finalització de l'estudi
void CPassaport6Object::simulationEnd() {
    setState(IDLE);
}