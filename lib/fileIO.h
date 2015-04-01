#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileIO : public QObject
{
    Q_OBJECT

public:
    explicit FileIO(QObject *parent = 0);

    Q_INVOKABLE void writeConfig(const QString &filename, const QString &inputText);
    Q_INVOKABLE QString readConfig(const QString &filename);
    Q_INVOKABLE void saveConfig(const QString &property, const QStringList &input);
    Q_INVOKABLE QStringList loadConfig(const QString &property);

};

#endif // FILEIO_H
