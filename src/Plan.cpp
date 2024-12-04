#include "Plan.h"
#include "Facility.h"
#include "vector"
#include <iostream>

//---------------- Rule of 5 ----------------

//Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) :
   plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
   status(PlanStatus::AVALIABLE), life_quality_score(0), economy_score(0), environment_score(0),
   facilities(vector <Facility*>()), underConstruction(vector <Facility*>()) {}

//Copy constuctor
Plan::Plan(Plan &other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()),
   status(other.status), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score),
   facilities(other.facilities), facilityOptions(other.facilityOptions)
   {
      for (Facility* f : other.facilities)
      {
         facilities.emplace_back(f->clone());
      }
      for (Facility* f : other.underConstruction)
      {
         underConstruction.emplace_back(f->clone());
      }
   }

//Move constructor
Plan::Plan(Plan &&other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()), facilityOptions(other.facilityOptions),
   status(other.status), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score),
   facilities(other.facilities), underConstruction(other.underConstruction) 
   {
      for (Facility* f : other.facilities)
         f = nullptr;
      for (Facility* f : other.underConstruction)
         f = nullptr;
      other.selectionPolicy = nullptr;
   }

//Destructor
Plan::~Plan()
{
   delete selectionPolicy;
   for (int i=0 ; i<facilities.size() ; i++)
   {
      delete facilities[i];
   }
   for (int i=0 ; i<underConstruction.size() ; i++)
   {
      delete underConstruction[i];
   }
}


//Methods
const int Plan::getlifeQualityScore() const
{
   return life_quality_score;
}

const int Plan::getEconomyScore() const
{
   return economy_score;
}

Plan* Plan::clone()
{
    return new Plan(*this);
}

const int Plan::getEnvironmentScore() const
{
   return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *SelectionPolicy)
{
   selectionPolicy = SelectionPolicy;
}

const string Plan::getSettlementName()
{
   return settlement.getName();
}


void Plan::step()
{
   while (status == PlanStatus::AVALIABLE)
   {
      underConstruction.push_back(new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName()));
      if(underConstruction.size() == static_cast<int>(settlement.getType()) + 1){
         status = PlanStatus::BUSY;
      }
   }

   Facility *curr_facility;
   
   for (int i=0 ; i<underConstruction.size() ; i++)
   {
      curr_facility = underConstruction[i];
      curr_facility->reduceTime();
      if (curr_facility->getStatus() == FacilityStatus::OPERATIONAL)
      {
         facilities.push_back(curr_facility);
         underConstruction.erase(underConstruction.begin() + i);
         i--;
      }
   }

   int type = -1;
   if (settlement.getType() == SettlementType::VILLAGE)
      type = 0;
   else if (settlement.getType() == SettlementType::CITY)
      type = 1;
   else //if (settlement.getType() == SettlementType::METROPOLIS)
      type = 2;
   if (underConstruction.size() == type)
      status = PlanStatus::BUSY;
   else
      status = PlanStatus::AVALIABLE;
}

const string Plan::getPolicy()
{
   return selectionPolicy->getType();
}

void Plan::printFacilities()
{
   for (Facility* f : facilities)
   {
      std::cout << "FacilityName: "+f->getName() +"\n";
      std::cout << "FacilityStatus: OPERATIONAL \n";;

   }
   for (Facility* f : underConstruction)
   {
      std::cout << "FacilityName: "+f->getName() +"\n";
      std::cout << "FacilityStatus: UNDER_CONSTRUCTIONS \n";

   }
}

void Plan::printStatus()
{
   if(status == PlanStatus::AVALIABLE)
   {
      std::cout << "PlanStatus: AVAILABLE \n";
   }

   else
   {
      std::cout << "PlanStatus: BUSY \n";
   }
}

const vector<Facility*>& Plan::getFacilities() const
{
   return facilities;
}

void Plan::addFacility(Facility* facility)
{
   underConstruction.push_back(facility);
}

const string Plan::toString() const
{
   return "PlanID: "+ std::to_string(plan_id) +"\n" + "SettlementName: "+settlement.getName() + "\n" + 
          "LifeQuality_Score: "+std::to_string(life_quality_score) +"\n" + "Economy_Score: "+std::to_string(economy_score) + "\n" +
          "Environment_Score: "+std::to_string(environment_score) +"\n \n";
}           


void Plan::printFacilities()
{
    for(Facility* fa : facilities)
    {
      std::cout << fa->getName() << std::endl;
      std::cout << "OPERATIONAL" << std::endl;
    }

    for(Facility* fa : underConstruction)
    {
      std::cout << fa->getName() << std::endl;
      std::cout << "UNDER_CONSTUCTIONS" << std::endl;
    }
}
