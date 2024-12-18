#include "Facility.h"
#include "Simulation.h"
//------------ FacilityType ----------
//Constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
: name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){}
    
//Methods
const string& FacilityType::getName() const {
    return name;
}

FacilityType* FacilityType::clone()
{
    return new FacilityType(*this);
}

int FacilityType::getCost() const{
    return price;
}

int FacilityType::getLifeQualityScore() const{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const{
    return environment_score;
}

int FacilityType::getEconomyScore() const{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const{
    return category;
}    

const string FacilityType::toString()
{
    std::string cur_category;
    if (category == FacilityCategory::LIFE_QUALITY){
        cur_category = "Life Quality";
    }

    else if(category == FacilityCategory::ECONOMY){
        cur_category = "Economy";
    }

    else{
        cur_category = "Environment";
    }

    return "Facility name: " + name +
           "\nCategory: " + cur_category +
            "\nPrice: " + std::to_string(price) +
           "\nLife Quality Score: " + std::to_string(lifeQuality_score) +
           "\nEconomy Score: " + std::to_string(economy_score) +
           "\nEnvironment Score: " + std::to_string(environment_score);
}

//---------- Facility ----------
//Default constructors
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
: FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}

Facility::Facility(const FacilityType &type, const string &settlementName)
: FacilityType(type.getName(), type.getCategory(), type.getCost(), type.getLifeQualityScore(), type.getEconomyScore(), type.getEnvironmentScore()), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()){}

//Copy constructor
Facility::Facility(const Facility &other)
: FacilityType(other.name, other.category, other.price, other.lifeQuality_score, other.economy_score, other.environment_score), settlementName(other.settlementName), status(other.status), timeLeft(other.price){}

//Methods

const string &Facility::getSettlementName() const{
return settlementName;
}

const int Facility::getTimeLeft() const{
return timeLeft;
}

void Facility::reduceTime(){
    if(timeLeft > 0){
        timeLeft--;
        if(timeLeft == 0){
            setStatus(FacilityStatus::OPERATIONAL);
        }
    }
}

void Facility::setStatus(FacilityStatus status){
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS){
        this->status = FacilityStatus::UNDER_CONSTRUCTIONS;
    }

    else{   //status == FacilityStatus::OPERATIONAL
        this->status = FacilityStatus::OPERATIONAL;
    }
}

const FacilityStatus& Facility::getStatus() const{
    return status;
}

const string Facility::toString() const{
    string stat = "BUSY";
    if (status == FacilityStatus::OPERATIONAL)
    {
        stat = "OPERATIONAL";
    }
    return "Facility name: " + name + "\n" +
           "Status: " + stat + "\n";
}

const string Facility::getStatusToString()
{
    if (getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS)
    {
        return "UNDER_CONSTRUCTIONS";
    }

    else
    {
        return "OPERATIONAL";
    }
}

Facility* Facility::clone()
{
    return new Facility(*this);
}