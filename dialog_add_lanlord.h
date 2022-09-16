#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "widget_lbl_textfield.h"
#include <string>

class AddLandlordDialog : public QDialog {
    // Q_OBJECT
public:
    explicit AddLandlordDialog(QWidget *parent = nullptr);

    ~AddLandlordDialog();

private slots:
    void addBtnClicked();

private:
    QVBoxLayout *layout;

    QDialogButtonBox *buttons_box;
    QPushButton *btn_add;
    LabeledTextField *field_name;
};