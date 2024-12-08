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

extern Simulation* backup;

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
    s.addAction(this);
}

const string SimulateStep::toString() const{
    return "step "+std::to_string(numOfSteps)+" COMPLETED";
}

SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
}



//----------Add Plan----------

//Constructor
AddPlan::AddPlan(const string&name, const string& SelectionPolicy) : BaseAction(), settlementName(name), selectionPolicy(SelectionPolicy) {}

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
            cout << getErrorMsg() +"\n";
        }
    }

    else{
            error("Cannot create this plan");
            cout << getErrorMsg() +"\n";

    }
    s.addAction(this);

}

const string AddPlan::toString() const{
    string s = "ERROR";
    if (getStatus() == ActionStatus::COMPLETED)
        return "plan "+settlementName+" "+selectionPolicy+" COMPLETED";
    return "plan "+settlementName+" "+selectionPolicy+" ERROR "+getErrorMsg();
}

AddPlan* AddPlan::clone() const{
    return new AddPlan(*this);
}



//----------Add Settlement----------

//Constructor
AddSettlement::AddSettlement(const string &name,SettlementType type) : BaseAction(), settlementName(name), settlementType(type) {}

//Methods
void AddSettlement::act(Simulation& s){
    if (!(settlementType == SettlementType::NOTF))
    {
        if (!s.isSettlementExists(settlementName)){
            s.addSettlement(new Settlement(settlementName, settlementType));
            complete();
        }

        else{
                error("Settlement already exists");
                cout << getErrorMsg() +"\n";

        }
    }
    else
    {
        error("Settlement type invalid");
        cout << getErrorMsg() +"\n";
    }
    s.addAction(this);
}

const string AddSettlement::toString() const{
    string type = "-1";
    if (settlementType == SettlementType::VILLAGE)
        type = "0";
    if (settlementType == SettlementType::CITY)
        type = "1";
    else if (settlementType == SettlementType::METROPOLIS)
        type = "2";
    if (getStatus() == ActionStatus::COMPLETED)
        return "settlement "+settlementName+" "+type+ " COMPLETED";
    return "settlement "+settlementName+" "+type+ " ERROR "+getErrorMsg();
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
        cout << getErrorMsg() +"\n";

    }
    simulation.addAction(this);
}

const string AddFacility::toString() const{
    string category = "-1";
    if (facilityCategory == FacilityCategory::LIFE_QUALITY)
        category = "0";
    else if (facilityCategory == FacilityCategory::ECONOMY)
        category = "1";
    else if (facilityCategory == FacilityCategory::ENVIRONMENT)
        category = "2";
    if (getStatus() == ActionStatus::ERROR)
        return "facility "+facilityName+" "+category+" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+ "ERROR" + getErrorMsg();

    return "facility "+facilityName+" "+category+" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore)+ "COMPLETED";
}

AddFacility* AddFacility::clone() const{
    return new AddFacility(*this);
}



//---------- PrintPlanStatus ----------

//Constructor
PrintPlanStatus::PrintPlanStatus(int planId): BaseAction(), planId(planId){}

//Methods
void PrintPlanStatus::act(Simulation &s){
    if(planId >= s.plans_size() || planId < 0){
        error("Plan doesn't exist");
        cout << getErrorMsg() +"\n";

    }

    else{
        s.getPlan(planId).printPlan();
        complete();

    }
    s.addAction(this);
}

const string PrintPlanStatus::toString() const{
    if (getStatus() == ActionStatus::ERROR)
        return "planStatus "+std::to_string(planId)+" ERROR "+getErrorMsg();
    return "planStatus "+std::to_string(planId)+" COMPLETED";
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
        error("Same policy");
        cout << getErrorMsg() +"\n";

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
            cout << getErrorMsg() +"\n";

        }
    }
    s.addAction(this);
}

const string ChangePlanPolicy::toString() const{
    if (getStatus() == ActionStatus::ERROR)
        return "changePolicy "+std::to_string(planId)+" "+newPolicy+ "ERROR "+getErrorMsg();

    return "changePolicy "+std::to_string(planId)+" "+newPolicy+ "COMPLETED";

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
    return "log";
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
    cout << "Simulation closed \n";
}

Close* Close::clone() const{
    return new Close(*this);
}

const string Close::toString() const{
    return "Simulation closed";
}



// ---------- BackupSimulation ----------

BackupSimulation::BackupSimulation() : BaseAction() {}

void BackupSimulation::act(Simulation& s)
{
    if (backup == nullptr)
    {
        delete backup;
    }
    backup = new Simulation(s);
    cout << "Backup Successfull \n";
    s.addAction(this);
}

BackupSimulation* BackupSimulation::clone() const{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const{
    return "Backup Successfull";
}



// ---------- RestoreSimulation ----------

RestoreSimulation::RestoreSimulation() : BaseAction() {}

void RestoreSimulation::act(Simulation& s)
{
    if (backup == nullptr)
    {
        error("No backup available");
        cout << getErrorMsg() + "\n";
    }
    else
    {
        s = *backup;
    }
    s.addAction(this);
}

RestoreSimulation* RestoreSimulation::clone() const{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const{
    return "Restore Successfull.";
}


