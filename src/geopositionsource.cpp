#include "geopositionsource.hpp"

GeoPositionSource::GeoPositionSource(QObject *parent) :
    QObject(parent),
    m_src(QGeoPositionInfoSource::createDefaultSource(this)),
    m_active(false)
{
    m_src->setProperty("canRunInBackground", true);

    bool ok;

    ok = connect(m_src, SIGNAL(positionUpdated(const QGeoPositionInfo&)), this, SLOT(positionUpdated(const QGeoPositionInfo&)));
    Q_ASSERT(ok);

    ok = connect(m_src, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
    Q_ASSERT(ok);

    Q_UNUSED(ok);
}

bool GeoPositionSource::isActive() const
{
    return m_active;
}

double GeoPositionSource::period() const
{
    return m_src->property("period").toDouble();
}

QString GeoPositionSource::provider() const
{
    return m_src->property("provider").toString();
}

bool GeoPositionSource::isStationaryDetectionEnabled() const
{
    return m_src->property("stationaryDetectionEnabled").toBool();
}

QDateTime GeoPositionSource::timestamp() const
{
    return m_src->lastKnownPosition().timestamp();
}

double GeoPositionSource::latitude() const
{
    return m_src->lastKnownPosition().coordinate().latitude();
}

double GeoPositionSource::longitude() const
{
    return m_src->lastKnownPosition().coordinate().longitude();
}

double GeoPositionSource::altitude() const
{
    return m_src->lastKnownPosition().coordinate().altitude();
}

void GeoPositionSource::setActive(bool active)
{
    if (m_active != active) {
        if (active)
            m_src->startUpdates();
        else
            m_src->stopUpdates();
        m_active = active;
        emit activeChanged();
    }
}

void GeoPositionSource::setPeriod(double period)
{
    m_src->setProperty("period", period);
    emit periodChanged();
}

void GeoPositionSource::setProvider(const QString& provider)
{
    m_src->setProperty("provider", provider);
    emit providerChanged();
}

void GeoPositionSource::setStationaryDetectionEnabled(bool enabled)
{
    m_src->setProperty("stationaryDetectionEnabled", enabled);
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
