#include <stdio.h>
#include <stdlib.h>
#include <math.h>   //pow
#include <string.h> //memset
#include "myfile.h"
#include "io.h"

int main()
{
    ScvImage *Image ;
    char strFilePath[50] = {0}; //文件地址
    char strOutPath[50] = {0};  //储存地址

    printf("Please input the path of .bmp file.\n");
    scanf("%s", strFilePath);
    printf("Please input the path of outFile.\n");
    scanf("%s", strOutPath);

    Image = LoadFile(strFilePath);
    if (Image->biBitCount == 24)
        GrayscaleProc(Image);
    saveImageToBmp(Image, strOutPath);
    return 0;
}
ScvImage *LoadFile(char *fileName) //出错返回NULL
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *Rgb;
    FILE *pfile;
    int biBitCount, i;
    long PlanNum;
    int width, height, lineByte;
    
    ScvImage *ImageL=(ScvImage*)malloc(sizeof(ScvImage));

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
            return NULL;
        }
        //使指针重新指向文件起始位置
        rewind(pfile);

        //判断是否位.bmp文件
        if (fileType != 0x4d42)
        {
            printf("Not the .bmp file");
            return NULL;
        }
        fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), pfile); //读取头文件
        printBmpHead(&bitHead);
        printf("\n");
        fread(&bitinfoHead, 1, sizeof(BITMAPINFOHEADER), pfile); //读取信息头
        printBmpInfoHead(&bitinfoHead);
        printf("\n");
    }
    else
    {
        printf("File open fail.\n");
        return NULL;
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
    return ImageL;
}

void GrayscaleProc(ScvImage *Image1)
{
    RGBQUAD *pColorTable = Image1->bmpRgb;
    int linewidth = Image1->widthByte;
    int height = Image1->bmpiHeader->biHeight;
    int i, j, y;
    BYTE *pBmpBuf, *pBmpBuf1;
    unsigned char *pb1, *pb2;

    //修改文件头,其中有两项需要修改，分别为bfSize和bfOffBits
    Image1->bmpfHeader->bfSize = 14 + 40 + 256 * sizeof(RGBQUAD) + linewidth * height;
    Image1->bmpfHeader->bfOffBits = 14 + 40 + 256 * sizeof(RGBQUAD);
    //修改信息头，其中有两项需要修改，1个位biBitCount:真彩图为24 ，应改成8；另一个是biSizeImage:由于每像素所占位数的变化，所以位图数据的大小发生变化
    Image1->bmpiHeader->biBitCount = 8;
    Image1->bmpiHeader->biSizeImage = linewidth * height;

    for (i = 0; i < 256; i++)
    {
        pColorTable[i].rgbRed = i;
        pColorTable[i].rgbGreen = i;
        pColorTable[i].rgbBlue = i;
    }
    Image1->bmpRgb = pColorTable;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < linewidth; j++)
        {
            pb1 = pBmpBuf + i * linewidth + j * 3;
            y = *(pb1)*0.299 + *(pb1 + 1) * 0.587 + *(pb1 + 2) * 0.114; //将每一个像素都按公式y=B*0.299+G*0.587+R*0.114进行转化
            pb2 = pBmpBuf1 + i * linewidth + j;
            *pb2 = y;
        }
    }
    Image1->data = pBmpBuf1;
}
void saveImageToBmp(ScvImage *image, char *filename)
{
    // int imageByteCount = image->widthBytes * image->height;

    // BitmapFileHeader fileHeader = {0};
    // fileHeader.bfType = 0x4D42; // "BM"
    // fileHeader.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + imageByteCount;
    // fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    // BitmapInfoHeader infoHeader = {0};
    // infoHeader.biSize = sizeof(BitmapInfoHeader);
    // infoHeader.biHeight = image->origin ? image->height : -image->height;
    // infoHeader.biWidth = image->width;
    // infoHeader.biPlanes = 1;
    // infoHeader.biBitCount = 24;
    // infoHeader.biSizeImage = imageByteCount;
    // infoHeader.biCompression = 0;
    BITMAPFILEHEADER *fileHeader = image->bmpfHeader;
    BITMAPINFOHEADER *infoHeader = image->bmpiHeader;
    RGBQUAD *rgb = image->bmpRgb;

    int lineByte = image->widthByte;
    int height = image->bmpiHeader->biHeight;

    FILE *bmpFile = fopen(filename, "wb");
    if (bmpFile != NULL)
    {
        fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
        fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
        fwrite(image->data, lineByte * height, 1, bmpFile);
        fclose(bmpFile);
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
void printRgbQuan(RGBQUAD *fRGB)
{
    printf("(%-3d,%-3d,%-3d)", fRGB->rgbRed, fRGB->rgbGreen, fRGB->rgbBlue);
}