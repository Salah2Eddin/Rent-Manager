#pragma once

#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>

#include <string>

#include "property.h"
#include "property_obj.h"

class PayRentDialog : public QDialog {
    // Q_OBJECT
public:
    explicit PayRentDialog(QWidget *parent = nullptr);

    ~PayRentDialog();

private slots:
    void payBtnClicked();

private:
    QVBoxLayout *layout;

    QDialogButtonBox *buttons_box;
    QPushButton *btn_pay;

    PropertyObj selected_property;
    std::vector<PropertyObj *> properties;
    QComboBox *combobox_properties, *combobox_months;
    QLabel *lbl_amount, *lbl_property_title, *lbl_month_title;

    void selectedPropertyChanged(int idx);
};
