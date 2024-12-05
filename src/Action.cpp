#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Action.h"
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "Auxiliary.h"

using std::string;
using std::vector;

using namespace std;



//----------Base Action----------

// Constructor
BaseAction::BaseAction() :
    errorMsg(""), status(ActionStatus::ERROR) {}

ActionStatus BaseAction::getStatus() const{
    return status;
}

//Methods
void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string error){
    errorMsg = error;
    status = ActionStatus::ERROR;
}

const string& BaseAction::getErrorMsg() const{
    return errorMsg;
}



//----------Simulate Step----------
SimulateStep::SimulateStep(const int num) : BaseAction(), numOfSteps(num) {}

void SimulateStep::act(Simulation &s){
    for (int i=0 ; i<numOfSteps ; i++){
        s.step();
    }
    complete();
}

const string SimulateStep::toString() const{
    string status = "";
    if (getStatus() == ActionStatus::COMPLETED)
        status = " COMPLETED";
    else
        status = " ERROR";
    return "step "+std::to_string(numOfSteps)+status+"\n";
}

SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
}



//----------Add Plan----------

//Constructor
AddPlan::AddPlan(const string&name, const string& SelectionPolicy) : BaseAction(), settlementName(name), selectionPolicy(selectionPolicy) {}

//Methods
void AddPlan::act(Simulation& s){
    if (s.isSettlementExists(settlementName)){
        if (selectionPolicy == "nve"){
            s.addPlan((s.findSettlement(settlementName)), new NaiveSelection());
            complete();
        }

        else if (selectionPolicy == "bal"){
            s.addPlan((s.findSettlement(settlementName)), new BalancedSelection(0,0,0));
            complete();
        }

        else if (selectionPolicy == "eco"){
            s.addPlan((s.findSettlement(settlementName)), new EconomySelection());
            complete();
        }

        else if (selectionPolicy == "env"){
            s.addPlan((s.findSettlement(settlementName)), new SustainabilitySelection());
            complete();
        }

        else{
            error("Cannot create this plan");
        }
    }

    else{
            error("Cannot create this plan");
    }
}

const string AddPlan::toString() const{
    string status = "";
    if (getStatus() == ActionStatus::COMPLETED)
        status = " COMPLETED";
    else
        status = " ERROR";
    return "plan "+settlementName+" "+selectionPolicy+status+"\n";
}

AddPlan* AddPlan::clone() const{
    return new AddPlan(*this);
}



//----------Add Settlement----------

//Constructor
AddSettlement::AddSettlement(const string &name,SettlementType type) : BaseAction(), settlementName(name), settlementType(type) {}

//Methods
void AddSettlement::act(Simulation& s){
    if (s.isSettlementExists(settlementName)){
        s.addSettlement(new Settlement(settlementName, settlementType));
        complete();
    }

    else{
            error("Settlement already exists");
    }
}

const string AddSettlement::toString() const{
    string status = "";
    if (getStatus() == ActionStatus::COMPLETED)
        status = " COMPLETED";

    else
        status = " ERROR";

    string type_toString = "";
    if (settlementType == SettlementType::CITY)
        type_toString = "CITY";

    else if (settlementType == SettlementType::METROPOLIS)
        type_toString = "METROPOLIS";

    else //VILLAGE
        type_toString = "VILLAGE";

    return "settlement "+settlementName+" "+type_toString+" "+status+"\n";
}

AddSettlement* AddSettlement::clone() const{
    return new AddSettlement(*this);
}

//----------Add Facility----------

//Constructor
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
: BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore){}

//Methods
void AddFacility::act(Simulation &simulation){
    if(!simulation.isFacilityExists(facilityName)){
        simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
        complete();
    }

    else{
        error("Facility already exists");
    }
}

const string AddFacility::toString() const{
    string status = "";
    if (getStatus() == ActionStatus::COMPLETED)
        status = " COMPLETED";

    else
        status = " ERROR";

    string cat = "";
    if (facilityCategory == FacilityCategory::ECONOMY)
        cat = "1";

    else if (facilityCategory == FacilityCategory::ENVIRONMENT)
        cat = "2";

    else //Life quality
        cat = "0";

    return "facility "+facilityName+" "+cat+" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+"\n";
}

AddFacility* AddFacility::clone() const{
    return new AddFacility(*this);
}



//---------- PrintPlanStatus ----------

//Constructor
PrintPlanStatus::PrintPlanStatus(int planId): BaseAction(), planId(planId){}

