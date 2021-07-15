#ifndef SEARCH_SCREEN_H
#define SEARCH_SCREEN_H

#include <QUrl>
#include <QProcess>

#include <PXContentWidget.h>
#include <QListWidget>

class SearchScreen : public PXContentWidget {
Q_OBJECT
public:
    SearchScreen(const QUrl &url);

private slots:
    void recollProcessHandler(int );
    void itemClickedHandler(QListWidgetItem *item);

signals:
    void searchItemClicked(const QUrl &url);

private:
    void clearList();
    QUrl      _url;
    QProcess *process;
    QListWidget *_listWidget;
};

#endif