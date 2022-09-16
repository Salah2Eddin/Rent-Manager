#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>

#include "dialog_add_lanlord.h"
#include "dialog_add_property.h"
#include "dialog_pay_rent.h"
#include "dialog_withdraw.h"

#include "widget_view_list_landlord.h"
#include "widget_view_list_property.h"

#include <iostream>

class MainWindow : public QMainWindow {
public:
    MainWindow();

    ~MainWindow();

private slots:

    void addLandlordDialog();

    void addPropertyDialog();

    void payRentDialog();

    void withdrawMoneyDialog();

    void landlordsView();

    void propertiesView();

    void depositHistoryView();

    void withdrawalHistoryView();

    void paymentHistoryView();

private:

    QWidget *buttons_box, *central;
    QPushButton *btn_add_landlord, *btn_add_property, *btn_pay_rent, *btn_withdraw, *btn_landlords_view,
            *btn_properties_view, *btn_withdrawal_view, *btn_deposit_view, *btn_payment_view;

    QHBoxLayout *central_layout;

    QVBoxLayout *buttons_box_layout;
    QListWidget *view_list;

    int current_view;

    void updateCurrentView();

};