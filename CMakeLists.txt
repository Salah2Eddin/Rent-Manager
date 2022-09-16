cmake_minimum_required(VERSION 3.23.2)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH C:/Qt/6.3.0/mingw_64)

project(rent_manager)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Python REQUIRED Development)


set(GUI_TYPE WIN32)
set(QT_HEADERS main_window.h dialog_add_lanlord.h widget_lbl_textfield.h dialog_add_property.h dialog_pay_rent.h dialog_withdraw.h widget_landlords_list_item.h widget_view_list_landlord.h widget_list_item_labels.h)
set(QT_SOURCES main_window.cpp dialog_add_lanlord.cpp widget_lbl_textfield.cpp dialog_add_property.cpp dialog_pay_rent.cpp dialog_withdraw.cpp widget_landlords_list_item.cpp widget_view_list_landlord.cpp widget_view_list_property.cpp dialog_edit_property.cpp widget_list_item_labels.cpp)
set(SOURCE_FILES main.cpp landlord.cpp property.cpp db_helper.cpp messages.cpp landlord_obj.cpp property_obj.cpp)
set(HEADER_FILES landlord.h property.h db_helper.h messages.h landlord_obj.h property_obj.h landlord.fwd.h landlord_obj.fwd.h property.fwd.h property_obj.fwd.h widget_view_list_property.h dialog_edit_property.h)

add_executable(rent_manager WIN32 ${QT_HEADERS} ${QT_SOURCES} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(rent_manager Qt::Widgets sqlite3#[[ Python::Python]])
