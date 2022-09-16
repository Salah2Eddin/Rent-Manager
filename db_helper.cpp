//
// Created by Salah on 06/09/2022.
//
#include "db_helper.h"

/* Creates initial database tables if they don't exist */
void SQLiteHelper::initDatabase() {
    std::string sql;

    sql = "CREATE TABLE IF NOT EXISTS landlord("
          "landlord_id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "name TEXT NOT NULL, "
          "balance REAL DEFAULT 0);";
    execute(sql);

    sql = "CREATE TABLE IF NOT EXISTS property( "
          "property_id INTEGER  PRIMARY KEY AUTOINCREMENT,"
          "building_number TEXT NOT NULL,"
          "floor TEXT NOT NULL,"
          "renter_name TEXT NOT NULL,"
          "rent REAL NOT NULL);";
    execute(sql);

    sql = "CREATE TABLE IF NOT EXISTS landlord_property("
          "landlord_id INTEGER, "
          "property_id INTEGER, "
          "percentage INTEGER, "
          "unique (landlord_id, property_id),"
          "FOREIGN KEY (landlord_id) REFERENCES landlord (landlord_id), "
          "FOREIGN KEY (property_id) REFERENCES property (property_id));";
    execute(sql);

    sql = "CREATE TABLE IF NOT EXISTS payment_history("
          "payment_id INTEGER PRIMARY KEY AUTOINCREMENT,"
          "payer_id INTEGER NOT NULL,"
          "payment_month INTEGER NOT NULL,"
          "payment_date DATE NOT NULL,"
          "FOREIGN KEY (payer_id) REFERENCES property (property_id));";
    execute(sql);

    sql = "CREATE TABLE IF NOT EXISTS withdrawal_history("
          "withdrawal_id INTEGER  PRIMARY KEY AUTOINCREMENT,"
          "receiver_id INTEGER  NOT NULL,"
          "amount REAL NOT NULL,"
          "withdrawal_date DATE NOT NULL,"
          "FOREIGN KEY (receiver_id) REFERENCES landlord (landlord_id));";
    execute(sql);

    sql = "CREATE TABLE IF NOT EXISTS deposit_history("
          "deposit_id INTEGER  PRIMARY KEY AUTOINCREMENT,"
          "receiver_id INTEGER  NOT NULL,"
          "amount REAL NOT NULL,"
          "deposit_date DATE NOT NULL,"
          "source TEXT NOT NULL,"
          "FOREIGN KEY (receiver_id) REFERENCES landlord (landlord_id));";
    execute(sql);
}

/* executes a sql query and shows an error message in case of errors.
 * callbackLandlord is called after each row affected by the query
 * callbackPropertyId(void *, int number_of_columns, char** data, char** column_names)
*/

void SQLiteHelper::execute(const std::string &query) {
    int error;
    error = sqlite3_open(DATABASE, &db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in database query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(db);
}
