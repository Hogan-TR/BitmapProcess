#ifndef __CV_H__
#define __CV_H__

#define PI 3.14159

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER //λͼͷ�ļ�����
{
    WORD bfType;      //λͼ���'BM', 2
    DWORD bfSize;     //�ļ���С, 4
    WORD bfReserved1; //������, 2
    WORD bfReserved2; //������, 2
    DWORD bfOffBits;  //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���, 4
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER //λͼ��Ϣͷ����
{
    DWORD biSize;         //��Ϣͷ��С, 4
    LONG biWidth;         //ͼ����, 4
    LONG biHeight;        //ͼ��߶�, 4
    WORD biPlanes;        //Ŀ���豸�ļ��𣬱�����1, 2
    WORD biBitCount;      //ÿ����λ��, 2
    DWORD biCompression;  //ѹ������, 4
    DWORD biSizeImage;    //ѹ��ͼ���С�ֽ���, 4
    LONG biXPelsPerMeter; //ˮƽ�ֱ���, 4
    LONG biYPelsPerMeter; //��ֱ�ֱ���, 4
    DWORD biClrUsed;      //λͼʵ���õ���ɫ����, 4
    DWORD biClrImportant; //λͼ����Ҫ��ɫ����, 4
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD //��ɫ�嶨��
{
    BYTE rgbBlue;     //��ɫֵ
    BYTE rgbGreen;    //��ɫֵ
    BYTE rgbRed;      //��ɫֵ
    BYTE rgbReserved; //����ֵ
} RGBQUAD;

// #pragma pack(1)
// typedef struct tagIMAGEDATA //������Ϣ
// {
//     BYTE red;
//     BYTE green;
//     BYTE blue;
// } IMAGEDATA;
// #pragma pack()

typedef struct
{
    int width;
    int height;
    int widthByte;
    int gwidthByte;
    int biBitCount;
    BYTE *imageData;
} CvImage;
#pragma pack(pop) //�ָ�����״̬

CvImage *LoadFile(char *);
int OutFile(char *, CvImage *);
void GrayscaleProc(CvImage *);
CvImage *RoateFile(CvImage *, int);

#endif