#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QLabel l;
   QPicture pi;
   QPainter p(&pi);

   p.setRenderHint(QPainter::Antialiasing);
   //p.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));


   double x = 0.5;
   int yPlot = 0;
   for(double i=2.5; i<4; i+=0.002){
    double y = i * (1- x) * x;
    for(int j=0; j<1000; j++){

        y = i* (1 - y) * y;
        if(j > 900){
            //przeksztalcenie
            yPlot = 500 - y*500;
            cout << yPlot << std::endl;
            //p.drawPoint(10, (int)yPlot);

            p.drawPoint((int)(i*1000-2500), (int)yPlot);
        }

    }
   }

   p.drawLine(0, 0, 1000, 500);
   p.end(); // Don't forget this line!

   l.setPicture(pi);
   l.show();
   return a.exec();
}
