#pragma once
#include "simulationobject.h"
#include "simulator.h"

class CRestauracio6Object:public CSimulationObject {
public:
    CRestauracio6Object(CSimulator* simulator, int category, int id, std::string nom);
    ~CRestauracio6Object() {}
    //M�tode que el simulador us invocar� per a recollir els estad�stics (print per consola)
    void showStatistics();
    //�s una funci� virtial pura aix� que us tocar� implementar-la indiferentment de si la invoqueu o no.
    bool AcceptEntity(CEntity* entitat) { return true; };
    //Processar un esdeveniment de simulaci�, funci� pura que us toca implementar
    void processEvent(CSimulationEvent* event);
    //M�tode que el simulador invocar� a l'inici de la simulaci�, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
    void simulationStart();
    //M�tode que el simulador us pot invocar a la finalitzaci� de l'estudi
    void simulationEnd();
};