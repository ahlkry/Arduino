#include "stdio.h"
#include "stdlib.h"
#include "00_2828.h"
#include "01RA8876.h"
#include "02API_RA8876.h"
#include "10TP.h"
#include "20Redes.h"


#define Canvas1    1024000*0
#define Canvas2   1024000*1
#define Canvas3   1024000*2

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

uint8_t RedSocialTurno;

//OS
TaskHandle_t LeerREDESTaskHandle;
TaskHandle_t POSTLCDTaskHandle;
void IniAPI ();
void LeerREDESTask(void *pvParameters);
void PostLCDTask(void *pvParameters);


void IniAPI (){
  Serial.begin (115200);
  Serial.println("Init LCD...");
 ledcSetup(ledChannel, freq, resolution);// pinMode(BL_PIN_LED, OUTPUT);
 ledcAttachPin(BL_PIN_LED, ledChannel);
 
 
  SSD2828_initial();
  Serial.println("2828 Passed...");
  Serial.println("Init Ra8876...");
  RA8876_HW_Reset();
  RA8876_initial();
  
  delay(20);
  Serial.println("RA8876 Passed...");
  Display_ON();
  //TOUCH_Init();
  //GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA,sizeof(GTP_CFG_DATA));
  
  //preparo y lleno los buffers en sdram
  //contexto a canvas 1
  Canvas_Image_Start_address(Canvas1);
  Canvas_image_width(400);
  Main_Image_Start_Address(Canvas1);        
  Main_Image_Width(400);              
  Main_Window_Start_XY(0,0);
  Active_Window_XY(0,0);
  Active_Window_WH(400,1280);
  
  Foreground_color_65k(color65k_red);
  Line_Start_XY(0,0);
  Line_End_XY(799,1279);
  Start_Square_Fill();
  DMA_24bit(1, 1, 0, 0, 400, 1280, 400, 0);
  //CARGO FONDOS EN CANVAS 3
  Canvas_Image_Start_address(Canvas3);
  Canvas_image_width(400);
  Main_Image_Start_Address(Canvas3); 
  DMA_24bit(1, 1, 0*128, 0, 128, 697, 128, 1219360);
  DMA_24bit(1, 1, 1*128, 0, 128, 697, 128, 1397792);
  DMA_24bit(1, 1, 2*128, 0, 128, 697, 128, 1576224);
  
  //CARGO FONTS NUMERICO
  Canvas_Image_Start_address(Canvas2);
  Canvas_image_width(400);
  Main_Image_Start_Address(Canvas2);        

  Foreground_color_65k(color65k_blue);
  Line_Start_XY(0,0);
  Line_End_XY(799,1279);
  Start_Square_Fill();
  
  DMA_24bit(1, 1, 0, 0, 74, 660, 74, 1024000);
  DMA_24bit(1, 1, 74, 0, 74, 660, 74, 1121680);
  for (uint8_t i=0;i<5;i++)
  delay(100);
  Canvas_Image_Start_address(Canvas1);
  Canvas_image_width(400);
  Main_Image_Start_Address(Canvas1);        
  

  RedesSocialesInit();

  //delay(2000);
  ledcWrite(ledChannel, 50+6);
  Display_ON();
  //delay(2000);
  xTaskCreate(  LeerREDESTask, // Task function
                "LeerREDES", // Task name
                configMINIMAL_STACK_SIZE + (1024*8), // Stack size 
                NULL, 
                tskIDLE_PRIORITY + 2, // Priority
                &LeerREDESTaskHandle); // Task handler
  xTaskCreate(  PostLCDTask, // Task function
                "PostLCD", // Task name
                configMINIMAL_STACK_SIZE + 2048, // Stack size 
                NULL, 
                tskIDLE_PRIORITY + 2, // Priority
                &POSTLCDTaskHandle); // Task handler
}

void MostrarNumeroYoutube (uint32_t X_init, uint32_t Y_init, uint32_t RecorridoY, uint8_t Saltos, uint8_t Retardo, uint32_t Numero, uint8_t Fondo){
  //FONDO YOUTUBE
  BTE_Memory_Copy_Chroma_key( Canvas3, 400, (Fondo * 128), 0, //SOURCE
                              Canvas1, 400, X_init, Y_init, //DESTINO
                              0x0,//COLOR NEGRO CHROMA KEY
                              128, //ANCHO
                              697);//ALTO
  //NUMERO A CIFRAS
  String BufferNum;
  BufferNum = (String)Numero;
  uint8_t Posicion=0;
  for (uint8_t i=BufferNum.length(); i>0 ;i--){
    //Serial.println (BufferNum[Posicion]);
    //Scroll_Y_Num(48, 60, 480 - (Posicion*60), 5, 3, (BufferNum[i-1] - 48), Fondo);
      for (uint16_t j=0; j<(RecorridoY- (Posicion*60)); j+=Saltos){
         //BORRO RASTRO
         BTE_Memory_Copy_Chroma_key( Canvas3, 400, (Fondo * 128) + 20,0 + 70 + j - Saltos, //SOURCE
                                  Canvas1, 400, X_init + 20, Y_init + 70 + j - Saltos, //DESTINO
                                  0x0,//COLOR NEGRO CHROMA KEY
                                  74, //ANCHO NUM
                                  60);//ALTO NUM
        //MUEVO EL NUMERO
        BTE_Memory_Copy_Chroma_key( Canvas2, 400, 0, (60 * (BufferNum[i-1] - 48)), //SOURCE
                                    Canvas1, 400, X_init + 20, Y_init + 70 + j,//DESTINO
                                    0x0,//COLOR NEGRO CHROMA KEY
                                    74, //ANCHO NUM
                                    60);//ALTO NUM
         vTaskDelay(Retardo);
      }
    Posicion++;
  }
  
}

void PostLCD_TareaPeriodica(){
  NTP_TareaPeriodica();
  vTaskDelay(3000);
  MostrarNumeroYoutube(25, 42, 480, 5, 5, Suscriptores, 0);
  vTaskDelay(3000);
  MostrarNumeroYoutube(25, 42, 480, 5, 5, Seguidores, 1);
  vTaskDelay(3000);
  MostrarNumeroYoutube(25, 42, 480, 5, 5, Likes, 2);
  vTaskDelay(3000);
}
void LeerREDESTask(void *pvParameters){
  for (;;){ 
    RedesSocialesTask();
  }
}
void PostLCDTask(void *pvParameters){
  for(;;){
    PostLCD_TareaPeriodica ();
  }
}
