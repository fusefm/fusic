#include <QtGui/QApplication>
#include "fusic.h"
#include "wizards/initialwizard.h"
#include "wizards/initialwizard_introduction.h"
#include "misc/fusicsettings.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    if(!fusicSettings::misc::hasCompletedInitalSetup())
    {
	initialWizard* wiz = new initialWizard();
	wiz->show();
    }
    else
    {
      fusic* foo = new fusic;
      foo->show();
    }
    return app.exec();
}
