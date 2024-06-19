#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

// Se declara un manejador de semáforo mutex que se va a usar para administrar el puerto serie.
// Se utiliza para garantizar que solo una Tarea acceda a este recurso en cualquier momento.
SemaphoreHandle_t xSemaforo_Pantalla;

int ruedaDelay = 5; 

TickType_t velocidadDelay;

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT 50
#define LOGO_WIDTH 50


void TaskRueda(void *pvParameters);
void TaskCuadrado(void *pvParameters);

void setup()
{
  Serial.begin(115200);

// Inicializa la pantalla
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

    // Dibuja el marco de la pantalla
    int midScreen = SCREEN_WIDTH / 2; // Calcula el punto medio horizontal de la pantalla
    display.drawRect(0, 0, midScreen, SCREEN_HEIGHT, SSD1306_WHITE);         // Dibuja el lado izquierdo
    display.drawRect(midScreen, 0, midScreen, SCREEN_HEIGHT, SSD1306_WHITE); // Dibuja el lado derecho

    // Muestra el marco en la pantalla
    display.display();

if ( xSemaforo_Pantalla == NULL )  // Confirma que el semáforo del puerto serial aún no se ha creado.
  {
    xSemaforo_Pantalla = xSemaphoreCreateMutex();  // Se crea un semáforo mutex que se usa para administrar el puerto serial
    if ( ( xSemaforo_Pantalla ) != NULL )
      xSemaphoreGive( ( xSemaforo_Pantalla) );  // Hace que el puerto serie esté disponible para su uso, "Dando" el semáforo.
  }

  xTaskCreatePinnedToCore(
      TaskRueda
      , "Rueda"
      ,4096
      ,NULL// Stack siz,
      ,3
      ,NULL // Priority
      ,1);
  xTaskCreatePinnedToCore(
    TaskCuadrado
    ,"Cuadrado"
    , 4096
    ,NULL
    ,6
    ,NULL
    ,0);

}


void loop()
{
  // Empty. Things are done in Tasks.
}


