#include "widget_view_list_landlord.h"

ViewListLandlordItem::ViewListLandlordItem(LandlordObj *l) {
    landlord = l;

    layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignLeft);
    layout->setSpacing(50);
    this->setLayout(layout);

    setMaximumHeight(120);

    lbl_name = new QLabel(QString::fromStdString("Name\n" + landlord->name));
    layout->addWidget(lbl_name);

    lbl_balance = new QLabel(QString::fromStdString("Balance\n" + std::to_string(landlord->balance)));
    layout->addWidget(lbl_balance);

    buttons_box = new QWidget();
    buttons_box_layout = new QVBoxLayout();
    buttons_box_layout->setAlignment(Qt::AlignTop);
    buttons_box->setMaximumWidth(300);
    buttons_box->setLayout(buttons_box_layout);

    btn_withdrawal_history = new QPushButton(tr("Withdrawal History"));
    connect(btn_withdrawal_history, &QPushButton::clicked, this, [&]() {
        auto history = Landlord::fetchWithdrawalHistory(landlord->id);

        auto list = new QListWidget();
        list->setSelectionMode(QAbstractItemView::NoSelection);
        std::unordered_map<std::string, std::string> title_value_map;
        for (auto entry: history) {
            title_value_map["Date"] = entry->date;
            title_value_map["Amount"] = std::to_string(entry->amount);

            auto widget = new ViewListItemLabels(title_value_map);

            auto list_item = new QListWidgetItem();
            list->addItem(list_item);
            list_item->setSizeHint(widget->size());
            list->setItemWidget(list_item, widget);
        }

        auto dial = new QDialog();
        auto dial_layout = new QGridLayout();
        dial->setLayout(dial_layout);

        dial->layout()->addWidget(list);
        dial->exec();

        for(auto ptr: history)
            delete ptr;
        delete list;
        delete dial_layout;
        delete dial;
    });
    buttons_box_layout->addWidget(btn_withdrawal_history);

    btn_deposit_history = new QPushButton(tr("Deposit History"));
    connect(btn_deposit_history, &QPushButton::clicked, this, [&]() {
        auto history = Landlord::fetchDepositHistory(landlord->id);

        auto list = new QListWidget();
        list->setSelectionMode(QAbstractItemView::NoSelection);
        std::unordered_map<std::string, std::string> title_value_map;
        for (auto entry: history) {
            title_value_map["Source"] = entry->source;
            title_value_map["Date"] = entry->date;
            title_value_map["Amount"] = std::to_string(entry->amount);

            auto widget = new ViewListItemLabels(title_value_map);

            auto list_item = new QListWidgetItem();
            list->addItem(list_item);
            list_item->setSizeHint(widget->size());
            list->setItemWidget(list_item, widget);
        }

        auto dial = new QDialog();
        auto dial_layout = new QGridLayout();
        dial->setLayout(dial_layout);

        dial->layout()->addWidget(list);
        dial->exec();

        for(auto ptr: history)
            delete ptr;
        delete list;
        delete dial_layout;
        delete dial;
    });
    buttons_box_layout->addWidget(btn_deposit_history);

    btn_delete = new QPushButton(tr("Delete"));
    connect(btn_delete, &QPushButton::clicked, this, [this]() {
        Landlord::remove(landlord->id);
        onRemove();
    });
    buttons_box_layout->addWidget(btn_delete);

    layout->addWidget(buttons_box, Qt::AlignRight);
}

ViewListLandlordItem::~ViewListLandlordItem() {
    delete landlord;

    delete btn_deposit_history;
    delete btn_withdrawal_history;
    delete btn_delete;

    delete buttons_box_layout;
    delete buttons_box;

    delete lbl_name;
    delete lbl_balance;

    delete layout;
}
