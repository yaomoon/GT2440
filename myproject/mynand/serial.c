
#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            50000000    // init.c中的clock_init函数设置PCLK为50MHz
#define UART_CLK        PCLK        //  UART0的时钟源设为PCLK
#define UART_BAUD_RATE  115200      // 波特率
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)


#ifndef GPHCON
#define GPHCON              (*(volatile unsigned long *)0x56000070)
#endif

#ifndef GPHUP
#define GPHUP               (*(volatile unsigned long *)0x56000078)
#endif

#ifndef ULCON0
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#endif

#ifndef UCON0
#define UCON0               (*(volatile unsigned long *)0x50000004)
#endif


#ifndef UFCON0
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#endif

#ifndef UMCON0
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#endif

#ifndef UBRDIV0
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)
#endif

#ifndef UTRSTAT0
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#endif

#ifndef UTXH0
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#endif

#ifndef URXH0
#define URXH0               (*(volatile unsigned char *)0x50000024)
#endif


#ifndef CLKDIVN
#define CLKDIVN     (*(volatile unsigned long *)0x4c000014)
#endif
#ifndef GSTATUS1
#define GSTATUS1    (*(volatile unsigned long *)0x560000B0)
#endif
#ifndef MPLLCON
#define MPLLCON         (*(volatile unsigned long *)0x4c000004)
#endif

#ifndef S3C2410_MPLL_200MHZ
#define S3C2410_MPLL_200MHZ    ((0x5c<<12)|(0x04<<4)|(0x00))
#endif
#ifndef S3C2440_MPLL_200MHZ
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#endif


void clock_init(void)
{
    // LOCKTIME = 0x00ffffff;   // 使用默认值即可
    CLKDIVN  = 0x03;            // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1

    /* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        /* 读出控制寄存器 */ 
    "orr    r1, r1, #0xc0000000\n"          /* 设置为“asynchronous bus mode” */
    "mcr    p15, 0, r1, c1, c0, 0\n"        /* 写入控制寄存器 */
    );

    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
        MPLLCON = S3C2410_MPLL_200MHZ;
    }
    else
    {
        MPLLCON = S3C2440_MPLL_200MHZ;  /* 现在，FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
    }       
}


/*
 * 初始化UART0
 * 115200,8N1,无流控
 */
void uart0_init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3内部上拉

    ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
    UCON0   = 0x05;     // 查询方式，UART时钟源为PCLK
    UFCON0  = 0x00;     // 不使用FIFO
    UMCON0  = 0x00;     // 不使用流控
    UBRDIV0 = UART_BRD; // 波特率为115200
}

/*
 * 发送一个字符
 */
static void my_putc(unsigned char c)
{
    /* 等待，直到发送缓冲区中的数据已经全部发送出去 */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* 向UTXH0寄存器中写入数据，UART即自动将它发送出去 */
    UTXH0 = c;
}

/*
 * 接收字符
 */
static unsigned char my_getc(void)
{
    /* 等待，直到接收缓冲区中的有数据 */
    while (!(UTRSTAT0 & RXD0READY));
    
    /* 直接读取URXH0寄存器，即可获得接收到的数据 */
    return URXH0;
}

/*
 * 判断一个字符是否数字
 */
static int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;       
}

/*
 * 判断一个字符是否英文字母
 */
static int isLetter(unsigned char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    else if (c >= 'A' && c <= 'Z')
        return 1;       
    else
        return 0;
}

void my_puts(unsigned char *str)
{
    while(*str)
    {
        my_putc(*str);
        str++;
    }
}
