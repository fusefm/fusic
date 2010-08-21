#include <QtGui/QApplication>
#include "fusic.h"
#include "wizards/initialwizard.h"
#include "wizards/initialwizard_introduction.h"
#include "misc/fusicsettings.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    if (!fusicSettings::misc::hasCompletedInitalSetup())
    {
        InitialWizard* wiz = new InitialWizard();
        wiz->show();
    }
    else
    {
        Fusic* foo = new Fusic;
        foo->show();
    }
    return app.exec();
}
