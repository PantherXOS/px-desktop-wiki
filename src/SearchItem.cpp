#include "SearchItem.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>

#include "Logger.h"

#define SEARCH_ICON_SIZE    32
SearchItem::SearchItem(const QString &name, const QString &address, const QString &fullAddress) : 
    _name(name), 
    _addr(QUrl(address)){
        GLOG_INF("Search item created [" + name.toStdString() + ", " + address.toStdString() + ", " + fullAddress.toStdString() + "]");
        QIcon _icon(QIcon::fromTheme("text-html"));
        QPixmap pixmap = _icon.pixmap(QSize(SEARCH_ICON_SIZE,SEARCH_ICON_SIZE), QIcon::Normal, QIcon::On);
        _iconLabel.setPixmap(pixmap);
        _iconLabel.setFixedSize(QSize(SEARCH_ICON_SIZE,SEARCH_ICON_SIZE));

        _nameLabel.setText(_addr.toString().remove(_addr.fileName()).remove("/"));
        _nameLabel.setStyleSheet("QLabel {font-weight: bold; color: blue}");
        
        _addrLabel.setText(fullAddress);
        _addrLabel.setStyleSheet("QLabel {color: gray}");
        _addrLabel.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        auto layout = new QHBoxLayout;
        layout->addWidget(&_iconLabel);
        layout->addWidget(&_nameLabel);
        layout->addWidget(&_addrLabel);

        auto mainLayout = new QVBoxLayout;
        mainLayout->addLayout(layout);
        mainLayout->addWidget(&_seperator);
        _widget = new QWidget();
        _widget->setLayout(mainLayout);
}

QUrl SearchItem::address(){
    return _addr;
}
    
QWidget *SearchItem::widget(){
    return _widget;
}