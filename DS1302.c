#include "DS1302.h"
#include "delay.h"


//RST PA5
//CLK PA7
//IO  PA6

//��ʼʱ�䶨��
char time_buf[8] = {0x20,0x23,3,27,10,10,00,01};//��ʼʱ��
u8 readtime[15];//��ǰʱ��
u8 sec_buf=0;  //�뻺��
u8 sec_flag=0; //���־λ

struct TIMEData TimeData;



//����ʱ��ͼ����ʼ�����������ŵ�ʱ��Ҫ���ͣ����Գ�ʼ����ʱ���˳������
void DS1302_Init()
{
	GPIO_InitTypeDef DS1302;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	DS1302.GPIO_Pin = GPIO_Pin_5; 				//RST
	DS1302.GPIO_Speed = GPIO_Speed_50MHz;
	DS1302.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_Init(GPIOA, &DS1302);			
	GPIO_ResetBits(GPIOA,GPIO_Pin_5); 
	
	DS1302.GPIO_Pin = GPIO_Pin_7; 				//CLK
	DS1302.GPIO_Speed = GPIO_Speed_50MHz;
	DS1302.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_Init(GPIOA, &DS1302);			
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
}

//����ģ���IO��PA6��Ҫ������������
void DS1302_IO_Output()
{
	GPIO_InitTypeDef DS1302_IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	DS1302_IO.GPIO_Pin = GPIO_Pin_6; 				//IO
	DS1302_IO.GPIO_Speed = GPIO_Speed_50MHz;
	DS1302_IO.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_Init(GPIOA, &DS1302_IO);			
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}

void DS1302_IO_Input()
{
	GPIO_InitTypeDef DS1302_IO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	DS1302_IO.GPIO_Pin = GPIO_Pin_6;              //IO
	DS1302_IO.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_Init(GPIOA, &DS1302_IO);
}



//��DS1302д��һ�ֽ�����
void DS1302_WriteByte(u8 addr,u8 data) 
{
	u8 i;
	DS1302_RST=0; //��ֹ���ݴ��� ��������������Ҫ
	DS1302_CLK=0; //ȷ��д����ǰSCLKΪ�͵�ƽ
	DS1302_RST=1;	//����DS1302����	
	DS1302_IO_Output();
	addr=addr&0xFE;  //���λ���㣬�Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for(i=0;i<8;i++) //д��Ŀ���ַ��addr
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //ʱ��������д������
		DS1302_CLK=0;
		addr=addr>>1;
	}	
	for (i=0;i<8;i++) //д�����ݣ�data
	{
		if(data&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1;    //ʱ��������д������
		DS1302_CLK=0;
		data = data >> 1;
	}
	DS1302_CLK=1;    // ��ʱ�ӵ�ƽ���ڸߵ�ƽ״̬ ��������֪״̬
	DS1302_RST=0;	//ֹͣDS1302����
}


//��DS1302����һ�ֽ�����
u8 DS1302_ReadByte(u8 addr) 
{
	u8 i,temp;	
	DS1302_RST=0; 			//��������Ҫ
	DS1302_CLK=0; 			//�Ƚ�SCLK�õ͵�ƽ,ȷ��д����ǰSCLK������
	DS1302_RST=1; 			//����DS1302����
	DS1302_IO_Output();
	//д��Ŀ���ַ��addr
	addr=addr|0x01; //���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for(i=0;i<8;i++) 
	{
		if (addr&0x01) DS1302_OUT=1;
		else DS1302_OUT=0;
		DS1302_CLK=1; //д����
		DS1302_CLK=0;
		addr = addr >> 1;
	}	
	//��DS1302�������ݣ�temp
	DS1302_IO_Input();
	for(i=0;i<8;i++)
	{
		temp=temp>>1;
		if (DS1302_IN) temp|=0x80;
		else temp&=0x7F;
		DS1302_CLK=1;
		DS1302_CLK=0;
	}	
	DS1302_CLK=1;  //��ʱ�ӵ�ƽ������֪״̬
	DS1302_RST=0;	//ֹͣDS1302����
	return temp;
}

//ʱ��д��
//д��ҲҪת����BCD��д�룬�����޷���ȷʶ��
void DS1302_WriteTime(char time_buf[8]) 
{
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x00);       //�ر�д���� 
	DS1302_WriteByte(DS1302_YEAR_ADDR,time_buf[1]);   //�� 
	DS1302_WriteByte(DS1302_MONTH_ADDR,time_buf[2]/10*16 + time_buf[2]%10);  //�� 
	DS1302_WriteByte(DS1302_DAY_ADDR,time_buf[3]/10*16 + time_buf[3]%10);    //�� 
	DS1302_WriteByte(DS1302_HOUR_ADDR,time_buf[4]/10*16 + time_buf[4]%10);   //ʱ 
	DS1302_WriteByte(DS1302_MIN_ADDR,time_buf[5]/10*16 + time_buf[5]%10);    //��
	DS1302_WriteByte(DS1302_SEC_ADDR,time_buf[6]/10*16 + time_buf[6]%10);    //��
	DS1302_WriteByte(DS1302_WEEK_ADDR,time_buf[7]/10*16 + time_buf[7]%10);	  //�� 
	DS1302_WriteByte(DS1302_CONTROL_ADDR,0x80);			//��д����  
	
}

//ʱ�����
void DS1302_ReadTime()  
{
	time_buf[1]=DS1302_ReadByte(DS1302_YEAR_ADDR);          //�� 
	time_buf[2]=DS1302_ReadByte(DS1302_MONTH_ADDR);         //�� 
	time_buf[3]=DS1302_ReadByte(DS1302_DAY_ADDR);           //�� 
	time_buf[4]=DS1302_ReadByte(DS1302_HOUR_ADDR);          //ʱ 
	time_buf[5]=DS1302_ReadByte(DS1302_MIN_ADDR);           //�� 
	time_buf[6]=DS1302_ReadByte(DS1302_SEC_ADDR);    //�룬������ĵ�7λ�����ⳬ��59
	time_buf[7]=DS1302_ReadByte(DS1302_WEEK_ADDR);          //�� 	
}


//DS1302��ȡ��ʱ���������ݵļ���
//����������8421�룬ת�������ʹ��
void DS1302_GetTime()
{ 
    DS1302_ReadTime(); //��ȡʱ��
    TimeData.year	=(time_buf[0]>>4)*1000	+(time_buf[0]&0x0F)*100+(time_buf[1]>>4)*10+(time_buf[1]&0x0F); //�������
    TimeData.month	=time_buf[2]/16*10	+time_buf[2]%16;  	//�����·�
    TimeData.day	=time_buf[3]/16*10	+time_buf[3]%16;    //��������
    TimeData.hour	=time_buf[4]/16*10	+time_buf[4]%16;   	//����Сʱ
    TimeData.minute	=time_buf[5]/16*10	+time_buf[5]%16; 	//�������
    TimeData.second	=time_buf[6]/16*10	+time_buf[6]%16; 	//��������
    TimeData.week	=time_buf[7]/16*10	+time_buf[7]%16;                      		//��������																										
}




