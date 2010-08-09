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

#ifndef DATABASECHOOSER_H
#define DATABASECHOOSER_H

#include <QDialog>
#include <QListWidget>


/**
 * @short A simple database chooser dialog.
 * This dialog will show a list of MySQL databases for a given set of username
 * connection parameters.
 *
 * <h3>Using the databaseChooser</h3>
 * The database chooser can be envoked using the following code.
 * @code
 * DatabaseChooser* dbDlg = new DatabaseChooser(this, 0,
 *                                              "username",
 *                                              "password",
 *                                              "someHost",
 *                                               8000);
 * dlgDb->exec();
 * @endcode
 *
 * To inspect the choosen database, simply access the selectedDatabase member.
 * @code
 * QString databaseSelection = dlgDb->selectedDatabase;
 * doSomethingWithDatabase(databaseSelection);
 * @endcode
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
class DatabaseChooser : public QDialog
{
  Q_OBJECT
  public:

    /**
     * Constructs a new dialog an attempts a connection to the database.
     * if the connection is unsucsessful then a error dialog is shown
     * and the listDatabases widget and the Ok button are disabled.
     *
     * @param parent The Parent QObject.
     * @param f Window Flags.
     * @param username The username for the database.
     * @param password The password for the database.
     * @param hostname The hostname for the password, can be IP format or a hostname.
     * @param port The port number for the database, if left at zero, default is used.
     */
    DatabaseChooser(QWidget* parent = 0, 
                    Qt::WindowFlags f = 0,
                    QString username = "",
                    QString password = "",
                    QString hostname = "",
                    int port = 0);

    /**
     * Sets the selectedDatabase member once the Ok button has been pressed.
     */
    virtual void accept();

    /**
     * The database that the user selected.
     */
    QString selectedDatabase;
  private:
    QListWidget* listDatabases;
    bool isValid;

};

#endif // DATABASECHOOSER_H