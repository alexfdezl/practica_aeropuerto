#pragma once
#include "simulationobject.h"
#include "simulator.h"

class CRestauracio6Object:public CSimulationObject {
public:
    CRestauracio6Object(CSimulator* simulator, int category, int id, std::string nom);
    ~CRestauracio6Object() {}
    //Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
    void showStatistics();
    //És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
    bool AcceptEntity(CEntity* entitat) { return true; };
    //Processar un esdeveniment de simulació, funció pura que us toca implementar
    void processEvent(CSimulationEvent* event);
    //Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
    void simulationStart();
    //Métode que el simulador us pot invocar a la finalització de l'estudi
    void simulationEnd();
};