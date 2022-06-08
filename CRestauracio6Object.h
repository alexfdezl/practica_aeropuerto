#pragma once
#include "simulationobject.h"
#include "simulator.h"
#include <map>
#include <list>

using namespace std;

class CRestauracio6Object :public CSimulationObject {
public:
    CRestauracio6Object(CSimulator* simulator, int category, int id, std::string nom);
    ~CRestauracio6Object() {}
    //M�tode que el simulador us invocar� per a recollir els estad�stics (print per consola)
    void showStatistics();
    //�s una funci� virtial pura aix� que us tocar� implementar-la indiferentment de si la invoqueu o no.
    std::map<CSimulationObject*, std::list<CEntity*> > sendMeNowMap;
    //Una llista d'objectes que volen enviarme una entitat
    std::list<CSimulationObject* > pendingAcceptList;
    bool AcceptEntity(CSimulationObject* emissor);
    //�s una funci� virtial pura aix� que us tocar� implementar-la indiferentment de si la invoqueu o no.
    bool SendMeNow(CSimulationObject* tincEspai);
    //Processar un esdeveniment de simulaci�, funci� pura que us toca implementar
    void processEvent(CSimulationEvent* event);
    //M�tode que el simulador invocar� a l'inici de la simulaci�, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
    void simulationStart();
    //M�tode que el simulador us pot invocar a la finalitzaci� de l'estudi
    void simulationEnd();
};