#include <stdio.h>
#include <stdlib.h>
#include <math.h>   //pow
#include <string.h> //memset
#include "myfile.h"
#include "io.h"

int main()
{
    ScvImage *Image = (ScvImage *)malloc(sizeof(ScvImage));
    char strFilePath[50] = {0}; //�ļ���ַ

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

        //�����ж��Ƿ��ܶ�ȡ
        if (fread(&fileType, 1, sizeof(WORD), pfile) <= 0)
        {
            printf("Read Type of file failed.\n");
            fclose(pfile);
        }
        //ʹָ������ָ���ļ���ʼλ��
        rewind(pfile);

        //�ж��Ƿ�λ.bmp�ļ�
        if (fileType != 0x4d42)
        {
            printf("Not the .bmp file");
        }
        fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), pfile); //��ȡͷ�ļ�
        // printBmpHead(&bitHead);
        // printf("\n");
        fread(&bitinfoHead, 1, sizeof(BITMAPINFOHEADER), pfile); //��ȡ��Ϣͷ
        // printBmpInfoHead(&bitinfoHead);
        // printf("\n");
    }
    else
    {
        printf("File open fail.\n");
    }

    biBitCount = bitinfoHead.biBitCount;
    if (biBitCount < 24) //�е�ɫ�壬����
    {
        PlanNum = (long)pow(2, biBitCount);
        Rgb = (RGBQUAD *)malloc(PlanNum * sizeof(RGBQUAD));
        memset(Rgb, 0, PlanNum * sizeof(RGBQUAD));
        fread(Rgb, 4, (int)pow(2, biBitCount), pfile);

        printf("Color Plate Number: %d\n", PlanNum);
        printf("\t��ɫ����Ϣ\n");
        for (i = 0; i < PlanNum; i++)
        {
            if (i % 5 == 0)
                printf("\n");
            printRgbQuan(&Rgb[i]);
        }
    }

    //���ڴ��д洢������Ϣ
    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    //λͼÿ���ֽ���������4��������
    //���ߣ�(width*biBitCount+31)/32*4
    lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
    BYTE *fColorData = (BYTE *)malloc(lineByte * height);
    //λͼ���ݶ�������
    fread(fColorData, lineByte * height, 1, pfile);
    fclose(pfile);

    Image->width = width;
    Image->height = height;
    Image->widthBytes = lineByte;
    Image->data = fColorData;
    
}
void printBmpHead(BITMAPFILEHEADER *BFileHead)
{
    printf("\tλͼ�ļ�ͷ\n");
    printf("�ļ���С: %d\n", BFileHead->bfSize);
    printf("������: %d\n", BFileHead->bfReserved1);
    printf("������: %d\n", BFileHead->bfReserved2);
    printf("ʵ��λͼ���ݵ�ƫ���ֽ���: %d\n", BFileHead->bfOffBits);
}

void printBmpInfoHead(BITMAPINFOHEADER *BInfoHead)
{
    printf("\tλͼ��Ϣͷ\n");
    printf("�ṹ��ĳ���: %d\n", BInfoHead->biSize);
    printf("λͼ��: %d\n", BInfoHead->biWidth);
    printf("λͼ��: %d\n", BInfoHead->biHeight);
    printf("biPlanesƽ����: %d\n", BInfoHead->biPlanes);
    printf("biBitCount������ɫλ��: %d\n", BInfoHead->biBitCount);
    printf("ѹ����ʽ: %d\n", BInfoHead->biCompression);
    printf("biSizeImageʵ��λͼ����ռ�õ��ֽ���: %d\n", BInfoHead->biSizeImage);
    printf("X����ֱ���: %d\n", BInfoHead->biXPelsPerMeter);
    printf("Y����ֱ���: %d\n", BInfoHead->biYPelsPerMeter);
    printf("ʹ�õ���ɫ��: %d\n", BInfoHead->biClrUsed);
    printf("��Ҫ��ɫ��: %d\n", BInfoHead->biClrImportant);
}
void printRgbQuan(RGBQUAD *fRGB)
{
    printf("(%-3d,%-3d,%-3d)", fRGB->rgbRed, fRGB->rgbGreen, fRGB->rgbBlue);
}