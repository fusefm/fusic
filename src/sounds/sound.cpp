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

#include "sound.h"

// For documentation, see sound.h

// Constructor implimentation.
sound::sound() : 
  title(QString()),
  location(QString()),
  duration(0),
  valid(false)
{
  
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



