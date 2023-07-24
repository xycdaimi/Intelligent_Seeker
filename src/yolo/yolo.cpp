#include "yolo/yolo.h"
#include<cstring>
#include<string.h>
LeiXin::LeiXin(string name,int x,int y,int h,int w){
    this->name=name;
    this->x=x;
    this->y=y;
    this->h=h;
    this->w=w;
}

LeiXin::LeiXin(){
}

void LeiXin::setLei(string name,int x,int y,int h,int w){
    this->name=name;
    this->x=x;
    this->y=y;
    this->h=h;
    this->w=w;
}

void LeiXin::setLei(int x,int y,int h,int w){
    this->x=x;
    this->y=y;
    this->h=h;
    this->w=w;
}

int LeiXin::getX(){
    return x;
}

int LeiXin::getY(){
    return y;
}

int LeiXin::getH(){
    return h;
}

int LeiXin::getW(){
    return w;
}

string LeiXin::getName(){
    return name;
}

string LeiXin::toString(){
    ostringstream oss;
    oss << "name: " << getName() << ", (" << getX() << "," << getY() << "), height: " << getH() << ", width: " << getW();
    return oss.str();
}

//返回链表长度
template<class LeiXin>
int LinkList<LeiXin>::Length()
{
    WuPin<LeiXin>* p = first->next;
    int count = 0;
    while (p != NULL)
    {
        p = p->next;
        count++;
    }
    return count;
}
//返回第i个元素的值
template<class LeiXin>
LeiXin LinkList<LeiXin>::Get(int i)
{
    WuPin<LeiXin>* p = first->next;
    int count = 1;
    while (p != NULL && count < i)
    {
        p = p->next;
        count++;
    }
    if (p == NULL) throw "Location";
    else return p->data;
}
//输入x，返回x的位置count
template<class LeiXin>
int LinkList<LeiXin>::Locate(LeiXin x)
{
    WuPin<LeiXin>* p = first->next;
    int count = 1;
    while (p != NULL)
    {
        if (p->data.getName() == x.getName()) return count;
        p = p->next;
        count++;
    }
    return 0;
}
//传入插入元素的位置i和数据x
template<class LeiXin>
void LinkList<LeiXin>::Insert(int i, LeiXin x)
{
    WuPin<LeiXin>* p = first;
    int count = 0;
    while (p != NULL && count < i - 1)
    {
        p = p->next;
        count++;
    }
    if (p == NULL) throw "Location";
    else {
        WuPin<LeiXin>* s = new WuPin<LeiXin>;
        s->data = x;
        s->next = p->next;
        p->next = s;
    }
}
//
template<class LeiXin>
void LinkList<LeiXin>::Inset(LeiXin x)
{
    WuPin<LeiXin>* p = first;
    while (p->next != NULL)
    {
        p = p->next;
    }
    WuPin<LeiXin>* s = new WuPin<LeiXin>;
    s->data = x;
    s->next = p->next;
    p->next = s;
}
//删除位置i并返回删除的值
template<class LeiXin>
LeiXin LinkList<LeiXin>::Delete(int i)
{
    WuPin<LeiXin>* p = first;
    int count = 0;
    while (p != NULL && count < i - 1)
    {
        p = p->next;
        count++;
    }
    if (p == NULL || p->next == NULL) throw "Location";
    else {
        WuPin<LeiXin>* q = p->next;
        LeiXin x = q->data;
        p->next = q->next;
        free(q);
        return x;
    }
}

//初始化yolo设置
YOLO::YOLO(Net_config config)
{
	
	cout << "Net use " << config.netname << endl;
	this->confThreshold = config.confThreshold;
	this->nmsThreshold = config.nmsThreshold;
	this->inpWidth = config.inpWidth;
	this->inpHeight = config.inpHeight;
	strcpy(this->netname, config.netname.c_str());

	ifstream ifs(config.classesFile.c_str());
	string line;
	while (getline(ifs, line)) this->classes.push_back(line);

	this->net = readNetFromDarknet(config.modelConfiguration, config.modelWeights);
	this->net.setPreferableBackend(DNN_BACKEND_OPENCV);
	this->net.setPreferableTarget(DNN_TARGET_CPU);
}
//计算识别概率最高非最大值的物品边界框
void YOLO::postprocess(Mat& frame, const vector<Mat>& outs,LinkList<LeiXin>* p)
{
	vector<int> classIds;
	vector<float> confidences;
	vector<Rect> boxes;

	for (size_t i = 0; i < outs.size(); ++i)
	{
		// 扫描网络层输出的所有边界框，并保留
		// 概率最高的将数据保存在box's class里 
		// 概率最高的在box里
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			Point classIdPoint;
			double confidence;
			// 获取最高概率的值和位置
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > this->confThreshold)
			{
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(Rect(left, top, width, height));
			}
		}
	}

	// 执行非最大抑制以消除冗余重叠框
	// 降低置信度
	vector<int> indices;
	NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		this->drawPred(classIds[idx], confidences[idx], box.x, box.y,
			box.x + box.width, box.y + box.height, frame,p);
	}
}
void YOLO::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame,LinkList<LeiXin>* p)   // Draw the predicted bounding box
{
	if(p->baocun==true){
	    LeiXin a{this->classes[classId],(right+left)/2,(bottom+top)/2,bottom-top,right-left};
	    p->Inset(a);
	}
	//Draw a rectangle displaying the bounding box
	
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255), 3);
	//Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	if (!this->classes.empty())
	{
		CV_Assert(classId < (int)this->classes.size());
		label = this->classes[classId] + ":" + label;
	}

	//Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = max(top, labelSize.height);
	//rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 255, 0), 1);
	
}

void YOLO::detect(Mat& frame,LinkList<LeiXin>* p)
{
	Mat blob;
	blobFromImage(frame, blob, 1 / 255.0, Size(this->inpWidth, this->inpHeight), Scalar(0, 0, 0), true, false);
	this->net.setInput(blob);
	vector<Mat> outs;
	this->net.forward(outs, this->net.getUnconnectedOutLayersNames());
	this->postprocess(frame, outs,p);

	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;
	string label = format("%s Inference time : %.2f ms", this->netname, t);
	putText(frame, label, Point(0, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
	if(p->baocun==true){
	    imwrite(format("../save-data/%s_out.jpg", this->netname), frame);
	}
}
