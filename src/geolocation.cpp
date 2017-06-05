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
            filter_ = new KalmanFilter();
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
    data_.timestamp.setMSecsSinceEpoch(geolocation_event_get_utc_time(event));
    data_.timestamp.setTimeSpec(Qt::UTC);
    data_.latitude = geolocation_event_get_latitude(event);
    data_.longitude = geolocation_event_get_longitude(event);
    data_.altitude = geolocation_event_get_altitude(event);
    data_.horizAccuracy = geolocation_event_get_accuracy(event);
    data_.vertAccuracy = geolocation_event_get_altitude_accuracy(event);
    data_.heading = geolocation_event_get_heading(event);
    data_.speed = geolocation_event_get_speed(event);
    data_.vertSpeed = geolocation_event_get_vertical_speed(event);
    data_.numSatellitesUsed = geolocation_event_get_num_satellites_used(event);
    data_.numSatellitesTotal = geolocation_event_get_num_satellites_total(event);
    data_.hdop = geolocation_event_get_hdop(event);
    data_.vdop = geolocation_event_get_vdop(event);
    data_.pdop = geolocation_event_get_pdop(event);
    data_.geoidHeight = geolocation_event_get_geoid_height(event);
    qDebug() << "timestamp" << data_.timestamp;
    qDebug() << "coordinate" << data_.latitude << data_.longitude << data_.altitude;
    qDebug() << "heading and speed" << data_.heading << data_.speed << data_.vertSpeed;
    qDebug() << "accuracy" << data_.horizAccuracy << data_.vertAccuracy;
    qDebug() << "num satellites" << data_.numSatellitesUsed << "/" << data_.numSatellitesTotal;
    if (filter_) {
        filter_->update(data_.timestamp.toMSecsSinceEpoch(),
                        data_.latitude, data_.longitude,
                        data_.horizAccuracy,
                        data_.speed);
        data_.latitude = filter_->latitude();
        data_.longitude = filter_->longitude();
        data_.horizAccuracy = filter_->accuracy();
        qDebug() << "filter cooridnate " << data_.latitude << data_.longitude;
        qDebug() << "filter accuracy" << data_.horizAccuracy;
    }
    emit dataChanged();
}

void GeoLocation::statusEvent(bps_event_t *event)
{
    Q_UNUSED(event);
}
