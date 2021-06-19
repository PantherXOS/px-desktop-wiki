#include "UrlLoader.h"
#include "Logger.h"
#include <QFile>
#include <QWebEngineHistory>

// void UrlLoader::loadHtmlToView(const QUrl &base, const QUrl &url) {
//     qDebug() << base << url;
// #if URL_APPROACH
//     view->setUrl(url);
// #else
//     QFile *f;
//     if(url.isLocalFile())
//         f = new QFile(base.toLocalFile() + url.toLocalFile());
//     else 
//         f = new QFile(url.toString());
//     qDebug() << "filename" << f->fileName();
//     if (!f->open(QFile::ReadOnly | QFile::Text)) {
//         GLOG_WRN("Could not read file \"" + base.toLocalFile().toStdString() + url.toLocalFile().toStdString() + "\"");
//         return;
//     }    
//     QTextStream in(f);
//     view->setHtml(in.readAll().toUtf8(), base);    
//     f->close();
//     delete f;
// #endif
// }

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
    PXContentWidget(getFullUrl(baseUrl, contentUrl).toString()) ,
    _baseUrl(baseUrl) {
        auto url = getFullUrl(_baseUrl, contentUrl);
        GLOG_INF("Base URL: " + _baseUrl.toString().toStdString() + ", Content URL: " + contentUrl.toString().toStdString());
        view= new QWebEngineView(this);
        
        connect(view, &QWebEngineView::urlChanged,[&](const QUrl &url){
            if(url.isLocalFile()) {
                GLOG_INF(" >  " + url.toString().toStdString());
            }
            if(view->url().fileName().isEmpty()) {
                loadHtmlToView(_baseUrl.toLocalFile() + url.toLocalFile() + "index.html");
            }
            emit urlChanged(url);
        });

        loadHtmlToView(url.toLocalFile());
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
