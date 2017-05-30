#ifndef GEOPOSITIONSOURCE_HPP_
#define GEOPOSITIONSOURCE_HPP_

#include <QObject>
#include <QDeclarativePropertyMap>

#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoPositionInfoSource>
#include <QtLocationSubset/QGeoSatelliteInfo>
#include <QtLocationSubset/QGeoSatelliteInfoSource>

using namespace QtMobilitySubset;

class GeoPositionSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(double period READ period WRITE setPeriod NOTIFY periodChanged)
    Q_PROPERTY(double accuracy) // TODO
    Q_PROPERTY(double responseTime) // TODO
    Q_PROPERTY(QString provider READ provider WRITE setProvider NOTIFY providerChanged)
    Q_PROPERTY(QString method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(bool stationaryDetectionEnabled READ isStationaryDetectionEnabled WRITE setStationaryDetectionEnabled NOTIFY stationaryDetectionEnabledChanged)
    Q_PROPERTY(bool locationServicesEnabled)    // TODO
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY positionChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY positionChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY positionChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY positionChanged)
public:
    explicit GeoPositionSource(QObject *parent = 0);

    bool isActive() const;
    double period() const;
    QString provider() const;
    QString method() const;
    bool isStationaryDetectionEnabled() const;

    QDateTime timestamp() const;
    double latitude() const;
    double longitude() const;
    double altitude() const;

signals:
    void activeChanged();
    void periodChanged();
    void providerChanged();
    void methodChanged();
    void stationaryDetectionEnabledChanged();
    void positionChanged();

public slots:
    void setActive(bool active);
    void setPeriod(double period);
    void setProvider(const QString& provider);
    void setMethod(const QString& method);
    void setStationaryDetectionEnabled(bool enabled);

private slots:
    void positionUpdated(const QGeoPositionInfo&);
    void updateTimeout();

private:
    QGeoPositionInfoSource *src;
    QString curMethod;
    bool active;
};

#endif
