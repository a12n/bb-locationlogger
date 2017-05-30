#include <cstdio>

#include "applicationui.hpp"
#include "geopositionsource.hpp"
#include "gpxfilewriter.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

static void stderrMsgHandler(QtMsgType, const char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
}

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);
    qInstallMsgHandler(stderrMsgHandler);

    app.setOrganizationName("a12n");
    app.setApplicationName("positionlogger");

    qmlRegisterType<GeoPositionSource>("a12n.geoPosition", 0, 1, "GeoPositionSource");
    qmlRegisterType<GpxFileWriter>("a12n.gpx", 0, 1, "GpxFileWriter");

    ApplicationUI appui;

    return app.exec();
}
