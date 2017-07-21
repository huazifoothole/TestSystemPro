#ifndef DEF_H
#define DEF_H

#define LATECH_PATH "/etc/LATECH"
#define SO_PATH		"/home/oneu/ccpos/dist/driver/la"

#ifndef WORD
#define WORD                unsigned short
#endif
#ifndef DWORD
#define DWORD               unsigned long
#endif
#ifndef LONG
#define LONG                long
#endif
#ifndef BYTE
#define BYTE                unsigned char
#endif

#ifndef TRUE
#define	TRUE	1
#endif

#ifndef FALSE
#define	FALSE	0
#endif

#define		NO_ERROR	0	//正确

//***********************************************************************
//						打印机相关定义
//***********************************************************************
#define NO_PRINTER      1
#define DATA_LINE_ERROR 2   //*
#ifndef POWER_ERROR
#define POWER_ERROR     3   //*
#endif
#define PRINTER_IS_BUSY 4
#define	TIME_OUT		5
#ifndef GET_HWVERSION_ERROR
#define	GET_HWVERSION_ERROR 6
#endif
#define	START_ERRORR    7
#define	COVER_ERROR		8
#define	PAPER_EMPTY_ERROR	9
#define	PAPER_JAM_ERROR		10
#define	PAPER_FEW_ERROR     11	//卷将纸错误
#define	PRINT_NOT_COMPLETE	12	//*上一打印未完成
#define	PRINTER_CUTTER_ERROR    13  //卷将纸错误
#define SET_CUTMODE_ERROR           20
#define SET_FONT_MATRIX_ERROR       21
#define SET_FONT_SIZE_ERROR         22
#define SET_FONT_ALIGNMENT_ERROR    23
#define SET_FONT_EMPHA_ERROR        24
#define SET_LINE_SPACE_ERROR        25
#define SET_CHAR_SPACE_ERROR        26
#define SET_LEFT_MARGIN_ERROR       27

#define PRINTER_CONFIG_ERROR    100     //参数文件
#define PRINTER_NOT_INIT        101     //
#define SET_AREA_WIDTH_ERROR    102
#ifndef OTHER_ERROR
#define	OTHER_ERROR		        500     //其它错误
#endif

//打印机能力定义
#define		MODE_SUPPORT_CHAR		0x01	//支持字符模式
#define		MODE_SUPPORT_IMAGE		0x02	//支持图像模式
#define		MODE_SUPPORT_VLEN_CUT	0x04	//支持不定长切纸模式
#ifndef     MODE_SUPPORT_BMARK
#define		MODE_SUPPORT_BMARK	0x08	//支持大黑标
#endif
#ifndef     MODE_SUPPORT_SMARK
#define		MODE_SUPPORT_SMARK	0x10	//支持小黑标
#endif
#ifndef     MODE_SUPPORT_2DBAR
#define		MODE_SUPPORT_2DBAR	0x20	//支持打印二维条码（PDF417）
#endif
/*old
//打印机返回值定义
#define		PRINTER_TIME_OUT		-1		//超时
#define		PRINTER_DATA_ERROR		-2		//数据线路故障
#define		PRINTER_PAPER_ERROR		-3		//卷纸错误
#define		PRINTER_POWER_ERROR		-4		//电源线故障
#define		PRINTER_COVER_ERROR		-5		//打印机上盖打开
#define		PRINTER_PAPER_JAM		-6		//卡纸
#define		PRINTER_NOT_COMPELTE	-7		//上一打印未完成
#define		PRINTER_OTHER_ERROR		-8		//其它错误

//打印机能力定义
#define		MODE_SUPPORT_CHAR		0x01	//支持字符模式
#define		MODE_SUPPORT_IMAGE		0x02	//支持图像模式
*/
//***********************************************************************
//						读票机相关定义
//***********************************************************************

