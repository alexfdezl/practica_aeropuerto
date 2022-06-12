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
};