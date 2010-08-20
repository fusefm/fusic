/*
    fusic - Radio playout software
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

#ifndef CART_H
#define CART_H

// Local includes:
#include "sound.h"

// Qt Includes:
#include <QColor>
#include <QSet>

/**
 * @short A struct for representing the posistion of a cart.
 * This struct contatins members for defining the posistion
 * of a cart on a cart wall.
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
struct posistion
{
  /**
   * The page number for the cart.
   * Can be in range 1 - 8.
   */
  unsigned int page;
  
  /**
   * The column number of the cart.
   * Can be in range 1 - 4.
   */
  unsigned int column;
  
  /**
   * The row number of the cart.
   * can be in range 1 - 5.
   */
  unsigned int row;
};

/**
 * @short A class for represeing a cart in Fusic.
 * This class extends the sound class to add members
 * specific for carts in fusic.
 *
 * @author Matthew Leach <matt@fusefm.co.uk>
 */
class cart : public sound
{
  public:
    /**
     * Get the posistion of the cart.
     * @returns a posistion struct set to the relevant posistion.
     */
    const struct posistion getPosistion();
    
    /**
     * Get the carts text to display on the cart button.
     * @returns a QString set to the text of the cart.
     */
    const QString getCartText();
    
    /**
     * Get the color of the cart.
     * @returns a QColor set to the color of the cart.
     */
    const QColor getColor();
    
    /**
     * Return all carts for a given show.
     * @param showID The ID number of the show.
     * @returns A QSet of cart class pointers.
     */
    static QSet<cart*> getCartsForShowID(int showID);
  
  private:
    virtual bool extraSetup(QSqlDatabase& db, int soundID);
    struct posistion cartPosistion;
    QString cartText;
    QColor color;
};

#endif // CART_H
