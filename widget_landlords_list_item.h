#pragma once

#include <QListWidgetItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QIntValidator>
#include <QLabel>
#include <QHBoxLayout>

#include <string>

#include "landlord_obj.h"

class LandlordListItem : public QWidget {
public:
    LandlordObj *landlord;
    std::string *name;
    int *id;

    QCheckBox *check_box;
    QLabel *name_lbl;
    QLineEdit *share_field;
    QHBoxLayout *layout;

    int getShare() const;

    explicit LandlordListItem(LandlordObj *landlord, QWidget *parent = nullptr);
    ~LandlordListItem();

private slots:
    void checkBoxChecked(bool state) const;
};