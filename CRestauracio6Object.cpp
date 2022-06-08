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
    return false;
}
//És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
bool CRestauracio6Object::SendMeNow(CSimulationObject* tincEspai) {
    return false;
};

//Processar un esdeveniment de simulació, funció pura que us toca implementar
void CRestauracio6Object::processEvent(CSimulationEvent* event) {
    if (event->getEntity() == NULL)
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i no rebo entitat.";
    else
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i  rebo entitat " + to_string(event->getEntity()->getId());

    float tempsEvent = 0;
    if (event->getEventType() == ePUSH) {
        if (getState() == IDLE) {
            ocupa_taula();
            tempsEvent = delay();
            CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
            m_Simulator->scheduleEvent(eventPush);
            std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
            setState(SERVICE);
        }
        if (getState() == SERVICE) {
            if (!taula_disponible()) {              //no hi ha taula
                cola_in.push(event);
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
    if (event->getEventType() == eSERVICE) {
        if (m_category > 0)
        {
            cola_out.push(event);
            if (cola_in.size() > 0) {
                tempsEvent = delay();
                CSimulationEvent* nextEvent = cola_in.front();
                CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, nextEvent->getEntity(), eSERVICE);
                cola_in.pop();
                m_Simulator->scheduleEvent(eventPush);
                std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
            }
            else {
                desocupa_taula();
                if (nClients == 0) setState(IDLE);
            }
            //
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
void CRestauracio6Object::simulationStart() {
    setState(IDLE);
    temps_estada();
    cout << getName() << " inicialitzat \n";
}
//Métode que el simulador us pot invocar a la finalització de l'estudi
void CRestauracio6Object::simulationEnd() {
    setState(IDLE);
}