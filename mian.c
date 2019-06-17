#include <stdio.h>
#include <stdlib.h>
#include <math.h>   //pow
#include <string.h> //memset
#include "myfile.h"
#include "io.h"

int main()
{
    ScvImage *Image = (ScvImage *)malloc(sizeof(ScvImage));
    char strFilePath[50] = {0}; //文件地址

    printf("Please input the path of .bmp file.\n");
    scanf("%s", strFilePath);
    LoadFile(strFilePath,Image);
    printf("%d",*(Image->data));
    return 0;
}
void LoadFile(char *fileName,ScvImage* Image)
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *Rgb;
    FILE *pfile;
    int biBitCount, i;
    long PlanNum;
    int width, height, lineByte;

    

    pfile = fopen(fileName, "r+");
    if (pfile != NULL)
    {
        printf("Successfully open the file.\n");
        WORD fileType;

        //初步判断是否能读取
        if (fread(&fileType, 1, sizeof(WORD), pfile) <= 0)
        {
            printf("Read Type of file failed.\n");
            fclose(pfile);
        }
        //使指针重新指向文件起始位置
        rewind(pfile);

        //判断是否位.bmp文件
        if (fileType != 0x4d42)
        {
            printf("Not the .bmp file");
        }
        fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), pfile); //读取头文件
        // printBmpHead(&bitHead);
        // printf("\n");
        fread(&bitinfoHead, 1, sizeof(BITMAPINFOHEADER), pfile); //读取信息头
        // printBmpInfoHead(&bitinfoHead);
        // printf("\n");
    }
    else
    {
        printf("File open fail.\n");
    }

    biBitCount = bitinfoHead.biBitCount;
    if (biBitCount < 24) //有调色板，读入
    {
        PlanNum = (long)pow(2, biBitCount);
        Rgb = (RGBQUAD *)malloc(PlanNum * sizeof(RGBQUAD));
        memset(Rgb, 0, PlanNum * sizeof(RGBQUAD));
        fread(Rgb, 4, (int)pow(2, biBitCount), pfile);

        printf("Color Plate Number: %d\n", PlanNum);
        printf("\t调色板信息\n");
        for (i = 0; i < PlanNum; i++)
        {
            if (i % 5 == 0)
                printf("\n");
            printRgbQuan(&Rgb[i]);
        }
    }

    //向内存中存储像素信息
    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    //位图每行字节数必须是4的整数倍
    //或者：(width*biBitCount+31)/32*4
    lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
    BYTE *fColorData = (BYTE *)malloc(lineByte * height);
    //位图数据读入数组
    fread(fColorData, lineByte * height, 1, pfile);
    fclose(pfile);

    Image->width = width;
    Image->height = height;
    Image->widthBytes = lineByte;
    Image->data = fColorData;
    
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
void printRgbQuan(RGBQUAD *fRGB)
{
    printf("(%-3d,%-3d,%-3d)", fRGB->rgbRed, fRGB->rgbGreen, fRGB->rgbBlue);
}