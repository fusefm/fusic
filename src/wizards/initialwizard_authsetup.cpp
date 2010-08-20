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
#include "initialwizard_authsetup.h"
#include "authfactory.h"

#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QGroupBox>

initialwizard_authsetup::initialwizard_authsetup(QWidget* parent): QWizardPage(parent)
{
  ui.setupUi(this);

  // Setup the auth module list.
  foreach(authBase* authMod, authFactory::getAuthModules())
  {
    QListWidgetItem* item =  new QListWidgetItem(authMod->getName(), ui.authModulesList);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(Qt::CheckStateRole, Qt::Unchecked);
    item->setData(Qt::UserRole, QVariant::fromValue(authMod));
  }

  connect(ui.authModulesList, SIGNAL(itemActivated(QListWidgetItem*)),
          this, SLOT(itemChanged(QListWidgetItem*)));

  connect(ui.authModulesList, SIGNAL(itemChanged(QListWidgetItem*)),
          this, SLOT(checkChanged(QListWidgetItem*)));

  connect(ui.authModulesList,SIGNAL(itemChanged(QListWidgetItem*)),
          this,SIGNAL(completeChanged()));

  connect(ui.configureButton, SIGNAL(released()),
          this, SLOT(configureButtonPress()));
}

void initialwizard_authsetup::itemChanged(QListWidgetItem* item)
{
  // Get the auth module.
  authBase* authModule = item->data(Qt::UserRole).value<authBase*>();

  // Set the description.
  ui.descriptionLabel->setText(authModule->getDescription());

  // Set the author.
  ui.authorLabel->setText(authModule->getAuthor());

  // if checked, enable the button.
  if(item->data(Qt::CheckStateRole) == Qt::Checked)
    ui.configureButton->setEnabled(true);
  else
    ui.configureButton->setEnabled(false);
}

void initialwizard_authsetup::checkChanged(QListWidgetItem* item)
{
  if(item->data(Qt::CheckStateRole) == Qt::Checked)
    ui.configureButton->setEnabled(true);
  else
    ui.configureButton->setEnabled(false);
}

void initialwizard_authsetup::configureButtonPress()
{
  // Get the auth module.
  authBase* authModule = ui.authModulesList->selectedItems().first()->
    data(Qt::UserRole).value<authBase*>();

  // Show the dialog.
  QDialog* authDialog = authModule->getsettingsDialog(this);
  authDialog->show();
}

bool initialwizard_authsetup::isComplete() const
{
  // Get all items.
  for(int i = 0; i < ui.authModulesList->count(); i++)
  {
    // Check if checked.
    if(ui.authModulesList->item(i)->data(Qt::CheckStateRole) == Qt::Checked)
      return true;
  }
}

#include "initialwizard_authsetup.moc"