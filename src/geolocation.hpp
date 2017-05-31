#ifndef GEOLOCATION_HPP_
#define GEOLOCATION_HPP_

#include <bb/AbstractBpsEventHandler>

#include <QObject>

class GeoLocation : public QObject, public bb::AbstractBpsEventHandler
{
    Q_OBJECT
public:
    explicit GeoLocation(QObject *parent = 0);
    virtual ~GeoLocation();

    virtual void event(bps_event_t *event);

    Q_INVOKABLE void startUpdates();
    Q_INVOKABLE void stopUpdates();

public slots:
    // Configuration properties
    Q_INVOKABLE void setAccuracy(unsigned int accuracy);
    Q_INVOKABLE void setPeriod(unsigned int period);
    Q_INVOKABLE void setResponseTime(unsigned int responseTime);
    Q_INVOKABLE void setStationaryDetection(bool enabled);
};

#endif
