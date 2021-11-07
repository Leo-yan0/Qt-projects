#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include "changepwd.h"
#include <QSqlQueryModel>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //初始化数据
    void initData();
    //初始化 可视化数据库操作窗口
    void InitModeView();

private slots:
    void on_actionChangePwd_triggered();

    void on_actionvehicleMana_triggered();

    void on_actionsaleSta_triggered();

    void on_comboBoxFactory_currentIndexChanged(const QString &arg1);

    void on_comboBoxBrand_currentIndexChanged(const QString &arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_sureBtn_clicked();

    void on_cancelBtn_clicked();

    void on_toolBox_currentChanged(int index);

    //新车入库槽函数

    void on_AddBtn_clicked();

    void on_DeleteBtn_clicked();

    void on_CertainBtn_clicked();

    void on_CancelBtn_clicked();

    void on_FindBtn_clicked();

private:
    Ui::MainWindow *ui;
    ChangePwd *pwdChange;
    QSqlTableModel *model;
};

#endif // MAINWINDOW_H
