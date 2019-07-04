#include "mainwindow.h"
#include <QtCore/QDebug>
#include <QtCore/QEventLoop>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QPixmap oPixmap(32, 32);
  oPixmap.load("systemTrayIcon.png");
  QIcon oIcon(oPixmap);

  QSystemTrayIcon *trayIcon = new QSystemTrayIcon(oIcon);
  trayIcon->show();
  mainwindow w;
  bool isVisible = true;
  w.setWindowIcon(oIcon);

  QEventLoop::connect(trayIcon, &QSystemTrayIcon::activated, [&]() {
    w.setVisible(isVisible);
    if (isVisible) {
      w.raise();
    }
    isVisible = !isVisible;
  });

  return app.exec();
}
