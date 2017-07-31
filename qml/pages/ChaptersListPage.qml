import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.mangasailor.FileIO 1.0
import harbour.mangasailor.GetHTML 1.0
import harbour.mangasailor.MangaReader 1.0

Page {
    id: page
    Component.onCompleted: getHTML.get(mainUrl)

    FileIO { id: fileIO }
    GetHTML {
        id: getHTML
        onHtmlChanged: {
            mangaChapters = mangaReader.getMangaChapters(html)
            chaptersNames = mangaReader.getChaptersNames(html)
        }
    }
    MangaReader { id: mangaReader }

    property string mainUrl
    property string mangaName

    property var mangaChapters
    property var chaptersNames

    onChaptersNamesChanged: {
        for( var i = 0; i < chaptersNames.length ; i++ ) {
            listModel.append({
                                 "name": chaptersNames[i],
                                 "link": mangaChapters[i],
                             })
        }
    }

    ListModel {
        id: listModel
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: listModel
        header: Column {
            width: page.width
            PageHeader { title: mangaName }
            Label {
                x: Theme.paddingMedium
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.secondaryHighlightColor
                text: qsTr("All ") + mangaName + qsTr(" Chapters")
            }
        }
        delegate: ListItem {
            id: listDelegate
            Label {
                x: Theme.paddingLarge*2
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-2*x
                horizontalAlignment: _fadeText ? Text.AlignRight : Text.AlignLeft
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeMedium
                text: name
            }
            onClicked: {
                pageStack.push(Qt.resolvedUrl("ChapReader.qml"), {chapUrl: link, mainUrl: mainUrl, mangaName: mangaName, imgTitle: name})
            }
            Timer {
                id: clickTimer
                interval: 1
                repeat: false
                running: false
                onTriggered: {
                    listView.enabled = true
                }
            }
        }
        VerticalScrollDecorator {}
    }
}
