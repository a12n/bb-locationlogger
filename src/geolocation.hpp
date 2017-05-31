#ifndef GEOLOCATION_HPP_
#define GEOLOCATION_HPP_

#include <QObject>
#include <bb/AbstractBpsEventHandler>

class GeoLocation : public QObject, public bb::AbstractBpsEventHandler
{
    Q_OBJECT
public:
    explicit GeoLocation(QObject *parent = 0);
    virtual ~GeoLocation();

    virtual void event(bps_event_t *event);

    Q_INVOKABLE void startUpdates();
    Q_INVOKABLE void stopUpdates();
};

#endif
