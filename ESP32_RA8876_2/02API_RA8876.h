void BTE_Memory_Copy_Chroma_key
(
unsigned long S0_Addr //Start address of Source 0
,unsigned short S0_W //image width of Source 0 (recommend = canvas image width) 
,unsigned short XS0 //coordinate X of Source 0
,unsigned short YS0 //coordinate Y of Source 0
,unsigned long Des_Addr //start address of Destination
,unsigned short Des_W //image width of Destination (recommend = canvas image width) 
,unsigned short XDes //coordinate X of Destination
,unsigned short YDes //coordinate Y of Destination
,unsigned long Background_color // transparent color
,unsigned short X_W //Width of BTE Window
,unsigned short Y_H //Length of BTE Window
)
{
  BTE_S0_Memory_Start_Address(S0_Addr);
  BTE_S0_Image_Width(S0_W);
  BTE_S0_Window_Start_XY(XS0,YS0);	

  BTE_Destination_Memory_Start_Address(Des_Addr);
  BTE_Destination_Image_Width(Des_W);
  BTE_Destination_Window_Start_XY(XDes,YDes);
	
	BTE_S0_Color_16bpp();
	BTE_S1_Color_16bpp();
	BTE_Destination_Color_16bpp();
 
#ifdef MCU_8bit_ColorDepth_8bpp	
  Background_color_256(Background_color); 
#endif
#ifdef MCU_8bit_ColorDepth_16bpp	
  Background_color_65k(Background_color); 
#endif
#ifdef MCU_8bit_ColorDepth_24bpp	
  Background_color_16M(Background_color); 
#endif
#ifdef MCU_16bit_ColorDepth_16bpp	
  Background_color_65k(Background_color); 
#endif
#ifdef MCU_16bit_ColorDepth_24bpp_Mode_1	
  Background_color_16M(Background_color); 
#endif
#ifdef MCU_16bit_ColorDepth_24bpp_Mode_2	
  Background_color_16M(Background_color); 
#endif

  BTE_Window_Size(X_W,Y_H); 
	
	BTE_Operation_Code(0x05);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
  
  BTE_Enable();
  Check_BTE_Busy();
}


void DMA_24bit
(
unsigned char SCS //SCS : 0 = Use SCS0, 1 = Use SCS1
,unsigned char Clk //Clk : SPI Clock = System Clock /{(Clk+1)*2}
,unsigned short X1 //X of DMA Coordinate
,unsigned short Y1 //Y of DMA Coordinate
,unsigned short X_W //DMA Block width
,unsigned short Y_H //DMA Block height
,unsigned short P_W //DMA Picture width
,unsigned long Addr //DMA Source Start address
)
{
  Enable_SFlash_SPI();									   //Enable Serial Flash
  if(SCS == 0)
   Select_SFI_0();										   //Select Serial Flash 0
  if(SCS == 1)
   Select_SFI_1();										   //Select Serial Flash 1
  
	Select_SFI_DMA_Mode();								   //Set Serial Flash DMA Mode
	
	//Select_SFI_24_DummyRead();
	//Select_SFI_Dual_Mode1();
	//SFI_DMA_Transfer_Number(Addr);
	Select_SFI_24bit_Address();
	
  SPI_Clock_Period(Clk);
  Goto_Pixel_XY(X1,Y1);									  //set Memory coordinate in Graphic Mode
  SFI_DMA_Destination_Upper_Left_Corner(X1,Y1);			  //DMA Destination position(x,y)
  SFI_DMA_Transfer_Width_Height(X_W,Y_H);				  //Set DMA Block (Height , Width)
  SFI_DMA_Source_Width(P_W);							  //Set DMA Source Picture Width
  SFI_DMA_Source_Start_Address(Addr); 						  //Set Serial Flash DMA Source Starting Address
  Start_SFI_DMA();									  //Start DMA
  Check_Busy_SFI_DMA();								  //DMA Busy Check
}
void DMA_32bit
(
unsigned char SCS //SCS : 0 = Use SCS0, 1 = Use SCS1
,unsigned char Clk //Clk : SPI Clock = System Clock /{(Clk+1)*2}
,unsigned short X1 //X of DMA Coordinate
,unsigned short Y1 //Y of DMA Coordinate 
,unsigned short X_W //DMA Block width
,unsigned short Y_H //DMA Block height
,unsigned short P_W //DMA Picture width
,unsigned long Addr //DMA Source Start address
)
{  

  Enable_SFlash_SPI();									   //Enable Serial Flash
  if(SCS == 0)
  {
   Select_SFI_0();										   //Select Serial Flash 0
  }
  if(SCS == 1)
  {														    
   Select_SFI_1();										   //Select Serial Flash 1
  }	 
  Select_SFI_DMA_Mode();								   //Set Serial Flash DMA Mode
  SPI_Clock_Period(Clk);

  Select_SFI_32bit_Address();							   //Set Serial Flash/ROM 32bits Address
  /////////////////////////////////////////****************DMA 
  Goto_Pixel_XY(X1,Y1);									  //set Memory coordinate in Graphic Mode
  SFI_DMA_Destination_Upper_Left_Corner(X1,Y1);			  //DMA Destination position(x,y)
  SFI_DMA_Transfer_Width_Height(X_W,Y_H);				  //Set DMA Block (Height , Width)
  SFI_DMA_Source_Width(P_W);							  //Set DMA Source Picture Width
  SFI_DMA_Source_Start_Address(Addr); 						  //Set Serial Flash DMA Source Starting Address

  Start_SFI_DMA();									  //Start DMA
  Check_Busy_SFI_DMA();								  //DMA Busy Check
  Select_SFI_24bit_Address();
}


