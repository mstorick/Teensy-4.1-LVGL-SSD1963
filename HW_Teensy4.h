/**
 * Set your pins here
 *
 * For the teensy 4.0 and 4.1
 *
 * If PORTS = USE_6_7_PORTS:
 *   - Faster GPIO writing speed.  Wire the correct pins to the LCD as shown in 
 *     the section defining the pins.
 *
 * If PORTS = USE_USER_PORTS
 *   - This allows you to define your own set of pins to use. Since we're not
 *     collating them into groupings that can have their writes optimized, it
 *     will take 16 write cycles to write out the data for one pixel. However,
 *     it also gives you the most flexibility in which pins you use for the
 *     LCD data bus. If high-speed/optimized data writes is not important to you
 *     but simplicity of wiring layout is (or if the above options conflict with
 *     something else you're using on the teensy), you can use this setting to
 *     set the pins where-ever you'd like them to be. By default it's set to
 *     pins DB0-DB15 = 0-15. You should modify the "DB_0" through to "DB_15"
 *     defines to match the pins you wish to use.
 */

// from https://github.com/ManojBR105/Teensy-4.0_digital_parallel_read-write
#define lsb0_3mask_8bit     0x0f             //                               0000 1111 in binary
#define lsb4_7mask_8bit     0xf0             //                               1111 0000 in binary
#define lsb4_9mask_16bit    0x03f0           //                     0000 0011 1111 0000 in binary
#define lsb10_11mask_16bit  0x0c00           //                     0000 1100 0000 0000 in binary
#define lsb12_15mask_16bit  0xf000           //                     1111 0000 0000 0000 in binary
// #define lsb16_18mask_32bit  0x00070000       // 0000 0000 0000 0111 0000 0000 0000 0000 in binary
// #define lsb19_19mask_32bit  0x00080000       // 0000 0000 0000 1000 0000 0000 0000 0000 in binary
// #define lsb20_21mask_32bit  0x00300000       // 0000 0000 0011 0000 0000 0000 0000 0000 in binary
// #define lsb22_23mask_32bit  0x00c00000       // 0000 0000 1100 0000 0000 0000 0000 0000 in binary
#define safe_clear6_8bit(n)  (n & 0xfc30ffff)// 1111 1100 0011 0000 1111 1111 1111 1111 in binary
#define safe_clear6_10bit(n) (n & 0xf030ffff)// 1111 0000 0011 0000 1111 1111 1111 1111 in binary
#define safe_clear6_12bit(n) (n & 0xf030fff3)// 1111 0000 0011 0000 1111 1111 1111 0011 in binary
#define safe_clear7_4bit(n)  (n & 0xfffffff0)// 1111 1111 1111 1111 1111 1111 1111 0000 in binary
#define safe_clear7_8bit(n)  (n & 0xfffcf3f0)// 1111 1111 1111 1100 1111 0011 1111 0000 in binary
#define safe_clear9_4bit(n)  (n & 0xfffffe8f)// 1111 1111 1111 1111 1111 1110 1000 1111 in binary 
static inline void safe_write_8bit(uint8_t data) {
  GPIO6_DR = safe_clear6_8bit(GPIO6_DR) | ((data & lsb0_3mask_8bit) << 16) | ((data & lsb4_7mask_8bit) << 18);
}
static inline void safe_write_16bit(uint16_t data) {
  GPIO6_DR = safe_clear6_12bit(GPIO6_DR) | ((data & lsb4_9mask_16bit) << 18) | ((data & lsb10_11mask_16bit) >> 8) | ((data & lsb12_15mask_16bit) << 4);
  GPIO7_DR = safe_clear7_4bit(GPIO7_DR)  |  (data & lsb0_3mask_8bit);
}

// my custom implementation
//                                       D3  D2                D1  D0
//                                         NN                    NN
#define clear6_16bit(n) (n & 0x3030cff3)// 0011 0000 0011 0000 1100 1111 1111 0011 in binary
#define lsb0_1mask_8bit 0x03             //                               0000 0011 in binary
#define lsb2_3mask_8bit 0x0c             //                               0000 1100 in binary
static inline void superspeed_write_16bit(uint16_t data) {
  GPIO6_DR = clear6_16bit(GPIO6_DR) | ((data & lsb4_9mask_16bit) << 18) | ((data & lsb10_11mask_16bit) >> 8) | ((data & lsb12_15mask_16bit) << 4)
   | ((data & lsb0_1mask_8bit) << 12) | ((data & lsb2_3mask_8bit) << 28);
}



