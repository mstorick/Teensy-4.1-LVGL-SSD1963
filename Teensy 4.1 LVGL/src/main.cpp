// ------------------ changes in tft_drivers/ssd1963/800alt/initlcd.h ------------------
//	// LCD_Write_COM(0x36);		//rotation
//	// LCD_Write_DATA(0x22);		// -- Set to 0x21 to rotate 180 degrees
//	LCD_Write_COM(0x36); // Set Address Mode
//	LCD_Write_DATA(0b01000010); //old = 0b00100010
// ------------------ changes in XPT2046_Touchscreen.cpp ------------------
// #define Z_THRESHOLD     600
// 
//   case 1:
// 	// xraw = x;
// 	// yraw = y;
// 	xraw = y;
// 	yraw = x;
// ------------------ changes in XPT2046_Touchscreen.h ------------------
// #if defined(__IMXRT1062__)
// #if __has_include(<FlexIOSPI.h>)
// 	#include <FlexIOSPI.h>
// #endif
// #endif
// ------------------ changes in tft_drivers/ssd1963/800alt/initlcd.h ------------------
// (put in all the delays)
//case SSD1963_800ALT:
	// LCD_Write_COM(0xE2);		//PLL multiplier, set PLL clock to 120M
	// delay(1);
	// LCD_Write_DATA(0x23);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	// delay(1);
	// LCD_Write_DATA(0x02);
	// delay(1);
	// LCD_Write_DATA(0x04);
	// delay(1);
// ------------------ changes in lv_conf.h -------------------------------
//#if 1
//
//#define LV_TICK_CUSTOM     1

#include <Arduino.h>
#include <lvgl.h>
// #include <TFT_eSPI.h>
// #include <lv_demo.h>
#include <UTFT.h>
#include <XPT2046_Touchscreen.h>

#define CS_PIN 10 //also ss
#define TIRQ_PIN 9
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);

const float A = 4.0195;
const float B = -1.1922;
const float C = 743.2416;
const float D = -1.1328;
const float E = 5.6296;
const float F = 1331.5;
// float A = 4.0016;
// float B = -1.1592;
// float C = 746.6656;
// float D = -1.1971;
// float E = 5.7480;
// float F = 1343.8;

extern uint8_t SmallFont[];

int R = 0;
int c = 0;

UTFT myGLCD(SSD1963_800ALT,3,4,22,23);   // Remember to change the model parameter to suit your display module!

/*Change to your screen resolution*/
static const uint32_t screenWidth  = 800;
static const uint32_t screenHeight = 480;

static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[ screenWidth * 10 ];
static lv_color_t* buf = new lv_color_t[ screenWidth * 300 ];

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print( lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc )
{
   Serial.printf( "%s(%s)@%d->%s\r\n", file, fn_name, line, dsc );
   Serial.flush();
}
#endif


void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
   uint32_t w = (area->x2 - area->x1 + 1);
   uint32_t h = (area->y2 - area->y1 + 1);
   uint32_t wh = w*h;

   cbi(myGLCD.P_CS, myGLCD.B_CS);
   myGLCD.setXY(area->x1, area->y1, area->x2, area->y2);
   // Serial.print(area->x1); Serial.print(", ");
   // Serial.print(area->y1); Serial.print(", ");
   // Serial.print(area->x2); Serial.print(", ");
   // Serial.print(area->y2); Serial.print("\n");

   uint32_t i = 0;
   uint32_t pix_len = 0;
   while(i < wh) {
      pix_len++;
      if(color_p->full == (color_p+1)->full && i+1 < wh) {
      } else {
         myGLCD.setColor(color_p->full);

         cbi(myGLCD.P_CS, myGLCD.B_CS);
         sbi(myGLCD.P_RS, myGLCD.B_RS);
         myGLCD._fast_fill_16(myGLCD.fch,myGLCD.fcl,(long)pix_len);
         sbi(myGLCD.P_CS, myGLCD.B_CS);
         pix_len = 0;
      }
      i++;
      color_p++;
   }
    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

