#pragma once
#include <Kinect.h>
#include <opencv2\opencv.hpp>
#include "QtConcurrent/QtConcurrent"
#include "cstring"
#include "QMutex"


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

	//kinect 2.0 ����ȿռ�ĸ�*���� 424 * 512���ڹ�������˵��
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

	static const int       cColorWidth  = 1920;
    static const int        cColorHeight = 1080;

public:
	CBodyBasics();
	~CBodyBasics();
	void                    Update();//��ùǼܡ�������ֵͼ�������Ϣ
	HRESULT                 InitializeDefaultSensor(char *);//���ڳ�ʼ��kinect
    //��ʾͼ���Mat
    cv::Mat skeletonImg;
    cv::Mat depthImg;
    cv::Mat colorImg;
    double rec_angle_and_dis[22];
    QMutex visitimg;
    QMutex skeletonImglock;
    bool readok;

private:
	int frames;
//    void thread_imwrite(std::string s,cv::Mat m);
	int r_elbow_angle;
	int r_knee_angle;
	int r_ankle_angle;

	char sav_path[1000];
	IKinectSensor*          m_pKinectSensor;//kinectԴ
	ICoordinateMapper*      m_pCoordinateMapper;//��������任
	IBodyFrameReader*       m_pBodyFrameReader;//���ڹǼ����ݶ�ȡ
	IDepthFrameReader*      m_pDepthFrameReader;//����������ݶ�ȡ
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader;//���ڱ�����ֵͼ��ȡ
	 IColorFrameReader*      m_pColorFrameReader;

	  RGBQUAD*                m_pColorRGBX;
    double calc_angle(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2);
    double calc_angle4(const Joint* pJoints, JointType joint0, JointType joint1, JointType joint2, JointType joint3);
    double calc_angle_vertical(const Joint* pJoints, JointType joint0, JointType joint1);
    double calc_dis(const Joint* pJoints, JointType joint0, JointType joint1);
	void show_angle(const Joint* pJoints);

	//ͨ����õ�����Ϣ���ѹǼܺͱ�����ֵͼ������
	void                    ProcessBody(int frames, int nBodyCount, IBody** ppBodies);
	//���Ǽܺ���
	void DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//���ֵ�״̬����
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);


};

