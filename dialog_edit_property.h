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

class EditPropertyDialog : public QDialog {
public:
    explicit EditPropertyDialog(PropertyObj *prop, QWidget *parent = nullptr);
    ~EditPropertyDialog();

    PropertyObj *property;
private slots:
    void editBtnClicked();
private:
    QVBoxLayout *layout;

    QDialogButtonBox *buttons_box;
    QPushButton *btn_add;
    QLabel *lbl_building_number, *lbl_floor_number, *lbl_renter_name;
    LabeledTextField *field_rent;

    QListWidget *list_landlords;
    std::vector<LandlordObj *> landlords;
};
