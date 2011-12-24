
#define GSTATUS1        (*(volatile unsigned int *)0x560000B0) //判断是2410或者2440的寄存器
#define BUSY            1 //读NFSTAT是用，它为1说明不忙

typedef unsigned int S3C24X0_REG32;


/* NAND FLASH (see S3C2410 manual chapter 6) */ //定义2410各个寄存器，给该
结构体一个初地址，则各个成员就能对应到每个寄存器，这几个寄存器地址是连续的
typedef struct {
    S3C24X0_REG32   NFCONF; //包括使能nand控制器，初始化ECC，nand片选，
                                //相关时序参数
    S3C24X0_REG32   NFCMD;// nand 命令寄存器
    S3C24X0_REG32   NFADDR;//地址寄存器
    S3C24X0_REG32   NFDATA; //数据寄存器
    S3C24X0_REG32   NFSTAT; //状态寄存器
    S3C24X0_REG32   NFECC;  //与ECC相关寄存器
} S3C2410_NAND;

/* NAND FLASH (see S3C2440 manual chapter 6, www.100ask.net) */
//2440相关寄存器
typedef struct {
    S3C24X0_REG32   NFCONF; //主要设置时序寄存器
    S3C24X0_REG32   NFCONT; //包括nand控制器使能，nand片选，初始化ECC，
    S3C24X0_REG32   NFCMD;
    S3C24X0_REG32   NFADDR;
    S3C24X0_REG32   NFDATA;
    S3C24X0_REG32   NFMECCD0;
    S3C24X0_REG32   NFMECCD1;
    S3C24X0_REG32   NFSECCD;
    S3C24X0_REG32   NFSTAT;
    S3C24X0_REG32   NFESTAT0;
    S3C24X0_REG32   NFESTAT1;
    S3C24X0_REG32   NFMECC0;
    S3C24X0_REG32   NFMECC1;
    S3C24X0_REG32   NFSECC;
    S3C24X0_REG32   NFSBLK;
    S3C24X0_REG32   NFEBLK;
} S3C2440_NAND;

// nand的相关操作函数,这里都是函数指针,2410和2440分别有自己的函数，函数指针
// 用来指向他们
typedef struct {
    void (*nand_reset)(void); //复位 ,发复位命令
    void (*wait_idle)(void); //忙等待
    void (*nand_select_chip)(void);// 片选选中
    void (*nand_deselect_chip)(void);//片选取消，每次操作前选中，操作后取消
    void (*write_cmd)(int cmd); // 发送命令,给相应寄存器赋值
    void (*write_addr)(unsigned int addr);//发送地址,要安装其地址组成赋值
    unsigned char (*read_data)(void); //读数据,读相关寄存器
}t_nand_chip;

//赋寄存器基地址
static S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4e000000;
static S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

static t_nand_chip nand_chip;

/* 供外部调用的函数 */
void nand_init(void);
//读nand，将main代码读到sdram中
void nand_read(unsigned char *buf, unsigned long start_addr, int size);

/* NAND Flash操作的总入口, 它们将调用S3C2410或S3C2440的相应函数 */
static void nand_reset(void);
static void wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void write_cmd(int cmd);
static void write_addr(unsigned int addr);
static unsigned char read_data(void);

/* S3C2410的NAND Flash处理函数 */
static void s3c2410_nand_reset(void);
static void s3c2410_wait_idle(void);
static void s3c2410_nand_select_chip(void);
static void s3c2410_nand_deselect_chip(void);
static void s3c2410_write_cmd(int cmd);
static void s3c2410_write_addr(unsigned int addr);
static unsigned char s3c2410_read_data();

/* S3C2440的NAND Flash处理函数 */
static void s3c2440_nand_reset(void);
static void s3c2440_wait_idle(void);
static void s3c2440_nand_select_chip(void);
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void);

/* S3C2410的NAND Flash操作函数 */

/* 复位 */
static void s3c2410_nand_reset(void)
{
    s3c2410_nand_select_chip();
    s3c2410_write_cmd(0xff);  // 复位命令
    s3c2410_wait_idle();
    s3c2410_nand_deselect_chip();
}

/* 等待NAND Flash就绪 */
static void s3c2410_wait_idle(void)
{
    int i;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void s3c2410_nand_select_chip(void)
{
    int i;
    s3c2410nand->NFCONF &= ~(1<<11);
    for(i=0; i<10; i++);    
}

/* 取消片选信号 */
static void s3c2410_nand_deselect_chip(void)
{
    s3c2410nand->NFCONF |= (1<<11);
}

/* 发出命令 */
static void s3c2410_write_cmd(int cmd)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFCMD;
    *p = cmd;
}

