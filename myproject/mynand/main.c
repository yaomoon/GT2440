
#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))

void my_puts(unsigned char *str);
void uart0_init(void);

void clock_init(void);

void  wait(unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main(void)
{
	unsigned long i = 0;
	
	GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;		// 将LED1-4对应的GPB5/6/7/8四个引脚设为输出
    clock_init();

    uart0_init();
    my_puts("this is nand test \r\n");
    my_puts("hello yaomoon \r\n");
	while(1){
		wait(300000);
		GPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1-4
		if(++i == 16)
			i = 0;
	}

	return 0;
}
