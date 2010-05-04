/*
    fusic - Radio playout software
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

// Local Includes:
#include "sound.h"
#include "fusicsettings.h"

// Qt Includes
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

// For documentation, see sound.h

// Constructor implimentation.
sound::sound() : 
  title(QString()),
  location(QString()),
  duration(0),
  valid(false)
{
  
}

sound::sound(int fileID)
{
  if(!doSetup(fileID))
  {
    invalidate();
  }
  else
  {
    valid = true;
  }
}

// public getter implimentation.
QString sound::getTitle()
{
  return title;
}

QString sound::getLocation()
{
  return location;
}

double sound::getDuration()
{
  return duration;
}

bool sound::isValid()
{
  return valid;
}

// Protected member implimentation.

void sound::invalidate()
{
  valid = false;
}

// Private member implimentation.

bool sound::extraSetup(QSqlDatabase& db)
{
  return true;
}

bool sound::doSetup(int fileID)
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  
  // Set database settings.
  db.setDatabaseName(fusicSettings::DBSettings::getDatabase());
  db.setHostName(fusicSettings::DBSettings::getHost());
  db.setUserName(fusicSettings::DBSettings::getUserName());
  db.setPassword(fusicSettings::DBSettings::getPassword());
  db.setPort(fusicSettings::DBSettings::getPort());
  
  // Attempt connection.
  if(!db.open())
    return false;
  
  // Do the query.  
  QSqlQuery query("SELECT * FROM sounds  WHERE sound_ID = ?");
  query.bindValue(0, fileID);
  query.exec();
  
  // Check results size.
  if(query.size() < 1)
  {
    db.close();
    return false;
  }
  
  // Set variables.
  int fIDLocation = query.record().indexOf("location");
  int fIDTitle = query.record().indexOf("title");
  int fIDDuration = query.record().indexOf("duration");
  
  // Get the first record.
  query.next();
  
  // Get the info.
  location = query.value(fIDLocation).toString();
  title = query.value(fIDTitle).toString();
  duration = query.value(fIDDuration).toDouble();
  
  // Do extra setup.
  if(!extraSetup(db))
  {
    db.close();
    return false;
  }
  
  // Close the connection.
  db.close();
  return true;  
}


