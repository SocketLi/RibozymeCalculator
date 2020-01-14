#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include "ribozyme_calculator.h"
#include "ui_mainwindow.h"
#include<QTableView>
#include"mywidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void SetResultView(std::vector<std::vector<std::string>> CalculateResult);


    ~MainWindow();
private slots:
      void on_calculate_clicked();
protected:
      bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::MainWindow *ui;
    ribozyme_calculator RNA_Calculator;
    MyWidget* DrawArea;
    int flag;
};

#endif // MAINWINDOW_H
