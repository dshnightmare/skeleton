#ifndef BODYANGLE_H
#define BODYANGLE_H

#include "Kinect.h"
#include <opencv2\opencv.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include "qdebug.h"

class jplot_2d{
public:
    float plot2d[JointType_Count][2];

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & plot2d;
    }
};
class jplot_3d{
public:
    float plot3d[JointType_Count][3];
    TrackingState status[JointType_Count];
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & plot3d;
        ar & status;
    }
};


class bodyangle
{
    friend class boost::serialization::access;
public:
    int frame;
    static const int        cDepthWidth = 512;
    static const int        cDepthHeight = 424;
    bodyangle();
    void initialize();
    void update(DepthSpacePoint depthSpacePosition[],Joint joints[]);
    void calcangles();
    cv::Mat draw();
    float angles[22];
    jplot_2d tjoint_coordinate_2d;
    jplot_3d tjoint_coordinate_3d;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & frame;
        ar & angles;
        ar & tjoint_coordinate_2d;
        ar & tjoint_coordinate_3d;
    }

private:
    void DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
    float calc_angle_verticalt(Joint joint0, Joint joint1);
    float calc_angle_vertical(const Joint* pJoints, JointType joint0, JointType joint1);
    float calc_angle3(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2);
    float calc_angle4(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2, JointType joint3);
    float calc_dis(const Joint* pJoints, JointType joint0, JointType joint1);

};


#endif // BODYANGLE_H
