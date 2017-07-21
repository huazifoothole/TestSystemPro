#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <QtCore>

# define FLAG_ISSET(v, f) ((v) & (f))
# define FLAG_SET(v, f)   ((v) |= (f))
# define FLAG_CLEAR(v, f) ((v) &= ~(f))

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#if 1
char* GetBmpInfo(const char *filename);
#else
int GetBmpInfo(const char *filename,int *BitWidht,int *BitHeight,int *ByteWidht,unsigned char *pImgData);
#endif
int WriteBmp(const char* pszFileName,  unsigned char* pImg, int width, int height);
char* CreateMark(char* buf, int len);
void SetBackgroundImage(QWidget *widget, const char *imgPath);

#endif // COMMON_H
