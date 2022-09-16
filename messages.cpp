//
// Created by Salah on 06/09/2022.
//
#include "messages.h"

int MessagesHelper::showMessageBox(const std::string &title, const std::string &message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(QString::fromStdString(title));
    msgBox.setText(QString::fromStdString(message));

    msgBox.show();
    return msgBox.exec();
}

