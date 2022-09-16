#include "main_window.h"

MainWindow::MainWindow() {
    central = new QWidget();
    this->setCentralWidget(central);

    buttons_box = new QWidget();

    btn_add_landlord = new QPushButton(tr("Add Landlord"));
    connect(btn_add_landlord, &QPushButton::clicked, this, &MainWindow::addLandlordDialog);

    btn_add_property = new QPushButton(tr("Add Property"));
    connect(btn_add_property, &QPushButton::clicked, this, &MainWindow::addPropertyDialog);

    btn_pay_rent = new QPushButton(tr("Pay rent"));
    connect(btn_pay_rent, &QPushButton::clicked, this, &MainWindow::payRentDialog);

    btn_withdraw = new QPushButton(tr("Withdraw"));
    connect(btn_withdraw, &QPushButton::clicked, this, &MainWindow::withdrawMoneyDialog);

    btn_landlords_view = new QPushButton(tr("Landlords View"));
    connect(btn_landlords_view, &QPushButton::clicked, this, &MainWindow::landlordsView);

    btn_properties_view = new QPushButton(tr("Properties View"));
    connect(btn_properties_view, &QPushButton::clicked, this, &MainWindow::propertiesView);

    btn_deposit_view = new QPushButton(tr("Deposit History View"));
    connect(btn_deposit_view, &QPushButton::clicked, this, &MainWindow::depositHistoryView);

    btn_withdrawal_view = new QPushButton(tr("Withdrawal History View"));
    connect(btn_withdrawal_view, &QPushButton::clicked, this, &MainWindow::withdrawalHistoryView);

    btn_payment_view = new QPushButton(tr("Payment History View"));
    connect(btn_payment_view, &QPushButton::clicked, this, &MainWindow::paymentHistoryView);

    buttons_box_layout = new QVBoxLayout();
    buttons_box_layout->setAlignment(Qt::AlignTop);
    buttons_box->setLayout(buttons_box_layout);
    buttons_box_layout->addWidget(btn_add_landlord);
    buttons_box_layout->addWidget(btn_add_property);
    buttons_box_layout->addWidget(btn_pay_rent);
    buttons_box_layout->addWidget(btn_withdraw);
    buttons_box_layout->addWidget(btn_landlords_view);
    buttons_box_layout->addWidget(btn_properties_view);
    buttons_box_layout->addWidget(btn_deposit_view);
    buttons_box_layout->addWidget(btn_withdrawal_view);
    buttons_box_layout->addWidget(btn_payment_view);


    view_list = new QListWidget();
    view_list->setSelectionMode(QAbstractItemView::NoSelection);
    view_list->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    view_list->setMinimumWidth(300);

    central_layout = new QHBoxLayout();
    central->setLayout(central_layout);
    central_layout->addWidget(view_list);
    central_layout->addWidget(buttons_box);

    current_view = 0;
    updateCurrentView();
}

MainWindow::~MainWindow() {
    delete btn_add_landlord;
    delete btn_add_property;
    delete btn_pay_rent;
    delete btn_withdraw;

    delete buttons_box_layout;
    delete buttons_box;
    delete central_layout;
    delete view_list;
    delete central;
}

void MainWindow::addLandlordDialog() {
    auto dial = new AddLandlordDialog(this);
    dial->exec();

    delete dial;

    updateCurrentView();
}

void MainWindow::addPropertyDialog() {
    auto dial = new AddPropertyDialog(this);
    dial->exec();

    delete dial;

    updateCurrentView();
}

void MainWindow::payRentDialog() {
    auto dial = new PayRentDialog(this);
    dial->exec();

    delete dial;

    updateCurrentView();
}

void MainWindow::withdrawMoneyDialog() {
    auto dial = new WithdrawDialog(this);
    dial->exec();

    delete dial;

    updateCurrentView();
}

void MainWindow::landlordsView() {
    view_list->clear();

    auto landlords = Landlord::fetch();
    for (auto *landlord: landlords) {
        auto landlord_item = new ViewListLandlordItem(landlord);
        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(landlord_item->size());

        landlord_item->onRemove = [this]() { updateCurrentView(); };

        view_list->addItem(list_item);
        view_list->setItemWidget(list_item, landlord_item);
    }

    current_view = 0;
}

void MainWindow::propertiesView() {
    view_list->clear();

    auto properties = Property::fetch();
    for (auto *property: properties) {
        auto property_item = new ViewListPropertyItem(property);
        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(property_item->size());

        property_item->onRemove = [this]() { updateCurrentView(); };

        view_list->addItem(list_item);
        view_list->setItemWidget(list_item, property_item);
    }
    current_view = 1;
}

void MainWindow::depositHistoryView() {
    view_list->clear();

    auto landlords = Landlord::fetch();
    std::map<int, std::string> id_name;
    for (auto landlord: landlords)
        id_name[landlord->id] = landlord->name;

    auto history = Landlord::fetchDepositHistory();
    std::unordered_map<std::string, std::string> mp;
    for (auto *entry: history) {
        mp["Source"] = entry->source;
        mp["Amount"] = std::to_string(entry->amount);
        mp["Date"] = entry->date;
        mp["Receiver"] = id_name[entry->receiver_id];

        auto property_item = new ViewListItemLabels(mp);
        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(property_item->size());

        view_list->addItem(list_item);
        view_list->setItemWidget(list_item, property_item);
    }
    current_view = 2;
}

void MainWindow::withdrawalHistoryView() {
    view_list->clear();

    auto landlords = Landlord::fetch();
    std::map<int, std::string> id_name;
    for (auto landlord: landlords)
        id_name[landlord->id] = landlord->name;

    auto history = Landlord::fetchWithdrawalHistory();
    std::unordered_map<std::string, std::string> mp;
    for (auto *entry: history) {
        mp["Amount"] = std::to_string(entry->amount);
        mp["Date"] = entry->date;
        mp["Receiver"] = id_name[entry->receiver_id];

        auto property_item = new ViewListItemLabels(mp);
        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(property_item->size());

        view_list->addItem(list_item);
        view_list->setItemWidget(list_item, property_item);
    }
    current_view = 3;
}

void MainWindow::paymentHistoryView() {
    view_list->clear();

    auto properties = Property::fetch();
    std::map<int, std::string> id_name;
    for (auto property: properties)
        id_name[property->id] = property->renter_name;

    auto history = Property::fetchPaymentHistory();
    std::unordered_map<std::string, std::string> mp;
    for (auto *entry: history) {
        mp["Month"] = std::to_string(entry->month);
        mp["Date"] = entry->date;
        mp["Property"] = id_name[entry->payer_id];

        auto property_item = new ViewListItemLabels(mp);
        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(property_item->size());

        view_list->addItem(list_item);
        view_list->setItemWidget(list_item, property_item);
    }
    current_view = 4;
}

void MainWindow::updateCurrentView() {
    switch (current_view) {
        case 0: {
            landlordsView();
            break;
        }
        case 1: {
            propertiesView();
            break;
        }
        case 2: {
            depositHistoryView();
            break;
        }
        case 3: {
            withdrawalHistoryView();
            break;
        }
        case 4:{
            paymentHistoryView();
            break;
        }
        default: {
            break;
        }
    }
}

