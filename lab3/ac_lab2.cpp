//Aadith Charugundla
//Computer Vision Period 3 Lab 1


#include <iostream> //input output stream
#include <fstream> //filestream
#include <cstdlib> //C Standard Genral Library
#include <cmath>
#include <time.h>
#include <algorithm> 
#include <bits/stdc++.h> 
#define R 800 //rows and columns
#define C 800
#define N 20
using namespace std;

string pixels[800][800]; //have to make it global because it didn't let me make an 800 by 800 2d array in main


class point
{
   public: 
   
   double x; //x-coord 
   double y; //y-coord  
   
   int px; //ppm x coord
   int py; //ppm y coord
   
   /*void point() //default constructor
   {
      x = (rand() * 1.0 / RAND_MAX); //double between 0 and 1
      y = (rand() * 1.0 / RAND_MAX); 
      px = (int)(x * 800);
      py = (int)(y * 800); 
   } 
   
   void point(double xc, double yc) //parametrized constructor
   {
      x = xc; 
      y = yc;
      px = (int)(x * 800); 
      py = (int)(y * 800); 
      
   }*/
   
   
   void setx(double i)
   {
       x = i; 
       px = (int)(x * 800); 
   }
   
   double getx()
   {
       return x; 
   }
   
   void sety(double j)
   {
       y = j; 
       py = (int)(y * 800); 
   }
   
   double gety()
   {
       return y; 
   }
   
   /*int compareTo(point p)
   {
       if(this.getx() > p.getx())
            return 1; 
       else if(this.getx() < p.getx())
           return -1; 
       return 0; 
   }*/
   
};

//point min[2];

double distance(point p1, point p2) //Calculate distance between 2 points
{
    double dy = p2.y - p1.y; 
    double dx = p2.x - p1.x;
    
    double distance = sqrt((pow(dy, 2) + pow(dx, 2)));  
    return distance; 
}


point* smallest(point p1[],int p1size, point p2[], int p2size)
{
     point min[2]; 
     
     
     printf("\n STARTING DISTANCE CALCULATION: \n"); 
     for(int x = 0; x < p1size; x++)
     {
         printf("%lf , %lf \n", p1[x].getx(), p1[x].gety()); 
     }
     for(int y = 0; y < p2size; y++)
     {
         printf("%lf , %lf \n", p2[y].getx(), p2[y].gety()); 
     }
     double d1 = 1.00; 
     double d2 = 1.00; 
     
     if(p1size == 2)
     {
         d1 = distance(p1[0], p1[1]);
         min[0] = p1[0]; 
         min[1] = p1[1]; 
     }
     else
     {         
         double dis1 = distance(p1[0], p1[1]);
         double dis2 = distance(p1[1], p1[2]);
         double dis3 = distance(p1[0], p1[2]);
         d1 = std::min(std::min(dis1, dis2), dis3);
         if(d1 == dis1)
         {
             min[0] = p1[0]; 
             min[1] = p1[1]; 
         }
         else if(d1 == dis2)
         {
             min[0] = p1[1]; 
             min[1] = p1[2]; 
         }
         else
         {
             min[0] = p1[0]; 
             min[1] = p1[2]; 
         }
             

     }
     
     if(p2size == 2)
     {
         d2 = distance(p2[0], p2[1]); 
         if(d2 < d1)
         {
             min[0] = p2[0]; 
             min[1] = p2[1]; 
         }
     }
     else
     {
         double dis3 = distance(p2[0], p2[1]);
         double dis4 = distance(p2[1], p2[2]);
         double dis5 = distance(p2[0], p2[2]);
         d2 = std::min(std::min(dis3, dis4), dis5);
         if(d1 == dis3)
         {
             min[0] = p2[0]; 
             min[1] = p2[1]; 
         }
         else if(d1 == dis4)
         {
             min[0] = p2[1]; 
             min[1] = p2[2]; 
         }
         else
         {
             min[0] = p2[0]; 
             min[1] = p2[2]; 
         }
          
         
     }
     
     point *minp = min; 
     printf("the two points: %lf , %lf and %lf , %lf \n", min[0].getx(), min[0].gety(), min[1].getx(), min[1].gety());
     return minp  ; 

}

/*point * smallerpair(point *p1, point *p2)
{
   printf("\n \n SMALLEST DISTNCE AMONG %lf, %lf  %lf, %lf AND %lf, %lf %lf, %lf: \n \n", p1[0].getx(), p1[0].gety(), p1[1].getx(), p1[1].gety(), p2[0].getx(), p2[0].gety(), p2[1].getx(), p2[1].gety());
   printf("DISTANCES: \n"); 
   printf("%lf AND %lf \n", distance(p1[0], p1[1]), distance(p2[0], p2[1]));
   if(distance(p1[0], p1[1]) < distance(p2[0], p2[1]))
   {
      printf("%lf, %lf \n", p1[0].getx(), p1[0].gety()); 
      return p1; 
   }
   printf("%lf, %lf \n \n", p2[0].getx(), p2[0]); 
   return p2; 
}*/

