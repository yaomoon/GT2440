
#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            50000000    // init.c�е�clock_init��������PCLKΪ50MHz
#define UART_CLK        PCLK        //  UART0��ʱ��Դ��ΪPCLK
#define UART_BAUD_RATE  115200      // ������
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
    // LOCKTIME = 0x00ffffff;   // ʹ��Ĭ��ֵ����
    CLKDIVN  = 0x03;            // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1

    /* ���HDIVN��0��CPU������ģʽӦ�ôӡ�fast bus mode����Ϊ��asynchronous bus mode�� */
__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        /* �������ƼĴ��� */ 
    "orr    r1, r1, #0xc0000000\n"          /* ����Ϊ��asynchronous bus mode�� */
    "mcr    p15, 0, r1, c1, c0, 0\n"        /* д����ƼĴ��� */
    );

    /* �ж���S3C2410����S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
        MPLLCON = S3C2410_MPLL_200MHZ;
    }
    else
    {
        MPLLCON = S3C2440_MPLL_200MHZ;  /* ���ڣ�FCLK=200MHz,HCLK=100MHz,PCLK=50MHz */
    }       
}


/*
 * ��ʼ��UART0
 * 115200,8N1,������
 */
void uart0_init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3����TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3�ڲ�����

    ULCON0  = 0x03;     // 8N1(8������λ���޽��飬1��ֹͣλ)
    UCON0   = 0x05;     // ��ѯ��ʽ��UARTʱ��ԴΪPCLK
    UFCON0  = 0x00;     // ��ʹ��FIFO
    UMCON0  = 0x00;     // ��ʹ������
    UBRDIV0 = UART_BRD; // ������Ϊ115200
}

/*
 * ����һ���ַ�
 */
static void my_putc(unsigned char c)
{
    /* �ȴ���ֱ�����ͻ������е������Ѿ�ȫ�����ͳ�ȥ */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* ��UTXH0�Ĵ�����д�����ݣ�UART���Զ��������ͳ�ȥ */
    UTXH0 = c;
}

/*
 * �����ַ�
 */
static unsigned char my_getc(void)
{
    /* �ȴ���ֱ�����ջ������е������� */
    while (!(UTRSTAT0 & RXD0READY));
    
    /* ֱ�Ӷ�ȡURXH0�Ĵ��������ɻ�ý��յ������� */
    return URXH0;
}

/*
 * �ж�һ���ַ��Ƿ�����
 */
static int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;       
}

/*
 * �ж�һ���ַ��Ƿ�Ӣ����ĸ
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
