#ifndef GPXFILEWRITER_HPP_
#define GPXFILEWRITER_HPP_

#include <QObject>
#include <QFile>
#include <QXmlStreamWriter>

class GpxFileWriter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(bool open READ isOpen WRITE setOpen NOTIFY openChanged)
    // TODO: error
public:
    explicit GpxFileWriter(QObject *parent = 0);

    QString fileName() const;
    bool isOpen() const;

    Q_INVOKABLE void writeTrackPoint(const QDateTime& timestamp, double latitude, double longitude, double altitude);

signals:
    void fileNameChanged();
    void openChanged();

public slots:
    void setFileName(const QString& fileName);
    void setOpen(bool flag);

private:
    void writeStartGpx();
    void writeEndGpx();

    QString fileBaseName;
    QFile *file;
    QXmlStreamWriter xml;
};

#endif
