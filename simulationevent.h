#pragma once
#include "simulationobject.h"


class CEntity;

enum enumEventType { ePUSH=0,eSERVICE=1};
        
class CSimulationEvent{
    public:
        CSimulationEvent(){};
        CSimulationEvent(float time,CSimulationObject* provider,CSimulationObject* consumer,CEntity* entity,enumEventType eventType);
        ~CSimulationEvent(){};
        float getTime(){return m_eventTime;};
        CSimulationObject* getProvider(){return m_provider;};
        CSimulationObject* getConsumer(){return m_consumer;};
        CEntity* getEntity(){return m_entity;};
        enumEventType getEventType(){return m_eventType;};
        void executed();
        void isUnexpected();
    protected:
        CSimulationObject* m_provider;
        CSimulationObject* m_consumer;
        CEntity* m_entity;
        enumEventType m_eventType;
        float m_eventTime;
        float m_schedulledTime;
};


