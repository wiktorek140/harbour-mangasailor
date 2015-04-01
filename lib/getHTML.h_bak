#ifndef GETHTML_H
#define GETHTML_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>


class GetHTML: public QObject
{
    Q_OBJECT

public:
    explicit GetHTML(QObject *parent = 0);

    Q_INVOKABLE QString getHTML(const QString &address);

private slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

};

#endif // GETHTML_H
