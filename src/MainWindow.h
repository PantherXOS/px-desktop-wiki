#ifndef EXAMPLE_MAINWINDOW
#define EXAMPLE_MAINWINDOW

#include <QPushButton>
#include <QLineEdit>

#include "PXMainWindow.h"
#include "PXSideBarItem.h"

class MainWindow : public PXMainWindow {
Q_OBJECT
public:
    explicit MainWindow(const QString &mainUrl, const QString &defaultPage="", QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QString          getInstalledWikiPath();
    void             buildSidebar();
    QWidget         *buildSidebarWidget();
    PXContentWidget *buildView();
    QString         _mainUrl;
    QString         _defaultPage;
};

#endif //EXAMPLE_MAINWINDOW
