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
    CEntity* nou = emissor->getCurrentEntity();
    //CPassenger* pax = (CPassenger*)nou->getEntity();
    if (nou) {          //pmr? si
        if (cola_in_PMR.size() < capacitat || PMRprocess == false) return true;
        return false;
    }
    else {
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
    CPassenger* pax;
    if (event->getEntity() == NULL)
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i no rebo entitat.";
    else {
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i  rebo entitat " + to_string(event->getEntity()->getId());
        pax=(CPassenger*)event->getEntity();
    }
        

    float tempsEvent;
    if (event->getEventType() == ePUSH) {
        if (pax->isPMR()) {
            if (cuaPMR.size() == 0) {
                //endservice
                cuaPMR.push_back(pax);
                tempsEvent = rand() % 90 + event->getTime();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                m_Simulator->scheduleEvent(eventPush);
            }
            else {
                cuaPMR.push_back(pax);
            }
        }
        else {
            if (cua1.size() == 0) {
                cua1.push_back(pax);
                tempsEvent = rand() % 90 + event->getTime();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                m_Simulator->scheduleEvent(eventPush);
            }
            else if (cua2.size() == 0) {
                cua2.push_back(pax);
                tempsEvent = rand() % 90 + event->getTime();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                m_Simulator->scheduleEvent(eventPush);
            }
            else if (cua3.size() == 0) {
                cua3.push_back(pax);
                tempsEvent = rand() % 90 + event->getTime();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
                m_Simulator->scheduleEvent(eventPush);
            }
            else {
                if (cua1.size() < cua2.size() && cua1.size() < cua3.size()) {
                    cua1.push_back(pax);
                }
                else if (cua2.size() < cua3.size() && cua2.size() < cua1.size()) {
                    cua2.push_back(pax);
                }
                else cua3.push_back(pax);
            }
        }
        tempsEvent = rand() % 90 + event->getTime();
        CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
        m_Simulator->scheduleEvent(eventPush);
        std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
        setState(SERVICE);
    }
    int a = 120;
    if (event->getEventType() == eSERVICE) {
        if (m_category > 0)
        {
            std::list<struct__route> destins;
            destins = m_Simulator->nextObject(event->getEntity(), this);
            int atzar = rand() % destins.size();
            for (int i = 0; i < atzar; i++) {
                destins.pop_front();
            }
            struct__route candidat = destins.front();
            CSimulationEvent* eventService;
            if (candidat.destination == NULL) {
                std::cout << " i no se trobar el  meu següent destí" + to_string(event->getEntity()->getId()) + "\n";
                destins = m_Simulator->nextObject(event->getEntity(), this);
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