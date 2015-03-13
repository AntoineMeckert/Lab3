#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "Traitement.h"
#include <math.h>  
using namespace std;
using namespace cv;

CTraitement::CTraitement(void)
{

}


//-----------------------------------------------
//				LAB 3
//-----------------------------------------------

Mat CTraitement::Histogramme2Sommet(Mat image)
{
	int valeurs[256];
	for (int i =0; i< 256;i++)
	{
		valeurs[i] = 0;
	}

	Mat retour(255,255, CV_8UC3);
	retour = Scalar(255,255,255);
	Mat grayHisto;
	cvtColor(image,grayHisto,CV_BGR2GRAY);
	
	for(int x = 0; x < grayHisto.cols * grayHisto.rows;x++)
	{
		uchar Color = grayHisto.data[x];
		valeurs[Color] ++;
	}

	int sommet1 = 0;
	int valeurSommet = 0;
	//Sommet 1
	for (int i = 0;i< 256;i++)
	{
		if(valeurs[i] > valeurSommet)
		{
			valeurSommet = valeurs[i];
			sommet1 = i;
		}
	}

	int lastx = 0;
	int lasty = 256;

	for (int i = 0; i< 256;i++)
	{
		line(retour,Point(lastx,lasty),Point(i,255-(valeurs[i]*255/valeurs[sommet1])),Scalar(0, 0, 0),1,8,0);
		lastx = i;
		lasty = 256-(valeurs[i]*256/valeurs[sommet1]);
	}

	
	ellipse( retour,
		Point( sommet1, 256-(valeurs[sommet1]*255/valeurs[sommet1])),
		Size( 4, 4 ),
		0,
		0,
		360,
		Scalar( 0, 0, 256 ),
		1,
		8 );
	//Sommet 2
	int sommet2 = 0;
	int valeurSommet2 = 0;
	for (int i = 0;i< 256;i++)
	{
		if (valeurs[i] > 700)
		{
			string s;
		}


		int diff =(i - sommet1) * (i - sommet1);
		if(diff*valeurs[i] > valeurSommet2)
		{
			valeurSommet2 = diff*valeurs[i];
			sommet2 = i;
		}
	}

	ellipse( retour,
		Point( sommet2, 255-(valeurs[sommet2]*255/valeurs[sommet1])),
		Size( 4, 4 ),
		0,
		0,
		360,
		Scalar( 0, 0, 255 ),
		1,
		8 );
	return retour;


}

int CTraitement::Inspection(Mat img, int seuilsBorder,int seuilsFil)
{
	Mat grayimg;
	cvtColor(img,grayimg,CV_BGR2GRAY);
	int seuils = seuilsBorder;
	Mat histo = Histogramme2Sommet(img);
	uchar Color = 0;
	bool set = false;

	for (int y = 0; y< histo.rows;y++)
	{
		for (int x = 0; x< histo.cols;x++)
		{
			if (histo.at<Vec3b>(Point(x,y)) == Vec3b(0,0,0))
			{
				if(!set)
				{
					Color = x;
					set = true;
				}
						 
			}
		}
	}

	//1 = top	2 = gauche	3 = bas		4 = droite
	uchar side = 0;

	//Détermine l'orientation des fils
	for (int x = 0; x< grayimg.cols;x++)
	{
		uchar col = grayimg.at<uchar>(Point(x,0));
		if(col < Color - seuils || col > Color + seuils)
			side = 1;

		col = grayimg.at<uchar>(Point(x,grayimg.rows-1));
		if(col < Color - seuils || col > Color + seuils)
			side = 3;
	}
	for (int y = 0; y< grayimg.rows;y++)
	{
		uchar col = grayimg.at<uchar>(Point(0,y));
		if(col < Color - seuils || col > Color + seuils)
			side = 2;

		col = grayimg.at<uchar>(Point(grayimg.cols -1,y));
		if(col < Color - seuils || col > Color + seuils)
			side = 4;
	}

	int count = 0;
	bool IsOk = false;
	
	bool LookForNew = true;
	switch(side)
	{
		//Top
	case 1:
		for(int x = 0; x<grayimg.cols;x++)
		{
			if(x > 4)
			{

				if(LookForNew)
				{
					if (grayimg.at<uchar>(Point(x,50)) != 204)
					{
						LookForNew = false;
					}
				}
				else if (grayimg.at<uchar>(Point(x,50)) == 204)
				{
					count++;
					LookForNew = true;
				}
			}
			
		}
		break;
		//Gauche
	case 2:
		for(int y = 0; y<grayimg.rows;y++)
		{
			if(y >4)
			{

				if(LookForNew)
				{
					if (grayimg.at<uchar>(Point(50,y)) != 204)
					{
						LookForNew = false;
					}
				}
				else if (grayimg.at<uchar>(Point(50,y)) == 204)
				{
					count++;
					LookForNew = true;
				}
			}

		}
		break;
		//Bas
	case 3:
		for(int x = 0; x<grayimg.cols;x++)
		{
			if(x >4)
			{
				
				if(LookForNew)
				{
					uchar col = grayimg.at<uchar>(Point(x,grayimg.rows -50));
					if (col != 204)
					{
						LookForNew = false;
					}
					
				}
				else if (grayimg.at<uchar>(Point(x,grayimg.rows -50)) == 204)
				{
					count++;
					LookForNew = true;
				}

			}

		}
		break;
		//Droite
	case 4:
		for(int y = 0; y<grayimg.rows;y++)
		{
			if(y >4)
			{

				if(LookForNew)
				{
					if (grayimg.at<uchar>(Point(grayimg.cols - 50,y)) != 204)
					{
						LookForNew = false;
					}
				}
				else if (grayimg.at<uchar>(Point(grayimg.cols - 50,y)) == 204)
				{
					count++;
					LookForNew = true;
				}
			}

		}
		break;
	}

	if (count / 2 >= 4)
	{
		IsOk = true;
	}
	return count;
}

