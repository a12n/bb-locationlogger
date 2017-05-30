#include "gpxfile.hpp"

#include <QDir>
#include <QDateTime>

GpxFile::GpxFile(QObject *parent) :
    QObject(parent),
    m_xml(),
    m_file()
{
    m_xml.setAutoFormatting(true);
    m_xml.setAutoFormattingIndent(2);
}

void GpxFile::open(const QString& fileName)
{
    const QString filePath =
            QDir::currentPath() + "/shared/misc/" + fileName;

    m_file = new QFile(filePath, this);
    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    m_xml.setDevice(m_file);
    m_xml.writeStartDocument("1.0");
    m_xml.writeStartElement("gpx");
    m_xml.writeDefaultNamespace("http://www.topografix.com/GPX/1/1");
    m_xml.writeAttribute("version", "1.1");
    m_xml.writeAttribute("creator", "Position Logger");
    m_xml.writeStartElement("trk");
    m_xml.writeStartElement("trkseg");
}

void GpxFile::addTrackPoint(const QDateTime& timestamp, double latitude, double longitude, double altitude)
{
    if (!m_file)
        return;

    m_xml.writeStartElement("trkpt");
    m_xml.writeAttribute("lat", QString::number(latitude, 'f'));
    m_xml.writeAttribute("lon", QString::number(longitude, 'f'));
    m_xml.writeTextElement("time", timestamp.toString(Qt::ISODate));
    m_xml.writeTextElement("ele", QString::number(altitude, 'f'));
    m_xml.writeEndElement();
}

void GpxFile::close()
{
    if (!m_file)
        return;

    m_xml.writeEndElement(); // trkseg
    m_xml.writeEndElement(); // trk
    m_xml.writeEndElement(); // gpx
    m_xml.writeEndDocument();
    m_xml.setDevice(0);

    m_file->close();
    delete m_file;
    m_file = 0;
}
