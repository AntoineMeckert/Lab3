#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#pragma once

class CTraitement
{
public:
	CTraitement(void);

	Mat Histogramme2Sommet(Mat image);
	int Inspection(Mat img, int seuilsBorder,int seuilsFil);
	int CercleRectangle(Mat img);
	Mat Binarisation(Mat img);
	Mat LissageMoyen3x3(Mat img);
	Mat LissageMedian3x3(Mat img);
	Mat LissageMoyen5x5(Mat img);
	Mat LissageMedian5x5(Mat img);
	Mat FiltrePasseHaut(Mat img);
	Mat HistogrammeCouleur(Mat img, int y);
	Mat EtirerHistogramme(Mat img);
	Mat Erosion(Mat img, int seuil);
	Mat Dillatation(Mat img, int seuil);
	Mat Fermeture(Mat img, int seuil);
	Mat Ouverture(Mat img, int seuil);

	~CTraitement(void);
};

