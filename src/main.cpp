#include "applicationui.hpp"
#include "geopositionsource.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    qmlRegisterType<GeoPositionSource>("a12n.geoPosition", 0, 1, "GeoPositionSource");

    ApplicationUI appui;

    return app.exec();
}
