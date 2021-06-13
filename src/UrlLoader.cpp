#include <QWebEngineView>

#include "Logger.h"
#include "UrlLoader.h"

UrlLoader::UrlLoader(const QUrl &url) : PXContentWidget(url.toString()){
    GLOG_INF("URL Loader: " + url.toString().toStdString());
    auto view= new QWebEngineView();
    view->load(url);
    setWidget(view);
}
