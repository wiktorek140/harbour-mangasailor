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
import harbour.mangasailor.CacheManager 1.0


Page {
    id: page

    FileIO { id: fileIO }
    GetHTML {
        id: getHTML
        onHtmlChanged: {
            if (source === 0){
                //print(html);
                mangaList = mangaReader.mangaList(html)
                chaptersList = mangaReader.chaptersList(html)
                linksList = mangaReader.linksList(html)
                mangalinksList = mangaReader.mangalinksList(html)
                iconsList = mangaReader.iconsList(html)
                favouritesList = getFavourites(mangaList,favourites)
            }
            refresh()
        }
    }

    MangaReader { id: mangaReader }
    CacheManager { id: cacheManager }

    onStatusChanged: {
        if ( status === PageStatus.Active ) {
            getHTML.get("http://www.mangareader.net/alphabetical")
            //cacheManager.clearCache()
            refresh()
        }
    }

    property int source: 0 // 0 = mangareader.com

    property string searchString: ""

    property string html

    property var mangaList
    property var chaptersList
    property var linksList
    property var mangalinksList
    property var iconsList
    property var favourites: fileIO.loadConfig("favourites")
    property var favouritesList

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
        html = getHTML.getHTML("http://www.mangareader.net/latest")
        print(html);
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
                spacing: Theme.paddingSmall
                property bool three: ( newIcon.visible && hot.visible && favourite.visible )
                property bool two: ( ( newIcon.visible && hot.visible ) || ( hot.visible && favourite.visible ) || ( newIcon.visible &&favourite.visible ) )
                property bool one: ( newIcon.visible || hot.visible || favourite.visible )
                property real iconsLength: {
                    var length = Theme.paddingLarge
                    if ( newIcon.visible )
                        length += newIcon.width
                    if ( hot.visible )
                        length += hot.width
                    if ( favourite.visible )
                        length += favourite.width
                    return length
                }
                Label {
                    id: label
                    width: row.width - row.iconsLength
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
                Rectangle {
                    id: newIcon
                    height: 40
                    width: 70
                    anchors.verticalCenter: parent.verticalCenter
                    color: "transparent"
                    border.color: Theme.secondaryHighlightColor
                    radius: 10
                    Label {
                        anchors.centerIn: parent
                        color: Theme.secondaryHighlightColor
                        text: qsTr("New")
                    }
                    visible: iconsList[index] === "new"
                }
                Rectangle {
                    id: hot
                    height: 40
                    width: 60
                    anchors.verticalCenter: parent.verticalCenter
                    color: "transparent"
                    border.color: Theme.secondaryHighlightColor
                    radius: 10
                    Label {
                        anchors.centerIn: parent
                        color: Theme.secondaryHighlightColor
                        text: qsTr("Hot")
                    }
                    visible: iconsList[index] === "hot"
                }
            }
            onPressAndHold:{
                favourite.visible = !favourite.visible
                console.log(newIcon.height + " " + newIcon.width)
                console.log(hot.height + " " + hot.width)
                if (favouritesList[index] === "true")
                    removeFavourite(section)
                else
                    addFavourite(section)
            }
            onClicked: {
                console.log("Go to: " + mangalinksList[index])
                pageStack.push(Qt.resolvedUrl("MangaPage.qml"), {mangaUrl: "http://www.mangareader.net" + mangalinksList[index], manga: mangaList[index]})
            }
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        PullDownMenu {

            MenuItem {
                text: "Get Updates"
                onClicked: getHTML.get("http://www.mangareader.net/latest")
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
                    pageStack.push(Qt.resolvedUrl("ImagePage.qml"), {chapUrl: "http://www.mangareader.net" + link, mainUrl: "http://www.mangareader.net" + mangalink, mangaName: manga, imgTitle: chapter})
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




