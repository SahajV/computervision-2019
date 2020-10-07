//SAHAJ VEDEREY

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>

using namespace std;
int w, h;

int **readFile(string file)
{
    ifstream infile;
    infile.open(file);
    infile.ignore(10, 'P');
    infile.ignore(1, '3');
    int width, height, intensity;
    infile >> width >> height >> intensity;
    w = width;
    h = height;
    // int imageM[width][height];

    int **imageM;
    imageM = (int **)malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++)
        imageM[i] = (int *)malloc(height * sizeof(int));

    int r, g, b;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            infile >> r >> g >> b;
            imageM[x][y] = (r + g + b) / 3;
        }
    }

    return imageM;
}

void makeImage(int **imageM, string fileN)
{
    ofstream img(fileN);
    img << "P3" << endl;
    img << w << " " << h << endl;
    img << "255" << endl;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            img << imageM[x][y];
            img << " ";
            img << imageM[x][y];
            img << " ";
            img << imageM[x][y];
            img << " ";
        }
        img << endl;
    }
}

int **sobelAndNonmaxSup(int **imageM)
{
    int gX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gY[3][3] = {{-1, 2, -1}, {0, 0, 0}, {1, 2, 1}};

    int rows = w;
    int columns = h;

    int **mag = new int *[w];
    int **angles = new int *[w];
    int **sup = new int *[w];
    for (int i = 0; i < w; ++i)
    {
        mag[i] = new int[h];
        angles[i] = new int[h];
        sup[i] = new int[h];
    }

    for (int x = 1; x < rows - 2; x++)
    {
        for (int y = 1; y < columns - 2; y++)
        {
            double s1 = 0;
            double s2 = 0;

            s1 = ((2 * imageM[x + 2][y + 1] + imageM[x + 2][y] + imageM[x + 2][y + 2]) - (2 * imageM[x][y + 1] + imageM[x][y] + imageM[x][y + 2]));
            s2 = ((2 * imageM[x + 1][y + 2] + imageM[x][y + 2] + imageM[x + 2][y + 2]) - (2 * imageM[x + 1][y] + imageM[x][y] + imageM[x + 2][y]));

            int angle = atan2(s2, s1) * 180 / 3.141592;
            double magn = sqrt(pow(s1, 2) + pow(s2, 2));
            // double finalP = 255;
            // if (magn < 80)
            //     finalP = 0;

            mag[x][y] = magn;
            angles[x][y] = angle;
        }
    }

    double theta;
    int pixel;
    sup = mag;

    for (int x = 1; x < rows - 1; x++)
    {
        for (int y = 1; y < columns - 1; y++)
        {
            pixel = mag[x][y];
            theta = angles[x][y];

            int q = 255;
            int r = 255;

            if (theta < 180)
                theta += 180;

             if (theta < 22.5 || theta > 157.5)
            {
                if ((mag[x - 1][y] > pixel) || (mag[x +  1][y] > pixel))
                    sup[x][y] = 0;
                
            }
            else if (theta >= 22.5 && theta <= 67.5)
            {
                if ((mag[x+1][y+1] > pixel) || (mag[x-1][y-1]) > pixel)
                    sup[x][y] = 0;
            }
            else if (theta > 67.5 && theta <= 112.5)
            {
                if (( mag[x][y- 1] > pixel) || ( mag[x][y +1] > pixel))
                    sup[x][y] = 0;
            }
            else 
            {
                if ((mag[x +1][y-1] > pixel) || (mag[x-1][y+1] > pixel))
                    sup[x][y] = 0;
            }
        }
    }

    return sup;
}

int **dblThreshAndHyster(int **imageM, int lower, int upper)
{
    int l = lower;
    int u = upper;

    int **fin = new int *[w];
    for (int i = 0; i < w; ++i)
    {
        fin[i] = new int[h];
    }

    for (int x = 1; x < w - 1; x++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            if (imageM[x][y] >= u)
            {
                fin[x][y] = 255;
            }
            else if (imageM[x][y] > l)
            {
                fin[x][y] = 0;
            }
            else if (imageM[x][y] <= u && imageM[x][y] >= l)
            {

                if (imageM[x + 1][y - 1] >= u || imageM[x + 1][y] >= u || imageM[x + 1][y + 1] >= u || imageM[x][y - 1] >= u || imageM[x][y + 1] >= u ||
                    imageM[x - 1][y - 1] >= u || imageM[x - 1][y] >= u || imageM[x - 1][y + 1] >= u)
                {
                    fin[x][y] = 255;
                }
                else
                {
                    fin[x][y] = 0;
                }
            }
        }
    }

    return fin;
}

int main()
{
    // cout << "Input file name with .ppm" << endl;
    // string file;
    // cin >> file;

    int **imageMG = readFile("image.ppm");
    // makeImage(imageMG, "imageg.ppm");

    int **imageMS = sobelAndNonmaxSup(imageMG);
    // makeImage(imageMS, "imagem.ppm");

    cout << "Input lower bound:" << endl;
    int upper, lower;
    cin >> lower;

    cout << "Input upper bound:" << endl;
    cin >> upper;

    int **imageMF = dblThreshAndHyster(imageMS, lower, upper);
    makeImage(imageMF, "imagef.ppm");

    return 0;
}