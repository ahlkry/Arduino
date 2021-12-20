#include "Arduino.h"
#define CS_PIN_2828           4
#define MOSI_PIN_2828         23
#define MISO_PIN_2828         19
#define SCL_PIN_2828          18
#define RST_PIN_ALL_SYSTEM    17
/* SSD2828 interface define */
#define MOSI_2828(n)   digitalWrite(MOSI_PIN_2828, n)//MOSI
#define SCLK_2828(n)  digitalWrite(SCL_PIN_2828, n)//SCL
#define CS_2828(n)    digitalWrite(CS_PIN_2828, n)//CS
#define RST_2828(n)   digitalWrite(RST_PIN_ALL_SYSTEM, n)//RST
#define MISO_2828_Read  digitalRead(MISO_PIN_2828)
void delay_us (uint32_t i){
  //delayMicroseconds(i);
  uint8_t a;a++;//a++;a++;//a++;a++;a++;
}
void delay2828 (uint32_t i){
  delay(i);
}
void SSD2828_Reset(void){
   RST_2828(0); //RST_2828=0;              
   delay2828(50);
   RST_2828(1);   //RST_2828=1;
   delay2828(100);
   CS_2828(1);    //CS_2828=1;
   SCLK_2828(0);  //SCLK_2828 =0;
   MOSI_2828(1);  //SDI_2828=1;
}
void SPI_3W_SET_Cmd(unsigned int Sdata) { 
  CS_2828(0);//   CS_2828=0;
  delay_us(1);//delay_us(50);
  MOSI_2828(0);// SDI_2828=0;
  delay_us(1);//delay_us(50);
  SCLK_2828(0);// SCLK_2828 =0; 
  delay_us(1);//delay_us(50);
  SCLK_2828(1);//   SCLK_2828 =1;
 delay_us(1);//delay_us(50);
  for(uint8_t i=8; i>0; i--) {
    if(Sdata&0x80)
      MOSI_2828(1);//   SDI_2828=1;
    else
      MOSI_2828(0);// SDI_2828=0;
    delay_us(1);//delay_us(50);
    SCLK_2828(0);// SCLK_2828 =0; 
    delay_us(1);//delay_us(50);
    SCLK_2828(1);//   SCLK_2828 =1;
    delay_us(1);//delay_us(50);
    Sdata <<= 1;
  }
  SCLK_2828(0);// SCLK_2828 =0;
  delay_us(1);//delay_us(50);
  CS_2828(1);//     CS_2828=1;
}
void SPI_3W_SET_PAs(unsigned int Sdata) { 
  CS_2828(0);//   CS_2828=0;
  delay_us(1);//delay_us(50);
  MOSI_2828(1);//   SDI_2828=1;
  delay_us(1);//delay_us(50);
  SCLK_2828(0);// SCLK_2828 =0; 
  delay_us(1);//delay_us(50);
  SCLK_2828(1);//   SCLK_2828 =1;
  delay_us(1);//delay_us(50);
  for(uint8_t i=8; i>0; i--) {
    if(Sdata&0x80)
      MOSI_2828(1);//   SDI_2828=1;
    else
      MOSI_2828(0);// SDI_2828=0;
    delay_us(1);//delay_us(50);
    SCLK_2828(0);// SCLK_2828 =0; 
    delay_us(1);//delay_us(50);
    SCLK_2828(1);//   SCLK_2828 =1;
    delay_us(1);//delay_us(50);
    Sdata <<= 1;
  }
  SCLK_2828(0);// SCLK_2828 =0;
  delay_us(1);//delay_us(50);
  CS_2828(1);//     CS_2828=1;
}
unsigned char SPI_ReadData(void) {   
  unsigned char Value = 0;
  CS_2828(0);//   CS_2828=0;
  SCLK_2828(0);// SCLK_2828 =0;
  for(uint8_t i=0; i<8; i++) {
    SCLK_2828(0);// SCLK_2828 =0; 
    SCLK_2828(1);//   SCLK_2828 =1;
    Value <<= 1;
    if(MISO_2828_Read==1)//   SDO_2828==1)
      Value |= 0x01;
  }
  CS_2828(1);//     CS_2828=1;
  return Value;
}
void SPI_WriteData(unsigned char value){
  SPI_3W_SET_PAs(value);
}
void SPI_WriteCmd(unsigned char value){
  SPI_3W_SET_Cmd(value);
}
//2014-08-29 ȥ��bd be
void GP_COMMAD_PA(unsigned int num){
  SPI_WriteCmd(0xbc);
  SPI_WriteData(num&0xff);  
  SPI_WriteData((num>>8)&0xff); 
  SPI_WriteCmd(0xbf);
}
void SSD2828_initial(){
  pinMode (CS_PIN_2828, OUTPUT);
  pinMode (MOSI_PIN_2828, OUTPUT);
  pinMode (MISO_PIN_2828, INPUT);
  pinMode (SCL_PIN_2828, OUTPUT);
  pinMode (RST_PIN_ALL_SYSTEM, OUTPUT);
  delay(5);
  
  SSD2828_Reset();
  SPI_WriteCmd(0xb7);
  SPI_WriteData(0x50);//50=TX_CLK 70=PCLK
  SPI_WriteData(0x00);   //Configuration Register

  SPI_WriteCmd(0xb8);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x00);//1=PLL disable
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xBA);//PLL=(TX_CLK/MS)*NS 
  SPI_WriteData(0x20);//14,D7-0=NS(0x01 : NS=1)
  SPI_WriteData(0x82);//42,D15-14=PLL00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)


  SPI_WriteCmd(0xBB);//LP Clock Divider LP clock = 400MHz / LPD / 8 = 480 / 8/ 8 = 7MHz
  SPI_WriteData(0x07);//D5-0=LPD=0x1 �C Divide by 2
  SPI_WriteData(0x00);

   SPI_WriteCmd(0xb9);
  SPI_WriteData(0x01);//1=PLL disable
  SPI_WriteData(0x00);
  //MIPI lane configuration
  SPI_WriteCmd(0xDE);
  SPI_WriteData(0x00);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xc9);
  SPI_WriteData(0x02);
  SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
  delay2828(100);
           
  SPI_WriteCmd(0xCA);
  SPI_WriteData(0x01);//CLK Prepare
  SPI_WriteData(0x23);//Clk Zero

  SPI_WriteCmd(0xCB); //local_write_reg(addr=0xCB,data=0x0510)
  SPI_WriteData(0x10); //Clk Post
  SPI_WriteData(0x05); //Clk Per

  SPI_WriteCmd(0xCC); //local_write_reg(addr=0xCC,data=0x100A)
  SPI_WriteData(0x05); //HS Trail
  SPI_WriteData(0x10); //Clk Trail
