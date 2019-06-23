#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CV.h"

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