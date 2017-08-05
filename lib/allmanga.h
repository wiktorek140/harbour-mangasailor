#ifndef ALLMANGA_H
#define ALLMANGA_H

#include <QStringList>
#include "getHTML.h"
#include "lib/pages/mangafox.h"

class GetHTML;

class AllManga : public QObject
{
    Q_OBJECT

public:
    explicit AllManga(QObject *parent = 0);
    QString specialParse(QString input, bool img=true);

    // Latest Updates
    Q_INVOKABLE QStringList mangaList(const QString &html,int w=0);
    Q_INVOKABLE QStringList chaptersList(const QString &html,int w=0);
    Q_INVOKABLE QStringList linksList(const QString &html,int w=0);
    Q_INVOKABLE QStringList mangalinksList(const QString &html,int w=0);
    Q_INVOKABLE QStringList iconsList(const QString &html,int w=0);

    // Manga Page
    Q_INVOKABLE QString getMangaImage(const QString &html,int w=0);
    Q_INVOKABLE QStringList getInfos(const QString &html,int w=0);
    Q_INVOKABLE QStringList getGenres(const QString &html,int w=0);
    Q_INVOKABLE QString getDescription(const QString &html,int w=0);

    Q_INVOKABLE QStringList getMangaChapters(const QString &html,int w=0);
    Q_INVOKABLE QStringList getChaptersNames(const QString &html,int w=0);
    Q_INVOKABLE QString getChapterName(const QString &html,int w=0);

    // Image Page
    Q_INVOKABLE QString getImage(const QString &html,int w=0);
    //Q_INVOKABLE QString getImgWidth(const QString &url,int w=0);
    //Q_INVOKABLE QString getImgHeight(const QString &url,int w=0);


    Q_INVOKABLE QStringList getNextPrev(const QString &html, const QString &bUrl, int w=0);
    Q_INVOKABLE QString getNextPageUrl(const QString &html,const QString &bUrl, int w=0);
    //Q_INVOKABLE QString getTitle(const QString &url,int w=0);

    Q_INVOKABLE int getLastPage(const QString &html, int w=0);
    Q_INVOKABLE bool isChapter(const QString &html, int w=0);

    QStringList pages = {"http://www.mangareader.net","http://mangafox.me"};
    Q_INVOKABLE QString pageBase(int num) {return pages.at(num);}

    Q_INVOKABLE bool parsed=false;
private:
    GetHTML getHtml;
    MangaFox mangaFox;
    QXmlStreamAttributes attributes;


};

#endif // ALLMANGA_H
