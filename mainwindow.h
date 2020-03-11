#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include "RibozymeCalculator.h"
#include "ui_mainwindow.h"
#include<QTableView>
#include<QPoint>
#include"common.h"
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
     void OnRightMenuClicked(QPoint ClickPos);
     void OnCopyActionClicked();
     void OnDrawActionClicked();
protected:
     bool eventFilter(QObject *watched, QEvent *event);
private:
     void SetUpPictureWidget();
     void SetUpResultView();
     void CreateRightMenu();
     Ui::MainWindow *ui;
     RibozymeCalculator RNA_Calculator;
     QMenu* RightMenu;
     std::string RibozymeSeq,MatchRNASeq,ResultViewRibozymeType;
     unsigned int PictureWidth,PictureHeight;
     bool flag;
};
#endif // MAINWINDOW_H
