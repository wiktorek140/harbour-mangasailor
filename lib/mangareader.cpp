#include "mangareader.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QStringList>

MangaReader::MangaReader(QObject *parent) :
    QObject(parent)
{}

QStringList MangaReader::mangaList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList manga;
    QString activeManga;


    bool first = false;
    bool second = false;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QString name = xml.name().toString();
            if (name == "td")
                first = true;
            if ( name == "strong")
                second = true;
            if (first && second) {
                activeManga = xml.readElementText();
            }
            QXmlStreamAttributes attributes = xml.attributes();
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                manga.append(activeManga);
            }
        }
        if (xml.isEndElement()) {
            QString name = xml.name().toString();
            if ( name == "td")
                first = false;
            if ( name == "strong")
                second = false;
        }
    }

    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return manga;
}

QStringList MangaReader::chaptersList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList chapters;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QXmlStreamAttributes attributes = xml.attributes();
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                QString activeChapter = xml.readElementText();
                chapters.append(activeChapter);
            }
        }
    }

    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return chapters;
}

QStringList MangaReader::linksList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList links;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QXmlStreamAttributes attributes = xml.attributes();
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                QString activeLink = attributes.value("href").toString();
                links.append(activeLink);
            }
        }
    }

    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return links;
}

QStringList MangaReader::mangalinksList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList links;
    QString activeMangaLink;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QString name = xml.name().toString();
            QXmlStreamAttributes attributes = xml.attributes();
            if (name == "td" && !attributes.hasAttribute("class")) {
                xml.readNext();
                attributes = xml.attributes();
                activeMangaLink = attributes.value("href").toString();
            }
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                links.append(activeMangaLink);
            }
        }
    }

    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return links;
}

QStringList MangaReader::iconsList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList icons;
    QString icon;
    bool first = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QString name = xml.name().toString();
            QXmlStreamAttributes attributes = xml.attributes();
            if (name == "td")
                first = true;
            if (!first)
                icon = "none";
            if ( first && name == "div") {
                icon = attributes.value("class").toString();
            }

            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                icons.append(icon);
                cout << xml.readElementText() << " " << icon << endl;
            }
        }
        if ( xml.isEndElement() ) {
            QString name = xml.name().toString();
            if ( name == "td")
                first = false;
        }

    }

    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return icons;
}

QStringList MangaReader::getImage(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QString image;
    QString imgWidth;
    QString imgHeight;
    QString nextUrl;
    QString prevUrl;
    QString endOfChap = "empty";
    QString title;
    QString page;
    QString lastPage;


    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QXmlStreamAttributes attributes = xml.attributes();
            if ( attributes.value("id").toString() == "recom_info") {
                xml.readNext();
                endOfChap = xml.readElementText();
                break;
            } else if( attributes.value("id").toString() == "img") {
                image = attributes.value("src").toString();
                imgWidth = attributes.value("width").toString();
                imgHeight = attributes.value("height").toString();
            } else if ( attributes.value("class").toString() == "next") {
                xml.readNext();
                attributes = xml.attributes();
                nextUrl = attributes.value("href").toString();
            } else if ( attributes.value("class").toString() == "prev") {
                xml.readNext();
                attributes = xml.attributes();
                prevUrl = attributes.value("href").toString();
            } else if ( attributes.value("id").toString() == "mangainfo_son" ) {
                xml.readNext();
                title = xml.readElementText();
            } else if( attributes.value("selected").toString() == "selected") {
                page = xml.readElementText();
            } else if( xml.name().toString() == "option") {
                lastPage = xml.readElementText();
            }
        }
    }

    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    QStringList list;
    list.append(image);
    list.append(imgWidth);
    list.append(imgHeight);
    list.append(nextUrl);
    list.append(prevUrl);
    list.append(endOfChap);
    list.append(title);
    list.append(page);
    list.append(lastPage);

    return list;
}
