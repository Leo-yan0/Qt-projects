#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>
namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

private slots:
    void on_AddBtn_clicked();

    void on_CertainBtn_clicked();

    void on_CancelBtn_clicked();

    void on_DeleteBtn_clicked();

    void on_FindBtn_clicked();

private:
    Ui::MyWidget *ui;
    QSqlTableModel *model;
};

#endif // MYWIDGET_H
