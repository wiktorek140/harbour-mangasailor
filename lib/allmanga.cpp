#include "allmanga.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QStringList>


AllManga::AllManga(QObject *parent) :
    QObject(parent)
{}


QStringList AllManga::mangaList(const QString &html,int w) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList manga;
    QString activeManga;
    QString name;

    bool first = false;
    bool second = false;
    bool third = false;
    int counter=0;

    switch(w){
    case 0:
    {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();

            if (xml.isStartElement()) {
                name = xml.name().toString();
                if (name == "ul" && xml.attributes().value("class") == "series_alpha")
                    first = true;
                if ( name == "li")
                    second = true;
                if ( name == "a")
                    third = true;
                if (first && second && third) {
                    attributes = xml.attributes();
                    if(attributes.hasAttribute("href") ) {
                        activeManga = xml.readElementText();
                        manga.append(activeManga);
                        cout<<"Readed: "<< activeManga <<endl;
                        counter++;
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
        break; }

    case 1: {
        /*while (!xml.atEnd() && !xml.hasError()) {
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
                    attributes = xml.attributes();
                    if(attributes.hasAttribute("href") ) {
                        activeManga = xml.readElementText();
                        manga.append(activeManga);
                        cout<<"Readed: "<< activeManga <<endl;
                        counter++;
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
        }*/

        manga = mangaFox.mangaList(xml);
        break; }

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


// Debrecated and unusable but still there
QStringList AllManga::chaptersList(const QString &html ,int w) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList chapters;
    QString activeChapter;

    //cout << xml.readElementText()<< endl;

    switch(w){
    case 0:
    {
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
        break;
    }
    case 1: {
        chapters = mangaFox.chaptersList(xml);
        break;
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

QStringList AllManga::linksList(const QString &html,int w) {

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList links;

    QString activeLink;
    QString name;
    bool first = false;
    bool second = false;
    bool third = false;
    //int counter=0;

    switch(w){
    case 0:
    {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            if (xml.isStartElement()) {
                name = xml.name().toString();
                if (name == "ul" && xml.attributes().value("class") == "series_alpha")
                    first = true;
                if ( name == "li")
                    second = true;
                if ( name == "a")
                    third = true;
                if (first && second && third) {
                    attributes = xml.attributes();
                    if(attributes.hasAttribute("href") && attributes.value("href").toString() != "") {
                        activeLink = attributes.value("href").toString();
                        links.append(activeLink);
                        //counter++;
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
        break; }

    case 1: {
        /*while (!xml.atEnd() && !xml.hasError()) {
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
                    attributes = xml.attributes();
                    if(attributes.hasAttribute("href") && attributes.value("href").toString() != "") {
                        activeLink = attributes.value("href").toString();
                        links.append(activeLink);
                        //counter++;
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
        }*/
        links = mangaFox.linksList(xml);
        break; }
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

QStringList AllManga::mangalinksList(const QString &html,int w) {

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

    return linksList(html,w);
}

QStringList AllManga::iconsList(const QString &html,int w) {

    QTextStream cout(stdout); // set cout for console debug

    QXmlStreamReader xml(specialParse(html));
    QStringList icons;
    QString icon;
    bool first = false,second = false, third= false;
    QString name;
    //int c=0;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError() ) {
            xml.readNext();
            if (xml.isStartElement()) {
                name = xml.name().toString();
                if (name == "ul" && xml.attributes().value("class") == "series_alpha")
                    first = true;
                if ( name == "li")
                    second = true;
                if ( name == "a")
                    third = true;
                if (first && second && third) {
                    icon = attributes.value("class").toString();
                } else icon="none";
                attributes = xml.attributes();
                if(attributes.hasAttribute("href") && attributes.value("href").toString() != "") {
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

        break; }

    case 1: {
        /*while (!xml.atEnd() && !xml.hasError() ) {
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
                    icon = attributes.value("class").toString();
                } else icon="none";
                attributes = xml.attributes();
                if(attributes.hasAttribute("href") && attributes.value("href").toString() != "") {
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
        }*/

        icons = mangaFox.iconsList(xml);
        break; }

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

QString AllManga::getMangaImage(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html,true));
    QString imageUrl;
    QString name;
    bool first = false;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            if (xml.isStartElement()) {
                name = xml.name().toString();
                attributes = xml.attributes();
                if ( name == "img")
                    imageUrl = attributes.value("src").toString();
            }
        }

        break; }
    case 1: {

        /*while (!xml.atEnd() && !xml.hasError() ) {
            xml.readNext();
            if (xml.isStartElement()) {
                name = xml.name().toString();
                if (name == "div" && xml.attributes().value("class").toString() =="cover")
                    first = true;

                attributes = xml.attributes();
                if(first && attributes.hasAttribute("src") && attributes.value("src").toString() != "") {
                    imageUrl = attributes.value("src").toString();
                }
            }
            if (xml.isEndElement()) {
                name = xml.name().toString();
                if ( name == "div")
                    first = false;

            }
        }*/
        imageUrl = mangaFox.getMangaImage(xml);
        break; }

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

QStringList AllManga::getInfos(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList infos;
    //QString alt="";
    QString name;
    bool first = false,second = false, h3 = false;

    switch(w){
    case 0: {
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

        break;
    }
    case 1: {
        /*while (!xml.atEnd() && !xml.hasError() ) {
            xml.readNext();
            if (xml.isStartElement()) {
                name = xml.name().toString();
                if (name == "div" && xml.attributes().value("id").toString() == "title")
                    first = true;
                if (name == "td" && xml.attributes().value("valign").toString()== "top")
                    second = true;
                attributes = xml.attributes();
                if(first && xml.name().toString() == "h3") {
                    h3=true;
                    infos.append(xml.readElementText());
                }
                if(first && second && !h3 && xml.name().toString() == "a" && attributes.hasAttribute("href") && attributes.hasAttribute("style")){
                    if(infos.isEmpty())infos.append(" ");
                    infos.append(xml.readElementText());
                    if(infos.length() == 2) infos.append("Temp");
                    if(infos.length() == 5) infos.append("Right to Left");
                }

            }
            if (xml.isEndElement()) {
                name = xml.name().toString();
                if ( name == "div")
                    first = false;
                if ( name == "h3")
                    h3 = false;
                if ( name == "td")
                    second = false;

            }
        }*/

        infos = mangaFox.getInfos(xml);
        break; }
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

QStringList AllManga::getGenres(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QStringList genres;
    QString genre;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( attributes.value("class").toString() == "genretags") {
                genre =  xml.readElementText();
                genres.append(genre);
            }
        }

        break;
    }
    case 1: {
        genres = mangaFox.getGenres(xml);
        break;}

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

QString AllManga::getDescription(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QString description;

    switch(w){
    case 0: {

        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            if ( xml.name().toString() == "p")
                description = xml.readElementText();
        }
        break;
    }
    case 1: {
        description = mangaFox.getDescription(xml);
        break;}

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

QStringList AllManga::getMangaChapters(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    //QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(specialParse(html));
    QStringList urls;
    QString getUrl;
    bool first = false,second=false;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a") {
                getUrl = "http://www.mangareader.net" + attributes.value("href").toString();
                urls.append(getUrl);
            }
            if ( attributes.value("id").toString() == "listing")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "table" )
                first = false;
        }

        break; }

    case 1: {

        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            //cout<< xml.name().toString()<<endl;
            //cout<< xml.attributes().value("id")<<endl;

            if(xml.isStartElement()){
                attributes = xml.attributes();
                if(xml.name().toString() == "li") first=true;
                if(xml.name().toString() == "div") second = true;
                if(first && second && xml.name().toString() == "a" && attributes.hasAttribute("class") && attributes.hasAttribute("href")) {
                    if(attributes.value("class").toString() == "tips"){
                        //cout<<attributes.value("href").toString()<<endl;
                        getUrl = attributes.value("href").toString();
                        urls.append(getUrl);
                    }
                    //getUrl = attributes.value("href").toString();

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
        */
        urls = mangaFox.getMangaChapters(xml);
        break; }

    }
    //cout<< xml.name().toString()<<endl;
    //cout<< xml.attributes().value("content")<<endl;
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

QStringList AllManga::getChaptersNames(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    //QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(specialParse(html));
    QStringList names;
    //QString aNames;
    bool first = false,second=false;
    QString name;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a") {

                name= xml.readElementText();
                cout<< name<<endl;
                names.append(name);
            }
            if ( attributes.value("id").toString() == "listing")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "table" )
                first = false;
        }

        break;
    }
    case 1: {
        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();

            if(xml.isStartElement()){
                attributes = xml.attributes();
                if(xml.name().toString() == "li") first=true;
                if(xml.name().toString() == "div") second = true;
                if(first && second && xml.name().toString() == "a" && attributes.hasAttribute("class") && attributes.hasAttribute("href")) {
                    //
                    if(attributes.value("class").toString() == "tips"){

                        name= xml.readElementText() ;
                        //cout <<name<<endl;
                        names.append(name);
                    }
                    //getUrl = attributes.value("href").toString();

                }
            }

            if(xml.isEndElement()){
                if(xml.name().toString() == "li")
                    first=false;
                if(xml.name().toString() == "div")
                    second=false;
            }

        }

        for(int k=0, s=names.size(), max=(s/2); k<max; k++) names.swap(k,s-(1+k));*/

        names = mangaFox.getChaptersNames(xml);
        break; }

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

QString AllManga::getChapterName(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    //QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(specialParse(html));
    QString name;
    bool first = false;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a" && attributes.hasAttribute("title")) {
                name = xml.readElementText();
            }
            if ( attributes.value("class").toString() == "c2")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "select" )
                first = false;
        }

        break; }
    case 1: {

        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a" && attributes.hasAttribute("href")) {
                name = xml.readElementText();
            }
            if ( attributes.value("class").toString() == "no")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "h1" )
                first = false;
        }*/
        name = mangaFox.getChapterName(xml);
        break; }

    }

    if (xml.hasError())
    {
        cout << "XML error12: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    return name;
}

