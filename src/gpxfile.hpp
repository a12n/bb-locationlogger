#ifndef GPXFILE_HPP_
#define GPXFILE_HPP_

#include <QFile>
#include <QObject>
#include <QXmlStreamWriter>

class GpxFile : public QObject
{
    Q_OBJECT

public:
    explicit GpxFile(QObject *parent = 0);

    Q_INVOKABLE void open(const QString& baseName);
    Q_INVOKABLE bool isOpen() const;
    Q_INVOKABLE void writeStartTrackPoint(const QDateTime& timestamp, double latitude, double longitude);
    Q_INVOKABLE void writeAltitude(double altitude);
    Q_INVOKABLE void writeGeoidHeight(double height);
    Q_INVOKABLE void writeNumSatellitesUsed(int num);
    Q_INVOKABLE void writeHdop(double hdop);
    Q_INVOKABLE void writeVdop(double vdop);
    Q_INVOKABLE void writePdop(double pdop);
    Q_INVOKABLE void writeEndTrackPoint();
    Q_INVOKABLE void writeStartExtensions(const QString& ns);
    Q_INVOKABLE void writeTextElement(const QString& name, const QString& text);
    Q_INVOKABLE void writeEndExtensions();
    Q_INVOKABLE void close();

signals:
    void error(const QString& err);

private:
    void writeStartGpx();
    void writeEndGpx();
    void checkError();

    static QString fileName(const QString& baseName);

    QFile *file;
    QXmlStreamWriter xml;
};

#endif
