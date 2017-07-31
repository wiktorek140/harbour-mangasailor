import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.mangasailor.FileIO 1.0
import harbour.mangasailor.MangaReader 1.0
import harbour.mangasailor.GetHTML 1.0

Page {
    id: page

    allowedOrientations: Orientation.Portrait | Orientation.Landscape
    onOrientationChanged: {
        if ( orientation === Orientation.Portrait ) {
            flick.width = parent.width
            flick.height = parent.height - flick.y
            flick.contentHeight = flick.contentHeight/flick.contentWidth*screen.width
            flick.contentWidth = screen.width
        }
        else {
            flick.width = parent.height
            flick.height = parent.width - flick.y
            flick.contentHeight = flick.contentHeight/flick.contentWidth*screen.height
            flick.contentWidth = screen.height
        }
        pinchArea.width = Math.max(flick.contentWidth, flick.width)
        pinchArea.height = Math.max(flick.contentHeight, flick.height)
    }
    Component.onCompleted: getHtml.get(chapUrl)

    backNavigation: !dockedPanel.open && ( ( flick.contentWidth === screen.width && orientation === Orientation.Portrait ) || ( flick.contentWidth === screen.height && orientation === Orientation.Landscape ) )

    property string chapUrl
    property string mainUrl
    property string mangaName
    property string imgTitle

    property int lastPage: 0
    property int loading: 0
    property var nextPrev
    property bool isChapter
    property bool nextIsChapter

    GetHTML {
        id: getHtml
        onHtmlChanged: {
        }
        onFinish: {
            //print(html)
            console.log("Na nowy rozdział")

            isChapter = mangaReader.isChapter(html)
            console.log("Rodzial:",isChapter)
            if ( isChapter && ( loading !== lastPage || lastPage === 0 ) ) {
                loading++
                var imgSrc = mangaReader.getImage(html)
                imgModel.append({
                                    'src': imgSrc
                                })
                if ( imgModel.count === 1 ) {
                    image.source = imgModel.get(0).src
                }
                var nextUrl = mangaReader.getNextPageUrl(html)

                if ( loading === 1 ) {
                    lastPage = mangaReader.getLastPage(html)
                    nextPrev = mangaReader.getNextPrev(html)
                    imgTitle = mangaReader.getChapterName(html)
                }
                if ( loading <= lastPage ) {
                    //console.log("HUH? IT executed?")
                    get(nextUrl)
                }
                console.log(loading + " /" + lastPage)
            } else {
                nextIsChapter = isChapter
                isChapter = true
            }
        }
    }

    property variant mangaChapters: [""]
    property variant chaptersNames: [""]

    GetHTML {
        id: getChaptersHtml
        onHtmlChanged: {
            mangaChapters = mangaReader.getMangaChapters(html)
            chaptersNames = mangaReader.getChaptersNames(html)
        }
    }

    MangaReader { id: mangaReader }

    ListModel {
        id: imgModel
    }
    /*
    SilicaFlickable {
        id: root
        anchors.fill: parent
        contentHeight: column.height
        PullDownMenu {
            MenuItem {
                text: qsTr("Latest Manga Updates")
                onClicked: pageStack.pop(pageStack.find( function(page){ return(page._depth === 0)} ))
            }

            MenuItem {
                text: mangaName
                onClicked: pageStack.push(Qt.resolvedUrl("MangaPage.qml"), {mangaUrl: mainUrl, manga: mangaName})
            }
        }

        Column {
            id: column
            width: parent.width
            PageHeader { title: imgTitle }

            Rectangle {
                id: photoFrame
                width: parent.width * image.scale
                height: image.height * image.scale
                smooth: true
                antialiasing: true
                color: "transparent"
                Image {
                    id: image
                    width: parent.width
                    height: sourceSize.height * width / sourceSize.width
                    antialiasing: true
                    MouseArea {
                        anchors.fill: parent
                        onClicked: dockedPanel.open = !dockedPanel.open
                    }
                }
                PinchArea {
                    anchors.fill: parent
                    pinch.target: photoFrame
                    pinch.minimumScale: 1
                    pinch.maximumScale: 3
                    MouseArea {
                        id: dragArea
                        anchors.fill: parent
                        drag.target: photoFrame
                        enabled: photoFrame.width > column.width
                    }
                }
                Rectangle {
                    width: image.width/5
                    height: image.height
                    color: mousePrev.pressed
                           ? Theme.highlightColor
                           : "transparent"
                    opacity: 0.3
                    anchors {
                        top: image.top
                        left: image.left
                    }
                    MouseArea {
                        id: mousePrev
                        anchors.fill: parent
                        onClicked: {
                            if ( preview.currentIndex !== 0 ) {
                                image.source = imgModel.get(--preview.currentIndex).src
                                root.contentY = 0
                            } else if ( !loadingTimer.running && nextPrev.length > 1) {
                                imgModel.clear()
                                getHtml.get(nextPrev[1])
                                loading = 0
                            }
                        }
                    }
                }
                Rectangle {
                    width: image.width/5
                    height: image.height
                    color: mouseNext.pressed
                           ? Theme.highlightColor
                           : "transparent"
                    opacity: 0.3
                    anchors {
                        top: image.top
                        right: image.right
                    }
                    MouseArea {
                        id: mouseNext
                        anchors.fill: parent
                        onClicked: {
                            if ( preview.currentIndex !== lastPage - 1 ) {
                                image.source = imgModel.get(++preview.currentIndex).src
                                root.contentY = 0
                            }  else if ( !loadingTimer.running && nextIsChapter) {
                                imgModel.clear()
                                getHtml.get(nextPrev[0])
                                loading = 0
                            }
                        }
                    }
                }

            }
        }
    }
*/

    SilicaFlickable {
        id: root
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Latest Manga Updates")
                onClicked: pageStack.pop(pageStack.find( function(page){ return(page._depth === 0)} ))
            }

            MenuItem {
                text: mangaName
                onClicked: pageStack.push(Qt.resolvedUrl("MangaPage.qml"), {mangaUrl: mainUrl, manga: mangaName})
            }
            enabled: !dockedPanel.open && ( ( flick.contentWidth === screen.width && orientation === Orientation.Portrait ) || ( flick.contentWidth === screen.height && orientation === Orientation.Landscape ) )
        }

        PageHeader { id: header; title: imgTitle }


        SilicaFlickable {
            id: flick
            anchors.top: header.bottom
            property real imgHeight: image.sourceSize.height
            property real imgWidth: image.sourceSize.width
            width: orientation === Orientation.Portrait ? parent.width : parent.height
            height: orientation === Orientation.Portrait ? parent.height - flick.y : parent.width - flick.y
            contentHeight: orientation === Orientation.Portrait ? imgHeight/imgWidth*screen.width : imgHeight/imgWidth*screen.height
            contentWidth: orientation === Orientation.Portrait ? screen.width : screen.height
            enabled: !dockedPanel.opened

            PinchArea {
                id: pinchArea
                anchors.fill: parent
                width: Math.max(flick.contentWidth, flick.width)
                height: Math.max(flick.contentHeight, flick.height)
                pinch.maximumScale: 4

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (orientation === Orientation.Portrait) {
                            if (flick.contentWidth === screen.width) {
                                dockedPanel.open = !dockedPanel.open
                                console.log("A")
                            } else {
                                flick.contentHeight = flick.imgHeight/flick.imgWidth*screen.width
                                flick.contentWidth = screen.width
                                console.log("B")
                            }
                        } else {
                            if (flick.contentWidth === screen.height) {
                                dockedPanel.open = !dockedPanel.open
                                console.log("C")
                            } else {
                                flick.contentHeight = flick.imgHeight/flick.imgWidth*screen.height
                                flick.contentWidth = screen.height
                                console.log("D")
                            }
                        }
                    }
                }

                property real initialWidth
                property real initialHeight
                onPinchStarted: {
                    initialWidth = flick.contentWidth
                    initialHeight = flick.contentHeight
                }
                onPinchUpdated: {
                    // adjust content pos due to drag
                    flick.contentX += pinch.previousCenter.x - pinch.center.x
                    flick.contentY += pinch.previousCenter.y - pinch.center.y

                    // resize content
                    if (orientation === Orientation.Portrait && initialWidth * pinch.scale >= screen.width)
                        flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
                    else if(orientation === Orientation.Landscape && initialWidth * pinch.scale >= screen.height)
                        flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
                }
                onPinchFinished: {
                    // Move its content within bounds.
                    flick.returnToBounds()
                }
                function resize(){
                    if ( orientation === Orientation.Portrait ) {
                        flick.contentHeight = flick.imgHeight/flick.imgWidth*screen.width
                        flick.contentWidth = screen.width
                    }
                    else {
                        flick.contentHeight = flick.imgHeight/flick.imgWidth*screen.height
                        flick.contentWidth = screen.height
                    }
                    flick.contentY = 0
                    flick.contentX = 0
                }

                Rectangle {
                    id: photoFrame
                    width: flick.contentWidth
                    height: flick.contentHeight
                    color: "transparent"
                    Image {
                        id: image
                        anchors.fill: parent
                    }
                }
            }
        }


        Rectangle {
            width: parent.width/5
            height: parent.height
            color: mousePrev.pressed
                   ? Theme.highlightColor
                   : "transparent"
            opacity: 0.3
            anchors {
                top: parent.top
                left: parent.left
            }
            MouseArea {
                id: mousePrev
                anchors.fill: parent
                onClicked: {
                    if ( preview.currentIndex !== 0 ) {
                        image.source = imgModel.get(--preview.currentIndex).src
                        pinchArea.resize()
                    } else if ( !loadingTimer.running && nextPrev.length > 1) {
                        imgModel.clear()
                        getHtml.get(nextPrev[1])
                        loading = 0
                    }
                }
            }
        }
        Rectangle {
            width: parent.width/5
            height: parent.height
            color: mouseNext.pressed
                   ? Theme.highlightColor
                   : "transparent"
            opacity: 0.3
            anchors {
                top: parent.top
                right: parent.right
            }
            MouseArea {
                id: mouseNext
                anchors.fill: parent
                onClicked: {
                    if ( preview.currentIndex !== lastPage - 1 ) {
                        image.source = imgModel.get(++preview.currentIndex).src
                        pinchArea.resize()
                    }  else if ( !loadingTimer.running && nextIsChapter) {
                        console.log("Nowy rozdzial")
                        imgModel.clear()
                        getHtml.get(nextPrev[0])
                        preview.currentIndex=0
                        loading = 0
                    }
                }

            }


        }

    }

    BusyIndicator {
        running: true
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        visible: image.status === Image.Loading
    }
    Label {
        anchors.centerIn: parent
        text: Math.floor(image.progress * 100) + " %"
        visible: image.status === Image.Loading
    }

    BusyIndicator {
        running: true
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        visible: imgModel.count === 0
    }
    DockedPanel {
        id: dockedPanel
        Rectangle {
            anchors.fill: parent
            color: Theme.rgba(Theme.highlightDimmerColor,0.95)
        }

        width: parent.width
        height: dockColumn.height

        dock: Dock.Bottom

        Column {
            id: dockColumn
            width: parent.width
            spacing: Theme.paddingMedium

            Rectangle { width: parent.width; height: Theme.paddingMedium; color: "transparent" }

            ListView {
                id: preview
                height: 150
                width: parent.width
                highlightMoveVelocity: 1000
                preferredHighlightBegin: parent.width / 2 - currentItem.width / 2
                preferredHighlightEnd: parent.width / 2 + currentItem.width / 2
                highlightRangeMode: ListView.StrictlyEnforceRange
                orientation: ListView.Horizontal
                model: imgModel
                delegate: Item {
                    id: imgFrame
                    width: img.sourceSize.width > 0
                           ? img.sourceSize.width * parent.height/img.sourceSize.height
                           : preview.height
                    height: preview.height
                    property real sourceHeight: img.sourceSize.height
                    property real sourceWidth: img.sourceSize.width
                    Image {
                        id: img
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: src
                        cache: true
                        opacity: preview.currentIndex === index ? 1 : 0.1
                    }
                    BusyIndicator {
                        running: true
                        size: BusyIndicatorSize.Medium
                        anchors.centerIn: parent
                        visible: img.status === Image.Loading
                    }
                    Label {
                        anchors.centerIn: parent
                        text: Math.floor(img.progress * 100) + " %"
                        font.pixelSize: Theme.fontSizeSmall - 2
                        visible: img.status === Image.Loading
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            image.source = imgModel.get(index).src
                            preview.currentIndex = index
                            pinchArea.resize()
                            dockedPanel.hide()
                        }
                    }
                }
            }
            Row {
                enabled: isChapter
                opacity: isChapter ? 1 : 0.1
                anchors.horizontalCenter: parent.horizontalCenter
                IconButton {
                    icon.source: "image://theme/icon-m-previous?" + (pressed
                                                                     ? Theme.highlightColor
                                                                     : Theme.primaryColor)
                    onClicked: preview.currentIndex = 0
                }
                IconButton {
                    icon.source: "image://theme/icon-m-left?" + (pressed
                                                                 ? Theme.highlightColor
                                                                 : Theme.primaryColor)
                    onClicked: preview.currentIndex !== 0 ? --preview.currentIndex : preview.currentIndex += 0
                }
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    text: (preview.currentIndex + 1) + " / " + preview.count
                }
                Label {
                    id: loadingLabel
                    anchors.verticalCenter: parent.verticalCenter
                }
                Timer {
                    id: loadingTimer
                    running: imgModel.count !== lastPage
                    repeat: true
                    interval: 500
                    onTriggered: {
                        if ( loadingLabel.text === "")
                            loadingLabel.text = "."
                        else if ( loadingLabel.text === ".")
                            loadingLabel.text = ".."
                        else if ( loadingLabel.text === "..")
                            loadingLabel.text = "..."
                        else if ( loadingLabel.text === "...")
                            loadingLabel.text = ""
                    }
                    onRunningChanged: loadingLabel.text = ""
                }
                IconButton {
                    icon.source: "image://theme/icon-m-right?" + (pressed
                                                                  ? Theme.highlightColor
                                                                  : Theme.primaryColor)
                    onClicked: preview.currentIndex !== ( imgModel.count - 1 ) ? ++preview.currentIndex : preview.currentIndex += 0
                }
                IconButton {
                    icon.source: "image://theme/icon-m-next?" + (pressed
                                                                 ? Theme.highlightColor
                                                                 : Theme.primaryColor)
                    onClicked: preview.currentIndex = imgModel.count - 1
                }

            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                IconButton {
                    icon.source: "image://theme/icon-m-page-up?" + (pressed
                                                                    ? Theme.highlightColor
                                                                    : Theme.primaryColor)
                    rotation: -90
                    onClicked: {
                        imgModel.clear()
                        if ( nextPrev.length > 1)
                            getHtml.get(nextPrev[1])
                        loading = 0
                    }
                    opacity: loadingTimer.running
                             ? 0.1
                             : isChapter
                               ? ( nextPrev.length > 1
                                  ? 1
                                  : 0.1 )
                               : 0.1

                    enabled: loadingTimer.running
                             ? false
                             : isChapter
                               ? ( nextPrev.length > 1
                                  ? true
                                  : false )
                               : false
                }
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    text: imgTitle
                }
                IconButton {
                    icon.source: "image://theme/icon-m-page-up?" + (pressed
                                                                    ? Theme.highlightColor
                                                                    : Theme.primaryColor)
                    rotation: 90
                    onClicked: {
                        imgModel.clear()
                        getHtml.get(nextPrev[0])
                        loading = 0
                    }
                    opacity: loadingTimer.running
                             ? 0.1
                             : nextIsChapter
                               ? 1
                               : 0.1

                    enabled: loadingTimer.running
                             ? false
                             : nextIsChapter
                               ? true
                               : false
                }
            }
        }
        /*
        SilicaFlickable {
            anchors.fill: parent
            ListView {
                id: preview

                //onCurrentIndexChanged: slider.value = currentIndex

                width: parent.width
                height: (parent.height - slider.height) / 2
                anchors.bottom: slider.top

                highlightMoveVelocity: 1000
                preferredHighlightBegin: parent.width / 2 - currentItem.width / 2
                preferredHighlightEnd: parent.width / 2 + currentItem.width / 2
                highlightRangeMode: ListView.StrictlyEnforceRange
                orientation: ListView.Horizontal
                model: imgModel
                delegate: Item {
                    id: imgFrame
                    width: img.sourceSize.width > 0
                           ? img.sourceSize.width * parent.height/img.sourceSize.height
                           : preview.height
                    height: preview.height
                    Image {
                        id: img
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: src
                        cache: true
                        opacity: preview.currentIndex === index ? 1 : 0.1
                    }
                    BusyIndicator {
                        running: true
                        size: BusyIndicatorSize.Medium
                        anchors.centerIn: parent
                        visible: img.status === Image.Loading
                    }
                    Label {
                        anchors.centerIn: parent
                        text: Math.floor(img.progress * 100) + " %"
                        font.pixelSize: Theme.fontSizeSmall - 2
                        visible: img.status === Image.Loading
                    }
                }
            }
            Row {
                width: parent.width - Theme.horizontalPageMargin * 2
                x: Theme.horizontalPageMargin
                anchors.bottom: parent.bottom
                IconButton {
                    id: left
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: "image://theme/icon-m-left?" + (pressed
                                                                 ? Theme.highlightColor
                                                                 : Theme.primaryColor)
                    onClicked: preview.currentIndex = 0
                }
                Slider {
                    id: slider
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - left.width - right.width
                    minimumValue: 0
                    maximumValue: imgModel.count - 1
                    value: 0
                    onValueChanged: preview.currentIndex = value
                    stepSize: 1
                    valueText: value + 1
                }
                IconButton {
                    id: right
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: "image://theme/icon-m-right?" + (pressed
                                                                  ? Theme.highlightColor
                                                                  : Theme.primaryColor)
                    onClicked: preview.currentIndex = imgModel.count - 1
                }
            }

            BusyIndicator {
                running: true
                size: BusyIndicatorSize.Medium
                anchors.centerIn: parent
                visible: imgModel.count === 0
            }
        }*/
    }
}
