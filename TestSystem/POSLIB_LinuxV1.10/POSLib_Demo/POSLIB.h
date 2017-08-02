/*Header file for Printer APIs*/

#ifndef	PRNT_H__
#define	PRNT_H__
#define PRN_USB

#define		POS_SUCCESS			  	1001		//Normal
#define		POS_FAIL		  		1002		//Communication error or not connect printer
#define		POS_ERROR_INVALID_HANDLE	1101		//Handle error
#define		POS_ERROR_INVALID_PARAMETER	1102		//Parameter error
#define		POS_ERROR_NOT_BITMAP		1103		//Not bitmap
#define		POS_ERROR_NOT_MONO_BITMAP	1104		//Not single color bitmap
#define		POS_ERROR_BEYOND_AREA		1105		//Bitmap is much larger
#define		POS_ERROR_INVALID_PATH		1106		//Invalid path or file
#define		POS_ERROR_FILE                1301		//File control error

const int POS_OPEN_SERIAL_PORT   = 0x11;    
const int POS_OPEN_PARALLEL_PORT = 0x12;    
const int POS_OPEN_BYUSB_PORT    = 0x13;    
const int POS_OPEN_PRINTNAME     = 0x14;
const int POS_OPEN_NETPORT       = 0x15;

#define		INVALID_HANDLE_VALUE		-1

#ifndef _MAC
#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */
#else   /* def _MAC */
#define _MAX_PATH   256 /* max. length of full pathname */
#define _MAX_DIR    32  /* max. length of path component */
#define _MAX_FNAME  64  /* max. length of file name component */
#endif  /* _MAC */

#define C10_04_1       1  // 1 byte
#define C10_04_2       2
#define C10_04_3       3
#define C10_04_4       4
#define C1D_72_1      21
#define C1D_72_2      22
#define C1B_75_00     23  
#define C1B_76        24
#define C1D_49_01     31
#define C1D_49_02     32
#define C1D_49_03     33  //4 bytes
#define C1D_49_11     34  //4
#define C1D_49_12     35  //7
#define C1D_49_13     36  //9
#define C1D_49_15     37  //12
#define C1D_49_71     38  //3
#define C1D_00        41  //32
#define C1D_07        42  //3
#define C1D_0F        43  //3

typedef unsigned short      WORD;
typedef unsigned long     DWORD;
typedef unsigned char     BYTE; 
typedef long      LONG;
typedef char          TCHAR; 
typedef int          BOOL; 
typedef unsigned int    u_int;
typedef u_int           SOCKET;

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#define TRUE  1
#define FALSE 0 

#pragma pack(1)
//the file header of bitmap
typedef struct tagBITMAPFILEHEADER {
	WORD bfType; 
	DWORD bfSize; 
	WORD bfReserved1; 
	WORD bfReserved2; 
	DWORD bfOffBits; 
} BITMAPFILEHEADER;

