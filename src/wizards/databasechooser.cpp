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

#include "databasechooser.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QString>
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

DatabaseChooser::DatabaseChooser(QWidget* parent, 
                                  Qt::WindowFlags f,
                                  QString username,
                                  QString password,
                                  QString hostname,
                                  int port) :
                                  QDialog(parent, f)
{
  // Add a label for the top of the dialog.
  QLabel* lblInstructions = new QLabel(
    "Please select a database from the selection below.", this);
  lblInstructions->setWordWrap(true);

  // Ok and Cancel buttons.
  QPushButton* btnOk = new QPushButton("Ok");
  QPushButton* btnCancel = new QPushButton("Cancel");
  connect(btnOk, SIGNAL(released()), this, SLOT(accept()));
  connect(btnCancel, SIGNAL(released()), this, SLOT(reject()));

  // Construct a new ListWidget and set to single selection only.
  listDatabases = new QListWidget;
  listDatabases->setSelectionMode(QAbstractItemView::SingleSelection);

  // Attempt databse connection.
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setUserName(username);
  db.setPassword(password);
  db.setHostName(hostname);
  if(port != 0)
    db.setPort(port);
  bool ok = db.open();
  if(!ok)
  {
    // Database connection failed.
    QString errorMessage = "Could not connect to MySQL: ";
    errorMessage.append(db.lastError().text());
    QMessageBox::critical(this, "MySQL Error", errorMessage);
    new QListWidgetItem("No Databases Found.", listDatabases);
    listDatabases->setEnabled(false);
    btnOk->setEnabled(false);
  }
  else
  {
    // Create a query for the databases.
    QSqlQuery databaseQuery("SHOW DATABASES;");
    int databaseField = databaseQuery.record().indexOf("Database");
    while(databaseQuery.next())
    {
      QString databaseName = databaseQuery.value(databaseField).toString();
      new QListWidgetItem(QIcon(":/icon/db.gif") ,
                          databaseName, listDatabases);
    }
  }

  // Add the layout for the buttons at the bottom.
  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(btnOk);
  buttonLayout->addWidget(btnCancel);

  // Set the main layout.
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(lblInstructions);
  layout->addWidget(listDatabases);
  layout->addLayout(buttonLayout);
  setLayout(layout);

  // Set the correct title.
  setWindowTitle("Select Database");
}

void DatabaseChooser::accept()
{
  // Set the database choice.
  if(!listDatabases->selectedItems().isEmpty())
    selectedDatabase = listDatabases->selectedItems().first()->text();
  else
    selectedDatabase = "";

  QDialog::accept();
}

#include "databasechooser.moc"