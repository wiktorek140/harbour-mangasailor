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
    QXmlStreamReader xml(specialParse(html));
    QStringList manga;
    QString activeManga;
    QString name;
    QXmlStreamAttributes attributes;

    bool first = false;
    bool second = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement()) {
            name = xml.name().toString();
            if (name == "td")
                first = true;
            if ( name == "strong")
                second = true;
            if (first && second) {
                activeManga = xml.readElementText();
            }
            attributes = xml.attributes();
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                manga.append(activeManga);
            }
        }
        if (xml.isEndElement()) {
            name = xml.name().toString();
            if ( name == "td")
                first = false;
            if ( name == "strong")
                second = false;
        }
    }

    if (xml.hasError())
    {
        cout << "XML error1: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return manga;
}

QStringList MangaReader::chaptersList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList chapters;
    QXmlStreamAttributes attributes;
    QString activeChapter;

    cout << xml.readElementText()<< endl;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
             attributes = xml.attributes();
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                activeChapter = xml.readElementText();
                chapters.append(activeChapter);
            }
        }
    }

    if (xml.hasError())
    {
        cout << "XML error2: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return chapters;
}

QStringList MangaReader::linksList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList links;
    QXmlStreamAttributes attributes;
    QString activeLink;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            attributes = xml.attributes();
            if(attributes.hasAttribute("class") && attributes.value("class").toString() == "chaptersrec") {
                activeLink = attributes.value("href").toString();
                links.append(activeLink);
            }
        }
    }

    if (xml.hasError())
    {
        cout << "XML error3: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return links;
}

QStringList MangaReader::mangalinksList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
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
        cout << "XML error4: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return links;
}

QStringList MangaReader::iconsList(const QString &html) {

    QTextStream cout(stdout); // set cout for console debug

    //html.replace(QString("&"),QString("&#038;"));

    QXmlStreamReader xml(specialParse(html));
    QStringList icons;
    QString icon;
    bool first = false;
    QString name;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            name = xml.name().toString();
            attributes = xml.attributes();
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
            name = xml.name().toString();
            if ( name == "td")
                first = false;
        }

    }

    if (xml.hasError())
    {
        cout << "XML error5: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return icons;
}

QString MangaReader::getMangaImage(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QString imageUrl;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QString name = xml.name().toString();
            attributes = xml.attributes();
            if ( name == "img")
                imageUrl = attributes.value("src").toString();
        }
    }
    if (xml.hasError())
    {
        cout << "XML error6: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return imageUrl;
}

QStringList MangaReader::getInfoHeaders(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList headers;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        attributes = xml.attributes();
        if( attributes.value("class").toString() == "propertytitle" )
            headers.append(xml.readElementText());
    }
    if (xml.hasError())
    {
        cout << "XML error7: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return headers;
}

QStringList MangaReader::getInfos(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList infos;
    bool first = false;

    while (!xml.atEnd()/* && !xml.hasError()*/) {
        xml.readNext();
        QString name = xml.name().toString();
        attributes = xml.attributes();
        if ( attributes.value("class").toString() == "propertytitle" && (xml.readElementText() != "Name:" || xml.readElementText() == "Alternate Name:"))
            first = true;
        else if ( name == "td" && first) {
            infos.append(xml.readElementText());
            first = false;
        }
    }
    if (xml.hasError())
    {
        cout << "XML error8: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return infos;
}

QStringList MangaReader::getGenres(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList genres;
    QString genre;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        attributes = xml.attributes();
        if ( attributes.value("class").toString() == "genretags") {
            genre =  xml.readElementText();
            genres.append(genre);
        }
    }
    if (xml.hasError())
    {
        cout << "XML error9: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    return genres;
}

QString MangaReader::getDescription(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QString description;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "p")
            description = xml.readElementText();
    }
    if (xml.hasError())
    {
        cout << "XML error10: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QStringList urls;
    bool first = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        attributes = xml.attributes();
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
        cout << "XML error11: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QStringList names;
    bool first = false;
    QString name;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        attributes = xml.attributes();
        if ( first && xml.isStartElement() && xml.name().toString() == "a") {

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
        cout << "XML error12: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return names;
}

QStringList MangaReader::getUrls(const QString &html) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList urls;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "option") {
            attributes = xml.attributes();
            urls.append(attributes.value("value").toString());
        }
    }
    if (xml.hasError())
    {
        cout << "XML error13: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QString imageUrl;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "img") {
            attributes = xml.attributes();
            imageUrl.append(attributes.value("src").toString());
        }
    }
    if (xml.hasError())
    {
        cout << "XML error14: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QString imageWidth;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "img") {
            attributes = xml.attributes();
            imageWidth.append(attributes.value("width").toString());
        }
    }
    if (xml.hasError())
    {
        cout << "XML error15: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QString imageHeight;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "img") {
            attributes = xml.attributes();
            imageHeight.append(attributes.value("height").toString());
        }
    }
    if (xml.hasError())
    {
        cout << "XML error16: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QStringList urls;
    bool first = false;
    QString getUrl;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        attributes = xml.attributes();
        if ( first && xml.isStartElement() && xml.name().toString() == "a") {
            getUrl = "http://www.mangareader.net" + attributes.value("href").toString();
            urls.append(getUrl);
        }
        if ( attributes.value("class").toString() == "c5")
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "table" )
            first = false;
    }
    if (xml.hasError())
    {
        cout << "XML error17: " << xml.errorString() << endl;
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
    QXmlStreamReader xml(specialParse(html));
    QString title;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "h1") {
            title.append(xml.readElementText());
        }
    }
    if (xml.hasError())
    {
        cout << "XML error18: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return title;
}

// Used to fix & symbol
QString MangaReader::specialParse(QString input) {
        input.replace(QRegularExpression("&(?!amp;)"),"&amp;");
        return input;
}
