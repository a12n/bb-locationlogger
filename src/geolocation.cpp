#include <cmath>

#include <bps/bps.h>
#include <bps/geolocation.h>

#include <QDebug>

#include "geolocation.hpp"

GeoLocation::GeoLocation(QObject *parent) :
    QObject(parent),
    latitude_(NAN),
    longitude_(NAN),
    altitude_(NAN),
    horizAccuracy_(NAN),
    vertAccuracy_(NAN),
    heading_(NAN),
    speed_(NAN),
    vertSpeed_(NAN),
    hdop_(NAN),
    vdop_(NAN),
    pdop_(NAN),
    geoidHeight_(NAN),
    timestamp_(),
    numSatellitesUsed_(0),
    numSatellitesTotal_(0),
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
    geolocation_request_events(0);
    // TODO: check return value
}

void GeoLocation::stopUpdates()
{
    geolocation_stop_events(0);
    // TODO: check return value
}

void GeoLocation::setAccuracy(unsigned int accuracy)
{
    geolocation_set_accuracy(accuracy);
}

void GeoLocation::setPeriod(unsigned int period)
{
    geolocation_set_period(period);
}

void GeoLocation::setResponseTime(unsigned int responseTime)
{
    geolocation_set_response_time(responseTime);
}

void GeoLocation::setStationaryDetection(bool enabled)
{
    geolocation_set_stationary(enabled);
}

void GeoLocation::setSmoothData(bool enabled)
{
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
    timestamp_.setMSecsSinceEpoch(geolocation_event_get_utc_time(event));
    timestamp_.setTimeSpec(Qt::UTC);
    latitude_ = geolocation_event_get_latitude(event);
    longitude_ = geolocation_event_get_longitude(event);
    altitude_ = geolocation_event_get_altitude(event);
    horizAccuracy_ = geolocation_event_get_accuracy(event);
    vertAccuracy_ = geolocation_event_get_altitude_accuracy(event);
    heading_ = geolocation_event_get_heading(event);
    speed_ = geolocation_event_get_speed(event);
    vertSpeed_ = geolocation_event_get_vertical_speed(event);
    numSatellitesUsed_ = geolocation_event_get_num_satellites_used(event);
    numSatellitesTotal_ = geolocation_event_get_num_satellites_total(event);
    hdop_ = geolocation_event_get_hdop(event);
    vdop_ = geolocation_event_get_vdop(event);
    pdop_ = geolocation_event_get_pdop(event);
    geoidHeight_ = geolocation_event_get_geoid_height(event);
    qDebug() << "timestamp" << timestamp_;
    qDebug() << "coordinate" << latitude_ << longitude_ << altitude_;
    qDebug() << "heading and speed" << heading_ << speed_ << vertSpeed_;
    qDebug() << "accuracy" << horizAccuracy_ << vertAccuracy_;
    qDebug() << "num satellites" << numSatellitesUsed_ << "/" << numSatellitesTotal_;
    if (filter_) {
        filter_->update(timestamp_.toMSecsSinceEpoch(), latitude_, longitude_, horizAccuracy_, speed_);
        latitude_ = filter_->latitude();
        longitude_ = filter_->longitude();
        horizAccuracy_ = filter_->accuracy();
        qDebug() << "filter cooridnate " << latitude_ << longitude_;
        qDebug() << "filter accuracy" << horizAccuracy_;
    }
    emit dataChanged();
}

void GeoLocation::statusEvent(bps_event_t *event)
{
    Q_UNUSED(event);
}
