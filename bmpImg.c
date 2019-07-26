#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

#pragma pack(push, 1)
typedef struct  //位图头文件定义
{
    WORD bfType;      //位图类别，'BM', 2
    DWORD bfSize;     //文件大小, 4
    WORD bfReserved1; //保留字, 2
    WORD bfReserved2; //保留字, 2
    DWORD bfOffBits;  //从文件头到实际位图数据的偏移字节数, 4
} BITMAPFILEHEADER;

typedef struct  //位图信息头定义
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

typedef struct  //调色板定义
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

int main()
{
    char InPath[100], OutPath[100]; // 输入、输出路径
    int choice;
    int angle;
    int f1, f2, Tf; //功能选择标记，结果标记
    CvImage *img;
    printf("\n<<------ 数字图像处理 图像旋转 ------>>\n\n");
    printf("1.图片信息读取\n"
           "2.图片灰度处理\n"
           "3.图片旋转处理\n"
           "4.退出程序\n\n");
    while (1) //轮询
    {
        f1 = 0; //每一次轮询初始化，f2不必初始化
        printf("请从以上选项选择功能(输4退出):\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            f1 = 1;
            f2 = 0;
            break;
        case 2:
            f1 = 2;
            f2 = 0;
            break;
        case 3:
            f1 = 2;
            f2 = 1;
            break;
        case 4:
            exit(1);
        default:
            printf("\n输入项错误,请重新尝试.\n");
            break;
        }

        if (f1 == 1) //功能1
        {
            printf("请输入待处理bmp文件地址：");
            scanf("%s", InPath);
            img = LoadFile(InPath, 1); //第二个传参为1，则输出图片信息
        }
        else if (f1 == 2) //功能2+3
        {
            printf("请输入输入输出地址：");
            scanf("%s", InPath);
            scanf("%s", OutPath);
            img = LoadFile(InPath, 0); // 第二个传参为0，则不输出图片信息
            if (img->biBitCount == 24) // bitBitCount为24则则进行灰度处理
                GrayscaleProc(img);
            if (f2) //功能3
            {
                printf("请输入旋转角度：");
                scanf("%d", &angle);
                angle = angle % 360; //防止angle过大
                img = RoateFile(img, angle);
            }
            Tf = OutFile(OutPath, img); //Tf值为 0 或 1，判断是否成功输出
            if (Tf)
                printf("图片输出成功.\n\n");
            else
                printf("图片输出失败.\n\n");
        }
    }
    return 0;
}

//灰度处理
void GrayscaleProc(CvImage *bmpImg)
{
    BYTE *Idata = bmpImg->imageData;
    BYTE *Ldata;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int widthByte = bmpImg->widthByte;
    int gwidthByte = bmpImg->gwidthByte;
    int i, j;
    Ldata = (BYTE *)malloc(sizeof(BYTE) * width * height);

    for (i = 0; i < height; i++) //灰度处理，三合一，公式
    {
        for (j = 0; j < width; j++)
        {
            Ldata[(height - 1 - i) * 1 * width + j] = (int)((float)Idata[(height - 1 - i) * 3 * width + j * 3] * 0.114 + (float)Idata[(height - 1 - i) * 3 * width + j * 3 + 1] * 0.587 + (float)Idata[(height - 1 - i) * 3 * width + j * 3 + 2] * 0.299);
        }
    }
    bmpImg->imageData = Ldata;
}
//旋转
CvImage *RoateFile(CvImage *bmpImg, int INangle)
{
    CvImage *bmpImgT;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int step;
    int Rot_step;
    double angle;
    int i, j;

    int midX_pre, midY_pre, midX_aft, midY_aft; //旋转前后的中心点的坐标
    int pre_i, pre_j, after_i, after_j;         //旋转前后对应的像素点坐标

    midX_pre = width / 2;
    midY_pre = height / 2;
    angle = 1.0 * INangle * PI / 180; //弧度角

    bmpImgT = (CvImage *)malloc(sizeof(CvImage));
    bmpImgT->width = width;
    bmpImgT->height = height;

    midX_aft = bmpImgT->width / 2;
    midY_aft = bmpImgT->height / 2;

    step = 1 * width;
    Rot_step = 1 * bmpImgT->width;
    bmpImgT->imageData = (BYTE *)malloc(sizeof(BYTE) * bmpImgT->width * bmpImgT->height * 1);

    //初始化旋转图像
    for (i = 0; i < bmpImgT->height; i++)
    {
        for (j = 0; j < bmpImgT->width; j++)
        {
            bmpImgT->imageData[(bmpImgT->height - 1 - i) * Rot_step + j] = 0;
        }
    }
    //旋转图像
    for (i = 0; i < bmpImgT->height; i++)
    {
        for (j = 0; j < bmpImgT->width; j++)
        {
            after_i = i - midX_aft;
            after_j = j - midY_aft;
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midX_pre;
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midY_pre;
            if (pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width) //在原图范围内
                bmpImgT->imageData[i * Rot_step + j] = bmpImg->imageData[pre_i * step + pre_j];
        }
    }
    //赋值，返回新结构体
    bmpImgT->widthByte = bmpImg->widthByte;
    bmpImgT->gwidthByte = bmpImg->gwidthByte;
    bmpImgT->biBitCount = bmpImg->biBitCount;
    return bmpImgT;
}

