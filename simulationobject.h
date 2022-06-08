#pragma once

#include <iostream>
#include <list>
#include "entity.h"
#include "structs.h"
#include <string>

using namespace std;

class CSimulator;

enum enumStateObject { AVAILABLE, BROKEN, BUSY, CLOSED, EMPTY, ESPATLLADA, FULL, FUNCIONANT, IDLE, INSERVICE, LOADING, MOVING, NOEMPTY, NOTFULL, NOTEMPTY, PARADA, SERVICE, OUTOFSERVICE, REPAIR };

class CSimulationObject {
public:
    CSimulationObject() {};
    //Constructora base
    CSimulationObject(CSimulator* simulator, int category, int id, std::string nom) {
        m_Simulator = simulator;
        m_category = category;
        m_id = id;
        m_name = nom;
    };
    //Destructor de l'objecte
    virtual ~CSimulationObject() {};
    //Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
    virtual void showStatistics() = 0;
    //És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
    virtual bool AcceptEntity(CSimulationObject* emissor) = 0;//Jo puc saber on es troba l'entitat demanant pel getPlace()
    //És una funció virtial pura així que us tocarà implementar-la indiferentment de si la invoqueu o no.
    virtual bool SendMeNow(CSimulationObject* tincEspai) = 0;
    //Retorna l'estat actual de l'objecte
    enumStateObject getState() { return m_state; };
    //Processar un esdeveniment de simulació, funció pura que us toca implementar
    virtual void processEvent(CSimulationEvent* event) = 0;
    //Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
    virtual void simulationStart() = 0;
    //Métode que el simulador us pot invocar a la finalització de l'estudi
    virtual void simulationEnd() = 0;
    //Recupera la categoria d'un objecte
    int getCategory() { return m_category; };
    //Recupera l'identificador únic de l'objecte
    int getID() { return m_id; };
    //Recupera el punter al simulador
    CSimulator* getSimulator() { return m_Simulator; };
    //Recupera el nom de l'objecte
    std::string getName() { return m_name; };
    //dona paxs que voldran baixar de planta
    bool ToBCN() { return m_toBCN; }
    //Fixem categoria objecte
    void setToBCN(bool siono) { m_toBCN = siono; }
    //Retorna l'entitat activa
    CEntity* getCurrentEntity() { return m_currentEntity; }
    //Fixa l'entitat activa
    void setCurrentEntity(CEntity* entitat) { m_currentEntity = entitat; }
protected:
    //Estableix l'estat de l'objecte
    void setState(enumStateObject estat);
    //Estat de simulació en el que es troba l'objecte
    enumStateObject m_state;
    //Punter al simulador
    CSimulator* m_Simulator;
    //Categoria de l'objecte (enrutament)
    int m_category;
    //Identificador únic
    int m_id;
    //nom de l'objecte
    std::string m_name;
    //els paxs d'aquest objecte volen baixar de planta
    bool m_toBCN;
    //Entitat activa en l'objecte, és l'entitat que està associada a l'event que s'esta avaluant
    CEntity* m_currentEntity;
};


