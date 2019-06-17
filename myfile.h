#ifndef __MYFILE_H__
#define __MYFILE_H__

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

#pragma pack(1)
typedef struct tagBITMAPFILEHEADER //λͼͷ�ļ�����
{
    WORD bfType;      //λͼ���'BM', 2
    DWORD bfSize;     //�ļ���С, 4
    WORD bfReserved1; //������, 2
    WORD bfReserved2; //������, 2
    DWORD bfOffBits;  //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���, 4
} BITMAPFILEHEADER;
#pragma pack()

#pragma pack(1)
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
#pragma pack()

#pragma pack(1)
typedef struct tagRGBQUAD //��ɫ�嶨��
{
    BYTE rgbBlue;     //��ɫֵ
    BYTE rgbGreen;    //��ɫֵ
    BYTE rgbRed;      //��ɫֵ
    BYTE rgbReserved; //����ֵ
} RGBQUAD;
#pragma pack()

#pragma pack(1)
typedef struct tagIMAGEDATA //������Ϣ
{
    BYTE red;
    BYTE green;
    BYTE blue;
} IMAGEDATA;
#pragma pack()

typedef struct ScvImage {
    BITMAPFILEHEADER *bmpfHeader;
    BITMAPINFOHEADER *bmpiHeader;
    RGBQUAD *bmpRgb;
    BYTE *data;
    int biBitCount;
    int widthByte;
} ScvImage;

#endif
