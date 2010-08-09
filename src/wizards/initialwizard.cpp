/*
    fusic - radio playout software.
    Copyright (C) 2010  Matthew Leach

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or	 FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "initialwizard.h"
#include "initialwizard_introduction.h"
#include "initialwizard_mysqlsetup.h"

initialWizard::initialWizard(QWidget* parent, Qt::WindowFlags flags): QWizard(parent, flags)
{
  addPage(new initialwizard_introduction);
  addPage(new initialwizard_mysqlSetup);
}