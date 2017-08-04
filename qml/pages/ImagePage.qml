import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.mangasailor.FileIO 1.0
import harbour.mangasailor.GetHTML 1.0
import harbour.mangasailor.MangaReader 1.0
import harbour.mangasailor.AllManga 1.0

Page {
    id: page
    backNavigation: !loading && ( drawer.open || notReleased || ( prevUrl === "null" && currentPage === 0 ) )

    property bool loading: true
    property bool notReleased
    property int source

    FileIO { id: fileIO }
    GetHTML { id: getHTML}


    MangaReader { id: mangaReader }
    AllManga { id: allManga }
    //backNavigation: false

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

    property var chapUrl
    property var mainUrl
    property var mangaName

    property var mangaChapters: allManga.getMangaChapters(mainUrl,source)
    onMangaChaptersChanged: console.log("MANGA CHAPTERS\n" + mangaChapters)
    property var chaptersNames: allManga.getChaptersNames(mainUrl,source)
    onChaptersNamesChanged: console.log("CHAPTERS NAMES\n" + chaptersNames)

    onStatusChanged: {
        if ( status == PageStatus.Active ) {
            getChap(chapUrl)
        }
    }

    property var html

    property var urls
    onUrlsChanged: if (urls.length === 0 ) notReleased = true; else notReleased = false

    property int currentPage: 0

    property string imgTitle

    property var imgSources: []
    property var imgWidths: []
    property var imgHeights: []

    property string prevUrl
    property string nextUrl


    function nextPage() {
        if( currentPage < urls.length - 1 ) {
            ++currentPage
            image.source = imgSources[currentPage]
            flick.contentHeight = orientation === Orientation.Portrait ? imgHeights[currentPage]/imgWidths[currentPage]*screen.width : imgHeights[currentPage]/imgWidths[currentPage]*screen.height
            flick.contentY = 0
            slider.value = currentPage + 1
        } else if (!loading) {
            getChap(nextUrl)
        }
    }

    function prevPage() {
        if ( currentPage > 0 ) {
            --currentPage
            image.source = imgSources[currentPage]
            flick.contentHeight = orientation === Orientation.Portrait ? imgHeights[currentPage]/imgWidths[currentPage]*screen.width : imgHeights[currentPage]/imgWidths[currentPage]*screen.height
            flick.contentY = 0
            slider.value = currentPage + 1
        } else if (!loading && prevUrl !== "null") {
            getChap(prevUrl)
        }
    }

    function getChap(url) {
        imgSources.splice(0,imgSources.length)
        imgWidths.splice(0,imgWidths.length)
        imgHeights.splice(0,imgHeights.length)
        slider.maximumValue = 20
        slider.value = 1
        currentPage = 0
        image.source = "null"
        chapUrl = url
        imgTitle = allManga.getTitle(url)
        html = getHTML.getHTML(url)
        urls = allManga.getUrls(html)
        timer.pages = urls.length
        slider.maximumValue = urls.length
        timer.index = 0
        loading = true
        timer.start()
    }


    Timer {
        id: timer
        interval: 800
        running: false
        repeat: true
        property int pages
        property int index
        onTriggered: {
            if (index < pages) {
                for ( var i = index; i >= 0 ; i--)
                    if ( typeof imgSources[i] === "undefined" || typeof imgHeights[i] === "undefined" || typeof imgWidths[i] === "undefined" )
                        var current = i
                if (typeof current === "undefined")
                    current = 0
                index = current
                console.log(index + " /" + pages + " /" + urls[index])
                imgSources[index] = allManga.getImage(urls[index])
                imgHeights[index] = allManga.getImgHeight(urls[index])
                imgWidths[index]  = allManga.getImgWidth(urls[index])
                if ( index === 0) {
                    image.source = imgSources[index]
                    flick.contentHeight = orientation === Orientation.Portrait ? imgHeights[index]/imgWidths[index]*screen.width : imgHeights[index]/imgWidths[index]*screen.height
                }
                index++
            } else if ( index == pages ) {
                loading = false
                timer.running = false
                nextUrl = mangaReader.getNextPrev(chapUrl)[0]
                prevUrl = typeof mangaReader.getNextPrev(chapUrl)[1] === "undefined" ? "null" : mangaReader.getNextPrev(chapUrl)[1]
                console.log ("next/prev " + nextUrl + "/" + prevUrl)
                //console.log(imgSources)
            }
        }
    }

    property int imgStatus
    property real imgProgress

    Drawer {
        id: drawer

        anchors.fill: parent
        dock: page.isPortrait ? Dock.Top : Dock.Left

        background: SilicaFlickable {
            anchors.fill: parent
            Column {
                anchors.fill: parent

                PageHeader { title: mangaName }
                PullDownMenu {
                    id: pullDownMenu
                    MenuItem {
                        text: "Latest Manga Update"
                        onClicked: pageStack.pop() //pageStack.find( function(page){ return(page._depth === 0) } ))
                        enabled: !loading
                    }
                    MenuItem {
                        text: mangaName
                        onClicked: pageStack.push(Qt.resolvedUrl("MangaPage.qml"), {mangaUrl: mainUrl, manga: mangaName})
                        enabled: !loading
                    }
                    busy: loading
                }
                ComboBox {
                    id: comboBox
                    width: page.width
                    label: "Chapter"
                    currentIndex: chaptersNames.indexOf(imgTitle)
                    enabled: !loading

                    menu: ContextMenu {
                        Repeater {
                            model: chaptersNames.length
                            delegate: MenuItem { text: chaptersNames[index] }
                        }
                    }
                    onCurrentIndexChanged: getChap(mangaChapters[currentIndex])
                }
                Slider {
                    id: slider
                    width: parent.width
                    minimumValue: 1
                    maximumValue: 20
                    stepSize: 1
                    value: currentPage + 1
                    label: "Page " + value + " of " + maximumValue
                    valueText: value
                    property int prevValue
                    onDownChanged: {
                        if ( !down ) {
                            if ( typeof imgSources[value-1] !== "undefined") {
                                currentPage = value - 1
                                image.source = imgSources[currentPage]
                                flick.contentHeight = orientation === Orientation.Portrait ? imgHeights[currentPage]/imgWidths[currentPage]*screen.width : imgHeights[currentPage]/imgWidths[currentPage]*screen.height
                            } else {
                                sliderTimer.start()
                            }
                        } else
                            console.log("Pressed")
                    }
                }
                Timer {
                    id: sliderTimer
                    interval: 10
                    repeat: false
                    running: false
                    onTriggered: slider.value = currentPage + 1

                }
            }
        }

        SilicaFlickable {
            anchors.fill: parent

            VerticalScrollDecorator {}
            HorizontalScrollDecorator {}
            MouseArea {
                enabled: drawer.open
                anchors.fill: flick
                onClicked: drawer.open = false
            }

            ViewPlaceholder {
                enabled: ( image.status === Image.Null || image.status === Image.Error ) && !notReleased
                text: "Loading " + imgTitle + ",\nplease wait."
            }
            ViewPlaceholder {
                enabled: notReleased
                text: imgTitle + " is not released yet :("
            }

            PullDownMenu {
                MenuItem {
                    text: "Latest Manga Update"
                    onClicked: pageStack.pop(pageStack.find( function(page){ return(page._depth === 0) } ))
                }
                MenuItem {
                    text: mangaName
                    onClicked: pageStack.push(Qt.resolvedUrl("MangaPage.qml"), {mangaUrl: mainUrl, manga: mangaName})
                }
                enabled: notReleased
                visible: notReleased
            }

            SilicaFlickable {
                id: flick
                width: orientation === Orientation.Portrait ? parent.width : parent.height
                height: orientation === Orientation.Portrait ? parent.height : parent.width
                //contentHeight: orientation === Orientation.Portrait ? imgHeights[currentPage]/imgWidths[currentPage]*screen.width : imgHeights[currentPage]/imgWidths[currentPage]*screen.height
                contentWidth: orientation === Orientation.Portrait ? screen.width : screen.height
                enabled: !drawer.opened && !notReleased

                PinchArea {
                    id: pinchArea
                    anchors.fill: parent
                    width: Math.max(flick.contentWidth, flick.width)
                    height: Math.max(flick.contentHeight, flick.height)

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent

                        property real prevX
                        property real prevY

                        onPressed: {
                            prevX = mouseX
                            prevY = mouseY
                        }
                        onReleased: {
                            if ( Math.abs(prevY-mouseY) < Theme.paddingMedium &&
                                    Math.abs(prevX-mouseX) < Theme.paddingMedium) { // Click
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
                            else if ( Math.abs(prevY-mouseY) < Math.abs(prevX-mouseX)) {
                                if (orientation === Orientation.Portrait && flick.contentWidth === screen.width ) {
                                    if ( mouseX > prevX) { // Swipe Right
                                        prevPage()
                                        comboBox.currentIndex = chaptersNames.indexOf(imgTitle)
                                    } else {               // Swipe Left
                                        nextPage()
                                        comboBox.currentIndex = chaptersNames.indexOf(imgTitle)
                                    }
                                }
                                else if (orientation === Orientation.Landscape && flick.contentWidth === screen.height ) {
                                    if ( mouseX > prevX) { // Swipe Right
                                        prevPage()
                                        comboBox.currentIndex = chaptersNames.indexOf(imgTitle)
                                    } else {               // Swipe Left
                                        nextPage()
                                        comboBox.currentIndex = chaptersNames.indexOf(imgTitle)
                                    }
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

                    Rectangle {
                        anchors.fill: parent
                        width: flick.contentWidth
                        height: flick.contentHeight
                        color: "transparent"
                        Image {
                            id: image
                            fillMode: Image.PreserveAspectFit
                            anchors.fill: parent
                            cache: true
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
