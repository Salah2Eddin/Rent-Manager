#include "widget_view_list_property.h"

ViewListPropertyItem::ViewListPropertyItem(PropertyObj *p) {
    property = p;

    layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignLeft);
    layout->setSpacing(50);
    setMaximumHeight(120);
    this->setLayout(layout);

    lbl_name = new QLabel(QString::fromStdString("اسم المستأجر\n" + property->renter_name));
    layout->addWidget(lbl_name);

    lbl_building_number = new QLabel(QString::fromStdString("رقم المبني\n" + property->building_number));
    layout->addWidget(lbl_building_number);

    lbl_floor = new QLabel(QString::fromStdString("الدور\n" + property->floor));
    layout->addWidget(lbl_floor);

    lbl_rent = new QLabel(QString::fromStdString("قيمة الايجار\n" + std::to_string(property->rent)));
    layout->addWidget(lbl_rent);

    buttons_box = new QWidget();
    buttons_box_layout = new QVBoxLayout();
    buttons_box_layout->setAlignment(Qt::AlignTop);
    buttons_box->setMaximumWidth(300);

    btn_payment_history = new QPushButton(tr("المدفوعات"));
    connect(btn_payment_history, &QPushButton::clicked, this, [this]() {
        auto history = Property::fetchPaymentHistory(property->id);

        auto list = new QListWidget();
        list->setSelectionMode(QAbstractItemView::NoSelection);
        std::unordered_map<std::string, std::string> title_value_map;
        for (auto entry: history) {
            title_value_map["التاريخ"] = entry->date;
            title_value_map["الشهر"] = std::to_string(entry->month);

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
    buttons_box_layout->addWidget(btn_payment_history);

    btn_edit = new QPushButton(tr("تعديل"));
    connect(btn_edit, &QPushButton::clicked, this, &ViewListPropertyItem::edit);
    buttons_box_layout->addWidget(btn_edit);

    btn_delete = new QPushButton(tr("حذف"));
    connect(btn_delete, &QPushButton::clicked, this, [this]() {
        Property::remove(property->id);
        onRemove();
    });
    buttons_box_layout->addWidget(btn_delete);

    buttons_box->setLayout(buttons_box_layout);
    layout->addWidget(buttons_box, Qt::AlignRight);

}

ViewListPropertyItem::~ViewListPropertyItem() {
    delete property;

    delete btn_edit;
    delete btn_delete;
    delete btn_payment_history;

    delete buttons_box_layout;
    delete buttons_box;

    delete lbl_name;
    delete lbl_building_number;
    delete lbl_floor;
    delete lbl_rent;

    delete layout;
}

void ViewListPropertyItem::edit() {
    auto dial = new EditPropertyDialog(property);

    dial->exec();

    delete dial;
    onEdit();
}
