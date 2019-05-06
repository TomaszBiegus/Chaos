#include "mainwindow.h"
#include <QApplication>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class Zespolniak{
public:
    double x;
    double y;
    Zespolniak(double a, double b);
    Zespolniak(const Zespolniak& z);
    void draw(Mat &picture);
};

Zespolniak::Zespolniak(const Zespolniak &z){
    x = z.x;
    y = z.y;
}

Zespolniak::Zespolniak(double a, double b){
    x = a;
    y = b;
}

int drawPoint(Mat &picture, Zespolniak c);
void convertToPrint(Zespolniak &c);
double distance(Zespolniak c1, Zespolniak c2);

int w = 1000;
const int h = 1000;

void Zespolniak::draw(Mat &picture){
    Zespolniak z(x, y);
    convertToPrint(z);
    Point pt(z.x, z.y);
    cv::line(picture, pt, pt, Scalar(255,0,0), 5);
}

double wspSprezystosci = 0.1;
double dT = 1; //nie zmieniac a wsp sprez od 0.1 do 1 daje dobre efekty
double energiaOld;

double obliczEnergie(const vector<Zespolniak> &punkty, const vector<double> &pedy){
    double energiaPotencjalnaSprezystosci = 0;
    double sumaEPot = 0;
    for(int i=1; i<punkty.size(); i++){
        energiaPotencjalnaSprezystosci = wspSprezystosci * abs(punkty[i].y - punkty[i-1].y) * 0.5 * abs(punkty[i].y - punkty[i-1].y);
        sumaEPot += energiaPotencjalnaSprezystosci;
    }
    double energiaKinetyczna = 0;
    double sumaEKin = 0;
    for(int i=1; i<pedy.size(); i++){
        energiaKinetyczna = pedy[i] * pedy[i];
        sumaEKin += energiaKinetyczna;
    }
    return sumaEPot + sumaEKin;
}

void obliczPolozeniaPunktow(const vector<Zespolniak> &punktyOld, const vector<double> &silyOld, const vector<double> &pedyOld, vector<Zespolniak> &punktyNew, vector<double> &silyNew, vector<double> &pedyNew){
    double sila, dP, ped, dS, polozenie;

    pedyNew.push_back(0);
    punktyNew.push_back(Zespolniak(punktyOld[0].x, punktyOld[0].y));
    silyNew.push_back(0);

    for(int i=1; i<punktyOld.size()-1; i++){
        sila = wspSprezystosci * ( (punktyOld[i-1].y - punktyOld[i].y) + (punktyOld[i+1].y - punktyOld[i].y) );
        //sila = wspSprezystosci * -punktyOld[i].y;
        //element nieliniowy
        sila += 0.0000000001 * sila * sila;
        silyNew.push_back(sila);

        dP = silyOld[i] * dT;
        ped = pedyOld[i] + dP;
        pedyNew.push_back(ped);

        dS = ped * dT;
        polozenie = punktyOld[i].y + dS;
        punktyNew.push_back(Zespolniak(punktyOld[i].x, polozenie));
    }

    pedyNew.push_back(0);
    punktyNew.push_back(Zespolniak(punktyOld[punktyOld.size()-1].x, punktyOld[punktyOld.size()-1].y));
    silyNew.push_back(0);

    //double energiaOld = obliczEnergie(punktyOld, pedyOld);
    double energiaNew = obliczEnergie(punktyNew, pedyNew);

    for(int i=0; i<punktyNew.size(); i++){
        punktyNew[i].y /= energiaNew;
        punktyNew[i].y *= energiaOld;

        pedyNew[i] /= energiaNew;
        pedyNew[i] *= energiaOld;
    }


}

int main(int, char**)
{
    namedWindow("String",1);
    const String& text = "Nonlinear string";

    vector<Zespolniak> punktyOld;
    vector<double> silyOld;
    vector<double> pedyOld;

    int krok = 10;
    int wychylenie = 50;
    for(int i=-w/2; i<=w/2; i+=krok){
        /*if(i==0){
            punktyOld.push_back(Zespolniak(i, -1000));
            silyOld.push_back(0);
            pedyOld.push_back(0);
        } else {
            punktyOld.push_back(Zespolniak(i, 0));
            silyOld.push_back(0);
            pedyOld.push_back(0);
        }*/
        if(i>=0){
            if(i % (2*krok) == 0){
                punktyOld.push_back(Zespolniak(i, 0));
            }else if(i % (4*krok) == (1*krok)){
                punktyOld.push_back(Zespolniak(i, wychylenie));
            }else {
                punktyOld.push_back(Zespolniak(i, -wychylenie));
            }
        } else {
            if(i % (2*krok) == 0){
                punktyOld.push_back(Zespolniak(i, 0));
            }else if(i % (4*krok) == -(3*krok)){
                punktyOld.push_back(Zespolniak(i, wychylenie));
            }else {
                punktyOld.push_back(Zespolniak(i, -wychylenie));
            }
        }


        silyOld.push_back(0);
        pedyOld.push_back(0);
        /*double y = abs(i) - 500;
        punktyOld.push_back(Zespolniak(i, y));
        silyOld.push_back(0);
        pedyOld.push_back(0);*/
    }

    energiaOld = obliczEnergie(punktyOld, pedyOld);

    vector<Zespolniak> punktyNew;
    vector<double> silyNew;
    vector<double> pedyNew;

    while(true){
        Mat picture(h, w, CV_8UC3, Scalar(230,255,230));
        cv::putText(picture, text, Point(0,15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,0,0), 1);

        //oblicz punkty new na podstawiep unktow old
        obliczPolozeniaPunktow(punktyOld, silyOld, pedyOld, punktyNew, silyNew, pedyNew);

        for(size_t i=0; i<punktyNew.size(); i++){
            punktyNew[i].draw(picture);
        }

        imshow("String", picture);

        punktyOld = punktyNew;
        silyOld = silyNew;
        pedyOld = pedyNew;

        punktyNew.clear();
        silyNew.clear();
        pedyNew.clear();

        if(waitKey(30)>=0) break;
    }
    return 0;
}

void convertToPrint(Zespolniak &p){
    p.x = p.x + w/2;
    p.y = h - (p.y + h/2);
}

double distance(Zespolniak c1, Zespolniak c2){
    return sqrt( (c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y) );
}
