#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CV.h"

int main()
{
    int Tf;
    CvImage *img = LoadFile("2.bmp");
    CvImage *RotImg;
    GrayscaleProc(img);
    RotImg = RoateFile(img, 73);
    Tf = OutFile("t1.bmp", RotImg);
    if (Tf)
        printf("Fail!");
    return 0;
}
CvImage *LoadFile(char *path)
{
    FILE *pfile;
    CvImage *bmgImg = (CvImage *)malloc(sizeof(CvImage));
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *rgb;
    int width, height, biBitCount;
    int widthByte, gwidthByte;

    if ((pfile = fopen(path, "rb")) == NULL)
    {
        printf("File open failed.\n");
        free(bmgImg);
        return NULL;
    }

    fread(&bitHead, sizeof(BITMAPFILEHEADER), 1, pfile);
    fread(&bitinfoHead, sizeof(BITMAPINFOHEADER), 1, pfile);

    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    biBitCount = bitinfoHead.biBitCount;
    widthByte = (width * biBitCount / 8 + 3) / 4 * 4;

    // printf("\tλͼ�ļ�ͷ\n");
    // printf("�ļ����ͣ�%d\n", bitHead.bfType);
    // printf("�ļ���С: %d\n", bitHead.bfSize);
    // printf("������: %d\n", bitHead.bfReserved1);
    // printf("������: %d\n", bitHead.bfReserved2);
    // printf("ʵ��λͼ���ݵ�ƫ���ֽ���: %d\n", bitHead.bfOffBits);

    // printf("\tλͼ��Ϣͷ\n");
    // printf("�ṹ��ĳ���: %d\n", bitinfoHead.biSize);
    // printf("λͼ��: %d\n", bitinfoHead.biWidth);
    // printf("λͼ��: %d\n", bitinfoHead.biHeight);
    // printf("biPlanesƽ����: %d\n", bitinfoHead.biPlanes);
    // printf("biBitCount������ɫλ��: %d\n", bitinfoHead.biBitCount);
    // printf("ѹ����ʽ: %d\n", bitinfoHead.biCompression);
    // printf("biSizeImageʵ��λͼ����ռ�õ��ֽ���: %d\n", bitinfoHead.biSizeImage);
    // printf("X����ֱ���: %d\n", bitinfoHead.biXPelsPerMeter);
    // printf("Y����ֱ���: %d\n", bitinfoHead.biYPelsPerMeter);
    // printf("ʹ�õ���ɫ��: %d\n", bitinfoHead.biClrUsed);
    // printf("��Ҫ��ɫ��: %d\n", bitinfoHead.biClrImportant);

    if (biBitCount == 24)
    {
        gwidthByte = (width * 8 / 8 + 3) / 4 * 4;
        bitHead.bfSize = 14 + 40 + 256 * sizeof(RGBQUAD) + gwidthByte * height;
        bitHead.bfOffBits = 14 + 40 + 256 * sizeof(RGBQUAD);

        bitinfoHead.biBitCount = 8;
        bitinfoHead.biSizeImage = gwidthByte * height;
    }
    if (biBitCount == 8)
    {
        rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256);
        fread(rgb, sizeof(RGBQUAD), 256, pfile);
        free(rgb);
    }
    bmgImg->imageData = (BYTE *)malloc(widthByte * height);
    fread(bmgImg->imageData, widthByte * height, 1, pfile);
    fclose(pfile);

    bmgImg->width = width;
    bmgImg->height = height;
    bmgImg->widthByte = widthByte;
    bmgImg->gwidthByte = gwidthByte;

    return bmgImg;
}

int OutFile(char *path, CvImage *bmgImg)
{
    FILE *pfile;
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *rgb;
    int width = bmgImg->width;
    int height = bmgImg->height;
    int gwidthByte = bmgImg->gwidthByte;
    int i, j;

    if ((pfile = fopen(path, "wb")) == NULL)
        return 0; //0�٣�1��

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
    for (i = 0; i < 256; i++)
    {
        rgb[i].rgbBlue = i;
        rgb[i].rgbGreen = i;
        rgb[i].rgbRed = i;
        rgb[i].rgbReserved = 0;
    }
    fwrite(rgb, sizeof(RGBQUAD), 256, pfile);
    free(rgb);
    fwrite(bmgImg->imageData, gwidthByte * height, 1, pfile);
    fclose(pfile);
}
void GrayscaleProc(CvImage *bmpImg)
{
    BYTE *Idata = bmpImg->imageData;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int widthByte = bmpImg->widthByte;
    int gwidthByte = bmpImg->gwidthByte;
    int i, j;
    int nLineStart, ngLineStart;

    for (i = 0; i < height; i++)
    {
        nLineStart = widthByte * i;
        ngLineStart = gwidthByte * i;
        for (j = 0; j < gwidthByte; j++)
            Idata[ngLineStart + j] = (int)((float)*(Idata + nLineStart + 3 * j) * 0.114 + (float)*(Idata + nLineStart + 3 * j + 1) * 0.587 + (float)*(Idata + nLineStart + 3 * j + 2) * 0.299);
    }
}
CvImage *RoateFile(CvImage *bmpImg, int INangle)
{
    CvImage *bmpImgT;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int step;
    int Rot_step;
    double angle;
    int i, j;

    int midX_pre, midY_pre, midX_aft, midY_aft; //��תǰ������ĵ������
    int pre_i, pre_j, after_i, after_j;         //��תǰ���Ӧ�����ص�����

    midX_pre = width / 2;
    midY_pre = height / 2;
    angle = 1.0 * INangle * PI / 100;

    bmpImgT = (CvImage *)malloc(sizeof(CvImage));
    bmpImgT->width = width;
    bmpImgT->height = height;

    midX_aft = bmpImgT->width / 2;
    midY_aft = bmpImgT->height / 2;

    step = 1 * width;
    Rot_step = 1 * bmpImgT->width;
    bmpImgT->imageData = (BYTE *)malloc(sizeof(BYTE) * bmpImgT->width * bmpImgT->height * 1);

    //��ʼ����תͼ��
    for (i = 0; i < bmpImgT->height; i++)
    {
        for (j = 0; j < bmpImgT->width; j++)
        {
            bmpImgT->imageData[(bmpImgT->height - 1 - i) * Rot_step + j] = 0;
        }
    }
    for (i = 0; i < bmpImgT->height; i++)
    {
        for (j = 0; j < bmpImgT->width; j++)
        {
            after_i = i - midX_aft;
            after_j = j - midY_aft;
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;
            if (pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width) //��ԭͼ��Χ��
                bmpImgT->imageData[i * Rot_step + j] = bmpImg->imageData[pre_i * step + pre_j];
        }
    }
    bmpImgT->widthByte = bmpImg->widthByte;
    bmpImgT->gwidthByte = bmpImg->gwidthByte;
    return bmpImgT;
}