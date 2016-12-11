#include "anglemanager.h"

static double showangleplot[28][30];
static QPen* pens[22];
static QString nameset[28]={QString::fromLocal8Bit("左膝屈曲") , QString::fromLocal8Bit("右膝屈曲") , QString::fromLocal8Bit("左膝伸直") , QString::fromLocal8Bit("右膝伸直") , QString::fromLocal8Bit("左膝股胫角") ,
                    QString::fromLocal8Bit("右膝股胫角") , QString::fromLocal8Bit("左膝间距") , QString::fromLocal8Bit("右膝间距") , QString::fromLocal8Bit("左踝间距") , QString::fromLocal8Bit("右踝间距") ,
                    QString::fromLocal8Bit("左髋屈曲") , QString::fromLocal8Bit("右髋屈曲") , QString::fromLocal8Bit("左髋伸直") , QString::fromLocal8Bit("右髋伸直") , QString::fromLocal8Bit("左髋外展") ,
                    QString::fromLocal8Bit("右髋外展") , QString::fromLocal8Bit("左髋内收") , QString::fromLocal8Bit("右髋内收") , QString::fromLocal8Bit("左髋内旋") , QString::fromLocal8Bit("右髋内旋") ,
                    QString::fromLocal8Bit("左髋外旋") , QString::fromLocal8Bit("右髋外旋"), QString::fromLocal8Bit("左肘屈曲"), QString::fromLocal8Bit("右肘屈曲"), QString::fromLocal8Bit("左肩外展"),
                    QString::fromLocal8Bit("右肩外展"), QString::fromLocal8Bit("左肩后伸"), QString::fromLocal8Bit("右肩后伸")};


AngleManager::AngleManager(QwtPlot *p)
{
    plot = p;
    for(int i = 0; i < 30; i++)
        xplot[i] = i;
    clear();
    makepen();
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setPalette(Qt::white);
    //canvas->setBorderRadius(10);
    plot->setCanvas(canvas);
    plot->setAxisScale(QwtPlot::xBottom, 0, 30);
    plot->setAxisScale(QwtPlot::yLeft, -100, 300);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->setMajorPen(Qt::black, 0, Qt::DotLine);//大格子
    grid->setMinorPen(Qt::gray, 0, Qt::DotLine);//大格子里的小格子
    grid->attach(plot);
}
AngleManager::~AngleManager(){

}

void AngleManager::clear(){
    for (int i=0;i<28;i++)
    {
        for (int j=0;j<30;j++)
        {
            showangleplot[i][j] = 0;
        }
        values[i].clear();
        //默认稳定状态初始化为0
        last_stable_angles[i] = 0;
        //默认最大值初始化为-360
        max_angles[i] = -360;
        //默认最小值初始话为360
        min_angles[i] = 360;
    }
    limpDiff = 0;
    limbDiff = 0;
    //check.clear();
    Time_cycle = 0;
    plot->replot();
}

void AngleManager::makepen()
{
    //
    pens[0] = new QPen(QColor(255,0,0),2,Qt::SolidLine);
    pens[1] = new QPen(QColor(0,255,255),2,Qt::SolidLine);
    pens[2] = new QPen(QColor(0,255,0),2,Qt::SolidLine);
    pens[3] = new QPen(QColor(255,0,255),2,Qt::SolidLine);
    pens[4] = new QPen(QColor(0,0,255),2,Qt::SolidLine);
    pens[5] = new QPen(QColor(255,255,0),2,Qt::SolidLine);
    pens[6] = new QPen(QColor(255,128,0),2,Qt::SolidLine);
    pens[7] = new QPen(QColor(0,128,255),2,Qt::SolidLine);

    pens[8] = new QPen(QColor(255,0,128),2,Qt::SolidLine);
    pens[9] = new QPen(QColor(0,255,128),2,Qt::SolidLine);

    pens[10] = new QPen(QColor(255,0,0),2,Qt::DotLine);
    pens[11] = new QPen(QColor(0,255,255),2,Qt::DotLine);
    pens[12] = new QPen(QColor(0,255,0),2,Qt::DotLine);
    pens[13] = new QPen(QColor(255,0,255),2,Qt::DotLine);
    pens[14] = new QPen(QColor(0,0,255),2,Qt::DotLine);
    pens[15] = new QPen(QColor(255,255,0),2,Qt::DotLine);
    pens[16] = new QPen(QColor(255,128,0),2,Qt::DotLine);
    pens[17] = new QPen(QColor(0,128,255),2,Qt::DotLine);
    pens[18] = new QPen(QColor(255,0,128),2,Qt::DotLine);
    pens[19] = new QPen(QColor(0,255,128),2,Qt::DotLine);
    pens[20] = new QPen(QColor(128,0,255),2,Qt::DotLine);
    pens[21] = new QPen(QColor(128,255,0),2,Qt::DotLine);
}

