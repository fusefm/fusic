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

#ifndef AUTHLDAPDIALOG_H
#define AUTHLDAPDIALOG_H

// Local includes:
#include "ui_authldapdialog.h"

// Qt Includes:
#include <QDialog>


class authLDAPDialog : public QDialog
{
  public:
    authLDAPDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

  private:
    Ui::authLDAPDialogUI ui;
};

#endif // AUTHLDAPDIALOG_H