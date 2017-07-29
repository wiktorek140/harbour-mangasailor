#ifndef MANGAREADER_H
#define MANGAREADER_H

#include <QStringList>
#include "getHTML.h"

class GetHTML;

class MangaReader : public QObject
{
    Q_OBJECT

public:
    explicit MangaReader(QObject *parent = 0);
    QString specialParse(QString input);
    // Latest Updates
    Q_INVOKABLE QStringList mangaList(const QString &html);
    Q_INVOKABLE QStringList chaptersList(const QString &html);
    Q_INVOKABLE QStringList linksList(const QString &html);
    Q_INVOKABLE QStringList mangalinksList(const QString &html);
    Q_INVOKABLE QStringList iconsList(const QString &html);

    // Manga Page
    Q_INVOKABLE QString getMangaImage(const QString &html);
    Q_INVOKABLE QStringList getInfoHeaders(const QString &html);
    Q_INVOKABLE QStringList getInfos(const QString &html);
    Q_INVOKABLE QStringList getGenres(const QString &html);
    Q_INVOKABLE QString getDescription(const QString &html);

    Q_INVOKABLE QStringList getMangaChapters(const QString &url);
    Q_INVOKABLE QStringList getChaptersNames(const QString &url);

    // Image Page
    Q_INVOKABLE QStringList getUrls(const QString &html);
    Q_INVOKABLE QString getImage(const QString &url);
    Q_INVOKABLE QString getImgWidth(const QString &url);
    Q_INVOKABLE QString getImgHeight(const QString &url);
    Q_INVOKABLE QStringList getNextPrev(const QString &url);
    Q_INVOKABLE QString getTitle(const QString &url);
private:
    GetHTML getHtml;
    QXmlStreamAttributes attributes;


};

#endif // MANGAREADER_H
