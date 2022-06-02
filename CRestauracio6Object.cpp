#include "CRestauracio6Object.h"
#include "simulator.h"
#include "simulationobject.h"
#include "simulationevent.h"
#include "passenger.h"
#include "structs.h"
#include <string>

using namespace std;

CRestauracio6Object::CRestauracio6Object(CSimulator* simulator, int category, int id, string nom) :CSimulationObject(simulator, category, id, nom)
{
	setState(SERVICE);
}

//M�tode que el simulador us invocar� per a recollir els estad�stics (print per consola)
void CRestauracio6Object::showStatistics() {
    cout << getName() << " No tinc estad�stics \n";
};

//Processar un esdeveniment de simulaci�, funci� pura que us toca implementar
void CRestauracio6Object::processEvent(CSimulationEvent* event) {
    if (event->getEntity() == NULL)
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i no rebo entitat.";
    else
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i  rebo entitat " + to_string(event->getEntity()->getId());

    float tempsEvent;
    if (event->getEventType() == ePUSH) {
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
                std::cout << " i no se trobar el  meu seg�ent dest�" + to_string(event->getEntity()->getId()) + "\n";
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

//M�tode que el simulador invocar� a l'inici de la simulaci�, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
void CRestauracio6Object::simulationStart() {
    setState(IDLE);
    cout << getName() << " inicialitzat \n";
}
//M�tode que el simulador us pot invocar a la finalitzaci� de l'estudi
void CRestauracio6Object::simulationEnd() {
    setState(IDLE);
}