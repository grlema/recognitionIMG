#pragma once
#include <fstream>
#include <string>
#include "vector"
#include <iostream>
using namespace std;

//txt�ı���·��
#define txtRows			9//txt�ı�����
#define txtCols			2//txt�ı�����
struct FileData
{
	vector<string> name;//��1��
	int data[txtRows][txtCols - 1];//����txt�ı������ݣ���2�п�ʼ��
};

FileData loadFileData(char* path);