//返回值定义
#define		NO_SCANNER				1001       //找不到读票机
#define 	PORT_ERROR				1002	//数据线故障
#ifndef     POWER_ERROR
#define		POWER_ERROR				1003	//电源线故障
#endif
#define		SCANNER_IS_BUSY			1004	//读票机忙
#ifndef     TIME_OUT
#define		TIME_OUT				1005	//超时
#endif
#define		SCANNER_DISABLED		1006	//	读票机被禁用
#ifndef     GET_HWVERSION_ERROR
#define		GET_HWVERSION_ERROR		1007	//	获取硬件信息失败
#endif
#define		START_ERROR				1008	//启动读票机失败
#define		TABPAR_NONE				1009	//参数文件不存在
#define		GET_ORIGIN_IMAGE_ERROR	1010       //获取原始图像数据失败
#define		SCANNER_READ_ERROR		1011	//读票机彩票（投注单）数据读取错误
#define		SCANNER_JAM_PAPER_ERROR	1012	//卡纸
#define		SCANNER_CONVEY_ERROR	1013	//运送失败
#define		ALREADY_BRAND_ERROR		1014	//标记已打印
#ifndef     OTHER_ERROR
#define		OTHER_ERROR				1500	//其它错误（未知错误）
#endif

#define		SCANNER_READ			1501	//读票机正在读票或投注单
#define		SCANNER_WAIT_ROLLBACK	1502	//读票机读票（或标记打印）完成（彩票还未退出）
#define		SCANNER_PRINT_BRAND		1503	//读票机正在打印标记
#define		SCANNER_ROLLBACK		1504	//读票机正在退出彩票
#define		BRAND_INDEX_ERROR		1511	//硬件不支持图像标记打印，打印为黑块时（成功）返回
#define		BRAND_IMAGE_ERROR		1512	//硬件不支持自定义图像标记打印，而打印为内置标记时返回
#define		BRAND_XPOS_ERROR		1513	//标记打印成功，但水平位置设置不成功时返回
#define		BRAND_YPOS_ERROR		1514	//标记打印成功，但垂直位置设置不成功时返回


//读票机能力定义
#define		MODE_SUPPORT_BRAND				0x0001	//支持标记打印
#define		MODE_SUPPORT_RAW				0x0002	//支持获取原始图像及图像识别
#define		MODE_SUPPORT_CUSTOM_BRAND		0x0004	//支持用户自定义标记打印
#define		MODE_SUPPORT_CALLBACK			0x0008	//支持回调方式
#define		MODE_SUPPORT_BRAND_XPOS        	0x0010    //支持标记打印自定义水平位置
#define		MODE_SUPPORT_BRAND_YPOS        	0x0020    //支持标记打印自定义垂直位置
#ifndef     MODE_SUPPORT_BMARK
#define		MODE_SUPPORT_BMARK				0x0040    //支持大黑标
#endif
#ifndef     MODE_SUPPORT_SMARK
#define		MODE_SUPPORT_SMARK        		0x0080    //支持小黑标
#endif
#ifndef     MODE_SUPPORT_2DBAR
#define		MODE_SUPPORT_2DBAR				0x0100    //支持二维条码(PDF417)
#endif

//*****************************************************************
//                  条码枪相关定义
//****************************************************************
//接口返回值定义
#ifndef NO_ERROR
#define NO_ERROR                0
#endif
#define NO_HSCANNER             2001
#ifndef DATA_LINE_ERROR
#define DATA_LINE_ERROR         2002
#endif
#ifndef POWER_ERROR
#define POWER_ERROR             2003
#endif
#define HSCANNER_IS_BUSY        2004
#ifndef TIME_OUT
#define TIME_OUT                2005
#endif
#define HSCANNER_DISABLED       2006
#ifndef GET_HWVERSION_ERROR
#define GET_HWVERSION_ERROR     2007
#endif
#ifndef START_ERROR
#define START_ERROR             2008
#endif
#ifndef TABPAR_NONE
#define TABPAR_NONE             2009
#endif
#ifndef GET_ORIGIN_IMAGE_ERROR
#define GET_ORIGIN_IMAGE_ERROR  2010
#endif
#define HSCANNER_READ_ERROR     2011
#ifndef OTHER_ERROR
#define OTHER_ERROR             2500
#endif

