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
#include <deque>

using namespace std;

class CRestauracio6Object :public CSimulationObject {
public:
    CRestauracio6Object(CSimulator* simulator, int category, int id, std::string nom, int capacitatMAX);
    ~CRestauracio6Object() {}
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
    deque<CSimulationEvent* > cola_in;
    deque<CSimulationEvent* > cola_out;
    void CRestauracio6Object::temps_estada();
    float CRestauracio6Object::delay();
    bool CRestauracio6Object::taula_disponible();
    int nClients;
    int capacitat;
    void CRestauracio6Object::ocupa_taula();
    void CRestauracio6Object::desocupa_taula();
    std::mt19937 g;
    std::piecewise_linear_distribution<double> dist;
    std::piecewise_linear_distribution<double> triangular_distribution(double min, double peak, double max);
    std::list<CSimulationObject*> listaPendents;
    std::map<CSimulationObject*, list<CEntity*>> mapaRebutjats;
    std::map<CSimulationObject*, list<CEntity*>>::iterator itMap;
    void CRestauracio6Object::gestioSendMeNow();
    int passengersIn;
    int passengersOut;
};