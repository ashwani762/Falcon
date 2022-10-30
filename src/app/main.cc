#include <iostream>
#include <QtApplication.h>
#include <QWindow>

auto main(int argc, char **argv) -> int {

  QtApplication qtApp(argc, argv);
  QWindow w;
  w.show();
  return qtApp.exec();
}