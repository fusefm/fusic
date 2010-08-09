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

#include "initialwizard_introduction.h"
#include <QVBoxLayout>
#include <QLabel>

initialwizard_introduction::initialwizard_introduction(QWidget* parent): QWizardPage(parent)
{
  setTitle("Fusic Introduction");
  
  QLabel* introText = new QLabel;
  introText->setText("Welcome to Fusic. The next few pages will"
  " guide you through setting up the database and authentication settings.");
  introText->setWordWrap(true);
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(introText);
  setLayout(layout);
}
