#include <QApplication>
#include <QStyleFactory>

#include "main_window.h"

#include <iostream>

#include "db_helper.h"

#include "landlord.h"
#include "property.h"

#include "landlord_obj.h"
#include "property_obj.h"

sqlite3 *SQLiteHelper::db;

int main(int argc, char *argv[]) {
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setApplicationName(QString::fromStdString("Rent Manager"));
    QApplication a(argc, argv);

    SQLiteHelper::initDatabase();

    MainWindow mw;
    mw.show();

    return QApplication::exec();
}