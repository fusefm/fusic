#include "fusic.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

Fusic::Fusic()
{
    QLabel* l = new QLabel( this );
    l->setText( "Hello World!" );
    setCentralWidget( l );
    QAction* a = new QAction(this);
    a->setText( "Quit" );
    connect(a, SIGNAL(triggered()), SLOT(close()) );
    menuBar()->addMenu( "File" )->addAction( a );
}

Fusic::~Fusic()
{}

#include "fusic.moc"
