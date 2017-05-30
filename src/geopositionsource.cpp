#include "geopositionsource.hpp"

GeoPositionSource::GeoPositionSource(QObject *parent) :
    QObject(parent),
    src(QGeoPositionInfoSource::createDefaultSource(this)),
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

QString GeoPositionSource::provider() const
{
    return src->property("provider").toString();
}

bool GeoPositionSource::isStationaryDetectionEnabled() const
{
    return src->property("stationaryDetectionEnabled").toBool();
}

QDateTime GeoPositionSource::timestamp() const
{
    return src->lastKnownPosition().timestamp();
}

double GeoPositionSource::latitude() const
{
    return src->lastKnownPosition().coordinate().latitude();
}

double GeoPositionSource::longitude() const
{
    return src->lastKnownPosition().coordinate().longitude();
}

double GeoPositionSource::altitude() const
{
    return src->lastKnownPosition().coordinate().altitude();
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

void GeoPositionSource::setProvider(const QString& provider)
{
    src->setProperty("provider", provider);
    emit providerChanged();
}

void GeoPositionSource::setStationaryDetectionEnabled(bool enabled)
{
    src->setProperty("stationaryDetectionEnabled", enabled);
    emit stationaryDetectionEnabledChanged();
}

void GeoPositionSource::positionUpdated(const QGeoPositionInfo& pos)
{
    emit positionChanged();
}

void GeoPositionSource::updateTimeout()
{
    // TODO
}
