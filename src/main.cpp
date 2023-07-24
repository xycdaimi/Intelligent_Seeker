#include "log_system/LoggingSystem.h"
#include "nlohmann/JSON.hpp"
#include <opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include"yolo/yolo.h"
using namespace std;
using namespace cv;
//yolo配置
 Net_config yolo_nets[5] = {
	{0.5, 0.4, 416, 416,"../coco.names", "../yolov3/yolov3.cfg", "../yolov3/yolov3.weights", "yolov3"},
	{0.5, 0.4, 608, 608,"../coco.names", "../yolov4/yolov4.cfg", "../yolov4/yolov4.weights", "yolov4"},
	{0.5, 0.4, 608, 608,"../coco.names", "../yolov4/yolov4-tiny.cfg", "../yolov4/yolov4-tiny.weights", "yolov4-tiny"},
	{0.5, 0.4, 320, 320,"../coco.names", "../yolo-fastest/yolo-fastest-xl.cfg", "../yolo-fastest/yolo-fastest-xl.weights", "yolo-fastest"},
	{0.5, 0.4, 320, 320,"../coco.names", "../yolobile/csdarknet53s-panet-spp.cfg", "../yolobile/yolobile.weights", "yolobile"}
};
/**
 * @brief 待序列化的结构体类型
 *
 */
struct Data
{
    std::string message;
    int number;
};

/**
 * @brief         序列化函数
 *
 * @param key     json实例对象
 * @param lock    待序列化数据
 * @return true   序列化成功
 * @return false  序列化失败
 */
bool write(nlohmann::json &key, struct Data &lock)
{
    try
    {
        key["message"] = lock.message;
        key["number"] = lock.number;
    }
    catch (const std::exception &e)
    {
        Log_ERROR << e.what() << Log_END;
        return false;
    }

    return true;
}

/**
 * @brief         反序列化函数
 *
 * @param key     json实例对象
 * @param lock    待获取的数据
 * @return true   反序列化成功
 * @return false  反序列化失败
 */
bool read(nlohmann::json &key, struct Data &lock)
{
    try
    {
        key.at("message").get_to(lock.message);
        key.at("number").get_to(lock.number);
    }
    catch (const std::exception &e)
    {
        Log_ERROR << e.what() << Log_END;
        return false;
    }

    return true;
}
//初始化头节点
template<class LeiXin>
LinkList<LeiXin>::LinkList()
{
    baocun=false;
    first = new WuPin<LeiXin>;
    first->next = NULL;
}

//析构
template<class LeiXin>
LinkList<LeiXin>::~LinkList()
{
    while (first != NULL)
    {
        WuPin<LeiXin>* q = first;
        first = first->next;
        delete q;
    }
}
//创建链表
template<class LeiXin>
LinkList<LeiXin>::LinkList(LeiXin a[], int n)
{
    baocun=false;
    first = new WuPin<LeiXin>;
    first->next = NULL;
    for (int i = 0; i < n; i++)
    {
        WuPin<LeiXin>* s = new WuPin<LeiXin>;
        s->data = a[i];
        s->next = first->next;
        first->next = s;
    }
}
//遍历输出
template<class LeiXin>
void LinkList<LeiXin>::PrintList()
{
    WuPin<LeiXin>* p = first->next;
    while (p != NULL)
    {
        cout << p->data.toString()<<endl;
        p = p->next;
    }
}
//从文件读入坐标
template<class LeiXin>
void LinkList<LeiXin>::input()
{
	WuPin<LeiXin>* p = first;
	fstream file;
	string name,x,y,height,width;
	string n0;
	int x0,y0,h0,w0;
	LeiXin e;
	file.open("../save-data/coordinate.txt");
	if (!file) 
	{
		cout << "错误！未找到文件！" << endl;
		exit(0);
	}
	file>>name>>x>>y>>height>>width;
	while(!file.eof())
	{
		file>>n0>>x0>>y0>>h0>>w0;
		e.setLei(n0,x0,y0,h0,w0);
		Inset(e);
	}
	cout << "输入 coordinate.txt 信息完毕\n\n";
	file.close();
}
//将坐标保存到文件
template<class LeiXin>
void LinkList<LeiXin>::output()
{
	WuPin<LeiXin>* p = first->next;
	string name="物品名称",x="x坐标",y="y坐标",height="高",width="宽";
	fstream file;
	file.open("../save-data/coordinate.txt", fstream::out | ios_base::trunc);
	if (!file) {
		cout << "错误！未找到文件！" << endl;
		exit(0);
	}
	file<<name<<"\t"<<x<<"\t"<<y<<"\t"<<height<<"\t"<<width;
	while (p!= NULL)
	{
		file<<endl<<p->data.getName()<<"\t"<<p->data.getX()<<"\t"<<p->data.getY()<<"\t"<<p->data.getH()<<"\t"<<p->data.getW();
		p=p->next;
	}
	file.close();
}
int Video_yolo()
{
    LinkList<LeiXin> p;
    //p.input();
    YOLO yolo_model(yolo_nets[3]);//yolov4有时候会崩溃
    Mat srcimg ;//= imread("/home/xyc/darknet-master/data/person.jpg");
    Mat images;
    char key;
    VideoCapture video = VideoCapture(0);//("/home/xyc/Desktop/whc.mp4");
    if (!video.isOpened()) {
        cout << "could find the video" << endl;
        return -1;
    }
    Size s = Size((int)video.get(CAP_PROP_FRAME_WIDTH), (int)video.get(CAP_PROP_FRAME_HEIGHT));
    //int fps = video.get(CAP_PROP_FPS);
    VideoWriter fourcc = VideoWriter("../save-data/video.mp4",0x7634706d/*CAP_OPENCV_MJPEG*/, 15, s, true);
    static const string kWinName = "Intelligent_Seeker";
    namedWindow(kWinName, WINDOW_NORMAL);
    //resizeWindow(kWinName,500,600);
    while (video.read(srcimg))
    {
        flip(srcimg,srcimg, 1);
        images = srcimg;
        key = waitKey(50);
        yolo_model.detect(images,&p);
        imshow(kWinName, srcimg);
        fourcc.write(srcimg);
        if (key == 'c') {
            break;
        }
    }
    p.baocun=true;
    yolo_model.detect(images,&p);
    //waitKey(0);
    video.release();
    fourcc.release();
    destroyAllWindows();
    p.output();
    return 0;
}
int main(int argc, char const *argv[])
{/*
    LoggingSystem::Initialize(LoggingSystem::Grade::INFO);

    Log_INFO << "you input message is :" << argv[1] << Log_END; // 输入需要保存的json文件路径
    
    std::string Configure(argv[1]);
    struct Data data;
    data.message = "hello world json success";
    data.number = 4;

    JSON::Write_Json(Configure, write, data);

    data.message = "error";

    JSON::Read_Json(Configure, read, data);

    std::cout << data.message << std::endl;

    LoggingSystem::Release();
    std::cout<<"ajksdhjsa"<<std::endl;*/
    Video_yolo();
    return 0;
}
