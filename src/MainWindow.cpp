#include "MainWindow.h"

#include <QAction>
#include <QDir>
#include <QMap>
#include <QStandardPaths>

#include "Logger.h"
#include "SearchScreen.h"

#define OVERVEIW_TITLE              "Overview"
#define MAIN_PAGE_TITLE             "Main page"
#define TABL_OF_CONTENT_TITLE       "Table of Contents"
#define GETTING_INVOLVED_TITLE      "Getting Involved"
#define SEARCH_TITLE                "Search"
#define INTERACTION_TITLE           "Interaction"
#define HELP_TITLE                  "Help"
#define CONTRIBUTING_TITLE          "Contributing"

QMap<QString, QString> leftsideItemUrl = {{MAIN_PAGE_TITLE, "index.html"},
                                          {TABL_OF_CONTENT_TITLE, "Table-of-contents/index.html"},
                                          {GETTING_INVOLVED_TITLE, "Getting-involved/index.html"},
                                          {SEARCH_TITLE, "Search/index.html"},
                                          {HELP_TITLE, "Help/index.html"},
                                          {CONTRIBUTING_TITLE, "Contributing/index.html"}};

MainWindow::MainWindow(const QUrl &mainUrl, const QString &defaultPage, QWidget *parent) :
        PXMainWindow("PantherX Wiki: ", QIcon::fromTheme("panther"), "", parent),
        _mainUrl(mainUrl),
        _defaultPage(defaultPage){
    auto installedWikiPath = getInstalledWikiPath();
    if(_mainUrl.isEmpty()) {
        if(installedWikiPath.isEmpty()) {
            GLOG_ERR("The PantherX Wiki html directory not found!!!");
            exit(1);
        } else {
            _mainUrl = installedWikiPath;
        }
    }
    GLOG_INF("=> Base URL: " + _mainUrl.toString().toStdString());
    buildSidebar();
    topBar()->searchBox()->setDisabled(true);
    topBar()->settingsButtonAction()->setVisible(false);
    topBar()->helpButtonAction()->setVisible(false);
}

void MainWindow::backButtonPressed(){
    auto searchScreen = qobject_cast<SearchScreen*>(((PXSideBarItem *)(sideBarList()->currentItem()))->getView());
    if(searchScreen)
        searchScreenLoader->back();
    else
        ((UrlLoader *)(((PXSideBarItem *)(sideBarList()->currentItem()))->getView()))->back();
}

void MainWindow::forwardButtonPressed(){
    auto searchScreen = qobject_cast<SearchScreen*>(((PXSideBarItem *)(sideBarList()->currentItem()))->getView());
    if(searchScreen)
        searchScreenLoader->forward();
    else
        ((UrlLoader *)(((PXSideBarItem *)(sideBarList()->currentItem()))->getView()))->forward();
}

QUrl MainWindow::getInstalledWikiPath(){
    auto list = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for(auto const &l: list){
        GLOG_INF("Finding wiki in: \"" + l.toStdString() +"\" ...");
        QString wikiPath = l + "/px-wiki"; 
        if(QDir(wikiPath).exists()){
            wikiPath += "/content/";
            GLOG_INF("PantherX Wiki found at: \"" + wikiPath.toStdString() +"\"");
            return QUrl::fromLocalFile(wikiPath);
        }
    }
    return QUrl("");
}

void MainWindow::buildSidebar(){
    UrlLoader *defaultLoader = nullptr;
    if(!_defaultPage.isEmpty()) {
        QUrl defaultUrl(_defaultPage);
        if(defaultUrl.fileName().isEmpty())
            defaultUrl = QUrl(_defaultPage + "index.html");
        defaultLoader = new UrlLoader(_mainUrl, defaultUrl);
        connect(defaultLoader, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));
        GLOG_INF("Will open: " + defaultUrl.toString().toStdString());
    }
    auto overviewItem = new PXSideBarItem(OVERVEIW_TITLE, PXSideBarItem::ItemType::Item, defaultLoader);
    
    auto mainUrlLoader = new UrlLoader(_mainUrl , QUrl(leftsideItemUrl[MAIN_PAGE_TITLE]));
    auto mainpageItem = new PXSideBarItem(MAIN_PAGE_TITLE, PXSideBarItem::ItemType::Subitem, mainUrlLoader);
    connect(mainUrlLoader, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));

    auto tableUrlLoader = new UrlLoader(_mainUrl, QUrl(leftsideItemUrl[TABL_OF_CONTENT_TITLE]));
    auto tableofcontentItem = new PXSideBarItem(TABL_OF_CONTENT_TITLE, PXSideBarItem::ItemType::Subitem,tableUrlLoader);
    connect(tableUrlLoader, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));
    
    auto involvedUrlLoader = new UrlLoader(_mainUrl ,QUrl(leftsideItemUrl[GETTING_INVOLVED_TITLE]));
    auto gettinginvolvedItem = new PXSideBarItem(GETTING_INVOLVED_TITLE, PXSideBarItem::ItemType::Subitem,involvedUrlLoader);
    connect(involvedUrlLoader, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));
    
    auto searchScreen = new SearchScreen(_mainUrl);
    connect(searchScreen, &SearchScreen::searchItemClicked, [&](const QUrl &url){
        if(searchScreenLoader) {
            searchScreenLoader->deleteLater();
            searchScreenLoader = nullptr;
        }
        searchScreenLoader = new UrlLoader(_mainUrl, url);
        addContent(searchScreenLoader);
    });
    auto searchItem = new PXSideBarItem(SEARCH_TITLE, PXSideBarItem::ItemType::Subitem, searchScreen);

    auto interactionItem = new PXSideBarItem(INTERACTION_TITLE, PXSideBarItem::ItemType::Item);

    auto helpUrlLoader = new UrlLoader(_mainUrl, QUrl(leftsideItemUrl[HELP_TITLE]));
    auto helpItem = new PXSideBarItem(HELP_TITLE, PXSideBarItem::ItemType::Subitem, helpUrlLoader);
    connect(helpUrlLoader, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));

    auto contributingUrlLoader = new UrlLoader(_mainUrl ,QUrl(leftsideItemUrl[CONTRIBUTING_TITLE]));
    auto contributingItem = new PXSideBarItem(CONTRIBUTING_TITLE, PXSideBarItem::ItemType::Subitem, contributingUrlLoader);
    connect(contributingUrlLoader, SIGNAL(urlChanged(const QUrl &)), this, SLOT(urlChanged(const QUrl &)));

    addItemToSideBar(overviewItem);
    addItemToSideBar(mainpageItem);
    addItemToSideBar(tableofcontentItem);
    addItemToSideBar(gettinginvolvedItem);
    addItemToSideBar(searchItem);
    addItemToSideBar(interactionItem);
    addItemToSideBar(helpItem);
    addItemToSideBar(contributingItem);

    if(_defaultPage.isEmpty())
        setDefaultItem(mainpageItem);
    else
        setDefaultItem(overviewItem);
}

MainWindow::~MainWindow() {
}

void MainWindow::sideBarItemHandler (QListWidgetItem* item){
    PXMainWindow::sideBarItemHandler(item);
    auto pxWidget = ((PXSideBarItem *)item)->getView();
    if(pxWidget){
        auto urlLoader = qobject_cast<UrlLoader*>(((PXSideBarItem *)item)->getView());
        if(urlLoader)
            urlLoader->goHome();
    }
}

void MainWindow::urlChanged(const QUrl &url){
    auto _str = url.toString().replace("//","/");
    searchBox()->setText(_str);
}