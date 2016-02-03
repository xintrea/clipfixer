#include <QMessageBox>
#include <QClipboard>
#include <QInputDialog>
#include <QFile>

#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  formatFileName="formats.txt";
  readFormats();

}


MainWindow::~MainWindow()
{
  delete ui;
}


// Загрузка форматов из файла
void MainWindow::readFormats()
{
  formats.clear();

  QFile formatFile(formatFileName);

  bool result=formatFile.open(QIODevice::ReadOnly | QIODevice::Text);

  if(!result)
  {
    QMessageBox msgBox;
    msgBox.setText("Список форматов пуст. Воспользуйтесь настройкой чтобы добавить форматы.");
    msgBox.exec();

    formatFile.close();

    return;
  }

  // Заполняется список форматов
  while(!formatFile.atEnd())
  {
    QByteArray line = formatFile.readLine();

    QString format=QString(line);

    if(format.trimmed().length()>0)
      formats << format.trimmed();
  }

  ui->currentDataFormatComboBox->clear();
  ui->currentDataFormatComboBox->addItems(formats);

  formatFile.close();
}


// Запись форматов, разделенных символом перевода строки
void MainWindow::saveFormats(QString text)
{
  QFile formatFile(formatFileName);

  bool result=formatFile.open(QIODevice::WriteOnly | QIODevice::Text);

  if(!result)
  {
    QMessageBox msgBox;
    msgBox.setText("Невозможно сохранить файл настроек.");
    msgBox.exec();

    formatFile.close();

    return;
  }

  formatFile.write(text.toUtf8());
  formatFile.close();
}


void MainWindow::on_viewBufferButton_clicked()
{
  // QMessageBox msgBox;
  // msgBox.setText("The document has been modified.");
  // msgBox.exec();

  QClipboard *clipboard = QApplication::clipboard();

  QString subType="html";
  QString html=clipboard->text(subType, QClipboard::Clipboard);

  ui->sourceFormattingText->setText(html);
  ui->sourceText->setPlainText(html);
}



void MainWindow::on_setupDataFormatButton_clicked()
{
  QString firstText;
  foreach (const QString &line, formats)
    firstText=firstText+line+"\n";

  bool result;
  QString text = QInputDialog::getMultiLineText(this,
      "Допустимые форматы",
      "Форматы в виде строк: Наименование, кол-во столбцов для столбца 1, кол-востолбцов для столбца 2, и. т. д",
      firstText,
      &result);

  if(result && !text.isEmpty())
  {
    saveFormats(text);
    readFormats();
  }

}

