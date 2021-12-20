/* touch panel interface define */
//ARDUINO 
#define SDA_PIN             21
#define SCL_PIN             22
#define INT_PIN             32

#define CT_IIC_SCL(n)   digitalWrite(SCL_PIN, n)//SCL
#define CT_IIC_SDA(n)   digitalWrite(SDA_PIN, n)//SDA
#define CT_READ_SDA     digitalRead(SDA_PIN)
#define CT_READ_INT     digitalRead(INT_PIN)

//Touch Status	 
#define Key_Down 0x01
#define Key_Up   0x00 

struct _ts_event
{
    uint16_t    x1;
    uint16_t    y1;
    uint16_t    x2;
    uint16_t    y2;
    uint16_t    x3;
    uint16_t    y3;
    uint16_t    x4;
    uint16_t    y4;
    uint16_t    x5;
    uint16_t    y5;
    uint8_t     touch_point;
		uint8_t     Key_Sta;
};

struct _ts_event ts_event;  

#define WRITE_ADD	0xba 
#define READ_ADD	0xbb

#define CT_MAX_TOUCH    5		//���ݴ��������֧�ֵĵ���

// Registers define
#define GTP_READ_COOR_ADDR    0x814e
#define GTP_REG_SLEEP         0x8040
#define GTP_REG_SENSOR_ID     0x814a
#define GTP_REG_CONFIG_DATA   0x8047
#define GTP_REG_VERSION       0x8440

//#define CT_READ_XY_REG 	0x814E  	//��ȡ����Ĵ��� 
//#define CT_CONFIG_REG   0x8047	//���ò�����ʼ�Ĵ���

uint8_t GTP_CFG_DATA[]=
{
/*
0x63,0x00,0x04,0x58,0x02,0x0A,0x3D,0x00,
0x01,0x08,0x28,0x0F,0x50,0x32,0x03,0x05,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,
0x19,0x1D,0x14,0x90,0x2F,0x89,0x23,0x25,
0xD3,0x07,0x00,0x00,0x00,0x02,0x03,0x1D,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x19,0x32,0x94,0xD5,0x02,
0x07,0x00,0x00,0x04,0xA2,0x1A,0x00,0x90,
0x1E,0x00,0x80,0x23,0x00,0x73,0x28,0x00,
0x68,0x2E,0x00,0x68,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x16,0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,
0x0C,0x09,0x08,0x07,0x06,0x05,0x04,0x01,
0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x28,
0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,
0x1F,0x1E,0x1C,0x1B,0x19,0x14,0x13,0x12,
0x11,0x10,0x0F,0x0E,0x0D,0x0C,0x0A,0x08,
0x07,0x06,0x04,0x02,0x00,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x71,0x01	 */

0x5A,0x00,0x05,0x90,0x01,0x05,0x0D,0x00,
0x01,0x08,0x28,0x05,0x50,0x32,0x03,0x05,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x8B,0x2B,0x0A,0x22,0x24,
0x31,0x0D,0x00,0x00,0x00,0x01,0x03,0x2D,
0x00,0x00,0x00,0x00,0x00,0x03,0x64,0x32,
0x00,0x00,0x00,0x1A,0x40,0x94,0xC5,0x02,
0x07,0x00,0x00,0x04,0x95,0x1C,0x00,0x7F,
0x22,0x00,0x71,0x28,0x00,0x62,0x31,0x00,
0x58,0x3A,0x00,0x58,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
0x12,0x14,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x16,0x18,
0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x24,
0x26,0x13,0x12,0x10,0x0F,0x0C,0x0A,0x08,
0x06,0x04,0x02,0x00,0xFF,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xE3,0x01
};



void CT_Delay(void)
{
	delay_us(1);
}
void CT_SDA_IN(void)
{
  pinMode(SDA_PIN, INPUT);
}
void CT_SDA_OUT(void)
{
	pinMode(SDA_PIN, OUTPUT);
}


void TOUCH_Init(void);
void TOUCH_Start(void);
void TOUCH_Stop(void);
uint8_t   TOUCH_Wait_Ack(void);
void TOUCH_Ack(void);
void TOUCH_NAck(void);

