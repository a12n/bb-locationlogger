#include <cmath>

#include <bps/bps.h>
#include <bps/geolocation.h>

#include <QDebug>

#include "geolocation.hpp"

GeoLocationData::GeoLocationData() :
    latitude(NAN),
    longitude(NAN),
    altitude(NAN),
    horizAccuracy(NAN),
    vertAccuracy(NAN),
    heading(NAN),
    speed(NAN),
    vertSpeed(NAN),
    hdop(NAN),
    vdop(NAN),
    pdop(NAN),
    geoidHeight(NAN),
    timestamp(),
    numSatellitesUsed(0),
    numSatellitesTotal(0)
{
}

GeoLocationData::GeoLocationData(bps_event_t *event) :
    latitude(geolocation_event_get_latitude(event)),
    longitude(geolocation_event_get_longitude(event)),
    altitude(geolocation_event_get_altitude(event)),
    horizAccuracy(geolocation_event_get_accuracy(event)),
    vertAccuracy(geolocation_event_get_altitude_accuracy(event)),
    heading(geolocation_event_get_heading(event)),
    speed(geolocation_event_get_speed(event)),
    vertSpeed(geolocation_event_get_vertical_speed(event)),
    hdop(geolocation_event_get_hdop(event)),
    vdop(geolocation_event_get_vdop(event)),
    pdop(geolocation_event_get_pdop(event)),
    geoidHeight(geolocation_event_get_geoid_height(event)),
    numSatellitesUsed(geolocation_event_get_num_satellites_used(event)),
    numSatellitesTotal(geolocation_event_get_num_satellites_total(event))
{
    timestamp.setMSecsSinceEpoch(geolocation_event_get_utc_time(event));
    timestamp.setTimeSpec(Qt::UTC);
}

double GeoLocationData::distanceTo(const GeoLocationData& other) const
{
    using std::asin;
    using std::cos;
    using std::sin;
    using std::sqrt;

    const double earth = 6371007.2;
    const double rad = M_PI / 180;

    const double a = sin(rad * (latitude - other.latitude) / 2);
    const double b = sin(rad * (longitude - other.longitude) / 2);
    const double d = 2 * asin(sqrt(a * a + cos(rad * latitude) * cos(rad * other.latitude) * b * b));

    return d * earth;
}

//----------------------------------------------------------------------------

GeoLocationDataFilter::~GeoLocationDataFilter()
{
}

struct SimpleFilter : public GeoLocationDataFilter
{
    virtual bool eval(GeoLocationData& next)
    {
        const double dist = prev.distanceTo(next);
        if (isnan(dist) || dist > (prev.horizAccuracy + next.horizAccuracy)) {
            prev = next;
            return true;
        } else {
            return false;
        }
    }

    GeoLocationData prev;
};

//----------------------------------------------------------------------------

GeoLocation::GeoLocation(QObject *parent) :
    QObject(parent),
    data_(),
    filter_(0)
{
    subscribe(geolocation_get_domain());
    bps_initialize();
    // TODO: check return values
    geolocation_set_provider(GEOLOCATION_PROVIDER_GNSS);
    geolocation_set_background(true);
    geolocation_set_report_satellite_info(true);
}

GeoLocation::~GeoLocation()
{
    bps_shutdown();
    delete filter_;
}

void GeoLocation::event(bps_event_t *event)
{
    if (!event)
        return;

    switch (bps_event_get_code(event)) {
        case GEOLOCATION_INFO :
            infoEvent(event);
            break;
        case GEOLOCATION_ERROR :
            errorEvent(event);
            break;
        case GEOLOCATION_STATUS :
            statusEvent(event);
            break;
    }
}

void GeoLocation::startUpdates()
{
    qDebug() << __FUNCTION__;
    geolocation_request_events(0);
    // TODO: check return value
}

void GeoLocation::stopUpdates()
{
    qDebug() << __FUNCTION__;
    geolocation_stop_events(0);
    // TODO: check return value
}

void GeoLocation::setAccuracy(unsigned int accuracy)
{
    qDebug() << __FUNCTION__ << accuracy;
    geolocation_set_accuracy(accuracy);
}

void GeoLocation::setPeriod(unsigned int period)
{
    qDebug() << __FUNCTION__ << period;
    geolocation_set_period(period);
}

void GeoLocation::setResponseTime(unsigned int responseTime)
{
    qDebug() << __FUNCTION__ << responseTime;
    geolocation_set_response_time(responseTime);
}

void GeoLocation::setStationaryDetection(bool enabled)
{
    qDebug() << __FUNCTION__ << enabled;
    geolocation_set_stationary(enabled);
}

void GeoLocation::setFilterData(bool enabled)
{
    qDebug() << __FUNCTION__ << enabled;
    if (enabled) {
        if (!filter_)
            filter_ = new SimpleFilter();
    } else {
        if (filter_) {
            delete filter_;
            filter_ = 0;
        }
    }
}

void GeoLocation::errorEvent(bps_event_t *event)
{
    switch (geolocation_event_get_error_code(event)) {
        case GEOLOCATION_ERROR_FATAL_DISABLED :
            emit error("disabled");
            break;
        case GEOLOCATION_ERROR_FATAL_NO_LAST_KNOWN_POSITION :
            emit error("noLastKnownPosition");
            break;
        case GEOLOCATION_ERROR_FATAL_INSUFFICIENT_PROVIDERS :
            emit error("insufficientProviders");
            break;
        case GEOLOCATION_ERROR_FATAL_INVALID_REQUEST :
            emit error("invalidRequest");
            break;
        case GEOLOCATION_ERROR_FATAL_PERMISSION :
            emit error("permission");
            break;
        case GEOLOCATION_ERROR_WARN_TIMEOUT :
            emit warning("timeout");
            break;
        case GEOLOCATION_ERROR_WARN_LOST_TRACKING :
            emit warning("lostTracking");
            break;
        case GEOLOCATION_ERROR_WARN_STATIONARY :
            emit warning("stationary");
            break;
    }
    qDebug() << "error message " << geolocation_event_get_error_message(event);
}

void GeoLocation::infoEvent(bps_event_t *event)
{
    GeoLocationData newData(event);
    bool changed = true;
    qDebug() << "raw timestamp" << newData.timestamp;
    qDebug() << "raw coordinate" << newData.latitude << newData.longitude << newData.altitude;
    qDebug() << "raw heading and speed" << newData.heading << newData.speed << newData.vertSpeed;
    qDebug() << "raw accuracy" << newData.horizAccuracy << newData.vertAccuracy;
    qDebug() << "raw num satellites" << newData.numSatellitesUsed << "/" << newData.numSatellitesTotal;
    if (filter_) {
        changed = filter_->eval(newData);
        qDebug() << "filter cooridnate " << newData.latitude << newData.longitude;
        qDebug() << "filter accuracy" << newData.horizAccuracy;
    }
    if (changed) {
        data_ = newData;
        emit dataChanged();
    }
}

void GeoLocation::statusEvent(bps_event_t *event)
{
    Q_UNUSED(event);
}
