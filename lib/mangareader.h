#ifndef MANGAREADER_H
#define MANGAREADER_H

#include <QStringList>

class MangaReader : public QObject
{
    Q_OBJECT

public:
    explicit MangaReader(QObject *parent = 0);

    Q_INVOKABLE QStringList mangaList(const QString &html);
    Q_INVOKABLE QStringList chaptersList(const QString &html);
    Q_INVOKABLE QStringList linksList(const QString &html);
    Q_INVOKABLE QStringList mangalinksList(const QString &html);
    Q_INVOKABLE QStringList iconsList(const QString &html);
    Q_INVOKABLE QStringList getImage(const QString &html);

};

#endif // MANGAREADER_H
