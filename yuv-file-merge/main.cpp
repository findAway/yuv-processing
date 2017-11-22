#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "#### param error ####\n");
        fprintf(stderr, "usage: thistool file1 file2 ...\n");
        return 1;
    }

    int width = 0;
    int height = 0;
    int yuvtype = 0;  // 0 for yuv444, 1 for yuv420

    cout << "select the input file yuv type(0 for yuv444, 1 for yuv420):";
    cin >> yuvtype;
    if (yuvtype != 0 && yuvtype != 1)
    {
        fprintf(stderr, "#### error yuvtype %d ####\n", yuvtype);
        return 1;
    }

    cout << "set the yuv width:";
    cin >> width;

    cout << "set the yuv height:";
    cin >> height;

    int filesize = 0;
    char poutFile[60];
    if (yuvtype == 0)
    {
        filesize = width*height*3;
        sprintf(poutFile, "mergeout-%dx%d-444.yuv", width, height);
    }
    else
    {
        filesize = width*height*3/2;
        sprintf(poutFile, "mergeout-%dx%d-420.yuv", width, height);
    }

    char* pYuvIn = new char[filesize];
    FILE* pfOut = fopen(poutFile, "wb");
    if (pfOut == 0)
    {
        fprintf(stderr, "#### open out file %s error ####\n", poutFile);
        return 1;
    }

    bool bSucc = true;
    //读出所有输入yuv数据写入输出文件
    for (int n = 1; n < argc; n++)
    {
        FILE* pfIn = fopen(argv[n], "rb");
        if (pfIn == 0)
        {
            fprintf(stderr, "#### open input file %s error ####\n", argv[n]);
            bSucc = false;
            break;
        }

        int readSize = fread(pYuvIn, 1, filesize, pfIn);
        if (readSize != filesize)
        {
            fprintf(stderr, "#### input file %s not match ####\n", argv[1]);
            bSucc = false;
            break;
        }

        fwrite(pYuvIn, 1, filesize, pfOut);

        fclose(pfIn);
        pfIn = 0;
    }

    delete[] pYuvIn;
    fclose(pfOut);

    if (bSucc)
    {
        printf("Success! outfile:%s\n", poutFile);
    }
    else
    {
        unlink(poutFile);
    }

    return 0;
}




