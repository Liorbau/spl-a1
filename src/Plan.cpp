#include "Plan.h"
#include "Facility.h"
#include "vector"
#include <iostream>

//---------------- Rule of 5 ----------------

//Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) :
   plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),
   facilities(vector <Facility*>()), underConstruction(vector <Facility*>()), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){}

//Copy constuctor
Plan::Plan(const Plan &other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()),
   status(other.status), facilities(other.facilities), underConstruction(other.underConstruction), facilityOptions(other.facilityOptions),
   life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score)
   {
      for (Facility* f : other.facilities)
      {
         facilities.push_back(f->clone());
      }
      for (Facility* f : other.underConstruction)
      {
         underConstruction.push_back(f->clone());
      }
      selectionPolicy = other.selectionPolicy->clone();
   }

//Move constructor
Plan::Plan(Plan &&other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()), 
   status(other.status), facilities(other.facilities), underConstruction(other.underConstruction),
   facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score)
   {
      other.selectionPolicy = nullptr;
      other.facilities.clear();
      other.underConstruction.clear();
   }

//Destructor
Plan::~Plan()
{
   delete selectionPolicy;
   for (size_t i=0 ; i<facilities.size() ; i++)
   {
      delete facilities[i];
   }
   for (size_t i=0 ; i<underConstruction.size() ; i++)
   {
      delete underConstruction[i];
   }
   facilities.clear();
   underConstruction.clear();
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
   int cap;
      if (settlement.getType() == SettlementType::VILLAGE) cap = 1;
      else if (settlement.getType() == SettlementType::CITY) cap = 2;
      else cap = 3;

   while (status == PlanStatus::AVALIABLE)
   {
      FacilityType f = selectionPolicy->selectFacility(facilityOptions);
      underConstruction.push_back(new Facility(f, settlement.getName()));
      life_quality_score += f.getLifeQualityScore();
      environment_score += f.getEnvironmentScore();
      economy_score += f.getEconomyScore();
      if(static_cast<int>(underConstruction.size()) == cap){
         status = PlanStatus::BUSY;
      }
   }

   for (int i=0 ; i<static_cast<int>(underConstruction.size()) ; i++)
   {
      underConstruction[i]->reduceTime();
      
      if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
      {
         facilities.push_back(underConstruction[i]->clone());
         underConstruction.erase(underConstruction.begin() + i);
         i--;
      }
   }

   if(static_cast<int>(underConstruction.size()) < cap){
         status = PlanStatus::AVALIABLE;
   }
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
   underConstruction.push_back(facility->clone());
}

void Plan::printPlan() 
{
   string s ="";
   if (status == PlanStatus::AVALIABLE)
      s = "AVAILABLE";
   else
      s = "BUSY";
   string p = "";
   std::cout << "PlanID: "+ std::to_string(plan_id) +"\n" ; 
   std::cout << "SettlementName: "+ settlement.getName() +"\n" ; 
   std::cout << "PlanStatus: "+ s +"\n" ; 
   std::cout << "SelectionPolicy "+ getPolicy()+ "\n";
   std::cout << "LifeQuality_Score: "+ std::to_string(life_quality_score) +"\n" ; 
   std::cout << "Economy_Score: "+ std::to_string(economy_score) +"\n" ; 
   std::cout << "Environment_Score: "+ std::to_string(environment_score) +"\n" ; 
                     
   for (Facility* f : facilities)
   {
      std::cout << "FacilityName: " + f->getName() + "\n" ;
      std::cout << "FacilityStatus: OPERATIONAL \n";
   }
   for (Facility* f : underConstruction)
   {
      std::cout << "FacilityName: " + f->getName() + "\n" ;
      std::cout << "FacilityStatus: UNDER_CONSTRUCTION \n";
   }

}           
