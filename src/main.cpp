#include <QCommandLineParser>
#include <QApplication>

#include "Logger.h"
#include "MainWindow.h"

#define APPLICATION_NAME    "px-desktop-wiki"
#define APPLICATION_VERSION "0.0.3"

Logger gLogger(std::string(APPLICATION_NAME));


#define ARG_NAME_PAGE       "page"

QMap<QString, QString> parseUrlScheme(const QString &url) {
    QStringList elements = url.split(':');
    QMap<QString, QString> map;
    if(elements.size() == 2){
        QStringList entries = elements[1].split("?");
        for(const auto &entry : entries) {
            QStringList parts = entry.split('=');
            if(parts.size() == 2) {
                QString result = parts[1];
                if(parts[0] == ARG_NAME_PAGE)
                    map[ARG_NAME_PAGE] = result;
            }
        }
    }
    return map;
}

int main (int argc, char** argv) {
    // https://git.pantherx.org/development/applications/px-desktop-wiki/-/issues/11#note_43087
    qputenv("QTWEBENGINE_DISABLE_SANDBOX", QByteArray(QString("ON").toUtf8()));    

    QMap<QString, QString> urlArgs;
    if(argc > 1) {
        for(int i=0; i<argc; i++)   
            urlArgs.insert(parseUrlScheme(argv[i]));
    }

    auto app = new QApplication(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(APPLICATION_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription(APPLICATION_NAME);
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption pageOption(QStringList() << "p" << "page",
                                    "Enter page for openning as default","url");
    parser.addOption(pageOption);
    
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
    
    QString page = (parser.value(pageOption).isEmpty() ? urlArgs[ARG_NAME_PAGE] : parser.value(pageOption));
    MainWindow mainwindow(parser.value(urlOption), page);
    mainwindow.showMaximized();
    app->exec();
    return 0;
}