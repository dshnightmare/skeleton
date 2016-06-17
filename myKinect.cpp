#include "myKinect.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <cstring>
#include <math.h>
#include <QMutex>

static QMutex rwimg;

using namespace std;
void thread_imwrite(string s,cv::Mat m)
{
    cv::Mat t1;
    m.copyTo(t1);
    cv::imwrite(s,t1);
}
void thread_rgbimwrite(string s,cv::Mat m)
{
    cv::imwrite(s,m);
}

/// Initializes the default Kinect sensor
HRESULT CBodyBasics::InitializeDefaultSensor(char *folder)
{
	//用于判断每次读取操作的成功与否
	HRESULT hr;
  
	frames = 0;

	strcpy(sav_path, folder);

	printf("%s %s\n", folder, sav_path);

	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	//搜索kinect
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr)){
		return hr;
	}

	//找到kinect设备
	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;//读取骨架
		IDepthFrameSource* pDepthFrameSource = NULL;//读取深度信息
		IColorFrameSource* pColorFrameSource = NULL;
		IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;//读取背景二值图

		//打开kinect
		hr = m_pKinectSensor->Open();
		//coordinatemapper
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		//bodyframe
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		//depth frame
		if (SUCCEEDED(hr)){
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr)){
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}

		//color frame
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}

		//body index frame
		if (SUCCEEDED(hr)){
			hr = m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
		}

		if (SUCCEEDED(hr)){
			hr = pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);
		}

		SafeRelease(pBodyFrameSource);
		SafeRelease(pDepthFrameSource);
		SafeRelease(pColorFrameSource);
		SafeRelease(pBodyIndexFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
//		std::cout << "Kinect initialization failed!" << std::endl;
		return E_FAIL;
	}

	//skeletonImg,用于画骨架、背景二值图的MAT
	skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	skeletonImg.setTo(0);

    colorImg.create(cColorHeight, cColorWidth, CV_8UC3);
	colorImg.setTo(0);

	//depthImg,用于画深度信息的MAT
	depthImg.create(cDepthHeight, cDepthWidth, CV_8UC1);
	depthImg.setTo(0);

	return hr;
}


/// Main processing function
void CBodyBasics::Update()
{
    readok=true;
	//每次先清空skeletonImg
    skeletonImglock.lock();
    skeletonImg.setTo(0);
    skeletonImglock.unlock();

	//如果丢失了kinect，则不继续操作
	if (!m_pBodyFrameReader)
	{
		return;
	}



	IBodyFrame* pBodyFrame = NULL;//骨架信息
	IDepthFrame* pDepthFrame = NULL;//深度信息
	IColorFrame* pColorFrame = NULL;
	IBodyIndexFrame* pBodyIndexFrame = NULL;//背景二值图

	//记录每次操作的成功与否
	HRESULT hr = S_OK;

	//---------------------------------------获取背景二值图并显示---------------------------------
/*
    if (SUCCEEDED(hr)){
        while(!SUCCEEDED(hr = m_pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame)));//获得背景二值图信息
//        if (!SUCCEEDED(hr)) readok=false;
    }

    if (SUCCEEDED(hr)){
        BYTE *bodyIndexArray = new BYTE[cDepthHeight * cDepthWidth];//背景二值图是8为uchar，有人是黑色，没人是白色
        pBodyIndexFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, bodyIndexArray);

        //把背景二值图画到MAT里
        uchar* skeletonData = (uchar*)skeletonImg.data;
        for (int j = 0; j < cDepthHeight * cDepthWidth; ++j){
            *skeletonData = bodyIndexArray[j]; ++skeletonData;
            *skeletonData = bodyIndexArray[j]; ++skeletonData;
            *skeletonData = bodyIndexArray[j]; ++skeletonData;
        }
        delete[] bodyIndexArray;
    }
    SafeRelease(pBodyIndexFrame);//必须要释放，否则之后无法获得新的frame数据
*/
	//-----------------------获取深度数据并显示--------------------------

    if (SUCCEEDED(hr)){
        while(!SUCCEEDED(hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame)));//获得深度数据
//        if (!SUCCEEDED(hr)) readok=false;
	}
    if (SUCCEEDED(hr)){
		UINT16 *depthArray = new UINT16[cDepthHeight * cDepthWidth];//深度数据是16位unsigned int
		pDepthFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, depthArray);

		//把深度数据画到MAT中
        uchar* depthData = (uchar*)depthImg.data;
		for (int j = 0; j < cDepthHeight * cDepthWidth; ++j){
			*depthData = depthArray[j];
			++depthData;
		}
		delete[] depthArray;
    }
    SafeRelease(pDepthFrame);//必须要释放，否则之后无法获得新的frame数据
