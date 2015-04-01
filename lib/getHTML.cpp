#include "getHTML.h"

GetHTML::GetHTML(QObject *parent) :
    QObject(parent)
{}

void GetHTML::get(const QString &url) {
    _reply = manager.get(QNetworkRequest(QUrl(url))); // Manager is my QNetworkAccessManager
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(error(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(downloadProgress(qint64, qint64)),
                this, SLOT(updateProgress(qint64, qint64)));
    connect(_reply, SIGNAL(finished()),
                this, SLOT(finished()));

}

QString GetHTML::getHTML(const QString &url) {
    // I HAVE NO IDEA WHAT I AM DOING BUT IT WORKS THANKS INTERNET
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
    QEventLoop event;
    connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    QString html = response->readAll(); // Source should be stored here
    return html;
}

void GetHTML::error(QNetworkReply::NetworkError err)
{
    // Manage error here.
    _reply->deleteLater();
}

void GetHTML::updateProgress(qint64 read, qint64 total)
{
    // This is where you can use the progress info for, lets say, update a progress bar:
    // progressBar->setMaximum(total);
    // progressBar->setValue(read);
}

void GetHTML::finished()
{
    QString html = _reply->readAll();
    QTextStream(stdout) << "Got Html" << endl;
    setHtml(html);
    _reply->deleteLater();
    // done
}
