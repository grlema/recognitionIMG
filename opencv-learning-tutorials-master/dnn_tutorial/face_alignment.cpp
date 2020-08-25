#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace cv::face;
using namespace std;

const String  lbpfilePath = "D:/opencv-3.4/opencv/build/etc/lbpcascades/lbpcascade_frontalface.xml";
bool myDetector(InputArray image, OutputArray faces, CascadeClassifier *face_cascade);
void face_alignment(Mat &face, Point left, Point right, Rect roi);

int main(int argc, char** argv) {
	Mat img = imread("D:/vcprojects/images/gaoyy.png");
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	imshow("input", img);

	CascadeClassifier face_cascade;
	face_cascade.load(lbpfilePath);

	FacemarkLBF::Params params;
	params.n_landmarks = 68; // 68����ע��
	params.initShape_n = 10;
	params.stages_n = 5; // �㷨��5��ǿ������
	params.tree_n = 6; // ģ����ÿ����ע��ṹ�� ��Ŀ
	params.tree_depth = 5; // ���������

	// ����LBF landmark �����
	Ptr<FacemarkLBF> facemark = FacemarkLBF::create(params);
	facemark->setFaceDetector((FN_FaceDetector)myDetector, &face_cascade);

	// ����ģ������
	facemark->loadModel("D:/vcprojects/images/lbfmodel.yaml");
	cout << "Loaded model" << endl;

	// ��ʼ���
	printf("start to detect landmarks...\n");
	vector<Rect> faces;
	facemark->getFaces(img, faces);
	vector< vector<Point2f> > shapes;
	if (facemark->fit(img, faces, shapes))
	{
		Point eye_left; // 36th
		Point eye_right; // 45th
		for (unsigned long i = 0; i<faces.size(); i++) {
			eye_left = shapes[i][36];
			eye_right = shapes[i][45];
			line(img, eye_left, eye_right, Scalar(255, 0, 0), 2, 8, 0);
			face_alignment(img(faces[i]), eye_left, eye_right, faces[i]);

			// ����������������
			rectangle(img, faces[i], Scalar(255, 0, 0));
			// ��������68�� landmark��λ
			for (unsigned long k = 0; k<shapes[i].size(); k++)
				cv::circle(img, shapes[i][k], 2, cv::Scalar(0, 0, 255), FILLED);
		}
		namedWindow("Detected_shape");
		imshow("Detected_shape", img);
		waitKey(0);
	}
	return 0;
}

bool myDetector(InputArray image, OutputArray faces, CascadeClassifier *face_cascade)
{
	Mat gray;

	if (image.channels() > 1)
		cvtColor(image, gray, COLOR_BGR2GRAY);
	else
		gray = image.getMat().clone();

	equalizeHist(gray, gray);

	std::vector<Rect> faces_;
	face_cascade->detectMultiScale(gray, faces_, 1.1, 1, CASCADE_SCALE_IMAGE, Size(50, 50));
	Mat(faces_).copyTo(faces);
	return true;
}

void face_alignment(Mat &face, Point left, Point right, Rect roi) {
	int offsetx = roi.x;
	int offsety = roi.y;

	// ��������λ��
	int cx = roi.width / 2;
	int cy = roi.height / 2;

	// ����Ƕ�
	int dx = right.x - left.x;
	int dy = right.y - left.y;
	double degree = 180 * ((atan2(dy, dx)) / CV_PI);

	// ��ת�������
	Mat M = getRotationMatrix2D(Point2f(cx, cy), degree, 1.0);
	Point2f center(cx, cy);
	Rect bbox = RotatedRect(center, face.size(), degree).boundingRect();
	M.at<double>(0, 2) += (bbox.width / 2.0 - center.x);
	M.at<double>(1, 2) += (bbox.height / 2.0 - center.y);

	// ����
	Mat result;
	warpAffine(face, result, M, bbox.size());
	imshow("face-alignment", result);
}
