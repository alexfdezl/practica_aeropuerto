#include "destructorObject.h"
#include "simulator.h"
#include "simulationobject.h"
#include "simulationevent.h"
#include "passenger.h"
#include "structs.h"
#include <string>

using namespace std;

CDestructorObject::CDestructorObject(CSimulator* simulator, int category, int id, string nom) :CSimulationObject(simulator, category, id, nom)
{
    setState(SERVICE);
}

//Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
void CDestructorObject::showStatistics() {
    cout << getName() << " Entitats destruïdes " << to_string(m_count) << "\n";
};

//Processar un esdeveniment de simulació, funció pura que us toca implementar
void CDestructorObject::processEvent(CSimulationEvent* event) {
    m_Simulator->deleteEntity(event->getEntity());
    m_count++;
}

//Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
void CDestructorObject::simulationStart() {
    setState(IDLE);
    m_count = 0;
    cout << getName() << " inicialitzat \n";
}
//Métode que el simulador us pot invocar a la finalització de l'estudi
void CDestructorObject::simulationEnd() {
    setState(IDLE);
}