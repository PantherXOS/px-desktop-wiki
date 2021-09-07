#ifndef SEARCH_SCREEN_H
#define SEARCH_SCREEN_H

#include <QUrl>
#include <QProcess>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include <PXContentWidget.h>
#include <PXProgressIndicator.h>

class SearchScreen : public PXContentWidget {
Q_OBJECT
public:
    enum WIKI_DB_STATE{
        NONE,
        NOT_EXIST,
        EXIST,
    };

    SearchScreen(const QUrl &url);
    ~SearchScreen(){
        _process->waitForFinished();
    }

private slots:
    void recollProcessHandler(int );
    void itemClickedHandler(QListWidgetItem *item);

signals:
    void searchItemClicked(const QUrl &url);
    void wikiDBchecked(WIKI_DB_STATE state);

private:
    void clearList();
    QUrl                 _url;
    QString              _keyword;
    QProcess            *_process;
    QListWidget         *_listWidget;
    QLineEdit           *_searchTextEdit;
    QLabel              *_messageLabel;
    QPushButton         *_indexButton;
    PXProgressIndicator *_progressIndicator;
    WIKI_DB_STATE _wikiDbState = WIKI_DB_STATE::NONE;
};

#endif