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
#include "song.h"

// Qt Includes:
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

// Public getter implimentation.

QString song::getAlbum()
{
    mutex.lock();
    mutex.unlock();
    return album;
}

QString song::getArtist()
{
    mutex.lock();
    mutex.unlock();
    return artist;
}

double song::getIntro()
{
    mutex.lock();
    mutex.unlock();
    return intro;
}

QString song::getGenre()
{
    mutex.lock();
    mutex.unlock();
    return genre;
}

// Do extra setup for extended properties.
bool song::extraSetup(QSqlDatabase &db, int soundID)
{
    // Ensure we have an active database connection.
    if(!db.isOpen())
        return false;

    QSqlQuery query("SELECT * FROM songs WHERE Sound_ID = ?");
    query.bindValue(0, soundID);
    query.exec();

    // Ensure we have a result.
    if(!query.first())
        return false;

    // Get field IDs.
    int fIDGenre = query.record().indexOf("genre");
    int fIDArtist = query.record().indexOf("artist");
    int fIDAlbum = query.record().indexOf("album");
    int fIDIntro = query.record().indexOf("intro");

    // Get the data.
    genre = query.value(fIDGenre).toString();
    artist = query.value(fIDArtist).toString();
    album = query.value(fIDAlbum).toString();
    intro = query.value(fIDIntro).toDouble();

    // All is good.
    return true;
}
