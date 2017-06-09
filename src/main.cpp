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
#ifndef QT_NO_DEBUG
    qInstallMsgHandler(stderrMsgHandler);
#endif // QT_NO_DEBUG

    app.setOrganizationName("a12n");
    app.setOrganizationDomain("a12n.bitbucket.org");
    app.setApplicationName("locationlogger");

    ApplicationUI appui;

    return app.exec();
}
