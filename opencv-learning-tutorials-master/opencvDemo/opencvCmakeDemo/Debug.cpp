#include "opencv2/opencv.hpp"
using namespace std;
#include "Debug.h"


//*************************************��ʾͼƬ****************************************
#define RESIZE(img_show,col)  cv::resize(img_show, img_show, cv::Size(col, img_show.rows*col / img_show.cols))

void showImages(const char *imageName, cv::Mat img) {
	cv::Mat img_show = img.clone();
	if (img_show.channels() == 1)
		cvtColor(img_show, img_show, cv::COLOR_GRAY2BGR);

	//char str[200];
	char str1[200];
	strcpy(str1, imageName);
	//sprintf(str, ",Size:%dx%d", image.rows, image.cols);
	//strcat(str1, str);
	RESIZE(img_show, 400);
	cv::imshow(str1, img_show);
	cv::waitKey(100);
}


void showImages(const char *imageName, cv::Mat img, cv::Rect face, std::vector<cv::Point> pts)
{
	cv::Mat img_show = img.clone();
	if (img_show.channels() == 1)
		cvtColor(img_show, img_show, cv::COLOR_GRAY2BGR);

	for (int i = 0; i < pts.size(); ++i) {
		//std::cout << "index: " << i << std::endl;
		cv::circle(img_show, pts.at(i), 2.f, cv::Scalar(0, 0, 255), -1, CV_AA);
		//		char str[3];
		//		itoa(i, str, 10);
		//		//line(imgDrawFace, cv::Point(shape.part(i).x(), shape.part(i).y()), cv::Point(shape.part(i).x(), shape.part(i).y()), cv::Scalar(0, i * 3, 255), 2);
		//		putText(img_show, str, pts.at(i), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 0));

	}
	cv::rectangle(img_show, face, { 255, 0, 0 }, 2);
	char str[200];
	char str1[200];
	strcpy(str1, imageName);
	//sprintf(str, ",Size:%dx%d", img.rows, img.cols);
	//strcat(str1, str);
	RESIZE(img_show, 400);
	cv::imshow(str1, img_show);
	cv::waitKey(100);
}


void showImages(const char *imageName, cv::Mat img, std::vector<cv::Point> pts)
{
	cv::Mat img_show = img.clone();
	if (img_show.channels() == 1)
		cvtColor(img_show, img_show, cv::COLOR_GRAY2BGR);

	for (int i = 0; i < pts.size(); ++i) {
		//std::cout << "index: " << i << std::endl;
		cv::circle(img_show, pts.at(i), 2.f, cv::Scalar(0, 0, 255), -1, CV_AA);
		//		char str[3];
		//		itoa(i, str, 10);
		//		//line(imgDrawFace, cv::Point(shape.part(i).x(), shape.part(i).y()), cv::Point(shape.part(i).x(), shape.part(i).y()), cv::Scalar(0, i * 3, 255), 2);
		//		putText(img_show, str, pts.at(i), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 0));
	}
	char str[200];
	char str1[200];
	strcpy(str1, imageName);
	//sprintf(str, ",Size:%dx%d", img.rows, img.cols);
	//strcat(str1, str);
	RESIZE(img_show, 400);
	cv::imshow(str1, img_show);
	cv::waitKey(100);
}



void showImages(const char *imageName, cv::Mat img, cv::Rect face)
{
	int thickness = img.cols*0.005;
	thickness = thickness > 1 ? thickness : 1;
	cv::Mat img_show = img.clone();
	if (img_show.channels() == 1)
		cvtColor(img_show, img_show, cv::COLOR_GRAY2BGR);

	cv::rectangle(img_show, face, { 255, 0, 0 }, thickness);
	char str[200];
	char str1[200];
	strcpy(str1, imageName);
	//sprintf(str, ",Size:%dx%d", img.rows, img.cols);
	//strcat(str1, str);
	RESIZE(img_show, 400);
	cv::imshow(str1, img_show);
	cv::waitKey(100);
}

