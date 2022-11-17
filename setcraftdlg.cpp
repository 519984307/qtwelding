#include "setcraftdlg.h"
#include "ui_setcraftdlg.h"

setcraftDlg::setcraftDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraftDlg)
{
    ui->setupUi(this);

    adoubleValidator_3 = new QDoubleValidator(0,0,3,this);//限制3位小数
    adoubleValidator_pose = new QDoubleValidator(0,0,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数
    adoubleValidator_posture = new QDoubleValidator(0,0,ROBOT_POSTURE_DECIMAL_PLACE,this);//限制4位小数
    ui->lineEdit_X->setValidator(adoubleValidator_pose);
    ui->lineEdit_Y->setValidator(adoubleValidator_pose);
    ui->lineEdit_Z->setValidator(adoubleValidator_pose);
    ui->lineEdit_RX->setValidator(adoubleValidator_posture);
    ui->lineEdit_RY->setValidator(adoubleValidator_posture);
    ui->lineEdit_RZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_pendulum_swing->setValidator(adoubleValidator_3);
    ui->lineEdit_pendulum_phaseangle->setValidator(adoubleValidator_3);


    m_mcs=mcs;

    for(int n=0;n<PENDULUM_ID_TOTAL_NUM;n++)
    {
        QString msg=m_mcs->craft->Pendulum_mode_toQString((Pendulum_mode)n);
        ui->comboBox_pendulum_mode->addItem(msg);
    }
}

setcraftDlg::~setcraftDlg()
{
    delete adoubleValidator_3;
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setcraftDlg::init_dlg_show()
{
    ui->comboBox_pendulum_mode->setModelColumn(m_mcs->craft->pendulum_mode);
    if(m_mcs->craft->posturelist.size()==1)
    {
        ui->lineEdit_X->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->craft->posturelist[0].RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->craft->posturelist[0].RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->craft->posturelist[0].RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    else
    {
        ui->lineEdit_X->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    ui->lineEdit_pendulum_swing->setText(QString::number(m_mcs->craft->pendulum_swing,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_pendulum_phaseangle->setText(QString::number(m_mcs->craft->pendulum_phaseangle,'f',3));
    UpdataUi();
}

void setcraftDlg::close_dlg_show()
{

}

void setcraftDlg::on_pushButton_clicked()//获取当前焊接姿态
{
    int num=0;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(ROB_WORK_DELAY_STEP);
        num++;
    }
    if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取当前机器人坐标完成"));
    }
}

void setcraftDlg::on_comboBox_pendulum_mode_currentIndexChanged(int index)//摆焊模式变化
{
    m_mcs->craft->pendulum_mode=(Pendulum_mode)index;
    UpdataUi();
}

void setcraftDlg::on_pushButton_2_clicked()//确定并保存
{
    bool ok;
    m_mcs->craft->posturelist.resize(1);
    m_mcs->craft->posturelist[0].X=ui->lineEdit_X->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("X内容格式错误"));
        return;
    }
    m_mcs->craft->posturelist[0].Y=ui->lineEdit_Y->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("Y内容格式错误"));
        return;
    }
    m_mcs->craft->posturelist[0].Z=ui->lineEdit_Z->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("Z内容格式错误"));
        return;
    }
    m_mcs->craft->posturelist[0].RX=ui->lineEdit_RX->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("RX内容格式错误"));
        return;
    }
    m_mcs->craft->posturelist[0].RY=ui->lineEdit_RY->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("RY内容格式错误"));
        return;
    }
    m_mcs->craft->posturelist[0].RZ=ui->lineEdit_RZ->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("RZ内容格式错误"));
        return;
    }
    m_mcs->craft->pendulum_phaseangle=ui->lineEdit_pendulum_phaseangle->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("XY相角内容格式错误"));
        return;
    }
    m_mcs->craft->pendulum_swing=ui->lineEdit_pendulum_swing->text().toFloat(&ok);
    if(ok==false)
    {
        ui->record->append(QString::fromLocal8Bit("摆幅内容格式错误"));
        return;
    }
    m_mcs->craft->SaveProject((char*)m_mcs->craft->craft_path.toStdString().c_str());
    done(1);
}

void setcraftDlg::UpdataUi()
{
    switch(m_mcs->craft->pendulum_mode)
    {
        case PENDULUM_ID_FLAT:
        {
            ui->lineEdit_pendulum_phaseangle->setDisabled(false);
            ui->lineEdit_pendulum_swing->setDisabled(false);
        }
        break;
        default:
        {
            ui->lineEdit_pendulum_phaseangle->setDisabled(true);
            ui->lineEdit_pendulum_swing->setDisabled(true);
        }
        break;
    }
}


