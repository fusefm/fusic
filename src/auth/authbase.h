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

#ifndef AUTHBASE_H
#define AUTHBASE_H

// Qt Includes:
#include <QMap>
#include <QThread>
#include <QDialog>

/**
 * @short A pure virtual class for defining how authentication should work.
 * 
 * This is a pure virtual class that should be used for creating authentication
 * modules for fusic. This is done easily via overwriting the doAuth() function.
 * There is also functions for setAuthor() and setDescription() that allow you 
 * to describe what kind of authentication this module provides and it's author.
 *
 * <h3>Module Settings</h3>
 * If the module requires settings to be retieved, then use the QSettings class.
 * There is also a function that allows you to return a QMap with a setting name 
 * and description that can is used to build a setting UI for configuring the module.
 *
 * @note For the moment, fusic only uses basic username and password authentication.
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
class authBase : public QThread
{
  Q_OBJECT
public:
  // Constructor.
  authBase(QObject* parent = 0);
  
  /**
   * Authentication modules should put the code here that actually does
   * the authentication. This will be spawned in a new thread to prevent
   * UI lockup. When authentication has finished, emit the finishedAuth
   * signal, with the parameter set to weather the authentication was
   * successful or not. If an error occured, emit the authError signal.
   *
   * Use the protected m_userID and m_password members for credentials.
   */
  virtual void doAuth();

  /**
   * Return the name of the module.
   * @returns A QString set to the name of the module.
   */
  virtual QString getName();
  
  /**
   * Modules should setup a UI for setting settings for that module.
   * the UI should also include a function for setting the settings 
   * within the dialog.
   *
   * @param parent The parent widget for the dialog.
   *
   * @returns Should return a pointer to a new QDialog that has been setup with the
   * nesseccry widgets and setting save functions.
   */
  virtual const QDialog* getsettingsDialog(QWidget* parent);
  
  /**
   * Return the author of the module.
   * 
   * @returns A QString set to the author of the module.
   */
  const QString getAuthor();
  
  /**
   * Return the description of the module.
   *
   * @returns a QString that is set to a breif description to
   * what kind authentication the module provides.
   */
  const QString getDescription();
  
  /**
   * Set the user's credentials to authenticate against. 
   * @note Ensure this is called before running the 
   * authentication.
   *
   * @param userID The users ID.
   * @param password The users password.
   */
  void setCredentials(const QString& userID, const QString& password);
  
  /**
   * Run authentication in a new thread.
   */
  void run();
  
signals:
  
  /**
   * This signal should be emitted once authentication has finished,
   * regardless as to weather the user has logged in successully or not.
   *
   * @param successful If set to true, the user was authenticated. If false,
   * the user didn't authenticate given the credentials.
   */
  void finishedAuth(const bool successful);
  
  /**
   * Emit this signal if there was an error doing the authentication.
   * 
   * @param msg A short error message saying what went wrong.
   */
  void authError(const QString msg);

protected:
  
  /**
   * Set the author of the module.
   *
   * @param author The author of the module.
   */
  void setAuthor(const QString author);
  
  /**
   * Set the description of the module. Provide a breif
   * description as to what kind of authentication the module does.
   *
   * @param description The description of the module.
   */
  void setDescription(const QString description);
  
  /**
   * The user ID to authenticate against.
   */
  QString mUserID;
  
  /**
   * The password to authenticate against.
   */
  QString mPassword;
  
private:
  QString mAuthor;
  QString mDescription;
};

#endif // AUTHBASE_H
