#include "landlord.h"
#include "landlord_obj.h"

void Landlord::createLandlord(const std::string &name) {
    std::stringstream formatter;
    formatter << "INSERT INTO " << LANDLORD_TABLE << " (name) VALUES (\"" << name << "\");";
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
        MessagesHelper::showMessageBox("Error", "Error in landlord creation query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);
}

int Landlord::callbackLandlord(void *data, int argc, char **argv, char **cols) {
    auto *landlords = (std::vector<LandlordObj *> *) data;
    auto *landlord = new LandlordObj;

    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        switch (i) {
            case 0: {
                landlord->id = std::stoi(s);
                break;
            }
            case 1: {
                landlord->name = s;
                break;
            }
            case 2: {
                landlord->balance = stoi(s);
                break;
            }
            default: {
                break;
            }
        }
    }
    landlords->push_back(landlord);

    return 0;
}

std::vector<LandlordObj *> Landlord::fetch(const std::string &filter) {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << LANDLORD_TABLE;
    if (!filter.empty()) {
        formatter << " WHERE " << filter;
    }
    formatter << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<LandlordObj *> v;

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return v;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), callbackLandlord, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    return v;
}

void Landlord::remove(int id) {
    std::stringstream formatter;
    formatter << "DELETE FROM " << LANDLORD_PROPERTY_TABLE << " WHERE landlord_id = " << id << ';';
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
    formatter << "DELETE FROM " << LANDLORD_TABLE << " WHERE landlord_id = " << id << ';';
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

int Landlord::callbackDepositHistory(void *data, int argc, char **argv, char **cols) {
    auto *objects = (std::vector<DepositHistoryObj *> *) data;
    auto *obj = new DepositHistoryObj;

    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        if (strcmp(cols[i], "receiver_id") == 0) {
            obj->receiver_id = stoi(s);
        } else if (strcmp(cols[i], "amount") == 0) {
            obj->amount = stoi(s);
        } else if (strcmp(cols[i], "deposit_date") == 0) {
            obj->date = s;
        } else if (strcmp(cols[i], "source") == 0) {
            obj->source = s;
        }
    }
    objects->push_back(obj);
    return 0;
}

std::vector<Landlord::DepositHistoryObj *> Landlord::fetchDepositHistory(int id) {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << DEPOSIT_HISTORY_TABLE;
    if (~id) { formatter << " WHERE receiver_id = " << id; }
    formatter << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<DepositHistoryObj *> v;

    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return v;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), callbackDepositHistory, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    return v;
}

int Landlord::callbackWithdrawalHistory(void *data, int argc, char **argv, char **cols) {
    auto *objects = (std::vector<WithdrawalHistoryObj *> *) data;
    auto *obj = new WithdrawalHistoryObj;
    for (int i = 0; i < argc; ++i) {
        std::string s(argv[i]);
        if (strcmp(cols[i], "receiver_id") == 0) {
            obj->receiver_id = stoi(s);
        }else if (strcmp(cols[i], "amount") == 0) {
            obj->amount = stoi(s);
        } else if (strcmp(cols[i], "withdrawal_date") == 0) {
            obj->date = s;
        }
    }
    objects->push_back(obj);
    return 0;
}

std::vector<Landlord::WithdrawalHistoryObj *> Landlord::fetchWithdrawalHistory(int id) {
    std::stringstream formatter;
    formatter << "SELECT * FROM " << WITHDRAWAL_HISTORY_TABLE;
    if (~id) { formatter << " WHERE receiver_id = " << id; }
    formatter << ';';
    std::string query = formatter.str();

#ifdef DEBUG
    std::cout << query << std::endl;
#endif

    std::vector<WithdrawalHistoryObj *> v;
    int error;
    error = sqlite3_open(DATABASE, &SQLiteHelper::db);

    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in opening database!");
        sqlite3_close(SQLiteHelper::db);
        return v;
    }

    char *errorMessage;
    error = sqlite3_exec(SQLiteHelper::db, query.c_str(), callbackWithdrawalHistory, &v, &errorMessage);
    if (error) {
        MessagesHelper::showMessageBox("Error", "Error in withdrawal history fetching query!");
        sqlite3_free(errorMessage);
    }
    sqlite3_close(SQLiteHelper::db);

    return v;
}
