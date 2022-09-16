#pragma once

// #define DEBUG

#include <string>
#include <vector>
#include <sstream>

#ifdef DEBUG
#include <iostream>
#endif //DEBUG

#include "property_obj.fwd.h"

#include "messages.h"
#include "db_helper.h"

#include "landlord_obj.fwd.h"

#ifndef DATABASE
#define DATABASE "database.db"
#endif

#ifndef PROPERTY_TABLE
#define PROPERTY_TABLE "property"
#endif

#ifndef LANDLORD_PROPERTY_TABLE
#define LANDLORD_PROPERTY_TABLE "landlord_property"
#endif

#ifndef PAYMENT_HISTORY_TABLE
#define PAYMENT_HISTORY_TABLE "payment_history"
#endif

struct PropertyObj {
public:
    int id, rent;
    std::string renter_name, building_number, floor;

    PropertyObj();

    void addLandlord(int landlord_id, int percentage) const;

    void removeProperty(int landlord_id) const;

    std::vector<std::pair<LandlordObj *, int>> getLandlords(const std::string &filter = "") const;

    void payRent(int month) const;

    void edit(double new_rent);

    void clearLandlords() const;

    static int callbackPropertyId(void *data, int argc, char **argv, char **cols);

    std::vector<std::string> fillRow() const;
};