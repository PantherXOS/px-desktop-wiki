#include "UrlLoader.h"
#include "Logger.h"
#include <QFile>
#include <QWebEngineHistory>

inline QUrl getFullUrl(const QUrl &baseUrl, const QUrl &contentUrl) {
    return QUrl(baseUrl.toString() + contentUrl.toString());
} 

void UrlLoader::loadHtmlToView(const QString &file) {
    QFile f(file);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        GLOG_WRN("Could not read file \"" + file.toStdString() + "\"");
        return;
    }    
    QTextStream in(&f);
    view->setHtml(in.readAll().toUtf8(), _baseUrl);
}

UrlLoader::UrlLoader(const QUrl &baseUrl, const QUrl &contentUrl, bool loadHtmlFile) : 
    PXContentWidget(getFullUrl(baseUrl, contentUrl).toLocalFile()) ,
    _baseUrl(baseUrl),
    _loadhtmlFile(loadHtmlFile),
    _fullUrl(getFullUrl(baseUrl, contentUrl)) {
        GLOG_INF("Base URL: " + _baseUrl.toString().toStdString() + ", Content URL: " + contentUrl.toString().toStdString());
        view = new QWebEngineView(this);
        
        connect(view, &QWebEngineView::urlChanged,[&](const QUrl &url){
            if(url.isLocalFile()) {
                GLOG_INF(" >  " + url.toString().toStdString());
            }
            if(view->url().fileName().isEmpty() && url.toString().compare(_baseUrl.toString())) {
                loadHtmlToView(_baseUrl.toLocalFile() + url.toLocalFile() + "index.html");
            }
            emit urlChanged(url);
        });
        goHome();
        setWidgetResizable(true);
        setWidget(view);
}

void UrlLoader::back(){
    if(view->history()->backItem().url().fileName() == "index.html") {
        view->history()->back();
    } else {
        view->history()->goToItem(view->history()->itemAt(view->history()->items().size()-3));
    }
}

void UrlLoader::forward(){
    view->history()->forward();
}

void UrlLoader::goHome(){
    view->history()->clear();
    loadHtmlToView(_fullUrl.toLocalFile());
}