void TaskRueda(void *pvParameters __attribute__((unused)) )
{
  
  uint8_t rueda = 0;
  // 'wheel1', 50x50px
  const unsigned char epd_bitmap_wheel1[] PROGMEM = {
      0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xfc, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
      0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0x80,
      0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03,
      0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x07, 0x00, 0x7f, 0x3f, 0x80, 0x18, 0x00, 0x0e, 0x01, 0xe3,
      0x31, 0xe0, 0x1c, 0x00, 0x0c, 0x03, 0x83, 0x30, 0x70, 0x0e, 0x00, 0x18, 0x07, 0x03, 0x30, 0x38,
      0x06, 0x00, 0x18, 0x0e, 0x03, 0x30, 0x1c, 0x07, 0x00, 0x30, 0x0c, 0x03, 0x30, 0x0e, 0x03, 0x00,
      0x30, 0x18, 0x07, 0xfc, 0x07, 0x03, 0x00, 0x20, 0x38, 0x1f, 0x1e, 0x07, 0x01, 0x80, 0x60, 0x3e,
      0x38, 0x07, 0x1f, 0x81, 0x80, 0x60, 0x7f, 0xf0, 0x03, 0xff, 0x81, 0x80, 0x60, 0x61, 0xe0, 0x01,
      0xf1, 0x80, 0x80, 0x60, 0x60, 0xc0, 0x00, 0xc1, 0xc0, 0xc0, 0x40, 0x78, 0xc0, 0x00, 0xc3, 0xc0,
      0xc0, 0xc0, 0x7f, 0xc0, 0x00, 0x7f, 0xc0, 0xc0, 0xc0, 0xc7, 0xc0, 0x00, 0x7c, 0xc0, 0xc0, 0xc0,
      0xc1, 0xc0, 0x00, 0x60, 0xc0, 0xc0, 0xc0, 0x40, 0xc0, 0x00, 0x60, 0xc0, 0xc0, 0x40, 0x60, 0xc0,
      0x00, 0xc0, 0xc0, 0xc0, 0x60, 0x60, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0x60, 0x60, 0x60, 0x01, 0xc1,
      0x80, 0x80, 0x60, 0x70, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x30, 0x78, 0x07, 0xc3, 0x81, 0x80,
      0x20, 0x38, 0xde, 0x1e, 0xc3, 0x01, 0x80, 0x30, 0x19, 0xcf, 0xfc, 0x67, 0x03, 0x00, 0x30, 0x0f,
      0x8c, 0x0c, 0x7e, 0x03, 0x00, 0x18, 0x0f, 0x1c, 0x06, 0x3c, 0x07, 0x00, 0x18, 0x07, 0x18, 0x03,
      0x38, 0x06, 0x00, 0x0c, 0x03, 0xf0, 0x03, 0xf0, 0x0e, 0x00, 0x0e, 0x01, 0xf0, 0x01, 0xe0, 0x0c,
      0x00, 0x07, 0x00, 0x7c, 0x0f, 0x80, 0x18, 0x00, 0x03, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x01,
      0x80, 0x01, 0xe0, 0x00, 0x70, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x70, 0x00,
      0x00, 0x01, 0xc0, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
      0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
      0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00};
  // 'wheel2', 50x50px
  const unsigned char epd_bitmap_wheel2[] PROGMEM = {
      0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xf8, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
      0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x03, 0x80,
      0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03,
      0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x07, 0x00, 0x7e, 0x3f, 0x80, 0x38, 0x00, 0x0e, 0x01, 0xe0,
      0x33, 0xe0, 0x1c, 0x00, 0x0c, 0x03, 0x80, 0x63, 0xf0, 0x0c, 0x00, 0x1c, 0x07, 0x00, 0x63, 0x38,
      0x06, 0x00, 0x18, 0x0f, 0x00, 0x62, 0x1c, 0x06, 0x00, 0x30, 0x0f, 0x80, 0xe6, 0x0e, 0x03, 0x00,
      0x30, 0x19, 0xe7, 0xfe, 0x06, 0x03, 0x00, 0x30, 0x38, 0x7f, 0x1e, 0x03, 0x01, 0x00, 0x60, 0x3c,
      0x38, 0x07, 0x03, 0x01, 0x80, 0x60, 0x7e, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x67, 0xe0, 0x01,
      0x81, 0x81, 0x80, 0x60, 0x61, 0xc0, 0x00, 0xc1, 0x81, 0x80, 0x60, 0x60, 0xc0, 0x00, 0xff, 0x80,
      0x80, 0x40, 0x40, 0xc0, 0x00, 0xff, 0xc0, 0xc0, 0x40, 0x40, 0xc0, 0x00, 0xe0, 0xc0, 0xc0, 0x40,
      0x40, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0x40, 0x40, 0xc0, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x60, 0xc0,
      0x00, 0xff, 0x80, 0x80, 0x60, 0x61, 0xc0, 0x00, 0xc1, 0x81, 0x80, 0x60, 0x67, 0xe0, 0x01, 0x81,
      0x81, 0x80, 0x60, 0x7e, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x3c, 0x38, 0x07, 0x03, 0x01, 0x80,
      0x30, 0x38, 0x7e, 0x1e, 0x03, 0x01, 0x00, 0x30, 0x18, 0xe7, 0xfe, 0x06, 0x03, 0x00, 0x30, 0x1f,
      0x80, 0xe6, 0x0e, 0x03, 0x00, 0x18, 0x0f, 0x00, 0x62, 0x1c, 0x06, 0x00, 0x1c, 0x07, 0x00, 0x63,
      0x38, 0x06, 0x00, 0x0c, 0x03, 0x80, 0x63, 0xf0, 0x0c, 0x00, 0x0e, 0x01, 0xe0, 0x31, 0xe0, 0x1c,
      0x00, 0x07, 0x00, 0x7c, 0x3f, 0x80, 0x38, 0x00, 0x03, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x01,
      0x80, 0x01, 0xe0, 0x00, 0x60, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x70, 0x00,
      0x00, 0x03, 0x80, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
      0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
      0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00};
  // 'wheel3', 50x50px
  const unsigned char epd_bitmap_wheel3[] PROGMEM = {
      0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xfc, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
      0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x03, 0x80,
      0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03,
      0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x06, 0x00, 0x7c, 0x0f, 0x80, 0x38, 0x00, 0x0e, 0x01, 0xf0,
      0x03, 0xe0, 0x1c, 0x00, 0x0c, 0x03, 0xf0, 0x03, 0xf0, 0x0c, 0x00, 0x18, 0x07, 0x38, 0x07, 0x38,
      0x06, 0x00, 0x18, 0x0f, 0x18, 0x06, 0x3c, 0x06, 0x00, 0x30, 0x1f, 0x8c, 0x0c, 0x7e, 0x03, 0x00,
      0x30, 0x19, 0x8f, 0xfc, 0x66, 0x03, 0x00, 0x60, 0x30, 0xde, 0x3e, 0xc3, 0x01, 0x00, 0x60, 0x70,
      0xf8, 0x07, 0x83, 0x01, 0x80, 0x60, 0x60, 0x70, 0x03, 0x81, 0x81, 0x80, 0x60, 0x60, 0x60, 0x01,
      0x81, 0x81, 0x80, 0x40, 0x60, 0xc0, 0x00, 0xc1, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0x00, 0xc0, 0x80,
      0x80, 0xc0, 0xc1, 0x80, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1, 0x80, 0x00, 0xe0, 0xc0, 0xc0, 0xc0,
      0xc7, 0x80, 0x00, 0xf8, 0xc0, 0xc0, 0xc0, 0xff, 0x80, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xf8, 0xc0,
      0x00, 0xc7, 0x80, 0x80, 0xc0, 0x60, 0xc0, 0x00, 0xc1, 0x80, 0x80, 0x60, 0x61, 0xe0, 0x01, 0xe1,
      0x81, 0x80, 0x60, 0x7f, 0xf0, 0x03, 0xff, 0x81, 0x80, 0x60, 0x7e, 0x38, 0x07, 0x1f, 0x01, 0x80,
      0x60, 0x38, 0x1e, 0x1e, 0x07, 0x01, 0x00, 0x30, 0x18, 0x0f, 0xf8, 0x06, 0x03, 0x00, 0x30, 0x1c,
      0x03, 0x70, 0x0e, 0x03, 0x00, 0x18, 0x0e, 0x03, 0x30, 0x1c, 0x06, 0x00, 0x18, 0x07, 0x03, 0x30,
      0x38, 0x06, 0x00, 0x0c, 0x03, 0x83, 0x30, 0x70, 0x0c, 0x00, 0x0e, 0x01, 0xe3, 0x31, 0xe0, 0x1c,
      0x00, 0x06, 0x00, 0x7f, 0x3f, 0x80, 0x18, 0x00, 0x03, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x01,
      0x80, 0x01, 0xe0, 0x00, 0x60, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x60, 0x00,
      0x00, 0x03, 0x80, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
      0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
      0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00};
  // 'wheel4', 50x50px
  const unsigned char epd_bitmap_wheel4[] PROGMEM = {
      0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xfc, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
      0x1e, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x03, 0x80,
      0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x07,
      0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x06, 0x00, 0x7f, 0x0f, 0x80, 0x18, 0x00, 0x0c, 0x01, 0xf3,
      0x01, 0xe0, 0x1c, 0x00, 0x1c, 0x03, 0xf3, 0x00, 0x70, 0x0c, 0x00, 0x18, 0x07, 0x31, 0x80, 0x38,
      0x06, 0x00, 0x30, 0x0e, 0x31, 0x80, 0x3c, 0x07, 0x00, 0x30, 0x1c, 0x19, 0x80, 0x7e, 0x03, 0x00,
      0x30, 0x38, 0x1f, 0xf9, 0xe6, 0x03, 0x00, 0x60, 0x30, 0x1e, 0x1f, 0x87, 0x01, 0x80, 0x60, 0x70,
      0x38, 0x07, 0x0f, 0x01, 0x80, 0x60, 0x60, 0x70, 0x03, 0x9f, 0x81, 0x80, 0xc0, 0x60, 0xe0, 0x01,
      0xf9, 0x80, 0x80, 0xc0, 0x60, 0xc0, 0x00, 0xe1, 0x80, 0xc0, 0xc0, 0xff, 0xc0, 0x00, 0xc0, 0x80,
      0xc0, 0xc0, 0xff, 0x80, 0x00, 0x40, 0xc0, 0xc0, 0xc0, 0xc1, 0x80, 0x00, 0x60, 0xc0, 0xc0, 0xc0,
      0xc1, 0x80, 0x00, 0x60, 0xc0, 0xc0, 0xc0, 0xff, 0x80, 0x00, 0x40, 0xc0, 0xc0, 0xc0, 0xff, 0xc0,
      0x00, 0xc0, 0x80, 0xc0, 0xc0, 0x60, 0xc0, 0x00, 0xe1, 0x80, 0xc0, 0xc0, 0x60, 0xe0, 0x01, 0xf9,
      0x80, 0x80, 0x60, 0x60, 0x70, 0x03, 0x9f, 0x81, 0x80, 0x60, 0x60, 0x38, 0x07, 0x0f, 0x01, 0x80,
      0x60, 0x30, 0x1e, 0x1f, 0x87, 0x01, 0x80, 0x30, 0x38, 0x1f, 0xf9, 0xe6, 0x03, 0x00, 0x30, 0x1c,
      0x19, 0xc0, 0x7e, 0x03, 0x00, 0x30, 0x0c, 0x31, 0x80, 0x3c, 0x07, 0x00, 0x18, 0x07, 0x31, 0x80,
      0x38, 0x06, 0x00, 0x1c, 0x03, 0xf3, 0x00, 0x70, 0x0c, 0x00, 0x0c, 0x01, 0xf3, 0x01, 0xe0, 0x0c,
      0x00, 0x06, 0x00, 0x7f, 0x0f, 0x80, 0x18, 0x00, 0x07, 0x00, 0x1f, 0xfe, 0x00, 0x30, 0x00, 0x03,
      0x80, 0x01, 0xe0, 0x00, 0x60, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00,
      0x00, 0x01, 0x80, 0x00, 0x00, 0x38, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x1e,
      0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xe0, 0x00, 0x00,
      0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00};

  // Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1472)
  const int epd_bitmap_allArray_LEN = 4;
  const unsigned char *epd_bitmap_allArray[4] = {
      epd_bitmap_wheel1,
      epd_bitmap_wheel2,
      epd_bitmap_wheel3,
      epd_bitmap_wheel4};

  while (1)
  {
    if ( xSemaphoreTake( xSemaforo_Pantalla, ( TickType_t ) 200 ) == pdTRUE )
    {
    // Draw the wheel image in the left half
    switch (rueda)
    {
    case 0:
      display.drawBitmap(7, (SCREEN_HEIGHT - LOGO_HEIGHT) / 2, epd_bitmap_wheel1, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
      rueda = 1;
      break;
    case 1:
      display.drawBitmap(7, (SCREEN_HEIGHT - LOGO_HEIGHT) / 2, epd_bitmap_wheel2, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
      rueda = 2;
      break;
    case 2:
      display.drawBitmap(7, (SCREEN_HEIGHT - LOGO_HEIGHT) / 2, epd_bitmap_wheel3, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
      rueda = 0;
      break;
    }
    display.display();
    
  xSemaphoreGive( xSemaforo_Pantalla);
  }
  vTaskDelay(ruedaDelay); // puedo controlar la velocidad de la rueda esto debo cambiar con el teclado

}
}
void TaskCuadrado(void *pvParameters __attribute__((unused)) ){
  
  int rectWidth = 20;
  int rectHeight = 20;

  // Coordenadas del rectángulo
  int x = 85;
  int y = 20;
  
  (void)pvParameters;

  if ( xSemaphoreTake(xSemaforo_Pantalla, ( TickType_t ) 200 ) == pdTRUE )
    {
  while (1)
  {
  // Rellenar de afuera hacia adentro
  for(int i = 0; i <= rectWidth / 2; i++) {
    display.drawRect(x + i, y + i, rectWidth - 2 * i, rectHeight - 2 * i, SSD1306_WHITE);
    display.display();
  }
  // Vaciar de afuera hacia adentro
  for(int i = 0; i <= rectWidth / 2; i++) {
    display.drawRect(x + i, y + i, rectWidth - 2 * i, rectHeight - 2 * i, SSD1306_BLACK);
    display.display();
  }
}
xSemaphoreGive( xSemaforo_Pantalla);
}
vTaskDelay(5); 
}
  
