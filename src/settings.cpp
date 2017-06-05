#include "settings.hpp"

Settings::Settings(QObject *parent) :
    QObject(parent),
    s(new QSettings(this))
{
}

void Settings::setValue(const QString& key, const QVariant& value)
{
    s->setValue(key, value);
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue) const
{
    return s->value(key, defaultValue);
}
