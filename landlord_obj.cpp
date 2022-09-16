#include "landlord_obj.h"

#include "property.h"
#include "property_obj.h"

LandlordObj::LandlordObj() : id(), name(), balance() {}

/* Adds property to this landlord */
void LandlordObj::addProperty(int property_id, int percentage) const {
    std::stringstream formatter;
    formatter << "INSERT INTO " << LANDLORD_PROPERTY_TABLE << " (landlord_id, property_id, percentage)";
    formatter << " VALUES (" << id << ',' << property_id << ',' << percentage << ");";
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in relationship query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

}

/* Removes property from this landlord */
void LandlordObj::removeProperty(int property_id) const {
    std::stringstream formatter;
    formatter << "DELETE FROM " << LANDLORD_PROPERTY_TABLE << " WHERE landlord_id = " << id
              << " AND property_id = " << property_id << ";";
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in relationship deletion query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

void LandlordObj::deposit(double amount, std::string src) {
    balance += amount;
    std::stringstream formatter;
    formatter << "UPDATE " << LANDLORD_TABLE << " SET balance = " << balance << " WHERE landlord_id = " << id
              << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in deposit query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    formatter.str("");
    formatter << "INSERT INTO " << DEPOSIT_HISTORY_TABLE << " (receiver_id, deposit_date, amount, source)"
              << " VALUES (" << id << ", date(\"now\"), " << amount << ", \"" << src << "\");";
    query = formatter.str();
#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in deposit history query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

void LandlordObj::withdraw(double amount) {
    balance -= amount;
    std::stringstream formatter;
    formatter << "UPDATE " << LANDLORD_TABLE << " SET balance = " << balance << " WHERE landlord_id = " << id
              << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    int error;

    error = sqlite3_open(DATABASE, &SQLiteHelper::db);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in withdrawal query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    formatter.str("");
    formatter << "INSERT INTO " << WITHDRAWAL_HISTORY_TABLE << " (receiver_id, withdrawal_date, amount)"
              << " VALUES (" << id << ", date(\"now\"), " << amount << ");";
    query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    error = sqlite3_open(DATABASE, &SQLiteHelper::db);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in withdrawal history query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

int LandlordObj::callbackLandlordsIds(void *data, int argc, char **argv, char **cols) {
    auto *landlordsId = (std::vector<std::pair<int, int>> *) data;
    std::pair<int, int> p;

    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        if (std::string(cols[i]) == "landlord_id") {
            p.first = stoi(s);
        } else if (std::string(cols[i]) == "percentage") {
            p.second = stoi(s);
        }
    }
    landlordsId->push_back(p);

    return 0;
}

std::vector<PropertyObj *> LandlordObj::getProperties(const std::string &filter) const {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << LANDLORD_PROPERTY_TABLE << " WHERE landlord_id = " << id;
    if (!filter.empty()) {
        formatter << " AND " << filter;
    }
    formatter << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<std::pair<int, int>> ids;

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return {};
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), PropertyObj::callbackPropertyId, &ids, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    formatter.str("");
    formatter << "SELECT * FROM " << PROPERTY_TABLE << " WHERE ";
    for (int i = 0; i < ids.size(); ++i) {
        formatter << "property_id = " << ids[i].first << ' ';
        if (i != ids.size() - 1)
            formatter << "OR ";
    }
    formatter << ';';
    query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<PropertyObj *> v;

    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return {};
    }

    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), Property::callbackProperty, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    return v;
}

std::vector<std::string> LandlordObj::fillRow() const {
    std::vector<std::string> arr = {name, std::to_string(balance)};
    return arr;
}

void LandlordObj::edit(int new_balance) {
    std::stringstream formatter;
    formatter << "UPDATE " << LANDLORD_TABLE << " SET balance = " << new_balance << " WHERE landlord_id = " << id
              << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    int error;

    error = sqlite3_open(DATABASE, &SQLiteHelper::db);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in landlord edit query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}
