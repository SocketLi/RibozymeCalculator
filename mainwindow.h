#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPainter>
#include <QMainWindow>
#include "RibozymeCalculator.h"
#include "ui_mainwindow.h"
#include<QTableView>
#include<QPoint>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>
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
     void OnBoxChange(const QString& text);
     //void on_export_2_clicked();

protected:
     bool eventFilter(QObject *watched, QEvent *event);
private:
     void SetUpPictureWidget();
     void SetUpResultView();
     void CreateRightMenu();
     Ui::MainWindow *ui;
     RibozymeCalculator RNA_Calculator;
     QMenu* RightMenu;
     std::string LastDrawRNASeq,LastDrawRibozymeType,ResultViewRibozymeType;
     unsigned int PictureWidth,PictureHeight;
     bool flag,HasDrawButtonClicked;
};
#endif // MAINWINDOW_H
