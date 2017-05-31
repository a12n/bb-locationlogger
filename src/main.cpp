#include <cstdio>

#include "applicationui.hpp"

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

    ApplicationUI appui;

    return app.exec();
}
