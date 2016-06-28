#include "bodyangle.h"

static cv::Mat skeletonImg;

bodyangle::bodyangle()
{

}

void bodyangle::initialize()
{

}

void bodyangle::update(DepthSpacePoint depthSpacePosition[], Joint joints[])
{
    for (int i=0;i<JointType_Count;i++)
    {
        tjoint_coordinate_3d.plot3d[i][0]=joints[i].Position.X;
        tjoint_coordinate_3d.plot3d[i][1]=joints[i].Position.Y;
        tjoint_coordinate_3d.plot3d[i][2]=joints[i].Position.Z;
        tjoint_coordinate_3d.status[i]=joints[i].TrackingState;
        tjoint_coordinate_2d.plot2d[i][0]=depthSpacePosition[i].X;
        tjoint_coordinate_2d.plot2d[i][1]=depthSpacePosition[i].Y;
    }
}
cv::Mat bodyangle::draw()
{

    skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
    skeletonImg.setTo(0);
    DepthSpacePoint joint_coordinate_2d[JointType_Count];
    Joint joint_coordinate_3d[JointType_Count];
    for (int i=0;i<JointType_Count;i++)
    {
        joint_coordinate_3d[i].Position.X=tjoint_coordinate_3d.plot3d[i][0];
        joint_coordinate_3d[i].Position.Y=tjoint_coordinate_3d.plot3d[i][1];
        joint_coordinate_3d[i].Position.Z=tjoint_coordinate_3d.plot3d[i][2];
        joint_coordinate_3d[i].TrackingState=tjoint_coordinate_3d.status[i];

        joint_coordinate_2d[i].X=tjoint_coordinate_2d.plot2d[i][0];
        joint_coordinate_2d[i].Y=tjoint_coordinate_2d.plot2d[i][1];
    }

    //---------------------------body-------------------------------
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_Head, JointType_Neck);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_Neck, JointType_SpineShoulder);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_SpineShoulder, JointType_SpineMid);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_SpineMid, JointType_SpineBase);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_SpineShoulder, JointType_ShoulderRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_SpineShoulder, JointType_ShoulderLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_SpineBase, JointType_HipRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_SpineBase, JointType_HipLeft);

    // -----------------------Right Arm ------------------------------------
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_ShoulderRight, JointType_ElbowRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_ElbowRight, JointType_WristRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_WristRight, JointType_HandRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_HandRight, JointType_HandTipRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_WristRight, JointType_ThumbRight);

    //----------------------------------- Left Arm--------------------------
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_ShoulderLeft, JointType_ElbowLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_ElbowLeft, JointType_WristLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_WristLeft, JointType_HandLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_HandLeft, JointType_HandTipLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_WristLeft, JointType_ThumbLeft);

    // ----------------------------------Right Leg--------------------------------
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_HipRight, JointType_KneeRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_KneeRight, JointType_AnkleRight);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_AnkleRight, JointType_FootRight);

    // -----------------------------------Left Leg---------------------------------
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_HipLeft, JointType_KneeLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_KneeLeft, JointType_AnkleLeft);
    DrawBone(joint_coordinate_3d, joint_coordinate_2d, JointType_AnkleLeft, JointType_FootLeft);

    return skeletonImg;
}

void bodyangle::DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1)
{
    TrackingState joint0State = pJoints[joint0].TrackingState;
    TrackingState joint1State = pJoints[joint1].TrackingState;

    if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
    {
        return;
    }


    CvPoint p1 = cvPoint(depthSpacePosition[joint0].X, depthSpacePosition[joint0].Y),
        p2 = cvPoint(depthSpacePosition[joint1].X, depthSpacePosition[joint1].Y);

    line(skeletonImg, p1, p2, cvScalar(255, 0, 0),3);

    if (joint0State == TrackingState_Tracked)
        circle(skeletonImg, p1,3,cvScalar(0, 255, 0), -1);
    else
        circle(skeletonImg, p1,3,cvScalar(0, 0, 255), -1);

    if (joint1State == TrackingState_Tracked)
        circle(skeletonImg, p2,3,cvScalar(0, 255, 0), -1);
    else
        circle(skeletonImg, p2,3,cvScalar(0, 0, 255), -1);

}

