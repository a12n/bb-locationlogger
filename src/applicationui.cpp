#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
    QObject(),
    m_translator(new QTranslator(this)),
    m_localeHandler(new LocaleHandler(this))
{
    bool ok = QObject::connect(m_localeHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
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

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_translator);
    // Initiate, load and install the application translation files.
    QString localeString = QLocale().name();
    QString fileName = QString("positionlogger_%1").arg(localeString);
    if (m_translator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_translator);
    }
}