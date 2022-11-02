#ifndef ROBOTDATA_H
#define ROBOTDATA_H
#include "global.h"
#include <modbus/modbus.h>
#include "calibration.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>
#if _MSC_VER
#include <QDir>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#define ROBOTDATA_PATH_MOTO       "./SAVE/robdata.bsd"

typedef enum ROBOT_MODEL_ID            //机器人型号
{
    ROBOT_MODEL_NULL=0,                //无机器人
    ROBOT_MODEL_EMERGEN=1,             //智昌机器人
}ROBOT_MODEL;

typedef enum ROBOT_STATE_ID            //机器人状态
{
    ROBOT_STATE_UNLINK=-1,              //未连接
    ROBOT_STATE_IDLE=0,                 //空闲
    ROBOT_STATE_MOVE=1,                 //忙碌
    ROBOT_STATE_STOP=2,                 //急停
}ROBOT_STATE;

class sent_info_robot             //发送机器人数据
{
public:
    modbus_t *ctx;                  //sock
    int addr;                       //发送寄存器
    std::vector<uint16_t> data;     //发送数据
};

class RobotData
{
public:
    static RobotData *Get();
    RobPos TCPpos;      //机器人当前TCP坐标

    ROBOT_MODEL robot_model;    //机器人型号
    QString robot_model_toQString();  //机器人型号字符串输出

    ROBOT_STATE robot_state;    //机器人状态
    QString robot_state_toQString();  //机器人状态字符串输出

    CAL_POSTURE cal_posture_model;  //机器人姿态标准

    float robot_speed;         //机器人当前移动速度

    bool b_connect;             //是否连接机器人

    int ConnectRobot(QString ip,int port);         //连接机器人

    void DisconnectRobot();

    modbus_t *ctx_posget;       //机器人坐标访问
    bool b_link_ctx_posget;     //机器人坐标访问连接

    std::vector<sent_info_robot> send_group_robot;    //发送机器人数据队列
    volatile bool b_send_group_robot;       //发送机器人数据是否异常
    int ctx_robot_dosomeing;    //机器人端口忙

    int SaveRob(char* filename);    //保存项目

protected:
    RobotData();
    ~RobotData();

    int LoadRob(char* filename);    //读取项目

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码
};



#endif // ROBOTDATA_H
