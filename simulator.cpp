#include "simulator.h"
#include "simulationevent.h"
#include "eventlist.h"
#include "montyobject.h"
#include "destructorObject.h"
#include "functions.h"
#include "passenger.h"
#include <fstream>
#include <string>
#include "CPassaport6Object.h"
#include "CRestauracio6Object.h"
//#include "estudiants.h"
#include <map>
#include <vector>

CSimulator::CSimulator() {
    m_IDObject = 0;
    m_IDEntity = 0;
    //Crear el model
    importModel();
    //Per simplicitat
    run();
}

void CSimulator::routeMap() {
    cout << "********************\n";
    cout << " Route Map per categories  \n";
    cout << "********************\n\n";
    //En un simulador a mida podríem connectar-nos a diferents fonts de dades: taules, API, arxius de text o disposar el model hard-codejat
    string definicioRuta;
    bool toFlight = true;
    ifstream arxiuRutes("runtime//routes.txt");
    if (arxiuRutes.is_open())
    {
        while (getline(arxiuRutes, definicioRuta))
        {
            if (definicioRuta == "toLand") {
                toFlight = false;
                continue;
            }
            //Una ruta és una cadena de Numeros separats per ',' on el primer nombre et marca la categoria d'origen i els darrers nombres les categories dels objectes als que pots arribar
            cout << definicioRuta << '\n';
            std::vector<int> ruta;
            ruta = splitInt(definicioRuta, ',');
            //El primer element no forma part de la ruta
            int clau = ruta[0];
            ruta.erase(ruta.begin());
            if (toFlight)
                m_routeFlight[clau] = ruta;
            else
                m_routeLand[clau] = ruta;
        }
        arxiuRutes.close();
    }
    else
        cout << "Unable to open file";
}

void CSimulator::importModel()
{
    //En un simulador a mida podríem connectar-nos a diferents fonts de dades: taules, API, arxius de text o disposar el model hard-codejat
    string descripcioObjecte;

    cout << "********************\n";
    cout << " Creació del model  \n";
    cout << "********************\n\n";
    ifstream arxiuConfiguracio("runtime//setup.txt");
    if (arxiuConfiguracio.is_open())
    {
        cout << "\nCrear objecte\n";
        cout << "Els objectes pertanyen a una categoria i són autoconfigurables en la seva constructora\n\n";
        while (getline(arxiuConfiguracio, descripcioObjecte))
        {
            std::vector<CSimulationObject*> destinacions;
            cout << descripcioObjecte << '\n';
            CSimulationObject* objecte = instantiate(descripcioObjecte);
            int categoria = objecte->getCategory();
            objecte->setToBCN((categoria > 15 || categoria < 24));
            //Recuperem la categòria de l'objecte per a poder inserir l'objecte a la llista que li toca
            if (m_objectes.count(categoria) != 0) //Cal recuperar l'entrada en el diccionari i afefim el nou objecte
            {
                m_objectes[categoria].push_back(objecte);
            }
            else
            {
                destinacions.push_back(objecte);
                m_objectes[categoria] = destinacions;
            }
        }
        arxiuConfiguracio.close();
    }
    else
        cout << "Unable to open file";


    routeMap();
    std::vector<int> idDestins;
    idDestins = m_routeLand[11];
    routeMap();
}

CSimulationObject* CSimulator::instantiate(std::string configuracio)
{
    std::vector<std::string> opcions;
    opcions = split(configuracio, ',');
    int categoria = std::stoi(opcions[0]);
    int objecte = std::stoi(opcions[1]);
    CSimulationObject* obj;
    switch (objecte) {
    case 1:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 2:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 3:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 4:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 5:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 6:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 7:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 8:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 9:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 10:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 11:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 12:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 13:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 14:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 15:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 16:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        return obj;
        break;
    case 17:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 18:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 19:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 20:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 21:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 22:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 23:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 24:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 25:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 26:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 27:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 28:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 29:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 30:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        // return new CPassadis20Object(this,categoria,newID(),opcions[2],stoi(opcions[4]));
        break;
    case 31:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 32:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 33:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 34:
        return new CPassaport6Object(this, categoria, newID(), opcions[2], 70);
        break;
    case 35:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 36:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 37:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 38:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 39:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 40:
        return new CRestauracio6Object(this, categoria, newID(), opcions[2], 350);
        break;
    case 41:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 42:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 43:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 44:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 45:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 46:
        obj = new CMontyObject(this, categoria, newID(), opcions[2]);
        cout << "Finger Sortida --> " << obj->getName();
        m_fingersSortida.push_back(obj);
        return obj;
        break;
    case 47:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 48:
        return new CMontyObject(this, categoria, newID(), opcions[2]);
        break;
    case 0://Destructor
        m_destructor = new CDestructorObject(this, categoria, newID(), opcions[2]);
        return m_destructor;
        break;
    }
    return NULL;
}

CSimulator::~CSimulator() {
    //Eliminem objectes que formen part del model
    for (auto llista : m_objectes) {
        //Recorrer tots els elements i enviar el simulation start
        for (auto item : llista.second) {
            delete item;
        }
    }
    m_objectes.clear();
    //Eliminem la llista d'esdeveniments
    m_eventList->reset();
    delete m_eventList;
}

void CSimulator::simulationStart()
{
    cout << "********************\n";
    cout << " Simulation Start  \n";
    cout << "********************\n\n";
    m_traceFile.open("runtime//trace.txt");

    //No controlo que l'arxiu estigui obert

    m_eventList = new CEventList();
    broadcast(1);
}