/* 发出地址 */
static void s3c2410_write_addr(unsigned int addr)
{
    int i;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFADDR;
    
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}

/* 读取数据 */
static unsigned char s3c2410_read_data(void)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFDATA;
    return *p;
}

/* S3C2440的NAND Flash操作函数 */

/* 复位 */
static void s3c2440_nand_reset(void)
{
    s3c2440_nand_select_chip();
    s3c2440_write_cmd(0xff);  // 复位命令
    s3c2440_wait_idle();
    s3c2440_nand_deselect_chip();
}

/* 等待NAND Flash就绪 */
static void s3c2440_wait_idle(void)
{
    int i;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;
    while(!(*p & BUSY))
        for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void s3c2440_nand_select_chip(void)
{
    int i;
    s3c2440nand->NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

/* 取消片选信号 */
static void s3c2440_nand_deselect_chip(void)
{
    s3c2440nand->NFCONT |= (1<<1);
}

/* 发出命令 */
static void s3c2440_write_cmd(int cmd)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
    *p = cmd;
}

/* 发出地址 */
static void s3c2440_write_addr(unsigned int addr)
{
    int i;
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
    
    *p = addr & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    *p = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}

/* 读取数据 */
static unsigned char s3c2440_read_data(void)
{
    volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
    return *p;
}


/* 在第一次使用NAND Flash前，复位一下NAND Flash */
static void nand_reset(void)
{
    nand_chip.nand_reset();
}

static void wait_idle(void)
{
    nand_chip.wait_idle();
}

static void nand_select_chip(void)
{
    int i;
    nand_chip.nand_select_chip();
    for(i=0; i<10; i++);
}

static void nand_deselect_chip(void)
{
    nand_chip.nand_deselect_chip();
}

static void write_cmd(int cmd)
{
    nand_chip.write_cmd(cmd);
}
static void write_addr(unsigned int addr)
{
    nand_chip.write_addr(addr);
}

static unsigned char read_data(void)
{
    return nand_chip.read_data();
}


/* 初始化NAND Flash */
void nand_init(void)
{
#define TACLS   0
#define TWRPH0  3
#define TWRPH1  0

    /* 判断是S3C2410还是S3C2440 */
    if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
    {
        nand_chip.nand_reset         = s3c2410_nand_reset;
        nand_chip.wait_idle          = s3c2410_wait_idle;
        nand_chip.nand_select_chip   = s3c2410_nand_select_chip;
        nand_chip.nand_deselect_chip = s3c2410_nand_deselect_chip;
        nand_chip.write_cmd          = s3c2410_write_cmd;
        nand_chip.write_addr         = s3c2410_write_addr;
        nand_chip.read_data          = s3c2410_read_data;

		/* 使能NAND Flash控制器, 初始化ECC, 禁止片选, 设置时序 */
        s3c2410nand->NFCONF = (1<<15)|(1<<12)|(1<<11)|(TACLS<<8)|(TWRPH0<<4)|(TWRPH1<<0);
    }
    else
    {
        nand_chip.nand_reset         = s3c2440_nand_reset;
        nand_chip.wait_idle          = s3c2440_wait_idle;
        nand_chip.nand_select_chip   = s3c2440_nand_select_chip;
        nand_chip.nand_deselect_chip = s3c2440_nand_deselect_chip;
        nand_chip.write_cmd          = s3c2440_write_cmd;
        nand_chip.write_addr         = s3c2440_write_addr;
        nand_chip.read_data          = s3c2440_read_data;

		/* 设置时序 */
        s3c2440nand->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
        /* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
        s3c2440nand->NFCONT = (1<<4)|(1<<1)|(1<<0);
    }
    
    /* 复位NAND Flash */
    nand_reset();
}


#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

/* 读函数 */
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
    int i, j;
    
    if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
        return ;    /* 地址或长度不对齐 */
    }

    /* 选中芯片 */
    nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* 发出READ0命令 */
      write_cmd(0);

      /* Write Address */
      write_addr(i);
      wait_idle();

//可以连续读512字节.即一页
      for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
          *buf = read_data();
          buf++;
      }
    }

    /* 取消片选信号 */
    nand_deselect_chip();
    
    return ;
}

