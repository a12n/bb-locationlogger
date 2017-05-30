#ifndef GPXDATA_HPP_
#define GPXFILE_HPP_

#include <QObject>
#include <QFile>
#include <QXmlStreamWriter>

class GpxFile : public QObject
{
    Q_OBJECT
public:
    explicit GpxFile(QObject *parent = 0);

    Q_INVOKABLE void open(const QString& fileName);
    Q_INVOKABLE void addTrackPoint(const QDateTime& timestamp, double latitude, double longitude, double altitude);
    Q_INVOKABLE void close();

private:
    QXmlStreamWriter m_xml;
    QFile *m_file;
};

#endif
