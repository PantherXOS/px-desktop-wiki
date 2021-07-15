#ifndef SEARCH_ITEM_H
#define SEARCH_ITEM_H

#include <QString>
#include <QWidgetAction>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QUrl>
#include <QDebug>

#include "PXSeperator.h"

class SearchItem : public QListWidgetItem{
    QOBJECT_H
public:
    SearchItem(const QString &name, const QString &address, const QString &fullAddress);
    QUrl address();
    QWidget *widget();

private:
    QString _name;
    QUrl    _addr;
    QWidget *_widget;
    QLabel  _openLabel;
    QLabel  _nameLabel;
    QLabel  _addrLabel;
    QLabel  _iconLabel;
    PXSeperator _seperator;
};
#endif