#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <global.h>

#define  CAL_RADIAN			(CV_PI/180.0)

typedef enum CAL_POSTURE_MODEL_ID          //机器人姿态角含义模式
{
    CAL_ROBOT_YASKAWA=0,        //安川姿态标准
    CAL_ROBOT_KUKA=1,           //库卡姿态标准
    CAL_ROBOT_PANASONIC=2,      //松下姿态标准
}CAL_POSTURE;

class Calibration
{
public:
    Calibration();
    ~Calibration();
    //眼在手外的标定,计算旋转和标定矩阵
    static bool hand_out_yes_point2RT(std::vector<Eigen::Vector3d> &p1,     //输入手坐标
                                       std::vector<Eigen::Vector3d> &p2,    //输入眼坐标
                                       Eigen::Matrix3d &R,                  //输出旋转矩阵
                                       Eigen::Vector3d &T,                  //输出平移举证
                                       double &err,                         //输出总体精度误差
                                       std::vector<double> &errgroup);      //输出单个精度误差

    //眼在手上的标定,计算旋转和标定矩阵
    static bool hand_on_yes_point2RT(CAL_POSTURE robot,                     //输入姿态标准
                                     RobPos robpos,                         //输入标定点手坐标
                                     std::vector<TCP_Leaserpos> data_group, //输入标定点眼坐标
                                     cv::Mat &matrix_camera2plane,          //输出眼到中间矩阵
                                     cv::Mat &matrix_plane2robot,           //输出中间矩阵到机器人矩阵
                                     double &err,                           //输出总体精度误差
                                     std::vector<double> &errgroup);        //输出单个精度误差

protected:

   static cv::Mat OLS_Plane_V(std::vector<Eigen::Vector3d> points);
   static Eigen::Matrix3d ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2);
   static cv::Mat computePlane2Robot_posture(cv::Mat mp0,cv::Mat mp_r,cv::Mat mp_s,Eigen::Matrix3d pst);
   static cv::Mat computePlane2Robot(cv::Mat plane, cv::Mat robot,int num);
   static Eigen::Matrix3d Euler2RotMatrixX(const double rx);
   static Eigen::Matrix3d Euler2RotMatrixY(const double ry);
   static Eigen::Matrix3d Euler2RotMatrixZ(const double rz);
   static Eigen::Matrix3d Yaskawa_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Kuka_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Panasonic_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Euler2RotMatrixXYZ(CAL_POSTURE robot,std::array<double,3> pst);
   static bool computeMatrix(CAL_POSTURE robot,RobPos robpos,std::vector<TCP_Leaserpos> data_group,cv::Mat &matrix_camera2plane,cv::Mat &matrix_plane2robot);
   static cv::Point3f ComputePosition(CAL_POSTURE robot,TCP_Leaserpos Pic,cv::Mat &matrix_camera2plane,cv::Mat &matrix_plane2robot);
   static cv::Mat ComputePos(cv::Mat Robot_Position,cv::Mat Pic_Position,Eigen::Matrix3d RPY_Rotation,cv::Mat &matrix_plane2robot);
};

#endif // CALIBRATION_H