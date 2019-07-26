#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

#pragma pack(push, 1)
typedef struct  //λͼͷ�ļ�����
{
    WORD bfType;      //λͼ���'BM', 2
    DWORD bfSize;     //�ļ���С, 4
    WORD bfReserved1; //������, 2
    WORD bfReserved2; //������, 2
    DWORD bfOffBits;  //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���, 4
} BITMAPFILEHEADER;

typedef struct  //λͼ��Ϣͷ����
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

typedef struct  //��ɫ�嶨��
{
    BYTE rgbBlue;     //��ɫֵ
    BYTE rgbGreen;    //��ɫֵ
    BYTE rgbRed;      //��ɫֵ
    BYTE rgbReserved; //����ֵ
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
#pragma pack(pop) //�ָ�����״̬

CvImage *LoadFile(char *, int);
int OutFile(char *, CvImage *);
void GrayscaleProc(CvImage *);
CvImage *RoateFile(CvImage *, int);

int main()
{
    char InPath[100], OutPath[100]; // ���롢���·��
    int choice;
    int angle;
    int f1, f2, Tf; //����ѡ���ǣ�������
    CvImage *img;
    printf("\n<<------ ����ͼ���� ͼ����ת ------>>\n\n");
    printf("1.ͼƬ��Ϣ��ȡ\n"
           "2.ͼƬ�Ҷȴ���\n"
           "3.ͼƬ��ת����\n"
           "4.�˳�����\n\n");
    while (1) //��ѯ
    {
        f1 = 0; //ÿһ����ѯ��ʼ����f2���س�ʼ��
        printf("�������ѡ��ѡ����(��4�˳�):\n");
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
            printf("\n���������,�����³���.\n");
            break;
        }

        if (f1 == 1) //����1
        {
            printf("�����������bmp�ļ���ַ��");
            scanf("%s", InPath);
            img = LoadFile(InPath, 1); //�ڶ�������Ϊ1�������ͼƬ��Ϣ
        }
        else if (f1 == 2) //����2+3
        {
            printf("���������������ַ��");
            scanf("%s", InPath);
            scanf("%s", OutPath);
            img = LoadFile(InPath, 0); // �ڶ�������Ϊ0�������ͼƬ��Ϣ
            if (img->biBitCount == 24) // bitBitCountΪ24������лҶȴ���
                GrayscaleProc(img);
            if (f2) //����3
            {
                printf("��������ת�Ƕȣ�");
                scanf("%d", &angle);
                angle = angle % 360; //��ֹangle����
                img = RoateFile(img, angle);
            }
            Tf = OutFile(OutPath, img); //TfֵΪ 0 �� 1���ж��Ƿ�ɹ����
            if (Tf)
                printf("ͼƬ����ɹ�.\n\n");
            else
                printf("ͼƬ���ʧ��.\n\n");
        }
    }
    return 0;
}

//�Ҷȴ���
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

    for (i = 0; i < height; i++) //�Ҷȴ�������һ����ʽ
    {
        for (j = 0; j < width; j++)
        {
            Ldata[(height - 1 - i) * 1 * width + j] = (int)((float)Idata[(height - 1 - i) * 3 * width + j * 3] * 0.114 + (float)Idata[(height - 1 - i) * 3 * width + j * 3 + 1] * 0.587 + (float)Idata[(height - 1 - i) * 3 * width + j * 3 + 2] * 0.299);
        }
    }
    bmpImg->imageData = Ldata;
}
//��ת
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
    angle = 1.0 * INangle * PI / 180; //���Ƚ�

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
    //��תͼ��
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
    //��ֵ�������½ṹ��
    bmpImgT->widthByte = bmpImg->widthByte;
    bmpImgT->gwidthByte = bmpImg->gwidthByte;
    bmpImgT->biBitCount = bmpImg->biBitCount;
    return bmpImgT;
}

CvImage *LoadFile(char *path, int print) //��ȡbmp�ļ�
{
    FILE *pfile;                                          //����һ���ļ�ָ��
    CvImage *bmpImg = (CvImage *)malloc(sizeof(CvImage)); //����һ��CvImage�ṹ��ռ䣬��ָ������ָ��
    BITMAPFILEHEADER bitHead;                             //�����ļ�ͷ
    BITMAPINFOHEADER bitinfoHead;                         //������Ϣͷ
    RGBQUAD *rgb;                                         //������ɫ��ָ��
    int width, height, biBitCount;
    int widthByte, gwidthByte; //����Ϊ4�������� widthByte ԭʼ gwidthByte 8λ
    int i, j, k;
    int channels, step, offset;
    BYTE pixVal;

    if ((pfile = fopen(path, "rb")) == NULL)
    {
        printf("File open failed.\n");
        free(bmpImg);
        return NULL;
    }

    fread(&bitHead, sizeof(BITMAPFILEHEADER), 1, pfile);     //�����ļ�ͷ
    fread(&bitinfoHead, sizeof(BITMAPINFOHEADER), 1, pfile); //������Ϣͷ

    width = bitinfoHead.biWidth;
    height = bitinfoHead.biHeight;
    biBitCount = bitinfoHead.biBitCount;
    widthByte = (width * biBitCount / 8 + 3) / 4 * 4; //4��������
    gwidthByte = (width * 8 / 8 + 3) / 4 * 4;

    if (print)
    {
        printf("\n\tλͼ�ļ�ͷ\n");
        printf("�ļ����ͣ�%d\n", bitHead.bfType);
        printf("�ļ���С: %d\n", bitHead.bfSize);
        printf("������: %d\n", bitHead.bfReserved1);
        printf("������: %d\n", bitHead.bfReserved2);
        printf("ʵ��λͼ���ݵ�ƫ���ֽ���: %d\n\n", bitHead.bfOffBits);

        printf("\tλͼ��Ϣͷ\n");
        printf("�ṹ��ĳ���: %d\n", bitinfoHead.biSize);
        printf("λͼ��: %d\n", bitinfoHead.biWidth);
        printf("λͼ��: %d\n", bitinfoHead.biHeight);
        printf("biPlanesƽ����: %d\n", bitinfoHead.biPlanes);
        printf("biBitCount������ɫλ��: %d\n", bitinfoHead.biBitCount);
        printf("ѹ����ʽ: %d\n", bitinfoHead.biCompression);
        printf("biSizeImageʵ��λͼ����ռ�õ��ֽ���: %d\n", bitinfoHead.biSizeImage);
        printf("X����ֱ���: %d\n", bitinfoHead.biXPelsPerMeter);
        printf("Y����ֱ���: %d\n", bitinfoHead.biYPelsPerMeter);
        printf("ʹ�õ���ɫ��: %d\n", bitinfoHead.biClrUsed);
        printf("��Ҫ��ɫ��: %d\n\n", bitinfoHead.biClrImportant);
    }

    if (biBitCount == 8)
    {
        rgb = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 256); //8λ�е�ɫ�壬���ȶ���
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
        //windows�涨ÿһ��ɨ����Ϊ4�ı��������㲹0
        offset = step % 4;
        if (offset != 0)
        {
            offset = 4 - offset;
        }
        //��ȡ��ɫͼ������
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

    //д��bmpImg�ṹ�壬�����ظ�main��ָ��
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
    for (i = 0; i < 256; i++) //��ɫ���ʼ��
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
