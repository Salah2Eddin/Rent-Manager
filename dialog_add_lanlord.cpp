#include "dialog_add_lanlord.h"
#include "landlord.h"

AddLandlordDialog::AddLandlordDialog(QWidget *parent) : QDialog(parent) {
    buttons_box = new QDialogButtonBox(Qt::Horizontal);

    btn_add = new QPushButton(tr("اضافة"));
    buttons_box->addButton(btn_add, QDialogButtonBox::AcceptRole);
    connect(buttons_box, &QDialogButtonBox::accepted, this, &AddLandlordDialog::addBtnClicked);

    buttons_box->addButton(QDialogButtonBox::Cancel);
    buttons_box->buttons()[1]->setText("التراجع");
    connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    field_name = new LabeledTextField(nullptr, "الاسم:");

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
        MessagesHelper::showMessageBox("البيانات ناقصة", "لم تملأ جميع البيانات");
        return;
    }
}

AddLandlordDialog::~AddLandlordDialog() {
    delete btn_add;
    delete buttons_box;
    delete field_name;
}