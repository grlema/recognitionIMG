#ifndef IMAGE_FEATURE_H
#define  IMAGE_FEATURE_H
#include <opencv2/core/core.hpp>
struct pcaData
{
	cv::Mat mean ;
	cv::Mat eigenvalues;
	cv::Mat eigenvectors;
};


cv::Mat image_processing(cv::Mat image);
cv::Mat getEdgeImage(cv::Mat image);


pcaData getPcaFeature(cv::Mat imageData, int num_components, cv::Mat &SCORE);

pcaData loadPcaPara(string path);


void savePcaPara(string path, pcaData data);

cv::Mat  projectionPCA(cv::Mat inputData, pcaData pca_data);

void getLBPFeature(cv::Mat image);

std::vector<float> getHOGFeature(cv::Mat image);

std::vector<float> getFeature(cv::Mat image, int featureType);
/***************** Matתvector **********************/
//template<typename _Tp> vector<_Tp> convertMat2Vector(const Mat &mat)
//{
//	return (vector<_Tp>)(mat.reshape(1, 1));//ͨ�������䣬����תΪһ��
//}

/****************** vectorתMat *********************/
template<typename _Tp> cv::Mat convertVector2Mat(vector<_Tp> v, int channels, int rows)
{
	cv::Mat mat = cv::Mat(v);//��vector��ɵ��е�mat
	cv::Mat dest = mat.reshape(channels, rows).clone();//PS������clone()һ�ݣ����򷵻س���
	return dest;
}

cv::Mat array2DToMat(int *data, int rows, int cols);

#endif
