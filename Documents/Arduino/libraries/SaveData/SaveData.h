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

#ifndef __SAVEDATA_H
#define __SAVEDATA_H

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class SaveDataClass
{
  public:
	SaveDataClass(void);
	void clear(void);
	void init(void);
	unsigned short get_count(void);
	int add_2byte(unsigned short data);
	int save_2byte(unsigned short address, unsigned short pdata);
	int load_2byte(unsigned short address, unsigned short *pdata);

  private:
	unsigned short count;
};

extern SaveDataClass SaveData;

#endif /* __SAVEDATA_H */
