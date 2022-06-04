#include "CPassaport6Object.h"
#include "simulator.h"
#include "simulationobject.h"
#include "simulationevent.h"
#include "passenger.h"
#include "structs.h"
#include <string>
#include <random>
#include <iostream>
#include <iomanip>
#include <array>
#include <map>

using namespace std;

CPassaport6Object::CPassaport6Object(CSimulator* simulator, int category, int id, string nom) :CSimulationObject(simulator, category, id, nom)
{
    setState(SERVICE);
}

std::piecewise_linear_distribution<double> triangular_distribution(double min, double peak, double max)
{
    std::array<double, 3> i{ min, peak, max };
    std::array<double, 3> w{ 0, 1, 0 };
    return std::piecewise_linear_distribution<double>{i.begin(), i.end(), w.begin()};
}

int triangular(int a, int b, int c) {
    std::random_device rd;
    // create a mersenne twister PRNG seeded from some implementation-defined random source
    std::mt19937 gen(rd());

    // create a triangular distribution with a minimum of 0, a peak at 20, and a maximum of 30
    auto dist = triangular_distribution(a, b, c);

    std::map<int, int> hist;

    // use our distribution to generate 10,000 random numbers
    // (truncated to integers for the sake of output; the generated numbers are actually real numbers)
    for (int i = 0; i < 10000; ++i) {
        double num = dist(gen);
        ++hist[num];
    }

    // print out a nice histogram of the numbers generated
    for (auto p : hist) {
        std::cout << std::setw(2) << std::setfill('0') << p.first << ' '
            << std::string(p.second / 10, '*') << '\n';
    }
}

//Métode que el simulador us invocarà per a recollir els estadístics (print per consola)
void CPassaport6Object::showStatistics() {
    cout << getName() << " No tinc estadístics \n";
};

//Processar un esdeveniment de simulació, funció pura que us toca implementar
void CPassaport6Object::processEvent(CSimulationEvent* event) {
    if (event->getEntity() == NULL)
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i no rebo entitat.";
    else
        std::cout << to_string(event->getTime()) + " soc " + getName() + " i  rebo entitat " + to_string(event->getEntity()->getId());

    float tempsEvent;
    if (event->getEventType() == ePUSH) {
        tempsEvent = rand() % 90 + event->getTime();
        CSimulationEvent* eventPush = new CSimulationEvent(tempsEvent, this, this, event->getEntity(), eSERVICE);
        m_Simulator->scheduleEvent(eventPush);
        std::cout << " i programo un event service per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
        setState(SERVICE);
    }
    int a = 120;
    if (event->getEventType() == eSERVICE) {
        if (m_category > 0)
        {
            std::list<struct__route> destins;
            destins = m_Simulator->nextObject(event->getEntity(), this);
            int atzar = rand() % destins.size();
            for (int i = 0; i < atzar; i++) {
                destins.pop_front();
            }
            struct__route candidat = destins.front();
            CSimulationEvent* eventService;
            if (candidat.destination == NULL) {
                std::cout << " i no se trobar el  meu següent destí" + to_string(event->getEntity()->getId()) + "\n";
                destins = m_Simulator->nextObject(event->getEntity(), this);
                setState(IDLE);
            }
            else {
                tempsEvent = candidat.time + m_Simulator->time();
                eventService = new CSimulationEvent(tempsEvent, this, candidat.destination, event->getEntity(), ePUSH);
                m_Simulator->scheduleEvent(eventService);
                std::cout << " i programo un event push per a l'entitat " + to_string(event->getEntity()->getId()) + "\n";
                setState(IDLE);
            }
        }
    }
}

//Métode que el simulador invocarà a l'inici de la simulació, abans de que hi hagi cap esdeveniment a la llista d'esdeveniments
void CPassaport6Object::simulationStart() {
    setState(IDLE);
    cout << getName() << " inicialitzat \n";
}
//Métode que el simulador us pot invocar a la finalització de l'estudi
void CPassaport6Object::simulationEnd() {
    setState(IDLE);
}