int AngleManager::fromCategoryToIndex(int i, int j, int k){
    int r = i;
    if(j == 1)
        r += 8;
    else if(j == 2)
        r += 10;
    else if(j==3)
        r += 22;
    else if(j==4)
        r+=24;
    r += k * 2;
    return r;
}

void AngleManager::refresh_angle(float *angles){
    Time_cycle++;
    Time_cycle %= 15;

    if(Time_cycle == 0)
        plot->detachItems();

    for (auto i = check.begin(); i != check.end(); i++){
        refresh_one_angle(*i, angles[*i]);
    }

    if (Time_cycle == 0){
        QwtPlotGrid *grid = new QwtPlotGrid();
        grid->enableXMin(true);
        grid->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));//大格子
        grid->setMinorPen(QPen(Qt::darkGray, 0 , Qt::DotLine));//大格子里的小格子
        grid->attach(plot);
        plot->replot();
    }
}

void AngleManager::setcheck(int i, int j, int k, bool flag){
    int index = fromCategoryToIndex(i, j, k);
    if(flag)
        check.insert(index);
    else{
        check.erase(index);
        for (int j=0;j<30;j++)
        {
            showangleplot[index][j] = 0;
        }
        values[index].clear();
    }
}

void AngleManager::refresh_one_angle(int index, float value){
    //判断是否处于稳态：最近的15个角度之间相差最多不超过5
    //todo
    int max = *std::max_element(values[index].begin(), values[index].end()),
            min = *std::min_element(values[index].begin(), values[index].end());
    int cur_value = (max + min) / 2;
    if(max - min < 5 &&  cur_value != last_stable_angles[index]){
        last_stable_angles[index] = cur_value;
        emit s_stable_angle(nameset[index], cur_value);
    }
    if(Time_cycle == 0){
        for (int i = 0; i<29; i++)
        {
            showangleplot[index][i] = showangleplot[index][i+1];
        }

        showangleplot[index][29] = value;
        QwtPlotCurve* curve = new QwtPlotCurve(nameset[index]);
        QwtSymbol* symbol = new QwtSymbol(QwtSymbol::Ellipse);
        symbol->setPen(Qt::white);
        symbol->setColor(pens[index%22]->color());
        symbol->setSize(8, 8);
        curve->setSymbol(symbol);
        curve->setPen(*pens[index%22]);
        curve->setSamples(xplot,showangleplot[index],30);
        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        plot->insertLegend(new QwtLegend(),QwtPlot::RightLegend, 0.1);
        curve->attach(plot);
    }

}

void AngleManager::r_angles(float *angles)
{
    for (int index=0;index<28;index++)
    {
        //记录15帧内的所有关节角度
        values[index].push_back(angles[index]);
        if(values[index].size() > 15)
            values[index].pop_front();

        //如果该帧和前一帧之间没有突变
        if(values[index].size() > 1){
            int det = abs(values[index][values[index].size() - 1] - values[index][values[index].size() - 2]);
            if(det < 30){
                max_angles[index] = std::max(max_angles[index], values[index].back());
                min_angles[index] = std::min(min_angles[index], values[index].back());
            }
        }
    }
    refresh_angle(angles);
}

