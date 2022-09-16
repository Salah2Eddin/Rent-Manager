#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QListWidget>
#include <QListWidgetItem>

#include <string>

#include "widget_lbl_textfield.h"
#include <widget_landlords_list_item.h>

#include "property.h"
#include "landlord.h"

#include "property_obj.h"

class AddPropertyDialog : public QDialog {
public:
    explicit AddPropertyDialog(QWidget *parent = nullptr);

    ~AddPropertyDialog();

private slots:
    void addBtnClicked();
private:
    QVBoxLayout *layout;

    QDialogButtonBox *buttons_box;
    QPushButton *btn_add;
    LabeledTextField *field_building_number, *field_floor_number, *field_renter_name, *field_rent;

    QListWidget *list_landlords;
    std::vector<LandlordObj *> landlords;
};
