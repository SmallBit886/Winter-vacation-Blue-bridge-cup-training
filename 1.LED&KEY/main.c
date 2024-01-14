/*
********************************************ͷ�ļ�������********************************************
*/
#include <STC15F2K60S2.H>
#include <intrins.h>
#define u8 	unsigned char
#define u16	unsigned int
/*
********************************************����������********************************************
*/
u8 led_water = 0xfe;	//LED��ˮ����
u8 led[9]={0xff,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};	//LED��������
u8 key_valu,key_down,key_up,key_old;	//����ɨ�����
bit key_flag;	//�������Ʊ���

/*
********************************************�Զ��庯����********************************************
*/
void Delay(u16 x)		//@12.000MHz
{
	unsigned char data i, j;
	while(x--)
	{	
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

void HC138(u8 x)//ͨ��ѡͨ
{
	switch (x)
	{
		case 4:
			//1000 0000
			P2 = ((P2 & 0x1f) | 0x80);
		break;
		case 5:
			//1010 0000
			P2 = ((P2 & 0x1f) | 0xa0);
		break;
		case 6:
			//1100 0000
			P2 = ((P2 & 0x1f) | 0xc0);
		break;
		case 7:
			//1110 0000
			P2 = ((P2 & 0x1f) | 0xe0);
		break;
	}
}
u8 Key_read()
{
	u8 temp = 0;
	if(P30 == 0) temp = 1;
	if(P31 == 0) temp = 2;
	if(P32 == 0) temp = 3;
	if(P33 == 0) temp = 4;
	return temp;
}
void Led_ON(u8 x)
{
	HC138(4);
	P0 = led[x];
}

/*
********************************************��������********************************************
*/
int main()
{
	//��ѭ��
	while(1)
	{
		key_valu = Key_read();	//��ȡ����ֵ
		key_down = key_valu & (key_valu ^ key_old);	//����½���
		key_up = ~key_valu & (key_valu ^ key_old);	//���������
		key_old = key_valu;		//ɨ�踨������
		
		switch(key_down)
		{
			case 1:
				Led_ON(1);
			break;
			case 2:
				Led_ON(2);
			break;
			case 3:
				key_flag = 1;
			break;
			case 4:
				key_flag = 0;
			break;
		}
		if(key_flag == 1)
		{
			HC138(4);
			P0 = led_water;
			Delay(500);
			led_water = _crol_(led_water,1);
		}
	}
}
