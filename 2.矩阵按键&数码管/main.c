#include <STC15F2K60S2.H>
#include <intrins.h>
/*************  ���س�������    **************/
unsigned char code t_display[]={                       //��׼�ֿ�
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //λ��
unsigned char Smg_buf[8] = {0,1,2,3,4,5,6,12};	//�������ʱ��������
unsigned char Led_buf[8] = {0,0,0,0,0,0,0,0};	//LED��ʱ��������
unsigned char Eignt_pos;	//ѭ��8�α���
unsigned int Smg_SlowDown;	//����ܼ��ٱ���
bit Flag = 0;	//ÿ����˸��־λ
void HC138(unsigned char channel)
{
	switch(channel)
	{
		case 0:
			P2 = (P2 & 0x1f);
		break;		
		case 4:
			P2 = ((P2 & 0x1f) | 0x80);
		break;
		case 5:
			P2 = ((P2 & 0x1f) | 0xa0);
		break;
		case 6:
			P2 = ((P2 & 0x1f) | 0xc0);
		break;
		case 7:
			P2 = ((P2 & 0x1f) | 0xe0);
		break;
	}
}
void Smg_dis(unsigned char dula,wela)
{
	P0 = 0xff;//��Ӱ
	HC138(7);
	HC138(0);
	P0 = T_COM[dula];//λѡ
	HC138(6);
	HC138(0);
	P0 = ~t_display[wela];//��ѡ
	HC138(7);
	HC138(0);
	
}
void Led_Disp(unsigned char addr,enable)
{
	static unsigned char temp = 0x00;
	static unsigned char temp_old = 0xff;
	if(enable) temp |= 0x01 << addr;
	else temp &= ~(0x01 << addr);
	if(temp != temp_old)
	{
		P0 = ~temp;
		P2 = P2 & 0x1f | 0x80;
		P2 &= 0x1f;
		temp_old = temp;
	}
}
void Delay500ms()		//@12.000MHz
{
	unsigned char data i, j, k;

	_nop_();
	i = 4;
	j = 205;
	k = 187;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Timer0_Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	ET0 = 1;
	EA = 1;
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}
void Smg_Proc()	//ÿ500msִ��һ��
{
	if(Smg_SlowDown) return ;
	Smg_SlowDown = 1;
	Led_buf[7] = 1;
	Led_buf[0] = 1;	
}
int main()
{
	Timer0_Init();
	while(1)
	{
		Smg_Proc();
		Delay500ms();
	}
}
void Timer0Server() interrupt 1
{
	if(++Smg_SlowDown == 500) Smg_SlowDown = 0;//����ܼ���
	
	if(++Eignt_pos == 8) Eignt_pos = 0;
	Smg_dis(Eignt_pos,Smg_buf[Eignt_pos]);
	Led_Disp(Eignt_pos,Led_buf[Eignt_pos]);
}