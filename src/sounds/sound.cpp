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
#include <QMutexLocker>

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
  doSetup(fileID);
}

// public getter implimentation.
QString sound::getTitle()
{
  // Attempt to lock and unlock the mutex to ensure that a threaded stup has finished.
  mutex.lock();
  mutex.unlock();
  return title;
}

QString sound::getLocation()
{
  // Attempt to lock and unlock the mutex to ensure that a threaded stup has finished.
  mutex.lock();
  mutex.unlock();
  return location;
}

double sound::getDuration()
{
  // Attempt to lock and unlock the mutex to ensure that a threaded stup has finished.
  mutex.lock();
  mutex.unlock();
  return duration;
}

bool sound::isValid()
{
  // Attempt to lock and unlock the mutex to ensure that a threaded stup has finished.
  mutex.lock();
  mutex.unlock();
  return valid;
}

// Protected member implimentation.

void sound::invalidate()
{
  valid = false;
}

// Private member implimentation.

sound::threadedSetup::threadedSetup(sound* s, int fileID)
{
  m_Sound = s;
  m_fileID = fileID;
}

void sound::threadedSetup::run()
{
    bool res = m_Sound->doSetup(m_fileID);
    
    // Emit the finished signal.
    emit m_Sound->setupComplete(res);
}

void sound::setFileID(int fileID)
{
  threadedSetup tSetup(this, fileID);
  tSetup.run();
}

bool sound::doSetup(int fileID)
{
  // Lock the mutex until setup is complete.
  QMutexLocker locker(&mutex);
  QSqlDatabase db = QSqlDatabase::database();
  
  // Ensure we have an open database
  if(!db.isOpen())
    if(!db.open())
      return false;
	
  // Do the query.  
  QSqlQuery query("SELECT * FROM sounds  WHERE sound_ID = ?", db);
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
  if(!extraSetup(db, fileID))
  {
    db.close();
    return false;
  }
  
  // Close the connection.
  db.close();
  
  // We are now a valid object.
  valid = true;
  return true;  
}
#include "sound.moc"