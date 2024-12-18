#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"

using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(Simulation& other); //Copy constuctor
        Simulation(Simulation&& other); //Move contructor
        ~Simulation(); //Destructor
        Simulation& operator=(Simulation& other); //Copy assignment operator //TODO
        Simulation& operator=(const Simulation&& other); //Move assignment operator //TODO
        void start();
        Simulation* clone();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement& findSettlement (string name); //Helper method
        bool isFacilityExists (const string &name); //Helper method
        const int plans_size ();
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        const vector<BaseAction*>& getActionsLog();
        const vector<Plan>& getPlans();
        void printFacilities();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};