CvImage *LoadFile(char *path, int print) //读取bmp文件
{
    FILE *pfile;                                          //申请一个文件指针
    CvImage *bmpImg = (CvImage *)malloc(sizeof(CvImage)); //创建一个CvImage结构体空间，及指向它的指针
    BITMAPFILEHEADER bitHead;                             //申请文件头
    BITMAPINFOHEADER bitinfoHead;                         //申请信息头
    RGBQUAD *rgb;                                         //创建调色板指针
    int width, height, biBitCount;
    int widthByte, gwidthByte; //必须为4的整数倍 widthByte 原始 gwidthByte 8位
    int i, j, k;
    int channels, step, offset;
    BYTE pixVal;

    if ((pfile = fopen(path, "rb")) == NULL)
    {
        printf("File open failed.\n");
        free(bmpImg);
        return NULL;
    }

    fread(&bitHead, sizeof(BITMAPFILEHEADER), 1, pfile);     //读入文件头
    fread(&bitinfoHead, sizeof(BITMAPINFOHEADER), 1, pfile); //读入信息头

    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    biBitCount = bitinfoHead.biBitCount;
    widthByte = (width * biBitCount / 8 + 3) / 4 * 4; //4的整数倍
    gwidthByte = (width * 8 / 8 + 3) / 4 * 4;

    if (print)
    {
        printf("\n\t位图文件头\n");
        printf("文件类型：%d\n", bitHead.bfType);
        printf("文件大小: %d\n", bitHead.bfSize);
        printf("保留字: %d\n", bitHead.bfReserved1);
        printf("保留字: %d\n", bitHead.bfReserved2);
        printf("实际位图数据的偏移字节数: %d\n\n", bitHead.bfOffBits);

        printf("\t位图信息头\n");
        printf("结构体的长度: %d\n", bitinfoHead.biSize);
        printf("位图宽: %d\n", bitinfoHead.biWidth);
        printf("位图高: %d\n", bitinfoHead.biHeight);
        printf("biPlanes平面数: %d\n", bitinfoHead.biPlanes);
        printf("biBitCount采用颜色位数: %d\n", bitinfoHead.biBitCount);
        printf("压缩方式: %d\n", bitinfoHead.biCompression);
        printf("biSizeImage实际位图数据占用的字节数: %d\n", bitinfoHead.biSizeImage);
        printf("X方向分辨率: %d\n", bitinfoHead.biXPelsPerMeter);
        printf("Y方向分辨率: %d\n", bitinfoHead.biYPelsPerMeter);
        printf("使用的颜色数: %d\n", bitinfoHead.biClrUsed);
        printf("重要颜色数: %d\n\n", bitinfoHead.biClrImportant);
    }

    if (biBitCount == 8)
    {
        rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256); //8位有调色板，需先读入
        fread(rgb, sizeof(RGBQUAD), 256, pfile);
        free(rgb);

        channels = 1;
        step = channels * width;
        offset = step % 4;
        if (offset != 0)
            offset = 4 - offset;
        bmpImg->imageData = (BYTE *)malloc(sizeof(BYTE) * width * height);
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                fread(&pixVal, sizeof(BYTE), 1, pfile);
                bmpImg->imageData[(height - 1 - i) * step + j] = pixVal;
            }
            if (offset != 0)
            {
                for (j = 0; j < offset; j++)
                {
                    fread(&pixVal, sizeof(BYTE), 1, pfile);
                }
            }
        }
    }
    else if (biBitCount == 24)
    {
        channels = 3;
        bmpImg->imageData = (BYTE *)malloc(sizeof(BYTE) * width * 3 * height);
        step = channels * width;
        //windows规定每一个扫描行为4的倍数，不足补0
        offset = step % 4;
        if (offset != 0)
        {
            offset = 4 - offset;
        }
        //读取彩色图像数据
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pfile);
                    bmpImg->imageData[(height - 1 - i) * step + j * 3 + k] = pixVal;
                }
            }
            if (offset != 0)
            {
                for (j = 0; j < offset; j++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pfile);
                }
            }
        }
    }
    fclose(pfile);

    //写入bmpImg结构体，并返回给main中指针
    bmpImg->width = width;
    bmpImg->height = height;
    bmpImg->widthByte = widthByte;
    bmpImg->gwidthByte = gwidthByte;
    bmpImg->biBitCount = biBitCount;

    return bmpImg;
}

int OutFile(char *path, CvImage *bmpImg)
{
    FILE *pfile;
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    RGBQUAD *rgb;
    BYTE pixVal = '\0';
    int width = bmpImg->width;
    int height = bmpImg->height;
    int gwidthByte = bmpImg->gwidthByte;
    int i, j, step, channels = 1, offset;
    step = width * channels;
    offset = step % 4;
    if (offset != 0)
    {
        offset = 4 - offset;
        step += offset;
    }

    if ((pfile = fopen(path, "wb")) == NULL)
        return 0; //0假，1真

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
    for (i = 0; i < 256; i++) //调色板初始化
    {
        rgb[i].rgbBlue = i;
        rgb[i].rgbGreen = i;
        rgb[i].rgbRed = i;
        rgb[i].rgbReserved = 0;
    }
    fwrite(rgb, sizeof(RGBQUAD), 256, pfile);
    free(rgb);
    for (i = height - 1; i > -1; i--)
    {
        for (j = 0; j < width; j++)
        {
            pixVal = bmpImg->imageData[i * width + j];
            fwrite(&pixVal, sizeof(BYTE), 1, pfile);
        }
        if (offset != 0)
        {
            for (j = 0; j < offset; j++)
            {
                pixVal = 0;
                fwrite(&pixVal, sizeof(BYTE), 1, pfile);
            }
        }
    }
    fclose(pfile);
    return 1;
}
