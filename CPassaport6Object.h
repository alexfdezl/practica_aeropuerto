#pragma once
#include "simulationobject.h"
#include "simulator.h"
#include <queue>
#include <list>
#include <random>
#include <iostream>
#include <iomanip>
#include <array>
#include <map>

using namespace std;

class CPassaport6Object :public CSimulationObject {
public:
    CPassaport6Object(CSimulator* simulator, int category, int id, std::string nom, int capacitat);
    ~CPassaport6Object() {}
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

private:
    queue<CSimulationEvent* > cola_in;
    queue<CSimulationEvent* > cola_in_PMR;
    queue<CSimulationEvent* > cola_out;
    queue<CSimulationEvent* > cola_out_PMR;
    int capacitat;
    bool noPMRprocess = false;
    bool PMRprocess = false;
    void CPassaport6Object::temps_incidents();
    float CPassaport6Object::delay();
    std::map<CSimulationObject*, list<CEntity*>> mapaRebutjats;
    std::map<CSimulationObject*, list<CEntity*>>::iterator itMap;
    std::mt19937 g;
    std::piecewise_linear_distribution<double> dist1;
    std::piecewise_linear_distribution<double> dist2;
    std::piecewise_linear_distribution<double> dist3;
    std::piecewise_linear_distribution<double> triangular_distribution(double min, double peak, double max);
    void CPassaport6Object::gestioSendMeNow();

};