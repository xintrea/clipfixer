#include <QMessageBox>
#include <QClipboard>
#include <QInputDialog>
#include <QFile>
#include <QTextTable>
#include <QMimeData>

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


QString MainWindow::getHtmlFromClipboard()
{
  QClipboard *clipboard = QApplication::clipboard();

  QString subType="html";
  QString html=clipboard->text(subType, QClipboard::Clipboard);

  return html;
}


void MainWindow::on_viewBufferButton_clicked()
{
  QString html=getHtmlFromClipboard();

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
      "Один формат на одной строке: Наименование, кол-во столбцов для столбца 1, кол-во столбцов для столбца 2, и. т. д",
      firstText,
      &result);

  if(result && !text.isEmpty())
  {
    saveFormats(text);
    readFormats();
  }

}


void MainWindow::on_processingButton_clicked()
{
  QString html=getHtmlFromClipboard();

  // Создается документ с содержимым, взятым из HTML-представления буфера обмена
  QTextDocument doc;
  doc.setHtml(html);

  QTextCursor cursor(&doc);

  cursor.movePosition(QTextCursor::WordRight);

  QTextTable *table=cursor.currentTable();

  if(table==0)
  {
    QMessageBox msgBox;
    msgBox.setText("Буфер обмена не содержит выделенных ячеек.");
    msgBox.exec();
    return;
  }

  // Запоминается ширина и высота таблицы
  int startTableColumns=table->columns();
  int startTableRows=table->rows();

  // Список ширин столбцов (сколько столбцов надо объединять)
  QStringList columnWidthListLine = currentFormat.split(",");
  columnWidthListLine.removeFirst(); // Удаляется название формата

  // Преобразование в список int
  QList<int> columnWidthList;
  foreach(const QString &widthLine, columnWidthListLine)
    columnWidthList << widthLine.toInt();

  // Объединение столбцов
  int i=0;
  foreach(const int &currentWidth, columnWidthList)
  {
    table->mergeCells(0, i, startTableRows, currentWidth);

    i=i+currentWidth;
  }

  // ui->translateBufferText->setText(doc.toHtml());

  QClipboard *clipboard = QApplication::clipboard();
  QMimeData *data = new QMimeData;
  data->setHtml( doc.toHtml() );
  clipboard->setMimeData(data);
}


// Если поменялось значение в выпадающем списке форматов
void MainWindow::on_currentDataFormatComboBox_currentTextChanged(const QString &arg1)
{
  currentFormat=arg1;

  // QMessageBox msgBox;
  // msgBox.setText("Установлен новый формат "+currentFormat);
  // msgBox.exec();
}