//一维条码类型定义
#define Interleaved25	            1
#define UPCEAN	                    2
#define UPCEAN_WithSupplementals	3
#define BooklanEAN	                4
#define ISSN	                    5
#define UCCCouponExtendedCode	    6
#define Code128	                    7
#define GS1_128	                    8
#define ISBT128	                    9
#define Code39	                    10
#define Code39_FullASCII	        11
#define TriopticCode39	            12
#define Code32	                    13
#define Code93	                    14
#define Code11	                    15
#define Matrix25	                16
#define Discrete25	                17
#define Codabar	                    18
#define MSI	                        19
#define Chinese25	                20
#define GS1_DataBarVariants	        21
#define Korean35	                22
#define ISBT_Concat	                23
//二维条码类型定义
#define DataMatrix	                51
#define PDF417	                    52
#define MicroPDF417	                53
#define TLC39	                    54
#define CompositeCodes	            55
#define Maxicode	                56
#define QRCode	                    57
#define MicroQR	                    58
#define Aztec	                    59

//蜂鸣参数定义
#define ONESHORTHI          0x00
#define TWOSHORTHI          0x01
#define THREESHORTHI        0x02
#define FOURSHORTHI         0x03
#define FIVESHORTHI         0x04
#define ONESHORTLO          0x05
#define TWOSHORTLO          0x06
#define THREESHORTLO        0x07
#define FOURSHORTLO         0x08
#define FIVESHORTLO         0x09
#define ONELONGHI          0x0A
#define TWOLONGHI          0x0B
#define THREELONGHI        0x0C
#define FOURLONGHI         0x0D
#define FIVELONGHI         0x0E
#define ONELONGLO          0x0F
#define TWOLONGLO          0x10
#define THREELONGLO        0x11
#define FOURLONGLO         0x12
#define FIVELONGLO         0x13
#define FASTHILOHILO        0x14
#define SLOWHILOHILO        0x15
#define HILO                0x16
#define LOHI                0x17
#define HILOHI          0x18
#define LOHILO          0x19


//***********************************************************************
//						UPS相关定义
//***********************************************************************

//UPS状态定义
#define		UPS_MAJOR_ELECT			1		//市电供电
#define		UPS_MINOR_ELECT			0		//备电供电

//***********************************************************************
//						顾显相关定义
//***********************************************************************

//显示方式定义
#define		CDU_DISP_NORMAL		0		//正常显示
#define		CDU_DISP_FLTR		1		//从右往左出显
#define		CDU_DISP_GLINT		2		//闪烁

//***********************************************************************
//						读卡器相关定义
//***********************************************************************

//读卡器能力定义
#define		MODE_SUPPORT_CARD		0x01		//支持磁卡
#define		MODE_KEYBORAD_INTERFACE	0x02		//键盘口


//各个库相同接口名
typedef void(*SCANNERCALLBACK)(int eventId,void* data);
typedef bool (*GetHWInformation)(char* const hwInfo,unsigned int const length);
typedef bool (*GetSWVersion)(char* const swVersion , unsigned int const length);
typedef bool (*ScanIsComplete)();
typedef int (*GetLastErrorCode)();
typedef void (*GetLastErrorStr)(char* const errStr, unsigned int const length);

/**
 * @brief The ERRTYPE enum
 * add by vince 2013-04-09
 * use to define the devices' signal like ScannerError(int errType, QString tips);
 */
enum ERRTYPE
{
	TYPE_API_NULL = -3,
	TYPE_STATUS = -2,
	TYPE_INFO = -1
};

#endif // DEF_H