//    imshow("depthImg", depthImg);
//    char s[100];
//    sprintf(s, "%s\\depth\\%d.png", sav_path, frames);
//    printf("%s\n",s);
//    string s1=s;
//    QtConcurrent::run(thread_imwrite,s1,depthImg);
//    cv::waitKey(5);

	if (!m_pColorFrameReader)
	{
		return;
	}


    while(!SUCCEEDED(hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame)));
//    if (!SUCCEEDED(hr)) readok=false;

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nBufferSize = 0;
		RGBQUAD *pBuffer = NULL;

		hr = pColorFrame->get_RelativeTime(&nTime);

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_FrameDescription(&pFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Width(&nWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFrameDescription->get_Height(&nHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
			}
			else if (m_pColorRGBX)
			{
				pBuffer = m_pColorRGBX;
				nBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);
			}
			else
			{
				hr = E_FAIL;
			}
		}

        if (SUCCEEDED(hr))
        {
            cv::Mat ColorIm(cColorHeight, cColorWidth, CV_8UC4, pBuffer);

//            char s[100];
//            sprintf(s, "%s\\color\\%d.jpg", sav_path, frames);
//            string s1=s;
//            QtConcurrent::run(thread_imwrite,s1,ColorIm);
//            rwimg.lock();
            cv::cvtColor(ColorIm,colorImg,CV_RGBA2RGB);
//            rwimg.unlock();
//            cv::resize(ColorIm, ColorIm, cv::Size(480, 270));
//            imshow("ColorImg", ColorIm);
//            cv::waitKey(5);
        }
        else
        {
            readok=false;
            printf("error color!");
        }
        SafeRelease(pFrameDescription);
    }

    SafeRelease(pColorFrame);




	//-----------------------------获取骨架并显示----------------------------
    if (SUCCEEDED(hr)){
        while(!SUCCEEDED(hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame)));//获取骨架信息
        if (!SUCCEEDED(hr)) readok=false;
	}
	if (SUCCEEDED(hr))
	{
		IBody* ppBodies[BODY_COUNT] = { 0 };//每一个IBody可以追踪一个人，总共可以追踪六个人

		if (SUCCEEDED(hr))
		{
			//把kinect追踪到的人的信息，分别存到每一个IBody中
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			//对每一个IBody，我们找到他的骨架信息，并且画出来
			ProcessBody(frames, BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);//释放所有
		}
	}
    SafeRelease(pBodyFrame);//必须要释放，否则之后无法获得新的frame数据

    if (readok==true)
    {
//        qDebug("%d %d\n", readok ,frames );
//        char s[100];
//        sprintf(s, "%s\\depth\\%d.png", sav_path, frames);
//    //    printf("%s\n",s);
//        string s1=s;
//        QtConcurrent::run(thread_imwrite,s1,depthImg);

//        sprintf(s, "%s\\color\\%d.jpg", sav_path, frames);
//        s1=s;
//        cv::Mat m;
//        cv::resize(colorImg,m,cv::Size(600,337));
////        colorImg.copyTo(m);
//        QtConcurrent::run(thread_rgbimwrite,s1,m);

//        sprintf(s, "%s\\2dskeleton\\%d.jpg", sav_path, frames);
//        s1=s;
//        cv::Mat m1;
//        skeletonImg.copyTo(m1);
//        QtConcurrent::run(thread_rgbimwrite,s1,m1);

        ++frames;
    }

}


