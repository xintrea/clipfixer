#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_viewBufferButton_clicked();

  void on_setupDataFormatButton_clicked();

private:
  Ui::MainWindow *ui;

  QString formatFileName;
  QStringList formats;

  void readFormats();

  void saveFormats(QString text);
};

#endif // MAINWINDOW_H
