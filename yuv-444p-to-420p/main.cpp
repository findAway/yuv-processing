#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
using namespace std;

//const char* const SRC_FILE = "352x288-444.yuv";
const char* const OUT_FILE = "out-420.yuv";

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "#### param error ####\n");
        fprintf(stderr, "useage: thistool inputfile\n");
        return 1;
    }

    FILE* pfSrc = fopen(argv[1], "r");
    if (pfSrc == 0)
    {
        printf("open file %s error\n", argv[1]);
        return 1;
    }

    //yuv图片的宽高如下
    int w = 352; 
    int h = 288;

    cout << "set the width:";
    cin >> w;

    cout << "set the height:";
    cin >> h;

    char filename[40];
    //sprintf(filename, "%dx%d-%s", w, h, OUT_FILE);
    sprintf(filename, "%s-420p", argv[1]);

    FILE* pfOut = fopen(filename, "w");
    if (pfOut == 0)
    {
        printf("open file %s error\n", OUT_FILE);
        return 1;
    }

    const int srcsize = w*h*3;
    const int outsize = w*h*3/2;

    unsigned char* const psrc = new unsigned char[srcsize];
    unsigned char* const pout = new unsigned char[outsize];

    //将原图yuv数据全部读取出来
    int nReadSize = fread(psrc, 1, srcsize, pfSrc);
    if (nReadSize != srcsize)
    {
        fprintf(stderr, "#### the input file not match the width and height ####\n");
        return 1;
    }

    unsigned char* psrcY = psrc;
    unsigned char* psrcU = psrcY + w*h;
    unsigned char* psrcV = psrcU + w*h;

    unsigned char* poutY = pout;
    unsigned char* poutU = poutY + w*h;
    unsigned char* poutV = poutU + w*h/4;

    //yuv444p及yuv420p的Y分量完全一致，直接拷贝
    memcpy(poutY, psrcY, w*h);

    //UV分量的处理，yuv444p的基础上隔行、隔列抽取
    int offset = 0;
    for (int m = 0; m < h; m += 2)
    {
        for (int n = 0; n < w; n += 2)
        {
            int srcoffset = m*w + n;
            poutU[offset] = psrcU[srcoffset];
            poutV[offset] = psrcV[srcoffset];
            offset++;
        }
    }

    fwrite(pout, 1, outsize, pfOut);

    fclose(pfSrc);
    fclose(pfOut);

    delete[] psrc;
    delete[] pout;
    
    printf("success! out file:%s\n", filename);

    return 0;
}
