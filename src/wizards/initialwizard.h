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

#ifndef INITIALWIZARD_H
#define INITIALWIZARD_H

#include <QWizard>


/**
 * @short A class that represents the initial wizard for fusic setup.
 * This wizard is shown the first time the program is run. It will
 * setup things that fusic needs to run, such as database and authentication
 * settings. If you would like to add another wizard page to this class,
 * simply add it to the constructor function.
 *
 * @author Matthew Leach <matthew@fusefm.co.uk>
 */
class InitialWizard : public QWizard
{
  public:
    InitialWizard(QWidget* parent = 0, Qt::WindowFlags flags = 0);
};

#endif // INITIALWIZARD_H
