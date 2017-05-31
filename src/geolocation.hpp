#ifndef GEOLOCATION_HPP_
#define GEOLOCATION_HPP_

#include <bb/AbstractBpsEventHandler>

#include <QDateTime>
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
    double latitude() const { return latitude_; }
    double longitude() const { return longitude_; }
    double altitude() const { return altitude_; }
    double horizAccuracy() const { return horizAccuracy_; }
    double vertAccuracy() const { return vertAccuracy_; }
    QDateTime timestamp() const { return timestamp_; }
    unsigned int numSatellitesUsed() const { return numSatellitesUsed_; }
    unsigned int numSatellitesTotal() const { return numSatellitesTotal_; }
    double hdop() const { return hdop_; }
    double vdop() const { return vdop_; }
    double pdop() const { return pdop_; }
    double geoidHeight() const { return geoidHeight_; }

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

private:
    void errorEvent(bps_event_t *event);
    void infoEvent(bps_event_t *event);
    void statusEvent(bps_event_t *event);

    double latitude_;
    double longitude_;
    double altitude_;
    double horizAccuracy_;
    double vertAccuracy_;
//    double heading_;
//    double speed_;
//    double vertSpeed_;
//    unsigned int ttff_;
    double hdop_;
    double vdop_;
    double pdop_;
    double geoidHeight_;
    QDateTime timestamp_;
    unsigned int numSatellitesUsed_;
    unsigned int numSatellitesTotal_;
};

#endif
