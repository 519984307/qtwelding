#include "robotdata.h"

RobotData *RobotData::Get()
{
    static RobotData fun;
    return &fun;
}

RobotData::RobotData()
{
#if _MSC_VER
    QString filePath =  "./SAVE";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
#else
    std::string dir = "./SAVE";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./SAVE",S_IRWXU);
    }
#endif

    b_connect=false;
    b_link_ctx_posget=false;
    robot_speed=0;
    robot_state=ROBOT_STATE_UNLINK;

    if(0!=LoadRob(ROBOTDATA_PATH_MOTO))
    {
        robot_model=ROBOT_MODEL_NULL;
    }
}

RobotData::~RobotData()
{

}

QString RobotData::robot_model_toQString()
{
    QString msg;
    switch(robot_model)
    {
    case ROBOT_MODEL_NULL:
        msg="无";
        break;
    case ROBOT_MODEL_EMERGEN:
        msg="智昌";
        break;
    }
    return msg;
}

QString RobotData::robot_state_toQString()
{
    QString msg;
    switch(robot_state)
    {
    case ROBOT_STATE_UNLINK:
        msg="未链接";
        break;
    case ROBOT_STATE_IDLE:
        msg="空闲";
        break;
    case ROBOT_STATE_MOVE:
        msg="移动";
        break;
    case ROBOT_STATE_SCAN:
        msg="扫描";
        break;
    case ROBOT_STATE_TRACE:
        msg="跟踪";
        break;
    }
    return msg;
}

int RobotData::ConnectRobot(QString ip,int port)         //连接机器人
{
    if(b_link_ctx_posget==false)
    {
        ctx_posget = modbus_new_tcp(ip.toUtf8(), port);
        if (modbus_connect(ctx_posget) == -1)
        {
            modbus_free(ctx_posget);
            return 1;
        }
        b_link_ctx_posget=true;
    }
    robot_state=ROBOT_STATE_IDLE;
    b_connect=true;
    return 0;
}

void RobotData::DisconnectRobot()
{
    if(b_connect==true)
    {
         if(b_link_ctx_posget==true)
         {
            modbus_close(ctx_posget);
            modbus_free(ctx_posget);
            b_link_ctx_posget=false;
         }
         b_connect=false;
         robot_state=ROBOT_STATE_UNLINK;
    }
}

int RobotData::SaveRob(char* filename)
{
    QVariantHash data=encoed_json();

    QJsonObject rootObj = QJsonObject::fromVariantHash(data);
    QJsonDocument document;
    document.setObject(rootObj);

    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    //根据实际填写路径
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file error!";
        return 1;
    }
    QTextStream in(&file);
    in << json_str;

    file.close();   // 关闭file

    return 0;
}

int RobotData::LoadRob(char* filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to load JSON file";
        return 1;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    if(0!=decoed_json(allData))
        return 1;

    return 0;
}

QVariantHash RobotData::encoed_json()
{
    QVariantHash data;

    data.insert("robot_model", robot_model);

    return data;
}

int RobotData::decoed_json(QByteArray allData)
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if (json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON error!";
        return 1;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonObject::Iterator it;
    for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
    {
        QString keyString=it.key();
        if(keyString=="robot_model")//机器人
        {
            robot_model=(ROBOT_MODEL)it.value().toInt();
        }
    }

    return 0;
}
