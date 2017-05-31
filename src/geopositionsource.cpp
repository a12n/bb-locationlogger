#include "geopositionsource.hpp"

GeoPositionSource::GeoPositionSource(QObject *parent) :
    QObject(parent),
    src(QGeoPositionInfoSource::createDefaultSource(this)),
    curMethod("all"),
    active(false)
{
    src->setProperty("canRunInBackground", true);

    bool ok;

    ok = connect(src, SIGNAL(positionUpdated(const QGeoPositionInfo&)), this, SLOT(positionUpdated(const QGeoPositionInfo&)));
    Q_ASSERT(ok);

    ok = connect(src, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
    Q_ASSERT(ok);

    Q_UNUSED(ok);
}

bool GeoPositionSource::isActive() const
{
    return active;
}

double GeoPositionSource::period() const
{
    return src->property("period").toDouble();
}

QString GeoPositionSource::method() const
{
    return curMethod;
}

bool GeoPositionSource::isStationaryDetectionEnabled() const
{
    return src->property("stationaryDetectionEnabled").toBool();
}

QDateTime GeoPositionSource::timestamp() const
{
    return pos.timestamp();
}

double GeoPositionSource::latitude() const
{
    return pos.coordinate().latitude();
}

double GeoPositionSource::longitude() const
{
    return pos.coordinate().longitude();
}

double GeoPositionSource::altitude() const
{
    return pos.coordinate().altitude();
}

void GeoPositionSource::setActive(bool active)
{
    if (this->active != active) {
        if (active)
            src->startUpdates();
        else
            src->stopUpdates();
        this->active = active;
        emit activeChanged();
    }
}

void GeoPositionSource::setPeriod(double period)
{
    src->setProperty("period", period);
    emit periodChanged();
}

void GeoPositionSource::setMethod(const QString& method)
{
    if (curMethod == method)
        return;

    if (method == "all") {
        src->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
    } else if (method == "nonSatellite") {
        src->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
    } else if (method == "satellite") {
        src->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
    } else if (method == "gps") {
        src->setProperty("provider", "gnss");
        src->setProperty("fixType", "gps_autonomous");
    } else if (method == "cellular") {
        src->setProperty("provider", "network");
        src->setProperty("fixType", "cellsite");
    } else if (method == "wifi") {
        src->setProperty("provider", "network");
        src->setProperty("fixType", "wifi");
    } else if (method == "msb") {
        src->setProperty("provider", "gnss");
        src->setProperty("fixType", "gps_ms_based");
    } else if (method == "msa") {
        src->setProperty("provider", "gnss");
        src->setProperty("fixType", "gps_ms_assisted");
    } else {
        // Invalid method
        return;
    }

    curMethod = method;
    emit methodChanged();
}

void GeoPositionSource::setStationaryDetectionEnabled(bool enabled)
{
    src->setProperty("stationaryDetectionEnabled", enabled);
    emit stationaryDetectionEnabledChanged();
}

void GeoPositionSource::positionUpdated(const QGeoPositionInfo& pos)
{
    this->pos = pos;
    emit positionChanged();
}

void GeoPositionSource::updateTimeout()
{
    // TODO
}
