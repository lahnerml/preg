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
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QSettings>
#include <QTimer>

mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::mainwindow),
      m_average_size(
          {{0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  1.6,  2.3,  3.1,
            4.1,  5.4,  7.4,  8.7,  10.1, 11.6, 13.0, 14.2, 15.3, 25.6, 26.7,
            27.8, 28.9, 30.0, 34.6, 35.6, 36.6, 37.6, 38.6, 39.9, 41.1, 42.4,
            43.7, 45.0, 46.2, 47.4, 48.6, 49.8, 50.7, 51.2, 51.7, 51.5}}),
      m_average_weight(
          {{0,    0,    0,    0,    0,    0,    0,    0,    1,    2,    4,
            7,    14,   23,   43,   70,   100,  140,  190,  240,  300,  360,
            430,  501,  600,  660,  760,  875,  1005, 1153, 1319, 1502, 1702,
            1918, 2146, 2383, 2622, 2859, 3083, 3288, 3462, 3597, 3685}})

{
  ui->setupUi(this);

  // Update displayed values every 5 seconds to cope with date changes.
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &::mainwindow::set_current_status);
  timer->start(5000);

  // Connect calendar signals to slots.
  connect(ui->projected_birth_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_start_date);
  connect(ui->projected_start_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_birth_date);
  connect(ui->projected_start_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_current_status);
  connect(ui->projected_birth_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_current_status);

  // Read previously entered values.
  load_settings();
}

mainwindow::~mainwindow() { delete ui; }

void mainwindow::set_birth_date() {
  m_start_date = ui->projected_start_widget->selectedDate();
  m_birth_date = m_start_date.addDays(280);
  ui->projected_birth_widget->setSelectedDate(m_birth_date);
}

void mainwindow::set_start_date() {
  m_birth_date = ui->projected_birth_widget->selectedDate();
  m_start_date = m_birth_date.addDays(-280);
  ui->projected_start_widget->setSelectedDate(m_start_date);
}

void mainwindow::set_current_status() {
  QDate today = QDate::currentDate();
  qint64 days = m_start_date.daysTo(today);
  QString result, measures;
  double avg_size_this, avg_size_next, size_diff;
  int avg_weight_this, avg_weight_next, weight_diff;

  // Only values between 0 and 280 are valid, allow almost 3 weeks
  if (0 <= days && days < 300) {
    int weeks = days / 7;
    int d = days % 7;
    int month = weeks / 4 + 1;

    avg_size_this = m_average_size[weeks + 1];
    avg_weight_this = m_average_weight[weeks + 1];
    // Before week 20 we measure crown to rump, from week 20 onwards we measure
    // crown to heel.  Do not generate a fast growing baby in week 19.
    // Also, do not let baby shrink if its overdue.
    if (weeks == 18) {
      avg_size_next = 16.4;
      avg_weight_next = m_average_weight[weeks + 2];
    } else if (weeks >= 40) {
      avg_size_this = m_average_size[42];
      avg_size_next = m_average_size[42];
      avg_weight_this = m_average_weight[42];
      avg_weight_next = m_average_weight[42];
    } else {
      avg_weight_next = m_average_weight[weeks + 2];
      avg_size_next = m_average_size[weeks + 2];
    }

    size_diff = avg_size_next - avg_size_this;
    weight_diff = avg_weight_next - avg_weight_this;

    result = QString("Week %1").arg(weeks + 1);
    result.append(QString(": W%1").arg(weeks));
    result.append(QString(" +%1").arg(d));
    result.append(QString(" (%1 days,").arg(days));
    days > 280 ? result.append(QString("%1 days overdue; ").arg(days - 280))
               : result.append(QString(" %1 remaining; ").arg(280 - days));
    result.append(QString("%1").arg(month));
    if (month == 1) {
      result.append(QString("st "));
    } else if (month == 2) {
      result.append(QString("nd "));
    } else if (month == 3) {
      result.append(QString("rd "));
    } else {
      result.append(QString("th "));
    }
    result.append(QString("month)"));
    measures = QString("📏 %1 cm").arg(avg_size_this + d / 7. * size_diff);
    weeks < 19 ? measures.append(QString(" (crown to rump)"))
               : measures.append(QString(" (crown to heel)"));
    measures.append(
        QString("\t ⚖ %1 g").arg(avg_weight_this + d / 7. * weight_diff));
  } else {
    result = "";
    measures = "";
    days = 0;
    avg_size_this = 0.0;
    avg_weight_this = 0;
  }
  ui->output_label->setText(result);
  ui->output_label_measures->setText(measures);

  ui->progressBar->setValue(days);
}

void mainwindow::closeEvent(QCloseEvent *event) {
  save_settings();
  event->accept();
}

void mainwindow::load_settings() {
  QSettings settings("preg.ini", QSettings::IniFormat);
  settings.beginGroup("Dates");
  m_start_date = settings.value("start_date").value<QDate>();
  m_birth_date = settings.value("birth_date").value<QDate>();
  ui->projected_start_widget->setSelectedDate(m_start_date);
  settings.endGroup();
}

void mainwindow::save_settings() {
  QSettings settings("preg.ini", QSettings::IniFormat);
  settings.beginGroup("Dates");
  settings.setValue("start_date", m_start_date);
  settings.setValue("birth_date", m_birth_date);
  settings.endGroup();
}