//Methods
void PrintPlanStatus::act(Simulation &s){
    if(planId > s.plans_size() || planId < 0){
        error("Plan doesn't exist");
    }

    else{
        cout << "PlanID: " + std::to_string(planId) +"\n";
        cout << "SettlementName: " + s.getPlan(planId).getSettlementName() +"\n";
        s.getPlan(planId).printStatus();
        cout << "SelectionPolicy: "+ s.getPlan(planId).getPolicy() +"\n";
        cout << "LifeQuality_score:" + to_string(s.getPlan(planId).getlifeQualityScore()) +"\n";
        cout << "EconomyScore _score:" + to_string(s.getPlan(planId).getEconomyScore()) +"\n";
        cout << "EnvrionmentScore _score:" + to_string(s.getPlan(planId).getEnvironmentScore()) +"\n";
        s.getPlan(planId).printFacilities();

    }
}

const string PrintPlanStatus::toString() const{
    string act = "Action: Print Plan Status";
    string stat = "";
    if (getStatus() == ActionStatus::COMPLETED)
        stat = "COMPLETED";

    else
        stat = "ERROR";

    stat = "Status: "+stat;
    if (stat == "ERROR")
        return act + "\n" + stat + "\n" + "Error: "+getErrorMsg(); 
    else

       return act + "\n" + stat; 

}

PrintPlanStatus* PrintPlanStatus::clone() const{
    return new PrintPlanStatus(*this);
}



//----------ChangePlanPolicy----------

//Constructor
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): BaseAction(), planId(planId), newPolicy(newPolicy){}

//Methods
void ChangePlanPolicy::act(Simulation &s){
    if(s.getPlan(planId).getPolicy() == newPolicy){
        error("Cannot change selection policy");
    }

    else{
        if(newPolicy == "nve"){
            cout << "planID: " + to_string(planId) +"\n";
            cout << "previousPolicy: "+ s.getPlan(planId).getPolicy()+ "\n";
            cout << "newPolicy: nve \n";
            s.getPlan(planId).setSelectionPolicy(new NaiveSelection());
        } 

        else if(newPolicy == "eco"){
            cout << "planID: " + to_string(planId) +"\n";
            cout << "previousPolicy: "+ s.getPlan(planId).getPolicy()+ "\n";
            cout << "newPolicy: eco \n";
            s.getPlan(planId).setSelectionPolicy(new EconomySelection());
        }

        else if(newPolicy == "env"){
            cout << "planID: " + to_string(planId) +"\n";
            cout << "previousPolicy: "+ s.getPlan(planId).getPolicy()+ "\n";
            cout << "newPolicy: env \n";
            s.getPlan(planId).setSelectionPolicy(new SustainabilitySelection());
        }

        else if(newPolicy == "bal"){
            cout << "planID: " + to_string(planId) +"\n";
            cout << "previousPolicy: "+ s.getPlan(planId).getPolicy()+ "\n";
            cout << "newPolicy: bal \n";
            s.getPlan(planId).setSelectionPolicy(new BalancedSelection(0, 0, 0));
        }

        else{
            error("Cannot change selection policy");
        }
    }
}

const string ChangePlanPolicy::toString() const{
    string act = "Action: Change Plan Policy";
    string stat = "";
    if (getStatus() == ActionStatus::COMPLETED)
        stat = "COMPLETED";

    else
        stat = "ERROR";

    stat = "Status: "+stat;
    if (stat == "ERROR")
        return act + "\n" + stat + "\n" + "Error: "+getErrorMsg(); 

    else
       return act + "\n" + stat; 

}

ChangePlanPolicy* ChangePlanPolicy::clone() const{
    return new ChangePlanPolicy(*this);
}



// ---------- PrintActionsLog ----------

//Constructor
PrintActionsLog::PrintActionsLog() : BaseAction() {}


//Methods
void PrintActionsLog::act(Simulation& s){
    for (BaseAction* a : s.getActionsLog()){
        cout << a->toString() +"\n";
    }
}

const string PrintActionsLog::toString() const{
    string act = "Action: Print actions log";
    string stat = "";

    if (getStatus() == ActionStatus::COMPLETED)
        stat = "COMPLETED";

    else
        stat = "ERROR";
    stat = "Status: "+stat;

    if (stat == "ERROR")
        return act + "\n" + stat + "\n" + "Error: "+getErrorMsg(); 

    else
       return act + "\n" + stat; 
}

PrintActionsLog* PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}



// -----------Close------------

//Constructor
Close::Close() : BaseAction() {}


//Methods
void Close::act(Simulation& s){
    s.close();
}

Close* Close::clone() const{
    return new Close(*this);
}

const string Close::toString() const{
    return "Simulation closed.";
}

/*

class Close : public BaseAction {
    public:
        Close();
        void act(Simulation &simulation) override;
        Close *clone() const override;
        const string toString() const override;
    private:


};*/

