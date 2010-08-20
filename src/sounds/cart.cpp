#include "cart.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

const QString cart::getCartText()
{
  mutex.lock();
  mutex.unlock();
  return cartText;
}

const QColor cart::getColor()
{
  mutex.lock();
  mutex.unlock();
  return color;
}

const posistion cart::getPosistion()
{
  mutex.lock();
  mutex.unlock();
  return cartPosistion;
}

bool cart::extraSetup(QSqlDatabase& db, int soundID)
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