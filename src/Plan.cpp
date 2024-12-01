#include "Plan.h"
#include "Facility.h"

//Default constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
: plan_id(planId),
  settlement(settlement),
  selectionPolicy(selectionPolicy), 
  status(PlanStatus::AVALIABLE), 
  facilityOptions(facilityOptions),
  life_quality_score(0), economy_score(0), environment_score(0){
    //Still needed: vector<Facility*> facilities;
       // vector<Facility*> underConstruction;
       // const vector<FacilityType> &facilityOptions;
    }


 //public:
        //Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        //const int getlifeQualityScore() const;
        //const int getEconomyScore() const;
       // const int getEnvironmentScore() const;
       // void setSelectionPolicy(SelectionPolicy *selectionPolicy);
       // void step();
       // void printStatus();
        //const vector<Facility*> &getFacilities() const;
        //void addFacility(Facility* facility);
       // const string toString() const;

  //  private:
      //  int plan_id;
      //  const Settlement &settlement;
      //  SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
      //  PlanStatus status;
       // vector<Facility*> facilities;
       // vector<Facility*> underConstruction;
       // const vector<FacilityType> &facilityOptions;
    //int life_quality_score, economy_score, environment_score;
