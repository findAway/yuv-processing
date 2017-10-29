#include <stdio.h>
#include <unistd.h>

const char* const SRC_FILE = "titles.yuv";
const char* const BACK_FILE = "background.yuv";
const char* const OUT_FILE = "out.yuv";

int main()
{
    FILE* pfSrc = fopen(SRC_FILE, "r");
    if (pfSrc == 0)
    {
        printf("open file %s error\n", SRC_FILE);
        return 1;
    }

    FILE* pfBack = fopen(BACK_FILE, "r");
    if (pfBack == 0)
    {
        printf("open file %s error\n", BACK_FILE);
        return 1;
    }

    FILE* pfOut = fopen(OUT_FILE, "w");
    if (pfOut == 0)
    {
        printf("open file %s error\n", OUT_FILE);
        return 1;
    }

    const int w = 480;
    const int h = 272;
    const int bufsize = w*h*3;

    unsigned char* const psrc = new unsigned char[bufsize];
    unsigned char* const pback = new unsigned char[bufsize];
    unsigned char* const pout = new unsigned char[bufsize];

    //将原图及背景图yuv数据全部读取出来
    fread(psrc, 1, bufsize, pfSrc);
    fread(pback, 1, bufsize, pfBack);

    unsigned char* psrcY = psrc;
    unsigned char* psrcU = psrcY + w*h;
    unsigned char* psrcV = psrcU + w*h;

    unsigned char* pbackY = pback;
    unsigned char* pbackU = pbackY + w*h;
    unsigned char* pbackV = pbackU + w*h;

    unsigned char* poutY = pout;
    unsigned char* poutU = poutY + w*h;
    unsigned char* poutV = poutU + w*h;

    //黑色的yuv分量值，将原图中的黑色变成透明
    const unsigned char Y_BLACK = 16;
    const unsigned char U_BLACK = 128;
    const unsigned char V_BLACK = 128;
    
    //开始yuv叠加处理
    for (int m = 0; m < h; m++)
    {
        for (int n = 0; n < w; n++)
        {
            int offset = m*w + n;
            unsigned char Y = psrcY[offset];
            unsigned char U = psrcU[offset];
            unsigned char V = psrcV[offset];

            if (Y == Y_BLACK && U == U_BLACK && V == V_BLACK)
            {
                poutY[offset] = pbackY[offset];
                poutU[offset] = pbackU[offset];
                poutV[offset] = pbackV[offset];
            }
            else
            {
                poutY[offset] = Y;
                poutU[offset] = U; 
                poutV[offset] = V;
            }
        }
    }

    fwrite(pout, 1, bufsize, pfOut);

    fclose(pfSrc);
    fclose(pfBack);
    fclose(pfOut);

    return 0;
}