/// Handle new body data
void CBodyBasics::ProcessBody(int frames, int nBodyCount, IBody** ppBodies)
{

	//记录操作结果是否成功
	HRESULT hr;

	char s[1000];
	sprintf(s, "%s\\skeleton\\%d.txt", sav_path, frames);
//	printf("%s\n", s);
	++frames;
	FILE *fout = fopen(s, "w");

	//对于每一个IBody
	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
		if (pBody)//还没有搞明白这里pBody和下面的bTracked有什么区别
		{
			BOOLEAN bTracked = false;
			hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				Joint joints[JointType_Count];//存储关节点类
				HandState leftHandState = HandState_Unknown;//左手状态
				HandState rightHandState = HandState_Unknown;//右手状态

				//获取左右手状态
				pBody->get_HandLeftState(&leftHandState);
				pBody->get_HandRightState(&rightHandState);

				//存储深度坐标系中的关节点位置
				DepthSpacePoint *depthSpacePosition = new DepthSpacePoint[_countof(joints)];

				//获得关节点类
				hr = pBody->GetJoints(_countof(joints), joints);
				if (SUCCEEDED(hr))
				{
					for (int j = 0; j < _countof(joints); ++j)
					{
						//将关节点坐标从摄像机坐标系（-1~1）转到深度坐标系（424*512）
						m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);

						fprintf(fout, "%d %f %f %f %d\n", j + 1, joints[j].Position.X, joints[j].Position.Y, joints[j].Position.Z, joints[j].TrackingState);

					}

					//------------------------hand state left-------------------------------
//					DrawHandState(depthSpacePosition[JointType_HandLeft], leftHandState);
//					DrawHandState(depthSpacePosition[JointType_HandRight], rightHandState);

					//---------------------------body-------------------------------
					DrawBone(joints, depthSpacePosition, JointType_Head, JointType_Neck);
					DrawBone(joints, depthSpacePosition, JointType_Neck, JointType_SpineShoulder);
					DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_SpineMid);
					DrawBone(joints, depthSpacePosition, JointType_SpineMid, JointType_SpineBase);
					DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_ShoulderRight);
					DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_ShoulderLeft);
					DrawBone(joints, depthSpacePosition, JointType_SpineBase, JointType_HipRight);
					DrawBone(joints, depthSpacePosition, JointType_SpineBase, JointType_HipLeft);

					// -----------------------Right Arm ------------------------------------ 
					DrawBone(joints, depthSpacePosition, JointType_ShoulderRight, JointType_ElbowRight);
					DrawBone(joints, depthSpacePosition, JointType_ElbowRight, JointType_WristRight);
					DrawBone(joints, depthSpacePosition, JointType_WristRight, JointType_HandRight);
					DrawBone(joints, depthSpacePosition, JointType_HandRight, JointType_HandTipRight);
					DrawBone(joints, depthSpacePosition, JointType_WristRight, JointType_ThumbRight);

					//----------------------------------- Left Arm--------------------------
					DrawBone(joints, depthSpacePosition, JointType_ShoulderLeft, JointType_ElbowLeft);
					DrawBone(joints, depthSpacePosition, JointType_ElbowLeft, JointType_WristLeft);
					DrawBone(joints, depthSpacePosition, JointType_WristLeft, JointType_HandLeft);
					DrawBone(joints, depthSpacePosition, JointType_HandLeft, JointType_HandTipLeft);
					DrawBone(joints, depthSpacePosition, JointType_WristLeft, JointType_ThumbLeft);

					// ----------------------------------Right Leg--------------------------------
					DrawBone(joints, depthSpacePosition, JointType_HipRight, JointType_KneeRight);
					DrawBone(joints, depthSpacePosition, JointType_KneeRight, JointType_AnkleRight);
					DrawBone(joints, depthSpacePosition, JointType_AnkleRight, JointType_FootRight);

					// -----------------------------------Left Leg---------------------------------
					DrawBone(joints, depthSpacePosition, JointType_HipLeft, JointType_KneeLeft);
					DrawBone(joints, depthSpacePosition, JointType_KneeLeft, JointType_AnkleLeft);
					DrawBone(joints, depthSpacePosition, JointType_AnkleLeft, JointType_FootLeft);


					show_angle(joints);

				}
				delete[] depthSpacePosition;
			}
		}
    }
//    char s2[100];
//    sprintf(s2, "%s\\2dskeleton\\%d.jpg", sav_path, frames);
//    string s3=s2;
//    QtConcurrent::run(thread_imwrite,s3,skeletonImg);
//    cv::imshow("skeletonImg", skeletonImg);
//    cv::waitKey(5);

	fclose(fout);

}

