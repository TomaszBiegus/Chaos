#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QLabel l;
   QPicture pi;
   QPainter p(&pi);

   p.setRenderHint(QPainter::Antialiasing);
   //p.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));

   srand( time( NULL ) );

   double x0 = 500, y0 = 250;
   //double x1, x2;
   double R = 10;
   //double r;
   for(int i=0; i<10000; i++){
        double x = 1000000, y = 1000000;
        while(sqrt(pow((x), 2) + pow((y), 2)) > R){
            x = (double)(rand() % (int)R);
            y = (double)(rand() % (int)R);
            //cout << x << ", " << y << ", " <<sqrt(pow((x-x0), 2) + pow((y-y0), 2)) << std::endl;
        }
        if(rand() % 2)
            x = -x;
        if(rand() % 2)
            y = -y;
       // cout << x << ", " << y << std::endl;
        for(int j=0; j<2; j++){
            double xtmp = x;
            x = y + 1 - 1.4 * xtmp*xtmp;
            y = 0.3 * xtmp;
        }
        x += x0;
        y += y0;
        //cout << x << ", iteracja" << y << std::endl;
        //p.drawPoint(10, (int)yPlot);
        if( (x > 1000) || (x < 0) || (y>500) || (y<0) )
            std::cout << "poza zakresem: " << x << ", " << y << std::endl;
        else
            p.drawPoint((int)x, (int)y);
   }


   p.drawLine(0, 0, 1000, 500);
   p.end(); // Don't forget this line!

   l.setPicture(pi);
   l.show();
   return a.exec();
}
