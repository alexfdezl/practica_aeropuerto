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

CPassaport6Object::CPassaport6Object(CSimulator* simulator, int category, int id, string nom) :CSimulationObject(simulator, category, id, nom)
{
    setState(SERVICE);
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
    cout << getName() << " No tinc estadístics \n";
};

bool CPassaport6Object::AcceptEntity(CSimulationObject* emissor) {
    CPassenger* pax = (CPassenger*)emissor->getCurrentEntity();
    if (pax->isPMR()) {          //PMR
        if (cola_in_PMR.size() < capacitat || PMRprocess == false) return true;
        return false;
    }
    else {                      //noPMR
        if (cola_in.size() < capacitat || noPMRprocess == false) return true;
        return false;
    }
    return false;
}
//És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
bool CPassaport6Object::SendMeNow(CSimulationObject* tincEspai) {
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
    CPassenger* pass = (CPassenger*)this->getCurrentEntity();
    if (event->getEventType() == ePUSH) {
        if (getState() == IDLE) {
            tempsEvent = delay();
            CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
            m_Simulator->scheduleEvent(eventPush);
            if (pax->isPMR()) {            //PMR
                PMRprocess = true;
            }
            else noPMRprocess = true;
            std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
            setState(SERVICE);
        }
        if (getState() == SERVICE) {
            if (pass->isPMR()) {            //PMR
                if (PMRprocess) {
                    cola_in_PMR.push(event->getProvider()); //NO DEBERIAMOS ALMACENAR LA ENTIDAD??
                    cola_in_PMR.push(this->getCurrentEntity());
                    cola_in_PMR.push(event);
                }
                else {
                    PMRprocess = true;
                    tempsEvent = delay();
                    CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                    m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
            }
            else {                      //noPMR
                if (noPMRprocess) {
                    cua_entrada.push(event->getProvider()); //NO DEBERIAMOS ALMACENAR LA ENTIDAD??
                    cola_in.push(event);
                }
                else {
                    noPMRprocess = true;
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
            if (pax->isPMR()) {         //PMR
                cua_sortida_PMR.push(event->getProvider()); //NO DEBERIAMOS ALMACENAR LA ENTIDAD??
                cola_out_PMR.push(event);
                if (cola_in_PMR.size() > 0) {
                    //newCommer=CuaSeguretatPMRArrive.front()
                    cola_in_PMR.pop();
                    tempsEvent = delay();
                    //CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, //newCommer, eSERVICE); //meto al siguiente tio
                    //m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {
                    PMRprocess = false;
                    if (!noPMRprocessing) setState(IDLE);
                }
            }
            else {                      //noPMR
                cua_sortida.push(event->getProvider()); //NO DEBERIAMOS ALMACENAR LA ENTIDAD??
                if (cua_entrada.size() > 0) {
                    //newCommer=CuaSeguretatPMRArrive.front()
                    cua_entrada.pop();
                    tempsEvent = delay();
                    //CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, //newCommer, eSERVICE); //meto al siguiente tio
                    //m_Simulator->scheduleEvent(eventPush);
                    std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                }
                else {
                    noPMRprocessing = false;
                    if (!PMRprocessing) setState(IDLE);
                }
            }

            //BUSQUEDA DE DESTINOS PARA LOS QUE HAN ACABADO
            std::list<struct__route> destins;
            destins = m_Simulator->nextObject(event->getEntity(), this);
            for (int i = 0; i < destins.size(); i++) {                                      //Elimino los sitios a los que no llego como candidatos
                int hora_vuelo = event->getEntity()->m_departureTime;
                if ((m_Simulator->time() + destins.front().time) >= hora_vuelo) {
                    destins.pop_front();
                }
            }
            struct__route candidat = destins.front();
            CSimulationEvent* eventService;
            if (candidat.destination == NULL) {
                std::cout << " i no se trobar el  meu següent destí" + to_string(event->getEntity()->getId()) + "\n";
                destins = m_Simulator->nextObject(event->getEntity(), this);                                                //???
                setState(IDLE);
            }
            else {
                tempsEvent = candidat.time + m_Simulator->time();
                eventService = new CSimulationEvent(tempsEvent, this, candidat.destination, event->getEntity(), ePUSH);
                m_Simulator->scheduleEvent(eventService);
                std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                setState(IDLE);
            }
        }
    }
    if (event->getEventType() == eFREEEVENT) {}

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