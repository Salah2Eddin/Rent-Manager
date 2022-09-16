//
// Created by Salah on 03/09/2022.
//
#include "property.h"
#include "property_obj.h"

void
Property::createProperty(const std::string &building_number, const std::string &floor, const std::string &renter_name,
                         double rent) {
    std::stringstream formatter;
    formatter << "INSERT INTO " << PROPERTY_TABLE << " (building_number, floor, renter_name, rent) VALUES (\""
              << building_number << "\", \"" << floor << "\", \"" << renter_name << "\", " << rent << ");";
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
        MessagesHelper::showMessageBox("Error", "Error in property creation query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

std::vector<PropertyObj *> Property::fetch(const std::string &filter) {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << PROPERTY_TABLE;
    if (!filter.empty()) {
        formatter << " WHERE " << filter;
    }
    formatter << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<PropertyObj *> v;

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return v;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), callbackProperty, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    return v;
}

int Property::callbackProperty(void *data, int argc, char **argv, char **cols) {
    auto *properties = (std::vector<PropertyObj *> *) data;
    auto *property = new PropertyObj;

    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        switch (i) {
            case 0: {
                property->id = stoi(s);
                break;
            }
            case 1: {
                property->building_number = s;
                break;
            }
            case 2: {
                property->floor = s;
                break;
            }
            case 3: {
                property->renter_name = s;
                break;
            }
            case 4: {
                property->rent = std::stod(s);
                break;
            }
            default: {
                break;
            }
        }
    }
    properties->push_back(property);

    return 0;
}

void Property::remove(int id) {
    std::stringstream formatter;
    formatter << "DELETE FROM " << LANDLORD_PROPERTY_TABLE << " WHERE property_id = " << id << ';';
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
        MessagesHelper::showMessageBox("Error", "Error in delete query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    formatter.str("");
    formatter << "DELETE FROM " << PROPERTY_TABLE << " WHERE property_id = " << id << ';';
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

    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in delete query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

int Property::callbackPaymentHistory(void *data, int argc, char **argv, char **cols) {
    auto *objects = (std::vector<PaymentHistoryObj *> *) data;
    auto *obj = new PaymentHistoryObj;
    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        if (strcmp(cols[i], "payer_id") == 0) {
            obj->payer_id = stoi(s);
        }else if (strcmp(cols[i], "payment_month") == 0) {
            obj->month = stoi(s);
        } else if (strcmp(cols[i], "payment_date") == 0) {
            obj->date = s;
        }
    }
    objects->push_back(obj);
    return 0;
}

std::vector<Property::PaymentHistoryObj *> Property::fetchPaymentHistory(int id) {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << PAYMENT_HISTORY_TABLE;
    if(~id) { formatter << " WHERE payer_id = " << id; }
    formatter << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<PaymentHistoryObj *> v;
    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return v;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), callbackPaymentHistory, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in withdrawal history fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    return v;
}
