//=============================================================================
// File Name : option.h
//=============================================================================

#define	FCLK		399650000		// 399.650MHz (400MHz)

#define	HCLK		(FCLK/4)
#define	PCLK		(FCLK/8)

/*************************************/
/* Memory Configuration (SDRAM 32MB) */
/*************************************/

// LCD Frame Buffer
#define END_OF_SDRAM	0x32000000				   	// 32MB
#define LCD_FB0			(END_OF_SDRAM - 0x800000)  	// 0x31800000 (must be 4BM align), size�� 2MB�� �Ҵ�
#define LCD_FB1			(END_OF_SDRAM - 0x400000)  	// 0x31C00000 (must be 4BM align), size�� 2MB�� �Ҵ�

// Non Cache Area
#define NON_CACHE_AREA	LCD_FB0						// FB���� 6MB (0x31800000~0x31e00000) Non-Cache ó��

// Heap End Address
#define	HEAP_END_ADDRESS	(NON_CACHE_AREA - 4)	// Heap : ZI_LIMIT ~ FB ���� �ּұ���

// MMU Translation Table �ּ� : �޸� ������ 16KB
#define _MMUTT_STARTADDRESS	(END_OF_SDRAM - 0x4000)	// MMUTT size => 0x1000(16KB)

// Stack Limit Address : 0x31E00000 ~ MMU-TT
#define END_OF_STACK	(LCD_FB1 + 0x200000)		// 6���� ���� �ִ������ => 2MB - 16KB