#define USE_USER_PORTS 0
#define USE_6_7_PORTS_16BIT 1
#define USE_6_7_PORTS_8BIT 2
#define USE_PORT_6_SUPERSPEED_16BIT 3

// SET WHICH PIN DEFINITIONS TO USE HERE
// (only uncomment 1 of these lines)
//
// #define PORTS  USE_USER_PORTS
// #define PORTS  USE_6_7_PORTS_16BIT
// #define PORTS  USE_6_7_PORTS_8BIT
#define PORTS  USE_PORT_6_SUPERSPEED_16BIT

#if (PORTS == USE_USER_PORTS)
	#pragma message("Using user-defined pins")
	#define DB_0 0
	#define DB_1 1
	#define DB_2 2
	#define DB_3 3
	#define DB_4 4
	#define DB_5 5
	#define DB_6 6
	#define DB_7 7
	#define DB_8 8
	#define DB_9 9
	#define DB_10 10
	#define DB_11 11
	#define DB_12 12
	#define DB_13 13
	#define DB_14 14
	#define DB_15 15
#elif (PORTS == USE_6_7_PORTS_8BIT)
	#define DB_8 19
	#define DB_9 18
	#define DB_10 14
	#define DB_11 15
	#define DB_12 17
	#define DB_13 16
	#define DB_14 22
	#define DB_15 23
#elif (PORTS == USE_6_7_PORTS_16BIT)
	#define DB_0 10
	#define DB_1 12
	#define DB_2 11
	#define DB_3 13
	#define DB_4 17
	#define DB_5 16
	#define DB_6 22
	#define DB_7 23
	#define DB_8 20
	#define DB_9 21
	#define DB_10 1
	#define DB_11 0
	#define DB_12 19
	#define DB_13 18
	#define DB_14 14
	#define DB_15 15
#elif (PORTS == USE_PORT_6_SUPERSPEED_16BIT)
	#define DB_0 24
	#define DB_1 25
	#define DB_2 26
	#define DB_3 27
	#define DB_4 17
	#define DB_5 16
	#define DB_6 22
	#define DB_7 23
	#define DB_8 20
	#define DB_9 21
	#define DB_10 1
	#define DB_11 0
	#define DB_12 19
	#define DB_13 18
	#define DB_14 14
	#define DB_15 15
#endif

// *** Hardware specific functions ***
void UTFT::_hw_special_init()
{
}

