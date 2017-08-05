#ifndef MANGAFOX_H
#define MANGAFOX_H

#include <QStringList>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFileInfo>
#include <QRegularExpression>

class MangaFox : public QObject
{
    Q_OBJECT
public:
    explicit MangaFox(QObject *parent = 0);

    //Manga lists
    QStringList mangaList(QXmlStreamReader &xml);
    QStringList chaptersList(QXmlStreamReader &xml);
    QStringList linksList(QXmlStreamReader &xml);
    QStringList mangalinksList(QXmlStreamReader &xml);
    //Not implemented properly but work
    QStringList iconsList(QXmlStreamReader &xml);

    // Manga info page
    QString getMangaImage(QXmlStreamReader &xml);
    QStringList getInfos(QXmlStreamReader &xml);
    QStringList getGenres(QXmlStreamReader &xml);
    QString getDescription(QXmlStreamReader &xml);

    //Manga chapterlist page
    QStringList getMangaChapters(QXmlStreamReader &xml);
    QStringList getChaptersNames(QXmlStreamReader &xml);

    // Image Page
    QString getChapterName(QXmlStreamReader &xml);
    QString getImage(QXmlStreamReader &xml);

    //Reader page
    QStringList getNextPrev(QXmlStreamReader &xml, QString bUrl="");
    QString getNextPageUrl(QXmlStreamReader &xml, const QString &bUrl);
    int getLastPage(QXmlStreamReader &xml);
    bool isChapter(QXmlStreamReader &xml);

};

#endif // MANGAFOX_H
