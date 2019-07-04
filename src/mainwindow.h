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
  std::array<double, 41> m_average_size;
  std::array<int, 41> m_average_weight;


  /** Read previously entered dates
   */
  void load_settings();

  /** Write selected dates to disk
   */
  void save_settings();
};

#endif // MAINWINDOW_H