void my_disp_flush_old(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x, y;

    /*It's a very slow but simple implementation.
     *`set_pixel` needs to be written by you to a set pixel on the screen*/
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            myGLCD.setColor(color_p->full);
            myGLCD.fillRect(x, y, x, y);
            color_p++;
        }
    }

    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}


/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
  float x, y;

  if (ts.touched()) {
     TS_Point d = ts.getPoint();
     if(d.z > 1000) {
        y = (A*d.y - D*d.x + D*C - A*F)/(A*E - D*B);
        x = (d.x - B*y - C)/A;
         if( x>=0 && x<=800 && y>=0 && y<=480) {
            if (x < 200 || (x < 280 && d.z > 1200) || (x < 430 && d.z > 1475) || d.z > 1800) {
                data->state = LV_INDEV_STATE_PR;
                data->point.x = (int16_t)x;
                data->point.y = (int16_t)y;

                //   Serial.print(x); Serial.print(",");
                //   Serial.print(y); Serial.print(",");
                //   Serial.print(d.z); Serial.print("\n");
                return;
            }
        }
     }
  }

  data->state = LV_INDEV_STATE_REL;
  return;



  //  uint16_t touchX, touchY;

  //  bool touched = tft.getTouch( &touchX, &touchY, 600 );

  //  if( !touched )
  //  {
  //     data->state = LV_INDEV_STATE_REL;
  //  }
  //  else
  //  {
  //     data->state = LV_INDEV_STATE_PR;

  //     /*Set the coordinates*/
  //     data->point.x = touchX;
  //     data->point.y = touchY;

  //     Serial.print( "Data x " );
  //     Serial.println( touchX );

  //     Serial.print( "Data y " );
  //     Serial.println( touchY );
  //  }
}

// ------------------------------------------- Begin LVGL Code --------------------------------------------------- //


// ------------------------------------------- End LVGL Code --------------------------------------------------- //

void setup()
{
   delay(100);
   myGLCD.InitLCD();
   myGLCD.setFont(SmallFont);
   myGLCD.clrScr();
   // myGLCD.LCD_Write_COM(0x36); // Set Address Mode
   // myGLCD.LCD_Write_DATA(0b11111111); //old = 0b00100010
   // delay(1000);
   Serial.begin( 115200 ); /* prepare for possible serial debug */
//    Serial.println( "Hello Arduino! (V8.0.X)" );
//    Serial.println( "I am LVGL_Arduino" );
   //SS=10,SCK=13,MOSI=11,MISO=12

   ts.begin();
   ts.setRotation(1);

   lv_init();

#if LV_USE_LOG != 0
   lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

   // tft.begin();          /* TFT init */
   // tft.setRotation( 2 ); /* Landscape orientation, flipped */

   /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  //  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  //  tft.setTouch( calData );

   lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 300 );

   /*Initialize the display*/
   static lv_disp_drv_t disp_drv;
   lv_disp_drv_init( &disp_drv );
   /*Change the following line to your display resolution*/
   disp_drv.hor_res = screenWidth;
   disp_drv.ver_res = screenHeight;
   disp_drv.flush_cb = my_disp_flush;
   disp_drv.draw_buf = &draw_buf;
   lv_disp_drv_register( &disp_drv );

   /*Initialize the input device driver*/
   static lv_indev_drv_t indev_drv;
   lv_indev_drv_init( &indev_drv );
   indev_drv.type = LV_INDEV_TYPE_POINTER;
   indev_drv.read_cb = my_touchpad_read;
   lv_indev_drv_register( &indev_drv );


   // ------------------------------------------- Begin LVGL Code --------------------------------------------------- //
    lv_obj_t* button;
    button = lv_btn_create(lv_scr_act());
    lv_obj_set_size(button, 120, 80);
    lv_obj_set_pos(button, 10, 200);
    //lv_obj_add_event_cb(button, btn_clicked, LV_EVENT_PRESSED, NULL);


   // ------------------------------------------- End LVGL Code --------------------------------------------------- //

   Serial.println( "Setup done" );
}

void loop()
{
   while(1) {
      lv_timer_handler(); // let the GUI do its work
      delay( 10 );
      // Serial.println(F_CPU);
      // Serial.println(F_BUS);
   }
}