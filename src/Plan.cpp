#include "Plan.h"
#include "Facility.h"
#include "vector"

//Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) :
   plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
   status(PlanStatus::AVALIABLE), life_quality_score(0), economy_score(0), environment_score(0),
   facilities(vector <Facility*>()), underConstruction(vector <Facility*>()) {}

//Copy constuctor
Plan::Plan(Plan &other) :
   plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy), facilityOptions(other.facilityOptions),
   status(other.status), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score),
   facilities(other.facilities), underConstruction(other.underConstruction) {}

//Methods
const int Plan::getlifeQualityScore() const
{
   return life_quality_score;
}

const int Plan::getEconomyScore() const
{
   return economy_score;
}

const int Plan::getEnvironmentScore() const
{
   return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *SelectionPolicy)
{
   selectionPolicy = SelectionPolicy;
}

void Plan::step()
{
   if (status == PlanStatus::AVALIABLE)
   {

   }
   //Step 3
   Facility *curr_facility;
   for (int i=0 ; i<underConstruction.size() ; i++)
   {
      curr_facility = underConstruction[i];
      curr_facility->reduceTime();
      if (curr_facility->getStatus() == FacilityStatus::OPERATIONAL)
      {
         //underConstruction.;
      }
   }

}

/*
 enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};
*/