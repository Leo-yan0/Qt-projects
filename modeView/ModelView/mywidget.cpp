#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    //打印qt支持的数据版本
    qDebug()<<QSqlDatabase::drivers();

    //添加sqlite数据库
    QSqlDatabase db= QSqlDatabase::addDatabase("QMYSQL");

    //设置数据库
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("ly12345");
    db.setDatabaseName("qsql");
    //打开数据库
    if(!db.open())
    {
        QMessageBox::warning(this,"failure",db.lastError().text());
        return;
    }
    //设置模型
    model=new QSqlTableModel(this);
    model->setTable("student");  //使用哪个表

    //把model放在view里面
    ui->tableView->setModel(model);

    //显示model里的数据
    model->select();

    model->setHeaderData(0,Qt::Horizontal,"学号");

    //设置model 的编辑模式 手动提交修改
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设定view中的数据库不允许修改
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_AddBtn_clicked()
{
    //添加空记录
    QSqlRecord record=model->record();//获取空记录
    //获取行号
    int row=model->rowCount();
    model->insertRecord(row,record);
}

void MyWidget::on_CertainBtn_clicked()
{
    model->submitAll(); //提交所有动作
}

void MyWidget::on_CancelBtn_clicked()
{
    model->revertAll(); //取消所有动作
    model->submitAll(); //提交所有动作
}

void MyWidget::on_DeleteBtn_clicked()
{
    //获取选中的模型
    QItemSelectionModel *selmod= ui->tableView->selectionModel();
    //取出模型里面的索引
    QModelIndexList list= selmod->selectedRows();
    //删除所有选中的行
    for(int i=0;i<list.size();i++)
    {
        model->removeRow(list.at(i).row());
    }
}

void MyWidget::on_FindBtn_clicked()
{
    QString name=ui->lineEdit->text();
    QString str=QString("name='%1'").arg(name);
    model->setFilter(str);
    model->select();
}
