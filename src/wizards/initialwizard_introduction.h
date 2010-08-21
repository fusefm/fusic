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

#ifndef INITIALWIZARD_INTRODUCTION_H
#define INITIALWIZARD_INTRODUCTION_H

#include <QWizard>


/**
 * @short An introduction page for the initial wizard.
 * This is an introductory wizard page.
 * @see initialWizard
 *
 * @author Matthew Leach <matthew@fusefm.co.uk>
 */
class InitialWizardIntroduction : public QWizardPage
{
  public:
    InitialWizardIntroduction(QWidget* parent = 0);
};

#endif // INITIALWIZARD_INTRODUCTION_H
