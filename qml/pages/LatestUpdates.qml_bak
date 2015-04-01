/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.mangasailor.FileIO 1.0
import harbour.mangasailor.GetHTML 1.0
import harbour.mangasailor.MangaReader 1.0


Page {
    id: page

    FileIO { id: fileIO }
    GetHTML { id: getHTML }
    MangaReader { id: mangaReader }

    //onStatusChanged: { if ( status === PageStatus.Active ) listView.enabled = true }

    property int source: 0 // 0 = mangareader.com

    property string searchString: ""

    property string html: getHTML.getHTML("http://www.mangareader.net/")

    property var mangaList: mangaReader.mangaList(html)
    property var chaptersList: mangaReader.chaptersList(html)
    property var linksList: mangaReader.linksList(html)
    property var mangalinksList: mangaReader.mangalinksList(html)
    property var iconsList: mangaReader.iconsList(html)
    property var favourites: fileIO.loadConfig("favourites")
    property var favouritesList: getFavourites(mangaList,favourites)

    function getFavourites(mangaList,favourites) {
        var favList = []
        for (var i = 0; i < mangaList.length; i++) {
            for ( var j = 0; j < favourites.length; j++) {
                if( mangaList[i] === favourites[j]) {
                    favList[i] = "true"
                    break;
                } else {
                    favList[i] = "false"
                }
            }
        }
        return favList
    }

    function addFavourite(title) {
        favourites.push(title)
        fileIO.saveConfig("favourites", favourites)
        favouritesList = getFavourites(mangaList,favourites)
    }

    function removeFavourite(title) {
        var index = favourites.indexOf(title)
        favourites.splice(index,1)
        fileIO.saveConfig("favourites", favourites)
        favouritesList = getFavourites(mangaList,favourites)
    }

    function refresh() {
        console.log("Called refresh")
        listModel.clear()
        favouritesList = getFavourites(mangaList,favourites)
        for( var i = 0; i < mangaList.length ; i++ )
            if ( searchString === "" || mangaList[i].indexOf(searchString) >= 0) {
                listModel.append({
                                     "chapter": chaptersList[i],
                                     "manga": mangaList[i],
                                     "link": linksList[i],
                                     "mangalink": mangalinksList[i]
                                 })
            }
    }

    function getUpdates() {
        html = getHTML.getHTML("http://www.mangareader.net/")
        mangaList = mangaReader.mangaList(html)
        chaptersList = mangaReader.chaptersList(html)
        linksList = mangaReader.linksList(html)
        mangalinksList = mangaReader.mangalinksList(html)
        iconsList = mangaReader.iconsList(html)

        refresh()
    }

    ListModel {
        id: listModel
        Component.onCompleted: refresh()
    }

    Component {
        id: sectionHeading
        BackgroundItem{
            width: parent.width
            height: label.height*2
            property int index: mangaList.indexOf(section)
            Row{
                id: row
                x: Theme.paddingLarge
                width: parent.width-2*x
                height: parent.height
                property bool three: ( newIcon.visible && hot.visible && favourite.visible )
                property bool two: ( ( newIcon.visible && hot.visible ) || ( hot.visible && favourite.visible ) || ( newIcon.visible &&favourite.visible ) )
                property bool one: ( newIcon.visible || hot.visible || favourite.visible )
                Label {
                    id: label
                    width: row.three ? row.width - favourite.width*3 : row.two ? row.width - favourite.width*2 : row.one ? row.width - favourite.width : row.width
                    anchors.verticalCenter: parent.verticalCenter
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeMedium
                    text: section
                }
                Image {
                    id: favourite
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://theme/icon-m-favorite-selected"
                    visible: favouritesList[index] === "true"
                }
                Image {
                    id: newIcon
                    height: favourite.height
                    width: favourite.width
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://theme/icon-l-clock"
                    visible: iconsList[index] === "new"
                }
                Image {
                    id: hot
                    anchors.verticalCenter: parent.verticalCenter
                    source: "hot.png"
                    visible: iconsList[index] === "hot"
                }
            }
            onPressAndHold:{
                favourite.visible = !favourite.visible
                if (favouritesList[index] === "true")
                    removeFavourite(section)
                else
                    addFavourite(section)
              }
            onClicked: console.log("Go to: " + mangalinksList[index])
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: "Get Updates"
                onClicked: getUpdates()
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: searchString === "" ? "No content" : "No manga found"
            hintText: searchString === "" ? "Pull down to Refresh" : "The search is case sensitive!"
        }

        header: Column {
            width: page.width
            PageHeader {
                id: pageHeader
                title: source === 0 ? qsTr("MangaReader.net") : qsTr("UI Template")
            }
            Label {
                x: Theme.paddingMedium
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.secondaryHighlightColor
                text: qsTr("Latest Updates")
            }
            SearchField {
                id: searchField
                width: parent.width
                placeholderText: "Search"
                onTextChanged: {
                    page.searchString = text
                    refresh()
                }
            }
        }

        currentIndex: -1

        model: listModel
        delegate: ListItem {
            id: listDelegate
            Label {
                x: Theme.paddingLarge*2
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-2*x
                horizontalAlignment: _fadeText ? Text.AlignRight : Text.AlignLeft
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.secondaryColor
                text: chapter
            }
            onClicked: {
                console.log("Clicked " + link)
                listView.enabled = false
                clickTimer.start()
            }
            Timer {
                id: clickTimer
                interval: 1
                repeat: false
                running: false
                onTriggered: {
                    pageStack.push(Qt.resolvedUrl("ImagePage.qml"), {url: "http://www.mangareader.net" + link, mainUrl: "http://www.mangareader.net" + mangalink, mangaName: manga})
                    listView.enabled = true
                }
            }
        }

        section.property: "manga"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading

        VerticalScrollDecorator {}
    }
}




