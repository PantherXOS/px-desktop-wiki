cmake_minimum_required(VERSION 3.10)
project(px-desktop-wiki)

set(CMAKE_CXX_STANDARD 14)

######## QT-qmake settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5DBus REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets WebEngineWidgets)
########

include_directories(src)

add_executable(px-desktop-wiki
        src/main.cpp
        src/Logger.cpp src/Logger.h
        src/MainWindow.cpp src/MainWindow.h
        src/UrlLoader.cpp src/UrlLoader.h
        src/SearchScreen.cpp src/SearchScreen.h
        src/SearchItem.cpp src/SearchItem.h
        )

target_link_libraries(px-desktop-wiki Qt5::Core Qt5::Widgets Qt5::Gui Qt5::WebEngineWidgets px-gui-library)
install(TARGETS px-desktop-wiki DESTINATION bin)
install(FILES   desktop/px-desktop-wiki.desktop
                DESTINATION share/applications)
