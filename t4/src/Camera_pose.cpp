//
// Created by whf on 10/19/23.
//

#include "Camera_pose.h"

void Camera_pose::find_feature_matches(const cv::Mat &img_1, const cv::Mat &img_2,
                                       std::vector<KeyPoint> &keypoints_1,
                                       std::vector<KeyPoint> &keypoints_2,
                                       std::vector<DMatch> &matches) {
    matches.clear();
    //-- 初始化
    Mat descriptors_1, descriptors_2;
    // 创建ORB特征检测子
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    //匹配子采用汉明距离
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    //-- 第一步:检测 Oriented FAST 角点位置
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    //-- 第二步:根据角点位置计算 BRIEF 描述子
    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    //-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
    vector<DMatch> match;
    matcher->match(descriptors_1, descriptors_2, match);
    //-- 第四步:匹配点对筛选
    double min_dist = 10000, max_dist = 0;
    //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
    for (int i = 0; i < descriptors_1.rows; i++) {
        double dist = match[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }
    //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.
    for (int i = 0; i < descriptors_1.rows; i++) {
        if (match[i].distance <= max(2 * min_dist, 30.0)) {
            matches.push_back(match[i]);
        }
    }
}

Point2d Camera_pose::pixel2cam(const cv::Point2d &p, const cv::Mat &K) {
    return Point2d
            (
                    (p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
                    (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1)
            );
}

void Camera_pose::pose_estimation_2d2d(std::vector<KeyPoint> keypoints_1,
                                       std::vector<KeyPoint> keypoints_2,
                                       std::vector<DMatch> matches,
                                       cv::Mat &R, cv::Mat &t) {
    // 相机内参,TUM Freiburg2
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    //-- 把匹配点转换为vector<Point2f>的形式
    vector<Point2f> points1;
    vector<Point2f> points2;

    for (int i = 0; i < (int) matches.size(); i++) {
        //记录特征点在本帧图像的索引
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        //记录特征点在上帧图像的索引
        points2.push_back(keypoints_2[matches[i].trainIdx].pt);
    }

    //-- 计算基础矩阵
    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat(points1, points2,FM_8POINT);

    //-- 计算本质矩阵
    Point2d principal_point(325.1, 249.7);  //相机光心, TUM dataset标定值
    double focal_length = 521;      //相机焦距, TUM dataset标定值
    Mat essential_matrix;
    essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);

//    //-- 计算单应矩阵
//    //-- 但是本例中场景不是平面，单应矩阵意义不大
//    Mat homography_matrix;
//    homography_matrix = findHomography(points1, points2, RANSAC, 3);

    //-- 从本质矩阵中恢复旋转和平移信息.
    // 此函数仅在Opencv3中提供
    recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);
}

Isometry3d Camera_pose::cvMat2Eigen(cv::Mat &rvec, cv::Mat &tvec) {
    Matrix3d r;
    cv2eigen(rvec,r);
    // 将平移向量和旋转矩阵转换成变换矩阵
    Isometry3d T = Eigen::Isometry3d::Identity();
    //转旋转向量
    AngleAxisd angle(r);
    Translation<double,3> trans(tvec.at<double>(0,0), tvec.at<double>(0,1), tvec.at<double>(0,2));
    T = angle;
    T(0,3) = tvec.at<double>(0,0);
    T(1,3) = tvec.at<double>(0,1);
    T(2,3) = tvec.at<double>(0,2);
    return T;
}