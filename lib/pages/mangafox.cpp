#include "mangafox.h"


MangaFox::MangaFox(QObject *parent) : QObject(parent)
{

}

QStringList MangaFox::mangaList(QXmlStreamReader &xml) {

    QStringList manga;
    QString name;

    bool first = false;
    bool second = false;
    bool third = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement()) {

            if (xml.name().toString() == "ul" && xml.attributes().value("id").contains("idx_"))
                first = true;
            if ( xml.name().toString() == "li")
                second = true;
            if ( xml.name().toString() == "a")
                third = true;

            if (first && second && third) {
                //attributes = xml.attributes();
                if(xml.attributes().hasAttribute("href") ) {
                    name = xml.readElementText();
                    if(name.contains("&quot;"))name.replace("&quot;","\"");
                    manga.append(name);
                }
            }
        }
        if (xml.isEndElement()) {

            if ( xml.name().toString() == "ul")
                first = false;
            if ( xml.name().toString() == "li")
                second = false;
            if (xml.name().toString() == "a")
                third = false;
        }
    }
    return manga;
}


// Deprecated and unusable but still there (should stay for the sake of fucking latest update...)
QStringList  MangaFox::chaptersList(QXmlStreamReader &xml) {
    QStringList chapters;

    //need to be fixed
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            //attributes = xml.attributes();
            if(xml.attributes().hasAttribute("class") && xml.attributes().value("class").toString() == "chaptersrec") {
                //activeChapter = xml.readElementText();
                chapters.append(xml.readElementText());
            }
        }
    }

    return chapters;
}

QStringList  MangaFox::linksList(QXmlStreamReader &xml) {

    QStringList links;
    QString name;

    bool first = false;
    bool second = false;
    bool third = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            name = xml.name().toString();
            if (name == "ul" && xml.attributes().value("id").contains("idx_"))
                first = true;
            if ( name == "li")
                second = true;
            if ( name == "a")
                third = true;
            if (first && second && third) {
                //attributes = xml.attributes();
                if(xml.attributes().hasAttribute("href") && xml.attributes().value("href").toString() != "") {
                    //activeLink = xml.attributes().value("href").toString();
                    links.append(xml.attributes().value("href").toString());
                }
            }
        }
        if (xml.isEndElement()) {
            name = xml.name().toString();
            if ( name == "ul")
                first = false;
            if ( name == "li")
                second = false;
            if (name == "a")
                third = false;
        }
    }

    return links;
}

QStringList  MangaFox::mangalinksList(QXmlStreamReader &xml) {

    //Need to be fixed probably for sake of latest updates

    /*QStringList links;
    QString activeMangaLink;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QString name = xml.name().toString();
            QXmlStreamAttributes attributes = xml.attributes();
            if (name == "td" && !xml.attributes().hasAttribute("class")) {
                xml.readNext();
                attributes = xml.attributes();
                activeMangaLink = xml.attributes().value("href").toString();
            }
            if(xml.attributes().hasAttribute("class") && xml.attributes().value("class").toString() == "chaptersrec") {
                links.append(activeMangaLink);
            }
        }
    }
    return links;
    */

    return linksList(xml);
}

QStringList  MangaFox::iconsList(QXmlStreamReader &xml) {

    QStringList icons;
    QString icon;
    QString name;
    bool first = false,second = false, third= false;

    while (!xml.atEnd() && !xml.hasError() ) {
        xml.readNext();
        if (xml.isStartElement()) {
            name = xml.name().toString();
            if (name == "ul" && xml.attributes().value("id").contains("idx_"))
                first = true;
            if ( name == "li")
                second = true;
            if ( name == "a")
                third = true;
            if (first && second && third) {
                icon = xml.attributes().value("class").toString();
            } else icon="none";
            //attributes = xml.attributes();
            if(xml.attributes().hasAttribute("href") && xml.attributes().value("href").toString() != "") {
                icons.append(icon);
            }
        }
        if (xml.isEndElement()) {
            name = xml.name().toString();
            if ( name == "ul")
                first = false;
            if ( name == "li")
                second = false;
            if (name == "a")
                third = false;
        }
    }

    return icons;
}

QString  MangaFox::getMangaImage(QXmlStreamReader &xml) {

    QString imageUrl;
    bool first = false;

    while (!xml.atEnd() && !xml.hasError() ) {
        xml.readNext();
        if (xml.isStartElement()) {
            //name = xml.name().toString();
            if (xml.name().toString() == "div" && xml.attributes().value("class").toString() =="cover")
                first = true;

            //attributes = xml.attributes();
            if(first && xml.attributes().hasAttribute("src") && xml.attributes().value("src").toString() != "") {
                imageUrl = xml.attributes().value("src").toString();
            }
        }
        if (xml.isEndElement()) {

            if ( xml.name().toString() == "div")
                first = false;
        }
    }

    return imageUrl;
}

QStringList  MangaFox::getInfos(QXmlStreamReader &xml) {

    // Need little tweaks but in some cases work
    QStringList infos;
    bool first = false,second = false, h3 = false;

    while (!xml.atEnd() && !xml.hasError() ) {
        xml.readNext();
        if (xml.isStartElement()) {
            //name = xml.name().toString();
            if (xml.name().toString() == "div" && xml.attributes().value("id").toString() == "title")
                first = true;
            if (xml.name().toString() == "td" && xml.attributes().value("valign").toString()== "top")
                second = true;
            //attributes = xml.attributes();
            if(first && xml.name().toString() == "h3") {
                h3=true;
                infos.append(xml.readElementText());
            }
            if(first && second && !h3 && xml.name().toString() == "a" && xml.attributes().hasAttribute("href") && xml.attributes().hasAttribute("style")){
                if(infos.isEmpty())infos.append(" ");
                infos.append(xml.readElementText());
                if(infos.length() == 2) infos.append("Temp");
                if(infos.length() == 5) infos.append("Right to Left");
            }

        }
        if (xml.isEndElement()) {
            //name = xml.name().toString();
            if (xml.name().toString() == "div")
                first = false;
            if (xml.name().toString() == "h3")
                h3 = false;
            if (xml.name().toString() == "td")
                second = false;

        }
    }

    return infos;
}

