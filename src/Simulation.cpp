#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "Auxiliary.h"

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
        string type_toString = line_toVector.at(0);
        if (type_toString == "settlement")
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
        else if (type_toString == "facility")
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
            
            facilitiesOptions.emplace_back(new FacilityType(name, cat, price, lq_score, eco_score, env_score));
        }
        else //=="plan"
        {
            Settlement* s;
            for (int i=0 ; i<settlements.size() ; i++)
            {
                if (settlements.at(i)->getName() == line_toVector.at(1))
                {
                    s = settlements.at(i);
                    break;
                }
            }
            const string plan_type = line_toVector.at(2);

            if (plan_type == "nve")
                plans.emplace_back(new Plan(planCounter, *s, new NaiveSelection(), facilitiesOptions));

            else if (plan_type == "bal")
                plans.emplace_back(new Plan(planCounter, *s, new BalancedSelection(0,0,0), facilitiesOptions));

            else if (plan_type == "eco")
                plans.emplace_back(new Plan(planCounter, *s, new EconomySelection(), facilitiesOptions));

            else //=="env"
                plans.emplace_back(new Plan(planCounter, *s, new SustainabilitySelection(), facilitiesOptions));

            planCounter++;

        }
    }
}

//Copy constructor
Simulation::Simulation(Simulation& other) :
    isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(other.actionsLog),
    plans(other.plans), settlements(other.settlements), facilitiesOptions(other.facilitiesOptions)
{
    for (BaseAction* ba : other.actionsLog)
    {
        //actionsLog.push_back(ba->clone()); //TODO
    }
    for (Plan p : other.plans)
    {
        plans.emplace_back(p.clone());
    }
    for (Settlement* s : other.settlements)
    {
        settlements.push_back(s->clone());
    }
    for (FacilityType f : other.facilitiesOptions)
    {
        facilitiesOptions.emplace_back(f.clone());
    }
}

//Move constructor
Simulation::Simulation(Simulation&& other) :
    isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(other.actionsLog),
    plans(other.plans), settlements(other.settlements), facilitiesOptions(other.facilitiesOptions)
{
    for (BaseAction* ba : other.actionsLog)
    {
        ba = nullptr;
    }
    for (Plan p : other.plans)
    {
        //TODO
    }
    for (Settlement* s : other.settlements)
    {
        s = nullptr;
    }
    for (FacilityType f : other.facilitiesOptions)
    {
        //TODO
    }
}

//Desctructor
Simulation::~Simulation()
{
   for (BaseAction* ba : actionsLog)
    {
        delete ba;
    }
    for (Plan p : plans)
    {
        //TODO
    }
    for (Settlement* s : settlements)
    {
        delete s;
    }
    for (FacilityType f : facilitiesOptions)
    {
        //TODO
    }
    actionsLog.clear();
    plans.clear();
    settlements.clear();
    facilitiesOptions.clear();
}

//Copy assignment operator
Simulation& Simulation::operator=(const Simulation& other)
{
    //TODO
}

//Move assignment operator
Simulation&& Simulation::operator=(const Simulation&& other)
{
    //TODO
}

//--------------- Methods ---------------

void Simulation::start()
{
    isRunning = true;
    cout << "The simulation has started" << endl;
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.emplace_back(new Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
}

void Simulation::addAction(BaseAction* action)
{
    actionsLog.push_back(action);
}

Settlement& Simulation::findSettlement (string name)
{
    for (Settlement* s : settlements)
    {
        if (s->getName() == name)
        {
            Settlement& s_ref = *s;
            return s_ref;
        }
    }
}


bool Simulation::addSettlement(Settlement* s)
{
    settlements.push_back(s);
    return true;
}

bool Simulation::addFacility(FacilityType f)
{
    facilitiesOptions.emplace_back(f);
    return true;
}

bool Simulation::isSettlementExists(const string &name)
{
    bool find = false;
    for (Settlement* s : settlements)
    {
        if (s->getName() == name)
            find = true;
    }
    return find;
}

bool Simulation::isFacilityExists(string name)
{
    bool find = false;
    for (FacilityType fa : facilitiesOptions)
    {
        if (fa.getName() == name)
        {
            find = true;
            break;
        }
    }
    return find;
}

const int Simulation::plans_size ()
{
    return plans.size();
}


Settlement& Simulation::getSettlement(const string &name)
{
    for (Settlement* s : settlements)
    {
        if (s->getName() == name)
            return *s;
    }
    throw std::invalid_argument("Settlement doesn't exist");
}

Plan& Simulation::getPlan(const int id)
{
    return plans.at(id);
}

void Simulation::step()
{
    for (Plan p :plans)
        p.step();
}

void Simulation::open()
{
    isRunning = true;
}

void Simulation::close()
{
    isRunning = false;
}
