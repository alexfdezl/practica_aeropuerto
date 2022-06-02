#pragma once

class CSimulationObject;
class CSimulationEvent;
class CEntity
{
public:
    float m_creationTime;
    float m_departureTime;
    CEntity(){}
    //registra de quan l'entitat entra en el sistema
    CEntity(int id,float currentTime){
        m_id=id;
        m_creationTime=currentTime;
    }
    virtual ~CEntity(){}
    //registra de quan l'entitat surt del sistema
    void departureTime(float currentTime){
        m_departureTime=currentTime;
    }
    //recuperar el temps de vida
    float getlifeTime(){
        return m_departureTime-m_creationTime;
    }
    //Recupera la posició on es troba l'entitat
    CSimulationObject* getPlace(){
      return m_objectPlace;
    }
    //Actualitza la posició on es troba l'entitat
    void setPlace(CSimulationObject* receptor){
      m_objectFrom=m_objectPlace;  
      m_objectPlace=receptor;
    }
    //Retorna l'identificador únic d'on ve l'entitat
    CSimulationObject* getFrom(){
        return m_objectFrom;
    } 
    //Recupera l'identificador de l'entitat
    int getId(){return m_id;};
protected:
    //identificador únic
    int m_id;
    //Actualitza la traça d'esdeveniments on aquesta entitat s'ha vist implicat
    void traceEvent(CSimulationEvent* event);   
    //Identificador d'on prové l'entitat
    CSimulationObject* m_objectFrom;
    //Element de simulació on es troba l'entitat
    CSimulationObject* m_objectPlace;
};

