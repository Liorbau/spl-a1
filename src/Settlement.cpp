#include "Settlement.h"

    Settlement::Settlement() : name(""), type(SettlementType::VILLAGE) {}

    Settlement::Settlement(const string &name, SettlementType type) :
        name(name),
        type(type)
    {}

    const string& Settlement::getName() const
    {
        return name;
    }

    SettlementType Settlement::getType() const
    {
        return type;
    }

    const string Settlement::toString() const
    {
        if (type == SettlementType::VILLAGE)
        {
            return "VILLAGE";
        }
        else if (type == SettlementType::CITY)
        {
            return "CITY";
        }
        else  //(type == SettlementType::METROPOLIS)
        {
            return "METROPOLIS";
        }
    }