point* recur(point *p, int size)
{
    
     printf("POINTER TO THE ARRAY: \n"); 
     
     for(int z = 0; z < size; z++)
         printf("%lf \n", p[z].x); 
    
     point p1[size/2];
     point p2[size - size/2];
     
     int p1size = sizeof(p1)/sizeof(p1[0]);
     int p2size = sizeof(p2)/sizeof(p2[0]);
     printf("SIZE OF p1: %d \n", p1size); 
     printf("SIZE OF p2: %d \n", p2size); 
     
     point *p1p; 
     point *p2p; 
      
     int p2counter = 0; 
     
     for(int x = 0; x < size/2; x++)
         {
             p1[x] = p[x];
             printf("%lf \n", p1[x].getx()); 
         }
         
         for(int y = size/2; y < (size); y++)
         {
             p2[p2counter] = p[y]; 
             printf("%lf \n", p2[p2counter].getx());
             p2counter++; 
         }
     
              
         p1p = p1; 
         p2p = p2; 
     
     if(sizeof(p1)/sizeof(p1[0]) == 2 || sizeof(p1)/sizeof(p1[0]) == 3)
     {
         return smallest(p1, p1size, p2, p2size); 
     }
     
     else
     {
         printf(";AOSJEF;LKAJSEF;LKASJEF;LKAJSEF;LKAJSD;LGKJSADF;LGKNAS;LGIUJEASRIG;JAE;LKJAWE;LGLKEFA;LEWKGJA;LSKGJAL;KSDGJ;LKSDJG;L \n");
         
         point *com1 = recur(p1p, p1size); 
         point como[2]; 
         
         for(int c1 = 0; c1 < 2; c1++)
         {
            como[c1] = com1[c1];
            printf("como: %lf %lf \n", como[c1].x, como[c1].y);  
         }
         
         point *com2 = recur(p2p, p2size); 
         
         
         point comt[2]; 
         
         
         for(int c2 = 0; c2 < 2; c2++) 
         {
            comt[c2] = com2[c2]; 
            printf("comt: %lf %lf \n", comt[c2].x, comt[c2].y); 
         }
         
         return smallest(como, 2, comt, 2);  
          
  
     }
     
     
     
}



int main()
{
    ofstream outFile("points.ppm"); //makes an output file stream named outFile
    outFile << "P3  800 800 1" << endl; //Writing to the file
    srand(time(NULL)); //random number seed changes based on time
    
    //fills the entire array with "1 1 1" (white)
   for(int r = 0; r < R; r++)
      for(int c = 0; c < C; c++)
         pixels[r][c] = "1 1 1"; 
    
    point *p; 
    point ps[N];
   p = ps; 
    for(int x = 0; x < N; x++)
    {
        ps[x].setx(rand() * 1.0 / RAND_MAX);
        ps[x].sety(rand() * 1.0 / RAND_MAX); 
    }
      
    std::sort(ps, ps + N, [](point const & a, point const & b) -> bool {return a.x < b.x; } );
    
    for(int s = 0; s < N; s++)
        printf("%lf \n", ps[s].x); 
    
    for(int a = 0; a < N; a++)
        pixels[ps[a].px][ps[a].py] = "0 0 0";
    
    point *simp = recur(p, sizeof(ps)/sizeof(ps[0]));
    //point sim[2];  
    //point *simpp = simp; 
    printf("\n RECURSION HAS FINISHED! \n"); 
      
   /* 
    sim[0] = simpp[0];
      printf("%lf , %lf \n", sim[0].x, sim[0].y);
      
    sim[1] = simp[1];
      printf("%lf , %lf \n", sim[1].x, sim[1].y); 
    
    
     */
    
    /*for(int mi = 0; mi < 2; mi++)
    {
      //sim[mi] = simp[0];
      printf("%lf , %lf \n", sim[mi].x, sim[mi].y);
      //printf("%d, %d \n", sim[mi].px, sim[mi].py);     
    }*/
    
    //pixels[sim[0].px][sim[0].py] = "1 0 0"; 
    //pixels[sim[1].px][sim[1].py] = "1 0 0"; 
    
    
    /*double table[40][40];   
    for(int i = 0; i < 40; i++)
        for(int j = 0; j < 40; j++)
            table[i][j] = distance(ps[i], ps[j]); 
            
        
   point tp1; 
   point tp2; 
   double min = 1.414;     
   for(int m = 0; m < 40; m++)
       for(int n = 0; n < 40; n++)
           if(m != n && table[m][n] < min)
           {
               min = table[m][n];
               tp1 = ps[m]; 
               tp2 = ps[n]; 
               n++; 
           }
   
   
   pixels[tp1.px][tp1.py] = "1 0 0"; 
   pixels[tp2.px][tp2.py] = "1 0 0"; 
   */
        
        
   //MAKING THE PPM FILE
   for(int c = 0; c < C - 1; c++)
   {
      for(int r = 0; r < R - 1; r++)
         outFile << pixels[r][c] + " "; 
      outFile << pixels[R - 1][c]<< endl; 
      
   }
   outFile << pixels[R - 1][C - 1];     
   outFile.close(); 
   return 0; 
// whats popping
}