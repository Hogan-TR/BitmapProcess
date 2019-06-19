#include <stdio.h>
#include <stdlib.h>
#include <math.h>   //pow
#include <string.h> //memset
#include "myfile.h"
#include "io.h"

int main()
{
    ScvImage *Image;
    char strFilePath[50] = {0}; //�ļ���ַ
    char strOutPath[50] = {0};  //�����ַ

    printf("Please input the path of .bmp file.\n");
    scanf("%s", strFilePath);
    printf("Please input the path of outFile.\n");
    scanf("%s", strOutPath);
    FILE *pfile = fopen(strFilePath, "rb");
    FILE *bmpFile = fopen(strOutPath, "wb");

    Image = LoadFile(strFilePath, pfile);
    if (Image->biBitCount == 24)
        GrayscaleProc(Image);

    saveImageToBmp(Image, bmpFile);
    return 0;
}
ScvImage *LoadFile(char *fileName, FILE *pfile) //������NULL
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *Rgb = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));
    int biBitCount, i;
    long PlanNum;
    int width, height, lineByte;

    ScvImage *ImageL = (ScvImage *)malloc(sizeof(ScvImage));

    if (pfile != NULL)
    {
        printf("Successfully open the file.\n");
        WORD fileType;

        //�����ж��Ƿ��ܶ�ȡ
        if (fread(&fileType, 1, sizeof(WORD), pfile) <= 0)
        {
            printf("Read Type of file failed.\n");
            fclose(pfile);
            return NULL;
        }
        //ʹָ������ָ���ļ���ʼλ��
        rewind(pfile);

        //�ж��Ƿ�λ.bmp�ļ�
        if (fileType != 0x4d42)
        {
            printf("Not the .bmp file");
            return NULL;
        }
        fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), pfile); //��ȡͷ�ļ�
        printBmpHead(&bitHead);
        printf("\n");
        fread(&bitinfoHead, 1, sizeof(BITMAPINFOHEADER), pfile); //��ȡ��Ϣͷ
        printBmpInfoHead(&bitinfoHead);
        printf("\n");
    }
    else
    {
        printf("File open fail.\n");
        return NULL;
    }

    biBitCount = bitinfoHead.biBitCount;
    if (biBitCount < 24) //�е�ɫ�壬����
    {
        PlanNum = (long)pow(2, biBitCount);
        Rgb = (RGBQUAD *)malloc(PlanNum * sizeof(RGBQUAD));
        memset(Rgb, 0, PlanNum * sizeof(RGBQUAD));
        fread(Rgb, 4, (int)pow(2, biBitCount), pfile);

        // printf("Color Plate Number: %d\n", PlanNum);
        // printf("\t��ɫ����Ϣ\n");
        // for (i = 0; i < PlanNum; i++)
        // {
        //     if (i % 5 == 0)
        //         printf("\n");
        //     printRgbQuan(&Rgb[i]);
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

    // ImageL.bmpfHeader = bitHead;
    // ImageL.bmpiHeader = bitinfoHead;
    // ImageL.bmpRgb =
    // ImageL.data =
    ImageL->bmpfHeader = &bitHead;
    ImageL->bmpiHeader = &bitinfoHead;
    ImageL->bmpRgb = Rgb;
    ImageL->data = fColorData;
    ImageL->biBitCount = biBitCount;
    ImageL->widthByte = lineByte;
    ImageL->gwidthByte = (width * 8 / 8 + 3) / 4 * 4;
    return ImageL;
}

