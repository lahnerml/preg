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
  double avg_size;
  int avg_weight;

  // Only values between 0 and 280 are valid.
  if (0 <= days && days < 280) {
    int weeks = days / 7;
    avg_size = m_average_size[weeks + 1];
    avg_weight = m_average_weight[weeks + 1];

    result = QString("Week %1").arg(weeks + 1);
    result.append(QString(": W%1").arg(weeks));
    result.append(QString(" +%1").arg(days % 7));
    result.append(QString(" (%1 days)").arg(days));
    measures = QString("📏 %1 cm").arg(avg_size);
    measures.append(QString("\t ⚖ %1 g").arg(avg_weight));
  } else {
    result = "";
    measures = "";
    days = 0;
    avg_size = 0.0;
    avg_weight = 0;
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
