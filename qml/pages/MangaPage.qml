import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.mangasailor.FileIO 1.0
import harbour.mangasailor.GetHTML 1.0
import harbour.mangasailor.MangaReader 1.0

Page {
    id: page

    FileIO { id: fileIO }
    GetHTML {
        id: getHTML
        onHtmlChanged: {
            if (source === 0){
                imageUrl = mangaReader.getMangaImage(html)
                infos = mangaReader.getInfos(html)
                genres = mangaReader.getGenres(html)
                description = mangaReader.getDescription(html)
            }
        }
    }

    property int source: 0 // 0 = mangareader.com
    MangaReader { id: mangaReader }

    onStatusChanged: {
        if ( status === PageStatus.Activating )
            getHTML.get(mangaUrl)
        if( status === PageStatus.Active )
            pageStack.pushAttached(Qt.resolvedUrl("ChaptersListPage.qml"), {mainUrl: mangaUrl, mangaName: manga})
    }

    property string mangaUrl
    property string manga
    property string imageUrl
    property var infoHeaders : ["Alternate Name:", "Year of Release:", "Status:", "Author:", "Artist:", "Reading Direction:"]
    property var infos
    property var genres
    property string description

    property var favourites: fileIO.loadConfig("favourites")

    function addFavourite(title) {
        favourites.push(title)
        fileIO.saveConfig("favourites", favourites)
        console.log(favourites)
    }

    function removeFavourite(title) {
        var index = favourites.indexOf(title)
        favourites.splice(index,1)
        fileIO.saveConfig("favourites", favourites)
        console.log(favourites)
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: page
        contentHeight: column.height
/*
        PullDownMenu {
            MenuItem {
                text: "Add " + manga + " to Favourites"
                onClicked: {
                    if ( favourites.indexOf(manga) === -1 )
                        addFavourite(manga)
                    else
                        removeFavourite(manga)
                }
            }
        }
*/
        Column {
            id: column
            width: parent.width - Theme.paddingLarge*2
            spacing: 20
            anchors {
                left: parent.left
                leftMargin: Theme.paddingLarge
            }
            PageHeader {
                id: pageHeader
                title: manga
            }
            Rectangle {
                width: parent.width
                height: width*2/3
                anchors.horizontalCenter: parent.horizontalCenter
                color: "transparent"
                Image {
                    id: image
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: imageUrl
                }
                BusyIndicator {
                    anchors.centerIn: parent
                    running: image.status == Image.Loading
                    size: BusyIndicatorSize.Medium
                    visible: image.status == Image.Loading
                }
            }
            Label {
                text: "Info"
                color: Theme.highlightColor
            }
            Column {
                id: infosColumn
                width: parent.width
                Label {
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    text: infoHeaders[0] + " " + infos[0]
                    visible: text !== infoHeaders[0] + " "
                    wrapMode: Text.WordWrap
                }
                Label {
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    text: infoHeaders[1] + " " + infos[1]
                    visible: text !== infoHeaders[1] + " "
                    wrapMode: Text.WordWrap
                }
                Label {
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    text: infoHeaders[2] + " " + infos[2]
                    visible: text !== infoHeaders[2] + " "
                    wrapMode: Text.WordWrap
                }
                Label {
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    text: infoHeaders[3] + " " + infos[3]
                    visible: text !== infoHeaders[3] + " "
                    wrapMode: Text.WordWrap
                }
                Label {
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    text: infoHeaders[4] + " " + infos[4]
                    visible: text !== infoHeaders[4] + " "
                    wrapMode: Text.WordWrap
                }
                Label {
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    text: infoHeaders[5] + " " + infos[5]
                    visible: text !== infoHeaders[5] + " "
                    wrapMode: Text.WordWrap
                }

            }

            Label {
                text: "Genres"
                color: Theme.highlightColor
            }
            Grid {
                width: parent.width
                columnSpacing: Theme.paddingMedium
                rowSpacing: Theme.paddingMedium
                Repeater {
                    id: repeater
                    model: genres.length
                    delegate: Label {
                        font.pixelSize: Theme.fontSizeSmall
                        text: genres[index]
                    }

                }
            }
            Label {
                text: "Description"
                color: Theme.highlightColor
            }
            Label {
                id: descriptionLabel
                width: parent.width
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                text: description
            }
        }
        VerticalScrollDecorator { flickable: flickable }
    }

}