int CTraitement::CercleRectangle(Mat img)
{
	int xDroite = 0;
	int xGauche = 1000;
	int yHaut = 1000;
	int yBas = 0;

	Point Haut;
	Point Droite;
	Point Milieu;
	
	for( int y = 0; y < img.rows; y++ ) 
	{
		for( int x = 0; x < img.cols; x++ ) 
		{
			if ( img.at<Vec3b>(y,x) == cv::Vec3b(0,0,0) ) 
			{
				if(x > xDroite)
					xDroite = x;

				if(x < xGauche)
					xGauche = x;

				if(y > yBas)
					yBas = y;

				if(y < yHaut)
					yHaut = y;
			} 
		}
	}
	Haut.x = (xDroite + xGauche)/2;
	Haut.y = yHaut;

	Droite.x = xDroite;
	Droite.y = (yBas + yHaut)/2;

	Milieu.x = (xDroite + xGauche)/2;
	Milieu.y = (yBas + yHaut)/2;

	Point Hypocondriaque;
	Hypocondriaque.x = (Milieu.x + Droite.x)/2;
	Hypocondriaque.y = (Milieu.y + Haut.y)/2;

	Point cancereux;
	cancereux.x = (((Milieu.x + Droite.x)/2 + Milieu.x )/2 + Milieu.x) /2;
	cancereux.y = (((Milieu.y + Haut.y)/2 + Milieu.y )/2 + Milieu.y) /2;

	Point sida;
	sida.x = Hypocondriaque.x + cancereux.x - Milieu.x;
	sida.y = Hypocondriaque.y + cancereux.y - Milieu.y;

	if (img.at<Vec3b>(sida) == cv::Vec3b(0,0,0) )
	{
		//cercle
		return 0;
	}
	else
		return 1; //rectangle
}

Mat CTraitement::Binarisation(Mat img)
{
	int count = 0;
	Mat gray;
	cvtColor(img, gray,CV_BGR2GRAY);

	for (int y = 0; y < gray.rows;y++)
	{
		for(int x = 0; x < gray.cols;x++)
		{
			uchar Color = gray.at<uchar>(Point(x,y));
			count += Color;
		}
	}
	

	int moyenne = count / (gray.rows * gray.cols);

	for (int y = 0; y < gray.rows;y++)
	{
		for(int x = 0; x < gray.cols;x++)
		{
			if(gray.at<uchar>(Point(x,y)) <= moyenne)
				gray.at<uchar>(Point(x,y)) = 0;
			else
				gray.at<uchar>(Point(x,y)) = 255;
		}
	}
	return gray;
}

Mat CTraitement::LissageMoyen3x3(Mat img)
{
	Mat gray;
	cvtColor(img,gray,CV_BGR2GRAY);

	for (int y = 1; y < gray.rows-1;y++)
	{
		for(int x = 1; x < gray.cols-1;x++)
		{
			int moy;
			int count = 0;
			uchar Color = gray.at<uchar>(Point(x,y));
			count +=gray.at<uchar>(Point(x-1,y-1));
			count +=gray.at<uchar>(Point(x,y-1));
			count +=gray.at<uchar>(Point(x+1,y-1));
			count +=gray.at<uchar>(Point(x-1,y));
			count +=gray.at<uchar>(Point(x+1,y));
			count +=gray.at<uchar>(Point(x-1,y+1));
			count +=gray.at<uchar>(Point(x,y+1));
			count +=gray.at<uchar>(Point(x+1,y+1));

			moy = count / 8;
			count +=gray.at<uchar>(Point(x,y)) = moy;
		}
	}

	return gray;
}

