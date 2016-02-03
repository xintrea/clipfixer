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

  void on_processingButton_clicked();

  void on_currentDataFormatComboBox_currentTextChanged(const QString &arg1);

private:
  Ui::MainWindow *ui;

  QString formatFileName;
  QStringList formats;
  QString currentFormat;

  void readFormats();
  void saveFormats(QString text);
  QString getHtmlFromClipboard();
};

#endif // MAINWINDOW_H
