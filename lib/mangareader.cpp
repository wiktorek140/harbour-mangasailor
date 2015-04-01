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

QString MangaReader::getMangaImage(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QString imageUrl;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QString name = xml.name().toString();
            QXmlStreamAttributes attributes = xml.attributes();
            if ( name == "img")
                imageUrl = attributes.value("src").toString();
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

    return imageUrl;
}

QStringList MangaReader::getInfoHeaders(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList headers;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        QXmlStreamAttributes attributes = xml.attributes();
        if( attributes.value("class").toString() == "propertytitle" )
            headers.append(xml.readElementText());
    }
    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return headers;
}

QStringList MangaReader::getInfos(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList infos;
    bool first = false;
    while (!xml.atEnd()/* && !xml.hasError()*/) {
        xml.readNext();
        QString name = xml.name().toString();
        QXmlStreamAttributes attributes = xml.attributes();
        if ( attributes.value("class").toString() == "propertytitle" && (xml.readElementText() != "Name:" || xml.readElementText() == "Alternate Name:"))
            first = true;
        else if ( name == "td" && first) {
            infos.append(xml.readElementText());
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
    return infos;
}

QStringList MangaReader::getGenres(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList genres;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        QXmlStreamAttributes attributes = xml.attributes();
        if ( attributes.value("class").toString() == "genretags") {
            QString genre =  xml.readElementText();
            genres.append(genre);
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

    return genres;
}

QString MangaReader::getDescription(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QString description;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "p")
            description = xml.readElementText();
    }
    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return description;
}

QStringList MangaReader::getMangaChapters(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(html);
    QStringList urls;
    bool first = false;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        QXmlStreamAttributes attributes = xml.attributes();
        if ( first && xml.isStartElement() && xml.name().toString() == "a") {
            QString getUrl = "http://www.mangareader.net" + attributes.value("href").toString();
            urls.append(getUrl);
        }
        if ( attributes.value("id").toString() == "listing")
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "table" )
            first = false;
    }
    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return urls;
}

QStringList MangaReader::getChaptersNames(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(html);
    QStringList names;
    bool first = false;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        QXmlStreamAttributes attributes = xml.attributes();
        if ( first && xml.isStartElement() && xml.name().toString() == "a") {
            QString name;
            name.append(xml.readElementText());
            names.append(name);
        }
        if ( attributes.value("id").toString() == "listing")
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "table" )
            first = false;
    }
    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return names;
}

QStringList MangaReader::getUrls(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(html);
    QStringList urls;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "option") {
            QXmlStreamAttributes attributes = xml.attributes();
            urls.append(attributes.value("value").toString());
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
    return urls;
}

QString MangaReader::getImage(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString fullUrl = "http://www.mangareader.net" + url;
    QString html = getHtml.getHTML(fullUrl);
    QXmlStreamReader xml(html);
    QString imageUrl;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "img") {
            QXmlStreamAttributes attributes = xml.attributes();
            imageUrl.append(attributes.value("src").toString());
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
    return imageUrl;
}

QString MangaReader::getImgWidth(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString fullUrl = "http://www.mangareader.net" + url;
    QString html = getHtml.getHTML(fullUrl);
    QXmlStreamReader xml(html);
    QString imageWidth;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "img") {
            QXmlStreamAttributes attributes = xml.attributes();
            imageWidth.append(attributes.value("width").toString());
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
    return imageWidth;
}

QString MangaReader::getImgHeight(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString fullUrl = "http://www.mangareader.net" + url;
    QString html = getHtml.getHTML(fullUrl);
    QXmlStreamReader xml(html);
    QString imageHeight;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "img") {
            QXmlStreamAttributes attributes = xml.attributes();
            imageHeight.append(attributes.value("height").toString());
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
    return imageHeight;
}

QStringList MangaReader::getNextPrev(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(html);
    QStringList urls;
    bool first = false;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        QXmlStreamAttributes attributes = xml.attributes();
        if ( first && xml.isStartElement() && xml.name().toString() == "a") {
            QString getUrl = "http://www.mangareader.net" + attributes.value("href").toString();
            urls.append(getUrl);
        }
        if ( attributes.value("class").toString() == "c5")
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "table" )
            first = false;
    }
    if (xml.hasError())
    {
        cout << "XML error: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return urls;
}

QString MangaReader::getTitle(const QString &url) {
    QTextStream cout(stdout); // set cout for console debug
    QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(html);
    QString title;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "h1") {
            title.append(xml.readElementText());
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
    return title;
}
