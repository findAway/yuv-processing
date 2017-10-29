#include <stdio.h>
#include <unistd.h>
#include <string.h>

const char* const SRC_FILE = "titles_444.yuv";           //yuv420p
const char* const BACK_FILE = "background_420.yuv";      //yuv420p
const char* const OUT_FILE = "out_420.yuv";              //yuv420p

void yuv420p_to_444p(int w, int h, const unsigned char* pin, unsigned char* pout)
{
    const unsigned char* psrcY = pin;
    const unsigned char* psrcU = psrcY + w*h;
    const unsigned char* psrcV = psrcU + w*h/4;

    unsigned char* poutY = pout;
    unsigned char* poutU = poutY + w*h;
    unsigned char* poutV = poutU + w*h;

    memcpy(poutY, psrcY, w*h);

    for (int m = 0; m < h; m++)
    {
        for (int n = 0; n < w; n++)
        {
            int offset_out = m*w + n;
            int offset_in = m/2*(w/2) + n/2;

            poutU[offset_out] = psrcU[offset_in];
            poutV[offset_out] = psrcV[offset_in];

            //printf("[%s] offset_out:%d, offset_in:%d\n", __FUNCTION__, offset_out, offset_in);
        }
    }
}

void yuv444p_to_420p(int w, int h, const unsigned char* pin, unsigned char* pout)
{
    const unsigned char* psrcY = pin;
    const unsigned char* psrcU = psrcY + w*h;
    const unsigned char* psrcV = psrcU + w*h;

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
}

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
    const int bufsize_444 = w*h*3;
    const int bufsize_420 = w*h*3/2;

    unsigned char* const psrc = new unsigned char[bufsize_444];
    unsigned char* const pback = new unsigned char[bufsize_420];
    unsigned char* const pback2 = new unsigned char[bufsize_444];
    unsigned char* const pout = new unsigned char[bufsize_444];
    unsigned char* const pout2 = new unsigned char[bufsize_420];

    //将原图及背景图yuv数据全部读取出来
    fread(psrc, 1, bufsize_444, pfSrc);
    fread(pback, 1, bufsize_420, pfBack);

    //背景图转换为444
    yuv420p_to_444p(w, h, pback, pback2);

#if 1
    //保存转换为yuv444的背景
    FILE* pfback444 = fopen("background_temp_444p.yuv", "w");
    if (0 != pfback444)
    {
        fwrite(pback2, 1, bufsize_444, pfback444);
        fclose(pfback444);
        pfback444 = 0;
    }
#endif

    //开始转换处理
    unsigned char* psrcY = psrc;
    unsigned char* psrcU = psrcY + w*h;
    unsigned char* psrcV = psrcU + w*h;

    unsigned char* pbackY = pback2;
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

    //叠加后的yuv420转换为444
    yuv444p_to_420p(w, h, pout, pout2);

#if 1
    //保存444叠加的输出
    FILE* pfout444 = fopen("out_temp_444p.yuv", "w");
    if (0 != pfout444)
    {
        fwrite(pout, 1, bufsize_444, pfout444);
        fclose(pfout444);
        pfout444 = 0;
    }
#endif
    
    //叠加后的yuv保存到输出文件
    fwrite(pout2, 1, bufsize_420, pfOut);

    fclose(pfSrc);
    fclose(pfBack);
    fclose(pfOut);

    delete[] psrc;
    delete[] pback;
    delete[] pout;
    delete[] pout2;
    delete[] pback2;

    return 0;
}