void AngleManager::r_joints(bodyangle joints){
    //跛行计算
    int diff = 0;
    //髋关节高度
    if (joints.tjoint_coordinate_3d.status[JointType_HipLeft] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_HipRight] == TrackingState_Tracked){
        diff = abs(joints.tjoint_coordinate_3d.plot3d[JointType_HipLeft][1] - joints.tjoint_coordinate_3d.plot3d[JointType_HipRight][1]) * 100;
        limpDiff = std::max(limpDiff, diff);
    }
    //膝关节高度，暂时不考虑，膝关节经常不稳定
    //双下肢长度（髋关节->踝关节）
    if(joints.tjoint_coordinate_3d.status[JointType_HipLeft] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_HipRight] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_KneeLeft] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_KneeRight] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_AnkleLeft] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_AnkleRight] == TrackingState_Tracked){
        int d1x, d1y, d1z, d2x, d2y, d2z, len1, len2;
        d1x = joints.tjoint_coordinate_3d.plot3d[JointType_HipLeft][0] - joints.tjoint_coordinate_3d.plot3d[JointType_KneeLeft][0];
        d1y = joints.tjoint_coordinate_3d.plot3d[JointType_HipLeft][1] - joints.tjoint_coordinate_3d.plot3d[JointType_KneeLeft][1];
        d1z = joints.tjoint_coordinate_3d.plot3d[JointType_HipLeft][2] - joints.tjoint_coordinate_3d.plot3d[JointType_KneeLeft][2];

        d2x = joints.tjoint_coordinate_3d.plot3d[JointType_KneeLeft][0] - joints.tjoint_coordinate_3d.plot3d[JointType_AnkleLeft][0];
        d2y = joints.tjoint_coordinate_3d.plot3d[JointType_KneeLeft][1] - joints.tjoint_coordinate_3d.plot3d[JointType_AnkleLeft][1];
        d2z = joints.tjoint_coordinate_3d.plot3d[JointType_KneeLeft][2] - joints.tjoint_coordinate_3d.plot3d[JointType_AnkleLeft][2];

        len1 = sqrt(d1x * d1x + d1y * d1y + d1z * d1z) + sqrt(d2x * d2x + d2y * d2y + d2z * d2z);

        d1x = joints.tjoint_coordinate_3d.plot3d[JointType_HipRight][0] - joints.tjoint_coordinate_3d.plot3d[JointType_KneeRight][0];
        d1y = joints.tjoint_coordinate_3d.plot3d[JointType_HipRight][1] - joints.tjoint_coordinate_3d.plot3d[JointType_KneeRight][1];
        d1z = joints.tjoint_coordinate_3d.plot3d[JointType_HipRight][2] - joints.tjoint_coordinate_3d.plot3d[JointType_KneeRight][2];

        d2x = joints.tjoint_coordinate_3d.plot3d[JointType_KneeRight][0] - joints.tjoint_coordinate_3d.plot3d[JointType_AnkleRight][0];
        d2y = joints.tjoint_coordinate_3d.plot3d[JointType_KneeRight][1] - joints.tjoint_coordinate_3d.plot3d[JointType_AnkleRight][1];
        d2z = joints.tjoint_coordinate_3d.plot3d[JointType_KneeRight][2] - joints.tjoint_coordinate_3d.plot3d[JointType_AnkleRight][2];

        len2 = sqrt(d1x * d1x + d1y * d1y + d1z * d1z) + sqrt(d2x * d2x + d2y * d2y + d2z * d2z);
        diff = abs(len1 - len2) * 100;
        //limpDiff = std::max(limpDiff, diff);
        limbDiff = std::max(limbDiff, diff);
    }
    //肩关节高度
    if(joints.tjoint_coordinate_3d.status[JointType_ShoulderLeft] == TrackingState_Tracked &&
            joints.tjoint_coordinate_3d.status[JointType_ShoulderRight] == TrackingState_Tracked){
        diff = abs(joints.tjoint_coordinate_3d.plot3d[JointType_ShoulderLeft][1] - joints.tjoint_coordinate_3d.plot3d[JointType_ShoulderRight][1]) * 100;
        limpDiff = std::max(limpDiff, diff);
    }
}

int AngleManager::get_max_angles(int index) const {
    return max_angles[index];
}
int AngleManager::get_min_angles(int index) const {
    return min_angles[index];
}
int AngleManager::get_limpDiff() const{
    return limpDiff;
}
int AngleManager::get_limbDiff() const{
    return limbDiff;
}
