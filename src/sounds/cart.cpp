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

#include "cart.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

const QString Cart::getCartText()
{
  mutex.lock();
  mutex.unlock();
  return cartText;
}

const QColor Cart::getColor()
{
  mutex.lock();
  mutex.unlock();
  return color;
}

const CartPosistion Cart::getPosistion()
{
  mutex.lock();
  mutex.unlock();
  return cartPosistion;
}

bool Cart::extraSetup(QSqlDatabase& db, int soundID)
{
  // Ensure we have an active database connection.
  if(!db.isOpen())
    return false;
  
  QSqlQuery query("SELECT * FROM tbl_carts WHERE File_ID = ?");
  query.bindValue(0, soundID);
  query.exec();
  
  // Ensure we have a result.
  if(!query.first())
    return false;
  
  // Find indexes.
  const int fIDPosistion = query.record().indexOf("Cart_ID");
  const int fIDCartText = query.record().indexOf("Cart_Title");
  const int fIDColor = query.record().indexOf("Cart_Colour");
  
  // Get values.
  QString tempPos = query.value(fIDPosistion).toString();
  cartText = query.value(fIDCartText).toString();
  QString tempColor = query.value(fIDColor).toString();
  
  // Do pre-processing.
  if(tempPos.length() != 3)
   return false;
  
  cartPosistion.page = tempPos.left(1).toUInt();
  cartPosistion.row = tempPos.mid(1, 1).toUInt();
  cartPosistion.column = tempPos.right(1).toUInt();
  
  if(tempColor.length() != 9)
    return false;
  
  color.setRgb(tempColor.left(3).toInt(),
               tempColor.mid(3, 3).toInt(),
               tempColor.left(3).toInt());
  
  // Finished setup.
  return true;
}

QSet<Cart*> Cart::getCartsForShowID(int showID)
{
  // Construct a new set.
  QSet<Cart*> cartSet;
  // Connect to the database.
  QSqlDatabase db = QSqlDatabase::database();
  if(!db.open())
    return cartSet;
  
  // Get cart File IDs for the show.
  QSqlQuery query("SELECT File_ID from tbl_carts WHERE Show_ID = ?");
  query.bindValue(0, showID);
  query.exec();
  
  int fIDFileID = query.record().indexOf("File_ID");
  
  while(query.next())
  {
    Cart* newCart = new Cart;
    newCart->setFileID(query.value(fIDFileID).toInt());
    cartSet.insert(newCart);
  }
  
  // Return the result.
  return cartSet;
}
