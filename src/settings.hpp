#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE void setValue(const QString& key, const QVariant& value);
    Q_INVOKABLE QVariant value(const QString& key, const QVariant& defaultValue) const;

private:
    QSettings *s;
};

#endif
