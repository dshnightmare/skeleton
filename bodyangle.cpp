#include "bodyangle.h"

static cv::Mat skeletonImg;

bodyangle::bodyangle()
{
    initialize();
}

void bodyangle::initialize()
{
    for (int i=0;i<JointType_Count;i++)
    {
        tjoint_coordinate_3d.plot3d[i][0]=0;
        tjoint_coordinate_3d.plot3d[i][1]=0;
        tjoint_coordinate_3d.plot3d[i][2]=0;
        tjoint_coordinate_2d.plot2d[i][0]=0;
        tjoint_coordinate_2d.plot2d[i][1]=0;
        tjoint_coordinate_3d.status[i]=TrackingState_NotTracked;
    }
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
    Joint planar1,planar2,planarn,plannardis,vec;
    float tt;
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

    //Ï¥ÇüÇú
    angles[0] = 180-calc_angle3(joint_coordinate_3d, JointType_HipLeft, JointType_KneeLeft, JointType_AnkleLeft);
    angles[1] = 180-calc_angle3(joint_coordinate_3d, JointType_HipRight, JointType_KneeRight, JointType_AnkleRight);

    //Ï¥ÉìÖ±
    angles[2] = angles[0];
    angles[3] = angles[1];

    //Ï¥¹ÉëÖ½Ç
    angles[4] = angles[0];
    angles[5] = angles[1];

    float tmax = joint_coordinate_3d[JointType_KneeLeft].Position.X>joint_coordinate_3d[JointType_KneeRight].Position.X?
                    joint_coordinate_3d[JointType_KneeLeft].Position.X:joint_coordinate_3d[JointType_KneeRight].Position.X;
    float tmin = joint_coordinate_3d[JointType_KneeLeft].Position.X+joint_coordinate_3d[JointType_KneeRight].Position.X - tmax;

    if (!( ( joint_coordinate_3d[JointType_HipLeft].Position.X > tmin && joint_coordinate_3d[JointType_HipLeft].Position.X < tmax) &&
           ( joint_coordinate_3d[JointType_AnkleLeft].Position.X > tmin && joint_coordinate_3d[JointType_AnkleLeft].Position.X < tmax ) &&
           ( joint_coordinate_3d[JointType_HipRight].Position.X > tmin && joint_coordinate_3d[JointType_HipRight].Position.X < tmax) &&
           ( joint_coordinate_3d[JointType_AnkleRight].Position.X > tmin && joint_coordinate_3d[JointType_AnkleRight].Position.X < tmax ) ))
    {
        angles[4] = -angles[4];
        angles[5] = -angles[5];
    }


    //Ï¥¼ä¾à
    angles[6] = calc_dis(joint_coordinate_3d, JointType_KneeRight, JointType_KneeLeft);
    angles[7] = angles[6];

    //õ×¼ä¾à
    angles[8] = calc_dis(joint_coordinate_3d, JointType_AnkleRight, JointType_AnkleLeft);
    angles[9] = angles[8];

    //÷ÅÇüÇú
    angles[10] = 180-calc_angle_vertical(joint_coordinate_3d, JointType_KneeLeft, JointType_HipLeft);
    angles[11] = 180-calc_angle_vertical(joint_coordinate_3d, JointType_KneeRight, JointType_HipRight);

    //÷ÅÉìÖ±
    angles[12] = -angles[10];
    angles[13] = -angles[11];

    //÷ÅÍâÕ¹
    angles[14] = calc_angle_vertical(joint_coordinate_3d, JointType_HipLeft, JointType_KneeLeft);
    angles[15] = calc_angle_vertical(joint_coordinate_3d, JointType_HipRight, JointType_KneeRight);

    //÷ÅÄÚÊÕ

    {
        planar2.Position.X=joint_coordinate_3d[JointType_SpineBase].Position.X-joint_coordinate_3d[JointType_HipRight].Position.X;
        planar2.Position.Y=joint_coordinate_3d[JointType_SpineBase].Position.Y-joint_coordinate_3d[JointType_HipRight].Position.Y;
        planar2.Position.Z=joint_coordinate_3d[JointType_SpineBase].Position.Z-joint_coordinate_3d[JointType_HipRight].Position.Z;
        planar1.Position.X=joint_coordinate_3d[JointType_KneeRight].Position.X-joint_coordinate_3d[JointType_HipRight].Position.X;
        planar1.Position.Y=joint_coordinate_3d[JointType_KneeRight].Position.Y-joint_coordinate_3d[JointType_HipRight].Position.Y;
        planar1.Position.Z=joint_coordinate_3d[JointType_KneeRight].Position.Z-joint_coordinate_3d[JointType_HipRight].Position.Z;
        vec.Position.X = joint_coordinate_3d[JointType_HipLeft].Position.X-joint_coordinate_3d[JointType_KneeLeft].Position.X;
        vec.Position.Y = joint_coordinate_3d[JointType_HipLeft].Position.Y-joint_coordinate_3d[JointType_KneeLeft].Position.Y;
        vec.Position.Z = joint_coordinate_3d[JointType_HipLeft].Position.Z-joint_coordinate_3d[JointType_KneeLeft].Position.Z;


        planarn.Position.X=planar1.Position.Y*planar2.Position.Z-planar1.Position.Z*planar2.Position.Y;
        planarn.Position.Y=planar1.Position.Z*planar2.Position.X-planar1.Position.X*planar2.Position.Z;
        planarn.Position.Z=planar1.Position.X*planar2.Position.Y-planar1.Position.Y*planar2.Position.X;

        tt = pow( pow(planarn.Position.X,2.0) +  pow(planarn.Position.Y,2.0) + pow(planarn.Position.Z,2.0) ,0.5);
        planarn.Position.X/=tt;planarn.Position.Y/=tt;planarn.Position.Z/=tt;
//        qDebug("%f %f %f %f\n",tt,planarn.Position.X,planarn.Position.Y,planarn.Position.Z);

        tt = planarn.Position.X*vec.Position.X+planarn.Position.Y*vec.Position.Y+planarn.Position.Z*vec.Position.Z;
//        qDebug("%f %f %f %f\n",tt,planarn.Position.X,planarn.Position.Y,planarn.Position.Z);
//        qDebug("%f %f %f %f\n",tt,vec.Position.X,vec.Position.Y,vec.Position.Z);

        plannardis.Position.X=planarn.Position.X*tt;plannardis.Position.Y=planarn.Position.Y*tt;plannardis.Position.Z=planarn.Position.Z*tt;

        vec.TrackingState=TrackingState_NotTracked;
        plannardis.TrackingState=TrackingState_NotTracked;
        if ( (joint_coordinate_3d[JointType_SpineBase].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_HipRight].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_KneeRight].TrackingState!=TrackingState_NotTracked))
        {
             plannardis.TrackingState=TrackingState_Tracked;
        }
        if ( (joint_coordinate_3d[JointType_HipLeft].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_KneeLeft].TrackingState!=TrackingState_NotTracked))
        {
             vec.TrackingState=TrackingState_Tracked;
        }


        angles[16] = calc_angle_verticalt(vec, plannardis);
    }

    {
        planar1.Position.X=joint_coordinate_3d[JointType_SpineBase].Position.X-joint_coordinate_3d[JointType_HipLeft].Position.X;
        planar1.Position.Y=joint_coordinate_3d[JointType_SpineBase].Position.Y-joint_coordinate_3d[JointType_HipLeft].Position.Y;
        planar1.Position.Z=joint_coordinate_3d[JointType_SpineBase].Position.Z-joint_coordinate_3d[JointType_HipLeft].Position.Z;
        planar2.Position.X=joint_coordinate_3d[JointType_KneeLeft].Position.X-joint_coordinate_3d[JointType_HipLeft].Position.X;
        planar2.Position.Y=joint_coordinate_3d[JointType_KneeLeft].Position.Y-joint_coordinate_3d[JointType_HipLeft].Position.Y;
        planar2.Position.Z=joint_coordinate_3d[JointType_KneeLeft].Position.Z-joint_coordinate_3d[JointType_HipLeft].Position.Z;
        vec.Position.X = joint_coordinate_3d[JointType_HipRight].Position.X-joint_coordinate_3d[JointType_KneeRight].Position.X;
        vec.Position.Y = joint_coordinate_3d[JointType_HipRight].Position.Y-joint_coordinate_3d[JointType_KneeRight].Position.Y;
        vec.Position.Z = joint_coordinate_3d[JointType_HipRight].Position.Z-joint_coordinate_3d[JointType_KneeRight].Position.Z;


        planarn.Position.X=planar1.Position.Y*planar2.Position.Z-planar1.Position.Z*planar2.Position.Y;
        planarn.Position.Y=planar1.Position.Z*planar2.Position.X-planar1.Position.X*planar2.Position.Z;
        planarn.Position.Z=planar1.Position.X*planar2.Position.Y-planar1.Position.Y*planar2.Position.X;

        tt = pow( pow(planarn.Position.X,2.0) +  pow(planarn.Position.Y,2.0) + pow(planarn.Position.Z,2.0) ,0.5);
        planarn.Position.X/=tt;planarn.Position.Y/=tt;planarn.Position.Z/=tt;

        tt = planarn.Position.X*vec.Position.X+planarn.Position.Y*vec.Position.Y+planarn.Position.Z*vec.Position.Z;
        plannardis.Position.X=planarn.Position.X*tt;plannardis.Position.Y=planarn.Position.Y*tt;plannardis.Position.Z=planarn.Position.Z*tt;

        vec.TrackingState=TrackingState_NotTracked;
        plannardis.TrackingState=TrackingState_NotTracked;
        if ( (joint_coordinate_3d[JointType_SpineBase].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_HipLeft].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_KneeLeft].TrackingState!=TrackingState_NotTracked))
        {
             plannardis.TrackingState=TrackingState_Tracked;
        }
        if ( (joint_coordinate_3d[JointType_HipRight].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_KneeRight].TrackingState!=TrackingState_NotTracked))
        {
             vec.TrackingState=TrackingState_Tracked;
        }
        angles[17] = calc_angle_verticalt(vec, plannardis);
    }

    //÷ÅÐý1

    if ( (joint_coordinate_3d[JointType_AnkleLeft].Position.X-joint_coordinate_3d[JointType_KneeLeft].Position.X)
         *(joint_coordinate_3d[JointType_KneeLeft].Position.X-joint_coordinate_3d[JointType_KneeRight].Position.X)>0)
    {
        angles[18] = calc_angle_vertical(joint_coordinate_3d, JointType_KneeLeft, JointType_AnkleLeft);
    }
    else
    {
        angles[18] = -calc_angle_vertical(joint_coordinate_3d, JointType_KneeLeft, JointType_AnkleLeft);
    }

    if ( (joint_coordinate_3d[JointType_AnkleRight].Position.X-joint_coordinate_3d[JointType_KneeRight].Position.X)
         *(joint_coordinate_3d[JointType_KneeRight].Position.X-joint_coordinate_3d[JointType_KneeLeft].Position.X)>0)
    {
        angles[19] = calc_angle_vertical(joint_coordinate_3d, JointType_KneeRight, JointType_AnkleRight);
    }
    else
    {
        angles[19] = -calc_angle_vertical(joint_coordinate_3d, JointType_KneeRight, JointType_AnkleRight);
    }


    //÷ÅÐý2
    angles[20] = -angles[18];
    angles[21] = -angles[19];

    //ÖâÇüÇú
    angles[22] = calc_angle3(joint_coordinate_3d, JointType_WristLeft, JointType_ElbowLeft, JointType_ShoulderLeft);
    angles[23] = calc_angle3(joint_coordinate_3d, JointType_WristRight, JointType_ElbowRight, JointType_ShoulderRight);

    //¼çÍâÕ¹
    angles[24] = calc_angle4(joint_coordinate_3d, JointType_SpineBase,JointType_SpineMid,JointType_ShoulderLeft,JointType_ElbowLeft);
    angles[25] = calc_angle4(joint_coordinate_3d, JointType_SpineBase,JointType_SpineMid,JointType_ShoulderRight,JointType_ElbowRight);

    //¼çºóÉì

    {
        planar1.Position.X=joint_coordinate_3d[JointType_SpineMid].Position.X-joint_coordinate_3d[JointType_SpineShoulder].Position.X;
        planar1.Position.Y=joint_coordinate_3d[JointType_SpineMid].Position.Y-joint_coordinate_3d[JointType_SpineShoulder].Position.Y;
        planar1.Position.Z=joint_coordinate_3d[JointType_SpineMid].Position.Z-joint_coordinate_3d[JointType_SpineShoulder].Position.Z;
        planar2.Position.X=joint_coordinate_3d[JointType_ShoulderLeft].Position.X-joint_coordinate_3d[JointType_SpineShoulder].Position.X;
        planar2.Position.Y=joint_coordinate_3d[JointType_ShoulderLeft].Position.Y-joint_coordinate_3d[JointType_SpineShoulder].Position.Y;
        planar2.Position.Z=joint_coordinate_3d[JointType_ShoulderLeft].Position.Z-joint_coordinate_3d[JointType_SpineShoulder].Position.Z;
        vec.Position.X = joint_coordinate_3d[JointType_ElbowLeft].Position.X-joint_coordinate_3d[JointType_ShoulderLeft].Position.X;
        vec.Position.Y = joint_coordinate_3d[JointType_ElbowLeft].Position.Y-joint_coordinate_3d[JointType_ShoulderLeft].Position.Y;
        vec.Position.Z = joint_coordinate_3d[JointType_ElbowLeft].Position.Z-joint_coordinate_3d[JointType_ShoulderLeft].Position.Z;


        planarn.Position.X=planar1.Position.Y*planar2.Position.Z-planar1.Position.Z*planar2.Position.Y;
        planarn.Position.Y=planar1.Position.Z*planar2.Position.X-planar1.Position.X*planar2.Position.Z;
        planarn.Position.Z=planar1.Position.X*planar2.Position.Y-planar1.Position.Y*planar2.Position.X;

        vec.TrackingState=TrackingState_NotTracked;
        planarn.TrackingState=TrackingState_NotTracked;
        if ( (joint_coordinate_3d[JointType_SpineMid].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_SpineShoulder].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_ShoulderLeft].TrackingState!=TrackingState_NotTracked))
        {
             planarn.TrackingState=TrackingState_Tracked;
        }
        if ( (joint_coordinate_3d[JointType_ElbowLeft].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_ShoulderLeft].TrackingState!=TrackingState_NotTracked))
        {
             vec.TrackingState=TrackingState_Tracked;
        }
        if (planarn.TrackingState!=TrackingState_NotTracked && vec.TrackingState!=TrackingState_NotTracked)
        {
            float d1 = vec.Position.X*planarn.Position.X+vec.Position.Y*planarn.Position.Y+vec.Position.Z*planarn.Position.Z;
            float d2_1 = pow(vec.Position.X,2)+pow(vec.Position.Y,2)+pow(vec.Position.Z,2);
            float d2_2 = pow(planarn.Position.X,2)+pow(planarn.Position.Y,2)+pow(planarn.Position.Z,2);
            float d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);
            angles[26] = acos(d1 / d2) * 180 / 3.1415926;
        }
        else
        {
            angles[26]=0;
        }
    }

    {
        planar2.Position.X=joint_coordinate_3d[JointType_SpineMid].Position.X-joint_coordinate_3d[JointType_SpineShoulder].Position.X;
        planar2.Position.Y=joint_coordinate_3d[JointType_SpineMid].Position.Y-joint_coordinate_3d[JointType_SpineShoulder].Position.Y;
        planar2.Position.Z=joint_coordinate_3d[JointType_SpineMid].Position.Z-joint_coordinate_3d[JointType_SpineShoulder].Position.Z;
        planar1.Position.X=joint_coordinate_3d[JointType_ShoulderRight].Position.X-joint_coordinate_3d[JointType_SpineShoulder].Position.X;
        planar1.Position.Y=joint_coordinate_3d[JointType_ShoulderRight].Position.Y-joint_coordinate_3d[JointType_SpineShoulder].Position.Y;
        planar1.Position.Z=joint_coordinate_3d[JointType_ShoulderRight].Position.Z-joint_coordinate_3d[JointType_SpineShoulder].Position.Z;
        vec.Position.X = joint_coordinate_3d[JointType_ElbowRight].Position.X-joint_coordinate_3d[JointType_ShoulderRight].Position.X;
        vec.Position.Y = joint_coordinate_3d[JointType_ElbowRight].Position.Y-joint_coordinate_3d[JointType_ShoulderRight].Position.Y;
        vec.Position.Z = joint_coordinate_3d[JointType_ElbowRight].Position.Z-joint_coordinate_3d[JointType_ShoulderRight].Position.Z;


        planarn.Position.X=planar1.Position.Y*planar2.Position.Z-planar1.Position.Z*planar2.Position.Y;
        planarn.Position.Y=planar1.Position.Z*planar2.Position.X-planar1.Position.X*planar2.Position.Z;
        planarn.Position.Z=planar1.Position.X*planar2.Position.Y-planar1.Position.Y*planar2.Position.X;

        vec.TrackingState=TrackingState_NotTracked;
        planarn.TrackingState=TrackingState_NotTracked;
        if ( (joint_coordinate_3d[JointType_SpineMid].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_SpineShoulder].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_ShoulderRight].TrackingState!=TrackingState_NotTracked))
        {
             planarn.TrackingState=TrackingState_Tracked;
        }
        if ( (joint_coordinate_3d[JointType_ElbowRight].TrackingState!=TrackingState_NotTracked) &&
              (joint_coordinate_3d[JointType_ShoulderRight].TrackingState!=TrackingState_NotTracked))
        {
             vec.TrackingState=TrackingState_Tracked;
        }
        if (planarn.TrackingState!=TrackingState_NotTracked && vec.TrackingState!=TrackingState_NotTracked)
        {
            float d1 = vec.Position.X*planarn.Position.X+vec.Position.Y*planarn.Position.Y+vec.Position.Z*planarn.Position.Z;
            float d2_1 = pow(vec.Position.X,2)+pow(vec.Position.Y,2)+pow(vec.Position.Z,2);
            float d2_2 = pow(planarn.Position.X,2)+pow(planarn.Position.Y,2)+pow(planarn.Position.Z,2);
            float d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);
            angles[27] = acos(d1 / d2) * 180 / 3.1415926;
        }
        else
        {
            angles[27]=0;
        }
    }

}

