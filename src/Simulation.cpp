
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "Auxiliary.h"
#include "Action.h"

using std::string;
using std::vector;

using namespace std;

//--------------- Rule-of-5 ---------------

//Constructor
Simulation::Simulation(const string &configFilePath) :
    isRunning(false), planCounter(0), actionsLog(vector<BaseAction*>()), plans(vector<Plan>()), settlements(vector<Settlement*>()), facilitiesOptions(vector<FacilityType>())
{
    string curr_line;
    ifstream MyReadFile(configFilePath);
    while (getline(MyReadFile, curr_line))
    {
        vector<string> line_toVector = Auxiliary::parseArguments(curr_line);
        if (line_toVector.at(0) == "settlement")
        {
            const string name = line_toVector.at(1);
            SettlementType type;
            if (line_toVector.at(2) == "0")
                type = SettlementType::VILLAGE;

            else if (line_toVector.at(2) == "1")
                type = SettlementType::CITY;

            else //=="2"
                type = SettlementType::METROPOLIS;

            settlements.push_back(new Settlement(name, type));
        }
        else if (line_toVector.at(0) == "facility")
        {
            const string name = line_toVector.at(1);
            const string category = line_toVector.at(2);
            const int price = std::stoi(line_toVector.at(3));
            const int lq_score = std::stoi(line_toVector.at(4));
            const int eco_score = std::stoi(line_toVector.at(5));
            const int env_score = std::stoi(line_toVector.at(6));
            FacilityCategory cat;

            if (category == "0")
                cat = FacilityCategory::LIFE_QUALITY;

            else if (category =="1")
                cat = FacilityCategory::ECONOMY;

            else //=="2"
                cat = FacilityCategory::ENVIRONMENT;
            
            facilitiesOptions.push_back(FacilityType(name, cat, price, lq_score, eco_score, env_score));
        }
        else if (line_toVector.at(0) == "plan")
        {
            int index = -1;
            for (size_t i=0 ; i<settlements.size() ; i++)
            {
                if (settlements.at(i)->getName() == line_toVector.at(1))
                {
                    index = i;
                    break;
                }
            }
            const string plan_type = line_toVector.at(2);

            if (plan_type == "nve")
                plans.push_back(Plan(planCounter, *settlements.at(index), new NaiveSelection(), facilitiesOptions));

            else if (plan_type == "bal")
                plans.push_back(Plan(planCounter, *settlements.at(index), new BalancedSelection(0,0,0), facilitiesOptions));

            else if (plan_type == "eco")
                plans.push_back(Plan(planCounter, *settlements.at(index), new EconomySelection(), facilitiesOptions));

            else //=="env"
                plans.push_back(Plan(planCounter, *settlements.at(index), new SustainabilitySelection(), facilitiesOptions));

            planCounter++;

        }
    }
}

void Simulation::printFacilities()
{
    for (FacilityType f : facilitiesOptions)
    {
        cout << f.toString() +"\n \n";
    }
}

//Copy constructor
Simulation::Simulation(Simulation& other) :
    isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(other.actionsLog),
    plans(other.plans), settlements(other.settlements), facilitiesOptions(other.facilitiesOptions)
{
    for (BaseAction* ba : other.actionsLog)
    {
        actionsLog.push_back(ba->clone()); 
    }
    for (Settlement* s : other.settlements)
    {
        settlements.push_back(s->clone());
    }
}

//Move constructor
Simulation::Simulation(Simulation&& other) :
    isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(std::move(other.actionsLog)),
    plans(std::move(other.plans)), settlements(std::move(other.settlements)), facilitiesOptions(std::move(other.facilitiesOptions))
{
    for (BaseAction*& ba : other.actionsLog)
    {
        ba = nullptr;
    }
    for (Settlement*& s : other.settlements)
    {
        s = nullptr;
    }

    other.actionsLog.clear(); 
    other.settlements.clear(); 
    other.facilitiesOptions.clear();
    other.plans.clear();
}

//Desctructor
Simulation::~Simulation()
{
   for (BaseAction* ba : actionsLog)
    {
        delete ba;
    }
    for (Settlement* s : settlements)
    {
        delete s;
    }
    actionsLog.clear();
    plans.clear();
    settlements.clear();
    facilitiesOptions.clear();
}

//Copy assignment operator
Simulation& Simulation::operator=(Simulation& other)
{
    if (this != &other)
    {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans.clear();
        facilitiesOptions.clear();
        for (Plan p : other.plans){
            plans.push_back(p);
        }
        
        for (FacilityType ft : other.facilitiesOptions){
            facilitiesOptions.push_back(ft);
        }

        for (BaseAction* ba : actionsLog)
        {
            delete ba;
        }
        for (Settlement* s : settlements)
        {
            delete s;
        }
        actionsLog.clear();
        settlements.clear();

        for (BaseAction* ba : other.actionsLog)
        {
            actionsLog.push_back(ba->clone());
        }
        for (Settlement* s : other.settlements)
        {
            settlements.push_back(s->clone());
        }
        
    }
    return *this;
}

