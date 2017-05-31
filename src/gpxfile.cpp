#include <QDateTime>

#include "gpxfile.hpp"

GpxFile::GpxFile(QObject *parent) :
    QObject(parent),
    file(new QFile(this)),
    xml()
{
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(1);
}

void GpxFile::open(const QString& baseName)
{
    close();

    file->setFileName("shared/misc/" + baseName);
    if (file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        xml.setDevice(file);
        writeStartGpx();
    }

    checkError();
}

void GpxFile::writeStartTrackPoint(const QDateTime& timestamp, double latitude, double longitude)
{
    if (!file->isOpen())
        return;

    xml.writeStartElement("trkpt");
    xml.writeAttribute("lat", QString::number(latitude, 'f', 8));
    xml.writeAttribute("lon", QString::number(longitude, 'f', 8));
    xml.writeTextElement("time", timestamp.toString(Qt::ISODate));

    checkError();
}

void GpxFile::writeAltitude(double altitude)
{
    xml.writeTextElement("ele", QString::number(altitude, 'f', 2));
    checkError();
}

void GpxFile::writeGeoidHeight(double height)
{
    xml.writeTextElement("geoidheight", QString::number(height, 'f', 2));
    checkError();
}

void GpxFile::writeNumSatellitesUsed(int num)
{
    xml.writeTextElement("sat", QString::number(num));
    checkError();
}

void GpxFile::writeHdop(double hdop)
{
    xml.writeTextElement("hdop", QString::number(hdop, 'f'));
    checkError();
}

void GpxFile::writeVdop(double vdop)
{
    xml.writeTextElement("vdop", QString::number(vdop, 'f'));
    checkError();
}

void GpxFile::writePdop(double pdop)
{
    xml.writeTextElement("pdop", QString::number(pdop, 'f'));
    checkError();
}

void GpxFile::writeEndTrackPoint()
{
    xml.writeEndElement(); // trkpt
    checkError();
}

void GpxFile::close()
{
    if (!file->isOpen())
        return;

    writeEndGpx();
    xml.setDevice(0);
    file->close();
}

void GpxFile::writeStartGpx()
{
    xml.writeStartDocument("1.0");
    xml.writeStartElement("gpx");
    xml.writeDefaultNamespace("http://www.topografix.com/GPX/1/1");
    xml.writeAttribute("version", "1.1");
    xml.writeAttribute("creator", "gnsslogger");
    xml.writeStartElement("trk");
    xml.writeStartElement("trkseg");
}

void GpxFile::writeEndGpx()
{
    xml.writeEndElement(); // trkseg
    xml.writeEndElement(); // trk
    xml.writeEndElement(); // gpx
    xml.writeEndDocument();
    checkError();
}

void GpxFile::checkError()
{
    switch (file->error()) {
        case QFile::NoError :
            break;
        case QFile::WriteError :
            emit error("write");
            break;
        case QFile::FatalError :
            emit error("fatal");
            break;
        case QFile::OpenError :
            emit error("open");
            break;
        default :
            emit error("unknown");
            break;
    }
}