void CBodyBasics::show_angle(const Joint* joints)
{

	//膝间距
    rec_angle_and_dis[6] = calc_dis(joints, JointType_KneeRight, JointType_KneeLeft);
    rec_angle_and_dis[7] = rec_angle_and_dis[6];
	//踝间距
    rec_angle_and_dis[8] = calc_dis(joints, JointType_AnkleRight, JointType_AnkleLeft);
    rec_angle_and_dis[9] = rec_angle_and_dis[8];
	//膝关节
    rec_angle_and_dis[0] = calc_angle(joints, JointType_HipLeft, JointType_KneeLeft, JointType_AnkleLeft);
    rec_angle_and_dis[1] = calc_angle(joints, JointType_HipRight, JointType_KneeRight, JointType_AnkleRight);

    rec_angle_and_dis[2] = rec_angle_and_dis[0];
    rec_angle_and_dis[4] = rec_angle_and_dis[0];

    rec_angle_and_dis[3] = rec_angle_and_dis[1];
    rec_angle_and_dis[5] = rec_angle_and_dis[1];

	//髋屈曲
    rec_angle_and_dis[10] = calc_angle4(joints, JointType_KneeLeft, JointType_HipLeft, JointType_SpineMid, JointType_SpineBase);
    rec_angle_and_dis[11] = calc_angle4(joints, JointType_KneeRight, JointType_HipRight, JointType_SpineMid, JointType_SpineBase);

    //髋伸直
    rec_angle_and_dis[13] = calc_angle4(joints, JointType_SpineBase, JointType_SpineMid, JointType_HipRight, JointType_KneeRight);
    rec_angle_and_dis[12] = calc_angle4(joints, JointType_SpineBase, JointType_SpineMid, JointType_HipLeft, JointType_KneeLeft);
	//髋外展
    rec_angle_and_dis[15] = calc_angle_vertical(joints, JointType_HipRight, JointType_KneeRight);
    rec_angle_and_dis[14] = calc_angle_vertical(joints, JointType_HipLeft, JointType_KneeLeft);
	//髋内收
    rec_angle_and_dis[17] = calc_angle4(joints, JointType_SpineBase, JointType_SpineMid, JointType_HipRight, JointType_KneeRight);
    rec_angle_and_dis[16] = calc_angle4(joints, JointType_SpineBase, JointType_SpineMid, JointType_HipLeft, JointType_KneeLeft);
	//髋旋1
    rec_angle_and_dis[19] = calc_angle_vertical(joints, JointType_KneeRight, JointType_AnkleRight);
    rec_angle_and_dis[18] = calc_angle_vertical(joints, JointType_KneeLeft, JointType_AnkleLeft);
	//髋旋2
    rec_angle_and_dis[21] = calc_angle_vertical(joints, JointType_AnkleRight, JointType_KneeRight);
    rec_angle_and_dis[20] = calc_angle_vertical(joints, JointType_AnkleLeft, JointType_KneeLeft);
	
/*{
	char s[100];
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 1);
	IplImage imgTmp = skeletonImg;
	IplImage *img = cvCloneImage(&imgTmp);

//	sprintf(s, "knee angle(r/l): %d, %d", r_knee,l_knee);
//	cvPutText(img, s, CvPoint{ 5, 20 }, &font, cvScalar(255, 0, 0));

//	sprintf(s, "knee distance: %d", knee_distance);
//	cvPutText(img, s, CvPoint{ 5, 40 }, &font, cvScalar(255,0,0));

//	sprintf(s, "ankle distance: %d", ankle_distance);
//	cvPutText(img, s, CvPoint{ 5, 60 }, &font, cvScalar(255, 0,0));

//	sprintf(s, "hip angle1(r/l): %d, %d", r_hip_angle1, l_hip_angle1);
//	cvPutText(img, s, CvPoint{ 5, 80 }, &font, cvScalar(255, 0, 0));
//	sprintf(s, "hip angle2(r/l): %d, %d", r_hip_angle2, l_hip_angle2);
//	cvPutText(img, s, CvPoint{ 5, 100 }, &font, cvScalar(255, 0, 0));
//	sprintf(s, "hip angle3(r/l): %d, %d", r_hip_angle1, l_hip_angle3);
//	cvPutText(img, s, CvPoint{ 5, 120 }, &font, cvScalar(255, 0, 0));
//	sprintf(s, "hip angle4(r/l): %d, %d", r_hip_angle4, l_hip_angle4);
//	cvPutText(img, s, CvPoint{ 5, 140 }, &font, cvScalar(255, 0, 0));
//	sprintf(s, "hip angle5(r/l): %d, %d", r_hip_angle5, l_hip_angle5);
//	cvPutText(img, s, CvPoint{ 5, 160 }, &font, cvScalar(255, 0, 0));
//	sprintf(s, "hip angle6(r/l): %d, %d", r_hip_angle6, l_hip_angle6);
//	cvPutText(img, s, CvPoint{ 5, 180 }, &font, cvScalar(255, 0, 0));


	skeletonImg=cv::Mat(img, true);
    }*/
}
double CBodyBasics::calc_angle_vertical(const Joint* pJoints, JointType joint0, JointType joint1)
{
	TrackingState joint0state = pJoints[joint0].TrackingState;
	TrackingState joint1state = pJoints[joint1].TrackingState;
	if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked))
	{
		double v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
		double v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
		double v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
		double v2x = 0;
		double v2y = 1.0;
		double v2z = 0;
		double d1 = v1x*v2x + v1y*v2y + v1z*v2z;
		double d2_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
		double d2_2 = pow(v2x, 2) + pow(v2y, 2) + pow(v2z, 2);
		double d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);

		double theta = acos(d1 / d2) * 180 / 3.1415926;
        if (theta<0.1) return 0;
        return theta;
	}
	else
	{
		return 0;
	}
}
double CBodyBasics::calc_angle(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2)
{
	TrackingState joint0state = pJoints[joint0].TrackingState;
	TrackingState joint1state = pJoints[joint1].TrackingState;
	TrackingState joint2state = pJoints[joint2].TrackingState;

	if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked) && (joint2state == TrackingState_Tracked))
	{
		double v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
		double v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
		double v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
		double v2x = pJoints[joint2].Position.X - pJoints[joint1].Position.X;
		double v2y = pJoints[joint2].Position.Y - pJoints[joint1].Position.Y;
		double v2z = pJoints[joint2].Position.Z - pJoints[joint1].Position.Z;

		double d1 = v1x*v2x + v1y*v2y + v1z*v2z;
		double d2_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
		double d2_2 = pow(v2x, 2) + pow(v2y, 2) + pow(v2z, 2);
		double d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);

		double theta = acos(d1 / d2) * 180 / 3.1415926;
        if (theta<0.1) return 0;
        return theta;
	}
	else
	{
		return 0;
	}
}
double CBodyBasics::calc_angle4(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2, JointType joint3)
{
	TrackingState joint0state = pJoints[joint0].TrackingState;
	TrackingState joint1state = pJoints[joint1].TrackingState;
	TrackingState joint2state = pJoints[joint2].TrackingState;
	TrackingState joint3state = pJoints[joint3].TrackingState;

	if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked) && (joint2state == TrackingState_Tracked) && (joint3state == TrackingState_Tracked))
	{
		double v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
		double v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
		double v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
		double v2x = pJoints[joint3].Position.X - pJoints[joint2].Position.X;
		double v2y = pJoints[joint3].Position.Y - pJoints[joint2].Position.Y;
		double v2z = pJoints[joint3].Position.Z - pJoints[joint2].Position.Z;

		double d1 = v1x*v2x + v1y*v2y + v1z*v2z;
		double d2_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
		double d2_2 = pow(v2x, 2) + pow(v2y, 2) + pow(v2z, 2);
		double d2 = pow(d2_1, 0.5)*pow(d2_2, 0.5);

		double theta = acos(d1 / d2) * 180 / 3.1415926;
        if (theta<0.1) return 0;
        return theta;
	}
	else
	{
		return 0;
	}
}
double CBodyBasics::calc_dis(const Joint* pJoints, JointType joint0, JointType joint1)
{
	TrackingState joint0state = pJoints[joint0].TrackingState;
	TrackingState joint1state = pJoints[joint1].TrackingState;
	if ((joint0state == TrackingState_Tracked) && (joint1state == TrackingState_Tracked))
	{
		double v1x = pJoints[joint0].Position.X - pJoints[joint1].Position.X;
		double v1y = pJoints[joint0].Position.Y - pJoints[joint1].Position.Y;
		double v1z = pJoints[joint0].Position.Z - pJoints[joint1].Position.Z;
		double d1_1 = pow(v1x, 2) + pow(v1y, 2) + pow(v1z, 2);
		double d1_2 = pow(d1_1, 0.5);
        if (d1_2<0.0001) return 0;
        return (d1_2*100);
	}
	else
	{
		return 0;
	}
}
//画手的状态
void CBodyBasics::DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState)
{
	//给不同的手势分配不同颜色
	CvScalar color;
	switch (handState){
	case HandState_Open:
		color = cvScalar(255, 0, 0);
		break;
	case HandState_Closed:
		color = cvScalar(0, 255, 0);
		break;
	case HandState_Lasso:
		color = cvScalar(0, 0, 255);
		break;
	default://如果没有确定的手势，就不要画
		return;
	}

	circle(skeletonImg,
		cvPoint(depthSpacePosition.X, depthSpacePosition.Y),
		20, color, -1);
}


/// Draws one bone of a body (joint to joint)
void CBodyBasics::DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
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

    if (joint0State == TrackingState_Tracked)
        circle(skeletonImg, p2,3,cvScalar(0, 255, 0), -1);
    else
        circle(skeletonImg, p2,3,cvScalar(0, 0, 255), -1);

}


/// Constructor
CBodyBasics::CBodyBasics() :
m_pKinectSensor(NULL),
m_pCoordinateMapper(NULL),
m_pBodyFrameReader(NULL){}

/// Destructor
CBodyBasics::~CBodyBasics()
{
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);

	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
	SafeRelease(m_pKinectSensor);
}
