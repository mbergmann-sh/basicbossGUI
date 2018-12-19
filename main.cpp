#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("BasicBoss");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("MB-SoftWorX");
    app.setOrganizationDomain("http://www.mbergmann-sh.de");

    /* Lokalisierung:             */
    /* Übersetzungsdatei für Qt: */

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    /* load our custom translations for this application */
    QTranslator myappTranslator;
    myappTranslator.load("myapp_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    // fire up GUI
    MainWindow w;
    w.show();

    return app.exec();
}
