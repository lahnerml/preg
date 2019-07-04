#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class mainwindow;
}

class mainwindow : public QWidget {
  Q_OBJECT

public:
  explicit mainwindow(QWidget *parent = nullptr);
  ~mainwindow();

public Q_SLOTS:
  void set_start_date();
  void set_birth_date();
  void set_current_status();

protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
  Ui::mainwindow *ui;
  QDate m_start_date;
  QDate m_birth_date;

  void load_settings();
  void save_settings();
};

#endif // MAINWINDOW_H
