#pragma once

#include <QApplication>

class QtApplication : public QApplication {
  Q_OBJECT
 public:
  QtApplication(int& argc, char** argv);
};