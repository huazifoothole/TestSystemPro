#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <dlfcn.h>
#include <math.h>
#include <string.h>
#include "POSLIB.h"
#include "usb.h"
#include <sys/types.h>
#include <stdlib.h>                                                 
#include <sys/time.h>  
///////////////////
#define SelectItem "Please select operation:				\n\
****************************************************************	\n\
0 :OpenCOM	\n\
1 :OpenLPT	\n\
2 :OpenUSB	\n\
3 :OpenNET	\n\
4 :Close Port	\n\
5 :Quit	\n\
****************************************************************	\n\
Please input your selection:"

///////////////////////
#define SelectItem1 "Please select operation:				\n\
****************************************************************	\n\
0 :Print Sample of 80 wide and stand mode	\n\
1 :Print Sample of 80 wide and page mode	\n\
2 :Print Sample of 56 wide and stand mode	\n\
3 :Print Sample of 56 wide and page mode	\n\
4 :Query Status	\n\
5 :Return	\n\
6 :Quit	\n\
****************************************************************	\n\
Please input your selection:"

#define Baudrate "Please select operation:				\n\
****************************************************************	\n\
0 :9600	\n\
1 :19200	\n\
2 :38400	\n\
3 :57600	\n\
4 :115200	\n\
5 :Return	\n\
6 :Quit	\n\
****************************************************************	\n\
Please input your selection:"

#define	ClearStdin	while((__ch = getchar()) != '\n' && __ch != EOF)	//printf("__ch = %d\n",__ch);

#define		POS_SUCCESS			  	1001		//Normal
#define		POS_FAIL		  		1002		//Communication error or not connect printer
#define		POS_ERROR_INVALID_HANDLE	1101		//Handle error
#define		POS_ERROR_INVALID_PARAMETER	1102		//Parameter error
#define		POS_ERROR_NOT_BITMAP		1103		//Not bitmap
#define		POS_ERROR_NOT_MONO_BITMAP	1104		//Not single color bitmap
#define		POS_ERROR_BEYOND_AREA		1105		//Bitmap is much larger
#define		POS_ERROR_INVALID_PATH		1106		//Invalid path or file
#define		POS_ERROR_FILE                1301		//File control error

#define POS_COM_DTR_DSR 0x00 //Flow control:DTR/DST

#define POS_COM_RTS_CTS 0x01 //Flow control:RTS/CTS 

#define POS_COM_XON_XOFF 0x02 //Flow control:XON/OFF 

#define POS_COM_NO_HANDSHAKE 0x03 //No handshake

#define POS_OPEN_PARALLEL_PORT 0x12 //Open the LPT port

#define POS_OPEN_BYUSB_PORT 0x13 //Open the USB port

#define POS_OPEN_PRINTNAME 0X14 //Open the driver port

#define POS_OPEN_NETPORT 0X15 //Open the NET port

#define POS_FONT_STYLE_NORMAL 0x00 //Normal font

#define POS_FONT_STYLE_BOLD 0x08 //Bold font

#define POS_FONT_STYLE_THIN_UNDERLINE 0x80 //Normal underline

#define POS_FONT_STYLE_THICK_UNDERLINE 0x100 //Double thick underline

#define POS_FONT_STYLE_UPSIDEDOWN 0x200 //upside down

#define POS_FONT_STYLE_REVERSE 0x400 //Reverse

#define POS_FONT_STYLE_CLOCKWISE_90 0x1000 //Clockwise 90

#define POS_PRINT_MODE_STANDARD 0x00 //Standard mode

#define POS_PRINT_MODE_PAGE 0x01 //Page mode

#define POS_BARCODE_TYPE_UPC_A 0x41 // UPC-A

#define POS_BARCODE_TYPE_UPC_E 0x42 // UPC-C

#define POS_BARCODE_TYPE_JAN13 0x43 // JAN13(EAN13)

#define POS_BARCODE_TYPE_JAN8 0x44 // JAN8(EAN8)

