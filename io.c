#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cv.h"

CvImage *LoadFile(char *path) //读取bmp文件, print参数控制是否输出图片信息
{
    FILE *pfile;                                          //申请一个文件指针
    CvImage *bmpImg = (CvImage *)malloc(sizeof(CvImage)); //创建一个CvImage结构体空间，及指向它的指针
    BITMAPFILEHEADER bitHead;                             //申请文件头
    BITMAPINFOHEADER bitinfoHead;                         //申请信息头
    RGBQUAD *rgb;                                         //创建调色板指针
    int width, height, biBitCount;                        //图像宽度/高度/每像素位数
    int widthByte;                                        //处理后的图像宽度(字节数为4的整数倍)

    if ((pfile = fopen(path, "rb")) == NULL) //加载文件，并判断文件是否成功打开
    {
        printf("File open failed when read pfile.\n");
        bmpImg->state = 0;
        return bmpImg;
    }

    fread(&bitHead, sizeof(BITMAPFILEHEADER), 1, pfile);     //读入文件头
    fread(&bitinfoHead, sizeof(BITMAPINFOHEADER), 1, pfile); //读入信息头

    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    biBitCount = bitinfoHead.biBitCount;
    widthByte = (width * biBitCount / 8 + 3) / 4 * 4; //宽度的字节数必须为4的整数倍

    // if (print)
    // {
    //     printf("\n\t位图文件头\n");
    //     printf("文件类型：%d\n", bitHead.bfType);
    //     printf("文件大小: %d\n", bitHead.bfSize);
    //     printf("保留字: %d\n", bitHead.bfReserved1);
    //     printf("保留字: %d\n", bitHead.bfReserved2);
    //     printf("实际位图数据的偏移字节数: %d\n\n", bitHead.bfOffBits);

    //     printf("\t位图信息头\n");
    //     printf("结构体的长度: %d\n", bitinfoHead.biSize);
    //     printf("位图宽: %d\n", bitinfoHead.biWidth);
    //     printf("位图高: %d\n", bitinfoHead.biHeight);
    //     printf("biPlanes平面数: %d\n", bitinfoHead.biPlanes);
    //     printf("biBitCount采用颜色位数: %d\n", bitinfoHead.biBitCount);
    //     printf("压缩方式: %d\n", bitinfoHead.biCompression);
    //     printf("biSizeImage实际位图数据占用的字节数: %d\n", bitinfoHead.biSizeImage);
    //     printf("X方向分辨率: %d\n", bitinfoHead.biXPelsPerMeter);
    //     printf("Y方向分辨率: %d\n", bitinfoHead.biYPelsPerMeter);
    //     printf("使用的颜色数: %d\n", bitinfoHead.biClrUsed);
    //     printf("重要颜色数: %d\n\n", bitinfoHead.biClrImportant);
    // }

    if (biBitCount == 8) //有调色板，需读入
    {
        rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256);
        fread(rgb, sizeof(RGBQUAD), 256, pfile);
        free(rgb);
    }

    bmpImg->imageData = (BYTE *)malloc(sizeof(BYTE) * widthByte * height);
    fread(bmpImg->imageData, sizeof(BYTE) * widthByte, height, pfile);
    fclose(pfile);

    bmpImg->bitHead = bitHead;
    bmpImg->bitinfoHead = bitinfoHead;
    bmpImg->rgb = rgb;
    bmpImg->widthByte = widthByte;
    bmpImg->state = 1; //成功读取

    return bmpImg;
}
CvImage *OutFile(char *path, CvImage *bmpImg)
{
    FILE *pfile;
    int width = bmpImg->bitinfoHead.biWidth;
    int height = bmpImg->bitinfoHead.biHeight;
    int widthByte = bmpImg->widthByte;
    int i;

    if ((pfile = fopen(path, "wb")) == NULL)
    {
        printf("File open failed when write pfile.\n");
        bmpImg->state = 0;
        return bmpImg; //写入文件打开失败
    }

    fwrite(&(bmpImg->bitHead), sizeof(BITMAPFILEHEADER), 1, pfile);     //写入文件头
    fwrite(&(bmpImg->bitinfoHead), sizeof(BITMAPINFOHEADER), 1, pfile); //写入信息头

    if (bmpImg->bitinfoHead.biBitCount == 8) //有调色板，写入
        fwrite(bmpImg->rgb, sizeof(RGBQUAD), 256, pfile);

    fwrite(bmpImg->imageData, sizeof(BYTE) * widthByte, height, pfile); //写入图像数据
    fclose(pfile);

    bmpImg->state = 1; //文件成功写入，可省略
    return bmpImg;
}