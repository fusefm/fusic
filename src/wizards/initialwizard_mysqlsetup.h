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

#ifndef INTROWIZARD_MYSQLSETUP_H
#define INTROWIZARD_MYSQLSETUP_H

// Local Includes:
#include "ui_initialwizard_mysqlsetup.h"

// Qt Includes:
#include <QWizard>
#include <QLineEdit>

/**
 * @short MySQL configuration wizard page.
 * This wiard page provides configuration options for a MySQL database connection.
 * Fields that are exported on this page:
 * @param db_username The username.
 * @param db_password The user's password.
 * @param db_host The hostname to connect to, can be IP or hostname.
 * @param db_port The connection port.
 * @param db_database The fusic database name.
 *
 * @see initialWizard
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
class InitialWizardMySQLSetup : public QWizardPage
{
  Q_OBJECT
  public:
    InitialWizardMySQLSetup(QWidget* parent = 0);
    virtual void initializePage();

  private slots:
    void showDBBrowser(bool checked);

  private:
    Ui::MySQLWizardPage ui;
};

#endif // INTROWIZARD_MYSQLSETUP_H
