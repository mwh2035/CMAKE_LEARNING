//
// Created by whf on 10/19/23.
//

#ifndef VO_CAMERA_POSE_H
#define VO_CAMERA_POSE_H
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace Eigen;

class Camera_pose {
public: void find_feature_matches(const Mat &img_1, const Mat &img_2,
                                  std::vector<KeyPoint> &keypoints_1,
                                  std::vector<KeyPoint> &keypoints_2,
                                  std::vector<DMatch> &matches);
        Point2d pixel2cam(const Point2d &p, const Mat &K);
        void pose_estimation_2d2d(std::vector<KeyPoint> keypoints_1,
                                  std::vector<KeyPoint> keypoints_2,
                                  std::vector<DMatch> matches,
                                  Mat &R, Mat &t);
        Isometry3d cvMat2Eigen( Mat& rvec, Mat& tvec );
};


#endif //VO_CAMERA_POSE_H
