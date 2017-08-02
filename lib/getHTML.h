#ifndef GETHTML_H
#define GETHTML_H

#include <QString>
#include <QObject>
#include <QtWebKit/QtWebKit>
#include <QNetworkAccessManager>


class GetHTML: public QObject
{
    Q_OBJECT

public:
    explicit GetHTML(QObject *parent = 0);

    Q_INVOKABLE void get(const QString &address);
    Q_INVOKABLE QString getHTML(const QString &address);
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged)

    void setHtml(const QString &a) {
        if (a != m_html) {
            m_html = a;
            emit htmlChanged();
        }
    }
    QString html() const {
        return m_html;
    }

signals:
    void htmlChanged();
    void finish();

private slots:
    void error(QNetworkReply::NetworkError err);
    void updateProgress(qint64 read, qint64 total);
    void finished();

private:
    QNetworkAccessManager  manager;
    QNetworkReply * _reply;
    QString m_html;

};

#endif // GETHTML_H
