#pragma once

#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <string>
#include <functional>

#include "widget_list_item_labels.h"

#include "dialog_edit_property.h"

#include "property.h"
#include "property_obj.h"

class ViewListPropertyItem : public QWidget {
public:
    explicit ViewListPropertyItem(PropertyObj *p);
    ~ViewListPropertyItem() override;

    PropertyObj *property;
    std::function<void()> onRemove, onEdit;

    void edit();

private:

    QHBoxLayout *layout;
    QLabel *lbl_name, *lbl_building_number, *lbl_floor, *lbl_rent;
    QPushButton *btn_edit, *btn_delete, *btn_payment_history;

    QWidget *buttons_box;

    QVBoxLayout *buttons_box_layout;
};
