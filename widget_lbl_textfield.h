#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

class LabeledTextField : public QWidget {
    // Q_OBJECT
public:
    explicit LabeledTextField(QWidget *parent = nullptr, std::string title = "Title");

    ~LabeledTextField();

    QLabel *lbl;
    QLineEdit *field;
};