float bodyangle::calc_angle_verticalt(Joint joint0, Joint joint1)
{
    TrackingState joint0state = joint0.TrackingState;
    TrackingState joint1state = joint1.TrackingState;
    if ((joint0state != TrackingState_NotTracked) && (joint1state != TrackingState_NotTracked))
    {
        float v1x = joint0.Position.X - joint1.Position.X;
        float v1y = joint0.Position.Y - joint1.Position.Y;
        float v1z = joint0.Position.Z - joint1.Position.Z;
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

float bodyangle::calc_angle_vertical(const Joint* pJoints, JointType joint0, JointType joint1)
{
    TrackingState joint0state = pJoints[joint0].TrackingState;
    TrackingState joint1state = pJoints[joint1].TrackingState;
    if ((joint0state != TrackingState_NotTracked) && (joint1state != TrackingState_NotTracked))
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

    if ((joint0state != TrackingState_NotTracked) && (joint1state != TrackingState_NotTracked) && (joint2state != TrackingState_NotTracked))
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

    if ((joint0state != TrackingState_NotTracked) && (joint1state != TrackingState_NotTracked) && (joint2state != TrackingState_NotTracked) && (joint3state != TrackingState_NotTracked))
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
    if ((joint0state != TrackingState_NotTracked) && (joint1state != TrackingState_NotTracked))
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
