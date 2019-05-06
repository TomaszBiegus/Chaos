#include "mainwindow.h"
#include <QApplication>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#include "opencv2/opencv.hpp"

using namespace cv;

class Zespolniak{
public:
    double x;
    double y;
    Zespolniak(double a, double b);
};

Zespolniak::Zespolniak(double a, double b){
    x = a;
    y = b;
}

int drawPoint(Mat &picture, Zespolniak c);
void convertToPrint(Zespolniak &c);
double distance(Zespolniak c1, Zespolniak c2);

int w = 1000;
const int h = 1000;

int main(int, char**)
{
    Mat picture(h, w, CV_8UC3, Scalar(0,0,0));
    namedWindow("String",1);

    const String& text = "Nonlinear string";
    cv::putText(picture, text, Point(0,30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0, 255), 1);

    Zespolniak c00(0, 0);
    convertToPrint(c00);
    Point pt00(c00.x, c00.y);
    cv::line(picture, pt00, pt00, Scalar(255,0,0), 1);

    imshow("Mandelbrot", picture);

    waitKey(0);
    return 0;
}

void convertToPrint(Zespolniak &p){
    p.x = p.x + w/2;
    p.y = h - (p.y + h/2);
}

double distance(Zespolniak c1, Zespolniak c2){
    return sqrt( (c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y) );
}
