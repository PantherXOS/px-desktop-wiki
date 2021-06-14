#include "MainWindow.h"

#include <QDir>
#include <QMap>
#include <QStandardPaths>

#include "Logger.h"
#include "UrlLoader.h"

#define OVERVEIW_TITLE              "Overview"
#define MAIN_PAGE_TITLE             "Main page"
#define TABL_OF_CONTENT_TITLE       "Table of Contents"
#define GETTING_INVOLVED_TITLE      "Getting Involved"
#define SEARCH_TITLE                "Search"
#define INTERACTION_TITLE           "Interaction"
#define HELP_TITLE                  "Help"
#define CONTRIBUTING_TITLE          "Contributing"

QMap<QString, QString> leftsideItemUrl = {{MAIN_PAGE_TITLE, "/index.html"},
                                          {TABL_OF_CONTENT_TITLE, "/content/content.html"},
                                          {GETTING_INVOLVED_TITLE, "/content/involved.html"},
                                          {SEARCH_TITLE, "/content/search.html"},
                                          {HELP_TITLE, "/content/help.html"},
                                          {CONTRIBUTING_TITLE, "/content/contributing.html"}};

MainWindow::MainWindow(const QString &mainUrl, const QString &defaultPage, QWidget *parent) :
        PXMainWindow("PantherX Wiki: ", QIcon::fromTheme("panther"), parent),
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
    GLOG_INF("=> Main Path Loading: " + mainUrl.toStdString());
    buildSidebar();
    topBar()->searchBox()->setDisabled(true);
    topBar()->backButtonAction()->setVisible(false);
    topBar()->forwardButtonAction()->setVisible(false);
    topBar()->settingsButtonAction()->setVisible(false);
    topBar()->helpButtonAction()->setVisible(false);
}

QString MainWindow::getInstalledWikiPath(){
    auto list = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for(auto const &l: list){
        GLOG_INF("Finding wiki in: \"" + l.toStdString() +"\" ...");
        QString wikiPath = l + "/px-wiki"; 
        if(QDir(wikiPath).exists()){
            GLOG_INF("PantherX Wiki found at: \"" + wikiPath.toStdString() +"\"");
            return wikiPath;
        }
    }
    return "";
}

void MainWindow::buildSidebar(){
    UrlLoader *defaultLoader = nullptr;
    if(!_defaultPage.isEmpty()) {
        defaultLoader = new UrlLoader(QUrl(_mainUrl + "/" + _defaultPage));
        GLOG_INF("Will open: " + _defaultPage.toStdString());
    }
    auto overviewItem = new PXSideBarItem(OVERVEIW_TITLE, PXSideBarItem::ItemType::Item, defaultLoader);
    
    auto mainUrlLoader = new UrlLoader(QUrl(_mainUrl + leftsideItemUrl[MAIN_PAGE_TITLE]));
    auto mainpageItem = new PXSideBarItem(MAIN_PAGE_TITLE, PXSideBarItem::ItemType::Subitem, mainUrlLoader);
    
    auto tableUrlLoader = new UrlLoader(QUrl(_mainUrl + leftsideItemUrl[TABL_OF_CONTENT_TITLE]));
    auto tableofcontentItem = new PXSideBarItem(TABL_OF_CONTENT_TITLE, PXSideBarItem::ItemType::Subitem,tableUrlLoader);

    auto involvedUrlLoader = new UrlLoader(QUrl(_mainUrl + leftsideItemUrl[GETTING_INVOLVED_TITLE]));
    auto gettinginvolvedItem = new PXSideBarItem(GETTING_INVOLVED_TITLE, PXSideBarItem::ItemType::Subitem,involvedUrlLoader);

    auto searchUrlLoader = new UrlLoader(QUrl(_mainUrl + leftsideItemUrl[SEARCH_TITLE]));
    auto searchItem = new PXSideBarItem(SEARCH_TITLE, PXSideBarItem::ItemType::Subitem, searchUrlLoader);

    auto interactionItem = new PXSideBarItem(INTERACTION_TITLE, PXSideBarItem::ItemType::Item);

    auto helpUrlLoader = new UrlLoader(QUrl(_mainUrl + leftsideItemUrl[HELP_TITLE]));
    auto helpItem = new PXSideBarItem(HELP_TITLE, PXSideBarItem::ItemType::Subitem, helpUrlLoader);

    auto contributingUrlLoader = new UrlLoader(QUrl(_mainUrl + leftsideItemUrl[CONTRIBUTING_TITLE]));
    auto contributingItem = new PXSideBarItem(CONTRIBUTING_TITLE, PXSideBarItem::ItemType::Subitem, contributingUrlLoader);

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
