#ifndef CART_H
#define CART_H

// Local includes:
#include "sound.h"

// Qt Includes:
#include <QColor>

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
  
  private:
    virtual bool extraSetup(QSqlDatabase& db, int soundID);
    struct posistion cartPosistion;
    QString cartText;
    QColor color;
};

#endif // CART_H
