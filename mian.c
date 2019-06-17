#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "myfile.h"

int main()
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    FILE *pfile;

    unsigned int Counti;
    int biBitCount;
    RGBQUAD *Rgb;

    char strFilePath[50]; //文件地址
    printf("Please input the path of .bmp file.\n");
    scanf("%s", strFilePath);

    pfile = fopen(strFilePath, "r+");

    if (pfile != NULL)
    {
        printf("Successfully open.\n");
        WORD fileType;

        if (fread(&fileType, 1, sizeof(WORD), pfile) <= 0)
        {
            printf("Read BMP failed.\n");
            fclose(pfile);
            return 0;
        }

        rewind(pfile); //使指针重新指向文件起始位置

        if (fileType != 0x4d42) //判断是否位.bmp文件
        {
            printf("Not the .bmp file");
            return 0;
        }
        fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), pfile); //读取头文件
        printBmpHead(&bitHead);
        printf("\n");
        fread(&bitinfoHead, 1, sizeof(BITMAPINFOHEADER), pfile); //读取信息头
        printBmpInfoHead(&bitinfoHead);
        printf("\n");
        biBitCount = bitinfoHead.biBitCount;
        if (biBitCount < 24)
        {
            Rgb = (RGBQUAD *)malloc((int)pow(2, biBitCount) * sizeof(RGBQUAD));
            fread(Rgb, 4, (int)pow(2, biBitCount), pfile);
        }
    }
    else
    {
        printf("File open fail.\n");
        return 0;
    }
}
void printBmpHead(BITMAPFILEHEADER *BFileHead)
{
    printf("\t位图文件头\n");
    printf("文件大小: %d\n", BFileHead->bfSize);
    printf("保留字: %d\n", BFileHead->bfReserved1);
    printf("保留字: %d\n", BFileHead->bfReserved2);
    printf("实际位图数据的偏移字节数: %d\n", BFileHead->bfOffBits);
}

void printBmpInfoHead(BITMAPINFOHEADER *BInfoHead)
{
    printf("\t位图信息头\n");
    printf("结构体的长度: %d\n", BInfoHead->biSize);
    printf("位图宽: %d\n", BInfoHead->biWidth);
    printf("位图高: %d\n", BInfoHead->biHeight);
    printf("biPlanes平面数: %d\n", BInfoHead->biPlanes);
    printf("biBitCount采用颜色位数: %d\n", BInfoHead->biBitCount);
    printf("压缩方式: %d\n", BInfoHead->biCompression);
    printf("biSizeImage实际位图数据占用的字节数: %d\n", BInfoHead->biSizeImage);
    printf("X方向分辨率: %d\n", BInfoHead->biXPelsPerMeter);
    printf("Y方向分辨率: %d\n", BInfoHead->biYPelsPerMeter);
    printf("使用的颜色数: %d\n", BInfoHead->biClrUsed);
    printf("重要颜色数: %d\n", BInfoHead->biClrImportant);
}