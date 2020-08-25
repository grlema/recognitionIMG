// imageCheck.cpp : �������̨Ӧ�ó������ڵ㡣

#include <fstream>
#include <string>
#include "vector"
#include <iostream>
#include "file_processing.h"
#include <opencv2/opencv.hpp>
#include "imageFeature.h"
#include "config.h"
#include "SVM.h"
using namespace std;

//ѵ��
int train(string trian_filename, string image_dir,string saveModel)
{
	MySVM *svm = new MySVM();
	FileData fileData = loadFileData((char *)trian_filename.c_str());
	cv::Mat labelsMat=array2DToMat((int*)fileData.data, txtRows, txtCols-1);
	int sampleNums = fileData.name.size();//��������
	int featureDim;//����ά��
	cv::Mat sampleFeatureMat;//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��	
	cv::Mat sampleLabelMat;//ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����

	for (size_t num = 0; num < sampleNums; num++)
	{
		string image_name = fileData.name.at(num);
		string image_path = image_dir+(string)"/"+ image_name;
		//ͼ��Ԥ����
		cv::Mat image = cv::imread(image_path);//��ȡͼƬ
		if (image.empty()) {
			std::cout << "no path:"<<image_path << endl;
			continue;
		}
		std::cout << image_path << endl;
		//ͼ��Ԥ����
		image = image_processing(image);
		//��ȡ����
		vector<float> per_fea = getFeature(image, 0);
		//�����һ������ʱ��ʼ���������������������
		if (0 == num)
		{
			std::cout << "����ά��:" << per_fea.size() << endl;
			featureDim = per_fea.size();//HOG�����ӵ�ά��
			 //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
			sampleFeatureMat = cv::Mat::zeros(sampleNums, featureDim, CV_32FC1);
			//��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
			sampleLabelMat = cv::Mat::zeros(sampleNums, 1, CV_32FC1);
		}

		//������������sampleFeatureMat
		for (int i = 0; i < featureDim; i++)
			sampleFeatureMat.at<float>(num, i) = per_fea[i];//��num�����������������еĵ�i��Ԫ��
		sampleLabelMat.at<float>(num, 0) = labelsMat.at<uchar>(num,0);//���������Ϊ1������
	}

	//����ѵ�����ݺͱ�ǩ
	cv::Mat trainData = sampleFeatureMat;
	cv::Mat trainLable = sampleLabelMat;
	if (doPCA)
	{
		cv::Mat SCORE;
		pcaData data = getPcaFeature(sampleFeatureMat, dimNum, SCORE);
		savePcaPara(pcaPath, data);
		std::cout << "PCA��ά:" << dimNum << endl;
		trainData = SCORE;
	}

	svm->svm_train(trainData, trainLable);
	svm->save_model(saveModel);
	return 0;
}

//����
int test(string model_path, string image_path) {

	MySVM *svm = new MySVM();
	svm->load_model(model_path);
	pcaData pca_data = loadPcaPara(pcaPath);


	std::cout << image_path << endl;
	cv::Mat image = cv::imread(image_path);//��ȡͼƬ
	 //ͼ��Ԥ����
	image = image_processing(image);
	//��ȡ����
	vector<float> per_fea = getFeature(image, 0);
	int featureDim = per_fea.size();//HOG�����ӵ�ά��
	//������������sampleFeatureMat
	cv::Mat sampleFeatureMat = cv::Mat::zeros(1, featureDim, CV_32FC1);
	for (int i = 0; i < featureDim; i++)
		sampleFeatureMat.at<float>(0, i) = per_fea[i];//��num�����������������еĵ�i��Ԫ��

	cv::Mat testData = sampleFeatureMat;
	if (doPCA)
	{
		testData = projectionPCA(testData, pca_data);
	}
	float predict=svm->svm_predict(testData);
	std::cout << "ʶ����:" << predict << endl;
	cout << "***************************SVM�������***************************" << endl;
	return 0;
}

//��������
int batch_test(string test_filename, string image_dir,string model_path) {
	MySVM *svm = new MySVM();
	svm->load_model(model_path);
	pcaData pca_data = loadPcaPara(pcaPath);

	FileData fileData = loadFileData((char *)test_filename.c_str());
	cv::Mat labelsMat = array2DToMat((int*)fileData.data, txtRows, txtCols - 1);
	int sampleNums = fileData.name.size();//��������
	int featureDim;//����ά��
	for (size_t num = 0; num < sampleNums; num++)
	{
		string image_name = fileData.name.at(num);
		string image_path = image_dir + (string)"/" + image_name;
		cv::Mat image = cv::imread(image_path);//��ȡͼƬ
											   //ͼ��Ԥ����
		image = image_processing(image);
		//��ȡ����
		vector<float> per_fea = getFeature(image, 0);
		int featureDim = per_fea.size();//HOG�����ӵ�ά��
		//������������sampleFeatureMat
		cv::Mat sampleFeatureMat = cv::Mat::zeros(1, featureDim, CV_32FC1);
		for (int i = 0; i < featureDim; i++)
			sampleFeatureMat.at<float>(0, i) = per_fea[i];//��num�����������������еĵ�i��Ԫ��

		cv::Mat testData = sampleFeatureMat;
		if (doPCA)
		{
			testData = projectionPCA(testData, pca_data);
		}
		float predict = svm->svm_predict(testData);
		std::cout << "ʶ����:" << predict << endl;
	}
	cout << "***************************SVM�������***************************" << endl;
	return 0;

}
int main() {
	train(train_filename, image_dir, saveSVMPath);
	test(saveSVMPath, test_image);
	batch_test(train_filename, image_dir, saveSVMPath);

}