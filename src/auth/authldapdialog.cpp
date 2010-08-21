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

#include "authldapdialog.h"
#include "ldapaccessgrouppicker.h"
#include <QSettings>
#include <QMessageBox>

AuthLDAPDialog::AuthLDAPDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  ui.setupUi(this);

  connect(ui.okCancelButtonBox, SIGNAL(clicked(QAbstractButton*)),
          this, SLOT(okOrCancelButtonPress(QAbstractButton*)));

  connect(ui.accessGroupsButton, SIGNAL(pressed()), this,
          SLOT(configureAccessgroupsPress()));
}

void AuthLDAPDialog::okOrCancelButtonPress(QAbstractButton* button)
{
  // Check if it's the Ok button:
  if(ui.okCancelButtonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
  {
    if(validate())
    {
      // Save settings:
      QSettings set;

      // Compose the LDAP URI from the settings.
      QString ldapURI = "ldap://";
      ldapURI += ui.hostnameEdit->text();
      if(!ui.portEdit->text().isEmpty())
      {
        ldapURI += ":";
        ldapURI += ui.portEdit->text();
      }

      // Set the value.
      set.setValue("LDAPSettings/URI", ldapURI);
      set.setValue("LDAPSettings/BindAttribute", ui.bindAttributeEdit->text());
      set.setValue("LDAPSettings/BaseDN", ui.baseDNEdit->text());

      // Close the dialog.
      accept();
    }
  }
}

bool AuthLDAPDialog::validate()
{
  QSettings set;
  if(ui.hostnameEdit->text().isEmpty())
  {
    QMessageBox::critical(this, "Error", "Please fill in the hostname field.");
    return false;
  }
  else if(ui.baseDNEdit->text().isEmpty())
  {
    QMessageBox::critical(this, "Error", "Please fill in the Base DN field.");
    return false;
  }
  else if(ui.bindAttributeEdit->text().isEmpty())
  {
    QMessageBox::critical(this, "Error", "Please fill in the Bind Attribute field.");
    return false;
  }
  else if(set.value("LDAPSettings/AccessGroups").toStringList().isEmpty())
  {
    QMessageBox::critical(this, "Error", "Please ensure that at least one access"
      " group is selected.");
      return false;
  }
  else
  {
    return true;
  }
}

void AuthLDAPDialog::configureAccessgroupsPress()
{
  LDAPAccessGroupPicker* accessGroupDialog = new
    LDAPAccessGroupPicker(this);
    accessGroupDialog->show();
}


#include "authldapdialog.moc"