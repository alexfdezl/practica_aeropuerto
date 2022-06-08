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
    CPassaport6Object(CSimulator* simulator, int category, int id, std::string nom);
    ~CPassaport6Object() {}
    //Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
    void showStatistics();
    //És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
    std::map<CSimulationObject*, std::list<CEntity*> > sendMeNowMap;
    //Una llista d'objectes que volen enviarme una entitat
    std::list<CSimulationObject* > pendingAcceptList;
    bool AcceptEntity(CSimulationObject* emissor);
    //És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
    bool SendMeNow(CSimulationObject* tincEspai);
    //Processar un esdeveniment de simulació, funció pura que us toca implementar
    void processEvent(CSimulationEvent* event);
    //Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
    void simulationStart();
    //Métode que el simulador us pot invocar a la finalització de l'estudi
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

    std::mt19937 g;
    std::piecewise_linear_distribution<double> dist1;
    std::piecewise_linear_distribution<double> dist2;
    std::piecewise_linear_distribution<double> dist3;
    std::piecewise_linear_distribution<double> triangular_distribution(double min, double peak, double max);

};