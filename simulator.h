#pragma once
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "passenger.h"
#include "simulationobject.h"
#include "simulationevent.h"
#include "structs.h"
#include <fstream>

enum enumLostFlight{CHECKCLOSE,OVERTIME};

class CEventList;
class CDestructorObject;
class CSimulator{
    public:
      //Crearà el model i ho gestionarà tot
      CSimulator();
      //Destroeix el model
      ~CSimulator();
      //Retorna el temps de simulació actual
      float time(){return m_currentTime;};
      //El passatger no arribarà a temps a la porta d'embarcament
      void losesFlight(CPassenger* pax, enumLostFlight rao);
      //Un objecte es queixa de rebre un event que no esperava
      void eventUnexpected(CSimulationEvent* event);
      //Retorna el temps per arribar a un objecte des de la posició actual
      float timeTo(CSimulationObject* desti,CPassenger* pax);
      //Programa un esdeveniment de simulació
      void scheduleEvent(CSimulationEvent* event);
      //Retorna una llista de tuples de possibles destins i temps per arribar
      std::list<struct__route> nextObject(CEntity* entitat, CSimulationObject* objecte);
       //Tracejant events
      void tracing(CSimulationEvent* event,bool scheduled);
      //Tracejant events d'estat
      void tracing(CSimulationObject* objecte);      
      //Crear una entitat que agafa vol
      CPassenger* createEntity(float temps, std::string vol);
      //Crear una entitat que ve amb vol
      CPassenger* createEntity(float temps);
    protected:
      friend class CDestructorObject;
      //Eliminar una entitat
      void deleteEntity(CEntity* entitat);
       //Importa un model des d'un arxiu de text
      void importModel();
      //Crear el mapa de rutes
      void routeMap();
      //Instanciació d'un objecte de simulació a partir d'una cadena de text que defineix la categoria d'objecte i la seva configuració
      CSimulationObject* instantiate(std::string configuracio);
      //Mostrarà els estadístics dels elements de simulació, farà una crida a tots els elements de simulació
      void showStatistics();
      //Envia un missatge a tots els objectes
      void broadcast(int missatge);
      //Executar la simulació
      void run();
      //Comprovació de final de simulació
      bool simulationFinished();
      //Inicialització de la simulació
      void simulationStart();
      //Fi de la simulació
      void simulationEnd();
      //Llista ordenada d'esdeveniments de simulació
      CEventList* m_eventList;
      //Diccionari per a enrutar paxs
      std::map<int,std::vector<CSimulationObject*> > m_objectes;
      //Ruta per agafar vol
      std::map<int,std::vector<int> > m_routeFlight;
      //Ruta per anar a BCN
      std::map<int,std::vector<int> > m_routeLand;
      //Temps de simulació
      float m_currentTime;
      //Arxiu on persistim la traça
      std::ofstream m_traceFile;
      //Comptador únic de recursos
      int m_IDObject;
      //Comptador únic d'entitats
      int m_IDEntity;
      //Dona l'identificador únic de cada objecte
      int newID(){ return ++m_IDObject;};
      //Dona l'identificado únic de l'entitat
      int newIDEntity(){ m_IDEntity++; return m_IDEntity;};
};

