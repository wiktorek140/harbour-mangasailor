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

    allowedOrientations: Orientation.Portrait | Orientation.Landscape
    onOrientationChanged: {
        if ( orientation === Orientation.Portrait ) {
            flick.width = parent.width
            flick.height = parent.height
            flick.contentHeight = flick.contentHeight/flick.contentWidth*screen.width
            flick.contentWidth = screen.width
        }
        else {
            flick.width = parent.height
            flick.height = parent.width
            flick.contentHeight = flick.contentHeight/flick.contentWidth*screen.height
            flick.contentWidth = screen.height
        }
        pinchArea.width = Math.max(flick.contentWidth, flick.width)
        pinchArea.height = Math.max(flick.contentHeight, flick.height)
    }

    property var url
    property var mainUrl
    property var mangaName

    property var html: getHTML.getHTML(url)
    property var imgSource: mangaReader.getImage(html)

    property var imgUrl: imgSource[0]
    property int imgWidth: imgSource[1]
    property int imgHeight: imgSource[2]
    property var nextUrl: imgSource[3]
    property var previousUrl: imgSource[4]
    property var endOfChap: imgSource[5]
    property var imgTitle: imgSource[6]
    property int imgPage: imgSource[7]
    property int lastPage: imgSource[8]
    property int imgStatus
    property real imgProgress


    onStatusChanged: {
        if (status == PageStatus.Active && endOfChap === "empty") {
            console.log(nextUrl)
            pageStack.pushAttached(Qt.resolvedUrl("ImagePage.qml"), {url: "http://www.mangareader.net" + nextUrl, mangaName: mangaName, mainUrl: mainUrl})
        }
    }

    Drawer {
        id: drawer

        anchors.fill: parent
        dock: page.isPortrait ? Dock.Top : Dock.Left

        background: SilicaFlickable {
            anchors.fill: parent
            Column {
                anchors.fill: parent

                PageHeader { title: imgTitle }
                PullDownMenu {
                    MenuItem {
                        text: "Latest Manga Update"
                        onClicked: pageStack.pop(pageStack.find( function(page){ return(page._depth === 0) } ))
                    }
                    MenuItem {
                        text: mangaName
                        onClicked: console.log("Go to " + mainUrl)
                    }
                }
                Slider {
                    width: parent.width
                    minimumValue: 1
                    maximumValue: lastPage
                    stepSize: 1
                    value: imgPage
                    label: "Page " + value + " of " + lastPage
                    valueText: value
                    onDownChanged: down === false ? console.log("Go to page " + value) : console.log("Pressed")
                }
            }
        }

        SilicaFlickable {
            anchors.fill: parent

            PullDownMenu {
                MenuItem {
                    text: "Latest Manga Update"
                    onClicked: pageStack.pop(pageStack.find( function(page){ return(page._depth === 0) } ))
                }
                MenuItem {
                    text: mangaName
                    onClicked: console.log("Go to " + mainUrl)
                }
                visible: endOfChap !== "empty"
            }

            VerticalScrollDecorator {}
            HorizontalScrollDecorator {}
            MouseArea {
                enabled: drawer.open
                anchors.fill: flick
                onClicked: drawer.open = false
            }

            ViewPlaceholder {
                enabled: endOfChap !== "empty"
                text: endOfChap + "\nis not Released yet :("
                hintText: "Pull down to go Back to main Page"
            }

            SilicaFlickable {
                id: flick
                width: orientation === Orientation.Portrait ? parent.width : parent.height
                height: orientation === Orientation.Portrait ? parent.height : parent.width
                contentHeight: orientation === Orientation.Portrait ? imgHeight/imgWidth*screen.width : imgHeight/imgWidth*screen.height
                contentWidth: orientation === Orientation.Portrait ? screen.width : screen.height
                enabled: !drawer.opened

                PinchArea {
                    id: pinchArea
                    anchors.fill: parent
                    width: Math.max(flick.contentWidth, flick.width)
                    height: Math.max(flick.contentHeight, flick.height)

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (orientation === Orientation.Portrait) {
                                if (flick.contentWidth === screen.width) {
                                    drawer.open = true
                                    console.log("A")
                                } else {
                                    flick.contentHeight = flick.contentHeight/flick.contentWidth*screen.width
                                    flick.contentWidth = screen.width
                                    console.log("B")
                                }
                            } else {
                                if (flick.contentWidth === screen.height) {
                                    drawer.open = true
                                    console.log("C")
                                } else {
                                    flick.contentHeight = flick.contentHeight/flick.contentWidth*screen.height
                                    flick.contentWidth = screen.height
                                    console.log("D")
                                }
                            }
                        }
                    }

                    property real initialWidth
                    property real initialHeight
                    onPinchStarted: {
                        if (endOfChap === "empty") {
                            initialWidth = flick.contentWidth
                            initialHeight = flick.contentHeight
                        }
                    }
                    onPinchUpdated: {
                        if (endOfChap === "empty") {
                            // adjust content pos due to drag
                            flick.contentX += pinch.previousCenter.x - pinch.center.x
                            flick.contentY += pinch.previousCenter.y - pinch.center.y

                            // resize content
                            if (orientation === Orientation.Portrait && initialWidth * pinch.scale >= screen.width)
                                flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
                            else if(orientation === Orientation.Landscape && initialWidth * pinch.scale >= screen.height)
                                flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
                        }
                    }
                    onPinchFinished: {
                        if (endOfChap === "empty") {
                            // Move its content within bounds.
                            flick.returnToBounds()
                        }


                    }

                    Rectangle {
                        anchors.fill: parent
                        width: flick.contentWidth
                        height: flick.contentHeight
                        color: "transparent"
                        Image {
                            id: image
                            anchors.fill: parent
                            source: imgUrl
                            onStatusChanged: page.imgStatus = image.status
                            onProgressChanged: page.imgProgress = image.progress
                        }
                    }
                }
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            size: BusyIndicatorSize.Large
            running: page.imgStatus === Image.Loading
            visible: running
        }
        Label {
            anchors.centerIn: parent
            text: Math.floor(page.imgProgress*100) + " %"
            visible: page.imgStatus === Image.Loading
        }
    }
}
