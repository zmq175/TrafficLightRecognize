#include <iostream>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

void showImage(const char* windowName, IplImage* img)
{
	cvNamedWindow(windowName, 1);
	cvResizeWindow(windowName, 500, 500);
	cvShowImage(windowName, img);
}

int main(int argc, char ** argv)
{
	Mat image = imread("1.jpg");
	int red_val = 0;
	int green_val = 50;
	Mat hsv;
	Mat red_mask;
	Mat green_mask;
	cvtColor(image, hsv, CV_BGR2HSV);
	//define range of RED color in HSV
	int r_lower[3] = { red_val - 10,100,100 };
	int r_upper[3] = { red_val + 10,255,255 };
	//define range of Green color in HSV
	int g_lower[3] = { green_val - 10,100,100 };
	int g_upper[3] = { green_val + 40,255,255 };
	vector<int> red_lower(r_lower,r_lower+3);
	vector<int> red_upper(r_upper, r_upper+3);
	vector<int> green_lower(g_lower, g_lower+3);
	vector<int> green_upper(g_upper, g_upper+3);
	inRange(hsv, red_lower, red_upper, red_mask);
	inRange(hsv, green_lower, green_upper, green_mask);
	Mat red_res;
	Mat green_res;
	bitwise_and(image, image, red_res, red_mask);
	bitwise_and(image, image, green_res, green_mask);
	//Structuring Element
	Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
	//Morphological Closing
	Mat red_closing;
	Mat green_closing;
	morphologyEx(red_res, red_closing, MORPH_CLOSE, kernel);
	morphologyEx(green_res, green_closing, MORPH_CLOSE, kernel);
	Mat red_gray, green_gray;
	cvtColor(red_closing, red_gray,CV_BGR2GRAY);
	cvtColor(green_closing, green_gray, CV_BGR2GRAY);
	Mat red_bw, green_bw;
	threshold(red_gray, red_bw, 128, 255, THRESH_BINARY | THRESH_OTSU);
	threshold(green_gray, green_bw, 128, 255, THRESH_BINARY | THRESH_OTSU);
	int red_black = countNonZero(red_bw);
	int green_black = countNonZero(green_bw);
	//imshow("g", green_bw);
	cout << (red_black > green_black ? "红灯" : "绿灯") << endl;
	waitKey();
	return 0;
	////加载原图
	//IplImage* srcImg = cvLoadImage("1.jpg", 1);
	//showImage("原图", srcImg);
	////转换成灰度图
	//IplImage* grayImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	//cvCvtColor(srcImg, grayImg, CV_BGR2GRAY);
	////膨胀
	//IplConvKernel *element1 = cvCreateStructuringElementEx(2 * 5 + 1, 2 * 5 + 1, 5, 5, CV_SHAPE_RECT, 0);//创建结构元素
	//IplImage* dilateImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	//cvDilate(grayImg, dilateImg, element1);
	////腐蚀
	//IplConvKernel *element2 = cvCreateStructuringElementEx(2 * 2 + 1, 2 * 2 + 1, 2, 2, CV_SHAPE_RECT, 0);//创建结构元素
	//IplImage* erodeImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	//cvErode(dilateImg, erodeImg, element2);
	////转换成二值图
	//IplImage* binaryImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	//cvThreshold(erodeImg, binaryImg, 200, 255, CV_THRESH_BINARY);
	//showImage("二值图", binaryImg);
	//CvMemStorage* storage = cvCreateMemStorage(0);
	//CvRect rect;
	//CvSeq* contours = NULL;
	//cvFindContours(binaryImg, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	//IplImage* outlineImg = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_32F, 3);
	//int index = 0;
	//CvSeq *pCurSeq = contours;
	//CvSeq *pOldSeq = NULL;
	//while (pCurSeq)
	//{
	//	double tmparea = fabs(cvContourArea(pCurSeq));
	//	if (tmparea < 400)  //删除掉面积比较小的轮廓
	//	{
	//		pOldSeq = pCurSeq;
	//		if (pOldSeq->h_prev)
	//		{
	//			pCurSeq = pOldSeq->h_prev;
	//			pCurSeq->h_next = pOldSeq->h_next;
	//			pOldSeq->h_next->h_prev = pCurSeq;
	//			pCurSeq = pCurSeq->h_next;
	//			cvClearSeq(pOldSeq);
	//		}
	//		else
	//		{
	//			pCurSeq = pOldSeq->h_next;
	//			pCurSeq->h_prev = NULL;
	//			cvClearSeq(pOldSeq);
	//		}
	//	}
	//	else
	//	{
	//		CvRect rect1 = cvBoundingRect(pCurSeq, 0);
	//		cvDrawContours(outlineImg, pCurSeq, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 0, 2, CV_FILLED, cvPoint(0, 0));
	//		cvRectangleR(outlineImg, rect1, CV_RGB(255, 0, 0), 1, 8, 0);
	//		//找出完整包含轮廓的最小矩形
	//		CvBox2D rect = cvMinAreaRect2(pCurSeq);
	//		//检测轮廓是否为凸包，1为凸包，0为非凸
	//		int checkcxt = cvCheckContourConvexity(pCurSeq);
	//		CvSeq* hull = cvConvexHull2(pCurSeq, 0, CV_CLOCKWISE, 0);//二维凸包
	//		CvMemStorage* hullStorage = cvCreateMemStorage(0);
	//		CvSeq* defect = cvConvexityDefects(pCurSeq, hull, hullStorage);//凸包中的缺陷
	//		cvDrawContours(outlineImg, defect, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, 2, CV_FILLED, cvPoint(0, 0));
	//		//用cvBoxPoints找出矩形的4个顶点
	//		CvPoint2D32f rect_pts0[4];
	//		cvBoxPoints(rect, rect_pts0);
	//		int npts = 4;
	//		CvPoint rect_pts[4];
	//		for (int rp = 0; rp<4; rp++)
	//			rect_pts[rp] = cvPointFrom32f(rect_pts0[rp]);
	//		CvPoint lpt = cvPoint(srcImg->width, srcImg->height);
	//		CvPoint rpt = cvPoint(0, 0);
	//		CvPoint tpt = cvPoint(srcImg->width, srcImg->height);
	//		CvPoint bpt = cvPoint(0, 0);
	//		//求最左点
	//		for (int i = 0; i<4; i++)
	//		{
	//			if (rect_pts[i].x<lpt.x)
	//				lpt = rect_pts[i];
	//		}
	//		for (int i = 0; i<4; i++)
	//		{
	//			if (rect_pts[i].x>rpt.x)
	//				rpt = rect_pts[i];
	//		}
	//		for (int i = 0; i<4; i++)
	//		{
	//			if (rect_pts[i].y<tpt.y)
	//				tpt = rect_pts[i];
	//		}
	//		for (int i = 0; i<4; i++)
	//		{
	//			if (rect_pts[i].y>bpt.y)
	//				bpt = rect_pts[i];
	//		}
	//		//画出对角线
	//		cvLine(outlineImg, lpt, rpt, CV_RGB(0, 0, 255));
	//		cvLine(outlineImg, tpt, bpt, CV_RGB(0, 0, 255));
	//		//画出中心点
	//		CvPoint *pt = rect_pts;
	//		cvPolyLine(outlineImg, &pt, &npts, 1, 1, CV_RGB(255, 0, 0), 2);
	//		cvCircle(outlineImg, cvPoint(rect.center.x, rect.center.y), 3, CV_RGB(255, 255, 0), 3, 8, 0);
	//		pCurSeq = pCurSeq->h_next;
	//	}
	//}
	//IplImage *edge = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	//IplImage *circle = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 3);
	//cvCopy(srcImg, circle);
	//cvCanny(grayImg, edge, 100, 150);
	//CvMemStorage *circle_storage = cvCreateMemStorage(0);
	//IplImage *smooth = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	//cvSmooth(grayImg, smooth, CV_GAUSSIAN, 5, 5);
	//CvSeq *circle_results = cvHoughCircles(edge, circle_storage, CV_HOUGH_GRADIENT, 2, grayImg->width / 10);
	//for (int i = 0; i < circle_results->total; i++)
	//{
	//	float *p = (float*)cvGetSeqElem(circle_results, i);
	//	CvPoint pt = CvPoint(cvRound(p[0]), cvRound(p[1]));
	//	cvCircle(circle, pt, cvRound(p[2]), CV_RGB(0xff, 0xff, 0xff));
	//}
	//showImage("circle", circle);
	//showImage("edge", edge);
	//showImage("抓取的轮廓图", outlineImg);
	//cvReleaseImage(&srcImg);
	//cvReleaseImage(&grayImg);
	//cvReleaseImage(&dilateImg);
	//cvReleaseImage(&erodeImg);
	//cvReleaseImage(&outlineImg);
	//waitKey();
	//return 0;
}