QString AllManga::getImage(const QString &html,int w) {
    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    QString imageUrl;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            if ( xml.isStartElement() && xml.name().toString() == "img") {
                attributes = xml.attributes();
                imageUrl.append(attributes.value("src").toString());
            }
        }

        break;
    }
    case 1: {

        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            if ( xml.isStartElement() && xml.name().toString() == "img" && xml.attributes().value("id").toString() == "image") {
                attributes = xml.attributes();
                imageUrl = attributes.value("src").toString();
            }
        }*/
        imageUrl = mangaFox.getImage(xml);
        break; }

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

QStringList AllManga::getNextPrev(const QString &html, const QString &bUrl, int w) {
    QTextStream cout(stdout); // set cout for console debug
    //QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(specialParse(html));
    QStringList urls;
    bool first = false;
    QString getUrl;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a") {
                getUrl = bUrl + attributes.value("href").toString();
                urls.append(getUrl);
            }
            if ( attributes.value("class").toString() == "c5" && xml.name().toString() == "table")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "table" )
                first = false;
        }

        break; }
    case 1: {
        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a") {
                getUrl = attributes.value("href").toString();
                cout<<"URL:"<<getUrl<<endl;
                urls.append(getUrl);
            }
            if ( xml.name().toString() == "div" && xml.attributes().hasAttribute("id") && xml.attributes().value("id").toString() == "chnav")
                first = true;
            if ( xml.isStartElement() && xml.name().toString() == "div" && xml.attributes().value("id").toString() =="pgnav" )
                first = false;
        }
        if(urls.length()>1){
            urls.swap(0,1);
            //getUrl= urls.at(0);
            //urls.replace(0,urls.at(1));
            //urls.replace(1,getUrl);
        }*/
        urls = mangaFox.getNextPrev(xml,bUrl);
        break; }

    }


    if (xml.hasError())
    {
        cout << "XML error17: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    //cout << "1:"<<urls.at(0)<< endl<<"2: "<<urls.at(1)<<endl;
    return urls;
}


