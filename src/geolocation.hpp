#ifndef GEOLOCATION_HPP_
#define GEOLOCATION_HPP_

#include <bb/AbstractBpsEventHandler>

#include <QObject>

class GeoLocation : public QObject, public bb::AbstractBpsEventHandler
{
    Q_OBJECT
    // Data properties
    Q_PROPERTY(double latitude READ latitude NOTIFY dataChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY dataChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY dataChanged)
    Q_PROPERTY(double horizAccuracy READ horizAccuracy NOTIFY dataChanged)
    Q_PROPERTY(double vertAccuracy READ vertAccuracy NOTIFY dataChanged)

public:
    explicit GeoLocation(QObject *parent = 0);
    virtual ~GeoLocation();

    virtual void event(bps_event_t *event);

    Q_INVOKABLE void startUpdates();
    Q_INVOKABLE void stopUpdates();

    // Data properties
    double latitude() const
    {
        return latitude_;
    }

    double longitude() const
    {
        return longitude_;
    }

    double altitude() const
    {
        return altitude_;
    }

    double horizAccuracy() const
    {
        return horizAccuracy_;
    }

    double vertAccuracy() const
    {
        return vertAccuracy_;
    }

signals:
    void dataChanged();

public slots:
    // Configuration properties
    Q_INVOKABLE void setAccuracy(unsigned int accuracy);
    Q_INVOKABLE void setPeriod(unsigned int period);
    Q_INVOKABLE void setResponseTime(unsigned int responseTime);
    Q_INVOKABLE void setStationaryDetection(bool enabled);

private:
    void errorEvent(bps_event_t *event);
    void infoEvent(bps_event_t *event);
    void statusEvent(bps_event_t *event);

    double latitude_;
    double longitude_;
    double altitude_;
    double horizAccuracy_;
    double vertAccuracy_;
};

#endif
