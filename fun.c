#include <stdio.h>
#include <stdlib.h>
#include "cv.h"

void GrayscaleProc(CvImage *bmpImg)
{
    int width = bmpImg->bitinfoHead.biWidth;
    int height = bmpImg->bitinfoHead.biHeight;
    int widthByte = bmpImg->widthByte;
    int i, j;

    BYTE *pData = (BYTE *)malloc(sizeof(BYTE) * height * widthByte);
    bmpImg->rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256);
    for (i = 0; i < 256; i++) //调色板初始化
    {
        bmpImg->rgb[i].rgbBlue = i;
        bmpImg->rgb[i].rgbGreen = i;
        bmpImg->rgb[i].rgbRed = i;
        bmpImg->rgb[i].rgbReserved = i;
    }

    // for (i = 0; i < height; i++)
    // {
    //     for (j = 0; j < widthByte; j++)
    //     {
    //         pData[i * widthByte + j] = (int)((float)bmpImg->imageData[i * widthByte + j * 3] * 0.114 + (float)bmpImg->imageData[i * widthByte + j * 3 + 1] * 0.587 + (float)bmpImg->imageData[i * widthByte + j * 3 + 2] * 0.299);
    //     }
    // }
    bmpImg->bitHead.bfSize = 54 + 256 * 4 + width;
    bmpImg->bitHead.bfOffBits = 54 + 256 * 4;
    bmpImg->bitinfoHead.biBitCount = 8;
    bmpImg->bitinfoHead.biSizeImage = height * width;
    bmpImg->bitinfoHead.biXPelsPerMeter = 0;
    bmpImg->bitinfoHead.biYPelsPerMeter = 0;
    bmpImg->bitinfoHead.biClrUsed = 256;
    bmpImg->bitinfoHead.biClrImportant = 256;
    // bmpImg->imageData = pData;
}