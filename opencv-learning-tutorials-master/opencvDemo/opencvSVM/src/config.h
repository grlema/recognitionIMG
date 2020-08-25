#ifndef CONFIG_H
#define CONFIG_H
#include <opencv2/opencv.hpp>
using namespace std;
//******************************************����ѵ������******************************************/
static string  project_root   = "E:/git/opencv-learning-tutorials";      //��Ŀ��Ŀ¼
static string  train_filename = project_root + "/data/train_labels.txt"; //ѵ����ǩ�ļ�
static string  image_dir      = project_root + "/data/trainData";        //ѵ��ͼ��Ŀ¼
static string  test_image     = project_root + "/data/trainData/A1.jpg"; //����ͼƬ
static string  modelPath      = project_root + "/opencvDemo/opencvSVM/model";//ģ�ͱ���Ŀ¼
static string  saveSVMPath    = modelPath + "/SVM_HOG.xml";//����SVMѵ���õ�ģ�͵�·��
static string  loadSVMPath	  = modelPath + "/SVM_HOG.xml"; //����SVMѵ���õ�ģ��

//******************************************����ѵ������******************************************/
#define toGray                   true//�Ƿ�ͼ��תΪ�Ҷ�ͼ
#define IM_RESIZE(src)           cv::resize(src, src, cv::Size(64, 64))
#define doPCA                    true// �Ƿ����PCA��ά����
#define dimNum                   95//��ά��100��
static string pcaPath		  =  modelPath + "/pca.xml";

#define predictTH                0.0
#endif 