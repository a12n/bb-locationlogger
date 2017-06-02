#ifndef KALMANFILTER_HPP_
#define KALMANFILTER_HPP_

#include <cmath>

#include <QtGlobal>

// Based on https://stackoverflow.com/a/15657798
class KalmanFilter
{
public:
    KalmanFilter(double q = 3.0) :
        q_(q),
        timestamp_(),
        latitude_(NAN),
        longitude_(NAN),
        variance_(NAN)
    {
    }

    double q() const { return q_; }
    qint64 timestamp() const { return timestamp_; }
    double latitude() const { return latitude_; }
    double longitude() const { return longitude_; }
    double accuracy() const { return std::sqrt(variance_); }

    void update(qint64 t, double lat, double lon, double acc, double speed)
    {
        if (acc < 1.0) acc = 1.0;
        if (isnan(variance_)) {
            timestamp_ = t;
            latitude_ = lat;
            longitude_ = lon;
            variance_ = acc * acc;
            return;
        }

        const qint64 tdiff = t - timestamp_;
        if (tdiff > 0) {
            variance_ += tdiff * q_ * q_ / 1000.0;
//            variance_ += tdiff * speed * speed / 1000.0;
            timestamp_ = t;
        }

        const double k = variance_ / (variance_ + acc * acc);
        latitude_ += k * (lat - latitude_);
        longitude_ += k * (lon - longitude_);
        variance_ = (1.0 - k) * variance_;
    }

private:
    const double q_;            // Speed factor (m/s)
    qint64 timestamp_;          // Milliseconds since Epoch
    double latitude_;
    double longitude_;
    double variance_;
};

#endif
