#ifndef YOLO_H
#define YOLO_H
#endif
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<string>
#include<cstdlib>
using namespace cv;
using namespace dnn;
using namespace std;

//物品数据
class LeiXin
{
private:
    string name;
    int x;
    int y;
    int h;
    int w;
public:
    LeiXin(string name,int x,int y,int h,int w);
    LeiXin();
    void setLei(string name,int x,int y,int h,int w);
    void setLei(int x,int y,int h,int w);
    int getX();
    int getY();
    int getH();
    int getW();
    string getName();
    string toString();
};
//定义一个节点
template<class LeiXin>
struct WuPin
{
	LeiXin data;
	WuPin<LeiXin>* next;
};
//定义一个链表
template<class LeiXin>
class LinkList
{
public:
    bool baocun;
    LinkList();
    LinkList(LeiXin a[], int n);
    ~LinkList();
    int Length();
    LeiXin Get(int i);
    int Locate(LeiXin x);
    void Insert(int i, LeiXin x);
    void Inset(LeiXin x);
    LeiXin Delete(int i);
    void PrintList();
    void output();
    void input();
private:
    WuPin<LeiXin>*  first;
};

struct Net_config
{
	float confThreshold; // Confidence threshold
	float nmsThreshold;  // Non-maximum suppression threshold
	int inpWidth;  // Width of network's input image
	int inpHeight; // Height of network's input image
	string classesFile;
	string modelConfiguration;
	string modelWeights;
	string netname;
};

class YOLO
{
	public:
		YOLO(Net_config config);
		void detect(Mat& frame,LinkList<LeiXin>* p);
	private:
		float confThreshold;
		float nmsThreshold;
		int inpWidth;
		int inpHeight;
		char netname[20];
		vector<string> classes;
		Net net;
		void postprocess(Mat& frame, const vector<Mat>& outs,LinkList<LeiXin>* p);
		void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame,LinkList<LeiXin>* p);
};


