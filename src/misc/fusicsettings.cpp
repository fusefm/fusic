/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "fusicsettings.h"

// Qt includes:
#include <QSettings>

// Getter Functions.
QString fusicSettings::DBSettings::getDatabase()
{
  QSettings set;
  return set.value("DBSettings/Database").toString();
}

QString fusicSettings::DBSettings::getHost()
{
  QSettings set;
  return set.value("DBSettings/Host").toString();
}

QString fusicSettings::DBSettings::getPassword()
{
  QSettings set;
  return set.value("DBSettings/Password").toString();
}

int fusicSettings::DBSettings::getPort()
{
  QSettings set;
  return set.value("DBSettings/Port").toInt();
}

QString fusicSettings::DBSettings::getUserName()
{
  QSettings set;
  return set.value("DBSettings/UserName").toString();
}

// Setter functions.

void fusicSettings::DBSettings::setDatabase(QString database)
{
  QSettings set;
  set.setValue("DBSettings/Database", database);
}

void fusicSettings::DBSettings::setHost(QString host)
{
  QSettings set;
  set.setValue("DBSettings/Host", host);
}

void fusicSettings::DBSettings::setPassword(QString password)
{
  QSettings set;
  set.setValue("DBSettings/Password", password);
}

void fusicSettings::DBSettings::setPort(int port)
{
  QSettings set;
  set.setValue("DBSettings/Port", port);
}

void fusicSettings::DBSettings::setUserName(QString username)
{
  QSettings set;
  set.setValue("DBSettings/UserName", username);
}