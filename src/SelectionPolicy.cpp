#include "SelectionPolicy.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

//Destructor
SelectionPolicy::~SelectionPolicy() = default;


//----------Naive selection policy----------

//Constructor
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1){}

//Methods
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    if(facilitiesOptions.empty()){
        throw std::runtime_error("No facility options");
    }

    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const{
    return "Naive Selection Policy, last selected index is: " + lastSelectedIndex;
} 

NaiveSelection* NaiveSelection::clone() const{
    return new NaiveSelection(*this);
}

//----------Balanced Selection Policy----------

//Constructors
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
: LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

//Methods
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int minDistance = -1; // נעדכן שיהיה מצב נוכחי
    for(int i = 0; i < facilitiesOptions.size(); i++){

        int cur_lifeQuality = facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore;
        int cur_economy = facilitiesOptions[i].getEconomyScore() + EconomyScore;
        int cur_environment = facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore;

        int min = std::min({cur_lifeQuality, cur_economy, cur_environment});
        int max = std::max({cur_lifeQuality, cur_economy, cur_environment});

        if(max - min < minDistance){
            
        }
    }
}





 /*public:
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore; */