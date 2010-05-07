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

#ifndef SOUND_H
#define SOUND_H

// Qt Includes.
#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include <QMutex>

/**
 * @short A high level class that represents a sound in the fusic system.
 * 
 * A sound object represents a sound within the fusic system. Usually this class
 * is not directly used; one of it's subclasses should be used instead.
 *
 * <h3> Creating a sound object </h3>
 * 
 * There are two ways to create a sound object.
 * 
 * 1) Using the constructor:
 * @code
 * sound theSound(12);
 * @endcode
 * Here, we pass the fileID of the sound we wish the object to represent.
 * The system will then populate all the relevant info of the class from the database.
 * @note Creating a sound in this fashion will block for database lookups.
 *
 * 2) Using the setFileID() method:
 * @code
 * sound theSound;
 * theSound.setFileID(12);
 * @endcode
 * Using this method will not block and spawns a thread to retrieve the sound info.
 * The class will be locked via mutex whilst the database thread is alive. You can
 * cancel this via the cancelCreation() method, however. Once all data has been retrieved,
 * the setupComplete signal will be emitted.
 * @note Creating a sound in this fashion will NOT block for database lookups.
 * 
 * @author Matthew Leach <matt@fusefm.co.uk>
 *
 */
class sound : public QObject
{
  Q_OBJECT
  
  /**
   * Construct an empty sound object.
   */
  sound();

  /**
   * Construct a sound object for the given fileID.
   * @note Calling this method will block until database lookups have finished.
   * @param fileID The file ID for the object to represent.
   */
  sound(int fileID);

public:

  // Public getter functions.
  /**
   * Get the title of the sound.
   * @returns A QString set to the title of the sound.
   */
  QString getTitle();

  /**
   * Get the location of the sound.
   * @returns A QString set to the location of the sound.
   */
  QString getLocation();

  /**
   * Get the duration of a sound.
   * @returns A double set to the duration of the sound in seconds.
   */
  double getDuration();

  /**
   * Check for a valid sound object.
   * @returns A bool. If true the sound object is valid. If false,
   * the object is invalid.
   */
  bool isValid();
  
  /**
   * Set the fileID for the object.
   * @note Calling this method will block for database lookups.
   * @param fileID The file ID for the object to represent.
   */
  void setFileID(int fileID);

signals:

  /**
   * This signal is emmited once the object has set up after 
   * the setFileID function has been called.
   * @param success Weather the object was created successfully or not.
   */
  void setupComplete(bool success);
  
protected:
  
  /**
   * Invalidate the sound object - only subclasses can do this.
   * If the object is already invalid, this function has no effect.
   */
  void invalidate();

private:

  bool doSetup(int fileID);
  
  /**
   * This function should be overwritten for subclasses to setup their own properties
   * ontop of the sound object's.
   * @param db A database object will be passed that is already connected.
   * @return A bool indicating weather the setup was successful or not. If a false is returned,
   * it will invalidate this sound object.
   */
  virtual bool extraSetup(QSqlDatabase& db);
  
  // Private storage vars:
  QString location;
  QString title;
  QMutex mutex;
  double duration;
  bool valid;
  
  class threadedSetup : public QThread
  {
      threadedSetup(sound* s, int fileID);
      
    public:
      virtual void run();
      
    private:
      sound* m_Sound;
      int m_fileID;
      
  };
  
  // Allow the threadedSetup class to access private members.
  friend class threadedSetup;
};



#endif // SOUND_H
