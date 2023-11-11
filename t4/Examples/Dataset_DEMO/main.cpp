#include <iostream>
#include <fstream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/eigen.hpp>
#include "Camera_pose.h"
#include "slam_visualizer.h"
/*****NAMESPACE*****/

using namespace std;
using namespace cv;
using namespace Eigen;
/*****MACRO*****/
#define USEDATASET 1
#define USECOUT 0
/*****CLASSDEF*****/
Camera_pose Pose_2D;
SlamVisualizer visualizer(1500, 960);

//图片序列地址
//const string img_path = "/home/whf/文档/ORB_SLAM2/data/rgbd_dataset_freiburg1_room/rgb";
//const char* traj_path = "/home/whf/文档/ORB_SLAM2/data/rgbd_dataset_freiburg1_room/groundtruth.txt";
//const char* rgb_path = "/home/whf/文档/ORB_SLAM2/data/rgbd_dataset_freiburg1_room/rgb.txt";
// 图片时间名称队列
queue<double> ImagSampleTime;
queue<string>  ImageTimeName;
vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses;
// 相机轨迹容器
vector<Eigen::Vector3d> traj;
cv::Mat img,imglast,imgout;
vector<KeyPoint> keypoints_1;
vector<KeyPoint> keypoints_2;
vector<DMatch> matches;

int main(int argc, char **argv) {
    if(argc != 2)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings path_to_sequence" << endl;
        return 1;
    }
    //读取图片索引
    string img_path = string(argv[1]) + "/rgb";
    string rgb_path = string(argv[1]) + "/rgb.txt";
    string traj_path = string(argv[1]) + "/groundtruth.txt";
#if USEDATASET
    ifstream fin_rgb(rgb_path);
    ifstream fin_traj(traj_path);
    if (!fin_rgb||!fin_traj) {
        cout << "cannot find trajectory file at " << endl;
        return 1;
    }
    //fin.eof()读取到文件结尾返回true
    while (!fin_rgb.eof()) {
        double time;
        string imagename;
        fin_rgb >> time >> imagename;
        ImagSampleTime.push(time);
        ImageTimeName.push(imagename);
#if USECOUT
        cout <<fixed << setprecision(6) << time <<","<< imagename <<endl;
#endif
    }
    // 初始化视窗
    visualizer.initDraw();
    while (!fin_traj.eof()) {
        double time, tx, ty, tz, qx, qy, qz, qw;
        fin_traj >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        if(time > ImagSampleTime.front()) {
            while (abs(time - ImagSampleTime.front()) >= 0.01) {
                ImagSampleTime.pop();
            }
        }
        // 常规操作
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        visualizer.activeAllView();
        // 注册ui回调函数
        visualizer.registerUICallback();
        // 两数据时间匹配
        if(abs(time - ImagSampleTime.front()) <= 0.01) {
            Eigen::Quaterniond quat(qw, qx, qy, qz);
            Eigen::Vector3d pos(tx, ty, tz);
            traj.push_back(pos);
            // 显示数据
            visualizer.displayData(pos, quat);
            // 绘制轨迹可视化部分
            visualizer.drawCoordinate();
            visualizer.drawCamWithPose(pos, quat);
            visualizer.drawTraj(traj);
            //不是第一次进入
            if(traj.size() != 1){
                string img_file = "/home/whf/文档/ORB_SLAM2/data/rgbd_dataset_freiburg1_room/" + ImageTimeName.front();
                img = cv::imread(img_file);
                Pose_2D.find_feature_matches(img,imglast,keypoints_1,keypoints_2,matches);
                drawKeypoints(img,keypoints_1,imgout,Scalar(0, 255, 0),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                visualizer.displayImg(img, imgout);
                imglast = img;
                ImagSampleTime.pop();
                ImageTimeName.pop();
            }else{
                string img_file = "/home/whf/文档/ORB_SLAM2/data/rgbd_dataset_freiburg1_room/" + ImageTimeName.front();
                img = cv::imread(img_file);
                imglast = img;
                visualizer.displayImg(img, img);
                ImagSampleTime.pop();
                ImageTimeName.pop();
            }
            // 循环与退出判断
            pangolin::FinishFrame();
            if (pangolin::ShouldQuit())
                break;
        }

    }
#endif
}


