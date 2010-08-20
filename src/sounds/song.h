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

#ifndef SONG_H
#define SONG_H

// Local Includes:
#include "sound.h"

/**
 * @short A class to represent a song in fusic.
 * This class extends the sound object to include properties that are
 * specific to songs.
 */
class song : public sound
{
  // Public getter functions.
public:
  /**
   * Get the artist of the song.
   * @returns A QString set to the artist of the song.
   */
  QString getArtist();
  
  /**
   * Get the album of the song.
   * @returns A QString set to the album of the song.
   */
  QString getAlbum();
  
  /**
   * Get the genre of the song.
   * @returns A QString set to the genre of the song.
   */
  QString getGenre();
  
  /**
   * Get the intro time of the song.
   * @returns A double set to the intro time of the song in seconds.
   */
  double getIntro();
  
private:    
  bool extraSetup(QSqlDatabase& db, int soundID);
  QString artist;
  QString album;
  QString genre;
  double intro;
};

#endif // SONG_H
