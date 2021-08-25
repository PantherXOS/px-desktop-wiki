#ifndef URL_LOADER_H
#define URL_LOADER_H

#include <QUrl>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QDesktopServices>

#include <PXContentWidget.h>

class UrlLoaderPage : public QWebEnginePage{
    Q_OBJECT
    public:
    bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool main) {
        // Check if remote url --> open in default browser
        if (type == QWebEnginePage::NavigationTypeLinkClicked && !url.isLocalFile()) {
            QDesktopServices::openUrl(url);
            return false;
        }
        return true;
    };
};

class UrlLoader : public PXContentWidget {
Q_OBJECT
public:
    UrlLoader(const QUrl &base, const QUrl &contentUrl);
    void back();
    void forward();
    void goHome();
    
signals:
    void urlChanged(const QUrl &url);

private:
    void loadHtmlToView(const QString &file);
    QUrl _baseUrl;
    QUrl _fullUrl;
    QWebEngineView *view;
};

#endif