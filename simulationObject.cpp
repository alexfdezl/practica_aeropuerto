#include "simulator.h"
#include "simulationobject.h"

void CSimulationObject::setState(enumStateObject estat)
{
    if (estat!=m_state){
        m_state=estat;
        m_Simulator->tracing(this);
    }
}