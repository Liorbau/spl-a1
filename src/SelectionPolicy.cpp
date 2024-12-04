#include "SelectionPolicy.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

//----------Naive selection policy----------

//Constructor
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1){}

//Copy constructor
NaiveSelection::NaiveSelection(const NaiveSelection &other) : 
    lastSelectedIndex(other.lastSelectedIndex) {}

//Methods
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    if(facilitiesOptions.empty()){
        throw std::runtime_error("No facility options");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions.at(lastSelectedIndex);
}

const string NaiveSelection::toString() const{
    return "Naive Selection Policy selected, last selected index is: " + std::to_string(lastSelectedIndex);
} 

NaiveSelection* NaiveSelection::clone() const{
    return new NaiveSelection(*this);
}

string NaiveSelection::getType()
{
    return "nve";
}

//----------Balanced Selection Policy----------

//Constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
: LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

//Copy constructor
BalancedSelection::BalancedSelection(const BalancedSelection &other) : 
    LifeQualityScore(other.LifeQualityScore), EconomyScore(other.EconomyScore), EnvironmentScore(other.EnvironmentScore) {}

//Methods
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    if(facilitiesOptions.empty()){
        throw std::runtime_error("No facility options");
    }

    int cur_lq = facilitiesOptions[0].getLifeQualityScore() + LifeQualityScore;
    int cur_eco = facilitiesOptions[0].getEconomyScore() + EconomyScore;
    int cur_env = facilitiesOptions[0].getEnvironmentScore() + EnvironmentScore;
    
    int min_diff = std::max({cur_lq, cur_eco, cur_env}) - std::min({cur_lq, cur_eco, cur_env});
    int selected_fac_index = 0;

    for(int i = 1; i < facilitiesOptions.size(); i++){
        cur_lq = std::abs(facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore);
        cur_eco = std::abs(facilitiesOptions[i].getEconomyScore() + EconomyScore);
        cur_env = std::abs(facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore);

        if(std::max({cur_lq, cur_eco, cur_env}) - std::min({cur_lq, cur_eco, cur_env}) < min_diff){
            min_diff = std::max({cur_lq, cur_eco, cur_env}) - std::min({cur_lq, cur_eco, cur_env});
            selected_fac_index = i;
        }
    }
    return facilitiesOptions[selected_fac_index];
}

const string BalancedSelection::toString() const{
    return "Balanced Selection Policy selected";
} 

BalancedSelection* BalancedSelection::clone() const{
    return new BalancedSelection(*this);
}

string BalancedSelection::getType()
{
    return "bal";
}

//----------Economy Selection Policy----------

//Constructors
EconomySelection::EconomySelection() : lastSelectedIndex(0){}

//Copy constructor
EconomySelection::EconomySelection(const EconomySelection &other) :
    lastSelectedIndex(other.lastSelectedIndex) {}

//Methods
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    if(facilitiesOptions.empty()){
        throw std::runtime_error("No facility options");
    }

    FacilityCategory cur_category = facilitiesOptions.at(lastSelectedIndex).getCategory();

    while(cur_category != FacilityCategory::ECONOMY){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
        cur_category = facilitiesOptions.at(lastSelectedIndex).getCategory();
    }

    return facilitiesOptions.at(lastSelectedIndex);
}

const string EconomySelection::toString() const{
    return "Economy Selection Policy selected, last selected index is: " + std::to_string(lastSelectedIndex);
}

EconomySelection* EconomySelection::clone() const{
    return new EconomySelection(*this);
}

string EconomySelection::getType()
{
    return "eco";
}
//----------Sustainability Selection Policy----------

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(0) {}

//Copy constructor
SustainabilitySelection::SustainabilitySelection(const SustainabilitySelection &other) :
    lastSelectedIndex(other.lastSelectedIndex) {}

//Methods
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    if(facilitiesOptions.empty()){
        throw std::runtime_error("No facility options");
    }

    FacilityCategory cur_category = facilitiesOptions.at(lastSelectedIndex).getCategory();

    while(cur_category != FacilityCategory::ENVIRONMENT){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
        cur_category = facilitiesOptions.at(lastSelectedIndex).getCategory();
    }

    return facilitiesOptions.at(lastSelectedIndex);
}

const string SustainabilitySelection::toString() const{
    return "Sustainability Selection Policy selected, last selected index is: " + std::to_string(lastSelectedIndex);
}

SustainabilitySelection* SustainabilitySelection::clone() const{
    return new SustainabilitySelection(*this);
}
string SustainabilitySelection::getType()
{
    return "env";
}