QString AllManga::getNextPageUrl(const QString &html, const QString &bUrl, int w){

    // <span class="next"><a href="/b-shock/49/2">Next</a></span>

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    bool first = false;
    QString getUrl;
    QString nCh;
    //cout<<html<<endl;
    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a") {
                getUrl = bUrl + attributes.value("href").toString();
                //cout<< getUrl<<endl;

            }
            if ( xml.name().toString() == "span" && attributes.value("class").toString() == "next")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "span" )
                first = false;
        }

        break; }

    case 1: {
        /*QFileInfo fi(bUrl);
        QString fileName = fi.fileName();
        //cout<<fileName<<endl;


        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "a") {
                nCh = attributes.value("href").toString();
            }
            if ( xml.name().toString() == "div" && attributes.value("class").toString() == "read_img")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "div" )
                first = false;
        }
        getUrl=bUrl;
        getUrl=getUrl.replace(fileName,nCh);
        //cout<<getUrl<<endl;*/
        getUrl = mangaFox.getNextPageUrl(xml,bUrl);
        break; }

    }


    if (xml.hasError())
    {
        cout << "XML error21: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }

    //cout<< "NEXT:" <<getUrl<< endl;
    return getUrl;
}
int AllManga::getLastPage(const QString &html,int w){

    QTextStream cout(stdout); // set cout for console debug
    //QString html = getHtml.getHTML(url);
    QXmlStreamReader xml(specialParse(html));
    QString t;
    bool first = false;
    int lastPage = 0 ;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "option" && attributes.hasAttribute("value")) {

                //t = xml.readElementText();
                lastPage = xml.readElementText().toInt();
                //lastPage= t.toInt();
                //cout<< "Text:"<<t<<endl;

            }
            if ( attributes.value("id").toString() == "selectpage" && xml.isStartElement() && xml.name().toString() == "div")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "select" )
                first = false;
        }

        break; }
    case 1: {
        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "option" && attributes.hasAttribute("value")) {

                //t = xml.readElementText();
                if( attributes.value("value").toString() != "0"){
                    //cout<<"Number: "<<xml.readElementText()<<endl;
                    t= attributes.value("value").toString();
                    //cout<<"Number2: "<<t<<endl;
                    lastPage = t.toInt();
                    //cout<<"Number3: "<<lastPage<<endl;
                    //lastPage = xml.readElementText().toInt();
                }

                //lastPage= t.toInt();
                //cout<< "LAST:"<<t<<endl;

            }
            if (xml.isStartElement() && xml.name().toString() == "select" && attributes.hasAttribute("class"))
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "select" )
                first = false;
        }*/
        lastPage = mangaFox.getLastPage(xml);
        break; }
    }

    if (xml.hasError())
    {
        cout << "XML error19: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    //lastPage=15;
    cout <<"LAST:"<< lastPage <<endl;

    return lastPage;
}