//*************************************����ͼƬ****************************************


void saveImage(const char *imageName, cv::Mat image) {
	cv::imwrite(imageName, image);
}

void saveImage(const char *imageName, cv::Mat image, std::vector<int> para) {
	cv::imwrite(imageName, image, para);
}

void saveImage(const char *imageName, cv::Mat image, cv::Rect face, std::vector<cv::Point> pts) {
	int thickness = image.cols*0.005;
	thickness = thickness > 1 ? thickness : 1;
	cv::Mat img = image.clone();
	for (int i = 0; i < pts.size(); ++i) {
		//std::cout << "index: " << i << std::endl;
		cv::circle(img, pts.at(i), 2.f, cv::Scalar(0, 0, 255), thickness, CV_AA);
	}
	cv::rectangle(img, face, { 255, 0, 0 }, thickness);
	cv::imwrite(imageName, img);
}

void saveImage(const char *imageName, cv::Mat img, cv::Rect face)
{
	cv::Mat img_show = img.clone();
	if (img_show.channels() == 1)
		cvtColor(img_show, img_show, cv::COLOR_GRAY2BGR);
	cv::rectangle(img_show, face, { 255, 0, 0 }, 2);
	cv::imwrite(imageName, img_show);
}


//*************************************��ȡ�ļ��б�****************************************
#ifdef _LINUX  
#include <memory.h>  
#include <dirent.h>  
vector<string> getFilesList(string dirpath) {
	vector<string> allPath;
	DIR *dir = opendir(dirpath.c_str());
	if (dir == NULL)
	{
		cout << "opendir error" << endl;
		return allPath;
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR) {//It's dir  
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			string dirNew = dirpath + separator + entry->d_name;
			vector<string> tempPath = getFilesList(dirNew);
			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());

		}
		else {
			//cout << "name = " << entry->d_name << ", len = " << entry->d_reclen << ", entry->d_type = " << (int)entry->d_type << endl;  
			string name = entry->d_name;
			string imgdir = dirpath + separator + name;
			//sprintf("%s",imgdir.c_str());  
			allPath.push_back(imgdir);
		}

	}
	closedir(dir);
	//system("pause");  
	return allPath;
}
#endif  

#ifdef _WIN32//__WINDOWS_  
#include <io.h>    
vector<string> getFilesList(string dir)
{
	vector<string> allPath;
	// ��Ŀ¼�������"\\*.*"���е�һ������  
	string dir2 = dir + separator + "*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir2.c_str(), &findData);
	if (handle == -1) {// ����Ƿ�ɹ�  
		cout << "can not found the file ... " << endl;
		return allPath;
	}
	while (_findnext(handle, &findData) == 0)
	{
		if (findData.attrib & _A_SUBDIR)//// �Ƿ�����Ŀ¼  
		{
			//������Ŀ¼Ϊ"."��".."���������һ��ѭ�������������Ŀ¼������������һ������  
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			// ��Ŀ¼�������"\\"����������Ŀ¼��������һ������  
			string dirNew = dir + separator + findData.name;
			vector<string> tempPath = getFilesList(dirNew);
			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else //������Ŀ¼�������ļ���������ļ������ļ��Ĵ�С  
		{
			string filePath = dir + separator + findData.name;
			allPath.push_back(filePath);
		}
	}
	_findclose(handle);    // �ر��������  
	return allPath;
}
#endif  

//***********************************�����ݱ��浽txt�ı���*************************************
void writeDatatxt(string path, string data, bool bCover) {

	//fstream fout(path, ios::app);
	fstream fout;
	if (bCover)
	{
		fout.open(path);//Ĭ���ǣ�ios_base::in | ios_base::out
	}
	else
	{
		fout.open(path, ios::app);//����д�븽�����ļ�ĩβ
	}
	fout << data << endl;
	fout.flush();
	fout.close();
}
