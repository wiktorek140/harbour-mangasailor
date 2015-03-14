#include "../lib/fileIO.h"
#include <QDir>
#include <QStringList>
#include <QStandardPaths>

//Constructor
FileIO::FileIO(QObject *parent) :
    QObject(parent)
{}


//Your custom function
void FileIO::write(const QString &filename, const QString &inputText) {
    QString dataLocation(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir dir(dataLocation);
    if (!dir.exists())
        dir.mkpath(".");
    QDir::setCurrent(dataLocation);
    QFile file( filename );
    QTextStream(stdout) << "Write Path: " << QDir::currentPath() << endl;
    if ( file.open(QIODevice::ReadWrite) ) {
        QTextStream stream( &file );
        stream << inputText << endl;
    }
    return;
}

QString FileIO::read(const QString &filename) {
    QString dataLocation(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir dir(dataLocation);
    if (!dir.exists())
        dir.mkpath(".");
    QDir::setCurrent(dataLocation);
    QString outputText;
    QFile file( filename );
    QTextStream(stdout) << "Read Path: " << QDir::currentPath() << endl;
    if ( file.exists()) {
        if ( file.open(QIODevice::ReadWrite) ) {
            QTextStream stream( &file );
            stream >> outputText;
        }
    }
    else {
        outputText = '0';
    }
    QTextStream(stdout) << filename << ": " << outputText << endl;
    return outputText;
}

void FileIO::save(const QString &property, const QStringList &input) {
    QTextStream cout(stdout);

    QString dataLocation(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir dir(dataLocation);
    if (!dir.exists())
        dir.mkpath(".");
    QDir::setCurrent(dataLocation);
    cout << "Save Path: " << QDir::currentPath() << endl;
    QFile file( property );
    if (file.exists())
        file.remove();
    if ( file.open(QIODevice::ReadWrite) ) {
        QTextStream stream( &file );
        for (int i = 0; i < input.length(); i++) {
            stream << input.at(i) << endl;
            cout << input.at(i) << endl;
        }
    }
    return;
}

QStringList FileIO::load(const QString &property) {
    QTextStream cout(stdout);

    QString dataLocation(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir dir(dataLocation);
    if (!dir.exists())
        dir.mkpath(".");
    QDir::setCurrent(dataLocation);
    cout << "Load Path: " << QDir::currentPath() << endl;
    QFile file( property );
    QStringList loading;
    if(file.exists()) {
        if ( file.open(QIODevice::ReadWrite) ) {
            QTextStream stream( &file );
            while ( !stream.atEnd() ) {
                loading << stream.readLine();
            }
        }
        file.close();
    } else {
        loading.append("null");
    }
    for ( int i = 0; i < loading.length(); i++)
        cout << loading[i] << " ";
    cout << endl;
    return loading;
}
