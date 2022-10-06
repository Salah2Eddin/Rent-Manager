#include "property_obj.h"

#include "landlord.h"
#include "landlord_obj.h"

PropertyObj::PropertyObj() : id(), building_number(), floor(), renter_name(), rent() {}

void PropertyObj::addLandlord(int landlord_id, int percentage) const {
    std::stringstream formatter;
    formatter << "INSERT INTO " << LANDLORD_PROPERTY_TABLE << "(landlord_id, property_id, percentage)";
    formatter << " VALUES (" << landlord_id << ',' << id << ',' << percentage << ");";
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
        MessagesHelper::showMessageBox("Error", "Error in relationship creation query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

}

void PropertyObj::removeProperty(int landlord_id) const {
    clearLandlords();
    std::stringstream formatter;
    formatter << "DELETE FROM " << PROPERTY_TABLE << " WHERE property_id = " << id << ";";
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
        MessagesHelper::showMessageBox("Error", "Error in property deletion query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

int PropertyObj::callbackPropertyId(void *data, int argc, char **argv, char **cols) {
    auto *propertiesId = (std::vector<std::pair<int, int>> *) data;
    std::pair<int, int> p;

    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        if (std::string(cols[i]) == "property_id") {
            p.first = stoi(s);
        } else if (std::string(cols[i]) == "percentage") {
            p.second = stoi(s);
        }
    }
    propertiesId->push_back(p);

    return 0;
}

std::vector<std::pair<LandlordObj *, int>> PropertyObj::getLandlords(const std::string &filter) const {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << LANDLORD_PROPERTY_TABLE << " WHERE property_id = " << id;
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
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), LandlordObj::callbackLandlordsIds, &ids, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    formatter.str("");
    formatter << "SELECT * FROM " << LANDLORD_TABLE << " WHERE ";
    for (int i = 0; i < ids.size(); ++i) {
        formatter << "landlord_id = " << ids[i].first << ' ';
        if (i != ids.size() - 1)
            formatter << "OR ";
    }
    formatter << ';';
    query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<LandlordObj *> v;

    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return {};
    }

    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), Landlord::callbackLandlord, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    std::vector<std::pair<LandlordObj *, int>> ret;

    for (int i = 0; i < v.size(); ++i) {
        ret.emplace_back(v[i], ids[i].second);
    }
    return ret;
}

void PropertyObj::payRent(int month) const {
    auto landlords = getLandlords();

    for (auto &landlord: landlords) {
        double amount = (double) landlord.second / 100 * rent;
        landlord.first->deposit(amount, renter_name + "- ايجار شهر: " + std::to_string(month));
    }

    for (int i = landlords.size() - 1; i >= 0; --i) {
        delete landlords[i].first;
    }

    std::stringstream formatter;
    formatter << "INSERT INTO " << PAYMENT_HISTORY_TABLE << " (payer_id, payment_date, payment_month) VALUES (" << id
              << ", date(\"now\"), " << month << ");";
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    int error = sqlite3_open(DATABASE, &SQLiteHelper::db);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in payment history query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

}

std::vector<std::string> PropertyObj::fillRow() const {
    std::vector<std::string> arr(4);
    arr[0] = renter_name;
    arr[1] = building_number;
    arr[2] = floor;
    arr[3] = std::to_string(rent);
    return arr;
}

void PropertyObj::edit(double new_rent) {
    std::stringstream formatter;
    formatter << "UPDATE " << PROPERTY_TABLE << " SET rent = " << new_rent << " WHERE property_id = " << id << ';';
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
        MessagesHelper::showMessageBox("Error", "Error in property edit query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

void PropertyObj::clearLandlords() const {
    std::stringstream formatter;
    formatter << "DELETE FROM " << LANDLORD_PROPERTY_TABLE << " WHERE property_id = " << id << ";";
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
};