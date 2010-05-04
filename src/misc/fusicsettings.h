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

#ifndef FUSICSETTINGS_H
#define FUSICSETTINGS_H

// Qt Includes:
#include <QString>

/**
 * @short A namespace containing classes related to differnt fusic settings.
 * 
 * This namespace contains classes that are used to get a set differnt settings 
 * within the fusic system. All classes should contain static functions that just
 * get and set the settings via the QSettings class.
 *
 * <h3>Using the settings classes</h3>
 *
 * Using these classes coulnd't be easier, simply just call the functions.
 * For example, to get the database username for connection, use,
 * @code 
 * QString username = fusicSettings::DBSettings::getUserName();
 * @endcode
 * 
 * if you would instead like to set a value, you can simply use the set functions,
 * for example,
 * @code
 * QString userName = "someUser";
 * fusicSettings::DBSettings::setUserName(userName);
 * @endcode
 * 
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
namespace fusicSettings
{

/**
 * A class for settings realting to database connection.
 * @see fusicSettings
 */
class DBSettings
{
public:

  // Get functions.
  /**
   * Get the database hostname.
   * @returns A QString set to the database's hostname.
   */
  static QString getHost();

  /**
   * Get the database username.
   * @returns A QString set to the database's username.
   */
  static QString getUserName();

  /**
   * Get the database's name.
   * @returns A QString set to the database's name.
   */
  static QString getDatabase();

  /**
   * Get the database password.
   * @returns A QString set to the database's password.
   */
  static QString getPassword();

  /**
   * Get the database port.
   * @returns An int set to the database's port number.
   */
  static int getPort();

  // Set functions.
  /**
   * Set the database's hostname setting.
   * @param host A QString that is set ot he database's hostname.
   */
  static void setHost(QString host);

  /**
   * Set the database's username setting.
   * @param host A QString that is set ot he database's username.
   */
  static void setUserName(QString username);

  /**
   * Set the database's name setting.
   * @param host A QString that is set ot he database's name.
   */
  static void setDatabase(QString database);

  /**
   * Set the database's password setting.
   * @param host A QString that is set ot he database's password.
   */
  static void setPassword(QString password);

  /**
   * Set the database's port number setting.
   * @param host An int that is set ot he database's port number.
   */
  static void setPort(int port);
};

}// namespace fusicSettings
#endif // FUSICSETTINGS_H
