#include "Simulation.h"
#include "Plan.h"
#include <iostream>

//Default constructor
Simulation::Simulation(const string &configFilePath)
: isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions(){} 

//Methods
void Simulation::start(){
    std::cout << "The simulation has started" << std::endl;
    isRunning = true;
}

//public:
        ///void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        //void addAction(BaseAction *action);
        //bool addSettlement(Settlement *settlement);
        //bool addFacility(FacilityType facility);
        //bool isSettlementExists(const string &settlementName);
        //Settlement &getSettlement(const string &settlementName);
        //Plan &getPlan(const int planID);
        //void step();
        //void close();
        //void open();
        
        
        //private:
        //bool isRunning;
        //int planCounter; //For assigning unique plan IDs
        //vector<BaseAction*> actionsLog;
        //vector<Plan> plans;
        //vector<Settlement*> settlements;
        //vector<FacilityType> facilitiesOptions;