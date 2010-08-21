#include "cartwall.h"
#include "ui_cartwall.h"

CartWall::CartWall(QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{
  ui = new Ui::CartWall;
  ui->setupUi(this);
}

#include "cartwall.moc"