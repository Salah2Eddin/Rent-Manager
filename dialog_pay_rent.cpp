#include "dialog_pay_rent.h"

PayRentDialog::PayRentDialog(QWidget *parent) {
    btn_pay = new QPushButton(tr("دفع"));

    buttons_box = new QDialogButtonBox(Qt::Horizontal);
    buttons_box->addButton(btn_pay, QDialogButtonBox::AcceptRole);
    connect(buttons_box, &QDialogButtonBox::accepted, this, &PayRentDialog::payBtnClicked);

    buttons_box->addButton(QDialogButtonBox::Cancel);
    buttons_box->buttons()[1]->setText("التراجع");
    connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);


    properties = Property::fetch();

    combobox_properties = new QComboBox();
    // combobox_properties->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    for (auto prop: properties) {
        combobox_properties->addItem(
                QString::fromStdString(prop->renter_name + " | " + prop->building_number + " | " + prop->floor));
    }
    connect(combobox_properties, &QComboBox::currentIndexChanged, this, &PayRentDialog::selectedPropertyChanged);

    lbl_property_title = new QLabel(tr("العقار: "));
    if (properties.empty())
        lbl_amount = new QLabel(QString::fromStdString("القيمة: "));
    else
        lbl_amount = new QLabel(QString::fromStdString("القيمة: " + std::to_string(properties[0]->rent)));

    lbl_month_title = new QLabel(tr("الشهر: "));
    combobox_months = new QComboBox();
    for (int i = 1; i <= 12; ++i) {
        combobox_months->addItem(QString::fromStdString(std::to_string(i)));
    }

    layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    // layout->setDirection(QBoxLayout::RightToLeft);

    layout->addWidget(lbl_property_title);
    layout->addWidget(combobox_properties);
    layout->addWidget(lbl_amount);
    layout->addWidget(lbl_month_title);
    layout->addWidget(combobox_months);
    layout->addWidget(buttons_box);
}

void PayRentDialog::payBtnClicked() {
    int selected_idx = combobox_properties->currentIndex();
    int selected_month = combobox_months->currentIndex()+1;
    properties[selected_idx]->payRent(selected_month);
    QDialog::close();
}

PayRentDialog::~PayRentDialog() {
    delete layout;
    delete btn_pay;
    delete buttons_box;
    delete combobox_properties;
    delete lbl_amount;

    for(auto ptr: properties)
        delete ptr;
}

void PayRentDialog::selectedPropertyChanged(int idx) {
    lbl_amount->setText(QString::fromStdString("القيمة: " + std::to_string(properties[idx]->rent)));
}
