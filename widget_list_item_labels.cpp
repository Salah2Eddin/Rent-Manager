#include "widget_list_item_labels.h"

ViewListItemLabels::ViewListItemLabels(const std::unordered_map<std::string, std::string>& title_value_map) {
    layout = new QVBoxLayout();
    setMaximumHeight(title_value_map.size()*20);

    setLayout(layout);

    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    for (auto [title, value]: title_value_map) {
        std::string txt = title;
        txt += ": ";
        txt += value;
        auto lbl = new QLabel(QString::fromStdString(txt));
        /*lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);*/
        //lbl->setFixedWidth(this->width());
        // lbl->setWordWrap(true);

        labels.push_back(lbl);
        layout->addWidget(lbl);
    }
}

ViewListItemLabels::~ViewListItemLabels() {
    delete layout;
    for(auto ptr: labels)
        delete ptr;
}
