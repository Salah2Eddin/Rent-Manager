#pragma once

#include <string>
#include <sqlite3.h>

#include "messages.h"

#define DATABASE "database.db"

struct SQLiteHelper {
public:
    static sqlite3 *db;

    static void initDatabase();

private:
    static void execute(const std::string &query);
};
