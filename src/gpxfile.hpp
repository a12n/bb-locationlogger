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

    Q_INVOKABLE bool isOpen() const;
    Q_INVOKABLE void writeStartTrackPoint(double latitude, double longitude); // 1
    Q_INVOKABLE void writeAltitude(double altitude); // 2
    Q_INVOKABLE void writeTimestamp(const QDateTime& timestamp); // 3
    Q_INVOKABLE void writeGeoidHeight(double height);            // 4
    Q_INVOKABLE void writeNumSatellitesUsed(int num);            // 5
    Q_INVOKABLE void writeHdop(double hdop);                     // 6
    Q_INVOKABLE void writeVdop(double vdop);                     // 7
    Q_INVOKABLE void writePdop(double pdop);                     // 8
    Q_INVOKABLE void writeEndTrackPoint();
    Q_INVOKABLE void writeStartExtensions(); // 9
    Q_INVOKABLE void writeTextElement(const QString& name, const QString& text);
    Q_INVOKABLE void writeEndExtensions();

signals:
    void error(const QString& err);

public slots:
    Q_INVOKABLE void open(const QString& baseName);
    Q_INVOKABLE void close();
    Q_INVOKABLE void setSaveOnMediaCard(bool enabled);

private:
    void writeStartGpx();
    void writeEndGpx();
    void checkError();

    static QString fileName(const QString& baseName);

    QFile *file;
    QXmlStreamWriter xml;
    bool saveOnMediaCard;
};

#endif