void TOUCH_Send_Byte(uint8_t txd);
uint8_t TOUCH_Read_Byte(unsigned char ack);
void Draw_Big_Point(uint16_t x,uint16_t y,uint16_t colour);
uint8_t GT911_read_data(void);
void inttostr(uint16_t value,uint8_t *str);
void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t GT911_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);

//IIC start
void TOUCH_Start(void)
{ 
	CT_SDA_OUT();
	CT_IIC_SDA(1);//SDA=1;  
	CT_IIC_SCL(1);//SCL=1;
	CT_Delay();//delay_us(5);
	CT_IIC_SDA(0);//SDA=0;  
	CT_Delay();
	CT_IIC_SCL(0);//SCL=0;
}	  
//IIC stop
void TOUCH_Stop(void)
{
	CT_SDA_OUT();
	CT_IIC_SCL(1);
	CT_Delay();//delay_us(5);
	CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA(1);						   	
}
//Wait for an answer signal
uint8_t TOUCH_Wait_Ack(void)
{	uint8_t errtime=0;
	CT_SDA_IN();
	CT_IIC_SDA(1);
	CT_IIC_SCL(1);
	CT_Delay();
 while(CT_READ_SDA)
	{
	    errtime++;
	    if(errtime>250)
		    {
		      TOUCH_Stop();
		      return 1;
		    }	
	}
	CT_IIC_SCL(0);//SCL=0;
	return 0;
}
//Acknowledge
void TOUCH_Ack(void)
{	
	CT_IIC_SCL(0);
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA(0);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
}
//NO Acknowledge		    
void TOUCH_NAck(void)
{	
	CT_IIC_SCL(0);
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA(1);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
}	
	

//IIC send one byte		  
void TOUCH_Send_Byte(uint8_t Byte)
{	  		
 	CT_SDA_OUT();
    CT_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
	CT_Delay();
	for(uint8_t t=0;t<8;t++)
    {
        CT_IIC_SDA((Byte&0x80)>>7);
        Byte<<=1;
		CT_IIC_SCL(1);
		CT_Delay();
		CT_IIC_SCL(0);
		CT_Delay();
    }
} 
//Read one byte��ack=0��Send Acknowledge��ack=1��NO Acknowledge   
uint8_t TOUCH_Read_Byte(uint8_t ack)
{	
	uint8_t receive=0;
	CT_SDA_IN();
	CT_Delay();//delay_us(5);
	for(uint8_t i=0;i<8;i++ )
	{
		CT_IIC_SCL(0);
		CT_Delay();
		CT_IIC_SCL(1);
		receive<<=1;
		if(CT_READ_SDA)receive++;
	}
	if (!ack)TOUCH_NAck();
	else TOUCH_Ack();
	return receive;
}

uint8_t GT911_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	TOUCH_Start();	
 	TOUCH_Send_Byte(WRITE_ADD);   	//����д���� 	 
	TOUCH_Wait_Ack();
	TOUCH_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	TOUCH_Wait_Ack();	 										  		   
	TOUCH_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	TOUCH_Wait_Ack(); 
	for(i=0;i<len;i++)
	{	   
    	TOUCH_Send_Byte(buf[i]);  	//������
		ret=TOUCH_Wait_Ack();
		if(ret)break;  
	}
    TOUCH_Stop();					//����һ��ֹͣ����	    
	return ret; 
}


