#include "Facility.h"

    //Constructor
    FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){}
    
    //Copy constructor
    FacilityType::FacilityType(const FacilityType &other)
    : name(other.name), category(other.category), price(other.price), lifeQuality_score(other.lifeQuality_score), economy_score(other.economy_score), environment_score(other.environment_score) {}

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

