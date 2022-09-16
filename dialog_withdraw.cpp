#include "dialog_withdraw.h"

WithdrawDialog::WithdrawDialog(QWidget *parent) {
    btn_withdraw = new QPushButton(tr("Withdraw"));

    buttons_box = new QDialogButtonBox(Qt::Horizontal);
    buttons_box->addButton(btn_withdraw, QDialogButtonBox::AcceptRole);

    buttons_box->addButton(QDialogButtonBox::Cancel);
    connect(buttons_box, &QDialogButtonBox::accepted, this, &WithdrawDialog::withdrawBtnClicked);
    connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);


    landlords = Landlord::fetch();

    combobox_landlords = new QComboBox();
    for (auto landlord: landlords) {
        combobox_landlords->addItem(QString::fromStdString(landlord->name));
    }
    connect(combobox_landlords, &QComboBox::currentIndexChanged, this, &WithdrawDialog::selectedLandlordChanged);

    if (landlords.empty())
        lbl_balance = new QLabel(QString::fromStdString("Balance: "));
    else
        lbl_balance = new QLabel(QString::fromStdString("Balance: " + std::to_string(landlords[0]->balance)));

    field_amount = new LabeledTextField(nullptr, "Amount: ");
    field_amount->field->setValidator(new QIntValidator);

    layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    layout->addWidget(combobox_landlords);
    layout->addWidget(lbl_balance);
    layout->addWidget(field_amount);
    layout->addWidget(buttons_box);
}

void WithdrawDialog::withdrawBtnClicked() {
    int selected_idx = combobox_landlords->currentIndex();
    int amount = field_amount->field->text().toInt();

    auto landlord = landlords[selected_idx];
    if (amount <= 0) {
        MessagesHelper::showMessageBox("Invalid Amount", "The selected amount can't be withdrawn");
        return;
    }

    landlord->withdraw(amount);
    QDialog::close();
}

void WithdrawDialog::selectedLandlordChanged(int idx) {
    lbl_balance->setText(QString::fromStdString("Balance: " + std::to_string(landlords[idx]->balance)));
}

WithdrawDialog::~WithdrawDialog() {
    delete layout;

    delete btn_withdraw;
    delete buttons_box;

    delete combobox_landlords;
    delete lbl_balance;
    delete field_amount;

    for (auto ptr: landlords)
        delete ptr;
}
