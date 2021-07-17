#include "SearchItem.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>
#include <QGuiApplication>

#include "Logger.h"

#define SEARCH_ICON_SIZE    32
#define WIDGET_SIZE         72

SearchItem::SearchItem(const QString &name, const QString &address, const QString &keyword, const QString &abstract) : 
    _name(name), 
    _addr(QUrl(address)){

        auto _abstract = abstract;
        _abstract.replace(QRegExp("\\b" + keyword + "\\b"),
            "<span style=\"background-color:yellow\">" + keyword + "</span>");

        GLOG_INF("Search item created [" + name.toStdString() + ", " + address.toStdString() + "]");
        QIcon _icon(QIcon::fromTheme("text-html"));
        QPixmap pixmap = _icon.pixmap(QSize(SEARCH_ICON_SIZE,SEARCH_ICON_SIZE), QIcon::Normal, QIcon::On);
        _iconLabel.setPixmap(pixmap);
        _iconLabel.setFixedSize(QSize(SEARCH_ICON_SIZE,SEARCH_ICON_SIZE));
        _iconLabel.setStyleSheet("QLabel {background-color: transparent}");

        _nameLabel.setText(_addr.toString().remove(_addr.fileName()).remove("/"));
        _nameLabel.setStyleSheet("QLabel {font-weight: bold; background-color: transparent}");

        _abstractLabel.setText(_abstract);
        _abstractLabel.setStyleSheet("QLabel {background-color: transparent}");
        _abstractLabel.setWordWrap(true);

        auto rLayout = new QVBoxLayout;
        rLayout->addWidget(&_nameLabel);
        rLayout->addWidget(&_abstractLabel);
        rLayout->setAlignment(Qt::AlignTop);

        auto layout = new QHBoxLayout;
        layout->addWidget(&_iconLabel);
        layout->addLayout(rLayout);

        auto mainLayout = new QVBoxLayout;
        mainLayout->addLayout(layout);
        mainLayout->addWidget(&_seperator);
        mainLayout->setContentsMargins(10,5,10,0);
        
        _widget = new QWidget();
        _widget->setLayout(mainLayout);
        auto widgetStyleSheet = "QWidget:hover{background-color: %1} Qwidget{background-color: transparent}";
        auto pal = QGuiApplication::palette();
        auto bgcolor =  pal.color(QPalette::Normal, QPalette::Highlight);
        _widget->setStyleSheet(QString::fromLatin1(widgetStyleSheet).arg(bgcolor.name()));
        _widget->setFixedHeight(WIDGET_SIZE);
}

QUrl SearchItem::address(){
    return _addr;
}
    
QWidget *SearchItem::widget(){
    return _widget;
}