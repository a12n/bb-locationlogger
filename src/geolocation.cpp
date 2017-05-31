#include <bps/bps.h>
#include <bps/geolocation.h>

#include "geolocation.hpp"

GeoLocation::GeoLocation(QObject *parent) :
    QObject(parent)
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

void GeoLocation::errorEvent(bps_event_t *event)
{
    Q_UNUSED(event);
}

void GeoLocation::infoEvent(bps_event_t *event)
{
    Q_UNUSED(event);
}

void GeoLocation::statusEvent(bps_event_t *event)
{
    Q_UNUSED(event);
}
