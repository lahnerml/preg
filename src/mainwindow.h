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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <array>
#include <QDate>
#include <QMainWindow>

namespace Ui {
class mainwindow;
}

class mainwindow : public QWidget {
  Q_OBJECT

public:
  explicit mainwindow(QWidget *parent = nullptr);
  ~mainwindow();

public Q_SLOTS:
  /** Slots catching input from respective calendar widgets and displaying the
   * respective information.
   */
  void set_start_date();
  void set_birth_date();
  void set_current_status();

protected:
  /** Overwrite closeEvent to save entered dates.
   */
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
  Ui::mainwindow *ui;
  QDate m_start_date;
  QDate m_birth_date;
  const std::array<double, 43> m_average_size;
  const std::array<int, 43> m_average_weight;


  /** Read previously entered dates
   */
  void load_settings();

  /** Write selected dates to disk
   */
  void save_settings();
};

#endif // MAINWINDOW_H
