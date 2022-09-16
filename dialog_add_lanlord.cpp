#include "dialog_add_lanlord.h"
#include "landlord.h"

AddLandlordDialog::AddLandlordDialog(QWidget *parent) : QDialog(parent) {
    btn_add = new QPushButton(tr("Add"));

    buttons_box = new QDialogButtonBox(Qt::Horizontal);
    buttons_box->addButton(btn_add, QDialogButtonBox::AcceptRole);

    buttons_box->addButton(QDialogButtonBox::Cancel);
    connect(buttons_box, &QDialogButtonBox::accepted, this, &AddLandlordDialog::addBtnClicked);
    connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    field_name = new LabeledTextField(nullptr, "Name:");

    layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    layout->addWidget(field_name);
    layout->addWidget(buttons_box);
}

void AddLandlordDialog::addBtnClicked() {
    std::string name = field_name->field->text().toStdString();
    if (!name.empty()) {
        Landlord::createLandlord(name);
        QDialog::close();
    }
    else {
        MessagesHelper::showMessageBox("Missing data", "You didn't fill all fields");
        return;
    }
}

AddLandlordDialog::~AddLandlordDialog() {
    delete btn_add;
    delete buttons_box;
    delete field_name;
}