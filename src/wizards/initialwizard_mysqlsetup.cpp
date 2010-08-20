/*
  fusic - radio playout software.
  Copyright (C) 2010  Matthew Leach

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "initialwizard_mysqlsetup.h"
#include "databasechooser.h"
#include "fusicsettings.h"

// Qt Includes:
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>

initialwizard_mysqlSetup::initialwizard_mysqlSetup(QWidget* parent): QWizardPage(parent)
{
  ui.setupUi(this);

  // Set password echo mode.
  ui.passwordEdit->setEchoMode(QLineEdit::Password);

  // Register fields.
  registerField("db_username*", ui.usernameEdit);
  registerField("db_password", ui.passwordEdit);
  registerField("db_host*", ui.hostnameEdit);
  registerField("db_port", ui.portEdit);
  registerField("db_database*", ui.databaseEdit);

  connect(ui.databaseBrowseButton, SIGNAL(clicked(bool)),
          this, SLOT(showDBBrowser(bool)));

}

void initialwizard_mysqlSetup::showDBBrowser(bool checked)
{
  if(ui.hostnameEdit->text().isEmpty() || ui.usernameEdit->text().isEmpty())
  {
    QMessageBox::critical(this, "Error", "Please enter at least a username and a hostname for the MySQL server.");
  }
  else
  {
    DatabaseChooser* chooserDlg = new DatabaseChooser(this,0,
                                                      ui.usernameEdit->text(),
                                                      ui.passwordEdit->text(),
                                                      ui.hostnameEdit->text(),
                                                      ui.portEdit->text().toInt());
    chooserDlg->exec();
    ui.databaseEdit->setText(chooserDlg->selectedDatabase);
  }
}

void initialwizard_mysqlSetup::initializePage()
{
  // Set default values.
  ui.usernameEdit->setText(fusicSettings::DBSettings::getUserName());
  ui.passwordEdit->setText(fusicSettings::DBSettings::getPassword());
  ui.hostnameEdit->setText(fusicSettings::DBSettings::getHost());
  ui.portEdit->setText(QString(fusicSettings::DBSettings::getPort()));
}


#include "initialwizard_mysqlsetup.moc"