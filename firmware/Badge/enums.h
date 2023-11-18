#ifndef _ENUMS_H_
#define _ENUMS_H_

#define FLASH_TARGET_OFFSET (256 * 1024)
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
//#define buzzer A2
#define btn 6
#define amp_en 5
#define tiempoPunto 70
#define tiempoRaya 210
#define silencioLetra 210
#define silencioPalabra 350

//Files system enum
enum{
  FS_ROOT=0,
  FS_HERRAMIENTAS,    //Juego
  FS_BUGCON,          //bugcon
  FS_CURIOSIDADES
  };

//Command enum
enum{
  CM_LS=1,
  CM_PWD,
  CM_CD,
  CM_CDA,
  CM_CDB,
  CM_CDC,
  CM_EXEA,
  CM_EXEB,
  CM_EXEC,
  CM_EXED,
  CM_EXEE,
  CM_EXEF,
  CM_EXEG,
  CM_EXEH,
  CM_HLP,
  };

//ID HID enum
enum
{
  RID_KEYBOARD = 1,
  RID_MOUSE,
  RID_CONSUMER_CONTROL, // Media, volume etc ..
};

// message ID
enum
{
  KEYBOARD_MESSAGE = 0,
  MOUSE_MESSAGE,
  CONSUMER_CONTROL_MESSAGE, // Media, volume etc ..
};

typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

int8_t processSerialCommand(void);
void printFS(int);
String serialDataString="";
void prueba1(void);
void printIndexFS(int index);
void reset(void);
#endif
