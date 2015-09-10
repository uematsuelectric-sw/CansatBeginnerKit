/*
SaveData:
 データを保存するためのライブラリ。
 保存対象のデバイスに依存せずデータを保存することを目的とする。

 なお現時点では、ATMEGA328Pの内蔵EEPROM(1KByte)に保存する。

The MIT License (MIT)

Copyright (c) 2015 Uematsu Electric Co.,Ltd. Toru OHTSUKA <t-ohtsuka@jupiter.ocn.ne.jp>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "SaveData.h"
#include "EEPROM.h"

#define DEBUG
#ifdef DEBUG
#include "DigiUSB.h"
#define DBG_PRINT(...) DigiUSB.print(__VA_ARGS__)
#endif

extern EEPROMClass EEPROM;

#define EEPROM_MAX_328P 1000
#define EEPROM_MAX_328P_SAVE2BYTE (1000/2)

/****************************************************************************/
void EEPROM_clear() {
	int i;
	
	for(i=0; i<EEPROM_MAX_328P; i++) {
		EEPROM.write(i , 0) ;
	}
}

/****************************************************************************/
int EEPROM_save2byte(unsigned short address, unsigned short data) {
	byte upper8bit,lower8bit;
	lower8bit = data & 0x00FF;			// 下位8bitのみ取得
	upper8bit = (data >> 8) & 0xFF; // 上位8bitのみ取得
	
	if(address >= EEPROM_MAX_328P_SAVE2BYTE) { // 範囲外アクセス防止
		return -1;
	}
	
#if 0
	DBG_PRINT("EEPROM_save2byte:");
	DBG_PRINT("L8:");
	DBG_PRINT(lower8bit, DEC);
	DBG_PRINT(",H8:");
	DBG_PRINT(upper8bit, DEC);
#endif

	EEPROM.write(address*2, lower8bit);
	EEPROM.write(address*2+1, upper8bit);
	
	return 0;
}

/****************************************************************************/
int EEPROM_load2byte(unsigned short address, unsigned short *data) {
	unsigned short upper8bit,lower8bit;
	
	if(address >= EEPROM_MAX_328P_SAVE2BYTE) { // 範囲外アクセス防止
		return -1;
	}
	
	lower8bit = EEPROM.read(address*2);
	upper8bit = EEPROM.read(address*2+1);
	
#if 0
	DBG_PRINT("EEPROM_load2byte:");
	DBG_PRINT("READ	lower8bit:");
	DBG_PRINT(lower8bit, DEC);
	DBG_PRINT(", upper8bit:");
	DBG_PRINT(upper8bit, DEC);
	DBG_PRINT("\r\n");
#endif
	
	*data = (upper8bit << 8) + lower8bit;
	
	return 0;
}

/****************************************************************************/
SaveDataClass::SaveDataClass(void)
{
	EEPROM_load2byte(0, &count);
}

/****************************************************************************/
void SaveDataClass::clear(void)
{
	EEPROM_clear();
	count = 0;
}

/****************************************************************************/
void SaveDataClass::init(void)
{
	int ret;
	ret = EEPROM_load2byte(0, &count);
}
/****************************************************************************/
unsigned short SaveDataClass::get_count(void) {
	return count;
}

/****************************************************************************/
int SaveDataClass::save_2byte(unsigned short address, unsigned short data) {
	
	return EEPROM_save2byte(address, data);
}

/****************************************************************************/
int SaveDataClass::load_2byte(unsigned short address, unsigned short *pdata) {
	
	return EEPROM_load2byte(address, pdata);
}

/****************************************************************************/
int SaveDataClass::add_2byte(unsigned short data) {
	
	// カウンタの更新と保存
	if(count+1 < EEPROM_MAX_328P_SAVE2BYTE) {
		EEPROM_save2byte(count+1, data); // 先頭2byteは管理領域。スキップするため +1 する
	
		count++;
		DBG_PRINT("add_2byte:");
		DBG_PRINT(count, DEC);
		DBG_PRINT("\r\n");
		EEPROM_save2byte(0, count);
		
		return 0;
	} else {
		return -1;
	}
}

SaveDataClass SaveData;

