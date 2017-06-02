#include "applicationui.hpp"
#include "geolocation.hpp"
#include "gpxfile.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
    QObject(),
    translator(new QTranslator(this)),
    localeHandler(new LocaleHandler(this))
{
    bool ok = QObject::connect(localeHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(ok);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(ok);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_geoLocation", new GeoLocation(this));
    qml->setContextProperty("_gpxFile", new GpxFile(this));

    // Create root object for the UI set the application scene
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(translator);
    // Initiate, load and install the application translation files.
    QString localeString = QLocale().name();
    QString fileName = QString("gnsslogger_%1").arg(localeString);
    if (translator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(translator);
    }
}
