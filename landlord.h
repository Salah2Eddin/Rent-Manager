#pragma once

// #define DEBUG

#include <string>
#include <vector>
#include <sstream>

#ifdef DEBUG
#include <iostream>
#endif

#include "landlord.fwd.h"

#include "messages.h"
#include "db_helper.h"

#include "landlord_obj.fwd.h"

#ifndef DATABASE
#define DATABASE "database.db"
#endif

#ifndef LANDLORD_TABLE
#define LANDLORD_TABLE "landlord"
#endif

#ifndef LANDLORD_PROPERTY_TABLE
#define LANDLORD_PROPERTY_TABLE "landlord_property"
#endif

#ifndef WITHDRAWAL_HISTORY_TABLE
#define WITHDRAWAL_HISTORY_TABLE "withdrawal_history"
#endif

struct Landlord {
public:
    struct DepositHistoryObj {
        int amount, receiver_id;
        std::string date, source;
    };

    struct WithdrawalHistoryObj {
        int amount, receiver_id;
        std::string date;
    };

    static void createLandlord(const std::string &name);

    static std::vector<LandlordObj *> fetch(const std::string &filter = "");

    static std::vector<DepositHistoryObj *> fetchDepositHistory(int id=-1);
    static std::vector<WithdrawalHistoryObj *> fetchWithdrawalHistory(int id=-1);

    static void remove(int id);

    static int callbackLandlord(void *data, int argc, char **argv, char **cols);
    static int callbackDepositHistory(void *data, int argc, char **argv, char **cols);
    static int callbackWithdrawalHistory(void *data, int argc, char **argv, char **cols);
};