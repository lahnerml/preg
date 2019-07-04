#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QSettings>
#include <QTimer>

mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::mainwindow) {
  ui->setupUi(this);

  m_average_size = {{0.0,  0.0,  0.0,  0.0,  0.0,  0.2,  0.4,  0.5,  1.5,
                     2.0,  2.5,  3.0,  5.0,  6.0,  7.1,  8.1,  9.4,  10.6,
                     12.0, 24.0, 25.0, 26.0, 28.0, 30.0, 31.0, 33.0, 34.0,
                     36.0, 37.0, 39.0, 40.0, 24.0, 43.0, 44.0, 45.0, 46.0,
                     47.0, 48.0, 49.0, 50.0, 51.0}};
  m_average_weight = {{0,    0,    0,    0,    0,    0,    0,    0,    0,
                       0,    0,    15,   16,   18,   25,   50,   90,   100,
                       125,  200,  300,  350,  475,  580,  700,  800,  900,
                       1000, 1100, 1250, 1400, 1600, 1800, 2000, 2250, 2550,
                       2750, 2950, 3100, 3250, 3400}};

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