//the info header of bitmap
typedef struct tagBITMAPINFOHEADER{
	DWORD  biSize; 
	LONG   biWidth; 
	LONG   biHeight; 
	WORD   biPlanes; 
	WORD   biBitCount; 
	DWORD  biCompression; 
	DWORD  biSizeImage; 
	LONG   biXPelsPerMeter; 
	LONG   biYPelsPerMeter; 
	DWORD  biClrUsed; 
	DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

//the struct of pallett
typedef struct tagRGBQUAD { 
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO { 
	BITMAPINFOHEADER bmiHeader; 
	RGBQUAD          bmiColors[1]; 
} BITMAPINFO, *PBITMAPINFO; 
#pragma pack()

enum Pr_Barcode_ {
	PR_UPC_A = 65,
	PR_UPC_E,
	PR_JAN13,
	PR_JAN8,
	PR_CODE39,
	PR_ITF,
	PR_CODABAR,
	PR_CODE93,
	PR_CODE128,
	PR_PDF417=75,
};


#ifdef	__cplusplus
extern "C" {
#endif



/*extra function*/

int POS_Open(    
	char *lpNmae,		// port name or printer name
	int nComBaudrate,      // the COM's baud rate
	int nComDataBits,      // the COM's data bits
	int nComStopBits,      // the COM's stop bits
	int nComParity,        // the COM's parity
	int nParam             // the COM's flow control or OPEN_PRINTER or OPEN_BYUSB_PORT
	);

int POS_Close(int hPort ,int nPortType);

int POS_Reset(int hPort,int nPortType);

int POS_SetMode(int hPort,int nPortType,int nPrintMode);

int POS_SetMotionUnit(int hPort,int nPortType,int nHorizontalMU,int nVerticalMU);

int POS_SetCharSetAndCodePage(int hPort,int nPortType,int nCharSet,int nCodePage);

int POS_FeedLine(int hPort,int nPortType);

int POS_SetLineSpacing(int hPort,int nPortType,int nDistance);

int POS_SetRightSpacing(int hPort,int nPortType,int nDistance);

int POS_KickOutDrawer(int hPort,int nPortType,int nID,int nOnTimes,int nOffTimes);

int POS_CutPaper(int hPort,int nPortType,int nMode,int nDistance);

int POS_S_SetAreaWidth(int hPort,int nPortType,int nWidth);

int POS_S_TextOut(int hPort,int nPortType,char *pszString,int nOrgx,int nWidthTimes,int nHeightTimes,int nFontType,int nFontStyle);

int POS_S_SetBarcode(int hPort,int nPortType,char *pszInfoBuffer,int nOrgx,int nType,int nWidthX,int nHeight,int nHriFontType,int nHriFontPosition,int nBytesToPrint);

int POS_PL_SetArea(int hPort,int nPortType,int nOrgx,int nOrgy,int nWidth,int nHeight,int nDirection);

int POS_PL_TextOut(int hPort,int nPortType,
	char *pszString,     // null-terminated string to be written
	int nOrgx,           // x-coordinate of starting position
	int nOrgy,           // y-coordinate of starting position
	int nWidthTimes,     // multiplier of the width of font
	int nHeightTimes,    // multiplier of the height of font
	int nFontType,       // font type
	int nFontStyle       // font style
	);

int POS_PL_SetBarcode(int hPort,int nPortType,
	char *pszInfo,            // null-terminated string to be coded
	int nOrgx,                // x-coordinate of starting position
	int nOrgy,                // y-coordinate of starting position
	int nType,                // the type of bar code to print
	int nWidthX,              // the bar code's base module width
	int nHeight,              // the bar code image's height
	int nHriFontType,         // the Human Readable Interpretation (HRI) 
	                             // characters' font type code
	int nHriFontPosition,     // the Human Readable Interpretation (HRI) 
	                             // characters' position code
	int nBytesOfInfo          // the number of bytes to print 
	);

int POS_PL_Print(int hPort,int nPortType);

int POS_PL_Clear(int hPort,int nPortType);

int POS_PreDownloadBmpToRAM(int hPort,int nPortType,
	char * lpPath,        // null-terminated string
	int nID                // the number of bit image to download
	);
int POS_PreDownloadBmpsToFlash(int hPort,int nPortType,
	char* pszPaths[],    // Pointer to string array
	int nCount           // the number of bit image to download
	);

int POS_S_DownloadAndPrintBmp(int hPort,int nPortType,
	char *pszPath,    // null-terminated string
	int nOrgx,        // x-coordinate of starting position
	int nMode         // mode of bit-image	
	);

int POS_S_PrintBmpInRAM(int hPort,int nPortType,
	int nID,         // the number of bit image downloaded
	int nOrgx,       // x-coordinate of starting position
	int nMode        // print mode
	);

int POS_S_PrintBmpInFlash(int hPort,int nPortType,
	int nID,         // the number of bit image downloaded
	int nOrgx,       // x-coordinate of starting position
	int nMode        // print mode
	);

int POS_PL_DownloadAndPrintBmp(int hPort,int nPortType,
	char *pszPath,    // null-terminated string	
	int nOrgx,        // x-coordinate of starting position
	int nOrgy,        // y-coordinate of starting position
	int nMode         // mode of bit-image
	);

int POS_PL_PrintBmpInRAM(int hPort,int nPortType,
	int nID,         // the number of bit image downloaded
	int nOrgx,       // x-coordinate of starting position
	int nOrgy,       // y-coordinate of starting position
	int nMode        // print mode
	);

int POS_BeginSaveFile(
	int hPort,
	char * lpFileName,    // file name
	BOOL bToPrinter        // whether send data to printer while to file.
	);

int POS_EndSaveFile(int hPort);

int POS_WriteFile(
	int hPort,
	int nPortType,
	char *pszData,        // pointer to the buffer containing data to be written
	int nBytesToWrite     // the number of data to be written
	) ;

int POS_ReadFile(
	int hPort,
	int nPortType,         // Handle to the port or file to be read
	char *pszData,        // pointer to the buffer to receive the read data 
	int nBytesToRead,     // the number of data to be read
	int nTimeouts         // time-out setting
	) ;

int POS_NETQueryStatus(
	int nPortType,
	char *ipAddress,
	char *pStatus        // pointer to a data buffer allocated with 1 bytes
	);

int POS_QueryStatus(
	int hPort,
	int nPortType,
	char *pStatus,     // pointer to a data buffer allocated with 1 bytes
	int nTimeouts
	);

int POS_RTQueryStatus(
	int hPort,
	int nPortType,
	char *pStatus        // pointer to a data buffer allocated with 1 bytes
	);

int POS_GetVersionInfo(
	int *pnMajor,
	int *pnMinor
	);

#ifdef	__cplusplus
}
#endif

#endif	/* PRNT_H__ */
