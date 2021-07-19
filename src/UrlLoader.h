#ifndef URL_LOADER_H
#define URL_LOADER_H

#include <QUrl>
#include <QWebEngineView>

#include <PXContentWidget.h>

class UrlLoader : public PXContentWidget {
Q_OBJECT
public:
    UrlLoader(const QUrl &base, const QUrl &contentUrl, bool loadHtmlFile = true);
    void back();
    void forward();
    void goHome();
    
signals:
    void urlChanged(const QUrl &url);

private:
    void loadHtmlToView(const QString &file);
    QUrl _baseUrl;
    bool _loadhtmlFile;
    QUrl _fullUrl;
    QWebEngineView *view;
};

#endif