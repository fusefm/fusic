
/*
    fusic - radio playout software.
    Copyright (C) 2010 Matthew Leach

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

// Local includes:
#include "authsql.h"

// Qt Includes:
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSettings>

authSQL::authSQL()
{
}

void authSQL::doAuth()
{
  // Create a new database object and connect.
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setUserName(getUserName());
  db.setDatabaseName(getDatabaseName());
  db.setHostName(getHostName());
  db.setPassword(getPassword());

  if(!db.open())
  {
    // Could not connect. return an error.
    emit authError("Could not connect to the database.");
    return;
  }

  // Perform an SQL query for the user with an MD5 on the password.
  QSqlQuery query;
  query.prepare("SELECT * FROM ? WHERE userName = ? "
                  "AND password = MD5(?)");
  query.bindValue(0, getTableName());
  query.bindValue(1, mUserID);
  query.bindValue(2, mPassword);
  query.exec();

  // Close the databse.
  db.close();

  // Check the size of the results set.
  if(query.size() == 0)
    emit finishedAuth(false);
  else
    emit finishedAuth(true);
}

QString authSQL::getName()
{
    return "MySQL Authentication";
}

const QString authSQL::getAuthor()
{
  return "Matthew Leach";
}

const QString authSQL::getDescription()
{
  return "Authenticates a MySQL server with a username and MD5"
    " password field.";
}

const QDialog* authSQL::getsettingsDialog(QWidget* parent)
{
  return new QDialog;
}

QString authSQL::getUserName()
{
  QSettings set;
  return set.value("SQLAuth/UserName").toString();
}

QString authSQL::getDatabaseName()
{
  QSettings set;
  return set.value("SQLAuth/DatabaseName").toString();
}

QString authSQL::getHostName()
{
  QSettings set;
  return set.value("SQLAuth/HostName").toString();
}

QString authSQL::getTableName()
{
  QSettings set;
  return set.value("SQLAuth/Table").toString();
}

QString authSQL::getPassword()
{
  QSettings set;
  return set.value("SQLAuth/Password").toString();
}
