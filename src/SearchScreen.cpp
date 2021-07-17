#include "SearchScreen.h"
#include "Logger.h"
#include "SearchItem.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProcess>
#include <QDebug>
#include <QByteArray>

SearchScreen::SearchScreen(const QUrl &url):
    PXContentWidget("Search") ,
    _url(url) {
        auto searchTextEdit = new QLineEdit(this);
        searchTextEdit->setPlaceholderText("Enter the keywords ...");
        searchTextEdit->addAction(QIcon::fromTheme("system-search"),QLineEdit::LeadingPosition);

        process = new QProcess(this);
        connect(searchTextEdit, &QLineEdit::textChanged, [&](const QString &text){
            _keyword = text;
            if(text.isEmpty())
                clearList();
            else {
                QStringList args = QStringList() << "-F" << "mtype filename url abstract" << "dir:" + _url.toLocalFile() << "-q" << text;
                process->start("recollq", args);
            }
        });
        connect(process, SIGNAL(finished(int)), this, SLOT(recollProcessHandler(int)));
        _listWidget = new QListWidget(this);
        connect(_listWidget, SIGNAL(itemPressed(QListWidgetItem *)),this, SLOT(itemClickedHandler(QListWidgetItem *)));
        auto layout = new QVBoxLayout();
        layout->addWidget(searchTextEdit);
        layout->addWidget(_listWidget);
        layout->setAlignment(Qt::AlignTop);
        setLayout(layout);
}

void SearchScreen::recollProcessHandler(int exitCode){
    clearList();
    if(!exitCode) {
        QString output = process->readAllStandardOutput();
        QStringList lines = output.split('\n');
        for(auto const &l: lines){
            QStringList info = l.split(" ");
            if(info.size()>=4) {
                QString type = QString::fromStdString(QByteArray::fromBase64(info[0].toUtf8()).toStdString());
                if(type == "text/html") {
                    QString name = QString::fromStdString(QByteArray::fromBase64(info[1].toUtf8()).toStdString()).remove("[").remove("]");
                    QString addr = QString::fromStdString(QByteArray::fromBase64(info[2].toUtf8()).toStdString()).remove("[").remove("]");
                    QString abstract = QString::fromStdString(QByteArray::fromBase64(info[3].toUtf8()).toStdString()).remove("[").remove("]");
                    SearchItem *item = new SearchItem(name,addr.remove(_url.toString()), _keyword, abstract);
                    _listWidget->addItem(item);
                    _listWidget->setItemWidget(item, item->widget());
                    item->setSizeHint(item->widget()->size());
                }
            }
        }
    } else {
        GLOG_WRN(process->readAllStandardError().toStdString());
    }
}

void SearchScreen::clearList(){
    for(int i=0; i<_listWidget->count(); i++) {
        delete _listWidget->item(i);
    }
    _listWidget->clear();
}

void SearchScreen::itemClickedHandler(QListWidgetItem *item){
    auto url = ((SearchItem *)item)->address();
    GLOG_INF("Search item click: " + url.toString().toStdString());
    emit searchItemClicked(url);
}
