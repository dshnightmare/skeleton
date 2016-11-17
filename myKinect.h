#pragma once
#include <Kinect.h>
#include <opencv2\opencv.hpp>
#include <bodyangle.h>
#include "QtConcurrent/QtConcurrent"
#include "cstring"
#include "QMutex"
#include "bodyangle.h"
#include "fstream"


// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class CBodyBasics
{

	//kinect 2.0 的深度空间的高*宽是 424 * 512，在官网上有说明
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

	static const int       cColorWidth  = 1920;
    static const int        cColorHeight = 1080;

public:
	CBodyBasics();
	~CBodyBasics();
	void                    Update();//获得骨架、背景二值图和深度信息
	HRESULT                 InitializeDefaultSensor(char *);//用于初始化kinect
    //显示图像的Mat
    cv::Mat skeletonImg;
    cv::Mat depthImg;
    cv::Mat colorImg;
    float rec_angle_and_dis[28];
    bodyangle recjoint;
    QMutex visitimg;
    QMutex skeletonImglock;
    bool readok;
    std::ofstream rwsave;
    std::ifstream rwload;

    Joint knee1,knee2;
    void setoffset(cv::Point3f leftdiv, cv::Point3f rightdiv, bool flag);

private:
	int frames;
//    void thread_imwrite(std::string s,cv::Mat m);
	int r_elbow_angle;
	int r_knee_angle;
	int r_ankle_angle;

    cv::Point3f lhip;
    cv::Point3f rhip;
    cv::Point3f lhdiv;
    cv::Point3f rhdiv;

    bool relocate_flag;

	char sav_path[1000];
	IKinectSensor*          m_pKinectSensor;//kinect源
	ICoordinateMapper*      m_pCoordinateMapper;//用于坐标变换
	IBodyFrameReader*       m_pBodyFrameReader;//用于骨架数据读取
	IDepthFrameReader*      m_pDepthFrameReader;//用于深度数据读取
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader;//用于背景二值图读取
	 IColorFrameReader*      m_pColorFrameReader;

	  RGBQUAD*                m_pColorRGBX;


	//通过获得到的信息，把骨架和背景二值图画出来
	void                    ProcessBody(int frames, int nBodyCount, IBody** ppBodies);
	//画骨架函数
	void DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//画手的状态函数
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);


};

