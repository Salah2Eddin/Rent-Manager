#pragma once

// #define DEBUG

#include <string>
#include <vector>
#include <sstream>

#ifdef DEBUG
#include <iostream>
#endif

#include "property.fwd.h"

#include "messages.h"
#include "db_helper.h"

#include "property_obj.fwd.h"

#ifndef DATABASE
#define DATABASE "database.db"
#endif

#ifndef PROPERTY_TABLE
#define PROPERTY_TABLE "property"
#endif

#ifndef LANDLORD_PROPERTY_TABLE
#define LANDLORD_PROPERTY_TABLE "landlord_property"
#endif

#ifndef DEPOSIT_HISTORY_TABLE
#define DEPOSIT_HISTORY_TABLE "deposit_history"
#endif

struct Property {
public:
    struct PaymentHistoryObj{
      std::string date;
      int month, payer_id;
    };

    static int callbackProperty(void *data, int argc, char **argv, char **cols);

    static void createProperty(const std::string &building_number, const std::string &floor, const std::string &renter_name, double rent);

    static std::vector<PropertyObj *> fetch(const std::string &filter = "");

    static int callbackPaymentHistory(void *data, int argc, char **argv, char **cols);
    static std::vector<PaymentHistoryObj *> fetchPaymentHistory(int id = -1);


    static void remove(int id);
};