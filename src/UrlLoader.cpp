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

UrlLoader::UrlLoader(const QUrl &baseUrl, const QUrl &contentUrl) : 
    PXContentWidget(getFullUrl(baseUrl, contentUrl).toLocalFile()) ,
    _baseUrl(baseUrl),
    _fullUrl(getFullUrl(baseUrl, contentUrl)) {
        GLOG_INF("Base URL: " + _baseUrl.toString().toStdString() + ", Content URL: " + contentUrl.toString().toStdString());
        view = new QWebEngineView(this);
        
        auto page = new UrlLoaderPage();
        view->setPage(page);

        connect(view, &QWebEngineView::urlChanged,[&](const QUrl &url){
            if(url.isLocalFile()) {
                GLOG_INF(" >  " + url.toString().toStdString());
                if(view->url().fileName().isEmpty() && (url.toString().compare(_baseUrl.toString()) < 0)) {
                    view->history()->back();
                    view->setUrl(QUrl::fromLocalFile(_baseUrl.toLocalFile() + url.toLocalFile() + "index.html"));
                    emit urlChanged(_baseUrl.toLocalFile() + url.toLocalFile() + "index.html");
                }
            }
        });
        goHome();
        setWidgetResizable(true);
        setWidget(view);
}

void UrlLoader::back(){
    if(view->history()->backItem().url().fileName() == "index.html") {
        view->history()->back();
    } else {
        view->history()->goToItem(view->history()->itemAt(0));
    }
}

void UrlLoader::forward(){
    view->history()->forward();
}

void UrlLoader::goHome(){
    view->setUrl(_fullUrl);
}