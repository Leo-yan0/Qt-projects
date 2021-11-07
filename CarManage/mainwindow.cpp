#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "domxml.h"
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1000,800);
    this->setWindowTitle("CarManager");
    ui->toolBox->setCurrentIndex(0);
    on_actionvehicleMana_triggered();
    //获取当前使用的数据库;
    QSqlDatabase::database();

    //初始化数据
    initData();

    //初始化使用modeView
    InitModeView();

    //创建 数据存储xml
    DomXML::createXML("./demo.xml");

}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化数据
void MainWindow::initData()
{
    //获取当前使用的数据库;
    QSqlDatabase::database();
    //定义模型
    QSqlQueryModel *queryModel=new QSqlQueryModel(this);
    queryModel->setQuery("select name from factory");//sql语句
    ui->comboBoxFactory->setModel(queryModel);

    ui->labelLast->setText("0");
    ui->lineEditTotalPrice->setEnabled(false);
}

//初始化modeView
void MainWindow::InitModeView()
{
    //设置模型
    model=new QSqlTableModel(this);
    model->setTable("brand");  //使用哪个表

    //把model放在view里面
    ui->tableView->setModel(model);

    //显示model里的数据
    model->select();

    model->setHeaderData(0,Qt::Horizontal,"编号");

    //设置model 的编辑模式 手动提交修改
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设定view中的数据库不允许修改
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//修改密码
void MainWindow::on_actionChangePwd_triggered()
{
    //创建修改密码窗口
    pwdChange=new ChangePwd;
    pwdChange->setGeometry(this->geometry());
    this->hide();
    pwdChange->show();
    connect(pwdChange,&ChangePwd::back,[=](){
        pwdChange->hide();
        this->setGeometry(pwdChange->geometry());
       this->show();
    });

}

//车辆管理
void MainWindow::on_actionvehicleMana_triggered()
{
    //切换到车辆管理
    ui->stackedWidget->setCurrentWidget(ui->car);
    ui->label->setText("车辆管理");
}


//销售统计
void MainWindow::on_actionsaleSta_triggered()
{
    //切换到
    ui->stackedWidget->setCurrentWidget(ui->calc);
    ui->label->setText("销售统计");
}

//厂家下拉框槽函数
void MainWindow::on_comboBoxFactory_currentIndexChanged(const QString &arg1)
{

    if(arg1=="请选择厂家")
    {
        //内容清空
        ui->comboBoxBrand->clear();
        ui->lineEditPrice->clear();
        ui->lineEditTotalPrice->clear();
        ui->labelLast->setText("0");
        ui->spinBox->setValue(0);
        ui->spinBox->setEnabled(false);
        ui->sureBtn->setEnabled(false);

    }
    else {

        //清空品牌下拉框上次执行的结果也留在其中
        ui->comboBoxBrand->clear();
        QSqlQuery query;
        QString str=QString("select name from brand where factory='%1'").arg(arg1);
        //执行sql语句
        query.exec(str);

        //获取内容
        while(query.next())
        {
            QString name=query.value(0).toString();
            ui->comboBoxBrand->addItem(name);
        }
        ui->spinBox->setEnabled(true);
    }
}

//品牌下拉框槽函数
void MainWindow::on_comboBoxBrand_currentIndexChanged(const QString &arg1)
{

    QSqlQuery query;
    QString str=QString("select price,last from brand where factory='%1' and name='%2'")
            .arg(ui->comboBoxFactory->currentText()).arg(arg1);
    //执行sql语句
    query.exec(str);

    //获取内容
    while(query.next())
    {
        //报价
        int price=query.value("price").toInt();
        //剩余数
        int last=query.value("last").toInt();

        //更新到对应的控件
        ui->lineEditPrice->setText(QString::number(price));
        ui->labelLast->setText(QString::number(last));

    }
}

//数据选择框槽函数
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    //大于零时才能点击确认按钮
    if(arg1>0)
    {
        ui->sureBtn->setEnabled(true);
    }
    else{
        ui->sureBtn->setEnabled(false);
    }
    //更新剩余数量
    //获取数据库剩余数量
    QSqlQuery query;
    QString str=QString("select sum,last from brand where factory='%1' and name='%2'")
            .arg(ui->comboBoxFactory->currentText()).arg(ui->comboBoxBrand->currentText());
    query.exec(str);
    int last=0;
    while(query.next())
    {
        last=query.value("last").toInt();

    }
    //如果选择的数目大于剩余数
    if(arg1>last){
        ui->spinBox->setValue(last);
        return;
    }

    //数据库中剩余量减去数据选择框的数 就是实际剩余数量
    int tempNum=last -arg1;
    ui->labelLast->setText(QString::number(tempNum));

    int sumPrice=ui->lineEditPrice->text().toInt()*arg1;

    ui->lineEditTotalPrice->setText(QString::number(sumPrice));

}

