#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "common.h"
#include <QString>
#include <QtGui>

 //(((x+3)/4)*4)
#define GET_ALIGN(x)  (((x+7)/8)*8)

#pragma pack(1)

typedef struct
{
        char	bfType[2];
        unsigned long	bfSize;
        unsigned short		bfReserved1;
        unsigned short		bfReserved2;
        unsigned long	bfOffBits;
}BMPFILEHEAD2;

typedef struct
{
        unsigned long	biSize;
        long	biWidth;
        long	biHeight;
        unsigned short		biPlanes;
        unsigned short		biBitCount;
        unsigned long	biCompression;
        unsigned long	biSizeImage;
        long	biXPelsPerMeter;
        long	biYPelsPerMeter;
        unsigned long	biClrUsed;
        unsigned long	biClrImportant;
}BMPINFOHEAD2;

typedef struct
{
        unsigned char rgbtBlue;       // blue level
        unsigned char rgbtGreen ;      // green level
        unsigned char rgbtRed ;        // red level
    unsigned char Reserved;
}RGBTRIPLE;  // rgbt;

#pragma pack()

#if 1
char* GetBmpInfo(const char *filename)
{
	int fd;
	long len;
	char* ret;

	if(filename == NULL) return NULL;
	fd=open(filename,O_RDONLY);
	if(fd == -1)
	{
			printf("open bmp file error!\n");
			return NULL;
	}
	len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	ret = (char*)malloc(len);
	if(ret != NULL)
	{
		memset(ret, 0, len);
		if(len != read(fd, ret, len))
		{
			free(ret);
			ret = NULL;
		}
	}

	return ret;
}
#else
int GetBmpInfo(const char *filename,int *BitWidht,int *BitHeight,int *ByteWidht,unsigned char *pImgData)
{
	int fid;
	BMPFILEHEAD2 fileHeadST;
	BMPINFOHEAD2 infoHeadST;
	long lOffBit,lWidth,lHeight,lSizeImg;
	int  nByteWidth;

	fid=open(filename,O_RDWR);
	if(fid==-1)
	{
			printf("open bmp file error!\n");
			return 0;
	}

	lseek(fid,0,SEEK_SET);
	read(fid,&fileHeadST,sizeof(BMPFILEHEAD2));
	read(fid,&infoHeadST,sizeof(BMPINFOHEAD2));

	printf("fileHeadST:\n");
	printf("\tbfOffBits %ld\n", fileHeadST.bfOffBits);
	printf("\tbfSize %ld\n", fileHeadST.bfSize);
	printf("\tbfType %s\n", fileHeadST.bfType);

	printf("\ninfoHeadST\n");
	printf("\tbiBitCount %d\n", infoHeadST.biBitCount);
	printf("\tbiClrImportant %ld\n", infoHeadST.biClrImportant);
	printf("\tbiClrUsed %ld\n", infoHeadST.biClrUsed);
	printf("\tbiCompression %ld\n", infoHeadST.biCompression);
	printf("\tbiHeight %ld\n", infoHeadST.biHeight);
	printf("\tbiPlanes %d\n", infoHeadST.biPlanes);
	printf("\tbiSize %ld\n", infoHeadST.biSize);
	printf("\tbiSizeImage %ld\n", infoHeadST.biSizeImage);
	printf("\tbiWidth %ld\n", infoHeadST.biWidth);
	printf("\tbiXPelsPerMeter %ld\n", infoHeadST.biXPelsPerMeter);
	printf("\tbiYPelsPerMeter %ld\n", infoHeadST.biYPelsPerMeter);


	lOffBit=fileHeadST.bfOffBits;
	lWidth=infoHeadST.biWidth;
	lHeight=infoHeadST.biHeight;
	if(lHeight<0)
	{
			lHeight=0-lHeight;
	}
	*BitHeight=lHeight;
	*BitWidht=lWidth;
	lSizeImg=fileHeadST.bfSize;//infoHeadST.biSizeImage;
	nByteWidth=lSizeImg/lHeight;
	*ByteWidht=nByteWidth;

	lseek(fid,lOffBit,SEEK_SET);
	read(fid,pImgData,lSizeImg);
	close(fid);
	return 1;
}
#endif
int WriteBmp(const char* pszFileName,  unsigned char* pImg, int width, int height)
{
        BMPFILEHEAD2 bfh;
        BMPINFOHEAD2 bmh;
        RGBTRIPLE bmiColors[] =
        {
            {0x00, 0x00, 0x00, 0x00},
            {0xFF, 0xFF, 0xFF, 0xFF}
        };
        FILE* fp;
        fp=fopen(pszFileName, "wb");
        if(fp == NULL)
        {
                printf("fopen bmp file error!\n");
                return -1;
        }

        //写位图文件头
        memcpy(bfh.bfType,"BM", 2);
        bfh.bfSize = GET_ALIGN(width)*height/8+sizeof(BMPFILEHEAD2)+sizeof(BMPINFOHEAD2)+2*sizeof(RGBTRIPLE);
        bfh.bfOffBits = sizeof(BMPFILEHEAD2)+sizeof(BMPINFOHEAD2)+2*sizeof(RGBTRIPLE);
        bfh.bfReserved1=0;
        bfh.bfReserved2=0;

        //设置位图参数
        bmh.biSize    = sizeof(BMPINFOHEAD2);
        bmh.biWidth   = width;
        bmh.biHeight  = height;
        bmh.biPlanes  = 1;
        bmh.biBitCount= 1;
        bmh.biCompression = 0;
        bmh.biSizeImage   = GET_ALIGN(width)*height/8;
        bmh.biXPelsPerMeter = 0;//3780
        bmh.biYPelsPerMeter = 0;//3780
        bmh.biClrUsed       = 0;
        bmh.biClrImportant  = 0;

        fwrite(&bfh, sizeof(BMPFILEHEAD2), 1, fp);
        fwrite(&bmh, sizeof(BMPINFOHEAD2), 1, fp);
        fwrite(&bmiColors, sizeof(RGBTRIPLE), 2, fp);
        fwrite(pImg, height*GET_ALIGN(width)/8, 1, fp);
#if 0
        //行倒过来，对齐后写入
        for(int i=0; i<height; i++)
        {
                fwrite(pImg+(height-i-1)*width, 1, GET_ALIGN(width), fp);
        }
#endif

        fclose(fp);

        return 0;
}

char* CreateMark(char* buf, int len)
{
        int i;

        memset(buf, 0, len);
        //srand((int)time(0));
         srand((unsigned)time(0)*10);

        for(i=0; i<len; i++)
        {
                if(0 == (rand()%2))
                {
                        strcat(buf, "0");
                }
                else
                {
                        strcat(buf, "1");
                }
        }
        buf[len] = '\0';

        return buf;
}

void SetBackgroundImage(QWidget *widget, const char *imgPath)
{
	QImage image(imgPath);
	QPixmap pixmap = QPixmap::fromImage(image.scaled(widget->geometry().width(), widget->geometry().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	QPalette palette;
	palette.setBrush(widget->backgroundRole(),QBrush(pixmap));
	widget->setPalette(palette);
	//widget->setMask(pixmap.mask());
	widget->setAutoFillBackground(true);
}


