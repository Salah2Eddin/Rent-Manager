#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>

#include "widget_lbl_textfield.h"

#include <string>

#include "landlord.h"
#include "landlord_obj.h"

class WithdrawDialog : public QDialog {
    // Q_OBJECT
public:
    explicit WithdrawDialog(QWidget *parent = nullptr);

    ~WithdrawDialog();

private slots:
    void withdrawBtnClicked();
    void selectedLandlordChanged(int idx);
private:
    QVBoxLayout *layout;

    QDialogButtonBox *buttons_box;
    QPushButton *btn_withdraw;

    LandlordObj selected_landlord;
    std::vector<LandlordObj *> landlords;

    QComboBox *combobox_landlords;
    QLabel *lbl_balance;

    LabeledTextField *field_amount;
};
