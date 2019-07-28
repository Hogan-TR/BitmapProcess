#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CV.h"

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