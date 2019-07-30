#include <stdio.h>
#include "cv.h"

int main()
{
    char Inpath[100], Outpath[100] = "Get.bmp";
    CvImage *Img;
    printf("请输入图像地址:");
    scanf("%s",Inpath);
    Img = LoadFile(Inpath);
    if (!Img->state) //Meet error
        return 0;
    // GrayscaleProc(Img);
    Img = OutFile(Outpath, Img);
    if (!Img->state)
        return 0;
    return 0;
}