#include <QDateTime>
#include <QDebug>
#include <QDir>

#include "gpxfile.hpp"

#define GNSSLOGGER_NS "urn:gnsslogger:hdg=heading,s=speed,vs=vertSpeed,acc=accuracy,vacc=vertAccuracy"

GpxFile::GpxFile(QObject *parent) :
    QObject(parent),
    file(new QFile(this)),
    xml(),
    saveOnMediaCard(false)
{
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(1);
}

void GpxFile::open(const QString& baseName)
{
    close();

    file->setFileName(fileName(baseName));
    qDebug() << "opening" << file->fileName();
    if (file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        xml.setDevice(file);
        writeStartGpx();
    }

    checkError();
}

bool GpxFile::isOpen() const
{
    return file->isOpen();
}

void GpxFile::writeStartTrackPoint(double latitude, double longitude)
{
    if (!isOpen())
        return;

    xml.writeStartElement("trkpt");
    xml.writeAttribute("lat", QString::number(latitude, 'f', 8));
    xml.writeAttribute("lon", QString::number(longitude, 'f', 8));

    checkError();
}

void GpxFile::writeAltitude(double altitude)
{
    if (!isOpen())
        return;

    xml.writeTextElement("ele", QString::number(altitude, 'f', 2));

    checkError();
}

void GpxFile::writeTimestamp(const QDateTime& timestamp)
{
    if (!isOpen())
        return;

    xml.writeTextElement("time", timestamp.toUTC().toString(Qt::ISODate));

    checkError();
}

void GpxFile::writeGeoidHeight(double height)
{
    if (!isOpen())
        return;

    xml.writeTextElement("geoidheight", QString::number(height, 'f', 2));

    checkError();
}

void GpxFile::writeNumSatellitesUsed(int num)
{
    if (!isOpen())
        return;

    xml.writeTextElement("sat", QString::number(num));

    checkError();
}

void GpxFile::writeHdop(double hdop)
{
    if (!isOpen())
        return;

    xml.writeTextElement("hdop", QString::number(hdop, 'f', 1));

    checkError();
}

void GpxFile::writeVdop(double vdop)
{
    if (!isOpen())
        return;

    xml.writeTextElement("vdop", QString::number(vdop, 'f', 1));

    checkError();
}

void GpxFile::writePdop(double pdop)
{
    if (!isOpen())
        return;

    xml.writeTextElement("pdop", QString::number(pdop, 'f', 1));

    checkError();
}

void GpxFile::writeEndTrackPoint()
{
    if (!isOpen())
        return;

    xml.writeEndElement(); // trkpt

    checkError();
}

void GpxFile::writeStartExtensions()
{
    if (!isOpen())
        return;

    xml.writeStartElement("extensions");

    checkError();
}

void GpxFile::writeTextElement(const QString& name, const QString& text)
{
    if (!isOpen())
        return;

    xml.writeTextElement(GNSSLOGGER_NS, name, text);

    checkError();
}

void GpxFile::writeEndExtensions()
{
    if (!isOpen())
        return;

    xml.writeEndElement(); // extensions

    checkError();
}

void GpxFile::writeComment(const QString& text)
{
    if (!isOpen())
        return;

    xml.writeComment(text);

    checkError();
}

void GpxFile::close()
{
    if (!isOpen())
        return;

    writeEndGpx();
    xml.setDevice(0);
    file->close();
}

void GpxFile::setSaveOnMediaCard(bool enabled)
{
    saveOnMediaCard = enabled;
}

void GpxFile::writeStartGpx()
{
    xml.writeStartDocument("1.0");
    xml.writeStartElement("gpx");
    xml.writeDefaultNamespace("http://www.topografix.com/GPX/1/1");
    xml.writeNamespace(GNSSLOGGER_NS, "x");
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

QString GpxFile::fileName(const QString& baseName) const
{
    // XXX: looks like sdcard directory path is undocumented
    QDir device("shared/misc");
    QDir sdcard("../../removable/sdcard/misc");

    if (saveOnMediaCard && sdcard.mkpath(".")) {
        return sdcard.filePath(baseName);
    } else {
        return device.filePath(baseName);
    }
}
