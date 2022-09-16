#include "widget_landlords_list_item.h"

LandlordListItem::LandlordListItem(LandlordObj *landlord, QWidget *parent):landlord(landlord) {
    id = new int(landlord->id);
    name = new std::string(landlord->name);

    setMaximumSize(200, 50);
    setMinimumSize(200, 50);

    check_box = new QCheckBox();
    connect(check_box, &QCheckBox::stateChanged, this, &LandlordListItem::checkBoxChecked);

    name_lbl = new QLabel(QString::fromStdString(*name));

    share_field = new QLineEdit();
    share_field->setValidator(new QIntValidator);
    share_field->setEnabled(false);

    layout = new QHBoxLayout();
    this->setLayout(layout);

    layout->addWidget(check_box);
    layout->addWidget(name_lbl);
    layout->addWidget(share_field, Qt::AlignRight);
}

LandlordListItem::~LandlordListItem() {
    delete landlord;
    delete name;
    delete id;

    delete check_box;
    delete name_lbl;
    delete share_field;
    delete layout;
}

int LandlordListItem::getShare() const {
    return share_field->text().toInt();
}

void LandlordListItem::checkBoxChecked(bool state) const {
    share_field->setEnabled(state);
}
