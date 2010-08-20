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

#ifndef INITIALWIZARD_AUTHSETUP_H
#define INITIALWIZARD_AUTHSETUP_H

// Local Includes:
#include "ui_initialwizard_authsetup.h"

// Qt Includes:
#include <QWizard>
#include <QListWidget>
#include <QLabel>

/**
 * @short Authentication confiugration page.
 * This wizard page provides configuration options for authentication
 * backend selection and configuration.
 *
 * @note No fields are exported by this page as all the settings
 * are internally stored in the auth module.
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
class initialwizard_authsetup : public QWizardPage
{
  Q_OBJECT
  public:
    /**
     * The constructor will setup the UI and populate the
     * auth modules list with the avaliable authentication
     * modules provided by the authFactory class.
     *
     * @param parent The parent dialog.
     */
    initialwizard_authsetup(QWidget* parent = 0);

  private slots:

    /**
     * This slot is called when the user has selected a differnt
     * authentication module from the list. It will update the
     * author and description field as well as enabling the
     * configure button (if required).
     *
     * @param item The item thas has been selected.
     */
    void itemChanged(QListWidgetItem* item);

    /**
     * This slot is called when the user changes the checked
     * state on an authentication module. This subsequently
     * changes the enabled state on the configure button.
     *
     * @param item The item that has had it's checked state
     * changed.
     */
    void checkChanged(QListWidgetItem* item);

    /**
     * This slot is called when the user presses the configure
     * module button and will show the authentication modules
     * configure dialog.
     */
    void configureButtonPress();

    /**
     * Override the default checking mechinism to ensure
     * a user can not progress through the wizard until
     * they have selected at least one authentication module.
     *
     * @returns A bool, true if can progress, false if otherwise.
     */
    virtual bool isComplete() const;

  private:
    Ui_WizardPage ui;
};

#endif // INITIALWIZARD_AUTHSETUP_H
