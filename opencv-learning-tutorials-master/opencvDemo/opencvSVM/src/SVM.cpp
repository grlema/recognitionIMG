#include "SVM.h"
#include <fstream>
#include <string>
#include "vector"
#include <iostream>
#include "file_processing.h"
#include <opencv2/opencv.hpp>
#include "imageFeature.h"
#include "config.h"
using namespace cv::ml;

MySVM::MySVM()
{
	model = SVM::create();
}

MySVM::~MySVM()
{
	model.release();
}
void MySVM::load_model(string model_path) {
	model = cv::ml::SVM::load(loadSVMPath);//��XML�ļ���ȡѵ���õ�SVMģ��
}
void MySVM::save_model(string model_path) {
	model->save(model_path);
}

cv::Ptr<cv::ml::SVM> MySVM::get_model() {
	return model;
}

void MySVM::svm_train(cv::Mat &trainData, cv::Mat &trainLabels) {
	/************************************************************************/
	//ѵ��SVM������
	//������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
	cv::TermCriteria  criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	model->setType(SVM::C_SVC);
	//�˺��� SIGMOID(40.7%)��LINEAR(93.3%)��RBF(95.3)��CHI2(54.5%),POLY
	//svm->setKernel(SVM::LINEAR);  
	model->setKernel(SVM::RBF);
	model->setTermCriteria(criteria);//�����㷨����ֹ����
	//����ѵ������   
	trainData.convertTo(trainData, CV_32FC1);
	trainLabels.convertTo(trainLabels, CV_32SC1);
	cv::Ptr<TrainData> tData = TrainData::create(trainData, ROW_SAMPLE, trainLabels);
	// ѵ��������  
	model->train(tData);
	//����ѵ�����XML�ļ�
	cout << "***************************SVMѵ�����***************************" << endl;
}

float MySVM::svm_predict(cv::Mat &FeatureData) {
	 //��ѵ���õ�SVM�������Բ���ͼƬ�������������з���
	float result = model->predict(FeatureData,cv::noArray(), cv::ml::StatModel::Flags::RAW_OUTPUT);//�������
	return result;
}