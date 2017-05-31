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
            // TODO
            break;
        case GEOLOCATION_ERROR :
            // TODO
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
