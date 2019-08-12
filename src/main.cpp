// MIT License
//
// Copyright (c) 2019 Michael Lahnert
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
