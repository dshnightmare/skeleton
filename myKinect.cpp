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

    recjoint.initialize();

	//skeletonImg,用于画骨架、背景二值图的MAT
	skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	skeletonImg.setTo(0);

    colorImg.create(cColorHeight, cColorWidth, CV_8UC3);
	colorImg.setTo(0);

    char s[1000];
    sprintf(s, "%s\\skeleton\\save", sav_path);
    rwsave.open(s);
    rwload.open(s);



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
            cv::cvtColor(ColorIm,colorImg,CV_RGBA2RGB);
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
        char s[100];
        sprintf(s, "%s\\color\\%d.jpg", sav_path, frames);
        string s1=s;
        cv::Mat m;
        cv::resize(colorImg,m,cv::Size(600,337));
//        colorImg.copyTo(m);
        QtConcurrent::run(thread_rgbimwrite,s1,m);

        sprintf(s, "%s\\2dskeleton\\%d.jpg", sav_path, frames);
        s1=s;
        cv::Mat m1;
        skeletonImg.copyTo(m1);
        QtConcurrent::run(thread_rgbimwrite,s1,m1);

        ++frames;
    }

}


/// Handle new body data
void CBodyBasics::ProcessBody(int frames, int nBodyCount, IBody** ppBodies)
{

	//记录操作结果是否成功
    HRESULT hr;
    int ptr=-1;
    float recdis = 100;
    //对于每一个IBody
	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
        if (pBody)
		{
			BOOLEAN bTracked = false;
			hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				Joint joints[JointType_Count];//存储关节点类

				//获得关节点类
				hr = pBody->GetJoints(_countof(joints), joints);
				if (SUCCEEDED(hr))
				{
                    float dis = joints[JointType_Head].Position.X;
                    if (dis<recdis)
                    {
                        recdis=dis;
                        ptr=i;
                    }
                }
            }
        }
    }
    if (ptr!=-1)
    {qDebug("s\n");
        IBody* pBody = ppBodies[ptr];
        Joint joints[JointType_Count];
        hr = pBody->GetJoints(_countof(joints), joints);
        joints[JointType_SpineShoulder].Position.Y=0.5*(joints[JointType_ShoulderLeft].Position.Y+joints[JointType_ShoulderRight].Position.Y);
        DepthSpacePoint *depthSpacePosition = new DepthSpacePoint[_countof(joints)];
        for (int j = 0; j < _countof(joints); ++j)
        {
            //将关节点坐标从摄像机坐标系（-1~1）转到深度坐标系（424*512）
            m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
        }
        recjoint.frame=frames;
        recjoint.update(depthSpacePosition,joints);
        recjoint.calcangles();
        {
            boost::archive::text_oarchive oa(rwsave);
            oa << recjoint;
        }
        skeletonImg=recjoint.draw();
        for (int i=0;i<22;i++)
        {
            rec_angle_and_dis[i]=recjoint.angles[i];
        }

//        bodyangle temp;
//        {
//            boost::archive::text_iarchive ia(rwload);
//            ia >> temp;
//        }

//        skeletonImg=temp.draw();

//        for (int i=0;i<22;i++)
//        {
//            rec_angle_and_dis[i]=temp.angles[i];
//        }

        delete[] depthSpacePosition;

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
    rwsave.close();
    rwload.close();
    SafeRelease(m_pBodyFrameReader);
    SafeRelease(m_pCoordinateMapper);

	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
	SafeRelease(m_pKinectSensor);
}
