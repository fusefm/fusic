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

#ifndef LDAPACCESSGROUPPICKER_H
#define LDAPACCESSGROUPPICKER_H

// Local Includes:
#include "ui_ldapaccessgrouppicker.h"

// Qt Includes:
#include <QDialog>

/**
 * @short A small dialog that allows the user to select LDAP access groups.
 * This class will allow a user to edit access groups that are
 * permitted to log into fusic via the LDAP auth module.
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
class LDAPAccessGroupPicker : public QDialog
{
  Q_OBJECT
  public:
    /**
     * Pre-populate the list with already defined groups.
     * @param parent The parent dialog.
     * @param f The window flags.
     */
    LDAPAccessGroupPicker(QWidget* parent = 0, Qt::WindowFlags f = 0);

  private slots:
    /**
     * Called when the user presses the add button.
     * Simply adds the text from the lineEdit widget to the
     * ListItem widget.
     */
    void addAccessGroupPress();

    /**
     * Called when the user preses the remove button.
     * Remove an access group from the list when pressed.
     */
    void removeAccessGroupPress();

    /**
     * Ensures that there is at least one access group
     * defined before saving the groups and calling accept();
     */
    void okButtonGroupBoxPress(QAbstractButton* btn);

  private:
    Ui::LDAPAccessGroupChooserUI ui;
};

#endif // LDAPACCESSGROUPPICKER_H
