#ifndef GEOLOCATION_HPP_
#define GEOLOCATION_HPP_

#include <bb/AbstractBpsEventHandler>

#include <QDateTime>
#include <QObject>

#include "kalmanfilter.hpp"

struct GeoLocationData
{
    GeoLocationData();

    double latitude;
    double longitude;
    double altitude;
    double horizAccuracy;
    double vertAccuracy;
    double heading;
    double speed;
    double vertSpeed;
//    unsigned int ttff;
    double hdop;
    double vdop;
    double pdop;
    double geoidHeight;
    QDateTime timestamp;
    unsigned int numSatellitesUsed;
    unsigned int numSatellitesTotal;
};

class GeoLocation : public QObject, public bb::AbstractBpsEventHandler
{
    Q_OBJECT
    // Data properties
    Q_PROPERTY(double latitude READ latitude NOTIFY dataChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY dataChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY dataChanged)
    Q_PROPERTY(double horizAccuracy READ horizAccuracy NOTIFY dataChanged)
    Q_PROPERTY(double vertAccuracy READ vertAccuracy NOTIFY dataChanged)
    Q_PROPERTY(double heading READ heading NOTIFY dataChanged)
    Q_PROPERTY(double speed READ speed NOTIFY dataChanged)
    Q_PROPERTY(double vertSpeed READ vertSpeed NOTIFY dataChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY dataChanged)
    Q_PROPERTY(unsigned int numSatellitesUsed READ numSatellitesUsed NOTIFY dataChanged)
    Q_PROPERTY(unsigned int numSatellitesTotal READ numSatellitesTotal NOTIFY dataChanged)
    Q_PROPERTY(double hdop READ hdop NOTIFY dataChanged)
    Q_PROPERTY(double vdop READ hdop NOTIFY dataChanged)
    Q_PROPERTY(double pdop READ hdop NOTIFY dataChanged)
    Q_PROPERTY(double geoidHeight READ geoidHeight NOTIFY dataChanged)

public:
    explicit GeoLocation(QObject *parent = 0);
    virtual ~GeoLocation();

    virtual void event(bps_event_t *event);

    Q_INVOKABLE void startUpdates();
    Q_INVOKABLE void stopUpdates();

    // Data properties
    double latitude() const { return data_.latitude; }
    double longitude() const { return data_.longitude; }
    double altitude() const { return data_.altitude_; }
    double horizAccuracy() const { return data_.horizAccuracy_; }
    double vertAccuracy() const { return data_.vertAccuracy_; }
    double heading() const { return data_.heading_; }
    double speed() const { return data_.speed_; }
    double vertSpeed() const { return data_.vertSpeed_; }
    QDateTime timestamp() const { return data_.timestamp_; }
    unsigned int numSatellitesUsed() const { return data_.numSatellitesUsed_; }
    unsigned int numSatellitesTotal() const { return data_.numSatellitesTotal_; }
    double hdop() const { return data_.hdop_; }
    double vdop() const { return data_.vdop_; }
    double pdop() const { return data_.pdop_; }
    double geoidHeight() const { return data_.geoidHeight_; }

signals:
    void dataChanged();
    void error(const QString& err);
    void warning(const QString& warn);

public slots:
    // Configuration properties
    Q_INVOKABLE void setAccuracy(unsigned int accuracy);
    Q_INVOKABLE void setPeriod(unsigned int period);
    Q_INVOKABLE void setResponseTime(unsigned int responseTime);
    Q_INVOKABLE void setStationaryDetection(bool enabled);
    Q_INVOKABLE void setFilterData(bool enabled);

private:
    void errorEvent(bps_event_t *event);
    void infoEvent(bps_event_t *event);
    void statusEvent(bps_event_t *event);

    GeoLocationData data_;
    KalmanFilter *filter_;
};

#endif