Mat CTraitement::LissageMedian3x3(Mat img)
{
	Mat gray;
	cvtColor(img,gray,CV_BGR2GRAY);
	uchar Array[8];

	for (int y = 1; y < gray.rows-1;y++)
	{
		for(int x = 1; x < gray.cols-1;x++)
		{
			int med;
			int count = 0;
			uchar Color = gray.at<uchar>(Point(x,y));
			Array[0]=gray.at<uchar>(Point(x-1,y-1));
			Array[1]=gray.at<uchar>(Point(x,y-1));
			Array[2]=gray.at<uchar>(Point(x+1,y-1));
			Array[3]=gray.at<uchar>(Point(x-1,y));
			Array[4]=gray.at<uchar>(Point(x+1,y));
			Array[5]=gray.at<uchar>(Point(x-1,y+1));
			Array[6]=gray.at<uchar>(Point(x,y+1));
			Array[7]=gray.at<uchar>(Point(x+1,y+1));

			sort(Array, Array + 8);

			med = (Array[3] + Array[4]) / 2;
			gray.at<uchar>(Point(x,y)) = med;
		}
	}

	return gray;
}

Mat CTraitement::LissageMoyen5x5(Mat img)
{
	Mat gray;
	cvtColor(img,gray,CV_BGR2GRAY);

	for (int y = 2; y < gray.rows-2;y++)
	{
		for(int x = 2; x < gray.cols-2;x++)
		{
			int moy;
			int count = 0;
			uchar Color = gray.at<uchar>(Point(x,y));
			count +=gray.at<uchar>(Point(x-1,y-1));
			count +=gray.at<uchar>(Point(x,y-1));
			count +=gray.at<uchar>(Point(x+1,y-1));
			count +=gray.at<uchar>(Point(x-1,y));
			count +=gray.at<uchar>(Point(x+1,y));
			count +=gray.at<uchar>(Point(x-1,y+1));
			count +=gray.at<uchar>(Point(x,y+1));
			count +=gray.at<uchar>(Point(x+1,y+1));

			count +=gray.at<uchar>(Point(x-2,y-2));
			count +=gray.at<uchar>(Point(x-1,y-2));
			count +=gray.at<uchar>(Point(x,y-2));
			count +=gray.at<uchar>(Point(x+1,y-2));
			count +=gray.at<uchar>(Point(x+2,y-2));


			count +=gray.at<uchar>(Point(x-2,y-1));
			count +=gray.at<uchar>(Point(x-2,y));
			count +=gray.at<uchar>(Point(x-2,y+1));

			count +=gray.at<uchar>(Point(x+2,y-1));
			count +=gray.at<uchar>(Point(x+2,y));
			count +=gray.at<uchar>(Point(x+2,y+1));

			count +=gray.at<uchar>(Point(x-2,y+2));
			count +=gray.at<uchar>(Point(x-1,y+2));
			count +=gray.at<uchar>(Point(x,y+2));
			count +=gray.at<uchar>(Point(x+1,y+2));
			count +=gray.at<uchar>(Point(x+2,y+2));

			count += gray.at<uchar>(Point(x,y));

			moy = count / 25;
			gray.at<uchar>(Point(x,y)) = moy;
		}
	}


	return gray;
}

Mat CTraitement::LissageMedian5x5(Mat img)
{
	Mat gray;
	cvtColor(img,gray,CV_BGR2GRAY);
	uchar Array[25];

	for (int y = 2; y < gray.rows-2;y++)
	{
		for(int x = 2; x < gray.cols-2;x++)
		{
			int med;
			int count = 0;
			uchar Color = gray.at<uchar>(Point(x,y));
			Array[0]=gray.at<uchar>(Point(x-1,y-1));
			Array[1]=gray.at<uchar>(Point(x,y-1));
			Array[2]=gray.at<uchar>(Point(x+1,y-1));
			Array[3]=gray.at<uchar>(Point(x-1,y));
			Array[4]=gray.at<uchar>(Point(x+1,y));
			Array[5]=gray.at<uchar>(Point(x-1,y+1));
			Array[6]=gray.at<uchar>(Point(x,y+1));
			Array[7]=gray.at<uchar>(Point(x+1,y+1));

			Array[8]=gray.at<uchar>(Point(x-2,y-2));
			Array[9]=gray.at<uchar>(Point(x-1,y-2));
			Array[10]=gray.at<uchar>(Point(x,y-2));
			Array[11]=gray.at<uchar>(Point(x+1,y-2));
			Array[12]=gray.at<uchar>(Point(x+2,y-2));


			Array[13]=gray.at<uchar>(Point(x-2,y-1));
			Array[14]=gray.at<uchar>(Point(x-2,y));
			Array[15]=gray.at<uchar>(Point(x-2,y+1));

			Array[16]=gray.at<uchar>(Point(x+2,y-1));
			Array[17]=gray.at<uchar>(Point(x+2,y));
			Array[18]=gray.at<uchar>(Point(x+2,y+1));

			Array[19]=gray.at<uchar>(Point(x-2,y+2));
			Array[20]=gray.at<uchar>(Point(x-1,y+2));
			Array[21]=gray.at<uchar>(Point(x,y+2));
			Array[22]=gray.at<uchar>(Point(x+1,y+2));
			Array[23]=gray.at<uchar>(Point(x+2,y+2));
			Array[24]=gray.at<uchar>(Point(x,y));

			sort(Array, Array + 25);

			med = Array[12];
			gray.at<uchar>(Point(x,y)) = med;
		}
	}

	return gray;
}

