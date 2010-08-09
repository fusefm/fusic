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

#ifndef AUTHSQL_H
#define AUTHSQL_H

#include "authbase.h"

class authSQL : public authBase
{
public:
  authSQL();
  virtual void doAuth();
  virtual QString getName();
  virtual const QString getAuthor();
  virtual const QString getDescription();
  virtual const QDialog* getsettingsDialog(QWidget* parent);
private:
  QString getDatabaseName();
  QString getUserName();
  QString getPassword();
  QString getHostName();
  QString getTableName();

};

#endif // AUTHSQL_H
