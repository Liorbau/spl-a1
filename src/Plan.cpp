#include "Plan.h"
#include "Facility.h"
#include "vector"
#include <iostream>

//---------------- Rule of 5 ----------------

//Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) :
   plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),
   facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){}

//Copy constuctor
Plan::Plan(const Plan &other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()),
   status(other.status), facilities(), underConstruction(), facilityOptions(other.facilityOptions),
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
   }

   //Copy constructor 2 
   Plan::Plan(const Plan &other, const Settlement &s):
   plan_id(other.plan_id), settlement(s), selectionPolicy(other.selectionPolicy->clone()),
   status(other.status), facilities(), underConstruction(), facilityOptions(other.facilityOptions),
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
   }

//Move constructor
Plan::Plan(Plan &&other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy), 
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
<<<<<<< HEAD
      string type = selectionPolicy->getType();
      if (type == "bal")
      {
         BalancedSelection pol = BalancedSelection(life_quality_score, economy_score, environment_score);
         FacilityType toBuild = pol.selectFacility(facilityOptions);
         life_quality_score = life_quality_score + toBuild.getLifeQualityScore(); 
         economy_score = economy_score + toBuild.getEconomyScore();
         environment_score = environment_score + toBuild.getEnvironmentScore();
         underConstruction.push_back(new Facility(toBuild, settlement.getName()));
      }
      else
      {
         FacilityType toBuild = selectionPolicy->selectFacility(facilityOptions);
         life_quality_score = life_quality_score + toBuild.getLifeQualityScore(); 
         economy_score = economy_score + toBuild.getEconomyScore();
         environment_score = environment_score + toBuild.getEnvironmentScore();
         underConstruction.push_back(new Facility(toBuild, settlement.getName()));
      }
=======
      Facility *new_fa = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
      underConstruction.push_back(new_fa);

>>>>>>> 64b74106211d3b04737b79fe8ab900e24066fca3
      if(static_cast<int>(underConstruction.size()) == cap){
         status = PlanStatus::BUSY;
      }
   }

   for (int i=0 ; i<static_cast<int>(underConstruction.size()) ; i++)
   {
      underConstruction[i]->reduceTime();
      
      if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
      {
<<<<<<< HEAD
         facilities.push_back(underConstruction[i]->clone());
=======
         life_quality_score += underConstruction[i]->getLifeQualityScore();
         environment_score += underConstruction[i]->getEnvironmentScore();
         economy_score += underConstruction[i]->getEconomyScore();
         facilities.push_back(underConstruction[i]);
>>>>>>> 64b74106211d3b04737b79fe8ab900e24066fca3
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
   std::cout << "SelectionPolicy: "+ getPolicy()+ "\n";
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
