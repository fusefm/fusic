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

#include "ldapaccessgrouppicker.h"
#include <QMessageBox>
#include <QSettings>

LDAPAccessGroupPicker::LDAPAccessGroupPicker(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  QSettings set;
  // Setup the UI.
  ui.setupUi(this);

  // Make connections.
  connect(ui.addButton, SIGNAL(pressed()), this, SLOT(addAccessGroupPress()));
  connect(ui.okButtonBox, SIGNAL(clicked(QAbstractButton*)),
          this, SLOT(okButtonGroupBoxPress(QAbstractButton*)));

  // Prepopulate with settings.
  foreach(QString accessGroup,
          set.value("LDAPSettings/AccessGroups").toStringList())
  {
    QListWidgetItem* item = new QListWidgetItem(accessGroup,
                                                ui.groupNameList);
  }
}

void LDAPAccessGroupPicker::addAccessGroupPress()
{
  // Ensure that there is some text.
  if(!ui.groupNameEdit->text().isEmpty())
  {
    // Add the text to the list.
    QListWidgetItem* item = new QListWidgetItem(
      ui.groupNameEdit->text(), ui.groupNameList);

    // Also clear the lineedit widget and set focus.
    ui.groupNameEdit->clear();
    ui.groupNameEdit->setFocus();
  }
}

void LDAPAccessGroupPicker::removeAccessGroupPress()
{
  // Ensure that there is a selection.
  if(!ui.groupNameList->selectedItems().isEmpty())
  {
    // Remove item from the list.
    ui.groupNameList->removeItemWidget(
      ui.groupNameList->selectedItems().first());
  }
}

void LDAPAccessGroupPicker::okButtonGroupBoxPress(QAbstractButton* btn)
{
  if(ui.okButtonBox->buttonRole(btn) == QDialogButtonBox::AcceptRole)
  {
    // Ensure that there is at least one item.
    if(ui.groupNameList->count() == 0)
    {
      QMessageBox::critical(this, "Error", "Please select at least one access group.");
      return;
    }
    else
    {
      QStringList accessGroups;
      QSettings set;
      for(int i = 0; i < ui.groupNameList->count(); i++)
      {
        accessGroups.append(ui.groupNameList->item(i)->text());
      }

      // Save the selection.
      set.setValue("LDAPSettings/AccessGroups", accessGroups);

      // Exit the dialog.
      accept();
    }
  }
}

#include "ldapaccessgrouppicker.moc"