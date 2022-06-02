#pragma once


#include <iostream>
#include <string.h>
#include "entity.h"

class CPassenger:public CEntity{
    public:
        //Constructora per a passatgers que volen agafar un vol
        CPassenger(int id,float currentTime);
        //Constructora per a passatgers que arriben amb un vol
        CPassenger(int id,float currentTime,std::string idFlight);
        //Destructora
        ~CPassenger(){};
        //Aconsegueix el finger on hauria d'embarcar
        int getNumberFinger();
        //Aconsegueix el temps en que surt el vol
        int getDepartureTime();
        //Retorna un booleà per indicar si cal processar esdeveniments associats amb l'equipatge
        bool carryBaggage();
        //Retorna un booleà indicant si ha ja realitzat el checkin
        bool checkinDone();
        //Retorna un booleà per indicar si és una PMR
        bool isPMR();
        //Retorna un booleà per indicar si el passatger surt de Barcelona, agafa avió
        bool takeFlight();
        //Retorna un booleà per indicar si el passatger es de l'espai Schengen
        bool isSchengen();  
         //Retorna un booleà per indicar si el passatger agafa un vol domèstic
        bool isDomestic();        
        //Perd l'avió        
        void lostFlight(){
            m_takeFlight=false;
        }
        //Marca al passatger com una entitat que ha realitzat el checkin
        void setCheckin(){
            m_checkin=true;
        }
    protected:
        //identificador únic
        int m_id;
        //Inicialització dels atributs del passatger
        void init(float currentTime,bool takeflight);
        //Estableix si el passatger és schengen i el finger destí
        void setSchenger(bool schengen);
        //valor booleà en funció d'una probabilitat acumulada
        bool randomBool(int limit);
        //valor enter en funció d'un valor màxim
        int randomInt(int limit);
        //Identificador de finger destí o origen del passatger
        int m_idFinger;
        //Hora de sortida
        int m_timeFlight;
        //Ha realitzat checkin
        bool m_checkin;
        //Porta equipatge
        bool m_baggage;
        //És una persona amb mobilitat reduïda
        bool m_PMR;
        //És un passatger que surt de BCN en direcció a alguna part del mòn
        bool m_takeFlight;
        //És un passatger de l'espai Schengen
        bool m_Schengen;
        //És un passatger que realitza un vol Nacional
        bool m_Domestic;
};