void UTFT::LCD_Writ_Bus(char ch,char cl, byte mode)
{
	switch (mode)
	{
	case 1:
		// Not implemented
		break;
	case 8:
#if (PORTS == USE_6_7_PORTS_8BIT)
		safe_write_8bit(ch);
		pulse_low(P_WR, B_WR);
		safe_write_8bit(cl);
		pulse_low(P_WR, B_WR);
#else
		((ch & 0x80) != 0) ? digitalWriteFast(DB_15, HIGH) : digitalWriteFast(DB_15, LOW);
		((ch & 0x40) != 0) ? digitalWriteFast(DB_14, HIGH) : digitalWriteFast(DB_14, LOW);
		((ch & 0x20) != 0) ? digitalWriteFast(DB_13, HIGH) : digitalWriteFast(DB_13, LOW);
		((ch & 0x10) != 0) ? digitalWriteFast(DB_12, HIGH) : digitalWriteFast(DB_12, LOW);
		((ch & 0x08) != 0) ? digitalWriteFast(DB_11, HIGH) : digitalWriteFast(DB_11, LOW);
		((ch & 0x04) != 0) ? digitalWriteFast(DB_10, HIGH) : digitalWriteFast(DB_10, LOW);
		((ch & 0x02) != 0) ? digitalWriteFast(DB_9, HIGH) : digitalWriteFast(DB_9, LOW);
		((ch & 0x01) != 0) ? digitalWriteFast(DB_8, HIGH) : digitalWriteFast(DB_8, LOW);
		pulse_low(P_WR, B_WR);
		((cl & 0x80) != 0) ? digitalWriteFast(DB_15, HIGH) : digitalWriteFast(DB_15, LOW);
		((cl & 0x40) != 0) ? digitalWriteFast(DB_14, HIGH) : digitalWriteFast(DB_14, LOW);
		((cl & 0x20) != 0) ? digitalWriteFast(DB_13, HIGH) : digitalWriteFast(DB_13, LOW);
		((cl & 0x10) != 0) ? digitalWriteFast(DB_12, HIGH) : digitalWriteFast(DB_12, LOW);
		((cl & 0x08) != 0) ? digitalWriteFast(DB_11, HIGH) : digitalWriteFast(DB_11, LOW);
		((cl & 0x04) != 0) ? digitalWriteFast(DB_10, HIGH) : digitalWriteFast(DB_10, LOW);
		((cl & 0x02) != 0) ? digitalWriteFast(DB_9, HIGH) : digitalWriteFast(DB_9, LOW);
		((cl & 0x01) != 0) ? digitalWriteFast(DB_8, HIGH) : digitalWriteFast(DB_8, LOW);
		pulse_low(P_WR, B_WR);
#endif
		break;
	case 16:
#if (PORTS == USE_6_7_PORTS_16BIT)
		// ((uint16_t)ch << 8) | cl;
		safe_write_16bit( (((uint16_t)ch) << 8) | ((uint16_t)cl) );
#elif (PORTS == USE_PORT_6_SUPERSPEED_16BIT)
		superspeed_write_16bit( (((uint16_t)ch) << 8) | ((uint16_t)cl) );
#else
		((ch & 0x80) != 0) ? digitalWriteFast(DB_15, HIGH) : digitalWriteFast(DB_15, LOW);
		((ch & 0x40) != 0) ? digitalWriteFast(DB_14, HIGH) : digitalWriteFast(DB_14, LOW);
		((ch & 0x20) != 0) ? digitalWriteFast(DB_13, HIGH) : digitalWriteFast(DB_13, LOW);
		((ch & 0x10) != 0) ? digitalWriteFast(DB_12, HIGH) : digitalWriteFast(DB_12, LOW);
		((ch & 0x08) != 0) ? digitalWriteFast(DB_11, HIGH) : digitalWriteFast(DB_11, LOW);
		((ch & 0x04) != 0) ? digitalWriteFast(DB_10, HIGH) : digitalWriteFast(DB_10, LOW);
		((ch & 0x02) != 0) ? digitalWriteFast(DB_9, HIGH) : digitalWriteFast(DB_9, LOW);
		((ch & 0x01) != 0) ? digitalWriteFast(DB_8, HIGH) : digitalWriteFast(DB_8, LOW);
		((cl & 0x80) != 0) ? digitalWriteFast(DB_7, HIGH) : digitalWriteFast(DB_7, LOW);
		((cl & 0x40) != 0) ? digitalWriteFast(DB_6, HIGH) : digitalWriteFast(DB_6, LOW);
		((cl & 0x20) != 0) ? digitalWriteFast(DB_5, HIGH) : digitalWriteFast(DB_5, LOW);
		((cl & 0x10) != 0) ? digitalWriteFast(DB_4, HIGH) : digitalWriteFast(DB_4, LOW);
		((cl & 0x08) != 0) ? digitalWriteFast(DB_3, HIGH) : digitalWriteFast(DB_3, LOW);
		((cl & 0x04) != 0) ? digitalWriteFast(DB_2, HIGH) : digitalWriteFast(DB_2, LOW);
		((cl & 0x02) != 0) ? digitalWriteFast(DB_1, HIGH) : digitalWriteFast(DB_1, LOW);
		((cl & 0x01) != 0) ? digitalWriteFast(DB_0, HIGH) : digitalWriteFast(DB_0, LOW);
#endif
		pulse_low(P_WR, B_WR);
		break;
	}
}

void UTFT::_set_direction_registers(byte mode)
{
    pinMode(DB_8, OUTPUT);
    pinMode(DB_9, OUTPUT);
    pinMode(DB_10, OUTPUT);
    pinMode(DB_11, OUTPUT);
    pinMode(DB_12, OUTPUT);
    pinMode(DB_13, OUTPUT);
    pinMode(DB_14, OUTPUT);
    pinMode(DB_15, OUTPUT);
    if (mode == 16)
    {
	pinMode(DB_0, OUTPUT);
	pinMode(DB_1, OUTPUT);
	pinMode(DB_2, OUTPUT);
	pinMode(DB_3, OUTPUT);
	pinMode(DB_4, OUTPUT);
	pinMode(DB_5, OUTPUT);
	pinMode(DB_6, OUTPUT);
	pinMode(DB_7, OUTPUT);
    }
}
void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
	long blocks;
