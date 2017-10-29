#include <stdio.h>
#include <unistd.h>
#include <string.h>

const char* const SRC_FILE = "titles.yuv";
const char* const OUT_FILE = "out.yuv";

int main()
{
    FILE* pfSrc = fopen(SRC_FILE, "r");
    if (pfSrc == 0)
    {
        printf("open file %s error\n", SRC_FILE);
        return 1;
    }

    FILE* pfOut = fopen(OUT_FILE, "w");
    if (pfOut == 0)
    {
        printf("open file %s error\n", OUT_FILE);
        return 1;
    }

    //yuv图片的宽高如下
    const int w = 480;
    const int h = 272;
    const int srcsize = w*h*3;
    const int outsize = w*h*3/2;

    unsigned char* const psrc = new unsigned char[srcsize];
    unsigned char* const pout = new unsigned char[outsize];

    //将原图yuv数据全部读取出来
    fread(psrc, 1, srcsize, pfSrc);

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

    return 0;
}
