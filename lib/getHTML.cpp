#include "getHTML.h"
#include <QtWebKit/QtWebKit>
#include <QNetworkAccessManager>

GetHTML::GetHTML(QObject *parent) :
    QObject(parent)
{}

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

void GetHTML::downloadProgress(qint64 bytesReceived, qint64 bytesTotal){
    double progress = bytesReceived/bytesTotal*100;
    QTextStream(stdout) << "Received: " << bytesReceived << endl
                        << "Total: " << bytesTotal << endl
                        << "Progress: " << progress << endl;
}
