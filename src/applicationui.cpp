#include "applicationui.hpp"
#include "geolocation.hpp"
#include "gpxfile.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/cascades/AbstractCover>

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

    GeoLocation *geoLocation = new GeoLocation(this);
    GpxFile *gpxFile = new GpxFile(this);

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_geoLocation", geoLocation);
    qml->setContextProperty("_gpxFile", gpxFile);

    // Create root object for the UI set the application scene
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // Create application cover
    QmlDocument *qmlCover = QmlDocument::create("asset:///Cover.qml").parent(this);

    qmlCover->setContextProperty("_geoLocation", geoLocation);

    // Create the QML Container from using the QMLDocument.
    Container *coverContainer = qmlCover->createRootObject<Container>();

    // Create a SceneCover and set the application cover
    SceneCover *sceneCover = SceneCover::create().content(coverContainer);
    Application::instance()->setCover(sceneCover);
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
