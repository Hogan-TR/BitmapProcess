#ifndef __CV_H__
#define __CV_H__

#define PI 3.14159

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER //位图头文件定义
{
    WORD bfType;      //位图类别，'BM', 2
    DWORD bfSize;     //文件大小, 4
    WORD bfReserved1; //保留字, 2
    WORD bfReserved2; //保留字, 2
    DWORD bfOffBits;  //从文件头到实际位图数据的偏移字节数, 4
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER //位图信息头定义
{
    DWORD biSize;         //信息头大小, 4
    LONG biWidth;         //图像宽度, 4
    LONG biHeight;        //图像高度, 4
    WORD biPlanes;        //目标设备的级别，必须是1, 2
    WORD biBitCount;      //每像素位数, 2
    DWORD biCompression;  //压缩类型, 4
    DWORD biSizeImage;    //压缩图像大小字节数, 4
    LONG biXPelsPerMeter; //水平分辨率, 4
    LONG biYPelsPerMeter; //垂直分辨率, 4
    DWORD biClrUsed;      //位图实际用到的色彩数, 4
    DWORD biClrImportant; //位图中重要的色彩数, 4
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD //调色板定义
{
    BYTE rgbBlue;     //蓝色值
    BYTE rgbGreen;    //绿色值
    BYTE rgbRed;      //红色值
    BYTE rgbReserved; //保留值
} RGBQUAD;

typedef struct
{
    int width;
    int height;
    int widthByte;
    int gwidthByte;
    int biBitCount;
    BYTE *imageData;
} CvImage;
#pragma pack(pop) //恢复对齐状态

CvImage *LoadFile(char *, int);
int OutFile(char *, CvImage *);
void GrayscaleProc(CvImage *);
CvImage *RoateFile(CvImage *, int);

#endif