void CSimulator::run() {
    simulationStart();
    int a = 0;
    cout << "\n********************\n";
    cout << " Simulation Run  \n";
    cout << "********************\n\n";
    CSimulationObject* entrada = m_objectes[1].at(0);
    CSimulationObject* final = m_objectes[16].at(0);
    CPassenger* pax;

    // Crear paxs que volen agafar un vol
        for(int i=0;i<300;i++){
            pax=createEntity(0.1f);
            float time=50;
            CSimulationEvent* ev=new CSimulationEvent(time,entrada,entrada,(CEntity*)pax,ePUSH);
            scheduleEvent(ev);
        }
        //Crear paxs que volen tornar a BCN
    for (int i = 0; i < 300; i++) {
        pax = createEntity(0.1f + i * 15, "SNC219");
        float time = 5;
        CSimulationEvent* ev = new CSimulationEvent(time, final, final, (CEntity*)pax, ePUSH);
        scheduleEvent(ev);
    }

    while (!simulationFinished())
    {
        CSimulationEvent* event = (CSimulationEvent*)this->m_eventList->remove();
        m_currentTime = event->getTime();
        tracing(event, true);
        event->getConsumer()->setCurrentEntity(event->getEntity());
        event->executed();
        event->getConsumer()->setCurrentEntity(NULL);
        delete event;
    }
    showStatistics();
    simulationEnd();
}

void CSimulator::deleteEntity(CEntity* entitat) {
    delete entitat;
}
void CSimulator::showStatistics() {
    cout << "\n********************\n";
    cout << " Show Statistics  \n";
    cout << "********************\n\n";
    broadcast(2);
};

void CSimulator::simulationEnd() {
    cout << "\n********************\n";
    cout << " Simulation End  \n";
    cout << "********************\n\n";
    m_traceFile.close();
}

void CSimulator::broadcast(int missatge) {
    for (auto llista : m_objectes) {
        //Recorrer tots els elements i enviar el simulation start
        for (auto item : llista.second) {
            switch (missatge) {
            case 1:
                item->simulationStart();
                break;
            case 2:
                item->showStatistics();
                break;
            case 3:
                item->simulationEnd();
                break;
            }
        }
    }
}
bool CSimulator::simulationFinished() {
    //Executo 1200 unitats de temps com a mínim (no en feu cas d'això, que el temps es fixarà més endavant)
    //return m_currentTime > 1200 || m_eventList->isEmpty();
    return m_eventList->isEmpty();
}

void CSimulator::losesFlight(CPassenger* pax, enumLostFlight rao) {
    //Comprovar si és cert o no
    pax->lostFlight();
}

void CSimulator::eventUnexpected(CSimulationEvent* event) {
    //event->isUnexpected();
}

void CSimulator::scheduleEvent(CSimulationEvent* event) {
    if (event->getEventType() == ePUSH) {
        tracing(event, false);
    }
    m_eventList->insert(event);
}

CPassenger* CSimulator::createEntity(float temps, string vol) {
    return new CPassenger(newIDEntity(), temps, vol);
}

CPassenger* CSimulator::createEntity(float temps) {
    return new CPassenger(newIDEntity(), temps);
}

std::list<struct__route> CSimulator::nextObject(CEntity* entitat, CSimulationObject* objecte) {
    std::list<struct__route> rutes;
    std::vector<int> idDestins;

    CPassenger* pax = (CPassenger*)entitat;

    if (pax->HaslostFlight()) {
        cout << "\n\nperd un vol\n\n";
        struct__route itemRoute = struct__route(m_destructor, 0);
        rutes.push_back(itemRoute);
    }

    if (pax->takeFlight()) {
        idDestins = m_routeFlight[objecte->getCategory()];
    }
    else {
        idDestins = m_routeLand[objecte->getCategory()];
    }

    float temps;

    //Atenció: 'auto' type specifier is a C++11 extension [-Wc++11-extensions]gcc 
    for (auto categoria : idDestins) {
        for (auto desti : m_objectes[categoria])
        {
            temps = std::abs((desti->getCategory() - objecte->getCategory()) * 15);
            struct__route itemRoute = struct__route(desti, temps);
            rutes.push_back(itemRoute);
        }
    }

    return rutes;
}
CSimulationObject* CSimulator::getFinger(int idFinger) {
    return m_fingersSortida[idFinger];
}

float CSimulator::timeTo(CSimulationObject* desti, CPassenger* pax) {
    float temps = std::abs((desti->getCategory() - pax->getPlace()->getCategory()) * 15);
    return temps;
}

void CSimulator::tracing(CSimulationEvent* event, bool scheduled) {
    //Sols farem un trace del push i del canvi d'estat.
    if (event->getEventType() == ePUSH) {
        string esdeveniment;
        if (scheduled)//surt un pax
            esdeveniment = to_string(m_currentTime) + " O " + to_string(event->getProvider()->getCategory()) + " " + to_string(event->getConsumer()->getCategory()) + " " + event->getProvider()->getName() + " " + event->getConsumer()->getName() + '\n';
        //  else//arriba un pax
        //      if (event->getConsumer()!=NULL && event->getConsumer()->getCategory()!=0)
        //          esdeveniment=to_string(m_currentTime)+" I "+to_string(event->getConsumer()->getCategory())+'\n';
        m_traceFile << esdeveniment;
    }
}

void CSimulator::tracing(CSimulationObject* objecte) {
    //Trace del canvi d'estat.
    string esdeveniment = to_string(m_currentTime) + " S " + objecte->getName() + " " + to_string(objecte->getState()) + '\n';
    m_traceFile << esdeveniment;
}
