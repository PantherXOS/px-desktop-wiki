#ifndef EXAMPLE_MAINWINDOW
#define EXAMPLE_MAINWINDOW

#include <QPushButton>
#include <QLineEdit>

#include "PXMainWindow.h"
#include "PXSideBarItem.h"
#include "UrlLoader.h"

class MainWindow : public PXMainWindow {
Q_OBJECT
public:
    explicit MainWindow(const QUrl &mainUrl, const QString &defaultPage="", QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void backButtonPressed      () override;
    void forwardButtonPressed   () override;
    void sideBarItemHandler     (QListWidgetItem*);

private:
    QUrl             getInstalledWikiPath();
    void             buildSidebar();
    QWidget         *buildSidebarWidget();
    PXContentWidget *buildView();
    QUrl            _mainUrl;
    QString         _defaultPage;
    UrlLoader       *searchScreenLoader = nullptr;
};

#endif //EXAMPLE_MAINWINDOW
