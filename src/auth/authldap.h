/*
    fusic - radio playout software.
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

#ifndef AUTHLDAP_H
#define AUTHLDAP_H

#include <authbase.h>

class authLDAP : public authBase
{

public:
    virtual void doAuth();
    virtual const QDialog* getsettingsDialog(QWidget* parent);
    virtual QString getName();
    virtual const QString getAuthor();
    virtual const QString getDescription();
private:
  const QString getLDAPErrorString(int errnumber);
  bool getLdapSettings();
  
  // LDAP Setting member variables.
  QString mLDAPURI;
  QString mBindAttribute;
  QString mBaseDN;
  struct timeval mTimeout;
  QStringList mAccessGroups;
};

#endif // AUTHLDAP_H