Mat CTraitement::FiltrePasseHaut(Mat img)
{
	Mat lisse = CTraitement::LissageMedian5x5(img);
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	Mat res = gray - lisse;
	return res;
}

Mat CTraitement::HistogrammeCouleur(Mat img, int y)
{
	Mat hist(255,img.cols,CV_8UC3,Scalar(255,255,255));
	Point prevB;
	Point prevR;
	Point prevG;

	prevB.x = 0;
	prevB.y = 255;
	prevR.x = 0;
	prevR.y = 255;
	prevG.x = 0;
	prevG.y = 255;
		
	for(int x = 0; x < img.cols;x++)
	{
		//BGR
		Vec3b data = img.at<Vec3b>(Point(x,y));

		line(hist,prevB,Point(x,255-data[0]),Scalar(255,0,0),1,8,0);
		line(hist,prevG,Point(x,255-data[1]),Scalar(0,255,0),1,8,0);
		line(hist,prevR,Point(x,255-data[2]),Scalar(0,0,255),1,8,0);

		prevB.x = x;
		prevB.y = 255-data[0];
		prevG.x = x;
		prevG.y = 255-data[1];
		prevR.x = x;
		prevR.y = 255-data[2];
	}

	return hist;
}

Mat CTraitement::EtirerHistogramme(Mat img)
{
	int valeurs[256];
	for (int i =0; i< 256;i++)
	{
		valeurs[i] = 0;
	}

	Mat retour(255,255, CV_8UC3);
	retour = Scalar(255,255,255);
	Mat grayHisto;
	cvtColor(img,grayHisto,CV_BGR2GRAY);

	for(int x = 0; x < grayHisto.cols * grayHisto.rows;x++)
	{
		uchar Color = grayHisto.data[x];
		valeurs[Color] ++;
	}

	int sommet1 = 0;
	int valeurSommet = 0;
	//Sommet 1
	for (int i = 0;i< 256;i++)
	{
		if(valeurs[i] > valeurSommet)
		{
			valeurSommet = valeurs[i];
			sommet1 = i;
		}
	}

	

	

	//Sommet 2
	int sommet2 = 0;
	int valeurSommet2 = 0;
	for (int i = 0;i< 256;i++)
	{
		if (valeurs[i] > 700)
		{
			string s;
		}


		int diff =(i - sommet1) * (i - sommet1);
		if(diff*valeurs[i] > valeurSommet2)
		{
			valeurSommet2 = diff*valeurs[i];
			sommet2 = i;
		}
	}

	int diff = abs(sommet2 - sommet1);

	int lastx = 0;
	int lasty = 256;


	int y = sommet2;
	int ind = sommet2;
	if(sommet1 < sommet2)
	{ind = sommet1; y = sommet1;}

	


	for (int i = 0; i<= diff;i++)
	{

		line(retour,Point(lastx,lasty),Point(i * (255/ diff),256-(valeurs[ind]*256/valeurs[sommet1])),Scalar(0, 0, 0),1,8,0);
		lastx = i * (255/ diff);
		lasty = 256-(valeurs[ind]*256/valeurs[sommet1]);
		ind ++;
	}
	return retour;
	
}

//Mat CTraitement::Erosion(Mat img, int seuil)
//{
//	Mat bin = Binarisation(img);
//
//	for (int y = 1; y < img.rows - 1;y++)
//	{
//		for (int x = 1; x < img.rows - 1;x++)
//		{
//			if(bin.at<uchar>(Point(x,y)))
//			{
//
//			}
//		}
//	}
//}

CTraitement::~CTraitement(void)
{
}
