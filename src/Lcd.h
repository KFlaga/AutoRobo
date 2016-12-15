#ifndef LCD_H_
#define LCD_H_

#include "Core.h"
#include "Pins.h"

#define LcdSetRSWriteCommand() ResetPin(LCD_RS_Port,LCD_RS_Pin)
#define LcdSetRSWriteData() SetPin(LCD_RS_Port,LCD_RS_Pin)
#define LcdEnable() SetPin(LCD_E_Port, LCD_E_Pin)
#define LcdDisable() ResetPin(LCD_E_Port, LCD_E_Pin)

void LcdInitPins4();
void LcdInit4Byte(bool wait_powerup);
void LcdClear4();
void LcdHome4();

// data: xxxxABCD -> D4(D) D5(C) D6(B) D7(A)
void LcdWrite4(uint8_t data);
void LcdWrite8(uint8_t data);

// Writes 8 bits
__inline__ void LcdWrite_NoDelay(uint8_t data);
// Writes line ( if linenum == 2 - line 2, else line 1 )
void LcdWriteLine_NoDelay(uint8_t linenum, uint8_t* data, uint8_t size);

// Inits nodelay functions -> must be called prior to nodelay write
void LcdInitNoDelay();

#define LcdWrite4High(data) LcdWrite4(data>>4)
#define LcdWrite4Low(data) LcdWrite4(data)

#endif
