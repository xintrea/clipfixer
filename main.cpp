#include "MainWindow.h"
#include <QApplication>

// v.0.4

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
