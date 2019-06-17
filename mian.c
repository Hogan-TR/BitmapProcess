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

    char strFilePath[50]; //�ļ���ַ
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

        rewind(pfile); //ʹָ������ָ���ļ���ʼλ��

        if (fileType != 0x4d42) //�ж��Ƿ�λ.bmp�ļ�
        {
            printf("Not the .bmp file");
            return 0;
        }
        fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), pfile); //��ȡͷ�ļ�
        printBmpHead(&bitHead);
        printf("\n");
        fread(&bitinfoHead, 1, sizeof(BITMAPINFOHEADER), pfile); //��ȡ��Ϣͷ
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