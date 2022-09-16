#pragma once

#include <QMessageBox>
#include <string>

struct MessagesHelper {
    static int showMessageBox(const std::string &title, const std::string &message);
};