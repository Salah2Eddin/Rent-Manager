#include "dialog_edit_property.h"


EditPropertyDialog::EditPropertyDialog(PropertyObj *prop, QWidget *parent) {
    property = prop;

    buttons_box = new QDialogButtonBox(Qt::Horizontal);

    btn_add = new QPushButton(tr("حفظ التعديلات"));
    connect(buttons_box, &QDialogButtonBox::accepted, this, &EditPropertyDialog::editBtnClicked);
    buttons_box->addButton(btn_add, QDialogButtonBox::AcceptRole);

    buttons_box->addButton(QDialogButtonBox::Cancel);
    buttons_box->buttons()[1]->setText("التراجع");
    connect(buttons_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    lbl_renter_name = new QLabel(QString::fromStdString("اسم المستأجر: " + property->renter_name));
    lbl_building_number = new QLabel(QString::fromStdString("رقم المبني: " + property->building_number));
    lbl_floor_number = new QLabel(QString::fromStdString("الدور: " + property->floor));

    field_rent = new LabeledTextField(nullptr, "الايجار:");
    field_rent->field->setValidator(new QIntValidator);
    field_rent->field->setText(QString::fromStdString(std::to_string(property->rent)));

    list_landlords = new QListWidget();
    list_landlords->setSelectionMode(QAbstractItemView::NoSelection);
    list_landlords->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    list_landlords->setMaximumHeight(200);

    landlords = Landlord::fetch();
    auto property_landlords = property->getLandlords();

    std::vector<std::pair<LandlordObj *, int>> checked_landlords;

    int j = 0;
    for (auto &landlord: landlords) {
        if (j < property_landlords.size() && (landlord->id == property_landlords[j].first->id)) {
            checked_landlords.emplace_back(landlord, property_landlords[j].second);
            j++;
        } else {
            checked_landlords.emplace_back(landlord, 0);
        }
    }

    for (auto [landlord, share]: checked_landlords) {
        auto landlord_widget = new LandlordListItem(landlord);

        auto list_item = new QListWidgetItem();
        list_item->setSizeHint(landlord_widget->size());

        if (share) {
            landlord_widget->check_box->setChecked(true);
            landlord_widget->share_field->setText(QString::fromStdString(std::to_string(share)));
        }

        list_landlords->addItem(list_item);
        list_landlords->setItemWidget(list_item, landlord_widget);
    }

    layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    layout->addWidget(lbl_renter_name);
    layout->addWidget(lbl_building_number);
    layout->addWidget(lbl_floor_number);
    layout->addWidget(field_rent);
    layout->addWidget(list_landlords);
    layout->addWidget(buttons_box);
}

void EditPropertyDialog::editBtnClicked() {
    int new_rent = field_rent->field->text().toInt();

    std::vector<std::pair<LandlordObj *, int>> selected_landlords;
    double sum = 0;
    for (int i = 0; i < list_landlords->count(); ++i) {
        auto *landlord_widget = dynamic_cast<LandlordListItem *>(list_landlords->itemWidget(list_landlords->item(i)));
        if (landlord_widget->check_box->checkState()) {
            selected_landlords.emplace_back(landlord_widget->landlord, landlord_widget->share_field->text().toInt());
            sum += selected_landlords.back().second;
        }
    }

    if (new_rent <= 0) {
        MessagesHelper::showMessageBox("تعديل مرفوض", "قيمة الايجار الجديدة غير صالحة");
        return;
    } else if (new_rent < 0) {
        MessagesHelper::showMessageBox("الايجار غير صالح", "قيمة الايجار غير صالحة");
        return;
    } else if (selected_landlords.empty()) {
        MessagesHelper::showMessageBox("تعديل مرفوض", "لم تختار ملاك لهذا العقار");
        return;
    } else if (sum != 100) {
        MessagesHelper::showMessageBox("تعديل مرفوض", "مجموع النسب لا يساوي 100%");
        return;
    }

    property->clearLandlords();
    property->edit(new_rent);
    for (auto [landlord, share]: selected_landlords) {
        property->addLandlord(landlord->id, share);
    }

    QDialog::close();
}

EditPropertyDialog::~EditPropertyDialog() {
    delete layout;

    delete btn_add;
    delete buttons_box;

    delete lbl_building_number;
    delete lbl_floor_number;
    delete lbl_renter_name;
    delete field_rent;

    delete list_landlords;
}
