#pragma once

#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <string>
#include <functional>

#include "widget_list_item_labels.h"

#include "landlord.h"
#include "landlord_obj.h"

class ViewListLandlordItem : public QWidget {
public:
    explicit ViewListLandlordItem(LandlordObj *l);

    ~ViewListLandlordItem() override;

    LandlordObj *landlord;

    std::function<void()> onRemove;

private:

    QHBoxLayout *layout;
    QLabel *lbl_name, *lbl_balance;
    QPushButton *btn_delete, *btn_deposit_history, *btn_withdrawal_history;

    QWidget *buttons_box;

    QVBoxLayout *buttons_box_layout;
};
