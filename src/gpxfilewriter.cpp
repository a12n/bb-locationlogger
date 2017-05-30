#include <QDateTime>
#include <QDir>

#include "gpxfilewriter.hpp"

GpxFileWriter::GpxFileWriter(QObject *parent) :
    QObject(parent),
    file(new QFile(this))
{
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(2);
}

QString GpxFileWriter::fileName() const
{
    return fileBaseName;
}

bool GpxFileWriter::isOpen() const
{
    return file->isOpen();
}

void GpxFileWriter::writeTrackPoint(const QDateTime& timestamp, double latitude, double longitude, double altitude)
{
    if (!isOpen())
        return;

    xml.writeStartElement("trkpt");
    xml.writeAttribute("lat", QString::number(latitude, 'f'));
    xml.writeAttribute("lon", QString::number(longitude, 'f'));
    xml.writeTextElement("time", timestamp.toString(Qt::ISODate));
    xml.writeTextElement("ele", QString::number(altitude, 'f', 2));
    xml.writeEndElement();
}

void GpxFileWriter::setFileName(const QString& fileName)
{
    setOpen(false);
    fileBaseName = fileName;
    file->setFileName(QDir::currentPath() + "/shared/misc/" + fileBaseName);
    emit fileNameChanged();
}

void GpxFileWriter::setOpen(bool flag)
{
    if (isOpen() != flag) {
        if (flag) {
            file->open(QIODevice::WriteOnly | QIODevice::Text);
            xml.setDevice(file);
            writeStartGpx();
        } else {
            writeEndGpx();
            xml.setDevice(0);
            file->close();
        }
        emit openChanged();
    }
}

void GpxFileWriter::writeStartGpx()
{
    xml.writeStartDocument("1.0");
    xml.writeStartElement("gpx");
    xml.writeDefaultNamespace("http://www.topografix.com/GPX/1/1");
    xml.writeAttribute("version", "1.1");
    xml.writeAttribute("creator", "Position Logger");
    xml.writeStartElement("trk");
    xml.writeStartElement("trkseg");
}

void GpxFileWriter::writeEndGpx()
{
    xml.writeEndElement(); // trkseg
    xml.writeEndElement(); // trk
    xml.writeEndElement(); // gpx
    xml.writeEndDocument();
}