#if (PORTS == USE_6_7_PORTS_16BIT)
		safe_write_16bit( (((uint16_t)ch) << 8) | ((uint16_t)cl) );
#elif (PORTS == USE_PORT_6_SUPERSPEED_16BIT)
		superspeed_write_16bit( (((uint16_t)ch) << 8) | ((uint16_t)cl) );
#else
		((ch & 0x80) != 0) ? digitalWriteFast(DB_15, HIGH) : digitalWriteFast(DB_15, LOW);
		((ch & 0x40) != 0) ? digitalWriteFast(DB_14, HIGH) : digitalWriteFast(DB_14, LOW);
		((ch & 0x20) != 0) ? digitalWriteFast(DB_13, HIGH) : digitalWriteFast(DB_13, LOW);
		((ch & 0x10) != 0) ? digitalWriteFast(DB_12, HIGH) : digitalWriteFast(DB_12, LOW);
		((ch & 0x08) != 0) ? digitalWriteFast(DB_11, HIGH) : digitalWriteFast(DB_11, LOW);
		((ch & 0x04) != 0) ? digitalWriteFast(DB_10, HIGH) : digitalWriteFast(DB_10, LOW);
		((ch & 0x02) != 0) ? digitalWriteFast(DB_9, HIGH) : digitalWriteFast(DB_9, LOW);
		((ch & 0x01) != 0) ? digitalWriteFast(DB_8, HIGH) : digitalWriteFast(DB_8, LOW);
		((cl & 0x80) != 0) ? digitalWriteFast(DB_7, HIGH) : digitalWriteFast(DB_7, LOW);
		((cl & 0x40) != 0) ? digitalWriteFast(DB_6, HIGH) : digitalWriteFast(DB_6, LOW);
		((cl & 0x20) != 0) ? digitalWriteFast(DB_5, HIGH) : digitalWriteFast(DB_5, LOW);
		((cl & 0x10) != 0) ? digitalWriteFast(DB_4, HIGH) : digitalWriteFast(DB_4, LOW);
		((cl & 0x08) != 0) ? digitalWriteFast(DB_3, HIGH) : digitalWriteFast(DB_3, LOW);
		((cl & 0x04) != 0) ? digitalWriteFast(DB_2, HIGH) : digitalWriteFast(DB_2, LOW);
		((cl & 0x02) != 0) ? digitalWriteFast(DB_1, HIGH) : digitalWriteFast(DB_1, LOW);
		((cl & 0x01) != 0) ? digitalWriteFast(DB_0, HIGH) : digitalWriteFast(DB_0, LOW);
#endif
	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);
		}
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;
#if (PORTS == USE_6_7_PORTS_8BIT)
		safe_write_8bit(ch);
#else
		((ch & 0x80) != 0) ? digitalWriteFast(DB_15, HIGH) : digitalWriteFast(DB_15, LOW);
		((ch & 0x40) != 0) ? digitalWriteFast(DB_14, HIGH) : digitalWriteFast(DB_14, LOW);
		((ch & 0x20) != 0) ? digitalWriteFast(DB_13, HIGH) : digitalWriteFast(DB_13, LOW);
		((ch & 0x10) != 0) ? digitalWriteFast(DB_12, HIGH) : digitalWriteFast(DB_12, LOW);
		((ch & 0x08) != 0) ? digitalWriteFast(DB_11, HIGH) : digitalWriteFast(DB_11, LOW);
		((ch & 0x04) != 0) ? digitalWriteFast(DB_10, HIGH) : digitalWriteFast(DB_10, LOW);
		((ch & 0x02) != 0) ? digitalWriteFast(DB_9, HIGH) : digitalWriteFast(DB_9, LOW);
		((ch & 0x01) != 0) ? digitalWriteFast(DB_8, HIGH) : digitalWriteFast(DB_8, LOW);
#endif

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}
