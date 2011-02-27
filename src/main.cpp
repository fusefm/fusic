#include <QtGui/QApplication>
#include "wizards/initialwizard.h"
#include "misc/fusicsettings.h"
#include "ui/cartwall.h"
#include <QSettings>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  // Set app and company names for settings.
  QCoreApplication::setOrganizationName("FuseFM");
  QCoreApplication::setOrganizationDomain("fusefm.co.uk");
  QCoreApplication::setApplicationName("Fusic");
  
  if (!fusicSettings::misc::hasCompletedInitalSetup())
  {
    // Show the wizard.
    InitialWizard* wiz = new InitialWizard();
    wiz->show();
  }
  else
  {
    // Show the carts dialog.
    CartWall* theWall = new CartWall;
    theWall->show();
  }
  return app.exec();
}
