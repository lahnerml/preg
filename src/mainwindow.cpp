#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QSettings>
#include <QTimer>

mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::mainwindow) {
  ui->setupUi(this);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &::mainwindow::set_current_status);
  timer->start(5000);

  connect(ui->projected_birth_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_start_date);
  connect(ui->projected_start_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_birth_date);
  connect(ui->projected_start_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_current_status);
  connect(ui->projected_birth_widget, &QCalendarWidget::selectionChanged, this,
          &::mainwindow::set_current_status);

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
  QString result;
  if (0 <= days && days < 280) {
    result = QString("Week %1").arg(days / 7);
    result.append(QString(" + %1").arg(days % 7));
    result.append(QString(" (%1 days)").arg(days));
  } else {
    result = "";
    days = 0;
  }
  ui->output_label->setText(result);
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
