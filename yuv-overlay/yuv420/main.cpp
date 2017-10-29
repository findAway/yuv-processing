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
    const int bufsize = w*h*3/2;

    unsigned char* const psrc = new unsigned char[bufsize];
    unsigned char* const pback = new unsigned char[bufsize];
    unsigned char* const pout = new unsigned char[bufsize];

    //将原图及背景图yuv数据全部读取出来
    fread(psrc, 1, bufsize, pfSrc);
    fread(pback, 1, bufsize, pfBack);

    unsigned char* psrcY = psrc;
    unsigned char* psrcU = psrcY + w*h;
    unsigned char* psrcV = psrcU + w*h/4;

    unsigned char* pbackY = pback;
    unsigned char* pbackU = pbackY + w*h;
    unsigned char* pbackV = pbackU + w*h/4;

    unsigned char* poutY = pout;
    unsigned char* poutU = poutY + w*h;
    unsigned char* poutV = poutU + w*h/4;

    //黑色的yuv分量值，将原图中的黑色变成透明
    const unsigned char Y_BLACK = 16;
    const unsigned char U_BLACK = 128;
    const unsigned char V_BLACK = 128;
    
    //开始yuv叠加处理
    //yuv420为4个y分量公用一组uv分量
    for (int m = 0; m < h/2; m++)
    {
        for (int n = 0; n < w/2; n++)
        {
            //uv的偏移
            int uvoff = m*w/2 + n;
            unsigned char U = psrcU[uvoff];
            unsigned char V = psrcV[uvoff];

            //uv对应的4个y分量的偏移
            int yoff0 = w*2*m + n*2;
            int yoff1 = yoff0 + 1;
            int yoff2 = yoff0 + w;
            int yoff3 = yoff2 + 1;
            unsigned char Y0 = psrcY[yoff0];
            unsigned char Y1 = psrcY[yoff1];
            unsigned char Y2 = psrcY[yoff2];
            unsigned char Y3 = psrcY[yoff3];

            //4个像素点都为黑色才全透，否则不透
            if (U == U_BLACK && V == V_BLACK && Y0 == Y_BLACK && Y1 == Y_BLACK && Y2 == Y_BLACK && Y3 == Y_BLACK)
            {
            #if 1
                poutU[uvoff] = pbackU[uvoff];
                poutV[uvoff] = pbackV[uvoff];
                poutY[yoff0] = pbackY[yoff0];
                poutY[yoff1] = pbackY[yoff1];
                poutY[yoff2] = pbackY[yoff2];
                poutY[yoff3] = pbackY[yoff3];
            #else
                poutU[uvoff] = ((unsigned int)U + (unsigned int)pbackU[uvoff])/2;
                poutV[uvoff] = ((unsigned int)V + (unsigned int)pbackV[uvoff])/2;
                poutY[yoff0] = ((unsigned int)Y0 + (unsigned int)pbackY[yoff0])/2;
                poutY[yoff1] = ((unsigned int)Y1 + (unsigned int)pbackY[yoff1])/2;
                poutY[yoff2] = ((unsigned int)Y2 + (unsigned int)pbackY[yoff2])/2;
                poutY[yoff3] = ((unsigned int)Y3 + (unsigned int)pbackY[yoff3])/2;
            #endif
            }
            else
            {
                poutU[uvoff] = U;
                poutV[uvoff] = V;
                poutY[yoff0] = Y0;
                poutY[yoff1] = Y1;
                poutY[yoff2] = Y2;
                poutY[yoff3] = Y3;
            }
        }
    }

    fwrite(pout, 1, bufsize, pfOut);

    fclose(pfSrc);
    fclose(pfBack);
    fclose(pfOut);

    return 0;
}
