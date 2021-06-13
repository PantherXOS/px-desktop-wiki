#include <QCommandLineParser>
#include <QApplication>

#include "Logger.h"
#include "MainWindow.h"

#define APPLICATION_NAME  "px-desktop-wiki"
#define APPLICATION_VERSION "0.0.1"

Logger gLogger(std::string(APPLICATION_NAME));

int main (int argc, char** argv) {
    auto app = new QApplication(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription(APPLICATION_NAME);
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption urlOption(QStringList() << "u" << "url",
                                    "Enter the url or path of local html","url");
    parser.addOption(urlOption);    

    QCommandLineOption debugOption(QStringList() << "d" << "debug",
                                    "Running in debug mode.");
    parser.addOption(debugOption);
    parser.process(*app);
    
    LogTarget logTarget = LogTarget::SYSLOG;
    LogLevel logLevel = LogLevel::INF;    
    if(parser.isSet(debugOption))
        logTarget = LogTarget::CONSOLE;
    
    GLOG_INIT(logTarget, logLevel);
    GLOG_INF("=> px-desktop-wiki");
    
    MainWindow mainwindow(parser.value(urlOption));
    mainwindow.show();
    app->exec();
    return 0;
}