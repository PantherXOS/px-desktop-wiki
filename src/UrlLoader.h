#ifndef URL_LOADER_H
#define URL_LOADER_H

#include <QUrl>
#include <PXContentWidget.h>

class UrlLoader : public PXContentWidget {
public:
    UrlLoader(const QUrl &url);

private:
};

#endif