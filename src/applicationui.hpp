#ifndef ApplicationUI_HPP_
#define APPLICATIONUI_HPP_

#include <QObject>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

class ApplicationUI : public QObject
{
    Q_OBJECT

public:
    ApplicationUI();

private slots:
    void onSystemLanguageChanged();

private:
    QTranslator *translator;
    bb::cascades::LocaleHandler *localeHandler;
};

#endif