Simulation* Simulation::clone()
{
    return new Simulation(*this);
}

//Move assignment operator
Simulation& Simulation::operator=(const Simulation&& other)
{
    if (this != &other)
    {
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (Plan p : other.plans){
            plans.push_back(p);
        }

        for (FacilityType ft : other.facilitiesOptions){
            facilitiesOptions.push_back(ft);
        }   

        for (BaseAction* ba : actionsLog)
        {
            delete ba;
        }
        for (Settlement* s : settlements)
        {
            delete s;
        }
        actionsLog.clear();
        plans.clear();

        actionsLog = move(other.actionsLog);
        settlements = move(other.settlements);
        
    }
    return *this;
}

//--------------- Methods ---------------

void Simulation::start(){
    open();
    cout << "The simulation has started" << endl;

    while (isRunning){
        cout << "\n ** Enter an action: \n\n";
        string action;
        getline(cin, action);
        vector<string> split_action = Auxiliary::parseArguments(action);

        if (split_action.empty()) continue;

        string cur_act = split_action[0];
        if (cur_act == "step"){
            SimulateStep(stoi(split_action[1])).act(*this);
        }

        else if (cur_act == "plan"){
            AddPlan(split_action[1], split_action[2]).act(*this);
        }

        else if (cur_act == "settlement"){
            SettlementType set_type;

            if (split_action[2] == "0"){
                set_type = SettlementType::VILLAGE;
            }

            else if (split_action[2] == "1"){
                set_type = SettlementType::CITY;
            }

            else if (split_action[2] == "2") {
                set_type = SettlementType::METROPOLIS;
            }
            else
                set_type = SettlementType::NOTF;

            AddSettlement(split_action[1], set_type).act(*this);
        }

        else if (cur_act == "facility"){
            FacilityCategory fac_cat;

            if (split_action[2] == "0"){
                fac_cat = FacilityCategory::LIFE_QUALITY;
            }

            else if (split_action[2] == "1"){
                fac_cat = FacilityCategory::ECONOMY;
            }

            else{ // (split_action[2] == "2")
                fac_cat = FacilityCategory::ENVIRONMENT;
            }

            AddFacility(split_action[1], fac_cat, stoi(split_action[3]), stoi(split_action[4]), stoi(split_action[5]), stoi(split_action[6])).act(*this);
        }

        else if (cur_act == "planStatus"){
            PrintPlanStatus(stoi(split_action[1])).act(*this);
        }

        else if (cur_act == "changePolicy"){
            ChangePlanPolicy(stoi(split_action[1]), split_action[2]).act(*this);
        }

        else if (cur_act == "log"){
            PrintActionsLog().act(*this);
        }

        else if (cur_act == "close"){
            Close().act(*this); //isRunning = false
        }

        else if (cur_act == "backup"){
            BackupSimulation().act(*this);
        }

        else if (cur_act == "restore"){
            RestoreSimulation().act(*this);
        }

        else{
            cout << "Action not valid." << endl;
        }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction* action){
    actionsLog.push_back(action->clone());
}

Settlement& Simulation::findSettlement (string name){
    for (Settlement* s : settlements){
        if (s->getName() == name){
            Settlement& s_ref = *s;
            return s_ref;
        }
    }
    throw std::runtime_error("Settlement does not exist");
}

bool Simulation::addSettlement(Settlement* s){
    settlements.push_back(s->clone());
    return true;
}

bool Simulation::addFacility(FacilityType f){
    facilitiesOptions.push_back(f);
    return true;
}

bool Simulation::isSettlementExists(const string &name){
    bool find = false;
    for (Settlement* s : settlements){
        if (s->getName() == name)
            find = true;
    }
    return find;
}

bool Simulation::isFacilityExists(const string &name) {
    for (const FacilityType &fa : facilitiesOptions) {
        if (fa.getName() == name) {
            return true;
        }
    }
    return false;
}

const int Simulation::plans_size ()
{
    return plans.size();
}

Settlement& Simulation::getSettlement(const string &name){
    for (Settlement* s : settlements){
        if (s->getName() == name)
            return *s;
    }
    throw std::invalid_argument("Settlement doesn't exist");
}

Plan& Simulation::getPlan(const int id){
    return plans[id];
}

void Simulation::step(){
    for (Plan &p :plans)
        p.step();
}

void Simulation::open(){
    isRunning = true;
}

void Simulation::close(){
    for (Plan p : plans)
    {
        p.printPlan();
    }
    isRunning = false;
}

const vector<BaseAction*>& Simulation::getActionsLog(){
    return actionsLog;
}