void bodyangle::calcangles()
{
    DepthSpacePoint joint_coordinate_2d[JointType_Count];
    Joint joint_coordinate_3d[JointType_Count];
    for (int i=0;i<JointType_Count;i++)
    {
        joint_coordinate_3d[i].Position.X=tjoint_coordinate_3d.plot3d[i][0];
        joint_coordinate_3d[i].Position.Y=tjoint_coordinate_3d.plot3d[i][1];
        joint_coordinate_3d[i].Position.Z=tjoint_coordinate_3d.plot3d[i][2];
        joint_coordinate_3d[i].TrackingState=tjoint_coordinate_3d.status[i];

        joint_coordinate_2d[i].X=tjoint_coordinate_2d.plot2d[i][0];
        joint_coordinate_2d[i].Y=tjoint_coordinate_2d.plot2d[i][1];
    }
    //膝关节角度，暂时统一采用臀、膝、踝三节点夹角
    angles[0] = calc_angle3(joint_coordinate_3d, JointType_HipLeft, JointType_KneeLeft, JointType_AnkleLeft);
    angles[1] = calc_angle3(joint_coordinate_3d, JointType_HipRight, JointType_KneeRight, JointType_AnkleRight);

    angles[2] = angles[0];
    angles[3] = angles[1];

    angles[4] = angles[0];
    angles[5] = angles[1];

    //膝间距
    angles[6] = calc_dis(joint_coordinate_3d, JointType_KneeRight, JointType_KneeLeft);
    angles[7] = angles[6];

    //踝间距
    angles[8] = calc_dis(joint_coordinate_3d, JointType_AnkleRight, JointType_AnkleLeft);
    angles[9] = angles[8];

    //髋屈曲
    angles[10] = calc_angle4(joint_coordinate_3d, JointType_KneeLeft, JointType_HipLeft, JointType_SpineMid, JointType_SpineBase);
    angles[11] = calc_angle4(joint_coordinate_3d, JointType_KneeRight, JointType_HipRight, JointType_SpineMid, JointType_SpineBase);

    //髋伸直
    angles[12] = calc_angle4(joint_coordinate_3d, JointType_SpineBase, JointType_SpineMid, JointType_HipLeft, JointType_KneeLeft);
    angles[13] = calc_angle4(joint_coordinate_3d, JointType_SpineBase, JointType_SpineMid, JointType_HipRight, JointType_KneeRight);

    //髋外展
    angles[14] = calc_angle_vertical(joint_coordinate_3d, JointType_HipLeft, JointType_KneeLeft);
    angles[15] = calc_angle_vertical(joint_coordinate_3d, JointType_HipRight, JointType_KneeRight);

    //髋内收
    angles[16] = calc_angle4(joint_coordinate_3d, JointType_SpineBase, JointType_SpineMid, JointType_HipLeft, JointType_KneeLeft);
    angles[17] = calc_angle4(joint_coordinate_3d, JointType_SpineBase, JointType_SpineMid, JointType_HipRight, JointType_KneeRight);

    //髋旋1
    angles[18] = calc_angle_vertical(joint_coordinate_3d, JointType_KneeLeft, JointType_AnkleLeft);
    angles[19] = calc_angle_vertical(joint_coordinate_3d, JointType_KneeRight, JointType_AnkleRight);


    //髋旋2
    angles[20] = calc_angle_vertical(joint_coordinate_3d, JointType_AnkleLeft, JointType_KneeLeft);
    angles[21] = calc_angle_vertical(joint_coordinate_3d, JointType_AnkleRight, JointType_KneeRight);

}

float bodyangle::calc_angle_vertical(const Joint* pJoints, JointType joint0, JointType joint1)
{
    TrackingState joint0state = pJoints[joint0].TrackingState;
    TrackingState joint1state = pJoints[joint1].TrackingState;
    if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked))
    {
        float v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
        float v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
        float v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
        float v2x = 0;
        float v2y = 1.0;
        float v2z = 0;
        float d1 = v1x*v2x + v1y*v2y + v1z*v2z;
        float d2_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
        float d2_2 = pow(v2x, 2) + pow(v2y, 2) + pow(v2z, 2);
        float d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);

        float theta = acos(d1 / d2) * 180 / 3.1415926;
        if (theta<0.1) return 0;
        return theta;
    }
    else
    {
        return 0;
    }
}
float bodyangle::calc_angle3(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2)
{
    TrackingState joint0state = pJoints[joint0].TrackingState;
    TrackingState joint1state = pJoints[joint1].TrackingState;
    TrackingState joint2state = pJoints[joint2].TrackingState;

    if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked) && (joint2state == TrackingState_Tracked))
    {
        float v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
        float v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
        float v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
        float v2x = pJoints[joint2].Position.X - pJoints[joint1].Position.X;
        float v2y = pJoints[joint2].Position.Y - pJoints[joint1].Position.Y;
        float v2z = pJoints[joint2].Position.Z - pJoints[joint1].Position.Z;

        float d1 = v1x*v2x + v1y*v2y + v1z*v2z;
        float d2_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
        float d2_2 = pow(v2x, 2) + pow(v2y, 2) + pow(v2z, 2);
        float d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);

        float theta = acos(d1 / d2) * 180 / 3.1415926;
        if (theta<0.1) return 0;
        return theta;
    }
    else
    {
        return 0;
    }
}
float bodyangle::calc_angle4(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2, JointType joint3)
{
    TrackingState joint0state = pJoints[joint0].TrackingState;
    TrackingState joint1state = pJoints[joint1].TrackingState;
    TrackingState joint2state = pJoints[joint2].TrackingState;
    TrackingState joint3state = pJoints[joint3].TrackingState;

    if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked) && (joint2state == TrackingState_Tracked) && (joint3state == TrackingState_Tracked))
    {
        float v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
        float v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
        float v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
        float v2x = pJoints[joint3].Position.X - pJoints[joint2].Position.X;
        float v2y = pJoints[joint3].Position.Y - pJoints[joint2].Position.Y;
        float v2z = pJoints[joint3].Position.Z - pJoints[joint2].Position.Z;

        float d1 = v1x*v2x + v1y*v2y + v1z*v2z;
        float d2_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
        float d2_2 = pow(v2x, 2) + pow(v2y, 2) + pow(v2z, 2);
        float d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);

        float theta = acos(d1 / d2) * 180 / 3.1415926;
        if (theta<0.1) return 0;
        return theta;
    }
    else
    {
        return 0;
    }
}
float bodyangle::calc_dis(const Joint* pJoints, JointType joint0, JointType joint1)
{
    TrackingState joint0state = pJoints[joint0].TrackingState;
    TrackingState joint1state = pJoints[joint1].TrackingState;
    if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked))
    {
        float v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
        float v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
        float v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
        float d1_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
        float d1_2 = pow(d1_1, 0.5);
        if (d1_2<0.0001) return 0;
        return (d1_2*100);
    }
    else
    {
        return 0;
    }
}
