# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-mangasailor

CONFIG += sailfishapp

SOURCES += src/harbour-mangasailor.cpp \
    lib/getHTML.cpp \
    lib/mangareader.cpp \
    lib/fileIO.cpp \
    lib/cachemanager.cpp

OTHER_FILES += qml/harbour-mangasailor.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-mangasailor.spec \
    rpm/harbour-mangasailor.yaml \
    translations/*.ts \
    harbour-mangasailor.desktop \
    harbour-mangasailor.png \
    qml/pages/ImagePage.qml \
    qml/pages/LatestUpdates.qml \
    qml/pages/hot.png \
    rpm/harbour-mangasailor.changes \
    qml/html.html \
    qml/pages/MangaPage.qml \
    qml/pages/ChaptersListPage.qml

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-mangasailor-de.ts

HEADERS += \
    lib/getHTML.h \
    lib/mangareader.h \
    lib/fileIO.h \
    lib/cachemanager.h

QT += core

DISTFILES += \
    qml/pages/AllMangas.qml