void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
 	TOUCH_Start();	
 	TOUCH_Send_Byte(WRITE_ADD);   	//����д���� 	 
	TOUCH_Wait_Ack(); 
 	TOUCH_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	TOUCH_Wait_Ack();  										  		   
 	TOUCH_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	TOUCH_Wait_Ack();

 	TOUCH_Start(); 	 	   
	TOUCH_Send_Byte(READ_ADD);    //���Ͷ�����		   
	TOUCH_Wait_Ack();    
	for(i=0;i<(len-1);i++)
	{	   
    	buf[i]=TOUCH_Read_Byte(1); //������
	} 
	buf[len-1]=TOUCH_Read_Byte(0);
     TOUCH_Stop();//����һ��ֹͣ����	    
}
uint8_t GT911_Send_Cfg(uint8_t * buf, uint16_t cfg_len)
{
	uint8_t ret=0;
	uint8_t retry=0;
	for(retry=0;retry<15;retry++)
	{
		ret=GT911_WR_Reg(GTP_REG_CONFIG_DATA,buf,cfg_len);
		if(ret==0)break;
		delay(10);
	}
	return ret;
}
uint8_t GT911_read_data(void)
{	
	uint8_t buf[40] = {0xff}; 
	for (uint8_t i=0;i<40;i++)
	buf[i]=0xff;
	uint8_t ret = 0;	uint8_t ss[4];
	GT911_RD_Reg(GTP_READ_COOR_ADDR,buf,40);		//һ�ζ�ȡ80���ֽ�
	ss[0]=0;		
 	GT911_WR_Reg(GTP_READ_COOR_ADDR,ss,1);	  //Must clear  Buffer status
  ts_event.touch_point = buf[0] & 0x0f;
	if (ts_event.touch_point == 0) 
	{  			
		return 0;
	}		
	//if (ts_event.touch_point==5)
	//{
		ts_event.y5 = (uint16_t)(buf[35] )<<8 | (uint16_t)buf[34];
		ts_event.x5 = (uint16_t)(buf[37] )<<8 | (uint16_t)buf[36];
	//}
	//if (ts_event.touch_point==4)
	//{
		ts_event.y4 = (uint16_t)(buf[27] )<<8 | (uint16_t)buf[26];
		ts_event.x4 = (uint16_t)(buf[29] )<<8 | (uint16_t)buf[28];
	//}
	//if (ts_event.touch_point==3)
	//{
		ts_event.y3 = (uint16_t)(buf[19] )<<8 | (uint16_t)buf[18];
		ts_event.x3 = (uint16_t)(buf[21] )<<8 | (uint16_t)buf[20];
	//}
	//if (ts_event.touch_point==2)
	//{
		ts_event.y2 = (uint16_t)(buf[11] )<<8 | (uint16_t)buf[10];
		ts_event.x2 = (uint16_t)(buf[13] )<<8 | (uint16_t)buf[12];
	//}
	//if (ts_event.touch_point==1)
	//{
		ts_event.y1 = (uint16_t)(buf[3] )<<8 | (uint16_t)buf[2];
		ts_event.x1 = (uint16_t)(buf[5] )<<8 | (uint16_t)buf[4];
	//}
	return ret;
}
void inttostr(uint16_t value,uint8_t *str)
{
	str[0]=value/1000+48;
	str[1]=value%1000/100+48;
	str[2]=value%1000%100/10+48;
 	str[3]=value%1000%100%10+48;
}
void TOUCH_Init(void)
{	
   	//CT_IIC_RESET(0);//RESET=0;						   
		//delay(200);
 		//CT_IIC_RESET(1);//RESET=1;
		//delay(200);
	ts_event.x1=0;
	ts_event.y1=0;
	ts_event.x2=0;
	ts_event.y2=0;
	ts_event.x3=0;
	ts_event.y3=0;
	ts_event.x4=0;
	ts_event.y4=0;
	ts_event.x5=0;
	ts_event.y5=0;
}						  
void TPTEST(void)
{
	/*uint8_t ss[4]; uint8_t re=1;//	uint8_t i;  uint8_t bb[187];
   	ts_event.Key_Sta=Key_Up;
	ts_event.x1=0;
	ts_event.y1=0;
	ts_event.x2=0;
	ts_event.y2=0;
	ts_event.x3=0;
	ts_event.y3=0;
	ts_event.x4=0;
	ts_event.y4=0;
	ts_event.x5=0;
	
	 	Select_Main_Window_16bpp();
		Main_Image_Start_Address(0);				
		Main_Image_Width(400);							
		Main_Window_Start_XY(0,0);	

    Canvas_Image_Start_address(0);//Layer 1
		Canvas_image_width(400);//
    Active_Window_XY(0,0);
		Active_Window_WH(400,1280);

		Foreground_color_65k(Black);
		Line_Start_XY(0,0);
		Line_End_XY(399,1279);
		Start_Square_Fill();

		 Foreground_color_65k(Black);
	Background_color_65k(Blue2);
	CGROM_Select_Internal_CGROM();
	Font_Select_12x24_24x24();
 	Goto_Text_XY(0,Line1);
	Show_String("www.buydisplay.ocm  CTP test");

 	Goto_Text_XY(0,Line52);
	Show_String("exit");
 	Goto_Text_XY(330,Line52);
	Show_String("clear");



  	Foreground_color_65k(White);
	Background_color_65k(Black);
	CGROM_Select_Internal_CGROM();
	Font_Select_8x16_16x16();
	

				TOUCH_Init();

			re=GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA,sizeof(GTP_CFG_DATA));
			if(re==0)
			{Goto_Text_XY(0,60);Show_String("send CFG OK");}
	 		else  {Goto_Text_XY(0,60);Show_String("send CFG NOK");}
		  
	   	ss[0]=0;		
 	GT911_WR_Reg(GTP_READ_COOR_ADDR,ss,1);	  //Must clear  Buffer status


	while(1)
	{	   	Foreground_color_65k(White);
			Background_color_65k(Black);
			CGROM_Select_Internal_CGROM();
			Font_Select_8x16_16x16();
	

		  	Goto_Text_XY(0,465);
		  	Show_String("Coordinate");



	   GT911_RD_Reg(GTP_READ_COOR_ADDR,ss,1);
	   if(ss[0]&0x80)
			{
					GT911_read_data();
					ts_event.Key_Sta=Key_Up;

					if(ts_event.x1<=50&&ts_event.x1>5&&ts_event.y1<=28&&ts_event.y1>5)return;
					if(ts_event.x1>=340&&ts_event.y1<=28)
					{	
						Select_Main_Window_16bpp();
						Main_Image_Start_Address(0);				
						Main_Image_Width(400);							
						Main_Window_Start_XY(0,0);	
				
				    	Canvas_Image_Start_address(0);//Layer 1
						Canvas_image_width(400);//
				    	Active_Window_XY(0,0);
						Active_Window_WH(400,1280);
				
						Foreground_color_65k(Black);
						Line_Start_XY(0,0);
						Line_End_XY(399,1279);
						Start_Square_Fill();
					
							 Foreground_color_65k(Black);
						Background_color_65k(Blue2);
						CGROM_Select_Internal_CGROM();
						Font_Select_12x24_24x24();
					 	Goto_Text_XY(0,Line1);
						Show_String("www.buydisplay.ocm  CTP test");
					
					 	Goto_Text_XY(0,Line52);
						Show_String("exit");
					 	Goto_Text_XY(330,Line52);
						Show_String("clear");
					}										   

					 else{
		
					inttostr(ts_event.x1,ss);	
			
					 
				  	Goto_Text_XY(100,465);   //Set the display position
				  	
				  	Show_String("X=");
					Text_Mode();   
					LCD_CmdWrite(0x04);
				  	LCD_DataWrite(ss[0]);
					 Check_Mem_WR_FIFO_not_Full();
				  	LCD_DataWrite(ss[1]);
					 Check_Mem_WR_FIFO_not_Full();
				  	LCD_DataWrite(ss[2]);
					 Check_Mem_WR_FIFO_not_Full();
 				  	LCD_DataWrite(ss[3]);
					 Check_Mem_WR_FIFO_not_Full();

					inttostr(ts_event.y1,ss);
				  	Goto_Text_XY(200, 465);   //Set the display position
				  	
				  	Show_String("Y=");
 					Text_Mode();		 
					LCD_CmdWrite(0x04);
				  	LCD_DataWrite(ss[0]);
					 Check_Mem_WR_FIFO_not_Full();
				  	LCD_DataWrite(ss[1]);
					 Check_Mem_WR_FIFO_not_Full();
				  	LCD_DataWrite(ss[2]);
					 Check_Mem_WR_FIFO_not_Full();
				  	LCD_DataWrite(ss[3]);
					  Check_Mem_WR_FIFO_not_Full();
					}
					
						ts_event.x1=0;
						ts_event.y1=0;
						ts_event.x2=0;
						ts_event.y2=0;
						ts_event.x3=0;
						ts_event.y3=0;
						ts_event.x4=0;
						ts_event.y4=0;
						ts_event.x5=0;
						ts_event.y5=0;

                   
			}


    }

*/

}