#define POS_BARCODE_TYPE_CODE39 0x45 // CODE39

#define POS_BARCODE_TYPE_ITF 0x46 // INTERLEAVED 2 OF 5

#define POS_BARCODE_TYPE_CODEBAR 0x47 // CODEBAR

#define POS_BARCODE_TYPE_CODE93 0x48 // 25

#define POS_BARCODE_TYPE_CODE128 0x49 //CODE 128

#define POS_FONT_TYPE_STANDARD 0x00 //Standard ASCII 

#define POS_FONT_TYPE_COMPRESSED 0x01 //Compressed ASCII

#define POS_HRI_POSITION_NONE 0x00 //Not print HRI

#define POS_HRI_POSITION_ABOVE 0x01 //Print HRI above the barcode

#define POS_HRI_POSITION_BELOW 0x02 //Print HRI below the barcode

#define POS_HRI_POSITION_BOTH 0x03 //Print HRI both above and below the barcode

#define POS_BITMAP_PRINT_NORMAL 0x00 //Normal font

#define POS_BITMAP_PRINT_DOUBLE_WIDTH 0x01 //double wide

#define POS_BITMAP_PRINT_DOUBLE_HEIGHT 0x02 //double hight

#define POS_BITMAP_PRINT_QUADRUPLE 0x03 //double wide and hight

#define POS_CUT_MODE_FULL 0x00 //full cut

#define POS_CUT_MODE_PARTIAL 0x01 //Partial cut

#define POS_AREA_BOTTOM_TO_TOP 0x1 //Bottom to top
//**************************************************************
//variable declare

//**************************************************************
//function declare

