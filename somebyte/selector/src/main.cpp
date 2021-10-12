#include <QApplication>
#include "mainwindow.hpp" 

int main (int argc, char** argv)
{
  QApplication app (argc, argv);
  SelectorMainWindow *mw = new SelectorMainWindow;

  QObject::connect (mw, SIGNAL(quit()), &app, SLOT(quit()));
  mw->showNormal();

  int ret =  app.exec ();
  delete mw;
  return ret;
}

