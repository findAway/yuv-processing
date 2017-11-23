#include <iostream>
#include <stdio.h>

using namespace std;

// 本程序运行时应有1个参数，为yuv文件
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
        fprintf(stderr, "#### parameter error ####\n");
        fprintf(stderr, "usage: thistool yuvfile\n");
		return 1;
	}

	int nYuvWidth = 0;
	int nYuvHeight = 0;

    //让用户输入yuv的宽高
    cout << "set the yuv width:";
    cin >> nYuvWidth;
    cout << "set the yuv height:";
    cin >> nYuvHeight;

	if (nYuvWidth <= 0 || nYuvHeight <= 0)
	{
		fprintf(stderr, "#### param error, image width or height error ####\n");
		return 1;
	}

    //让用户选择yuv的类型
    int nYuvType = 0;
    cout << "select the input file yuv type(0 for yuv444, 1 for yuv420):";
    cin >> nYuvType;
    if (nYuvType != 0 && nYuvType != 1)
    {
        fprintf(stderr, "#### yuv type %d error ####\n", nYuvType);
        return 1;
    }

	//读取yuv大小。如果用户给的yuv宽高正确的话，那么输入yuv文件的大小应该是整数倍于一帧yuv的大小
	FILE* pfYuv = fopen(argv[1], "rb");
	if (pfYuv == NULL)
	{
		fprintf(stderr, "#### yuv file %s not exist ####\n", argv[1]);
		return 1;
	}
	
	fseek(pfYuv, 0, SEEK_END);
	long lYuvFileSize = ftell(pfYuv);
	long lSigYuvSize = 0;
    if (nYuvType == 0)
    {
        lSigYuvSize = nYuvWidth * nYuvHeight * 3;
    }
    else
    {
        lSigYuvSize = (nYuvWidth * nYuvHeight * 3)>>1;
    }

    if (lYuvFileSize % lSigYuvSize != 0)
    {
        fprintf(stderr, "#### error, file size:%ld, single yuv size:%ld ####\n", lYuvFileSize, lSigYuvSize);
        fclose(pfYuv);
        return 1;
    }

    char pOutFile[40];
    if (0 == nYuvType)
    {
        sprintf(pOutFile, "reverse-%dx%d-444.yuv", nYuvWidth, nYuvHeight);
    }
    else
    {
        sprintf(pOutFile, "reverse-%dx%d-420.yuv", nYuvWidth, nYuvHeight);
    }

    //打开文件用于yuv反转后输出
    FILE* pfYuvReverse = fopen(pOutFile, "wb");
    if (pfYuvReverse == NULL)
    {
        fprintf(stderr, "#### open reverse_out.yuv error ####\n");
        fclose(pfYuv);
        return 1;
    }

    //yuv文件反转操作
    char* pYuvBuf = new char[lSigYuvSize];
    int nTimes = lYuvFileSize / lSigYuvSize;
    for (int nIndex = 0; nIndex < nTimes; nIndex++)
    {
        //先定位到源yuv文件的读取位置
        fseek(pfYuv, (nTimes-1-nIndex)*lSigYuvSize, SEEK_SET);
        //读取一帧yuv大小的数据
        fread(pYuvBuf, lSigYuvSize, 1, pfYuv);
        //写入输出文件
        fwrite(pYuvBuf, lSigYuvSize, 1, pfYuvReverse);
    }

    fclose(pfYuv);
    fclose(pfYuvReverse);
    delete[] pYuvBuf;

    printf("Success... out file:%s\n", pOutFile);
    return 0;
}