bool AllManga::isChapter(const QString &html,int w){
    //My solution. It works soo...

    QTextStream cout(stdout); // set cout for console debug
    QXmlStreamReader xml(specialParse(html));
    //QString t;
    bool isChapter = false, isA=false;
    bool first = false;

    switch(w){
    case 0: {
        while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            attributes = xml.attributes();
            if ( first && xml.isStartElement() && xml.name().toString() == "option") {
                //t = xml.readElementText();
                if(attributes.hasAttribute("selected")) isA=true;
                //cout<< "Text:"<<t<<endl;

            }
            if ( attributes.value("id").toString() == "selectpage" && xml.isStartElement() && xml.name().toString() == "div")
                first = true;
            if ( xml.isEndElement() && xml.name().toString() == "select" )
                first = false;
        }

        break; }

    case 1: {

        /*while (!xml.atEnd() && !xml.hasError()) {
            xml.readNext();
            if ( xml.isStartElement() && xml.name().toString() == "img" && xml.attributes().value("id").toString() == "image") {
                //attributes = xml.attributes();
                //imageUrl = attributes.value("src").toString();
                isA=true;
            }
            if (xml.isStartElement()){}
        }*/

        isChapter = mangaFox.isChapter(xml);
        break; }

    }


    if (xml.hasError())
    {
        cout << "XML error20: " << xml.errorString() << endl;
    }
    else if (xml.atEnd())
    {
        cout << "Reached end, done" << endl;
    }
    if (isA) isChapter=true;
    return isChapter;
}


// Used to fix & symbol
QString AllManga::specialParse(QString input, bool img) {
        if(img) input.replace(QRegularExpression("&(?!amp;)"),"&amp;");
        input.replace(QRegularExpression("&'")," ");
        input.replace(QRegularExpression("<img .*\\\">"),"Dummy");

        input.replace("<br>","<br/>");
        input.replace(">_<","&gt;_&lt;");

        return input;
}


