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
  setTitle("MySQL Setup");
  setSubTitle("Set basic MySQL settings an attempt a connection.");
  
  QVBoxLayout* mainLayout = new QVBoxLayout;
  
  // Server settings.
  QGroupBox *serverBox = new QGroupBox("Server Settings");
  
  editUsername = new QLineEdit;
  editPassword = new QLineEdit;
  editPassword->setEchoMode(QLineEdit::Password);
  editHost = new QLineEdit;
  editPort = new QLineEdit;
  
  QFormLayout* serverFormLayout = new QFormLayout;
  serverFormLayout->addRow("&Username:", editUsername);
  serverFormLayout->addRow("&Password:", editPassword);
  serverFormLayout->addRow("&Hostname:", editHost);
  serverFormLayout->addRow("&Port:", editPort);
  
  registerField("db_username*", editUsername);
  registerField("db_password", editPassword);
  registerField("db_host*", editHost);
  registerField("db_port", editPort);
  
  serverBox->setLayout(serverFormLayout);
  
  // Table Settings.
  QGroupBox * tableBox = new QGroupBox("Table Settings");
  
  editDatabase = new QLineEdit;
  registerField("db_database*", editDatabase);
  QPushButton* dbBrowse = new QPushButton("...");
  QHBoxLayout* browseLayout = new QHBoxLayout;
  browseLayout->addWidget(editDatabase);
  browseLayout->addWidget(dbBrowse);
  connect(dbBrowse, SIGNAL(clicked(bool)), this, SLOT(showDBBrowser(bool)));
  
  QFormLayout* tableFormLayout = new QFormLayout;
  tableFormLayout->addRow("Database:", browseLayout);
  
  tableBox->setLayout(tableFormLayout);
  
  mainLayout->addWidget(serverBox);
  mainLayout->addWidget(tableBox);
  setLayout(mainLayout);
}

void initialwizard_mysqlSetup::showDBBrowser(bool checked)
{
  if(editHost->text().isEmpty() || editUsername->text().isEmpty())
  {
    QMessageBox::critical(this, "Error", "Please enter at least a username and a hostname for the MySQL server.");
  }
  else
  {
    DatabaseChooser* chooserDlg = new DatabaseChooser(this,0,
                                                      editUsername->text(),
                                                      editPassword->text(),
                                                      editHost->text(),
                                                      editPort->text().toInt());
    chooserDlg->exec();
    editDatabase->setText(chooserDlg->selectedDatabase);
  }
}



#include "initialwizard_mysqlsetup.moc"