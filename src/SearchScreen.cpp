#include "SearchScreen.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProcess>
#include <QDebug>
#include <QByteArray>
#include <PXSettings.h>

#include "Logger.h"
#include "SearchItem.h"

SearchScreen::SearchScreen(const QUrl &url):
    PXContentWidget("Search") ,
    _url(url),
    _wikiDbState(WIKI_DB_STATE::NONE){
        connect(this, &SearchScreen::wikiDBchecked, [&](WIKI_DB_STATE state){
            _progressIndicator->setVisible(false);
            _progressIndicator->stopAnimation();
            if(state == WIKI_DB_STATE::EXIST) {
                _messageLabel->setVisible(false);
                _indexButton->setVisible(false);
                _searchTextEdit->setVisible(true);
                _listWidget->setVisible(true);
            } else if(state == WIKI_DB_STATE::NOT_EXIST) {
                _messageLabel->setVisible(true);
                _indexButton->setVisible(true);
                _searchTextEdit->setVisible(false);
                _listWidget->setVisible(false);
            }
        });

        _process = new QProcess();
        connect(_process, SIGNAL(finished(int)), this, SLOT(recollProcessHandler(int)));
        // ---------------------------------------------------------------------------------------------------------
        _messageLabel = new QLabel("Search automatically indexes your files every 6 hours but has not run yet.");
        _messageLabel->setVisible(false);
        _messageLabel->setWordWrap(true);
        auto font = _messageLabel->font();
        font.setPointSize(18);
        _messageLabel->setFont(font);
        _messageLabel->setAlignment(Qt::AlignCenter);
        
        _indexButton  = new QPushButton("Start indexing now");
        _indexButton->setVisible(false);
        _indexButton->setStyleSheet("QPushButton {background-color: " + PXSettings::Colors::blueColor().name() + "; color: white;}\
                                     QPushButton:disabled {background-color: " + PXSettings::Colors::grayColor().name() + "; color: white;}");
        connect(_indexButton, &QPushButton::pressed, [&](){
            _process->start("recollindex", QStringList());
            _indexButton->setEnabled(false);
            _progressIndicator->setVisible(true);
            _progressIndicator->startAnimation();
        });

        _progressIndicator = new PXProgressIndicator(this);
        _progressIndicator->setFixedSize(QSize(32,32));
        _progressIndicator->startAnimation();

        auto buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(_indexButton);
        buttonLayout->addWidget(_progressIndicator);
        buttonLayout->setAlignment(Qt::AlignCenter);

        _searchTextEdit = new QLineEdit(this);
        _searchTextEdit->setVisible(false);
        _searchTextEdit->setPlaceholderText("Enter the keywords ...");
        _searchTextEdit->addAction(QIcon::fromTheme("system-search"),QLineEdit::LeadingPosition);

        connect(_searchTextEdit, &QLineEdit::textChanged, [&](const QString &text){
            _keyword = text;
            if(text.isEmpty())
                clearList();
            else {
                QStringList args = QStringList() << "-F" << "mtype filename url abstract" << "dir:" + _url.toLocalFile() << "-q" << text;
                _process->start("recollq", args);
            }
        });
        _listWidget = new QListWidget(this);
        _listWidget->setVisible(false);
        connect(_listWidget, SIGNAL(itemPressed(QListWidgetItem *)),this, SLOT(itemClickedHandler(QListWidgetItem *)));
        auto layout = new QVBoxLayout();
        layout->addWidget(_messageLabel);
        layout->addLayout(buttonLayout);
        layout->addWidget(_searchTextEdit);
        layout->addWidget(_listWidget);
        layout->setAlignment(Qt::AlignTop);
        setLayout(layout);
        // ---------------------------------------------------------------------------------------------------------
        // Chech wiki is indexed or not
        QStringList args;
        if (_url.toLocalFile().at(_url.toLocalFile().size() - 1) == "/")
            args << "dir:" + _url.toLocalFile().left(_url.toLocalFile().size() - 1);
        else 
            args << "dir:" + _url.toLocalFile();
        _process->start("recollq", args);
}

void SearchScreen::recollProcessHandler(int exitCode){
    QString output = _process->readAllStandardOutput();
    QString error  = _process->readAllStandardError();
    // qDebug() << "output" << output;
    GLOG_WRN(error.toStdString());
    if(_wikiDbState == WIKI_DB_STATE::NONE){
        _wikiDbState = WIKI_DB_STATE::NOT_EXIST;
        QStringList lines = output.split('\n');
        for(auto const &l: lines){
            QStringList info = l.split(" ");
            if(info.size()>=4) {
                _wikiDbState = WIKI_DB_STATE::EXIST;
                GLOG_INF("WIKI indexed by recoll");
                break;
            }
        }
        emit wikiDBchecked(_wikiDbState);
        if (_wikiDbState == WIKI_DB_STATE::NOT_EXIST)
            GLOG_INF("WIKI is not indexed by recoll");
        return;
    } else if (_wikiDbState == WIKI_DB_STATE::EXIST) {
        clearList();
        if(!exitCode) {
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
            GLOG_WRN(_process->readAllStandardError().toStdString());
        }
    } else {
        _wikiDbState = WIKI_DB_STATE::EXIST;
        emit wikiDBchecked(_wikiDbState);
        GLOG_INF("WIKI indexed by recoll DB just now");
    }
}

void SearchScreen::clearList(){
    for(int i=0; i<_listWidget->count(); i++) {
        delete _listWidget->item(i);
    }
    _listWidget->clear();
}

void SearchScreen::itemClickedHandler(QListWidgetItem *item){
    _listWidget->scrollToItem(item);
    auto url = ((SearchItem *)item)->address();
    GLOG_INF("Search item click: " + url.toString().toStdString());
    emit searchItemClicked(url);
}