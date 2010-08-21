#ifndef CARTWALL_H
#define CARTWALL_H

#include <qt4/QtGui/QMainWindow>

namespace Ui
{
  class CartWall;
}

class CartWall : public QMainWindow
{
  Q_OBJECT
  
  public:
    CartWall(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    
  private:
    Ui::CartWall* ui;
};

#endif // CARTWALL_H