char	__ch;
/**************************************************************
main function
**************************************************************/
int main()
{
	int operate;
	int operateBaudrate;
	int operate1;
	int hPort = -1;
	int porttype = -1;
	char Status = 0;
	char buffer0a[] = {0x0a};
	char	IP[255];
	memset(IP,0x00,255);
	
 
//Select operation
	while(TRUE)
	{
	INPUTAGAIN:
		operate = -1;
		printf(SelectItem);	
		scanf("%d",&operate);
		ClearStdin;
		char	FileName[255];
		memset(FileName,0x00,255);

		int nRet;
		switch(operate)
		{
			case 0:	//Open the COM port
					if ((hPort != -1) 
						&& (hPort != POS_ERROR_INVALID_PARAMETER)
						&& (hPort != POS_FAIL))
					{
						POS_Close(hPort,porttype);
					}
					porttype = 0;

				INPUTAGAIN_COMNUM:
					char	COMID[5];
					memset(COMID,0x00,5);
					printf("Please input the COM port number(1-255):");
					scanf("%s",COMID);
					int nCOMID;
					nCOMID = -1;
					nCOMID = atoi(COMID);
					if ((nCOMID < 1) || (nCOMID > 255))
					{
						printf("The data inputed is error!Please select again!\n");	
						goto INPUTAGAIN_COMNUM;
					}

				INPUTAGAIN_Baudrate:
					int	COMBaudrate;
					operateBaudrate = -1;
					printf(Baudrate);	
					scanf("%d",&operateBaudrate);
					ClearStdin;
					switch(operateBaudrate)
					{
						case 0:COMBaudrate = 9600;
							break;
						case 1:COMBaudrate = 19200;
							break;
						case 2:COMBaudrate = 38400;
							break;
						case 3:COMBaudrate = 57600;
							break;
						case 4:COMBaudrate = 115200;
							break;
						case 5:goto INPUTAGAIN;
							break;
						case 6:goto EXITLINE;
							break;
						default:	printf("The data inputed is error!Please select again!\n");	
						goto INPUTAGAIN_Baudrate;
					}

					hPort = POS_Open(COMID,COMBaudrate,8,1,0,1);
					if ((hPort == POS_FAIL) 
						|| (hPort == POS_ERROR_INVALID_PARAMETER)
						|| (hPort == -1))
					{
						printf("Open port failed!\n");
						goto INPUTAGAIN;
					}

					break;

			case 1:{	//Open the LPT port
					if ((hPort != -1) 
						&& (hPort != POS_ERROR_INVALID_PARAMETER)
						&& (hPort != POS_FAIL))
					{
						POS_Close(hPort,porttype);
					}
					porttype = 1;

				INPUTAGAIN_LPTNUM:
					char	LPTID[5];
					memset(LPTID,0x00,5);
					printf("Please input the LPT port number(1-255):");
					scanf("%s",LPTID);
					int nLPTID;
					nLPTID = -1;
					nLPTID = atoi(LPTID);
					if ((nLPTID < 1) || (nLPTID > 255))
					{
						printf("The data inputed is error!Please select again!\n");	
						goto INPUTAGAIN_LPTNUM;
					}
					hPort = POS_Open(LPTID,0,0,0,0,0x12);
					if ((hPort == POS_FAIL) 
						|| (hPort == POS_ERROR_INVALID_PARAMETER)
						|| (hPort == -1))
					{
						printf("Open port failed!\n");
						goto INPUTAGAIN;
					}

					break;
				}

			case 2:{	//Open the USB port
					if ((hPort != -1) 
						&& (hPort != POS_ERROR_INVALID_PARAMETER)
						&& (hPort != POS_FAIL))
					{
						POS_Close(hPort,porttype);
					}
					porttype = 2;

				INPUTAGAIN_USBNUM:
					char	USBID[5];
					memset(USBID,0x00,5);
					printf("Please input the USB ID:");
					scanf("%s",USBID);
					int nUSBID;
					nUSBID = -1;
					nUSBID = atoi(USBID);
					if (nUSBID < 0)
					{
						printf("The data inputed is error!Please select again!\n");	
						goto INPUTAGAIN_USBNUM;
					}
					hPort = POS_Open(USBID,0,0,0,0,0x13);
					if ((hPort == POS_FAIL) 
						|| (hPort == POS_ERROR_INVALID_PARAMETER)
						|| (hPort == -1))
					{
						printf("Open port failed!\n");
						goto INPUTAGAIN;
					}else
					{
					nRet = POS_WriteFile(hPort,porttype,buffer0a,1);
                                          if(nRet)
						{
						printf("Send data success!!\n");
						}
					}

					break;
				}

			case 3:{	//Open the NET port
					if ((hPort != -1) 
						&& (hPort != POS_ERROR_INVALID_PARAMETER)
						&& (hPort != POS_FAIL))
					{
						POS_Close(hPort,porttype);
					}
					porttype = 3;

					printf("Please input the IP:");
					scanf("%s",IP);
					hPort = POS_Open(IP,0,0,0,0,0x15);
					if ((hPort == POS_FAIL) 
						|| (hPort == POS_ERROR_INVALID_PARAMETER)
						|| (hPort == -1))
					{
						printf("Open port failed!\n");
						goto INPUTAGAIN;
					}

					break;
				}

			case 4:	//Close the port
					if (hPort != -1)
					{
						nRet = POS_Close(hPort,porttype);
						if (nRet != POS_SUCCESS)
						{
							printf("Close port failed!\n");
						}
					}
					goto INPUTAGAIN;
					break;

			case 5:	goto EXITLINE;

			default:	printf("The data inputed is error!Please select again!\n");	
					goto INPUTAGAIN;
		}

		while(true)
		{
			INPUTAGAIN1:
			printf(SelectItem1);	
			scanf("%d",&operate1);
			ClearStdin;
			switch(operate1)
			{
			case 0:{	//Print Sample of 80 wide and stand mode

					static bool bIsFirst = true;
					if (bIsFirst)
					{
						//Download logos to flash
						char *pBitImages[2];
						pBitImages[0] = "Kitty.bmp";
						

						int nRet = POS_PreDownloadBmpsToFlash(hPort,porttype,pBitImages, 1);
						if(POS_SUCCESS != nRet)
						{
							printf("Download logo failed!\n");
							goto INPUTAGAIN1;
						}
						bIsFirst = false;
					}

					int nRet = POS_SetMotionUnit(hPort,porttype,180, 180);

					if(POS_SUCCESS != nRet)
					{
						printf("Set moniton unit failed!\n");
						goto INPUTAGAIN1;
					}

					POS_SetMode(hPort,porttype,POS_PRINT_MODE_STANDARD);
	
					POS_SetRightSpacing(hPort,porttype,0);

					POS_SetLineSpacing(hPort,porttype,100);
					POS_S_TextOut(hPort,porttype,"POS Thermal Printer", 50, 2, 3, POS_FONT_TYPE_STANDARD, 
						POS_FONT_STYLE_NORMAL);
		
					POS_SetLineSpacing(hPort,porttype,35);

					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					POS_S_TextOut(hPort,porttype,"POS Thermal Printer", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_THICK_UNDERLINE);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"POS Thermal Printer", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_THIN_UNDERLINE);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					POS_SetLineSpacing(hPort,porttype,24);

	
					POS_SetRightSpacing(hPort,porttype,0);
					POS_S_TextOut(hPort,porttype,"BTP-R580", 20, 1, 1, POS_FONT_TYPE_STANDARD, 
						POS_FONT_STYLE_NORMAL);
					POS_S_TextOut(hPort,porttype,"Thermal Printer", 200, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);

					POS_SetRightSpacing(hPort,porttype,2);
					POS_S_TextOut(hPort,porttype,"BTP-98NP", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_S_TextOut(hPort,porttype,"Thermal Printer", 200, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);

					POS_SetRightSpacing(hPort,porttype,4);
					POS_S_TextOut(hPort,porttype,"BTP-2002CP", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_S_TextOut(hPort,porttype,"Thermal Printer", 200, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					//Print some strings of different fonts

					POS_SetRightSpacing(hPort,porttype,2);
					POS_S_TextOut(hPort,porttype,"Normal Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"Converse Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_REVERSE);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"Rotate Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_CLOCKWISE_90);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"Upside down Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_UPSIDEDOWN);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
	

					//Print a bar code

					POS_SetRightSpacing(hPort,porttype,0);

					POS_S_TextOut(hPort,porttype,"----------------------------------", 50, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);

					POS_S_TextOut(hPort,porttype,"Barcode - Code 128", 160, 1, 1, POS_FONT_TYPE_COMPRESSED,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					POS_S_SetBarcode(hPort,porttype,"{A*1234ABCDE*{C5678", 50, POS_BARCODE_TYPE_CODE128,
						2, 50, POS_FONT_TYPE_COMPRESSED, POS_HRI_POSITION_BOTH, 19);
					POS_FeedLine(hPort,porttype);
	
					POS_S_TextOut(hPort,porttype,"----------------------------------", 50, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);	
					POS_FeedLine(hPort,porttype);

					// Print the logos of FLASH

					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"-------------> Logo 1", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_S_PrintBmpInFlash(hPort,porttype,1, 20, POS_BITMAP_PRINT_NORMAL);

					POS_FeedLine(hPort,porttype);
					//POS_S_TextOut(hPort,porttype,"-------------> Logo 2", 20, 1, 1, POS_FONT_TYPE_STANDARD,
					//	POS_FONT_STYLE_NORMAL);
					//POS_FeedLine(hPort,porttype);
					//POS_S_PrintBmpInFlash(hPort,porttype,2, 20, POS_BITMAP_PRINT_NORMAL);

					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					POS_CutPaper(hPort,porttype,POS_CUT_MODE_FULL, 0);
					break;
				}
			case 1:{	//Print Sample of 80 wide and page mode
					//Download logos to RAM

					int nRet = POS_PreDownloadBmpToRAM(hPort,porttype,"Kitty.bmp", 0);// ID0
					if(POS_SUCCESS != nRet)
					{
						printf("Download logo failed!\n");
						goto INPUTAGAIN1;
					}

					POS_PreDownloadBmpToRAM(hPort,porttype,"Look.bmp", 1); // ID1

					POS_SetMotionUnit(hPort,porttype,180, 180);

					POS_SetMode(hPort,porttype,POS_PRINT_MODE_PAGE);	

					POS_PL_SetArea(hPort,porttype,10, 10, 620, 800, POS_AREA_BOTTOM_TO_TOP);
					POS_PL_Clear(hPort,porttype);

					POS_SetRightSpacing(hPort,porttype,0);

					POS_PL_TextOut(hPort,porttype,"Beiyang POS Thermal Printer", 20, 80, 2, 2, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_THICK_UNDERLINE);


					POS_SetRightSpacing(hPort,porttype,0);
					POS_PL_TextOut(hPort,porttype,"BTP-R580", 30, 140, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_TextOut(hPort,porttype,"Thermal Printer", 300, 140, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					POS_SetRightSpacing(hPort,porttype,4);
					POS_PL_TextOut(hPort,porttype,"BTP-98NP", 30, 180, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_TextOut(hPort,porttype,"Thermal Printer", 300, 180, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					POS_SetRightSpacing(hPort,porttype,8);
					POS_PL_TextOut(hPort,porttype,"BTP-2002CP", 30, 220, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_TextOut(hPort,porttype,"Thermal Printer", 300, 220, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					POS_SetRightSpacing(hPort,porttype,0);

					POS_PL_TextOut(hPort,porttype,"********************", 110, 260, 2, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					// Print a bar code

					POS_PL_TextOut(hPort,porttype,"Barcode - Code 128", 260, 290, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_SetBarcode(hPort,porttype,"{A*123ABC*{C34567890", 40, 360, POS_BARCODE_TYPE_CODE128, 3, 50,
						POS_FONT_TYPE_COMPRESSED, POS_HRI_POSITION_BELOW, 20);	

					// Print the logos of RAM

					POS_PL_PrintBmpInRAM(hPort,porttype,0, 50,  450, POS_BITMAP_PRINT_NORMAL);
					POS_PL_PrintBmpInRAM(hPort,porttype,0, 230, 450, POS_BITMAP_PRINT_NORMAL);
					POS_PL_PrintBmpInRAM(hPort,porttype,1, 410, 450, POS_BITMAP_PRINT_NORMAL);
					POS_PL_PrintBmpInRAM(hPort,porttype,1, 530, 450, POS_BITMAP_PRINT_NORMAL);

					POS_PL_Print(hPort,porttype);
					POS_PL_Clear(hPort,porttype);
					POS_CutPaper(hPort,porttype,POS_CUT_MODE_PARTIAL, 150);

					break;
				}
			case 2:{	//Print Sample of 56 wide and stand mode
					static bool bIsFirst = true;
					if (bIsFirst)
					{
						// Download logos to Flash

						char *pBitImages[1];
						pBitImages[0] = "Kitty.bmp";
					//BitImages[1] = "Look.bmp";

						int nRet = POS_PreDownloadBmpsToFlash(hPort,porttype,pBitImages, 1);

						if(POS_SUCCESS != nRet)
						{
							printf("Download logo failed!\n");
							goto INPUTAGAIN1;
						}

						bIsFirst = false;
					}

					int nRet = POS_SetMotionUnit(hPort,porttype,180, 180);

					if(POS_SUCCESS != nRet)
					{
						printf("Set moniton unit failed!\n");
						goto INPUTAGAIN1;
					}

					POS_SetMode(hPort,porttype,POS_PRINT_MODE_STANDARD);
	
					POS_SetRightSpacing(hPort,porttype,0);

					POS_SetLineSpacing(hPort,porttype,100);
					POS_S_TextOut(hPort,porttype,"POS Thermal Printer", 30, 1, 2, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
		
					POS_SetLineSpacing(hPort,porttype,35);

					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"POS Thermal Printer", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_THICK_UNDERLINE);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"POS Thermal Printer", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_THIN_UNDERLINE);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					POS_SetLineSpacing(hPort,porttype,24);
	
					POS_SetRightSpacing(hPort,porttype,0);
					POS_S_TextOut(hPort,porttype,"BTP-R580", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_S_TextOut(hPort,porttype,"POS Printer", 200, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);

					POS_SetRightSpacing(hPort,porttype,2);
					POS_S_TextOut(hPort,porttype,"BTP-98NP", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_S_TextOut(hPort,porttype,"POS Printer", 200, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);

					POS_SetRightSpacing(hPort,porttype,4);
					POS_S_TextOut(hPort,porttype,"BTP-2002CP", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_S_TextOut(hPort,porttype,"POS Printer", 200, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					//Print some strings of different fonts

					POS_SetRightSpacing(hPort,porttype,5);
					POS_S_TextOut(hPort,porttype,"Normal Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"Converse Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_REVERSE);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"Rotate Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_CLOCKWISE_90);
					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"Upside down Font", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_UPSIDEDOWN);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					// Print a bar code

					POS_SetRightSpacing(hPort,porttype,0);
	
					POS_S_TextOut(hPort,porttype,"-----------------------", 50, 1, 1, POS_FONT_TYPE_STANDARD, 
						POS_FONT_STYLE_NORMAL);

					POS_FeedLine(hPort,porttype);

					POS_S_TextOut(hPort,porttype,"Barcode - Code 128", 100, 1, 1, POS_FONT_TYPE_COMPRESSED,
						POS_FONT_STYLE_NORMAL);

					POS_FeedLine(hPort,porttype);

					POS_S_SetBarcode(hPort,porttype,"{A*123AB{C567", 40, POS_BARCODE_TYPE_CODE128, 2, 50,
						POS_FONT_TYPE_COMPRESSED, POS_HRI_POSITION_BOTH, 13);
	
					POS_S_TextOut(hPort,porttype,"-----------------------", 50, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					POS_FeedLine(hPort,porttype);

					// Print the logos of FLASH

					POS_FeedLine(hPort,porttype);
					POS_S_TextOut(hPort,porttype,"-------------> Logo 1", 20, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_FeedLine(hPort,porttype);
					POS_S_PrintBmpInFlash(hPort,porttype,1, 20, POS_BITMAP_PRINT_NORMAL);
					POS_FeedLine(hPort,porttype);

					//POS_S_TextOut(hPort,porttype,"-------------> Logo 2", 20, 1, 1, POS_FONT_TYPE_STANDARD,
					//	POS_FONT_STYLE_NORMAL);
					//POS_FeedLine(hPort,porttype);
					//POS_S_PrintBmpInFlash(hPort,porttype,2, 20, POS_BITMAP_PRINT_NORMAL);

					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);
					POS_FeedLine(hPort,porttype);

					POS_CutPaper(hPort,porttype,POS_CUT_MODE_FULL, 0);
					break;
				}
			case 3:{	//Print Sample of 56 wide and page mode

					// Download logos to RAM

					int nRet = POS_PreDownloadBmpToRAM(hPort,porttype,"Kitty.bmp", 0); // ID 0
					if(POS_SUCCESS != nRet)
					{
						printf("Download logo failed!\n");
						goto INPUTAGAIN1;
					}
					POS_PreDownloadBmpToRAM(hPort,porttype,"Look.bmp", 1); // ID 1

					POS_SetMotionUnit(hPort,porttype,180, 180);

					POS_SetMode(hPort,porttype,POS_PRINT_MODE_PAGE);	

					POS_PL_SetArea(hPort,porttype,10, 10, 440, 800, POS_AREA_BOTTOM_TO_TOP);

					POS_PL_Clear(hPort,porttype);

					POS_SetRightSpacing(hPort,porttype,0);

					POS_PL_TextOut(hPort,porttype,"Beiyang POS Thermal Printer", 0, 50, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_THICK_UNDERLINE);

					POS_SetRightSpacing(hPort,porttype,0);

					POS_PL_TextOut(hPort,porttype,"BTP-R580", 5, 80, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_TextOut(hPort,porttype,"POS Thermal Printer", 230, 80, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);


					POS_SetRightSpacing(hPort,porttype,4);
					POS_PL_TextOut(hPort,porttype,"BTP-98NP", 5, 110, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_TextOut(hPort,porttype,"POS Thermal Printer", 230, 110, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					POS_SetRightSpacing(hPort,porttype,8);
					POS_PL_TextOut(hPort,porttype,"BTP-2002CP", 5, 140, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
					POS_PL_TextOut(hPort,porttype,"POS Thermal Printer", 230, 140, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);
	
					POS_SetRightSpacing(hPort,porttype,0);
					POS_SetLineSpacing(hPort,porttype,0);

					POS_PL_TextOut(hPort,porttype,"********************", 70, 170, 2, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					// Print the bar code

					POS_PL_TextOut(hPort,porttype,"Barcode - Code 128", 180, 195, 1, 1, POS_FONT_TYPE_STANDARD,
						POS_FONT_STYLE_NORMAL);

					POS_PL_SetBarcode(hPort,porttype,"{A*12345ABC*{C90", 5, 260, POS_BARCODE_TYPE_CODE128, 3, 50,
						POS_FONT_TYPE_COMPRESSED, POS_HRI_POSITION_BELOW, 16);	
	
					// Print the logos of RAM

					POS_PL_PrintBmpInRAM(hPort,porttype,0, 50, 330, POS_BITMAP_PRINT_NORMAL);	
					POS_PL_PrintBmpInRAM(hPort,porttype,1, 410, 330, POS_BITMAP_PRINT_NORMAL);


					POS_PL_Print(hPort,porttype);

					POS_PL_Clear(hPort,porttype);

					POS_CutPaper(hPort,porttype,POS_CUT_MODE_PARTIAL, 150);
					break;
				}
			case 4:	
					Status = 0;
					
					if ((porttype == 0) || (porttype == 2))
					{	
                                        	nRet = POS_RTQueryStatus(hPort,porttype,&Status);

					}
					else if (porttype == 3)
					{
						nRet = POS_NETQueryStatus(porttype,IP,&Status);
					}
					else
					{
						printf("The data inputed is error!Please select again!\n");
						goto INPUTAGAIN1;
					}
					if(POS_SUCCESS != nRet)
					{
						printf("Get status failed!\n");
						goto INPUTAGAIN1;
					}
					if(Status == 1)
					{
						printf("All is ok!\n");
					}
					else
					{
						int iBits[8];
						int i = 0;
						for (i; i < 8; i++)
						{
							iBits[i] = (Status >> i) & 0x01;
						}
						if (iBits[0] == 0)
						{
							printf( "Cashdrawer open!\n");
						}
			
						if (iBits[1] == 1)
						{
							printf("Printer offline!\n");
						}
			
						if (iBits[2] == 1)
						{
							printf("Door open!\n");
						}

						if (iBits[3] == 1)
						{
							printf( "Feeding!\n");
						}
			
						if (iBits[4] == 1)
						{
							printf( "Printer error!\n");
						}
			
						if (iBits[5] == 1)
						{
							printf( "Cutter error!\n");
						}
			
						if (iBits[6] == 1)
						{
							printf( "Paper near end!\n");
						}
			
						if (iBits[7] == 1)
						{
							printf( "Paper end!\n");
						}
					}
					printf("Press \"Enter\" to continue...");
					ClearStdin;
					break;

			case 5:	goto INPUTAGAIN;
			case 6:	goto EXITLINE;

			default:	printf("The data inputed is error!Please select again!\n");	
					goto INPUTAGAIN1;
			}
		}
		ClearStdin;
	}
//**************************************************************
//Exit main function
EXITLINE:
	POS_Close(hPort,porttype);
	return 1;
}


