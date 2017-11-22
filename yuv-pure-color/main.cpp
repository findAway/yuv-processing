#include <iostream>
#include <stdio.h>
#include <string.h>

const char* const outfile = "444.yuv";

int main()
{
    using namespace std;
    int w = 352;
    int h = 288;

    unsigned int Y = 0;
    unsigned int U = 0;
    unsigned int V = 0;

    cout << "set the width:";
    cin >> w;

    cout << "set the height:";
    cin >> h;

    cout << "set the value of y (from 0 to 255):";
    cin >> Y;

    cout << "set the value of u (from 0 to 255):";
    cin >> U;

    cout << "set the value of v (from 0 to 255):";
    cin >> V;

    cout << "y is " << Y << endl;
    cout << "u is " << U << endl;
    cout << "v is " << V << endl;

    const int bufsize = w*h*3;
    unsigned char* const pyuv = new unsigned char[bufsize];

    unsigned char* py = pyuv;
    unsigned char* pu = py + w*h;
    unsigned char* pv = pu + w*h;

    memset(py, (unsigned char)Y, w*h);
    memset(pu, (unsigned char)U, w*h);
    memset(pv, (unsigned char)V, w*h);

    char filename[40];
    sprintf(filename, "%dx%d-%s", w, h, outfile);

    FILE* pfout = fopen(filename, "w");
    if (0 == pfout)
    {
        cout << "open file error: " << filename << endl;
        return 1;
    }

    fwrite(pyuv, 1, bufsize, pfout);

    fclose(pfout);
    delete[] pyuv;

    cout << "finish... out file is: " << filename << endl;

    return 0;
}

