#include "widget_lbl_textfield.h"

LabeledTextField::LabeledTextField(QWidget *parent, std::string title) : QWidget(parent) {
    lbl = new QLabel(QString::fromStdString(title));
    field = new QLineEdit();

    this->setMinimumSize(300, 20);

    auto *layout = new QHBoxLayout();
    this->setLayout(layout);
    // layout->setDirection(QBoxLayout::RightToLeft);

    layout->addWidget(lbl);
    layout->addWidget(field);
}

LabeledTextField::~LabeledTextField() {
    delete lbl;
    delete field;
}