/*============================================================================*/

  //LCD driver initialization
  SPI_WriteCmd(0xB7);
  SPI_WriteData(0x50);//10=TX_CLK 30=PCLK
  SPI_WriteData(0x02);

  SPI_WriteCmd(0xBD);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);
////////////////////////////////////////////

  GP_COMMAD_PA(1);
  SPI_WriteData(0x10); 
       

     GP_COMMAD_PA(2);       
     SPI_WriteData(0xCD);
     SPI_WriteData(0xAA);
     
    GP_COMMAD_PA(2);
     SPI_WriteData(0x41);
     SPI_WriteData(0x34);

     GP_COMMAD_PA(2);
     SPI_WriteData(0x30);
     SPI_WriteData(0x00);

     GP_COMMAD_PA(2);
    SPI_WriteData(0x39);
     SPI_WriteData(0x11);

     GP_COMMAD_PA(2);
     SPI_WriteData(0x32);
     SPI_WriteData(0x00);

     GP_COMMAD_PA(2);
    SPI_WriteData(0x33);
     SPI_WriteData(0x38);///////////08

     GP_COMMAD_PA(2);
     SPI_WriteData(0x35);
     SPI_WriteData(0x24);/////27
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x4F);
     SPI_WriteData(0x35);//////12
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x4E);
     SPI_WriteData(0x27);////17
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x41);
    SPI_WriteData(0x56);
    
     GP_COMMAD_PA(9);
     SPI_WriteData(0x55);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     
     GP_COMMAD_PA(17);
     SPI_WriteData(0x56);
     SPI_WriteData(0x00);
    SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
    SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
    SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
    SPI_WriteData(0x00);
     SPI_WriteData(0x0F);
     SPI_WriteData(0x00);
    SPI_WriteData(0x0F);
    
    
    
     GP_COMMAD_PA(2);
     SPI_WriteData(0x65);
     SPI_WriteData(0x08);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x3A);
    SPI_WriteData(0x08);
    
     GP_COMMAD_PA(2);
     SPI_WriteData(0x36);
     SPI_WriteData(0x49);
     
      GP_COMMAD_PA(2);
     SPI_WriteData(0x67);
     SPI_WriteData(0x82);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x69);
    SPI_WriteData(0x20);
    
     GP_COMMAD_PA(2);
     SPI_WriteData(0x6C);
     SPI_WriteData(0x80);
     
     GP_COMMAD_PA(2);
    SPI_WriteData(0x6D);
     SPI_WriteData(0x01);
     
     
     GP_COMMAD_PA(20);
     SPI_WriteData(0x53);
     SPI_WriteData(0x1F);
     SPI_WriteData(0x19);
     SPI_WriteData(0x15);
     SPI_WriteData(0x11);
     SPI_WriteData(0x11);
     SPI_WriteData(0x11);
     SPI_WriteData(0x12);
     SPI_WriteData(0x14);
     SPI_WriteData(0x15);
     SPI_WriteData(0x11);
    SPI_WriteData(0x0D);
     SPI_WriteData(0x0B);
     SPI_WriteData(0x0B);
     SPI_WriteData(0x0D);
    SPI_WriteData(0x0C);
     SPI_WriteData(0x0C);
     SPI_WriteData(0x08);
     SPI_WriteData(0x04);
     SPI_WriteData(0x00);
     
         
     GP_COMMAD_PA(20);
     SPI_WriteData(0x54);
    SPI_WriteData(0x1F);
     SPI_WriteData(0x19);
     SPI_WriteData(0x15);
     SPI_WriteData(0x11);
     SPI_WriteData(0x11);
     SPI_WriteData(0x11);
     SPI_WriteData(0x13);
     SPI_WriteData(0x15);
     SPI_WriteData(0x16);
     SPI_WriteData(0x11);
     SPI_WriteData(0x0D);
     SPI_WriteData(0x0C);
     SPI_WriteData(0x0C);
     SPI_WriteData(0x0E);
     SPI_WriteData(0x0C);
     SPI_WriteData(0x0C);
     SPI_WriteData(0x08);
     SPI_WriteData(0x04);
    SPI_WriteData(0x00);
    
     GP_COMMAD_PA(2);
     SPI_WriteData(0x6B);
     SPI_WriteData(0x00);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x58);
     SPI_WriteData(0x00);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x73);
     SPI_WriteData(0xF0);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x76);
     SPI_WriteData(0x40);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x77);
     SPI_WriteData(0x04);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x74);
     SPI_WriteData(0x17);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x5E);
     SPI_WriteData(0x03);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x68);
     SPI_WriteData(0x10);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x6A);
     SPI_WriteData(0x00);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x28);
     SPI_WriteData(0x31);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x29);
     SPI_WriteData(0x21);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x63);
     SPI_WriteData(0x04);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x27);
     SPI_WriteData(0x00);
     
     GP_COMMAD_PA(2);
    SPI_WriteData(0x7C);
     SPI_WriteData(0x80);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x2E);
     SPI_WriteData(0x05);

     GP_COMMAD_PA(2);
     SPI_WriteData(0x4C);
     SPI_WriteData(0x80);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x50);
     SPI_WriteData(0xC0);
     
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x78);
     SPI_WriteData(0x6E);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x2D);
     SPI_WriteData(0x31);
     
     GP_COMMAD_PA(2);
     SPI_WriteData(0x49);
     SPI_WriteData(0x00);

     GP_COMMAD_PA(2);
     SPI_WriteData(0x4D);
     SPI_WriteData(0x00); 
            


     GP_COMMAD_PA(1);
     SPI_WriteData(0x11);
     delay2828(120);
     GP_COMMAD_PA(1);
    SPI_WriteData(0x29);
  delay2828(120);  

  SPI_WriteCmd(0xb7);
  SPI_WriteData(0x50);
  SPI_WriteData(0x00);   //Configuration Register

  SPI_WriteCmd(0xb8);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x00);//1=PLL disable
  SPI_WriteData(0x00);

    SPI_WriteCmd(0xBA);//
  SPI_WriteData(0x20);//14,D7-0=NS(0x01 : NS=1)
  SPI_WriteData(0x82);//42,D15-14=PLL00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)


  SPI_WriteCmd(0xBB);//LP Clock Divider LP clock = 400MHz / LPD / 8 = 480 / 8/ 8 = 7MHz
  SPI_WriteData(0x07);//D5-0=LPD=0x1 �C Divide by 2
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xb9);
  SPI_WriteData(0x01);//1=PLL disable
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xc9);
  SPI_WriteData(0x02);
  SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
  delay2828(100);

  SPI_WriteCmd(0xCA);
  SPI_WriteData(0x01);//CLK Prepare
  SPI_WriteData(0x23);//Clk Zero

  SPI_WriteCmd(0xCB); //local_write_reg(addr=0xCB,data=0x0510)
  SPI_WriteData(0x10); //Clk Post
  SPI_WriteData(0x05); //Clk Per

  SPI_WriteCmd(0xCC); //local_write_reg(addr=0xCC,data=0x100A)
  SPI_WriteData(0x05); //HS Trail
  SPI_WriteData(0x10); //Clk Trail

  SPI_WriteCmd(0xD0);
  SPI_WriteData(0x00);
  SPI_WriteData(0x00);

  //RGB interface configuration
  SPI_WriteCmd(0xB1);
  SPI_WriteData(0x18);//HSPW 07
  SPI_WriteData(0x02);//VSPW 05

  SPI_WriteCmd(0xB2);
  SPI_WriteData(0xa0);//HBPD 0x64=100
  SPI_WriteData(0x0a);//VBPD 8 ��С����

  SPI_WriteCmd(0xB3);
  SPI_WriteData(0xa0);//HFPD 8
  SPI_WriteData(0x0c);//VFPD 10

  SPI_WriteCmd(0xB4);//Horizontal active period 720=02D0
  SPI_WriteData(0x90);//013F=319 02D0=720
  SPI_WriteData(0x01);

  SPI_WriteCmd(0xB5);//Vertical active period 1280=0500
  SPI_WriteData(0x00);//01DF=479 0500=1280
  SPI_WriteData(0x05);

  
  SPI_WriteCmd(0xB6);//RGB CLK  16BPP=00 18BPP=01
  SPI_WriteData(0x0b);//D7=0 D6=0 D5=0  D1-0=11 �C 24bpp   //07
  SPI_WriteData(0x00);//D15=VS D14=HS D13=CLK D12-9=NC D8=0=Video with blanking packet. 00-F0

  //MIPI lane configuration
  SPI_WriteCmd(0xDE);//ͨ����
  SPI_WriteData(0x03);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xD6);//  05=BGR  04=RGB
  SPI_WriteData(0x05);//D0=0=RGB 1:BGR D1=1=Most significant byte sent first
  SPI_WriteData(0x00);

  SPI_WriteCmd(0xDB);
  SPI_WriteData(0x58);
  SPI_WriteData(0x00);


  SPI_WriteCmd(0xB7);
  SPI_WriteData(0x6B);   //4b
  SPI_WriteData(0x02);
}
