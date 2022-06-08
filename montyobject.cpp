#include "montyobject.h"
#include "simulator.h"
#include "simulationobject.h"
#include "simulationevent.h"
#include "passenger.h"
#include "structs.h"
#include <string>

using namespace std;

CMontyObject::CMontyObject(CSimulator* simulator, int category, int id, string nom) :CSimulationObject(simulator, category, id, nom)
{
    setState(SERVICE);
}

//Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
void CMontyObject::showStatistics() {
};

bool CMontyObject::AcceptEntity(CSimulationObject* emissor)
{
    return true;
}
//És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
bool CMontyObject::SendMeNow(CSimulationObject* tincEspai)
{
    return false;
};

//Processar un esdeveniment de simulació, funció pura que us toca implementar
void CMontyObject::processEvent(CSimulationEvent* event) {
    float tempsEvent;
    if (event->getEventType() == ePUSH) {
        ((CPassenger*)event->getEntity())->takeFlight();
        tempsEvent = 90 + event->getTime();
        CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
        m_Simulator->scheduleEvent(eventPush);
        setState(SERVICE);
    }
    int a = 120;
    if (event->getEventType() == eSERVICE) {
        if (m_category > 0)
        {
            std::list<struct__route> destins;
            destins = m_Simulator->nextObject(event->getEntity(), this);
            struct__route candidat = destins.front();
            CSimulationEvent* eventService;
            if (candidat.destination == NULL) {
                std::cout << " i no se trobar el  meu següent destí" + to_string(event->getEntity()->getId()) + "\n";
                destins = m_Simulator->nextObject(event->getEntity(), this);
                setState(IDLE);
            }
            else {
                tempsEvent = candidat.time + m_Simulator->time();
                if (candidat.destination->AcceptEntity(this))
                {
                    eventService = new CSimulationEvent(tempsEvent, this, candidat.destination, event->getEntity(), ePUSH);
                    m_Simulator->scheduleEvent(eventService);
                    setState(IDLE);
                }
                else {
                    sendMeNowMap[candidat.destination].push_back(event->getEntity());
                }
            }
        }
    }
}

//Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
void CMontyObject::simulationStart() {
    setState(IDLE);
}
//Métode que el simulador us pot invocar a la finalització de l'estudi
void CMontyObject::simulationEnd() {
    setState(IDLE);
}