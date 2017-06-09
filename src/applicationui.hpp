#ifndef APPLICATIONUI_HPP_
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

    Q_INVOKABLE QString formatInteger(int n) const;
    Q_INVOKABLE QString formatReal(double x, int prec) const;

private slots:
    void onSystemLanguageChanged();

private:
    QTranslator *translator;
    bb::cascades::LocaleHandler *localeHandler;
};

#endif
