/*
    fusic - radio playout software.
    Copyright (C) 2010  Matthew Leach

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or	 FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "initialwizard.h"
#include "initialwizard_introduction.h"
#include "initialwizard_mysqlsetup.h"
#include "initialwizard_authsetup.h"
#include "fusicsettings.h"
#include "cartwall.h"
#include <QSettings>

InitialWizard::InitialWizard(QWidget* parent, Qt::WindowFlags flags): QWizard(parent, flags)
{
  addPage(new InitialWizardIntroduction);
  addPage(new InitialWizardMySQLSetup);
  addPage(new InitialWizardAuthSetup);
}

void InitialWizard::accept()
{ 
  // Save the settings.
  fusicSettings::DBSettings::setDatabase(field("db_database").toString());
  fusicSettings::DBSettings::setHost(field("db_host").toString());
  fusicSettings::DBSettings::setPassword(field("db_password").toString());
  fusicSettings::DBSettings::setPort(field("db_port").toInt());
  fusicSettings::DBSettings::setUserName(field("db_username").toString());
  fusicSettings::misc::setHasCompletedInitalSetup();

  // Show a new cart wall.
  CartWall* theWall = new CartWall;
  theWall->show();

  // Show the carts dialog.
    QDialog::accept();
}
