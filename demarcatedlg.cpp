#include "demarcatedlg.h"
#include "ui_demarcatedlg.h"

demarcateDlg::demarcateDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::demarcateDlg)
{
    ui->setupUi(this);

    m_mcs=mcs;  

    switch(m_mcs->e2proomdata.demdlg_radio_mod)
    {
        case 0:
            ui->radio1->setChecked(1);
        break;
        case 1:
            ui->radio2->setChecked(1);
        break;
    }

    ui->a00_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(0,0),'f',3));
    ui->a01_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(0,1),'f',3));
    ui->a02_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(0,2),'f',3));
    ui->a10_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(1,0),'f',3));
    ui->a11_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(1,1),'f',3));
    ui->a12_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(1,2),'f',3));
    ui->a20_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(2,0),'f',3));
    ui->a21_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(2,1),'f',3));
    ui->a22_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(2,2),'f',3));
    ui->t0_label->setText(QString::number(m_mcs->e2proomdata.demdlg_T(0),'f',3));
    ui->t1_label->setText(QString::number(m_mcs->e2proomdata.demdlg_T(1),'f',3));
    ui->t2_label->setText(QString::number(m_mcs->e2proomdata.demdlg_T(2),'f',3));
}

demarcateDlg::~demarcateDlg()
{
    delete ui;
}

void demarcateDlg::init_dlg_show()
{
    now_robpos=m_mcs->e2proomdata.demdlg_Robotpos.size()-1;
    now_leaserpos=m_mcs->e2proomdata.demdlg_Leaserpos.size()-1;
    updataRoblistUi();
    updataLeaserlistUi();
}

void demarcateDlg::close_dlg_show()
{

}

void demarcateDlg::on_radio1_clicked()      //眼在手上
{
    m_mcs->e2proomdata.demdlg_radio_mod=0;
}


void demarcateDlg::on_radio2_clicked()      //眼在手外
{
    m_mcs->e2proomdata.demdlg_radio_mod=1;
}

void demarcateDlg::on_pushButton_clicked()  //添加TCP点
{
    int num=0;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(10000);
        num++;
    }
    if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        if(now_robpos==m_mcs->e2proomdata.demdlg_Robotpos.size()-1)
        {
            m_mcs->e2proomdata.demdlg_Robotpos.push_back(m_mcs->rob->TCPpos);
        }
        else
        {
            m_mcs->e2proomdata.demdlg_Robotpos.insert(m_mcs->e2proomdata.demdlg_Robotpos.begin()+now_robpos+1,m_mcs->rob->TCPpos);
        }
        ui->record->append(QString::fromLocal8Bit("添加TCP点成功"));
        now_robpos++;
        updataRoblistUi();
    }
}

void demarcateDlg::on_pushButton_3_clicked() //删除TCP点
{
    if(now_robpos>=0&&m_mcs->e2proomdata.demdlg_Robotpos.size()>now_robpos)
    {
        m_mcs->e2proomdata.demdlg_Robotpos.erase(m_mcs->e2proomdata.demdlg_Robotpos.begin()+now_robpos);
        ui->record->append(QString::fromLocal8Bit("删除TCP点"));
        if(now_robpos>=m_mcs->e2proomdata.demdlg_Robotpos.size())
            now_robpos=now_robpos-1;
        updataRoblistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要删除的TCP点"));
    }
}


void demarcateDlg::on_pushButton_2_clicked()   //替换TCP点
{
    if(now_robpos>=0&&m_mcs->e2proomdata.demdlg_Robotpos.size()>now_robpos)
    {
        int num=0;
        m_mcs->rob->TCPpos.nEn=false;
        while (m_mcs->rob->TCPpos.nEn==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(10000);
            num++;
        }
        if(m_mcs->rob->TCPpos.nEn==false)
        {
            ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
        }
        else
        {
            m_mcs->e2proomdata.demdlg_Robotpos[now_robpos]=m_mcs->rob->TCPpos;
            ui->record->append(QString::fromLocal8Bit("替换TCP点成功"));
            updataRoblistUi();
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要替换的TCP点"));
    }
}


void demarcateDlg::on_pushButton_8_clicked()    //清空TCP点
{
    m_mcs->e2proomdata.demdlg_Robotpos.clear();
    ui->record->append(QString::fromLocal8Bit("清空TCP点"));
    now_robpos=m_mcs->e2proomdata.demdlg_Robotpos.size()-1;
    updataRoblistUi();
}


void demarcateDlg::on_pushButton_4_clicked()    //添加激光头点
{
    int num=0;
    m_mcs->resultdata.pos1.nEn=false;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->resultdata.pos1.nEn==false||
           m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(10000);
        num++;
    }
    if(m_mcs->resultdata.pos1.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取激光头坐标失败"));
    }
    else if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        if(now_leaserpos==m_mcs->e2proomdata.demdlg_Leaserpos.size()-1)
        {
            TCP_Leaserpos sing;
            sing.robotpos=m_mcs->rob->TCPpos;
            sing.leaserpos=m_mcs->resultdata.pos1;
            m_mcs->e2proomdata.demdlg_Leaserpos.push_back(sing);
        }
        else
        {
            TCP_Leaserpos sing;
            sing.robotpos=m_mcs->rob->TCPpos;
            sing.leaserpos=m_mcs->resultdata.pos1;
            m_mcs->e2proomdata.demdlg_Leaserpos.insert(m_mcs->e2proomdata.demdlg_Leaserpos.begin()+now_leaserpos+1,sing);
        }
        ui->record->append(QString::fromLocal8Bit("添加TCP点成功"));
        now_leaserpos++;
        updataLeaserlistUi();
    }
}


