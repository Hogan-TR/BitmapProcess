#include <stdio.h>
#include "myfile.h"

int main()
{
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitinfoHead;
    FILE *pfile;

    char strFilePath[50]; //文件路径
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
        if (fileType != 0x4d42)
        {
            printf("Not the .bmp file");
            return 0;
        }
        
    }
}