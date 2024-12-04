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



//---------- Base Action ----------

BaseAction::BaseAction() :
    errorMsg(""), status(ActionStatus::ERROR) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string error)
{
    errorMsg = error;
    status = ActionStatus::ERROR;
}

const string& BaseAction::getErrorMsg() const
{
    return errorMsg;
}



//---------- Simulate Step ----------

SimulateStep::SimulateStep(const int num) : BaseAction(), numOfSteps(num) {}

void SimulateStep::act(Simulation &s)
{
    for (int i=0 ; i<numOfSteps ; i++)
    {
        s.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    string act = "Action: step";
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

SimulateStep* SimulateStep::clone() const
{
    return new SimulateStep(*this);
}



//---------- Add Plan ----------

AddPlan::AddPlan(const string&name, const string& SelectionPolicy) : BaseAction(), settlementName(name), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation& s)
{
    if (s.isSettlementExists(settlementName))
    {
        if (selectionPolicy == "nve")
        {
            s.addPlan((s.findSettlement(settlementName)), new NaiveSelection());
            complete();
        }
        else if (selectionPolicy == "bal")
        {
            s.addPlan((s.findSettlement(settlementName)), new BalancedSelection(0,0,0));
            complete();
        }
        else if (selectionPolicy == "eco")
        {
            s.addPlan((s.findSettlement(settlementName)), new EconomySelection());
            complete();
        }
        else if (selectionPolicy == "env")
        {
            s.addPlan((s.findSettlement(settlementName)), new SustainabilitySelection());
            complete();
        }
        else
        {
            error("Cannot create this plan");
        }
    }
    else
    {
            error("Cannot create this plan");
    }
}

const string AddPlan::toString() const
{
    string act = "Action: Add Plan";
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

AddPlan* AddPlan::clone() const
{
    return new AddPlan(*this);
}



//---------- Add Settlement ----------

AddSettlement::AddSettlement(const string &name,SettlementType type) : BaseAction(), settlementName(name), settlementType(type) {}

void AddSettlement::act(Simulation& s)
{
    if (s.isSettlementExists(settlementName))
    {
        s.addSettlement(new Settlement(settlementName, settlementType));
        complete();
    }
    else
    {
            error("Settlement already exists");
    }
}

const string AddSettlement::toString() const
{
    string act = "Action: Add Settlement";
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

AddSettlement* AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

//---------- Add Facility ----------

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
: BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore){}

void AddFacility::act(Simulation &simulation)
{
    if(!simulation.isFacilityExists(facilityName)){
        simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
        complete();
    }

    else{
        error("Facility already exists");
    }
}

const string AddFacility::toString() const
{
    string act = "Action: Add Facility";
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

AddFacility* AddFacility::clone() const
{
    return new AddFacility(*this);
}



//---------- PrintPlanStatus ----------

//Constructor
PrintPlanStatus::PrintPlanStatus(int planId): BaseAction(), planId(planId){}

//Methods
void PrintPlanStatus::act(Simulation &s)
{
    if(planId > s.plans_size() || planId < 0)
    {
        error("Plan doesn't exist");
    }

    else
    {
        cout << "PlanID: " + std::to_string(planId) << endl;
        cout << "SettlementName: " + s.getPlan(planId).getSettlementName() << endl;
        s.getPlan(planId).printStatus();
        cout << "SelectionPolicy: "+ s.getPlan(planId).getPolicy() << endl;
    }
}




/* class PrintPlanStatus: public BaseAction {
    public:
        PrintPlanStatus(int planId);
        void act(Simulation &simulation) override;
        PrintPlanStatus *clone() const override;
        const string toString() const override;
    private:
        const int planId;
};

;*/
