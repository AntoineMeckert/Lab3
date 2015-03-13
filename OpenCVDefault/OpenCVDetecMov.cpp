#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "Traitement.h"
using namespace std;
using namespace cv;

int m_MouseX1 = 0;
int m_MouseY1 = 0;
int m_MouseX2 = 0;
int m_MouseY2 = 0;
int m_SeuilX = 0;
bool m_MSet1 = false;
bool m_MSet2 = true;
Mat m_image;
bool DrawHisto = false;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONUP )
	{
		
			m_MouseY1 = y;
			DrawHisto = true;
	}
	if (event == EVENT_MOUSEMOVE)
	{
		m_MouseY1 = y;
	}
}

int main(int argc, char** argv)
{
	Mat imageSeuil;
	string ImageLocation;
    bool ConvertInGray = false;
	VideoCapture cam(0);
	CTraitement traitement;
	bool IsSobel = false;
	bool IsCircle = false;
	bool IsRectangle = false;
	bool IsCam = false;
	bool IsFil = false;
	string s;
	cout << "Emplacement de l'image: \r\n";
	cin >> ImageLocation;

	if(ImageLocation == "cercle.png") IsCircle = true;
	if(ImageLocation == "rectangle.png") IsRectangle = true;
	if(ImageLocation[0] == 'f' && ImageLocation[1] == 'i' && ImageLocation[2] == 'l') IsFil = true;

	while(true)
	{
		waitKey(1);

		//Convert To Gray

			if(ConvertInGray) 
				m_image = imread(ImageLocation, CV_BGR2GRAY);
			else
				m_image = imread(ImageLocation, CV_LOAD_IMAGE_COLOR);

		namedWindow("Histogramme",WINDOW_AUTOSIZE);
		imshow("Histogramme", traitement.Histogramme2Sommet(m_image));
		namedWindow("Image",WINDOW_AUTOSIZE);
		setMouseCallback("Image", CallBackFunc, NULL );
		imshow("Image", m_image);
		if(IsFil) cout << traitement.Inspection(m_image,40,76);
		
		if (IsRectangle || IsCircle)
		{
			int type = traitement.CercleRectangle(m_image);
			switch(type)
			{
			case 0: cout << "cercle";
				break;
			case 1: cout << "rectangle";
				break;		
			}
		}
		

		imshow("binarisation", traitement.Binarisation(m_image));
		imshow("lissageMoyen", traitement.LissageMoyen5x5(m_image));
		imshow("LissageMed",traitement.LissageMedian5x5(m_image));
		imshow("passhaut",traitement.FiltrePasseHaut(m_image));
		if(DrawHisto)
		{
			imshow("couleurhistogramme", traitement.HistogrammeCouleur(m_image,m_MouseY1));
		}
		
		imshow("yolo", traitement.EtirerHistogramme(m_image));
	}

	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}




