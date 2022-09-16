#include "dialog_add_property.h"


AddPropertyDialog::AddPropertyDialog(QWidget *parent) {
    btn_add = new QPushButton(tr("Add"));

    buttons_box = new QDialogButtonBox(Qt::Horizontal);
    buttons_box->addButton(btn_add, QDialogButtonBox::AcceptRole);

    buttons_box->addButton(QDialogButtonBox::Cancel);
    connect(buttons_box, &QDialogButtonBox::accepted, this, &AddPropertyDialog::addBtnClicked);
    connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    field_renter_name = new LabeledTextField(nullptr, "Renter name:");

    field_building_number = new LabeledTextField(nullptr, "Building Number:");

    field_floor_number = new LabeledTextField(nullptr, "Floor Number:");

    field_rent = new LabeledTextField(nullptr, "Rent:");
    field_rent->field->setValidator(new QIntValidator);

    list_landlords = new QListWidget();
    list_landlords->setSelectionMode(QAbstractItemView::NoSelection);
    list_landlords->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    list_landlords->setMaximumHeight(200);

    landlords = Landlord::fetch();

    for (auto *landlord: landlords) {
        auto landlord_widget = new LandlordListItem(landlord);

        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(landlord_widget->size());

        list_landlords->addItem(list_item);
        list_landlords->setItemWidget(list_item, landlord_widget);
    }

    layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);

    layout->addWidget(field_renter_name);
    layout->addWidget(field_building_number);
    layout->addWidget(field_floor_number);
    layout->addWidget(field_rent);
    layout->addWidget(list_landlords);
    layout->addWidget(buttons_box);
}

void AddPropertyDialog::addBtnClicked() {
    std::string name = field_renter_name->field->text().toStdString();
    std::string b_num = field_building_number->field->text().toStdString();
    std::string f_num = field_floor_number->field->text().toStdString();
    double r = field_rent->field->text().toDouble();

    std::vector<std::pair<LandlordObj *, int>> selected_landlords;
    double sum = 0;
    for (int i = 0; i < list_landlords->count(); ++i) {
        auto *landlord_widget = dynamic_cast<LandlordListItem *>(list_landlords->itemWidget(list_landlords->item(i)));
        if (landlord_widget->check_box->checkState()) {
            selected_landlords.emplace_back(landlord_widget->landlord, landlord_widget->share_field->text().toInt());
            sum += selected_landlords.back().second;
        }
    }

    if (name.empty() || b_num.empty() || f_num.empty() || r == 0) {
        MessagesHelper::showMessageBox("Missing data", "You didn't fill all fields");
        return;
    } else if (selected_landlords.empty()) {
        MessagesHelper::showMessageBox("Missing landlords", "You didn't select any landlords for this property");
        return;
    } else if (sum != 100) {
        MessagesHelper::showMessageBox("Invalid shares", "Total sum of shares is not 100%");
        return;
    }

    Property::createProperty(b_num, f_num, name, r);
    PropertyObj *property = Property::fetch().back();
    for (auto [landlord, share]: selected_landlords) {
        property->addLandlord(landlord->id, share);
    }

    QDialog::close();
}

AddPropertyDialog::~AddPropertyDialog() {
    delete layout;

    delete btn_add;
    delete buttons_box;

    delete field_building_number;
    delete field_floor_number;
    delete field_renter_name;
    delete field_rent;

    delete list_landlords;
}