QStringList  MangaFox::getGenres(QXmlStreamReader &xml) {

    // Need to be writted
    QStringList genres;
    QString genre;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        //attributes = xml.attributes();
        if ( xml.attributes().value("class").toString() == "genretags") {
            genre =  xml.readElementText();
            genres.append(genre);
        }
    }

    return genres;
}

QString  MangaFox::getDescription(QXmlStreamReader &xml) {
//Need to be writted
    QString description;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.name().toString() == "p")
            description = xml.readElementText();
    }

    return description;
}

QStringList  MangaFox::getMangaChapters(QXmlStreamReader &xml) {

    QStringList urls;
    QString getUrl;
    bool first = false,second=false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        //cout<< xml.name().toString()<<endl;
        //cout<< xml.attributes().value("id")<<endl;

        if(xml.isStartElement()){
            //attributes = xml.attributes();
            if(xml.name().toString() == "li") first=true;
            if(xml.name().toString() == "div") second = true;
            if(first && second && xml.name().toString() == "a" && xml.attributes().hasAttribute("class") && xml.attributes().hasAttribute("href")) {
                if(xml.attributes().value("class").toString() == "tips"){
                    //cout<<xml.attributes().value("href").toString()<<endl;
                    getUrl = xml.attributes().value("href").toString();
                    urls.append(getUrl);
                }


            }
        }

        if(xml.isEndElement()){
            if(xml.name().toString() == "li")
                first=false;
            if(xml.name().toString() == "div")
                second=false;
        }

    }

    for(int k=0, s=urls.size(), max=(s/2); k<max; k++) urls.swap(k,s-(1+k));

    return urls;
}

QStringList MangaFox::getChaptersNames(QXmlStreamReader &xml) {

    QStringList names;
    bool first = false,second=false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if(xml.isStartElement()){

            if(xml.name().toString() == "li") first=true;
            if(xml.name().toString() == "div") second = true;
            if(first && second && xml.name().toString() == "a" && xml.attributes().hasAttribute("class") && xml.attributes().hasAttribute("href")) {
                if(xml.attributes().value("class").toString() == "tips"){
                    names.append(xml.readElementText());
                }
            }
        }

        if(xml.isEndElement()){
            if(xml.name().toString() == "li")
                first=false;
            if(xml.name().toString() == "div")
                second=false;
        }

    }

    for(int k=0, s=names.size(), max=(s/2); k<max; k++) names.swap(k,s-(1+k));

    return names;
}

QString  MangaFox::getChapterName(QXmlStreamReader &xml) {

    QString name;
    bool first = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( first && xml.isStartElement() && xml.name().toString() == "a" && xml.attributes().hasAttribute("href")) {
            name = xml.readElementText();
        }
        if ( xml.attributes().value("class").toString() == "no")
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "h1" )
            first = false;
    }

    return name;
}

QString  MangaFox::getImage(QXmlStreamReader &xml) {

    QString imageUrl;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "img" && xml.attributes().value("id").toString() == "image") {
            imageUrl = xml.attributes().value("src").toString();
        }
    }
    return imageUrl;
}

QStringList  MangaFox::getNextPrev(QXmlStreamReader &xml, QString bUrl) {

    QStringList urls;
    bool first = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if ( first && xml.isStartElement() && xml.name().toString() == "a") {
            urls.append(xml.attributes().value("href").toString());
        }
        if ( xml.name().toString() == "div" && xml.attributes().hasAttribute("id") && xml.attributes().value("id").toString() == "chnav")
            first = true;
        if ( xml.isStartElement() && xml.name().toString() == "div" && xml.attributes().value("id").toString() =="pgnav" )
            first = false;
    }
    if(urls.length()>1) urls.swap(0,1);

    return urls;
}


QString  MangaFox::getNextPageUrl(QXmlStreamReader &xml, const QString &bUrl){

    bool first = false;
    QString getUrl;
    QString nCh;
    QFileInfo fi(bUrl);

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( first && xml.isStartElement() && xml.name().toString() == "a") {
            nCh = xml.attributes().value("href").toString();
        }
        if ( xml.name().toString() == "div" && xml.attributes().value("class").toString() == "read_img")
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "div" )
            first = false;
    }
    getUrl=bUrl;
    getUrl=getUrl.replace(fi.fileName(),nCh);

    return getUrl;
}

int  MangaFox::getLastPage(QXmlStreamReader &xml){

    QString t;
    bool first = false;
    int lastPage = 1;


    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( first && xml.isStartElement() && xml.name().toString() == "option" && xml.attributes().hasAttribute("value")) {
            if( xml.attributes().value("value").toString() != "0"){
                t = xml.attributes().value("value").toString();
                lastPage = t.toInt();
            }
        }
        if (xml.isStartElement() && xml.name().toString() == "select" && xml.attributes().hasAttribute("class"))
            first = true;
        if ( xml.isEndElement() && xml.name().toString() == "select" )
              first = false;
        }

    return lastPage;
}

bool  MangaFox::isChapter(QXmlStreamReader &xml){
    //My solution. It works soo...

    bool isChapter = false;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if ( xml.isStartElement() && xml.name().toString() == "img" && xml.attributes().value("id").toString() == "image") {
            isChapter=true;
        }
    }

    return isChapter;
}
