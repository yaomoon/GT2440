
#define GPBCON (*(volatile unsigned long *) 0x56000010)
#define GPBDAT (*(volatile unsigned long *) 0x56000014)

void wait( unsigned long delay)
{
    for(; delay>0; delay--);
}

void led_init(void)
{
    GPBCON = (1<<(5*2)) | (1<<(6*2)) | (1<<(7*2)) | (1<<(8*2));
}
void main(void)
{
    
    int i;

    led_init();

    while(1)
    {
        for (i = 5; i < 9 ; i++) 
        {
            wait(30000);
            GPBDAT = (~(1 << i));
        }
        
    }
    
}