void GrayscaleProc(ScvImage *Image1)
{
    RGBQUAD *pColorTable = Image1->bmpRgb;
    int width = Image1->bmpiHeader->biWidth;
    int height = Image1->bmpiHeader->biHeight;
    int linewidth = Image1->widthByte;
    int glinewidth = Image1->gwidthByte;
    int i, j, y;
    int nLineStart, ngLineStart;
    int red, green, blue;
    // BYTE *pBmpBuf, *pBmpBuf1;
    // unsigned char *pb1, *pb2;

    //�޸��ļ�ͷ,������������Ҫ�޸ģ��ֱ�ΪbfSize��bfOffBits
    Image1->bmpfHeader->bfSize = 14 + 40 + 256 * sizeof(RGBQUAD) + Image1->gwidthByte * height;
    Image1->bmpfHeader->bfOffBits = 14 + 40 + 256 * sizeof(RGBQUAD);
    //�޸���Ϣͷ��������������Ҫ�޸ģ�1��λbiBitCount:���ͼΪ24 ��Ӧ�ĳ�8����һ����biSizeImage:����ÿ������ռλ���ı仯������λͼ���ݵĴ�С�����仯
    Image1->bmpiHeader->biBitCount = 8;
    Image1->bmpiHeader->biSizeImage = Image1->gwidthByte * height;

    for (i = 0; i < 256; i++)
    {
        pColorTable[i].rgbRed = i;
        pColorTable[i].rgbGreen = i;
        pColorTable[i].rgbBlue = i;
        pColorTable[i].rgbReserved = 0;
    }
    Image1->bmpRgb = pColorTable; //ע�͵�
    // for (i = 0; i < height; i++)
    // {
    //     for (j = 0; j < linewidth; j++)
    //     {
    //         pb1 = pBmpBuf + i * linewidth + j * 3;
    //         y = *(pb1)*0.299 + *(pb1 + 1) * 0.587 + *(pb1 + 2) * 0.114; //��ÿһ�����ض�����ʽy=B*0.299+G*0.587+R*0.114����ת��
    //         pb2 = pBmpBuf1 + i * linewidth + j;
    //         *pb2 = y;
    //     }
    // }
    for (i = 0; i < height; i++)
    {
        nLineStart = linewidth * i; // 24 λ���ͼ��ֵ
        ngLineStart = glinewidth * i; // �Ҷ�ͼ��ֵ
        for (j = 0; j < linewidth; j++)
        {
            (Image1->data)[ngLineStart + j] = (int)((float)*((Image1->data) + nLineStart + 3 * j) * 0.114 + (float)*((Image1->data) + nLineStart + 3 * j + 1) * 0.587 + (float)*((Image1->data) + nLineStart + 3 * j + 2) * 0.299);
        }
    }
}
void saveImageToBmp(ScvImage *image, FILE *bmpFile)
{
    ScvImage *saveOut = image;
    BITMAPFILEHEADER b1;
    BITMAPINFOHEADER b2, *f2 = image->bmpiHeader;
    RGBQUAD *b3 = image->bmpRgb;
    BYTE *b4 = image->data;
    int lineByte = image->widthByte;
    int height = image->bmpiHeader->biHeight;

    b1.bfType = image->bmpfHeader->bfType;
    b1.bfSize = image->bmpfHeader->bfSize;
    b1.bfReserved1 = image->bmpfHeader->bfReserved1;
    b1.bfReserved2 = image->bmpfHeader->bfReserved2;
    b1.bfOffBits = image->bmpfHeader->bfOffBits;

    b2.biSize = f2->biSize;
    b2.biWidth = f2->biWidth;
    b2.biHeight = f2->biHeight;
    b2.biPlanes = f2->biPlanes;
    b2.biBitCount = f2->biBitCount;
    b2.biCompression = f2->biCompression;
    b2.biSizeImage = f2->biSizeImage;
    b2.biXPelsPerMeter = f2->biXPelsPerMeter;
    b2.biYPelsPerMeter = f2->biYPelsPerMeter;
    b2.biClrUsed = f2->biClrUsed;
    b2.biClrImportant = f2->biClrImportant;

    if (bmpFile != NULL)
    {
        fwrite(&b1, sizeof(BITMAPFILEHEADER), 1, bmpFile);
        fwrite(&b2, sizeof(BITMAPINFOHEADER), 1, bmpFile);
        fwrite(b3, sizeof(RGBQUAD), 256, bmpFile);
        fwrite(b4, image->gwidthByte * height, 1, bmpFile);
        fclose(bmpFile);
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
void printRgbQuan(RGBQUAD *fRGB)
{
    printf("(%-3d,%-3d,%-3d)", fRGB->rgbRed, fRGB->rgbGreen, fRGB->rgbBlue);
}