void demarcateDlg::on_pushButton_6_clicked()    //删除激光头点
{
    if(now_leaserpos>=0&&m_mcs->e2proomdata.demdlg_Leaserpos.size()>now_leaserpos)
    {
        m_mcs->e2proomdata.demdlg_Leaserpos.erase(m_mcs->e2proomdata.demdlg_Leaserpos.begin()+now_leaserpos);
        ui->record->append(QString::fromLocal8Bit("删除激光头点"));
        if(now_leaserpos>=m_mcs->e2proomdata.demdlg_Leaserpos.size())
            now_leaserpos=now_leaserpos-1;
        updataLeaserlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要删除的激光头点"));
    }
}


void demarcateDlg::on_pushButton_5_clicked()    //替换激光头点
{
    if(now_leaserpos>=0&&m_mcs->e2proomdata.demdlg_Leaserpos.size()>now_leaserpos)
    {
        int num=0;
        m_mcs->resultdata.pos1.nEn=false;
        m_mcs->rob->TCPpos.nEn=false;
        while (m_mcs->resultdata.pos1.nEn==false||
               m_mcs->rob->TCPpos.nEn==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(10000);
            num++;
        }
        if(m_mcs->resultdata.pos1.nEn==false)
        {
            ui->record->append(QString::fromLocal8Bit("获取激光头坐标失败"));
        }
        else if(m_mcs->rob->TCPpos.nEn==false)
        {
            ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
        }
        else
        {
            TCP_Leaserpos sing;
            sing.robotpos=m_mcs->rob->TCPpos;
            sing.leaserpos=m_mcs->resultdata.pos1;
            m_mcs->e2proomdata.demdlg_Leaserpos[now_leaserpos]=sing;
            ui->record->append(QString::fromLocal8Bit("替换激光头点成功"));
            updataLeaserlistUi();
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要替换的激光头点"));
    }
}


void demarcateDlg::on_pushButton_9_clicked()     //清空激光头点
{
    m_mcs->e2proomdata.demdlg_Leaserpos.clear();
    ui->record->append(QString::fromLocal8Bit("清空激光头点"));
    now_leaserpos=m_mcs->e2proomdata.demdlg_Leaserpos.size()-1;
    updataLeaserlistUi();
}


void demarcateDlg::on_pushButton_7_clicked()      //计算标定结果
{
    if(m_mcs->e2proomdata.demdlg_Leaserpos.size()!=m_mcs->e2proomdata.demdlg_Robotpos.size())
    {
        ui->record->append(QString::fromLocal8Bit("TCP坐标点要与激光头坐标点个数相同"));
    }
    else if(m_mcs->e2proomdata.demdlg_Robotpos.size()<4)
    {
        ui->record->append(QString::fromLocal8Bit("TCP坐标点个数要至少大于4个"));
    }
    else
    {
        m_mcs->e2proomdata.write_demdlg_para();



        ui->record->append(QString::fromLocal8Bit("标定完成"));
    }
}

void demarcateDlg::updataRoblistUi()
{
    ui->robposlist->clear();
    for(int n=0;n<m_mcs->e2proomdata.demdlg_Robotpos.size();n++)
    {
        QString msg;
        msg="TCP"+QString::number(n)+":"+
            QString::number(m_mcs->e2proomdata.demdlg_Robotpos[n].X,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Robotpos[n].Y,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Robotpos[n].Z,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Robotpos[n].RX,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Robotpos[n].RY,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Robotpos[n].RZ,'f',3);
        ui->robposlist->addItem(msg);
    }
    if(m_mcs->e2proomdata.demdlg_Robotpos.size()>0)
    {
        ui->robposlist->setCurrentRow(now_robpos);
    }
}

void demarcateDlg::updataLeaserlistUi()
{
    ui->leaserposlist->clear();
    for(int n=0;n<m_mcs->e2proomdata.demdlg_Leaserpos.size();n++)
    {
        QString msg;
        msg="CAM"+QString::number(n)+":"+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].leaserpos.Y,'f',2)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].leaserpos.Z,'f',2)+" TCP:"+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].robotpos.X,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].robotpos.Y,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].robotpos.Z,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].robotpos.RX,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].robotpos.RY,'f',3)+","+
            QString::number(m_mcs->e2proomdata.demdlg_Leaserpos[n].robotpos.RZ,'f',3);

        ui->leaserposlist->addItem(msg);
    }
    if(m_mcs->e2proomdata.demdlg_Leaserpos.size()>0)
    {
        ui->leaserposlist->setCurrentRow(now_leaserpos);
    }
}

void demarcateDlg::on_robposlist_itemClicked(QListWidgetItem *item)
{
    now_robpos=ui->robposlist->currentRow();
}


void demarcateDlg::on_leaserposlist_itemClicked(QListWidgetItem *item)
{
    now_leaserpos=ui->leaserposlist->currentRow();
}

