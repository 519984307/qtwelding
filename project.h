#ifndef PROJECT_H
#define PROJECT_H

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

#define PROJECT_PATH_MOTO       "./SAVE/path.json"

typedef enum PROJECT_ID
{
    PROGECT_ID_TEACH_SCAN=0,       //示教扫描类型
}Project_ID;

class Project
{
public:
    static Project *Get();

    QString project_path;       //最后一次项目路径

    Project_ID project_Id;      //项目总类型id

    QString project_Id_toQString(); //项目总类型id转QString输出

    QString project_name;       //项目名

    int SaveProject(char* filename);    //保存项目

    int LoadProject(char* filename);    //读取项目

    int SaveProjectPath(char* filename);    //保存最后一次项目路径，以便下次打开

    int LoadProjectPath(char* filename);    //读取最后一次项目路径，以便下次打开

protected:
    Project();
    ~Project();

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码

    QVariantHash encoedpath_json(); //把项目路径进行json编码

    int decoedpath_json(QByteArray allData);//把项目路径进行json解码

    QString JsonToQstring(QJsonObject jsonObject);

    QJsonObject QstringToJson(QString jsonString);
};

#endif // PROJECT_H