//确定按钮  修改数据库内容
void MainWindow::on_sureBtn_clicked()
{
    //获取信息
    //获取销售数据
    int num=ui->spinBox->value();
    //剩余数量
    int last=ui->labelLast->text().toInt();

    //获取数据库的销量
    QSqlQuery query;
    QString str=QString("select last,sell from brand where factory='%1' and name='%2'")
            .arg(ui->comboBoxFactory->currentText()).arg(ui->comboBoxBrand->currentText());
    query.exec(str);
    int sell=0;
    while(query.next())
    {
        sell=query.value("sell").toInt();

    }
    //更新数据库 剩余量 销量
    sell+=num;
    str=QString("update brand set sell=%1,last=%2 where factory='%3' and name='%4'")
            .arg(sell).arg(last).arg(ui->comboBoxFactory->currentText())
            .arg(ui->comboBoxBrand->currentText());
    if(!query.exec(str))
    {
        QMessageBox::information(this,"warning",query.lastError().text());
    }
    else {
        //xml
        QStringList list;
        list<<ui->comboBoxFactory->currentText()<<ui->comboBoxBrand->currentText()
           <<ui->lineEditPrice->text()
          <<QString::number(num)
         <<ui->lineEditTotalPrice->text();
        DomXML::appendXML("./demo.xml",list);

        //显示信息
        QStringList fl,bl,pl,nl,tl;
        DomXML::readXML("./demo.xml",fl,bl,pl,nl,tl);
        for (int i=0;i<fl.size();i++) {
            QString str=QString("[%1] %2::%3 卖出了%4台，单价%5, 总价：%6")
                    .arg(QDateTime::currentDateTime().toString("yyyy-mm-dd hh-mm-ss"))
                    .arg(fl.at(i))
                    .arg(bl.at(i))
                    .arg(nl.at(i))
                    .arg(pl.at(i))
                    .arg(tl.at(i));
            ui->textEdit->append(str);
        }

        //修改数据库数据
        QMessageBox::information(this,"prompt","数据修改成功");
        on_cancelBtn_clicked();
        model->select();
    }

}

//取消按钮  回复原来状态
void MainWindow::on_cancelBtn_clicked()
{
    //on_comboBoxFactory_currentIndexChanged("请选择厂家");
    ui->comboBoxFactory->setCurrentIndex(0);
}

//车辆入库，车辆销售切换槽函数
void MainWindow::on_toolBox_currentChanged(int index)
{
    ui->toolBox->setCurrentIndex(index);
    if(index==0)
        ui->textEdit->show();
    else {
        ui->textEdit->hide();
    }
}


//modeView槽函数
//点击添加，则添加数据
void MainWindow::on_AddBtn_clicked()
{
    //添加空记录
    QSqlRecord record=model->record();//获取空记录
    //获取行号
    int row=model->rowCount();
    model->insertRecord(row,record);
}

//确定按钮 提交记录到数据库
void MainWindow::on_CertainBtn_clicked()
{
    model->submitAll(); //提交所有动作
}

//取消按钮 取消所有动作
void MainWindow::on_CancelBtn_clicked()
{
    model->revertAll(); //取消所有动作
    model->submitAll(); //提交所有动作
}

//删除按钮 删除选定的行
void MainWindow::on_DeleteBtn_clicked()
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

//查找按钮 按照车名查找记录
void MainWindow::on_FindBtn_clicked()
{
    QString name=ui->lineEdit->text();
    QString str=QString("name='%1'").arg(name);
    model->setFilter(str);
    model->select();
}
