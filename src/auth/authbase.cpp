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

#include "authbase.h"

authBase::authBase(QObject* parent): QThread(parent)
{
}

void authBase::run()
{
  doAuth();
}

void authBase::setCredentials(const QString& userID, const QString& password)
{
    mPassword = password;
    mUserID = userID;
}

void authBase::doAuth()
{
  emit authError("authBase should not be used for authentication.");
}

const QString authBase::getAuthor()
{
  return mAuthor;
}

const QString authBase::getDescription()
{
  return mDescription;
}

const QDialog* authBase::getsettingsDialog(QWidget* parent)
{
  return new QDialog(parent);
}

void authBase::setAuthor(QString author)
{
  mAuthor = author;
}

void authBase::setDescription(QString description)
{
  mDescription = description;
}

