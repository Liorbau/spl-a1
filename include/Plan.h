#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement& settlement, SelectionPolicy* selectionPolicy, const vector<FacilityType>& facilityOptions);
        Plan(Plan& other); //Copy constuctor
        Plan(Plan&& other); //Move contructor
        ~Plan(); //Destructor
        Plan& operator= (Plan& other) = delete; //Unimplemented copy assignment operator
        Plan&& operator= (Plan&& other) = delete; //Unimplemented move assignment operator
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        Plan* clone();
        const string getSettlementName();
        const int getEnvironmentScore() const;
        const string getPolicy();
        void setSelectionPolicy(SelectionPolicy* selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*>& getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

    private:
        int plan_id;
        const Settlement& settlement;
        SelectionPolicy* selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType>& facilityOptions;
        int life_quality_score, economy_score, environment_score;
};