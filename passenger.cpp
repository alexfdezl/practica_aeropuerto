#include "passenger.h"
#include "entity.h"

using namespace std;

//Constructora per a passatgers que volen agafar un vol
CPassenger::CPassenger(int id, float currentTime) :CEntity(id, currentTime)
{
    init(currentTime, true);
    setSchenger(randomBool(80));
}

//Constructora per a passatgers que arriben amb un vol
CPassenger::CPassenger(int id, float currentTime, std::string idFlight) :CEntity(id, currentTime)
{
    init(currentTime, false);
    setSchenger(idFlight == "SCH");
}

void CPassenger::init(float currentTime, bool outBCN) {
    m_timeFlight = randomInt(300) + currentTime;
    m_checkin = randomBool(30);
    m_baggage = randomBool(50);
    m_PMR = randomBool(5);
    m_takeFlight = outBCN;
    m_LostFlight = false;
}
void CPassenger::setSchenger(bool schengen) {
    m_Schengen = schengen;
    if (m_Schengen) {
        m_idFinger = randomInt(16);
        m_Domestic = randomBool(5);
    }
    else {
        m_idFinger = 10 + randomInt(2);
        m_Domestic = false;
    }
}
int CPassenger::getNumberFinger() {
    return this->m_idFinger;
}
int CPassenger::getDepartureTime() {
    return this->m_timeFlight;
}
bool CPassenger::carryBaggage() {
    return this->m_baggage;
}
bool CPassenger::checkinDone() {
    return this->m_checkin;
}
bool CPassenger::isPMR() {
    return this->m_PMR;
}
bool CPassenger::takeFlight() {
    return m_takeFlight;
}
bool CPassenger::isSchengen() {
    return m_Schengen;
}
bool CPassenger::isDomestic() {
    return m_Domestic;
}
bool CPassenger::randomBool(int limit) {
    //No agafeu aquesta funció com a referència penseu com han de ser el generador de nombres aleatoris
    srand(time(0));
    return (rand() % 100 + 1 < limit);
}
int CPassenger::randomInt(int limit) {
    //No agafeu aquesta funció com a referència penseu com han de ser el generador de nombres aleatoris
    srand(time(0));
    return rand() % limit + 1;
}
