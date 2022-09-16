#pragma once

#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <string>

class ViewListItemLabels : public QWidget {
public:
    explicit ViewListItemLabels(const std::unordered_map<std::string, std::string>& title_value_map);
    ~ViewListItemLabels();

private:
    QVBoxLayout *layout;
    std::vector<QLabel*> labels;
};
