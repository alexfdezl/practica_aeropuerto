#pragma once
#include <string>

#include "simulationobject.h"

//Informació del temps que es triga a arribar a destí
struct struct__route{
    CSimulationObject* destination;
    float time;
    
    struct__route(CSimulationObject* desti, float temps){
        destination=desti;
        time=temps;
    }
};
