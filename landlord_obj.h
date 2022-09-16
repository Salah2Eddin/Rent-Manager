#pragma once

// #define DEBUG

#include <string>
#include <vector>
#include <sstream>

#ifdef DEBUG

#include <iostream>

#endif

#include "landlord.fwd.h"
#include "landlord_obj.fwd.h"

#include "messages.h"
#include "db_helper.h"

#include "property.fwd.h"
#include "property_obj.fwd.h"

#ifndef DATABASE
#define DATABASE "database.db"
#endif

#ifndef LANDLORD_TABLE
#define LANDLORD_TABLE "landlord"
#endif

#ifndef LANDLORD_PROPERTY_TABLE
#define LANDLORD_PROPERTY_TABLE "landlord_property"
#endif

#ifndef DEPOSIT_HISTORY_TABLE
#define DEPOSIT_HISTORY_TABLE "deposit_history"
#endif

#ifndef WITHDRAWAL_HISTORY_TABLE
#define WITHDRAWAL_HISTORY_TABLE "withdrawal_history"
#endif

struct LandlordObj {
public:
    int id;
    std::string name;
    int balance;

    LandlordObj();

    void addProperty(int property_id, int percentage) const;

    void removeProperty(int property_id) const;

    [[nodiscard]] std::vector<PropertyObj *> getProperties(const std::string &filter = "") const;

    void edit(int new_balance);

    void deposit(double amount, std::string src);

    void withdraw(double amount);

    static int callbackLandlordsIds(void *data, int argc, char **argv, char **cols);

    std::vector<std::string> fillRow() const;
};
