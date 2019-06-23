#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CV.h"

CvImage *LoadFile(char *path, int print) //读取bmp文件
{
    FILE *pfile;                                          //申请一个文件指针
    CvImage *bmpImg = (CvImage *)malloc(sizeof(CvImage)); //创建一个CvImage结构体空间，及指向它的指针
    BITMAPFILEHEADER bitHead;                             //申请文件头
    BITMAPINFOHEADER bitinfoHead;                         //申请信息头
    RGBQUAD *rgb;                                         //创建调色板指针
    int width, height, biBitCount;   
    int widthByte, gwidthByte;                          //必须为4的整数倍 widthByte 原始 gwidthByte 8位
    int i, j, k;
    int channels, step, offset;
    BYTE pixVal;

    if ((pfile = fopen(path, "rb")) == NULL)
    {
        printf("File open failed.\n");
        free(bmpImg);
        return NULL;
    }

    fread(&bitHead, sizeof(BITMAPFILEHEADER), 1, pfile); //读入文件头
    fread(&bitinfoHead, sizeof(BITMAPINFOHEADER), 1, pfile); //读入信息头

    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    biBitCount = bitinfoHead.biBitCount;
    widthByte = (width * biBitCount / 8 + 3) / 4 * 4; //4的整数倍
    gwidthByte = (width * 8 / 8 + 3) / 4 * 4;

    if (print)
    {
        printf("\n\t位图文件头\n");
        printf("文件类型：%d\n", bitHead.bfType);
        printf("文件大小: %d\n", bitHead.bfSize);
        printf("保留字: %d\n", bitHead.bfReserved1);
        printf("保留字: %d\n", bitHead.bfReserved2);
        printf("实际位图数据的偏移字节数: %d\n\n", bitHead.bfOffBits);

        printf("\t位图信息头\n");
        printf("结构体的长度: %d\n", bitinfoHead.biSize);
        printf("位图宽: %d\n", bitinfoHead.biWidth);
        printf("位图高: %d\n", bitinfoHead.biHeight);
        printf("biPlanes平面数: %d\n", bitinfoHead.biPlanes);
        printf("biBitCount采用颜色位数: %d\n", bitinfoHead.biBitCount);
        printf("压缩方式: %d\n", bitinfoHead.biCompression);
        printf("biSizeImage实际位图数据占用的字节数: %d\n", bitinfoHead.biSizeImage);
        printf("X方向分辨率: %d\n", bitinfoHead.biXPelsPerMeter);
        printf("Y方向分辨率: %d\n", bitinfoHead.biYPelsPerMeter);
        printf("使用的颜色数: %d\n", bitinfoHead.biClrUsed);
        printf("重要颜色数: %d\n\n", bitinfoHead.biClrImportant);
    }

    if (biBitCount == 8)
    {
        rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256); //8位有调色板，需先读入
        fread(rgb, sizeof(RGBQUAD), 256, pfile); 
        free(rgb);

        channels = 1;
        step = channels * width;
        offset = step % 4;
        if (offset != 0)
            offset = 4 - offset;
        bmpImg->imageData = (BYTE *)malloc(sizeof(BYTE) * width * height);
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                fread(&pixVal, sizeof(BYTE), 1, pfile);
                bmpImg->imageData[(height - 1 - i) * step + j] = pixVal;
            }
            if (offset != 0)
            {
                for (j = 0; j < offset; j++)
                {
                    fread(&pixVal, sizeof(BYTE), 1, pfile);
                }
            }
        }
    }
    else if (biBitCount == 24)
    {
        channels = 3;
        bmpImg->imageData = (BYTE *)malloc(sizeof(BYTE) * width * 3 * height);
        step = channels * width;
        //windows规定每一个扫描行为4的倍数，不足补0
        offset = step % 4;
        if (offset != 0)
        {
            offset = 4 - offset;
        }
        //读取彩色图像数据
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pfile);
                    bmpImg->imageData[(height - 1 - i) * step + j * 3 + k] = pixVal;
                }
            }
            if (offset != 0)
            {
                for (j = 0; j < offset; j++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pfile);
                }
            }
        }
    }
    fclose(pfile);

    //写入bmpImg结构体，并返回给main中指针
    bmpImg->width = width;
    bmpImg->height = height;
    bmpImg->widthByte = widthByte;
    bmpImg->gwidthByte = gwidthByte;
    bmpImg->biBitCount = biBitCount;

    return bmpImg;
}

int OutFile(char *path, CvImage *bmpImg)
{
    FILE *pfile;
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *rgb;
    BYTE pixVal = '\0';
    int width = bmpImg->width;
    int height = bmpImg->height;
    int gwidthByte = bmpImg->gwidthByte;
    int i, j, step, channels = 1, offset;
    step = width * channels;
    offset = step % 4;
    if (offset != 0)
    {
        offset = 4 - offset;
        step += offset;
    }

    if ((pfile = fopen(path, "wb")) == NULL)
        return 0; //0假，1真

    bitHead.bfType = 0x4D42;
    bitHead.bfSize = 54 + 256 * 4 + width;
    bitHead.bfReserved1 = 0;
    bitHead.bfReserved2 = 0;
    bitHead.bfOffBits = 54 + 256 * 4;
    fwrite(&bitHead, sizeof(BITMAPFILEHEADER), 1, pfile);

    bitinfoHead.biSize = 40;
    bitinfoHead.biWidth = width;
    bitinfoHead.biHeight = height;
    bitinfoHead.biPlanes = 1;
    bitinfoHead.biBitCount = 8;
    bitinfoHead.biCompression = 0;
    bitinfoHead.biSizeImage = height * width * 1;
    bitinfoHead.biXPelsPerMeter = 0;
    bitinfoHead.biYPelsPerMeter = 0;
    bitinfoHead.biClrUsed = 256;
    bitinfoHead.biClrImportant = 256;
    fwrite(&bitinfoHead, sizeof(BITMAPINFOHEADER), 1, pfile);

    rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256); 
    for (i = 0; i < 256; i++) //调色板初始化
    {
        rgb[i].rgbBlue = i;
        rgb[i].rgbGreen = i;
        rgb[i].rgbRed = i;
        rgb[i].rgbReserved = 0;
    }
    fwrite(rgb, sizeof(RGBQUAD), 256, pfile);
    free(rgb);
    for (i = height - 1; i > -1; i--)
    {
        for (j = 0; j < width; j++)
        {
            pixVal = bmpImg->imageData[i * width + j];
            fwrite(&pixVal, sizeof(BYTE), 1, pfile);
        }
        if (offset != 0)
        {
            for (j = 0; j < offset; j++)
            {
                pixVal = 0;
                fwrite(&pixVal, sizeof(BYTE), 1, pfile);
            }
        }
    }
    fclose(pfile);
    return 1;
}
