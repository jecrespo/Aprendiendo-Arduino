//************************************************************************
//*	Arduino Exploer
//*		(C) 2010 by Mark Sproul
//*		Open source as per standard Arduino code
//*
//*	This is done in the old fashion monitor style low level program
//*	it allows you to look at various aspects of the Arduino environment
//*	without writting special code. It can be used for debugging new hardware
//*
//*		#	=	Comm with Serial1, i.e. open a terminal window at 9600 board <esc><esc> to exit
//*		0	=	Zero address counters
//*		?	=	CPU Stats
//*		B	=	Blink LED
//*		E	=	Dump EEPROM
//*		F	=	Dump FLASH
//*		I	=	I2C scan
//*		M	=	Map I/O pins
//*		P	=	Show ONE pin
//*		R	=	Dump RAM
//*		S	=	Show pins
//*		T	=	Play Tone
//*		V	=	Show interrupt vectors
//*		W	=	Watch for input change
//*		Y	=	Port ouput, blink all 8 bits in a port by port letter
//*		Z	=	PWM to port, if the port does not support PWM, it will blink on and off
//*
//*	This has been tested on the following Arduino boards
//*		Arduino Duemilanove		__AVR_ATmega328P__
//*		Arduino LilyPad 		__AVR_ATmega328P__
//*		Arduino Mega			__AVR_ATmega1280__
//*		Liquidware Illuminato	__AVR_ATmega645__
//*		Liquidware TouchSlide	__AVR_ATmega2560__
//*		Duino644				__AVR_ATmega644__
//*		Sanguino				__AVR_ATmega644P__
//*		Teensy 2				__AVR_ATmega32U4__
//*		Teensy++ 2				__AVR_AT90USB1286__
//*		Wiring 1281				__AVR_ATmega1281__
//************************************************************************
//*	Mar 13,	2010	<MLS> Started on Arduino Explorer
//*	Mar 16,	2010	<MLS> Added map port command
//*	Mar 21,	2010	<MLS> Added Port output (blink entire port)
//*	Mar 22,	2010	<MLS> Added VECTOR command
//*	Mar 25,	2010	<MLS> Added memory map function (with help from John O)
//*	Mar 29,	2010	<MLS> Received the Teensy 2.0 and Teensy++ 2.0
//*	Mar 29,	2010	<MLS> Had to write my own pin# -> Port table
//*	Apr  2,	2010	<MLS> tested on LilyPad
//*	Apr 18,	2010	<MLS> Added fuse byte display
//*	Apr 23,	2010	<MLS> Added support for extended memory (Cerebot Plus)
//*	Jun  5,	2010	<MLS> Added support for the Wiring board (1281)
//*	Jun 21,	2010	<MLS> Added ShowOneAnalogPin
//*	Jul  3,	2010	<MLS> More work to get it working on a 168 (Memory reduction)
//************************************************************************

#ifdef __MWERKS__
	#include	"codewarrior.h"
	#include	<stdlib.h>
	char pgm_read_byte_far(unsigned long);
#else
	#include	<avr/pgmspace.h>
	#include	"EEPROM.h"
#endif

#include	<ctype.h>
#include	<avr/version.h>
#include	<avr/io.h>
#include	<avr/boot.h>

#include	<stdio.h>


#include	"Arduino.h"
#include	"HardwareSerial.h"
#include	"pins_arduino.h"
#include	"ToneNotes.h"

#include "Wire.h"
extern "C" { 
	#include "utility/twi.h"	// from Wire library, so we can do bus scanning
	//#include "twi.h"			// from Wire library, so we can do bus scanning
}



enum 
{
	kDUMP_FLASH	=	0,
	kDUMP_EEPROM,
	kDUMP_RAM
};


//************************************************************************
//		avr-objdump -d ArduinoExplorer.cpp.elf  > asm.txt
//		# create an empty file called foo.h
//		touch foo.h
//		# get a listing of all the predefined macros
//		avr-cpp -dM foo.h
//************************************************************************

//************************************************************************
#if defined(_BOARD_LIQUIDWARE_TOUCH_SLIDE_) || defined(_LIQUIDWARE_TOUCH_SLIDE_)
	#define	kCPU_NAME	"ATmega2560 (Liquidware touchslide)"

	#define	kRXD_pin				0
	#define	kTXD_pin				1

	#define	kLED_PIN				13
	#define	kLED_pin_First			13
	#define	kLED_pin_Last			15


	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		16

//	#define	kDigitalPin_FirstAnalog	9
//	#define	kAnalogPin_Count		0

//	#define	kAnalogPin_Last			9
	#define	_ENABLE_XTENDED_MEMORY_

//************************************************************************
#elif defined(_BOARD_WIRING_1281_)
	#define	kCPU_NAME	"ATmega1281 (Wiring-1281)"

	#define	kRXD_pin				32
	#define	kTXD_pin				33
	

	#define	kLED_PIN				48

	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		54

	#define	kDigitalPin_FirstAnalog	40

	#define	kAnalogPin_Count		8

//	#define	_ENABLE_XTENDED_MEMORY_

//************************************************************************
#elif defined(_BOARD_CEREBOT_PLUS_)
//	avrdude: Device signature = 0x1e9801
//	avrdude: safemode: hfuse reads as 99
//	avrdude: safemode: lfuse reads as EE
//	avrdude: safemode: efuse reads as FF

//	avrdude: Device signature = 0x1e9801
//	avrdude: safemode: lfuse reads as FF
//	avrdude: safemode: hfuse reads as DA
//	avrdude: safemode: efuse reads as FD
//	CPU Type         = Cerebot Plus ATmega2560
//	__AVR_ARCH__     = 6
//	AVR LibC Version = 1.6.4
//	CPU signature = 1E 98 01
//	Low fuse byte = FF
//	High fuse byte= DA
//	Lock fuse byte= FF
//	Ext  fuse byte= FD
//	RAM size      = 8 K
//	FLASH size    = 256 K
//	EEPROM size   = 4096 bytes
//	eXtended RAM  = 55 K

	#define	kCPU_NAME	"Cerebot Plus ATmega2560"

	#define	kRXD_pin				0
	#define	kTXD_pin				1
	

	#define	kLED_PIN				3
	#define	kLED_pin_First			2
	#define	kLED_pin_Last			3

	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		70

	#define	kDigitalPin_FirstAnalog	54

	#define	kAnalogPin_Count		16

	#define	_ENABLE_XTENDED_MEMORY_

//************************************************************************
#elif defined(__AVR_ATmega2560__)

	#define	kCPU_NAME	"ATmega2560 (Mega)"

	#define	kRXD_pin				0
	#define	kTXD_pin				1
	
	#define	kLED_PIN				13

	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		70

	#define	kDigitalPin_FirstAnalog	54

	#define	kAnalogPin_Count		16

//	#define	_ENABLE_XTENDED_MEMORY_

//************************************************************************
#elif defined(__AVR_ATmega1280__)
	#define	kCPU_NAME	"ATmega1280 (Mega)"

	#define	kRXD_pin				0
	#define	kTXD_pin				1
	
	#define	kLED_PIN				13

	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		70

	#define	kDigitalPin_FirstAnalog	54

	#define	kAnalogPin_Count		16

	#define	_ENABLE_XTENDED_MEMORY_

//#define	_SUPPORT_LIQUIDWARE_BUTTONPAD_
#ifdef _SUPPORT_LIQUIDWARE_BUTTONPAD_
	//*	Liquidware Button pad only works on the Mega
	#include "ButtonPad.h"
	ButtonPad	CButtonPad	=	ButtonPad();
#endif

//************************************************************************
#elif defined(_BOARD_LIQUIDWARE_ILLUMINATO_ ) || defined(__AVR_ATmega645__ )

	#define	kCPU_NAME	"ATmega645 (illuminato)"
	
	#define	kRXD_pin				0
	#define	kTXD_pin				1

//	#define	kLED_PIN				13
	#define	kLED_PIN				42

	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		43

	#define	kDigitalPin_FirstAnalog	36

	#define	kAnalogPin_Count		6


//************************************************************************
#elif defined(_BOARD_DUINO644_ )
		#define SPEAKER_PIN1      22  // PC6;
		#define SPEAKER_PIN2      23  // PC7;
	#ifdef __AVR_ATmega644__
		#define	kCPU_NAME			"ATmega644 (Duino644)"
	#endif
	#ifdef __AVR_ATmega644P__
		#define	kCPU_NAME			"ATmega644P (Duino644)"
	#endif
	
		#define	kRXD_pin				8
		#define	kTXD_pin				9

		#define	kDigitalPin_First		0
		#define	kDigitalPin_Count		32

		#define	kDigitalPin_FirstAnalog	24

		#define	kAnalogPin_Count		8


		#define _SUPPORT_RTC_

//************************************************************************
#elif defined(__AVR_ATmega644__ ) || defined(__AVR_ATmega644P__ )

	#ifdef __AVR_ATmega644__
		#define	kCPU_NAME			"ATmega644 (Sanguino)"
	#endif
	#ifdef __AVR_ATmega644P__
		#define	kCPU_NAME			"ATmega644P (Sanguino)"
	#endif
	
		#define	kLED_PIN				0
		#define	kRXD_pin				8
		#define	kTXD_pin				9

		#define	kDigitalPin_First		0
		#define	kDigitalPin_Count		32

		#define	kDigitalPin_FirstAnalog	24

		#define	kAnalogPin_Count		8


//************************************************************************
#elif defined(__AVR_ATmega32U4__)
	#define	kCPU_NAME				"ATmega32U4 (Teensy 2.0)"
	#define	kRXD_pin				7
	#define	kTXD_pin				8

	#define	kLED_PIN				11


	#define	kDigitalPin_First		0
	//*	total pins, digital and analog
	#define	kDigitalPin_Count		25

	#define	kDigitalPin_FirstAnalog	25
	#define	kAnalogPin_Count		12

//*	the TEENSY arduino does the pin mapping macros differently
//*	http://www.pjrc.com/teensy/pinout.html
prog_char	gPinToPortNameTable[]	PROGMEM	=	
{
	'B',	//	0	PB0
	'B',	//	1	PB1
	'B',	//	2	PB2
	'B',	//	3	PB3
	'B',	//	4	PB7
	'D',	//	5	PD0
	'D',	//	6	PD1
	'D',	//	7	PD2
	'D',	//	8	PD3
	'C',	//	9	PC6
	'C',	//	10	PC7
	'D',	//	11	PD6	A10
	'D',	//	12	PD7	A9
	'B',	//	13	PB4	A8
	'B',	//	14	PB5	A7
	'B',	//	15	PB6	A6
	'F',	//	16	PF7	A5
	'F',	//	17	PF6 A4
	'F',	//	18	PF5 A3
	'F',	//	19	PF4 A2
	'F',	//	20	PF1 A1
	'F',	//	21	PF0 A0
	'D',	//	22	PD4	A11
	'D',	//	23	PD5
	'E',	//	24	PE6
};


//************************************************************************
#elif defined(__AVR_AT90USB1286__)
	#define	kCPU_NAME	"AT90USB1286 (Teensy++ 2.0)"
	#define	kRXD_pin	2
	#define	kTXD_pin	3

	#define	kLED_PIN	6


	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		46



	#define	kDigitalPin_FirstAnalog	38
	#define	kAnalogPin_Count		8

//*	the TEENSY arduino does the pin mapping macros differently
//*	http://www.pjrc.com/teensy/pinout.html
prog_char	gPinToPortNameTable[]	PROGMEM	=	
{
	'D',	//	0
	'D',	//	1
	'D',	//	2
	'D',	//	3
	'D',	//	4
	'D',	//	5
	'D',	//	6
	'D',	//	7
	'E',	//	8
	'E',	//	9
	'C',	//	10
	'C',	//	11
	'C',	//	12
	'C',	//	13
	'C',	//	14
	'C',	//	15
	'C',	//	16
	'C',	//	17
	'E',	//	18
	'E',	//	19
	'B',	//	20
	'B',	//	21
	'B',	//	22
	'B',	//	23
	'B',	//	24
	'B',	//	25
	'B',	//	26
	'B',	//	27
	'A',	//	28
	'A',	//	29
	'A',	//	30
	'A',	//	31
	'A',	//	32
	'A',	//	33
	'A',	//	34
	'A',	//	35
	'E',	//	36
	'E',	//	37
	'F',	//	38	analog
	'F',	//	39	analog
	'F',	//	40	analog
	'F',	//	41	analog
	'F',	//	42	analog
	'F',	//	43	analog
	'F',	//	44	analog
	'F',	//	45	analog
};


//************************************************************************
#else
	//*	default for Arduino
	#define	kRXD_pin	0
	#define	kTXD_pin	1

	#define	kLED_PIN	13

	#define	kDigitalPin_First		0
	#define	kDigitalPin_Count		20

	#define	kDigitalPin_FirstAnalog	14

	#define	kAnalogPin_Count		6


#endif

//************************************************************************
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(USART3_RX_vect)
	#define	SERIAL_PORT_COUNT		4
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1281__) || defined(USART1_RX_vect)
	#define	SERIAL_PORT_COUNT		2
#else
	#define	SERIAL_PORT_COUNT		1
#endif


//************************************************************************
#if !defined(RAMSTART)
	#define RAMSTART     (0x100)
#endif

#ifdef __AVR_ATmega168__ 
	#define	kCPU_NAME	"ATmega168"
#endif

#ifdef __AVR_ATmega328P__
	#define	kCPU_NAME	"ATmega328P"
#endif



#ifdef _VECTORS_SIZE
	#define	kInterruptVectorCount (_VECTORS_SIZE / 4)
#else
	#define	kInterruptVectorCount 23
#endif





//************************************************************************
//*	cpus such as the 168 do not have enough memory to support some of this stuff
#if (RAMEND > 1000)
#endif
#if (RAMEND > 1500)
//	#define _SUPPORT_SD_CARD_
	#define _SUPPORT_I2C_TESTING_
	#define _SUPPORT_BUTTON_STATE_
#endif
#if (FLASHEND > 2000)
	#define	_DEBUG_INTERRUPT_NAMES_
	#include	"avrinterruptnames.h"
#endif

//************************************************************************
#ifdef _SUPPORT_SD_CARD_
	#include "SDuFAT.h"
#endif



//#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#if defined(__AVR_ATmega1280__) 
//	#include	"UnivGraphicsLib.h"
//	#include	"HersheyFonts.h"
//	#define	_SUPPORT_GRAPHICS_
#endif

#ifdef _ENABLE_XTENDED_MEMORY_
	static long	XtendedMemoryCheck(boolean displayMem);
#endif


//************************************************************************
//*	in an attempt to save RAM and use program space more effiently, printed strings are here
#ifdef kCPU_NAME
	prog_char	gTextMsg_CPU_Name[]			PROGMEM	=	kCPU_NAME;
#else
	prog_char	gTextMsg_CPU_Name[]			PROGMEM	=	"UNKNOWN";
#endif

#ifdef _INTERRUPT_NAMES_DEFINED_
	//													 123456789 123456789 123456789 123456789 123456789 
	prog_char	gTextMsg_VECTOR_HEADER[]	PROGMEM	=	"V#   ADDR   op code     inst      addr    Interrupt";
#else
	prog_char	gTextMsg_VECTOR_HEADER[]	PROGMEM	=	"V#   ADDR   op code     inst      addr";
#endif

	prog_char	gTextMsg_Explorer[]			PROGMEM	=	"Arduino explorer";
	prog_char	gTextMsg_Prompt[]			PROGMEM	=	"Arduino>";
	prog_char	gTextMsg_HUH[]				PROGMEM	=	"Huh?";


	prog_char	gTextMsg_CPU_Type[]			PROGMEM	=	"CPU Type         = ";
	prog_char	gTextMsg_AVR_ARCH[]			PROGMEM	=	"__AVR_ARCH__     = ";

	prog_char	gTextMsg_GCC_VERSION[]		PROGMEM	=	"GCC Version      = ";
	prog_char	gTextMsg_GCC_VERSION_STR[]	PROGMEM	=	__VERSION__;
	prog_char	gTextMsg_CPU_SIGNATURE[]	PROGMEM	=	"CPU signature = ";




	prog_char	gTextMsg_AVR_LIBC[]			PROGMEM	=	"AVR LibC Version = ";
	prog_char	gTextMsg_AVR_LIBC_VER_STR[]	PROGMEM	=	__AVR_LIBC_VERSION_STRING__;

	prog_char	gTextMsg_DIGITALPIN[]		PROGMEM	=	"Digital Pin # ";
	prog_char	gTextMsg_ANALOGPIN[]		PROGMEM	=	"Analog Pin # ";
	prog_char	gTextMsg_IS_NOW[]			PROGMEM	=	" is now ";
		
	prog_char	gTextMsg_SPACE[]			PROGMEM	=	" ";
	prog_char	gTextMsg_K[]				PROGMEM	=	" K";
	prog_char	gTextMsg_bytes[]			PROGMEM	=	" bytes";


	prog_char	gTextMsg_WHAT_PIN[]			PROGMEM	=	"what pin #:";
	prog_char	gTextMsg_NOT_PWM[]			PROGMEM	=	"Pin does not support PWM, it will just blink";
	
	prog_char	gTextMsg_WHAT_PORT[]		PROGMEM	=	"what port:";
	prog_char	gTextMsg_PIN_OUT_OF_RANGE[]	PROGMEM	=	"Pin number out of range";
	prog_char	gTextMsg_DIGITAL_PIN[]		PROGMEM	=	"Digital pin ";


	prog_char	gTextMsg_RX_DATA[]			PROGMEM	=	"RX Data";
	prog_char	gTextMsg_TX_DATA[]			PROGMEM	=	"TX Data";
	prog_char	gTextMsg_AnalogIN[]			PROGMEM	=	"Analog Input ";
	prog_char	gTextMsg_ON_BOARD_LED[]		PROGMEM	=	"On board LED";
	prog_char	gTextMsg_SERIAL_PIN[]		PROGMEM	=	"That was a Serial pin";



	prog_char	gTextMsg_I2C_SCAN[]			PROGMEM	=	"starting scanning of I2C bus from ";
	prog_char	gTextMsg_TO[]				PROGMEM	=	" to ";
	prog_char	gTextMsg_DotDotDot[]		PROGMEM	=	"...";

	prog_char	gTextMsg_I2C_DevCnt[]		PROGMEM	=	"I2C device count = ";

#if defined(kLED_pin_First) && defined(kLED_pin_Last)
	prog_char	gTextMsg_MULTIPLE_LED[]		PROGMEM	=	"Multiple LED display";
#endif
	prog_char	gTextMsg_BlinkLEDpin[]		PROGMEM	=	"Blink LED default pin# = ";
	prog_char	gTextMsg_PortNotSupported[]	PROGMEM	=	"Port not supported";
	prog_char	gTextMsg_MustBeLetter[]		PROGMEM	=	"Must be a letter";
	prog_char	gTextMsg_MORE[]				PROGMEM	=	"--MORE--";
	prog_char	gTextMsg_Available_RAM[]	PROGMEM	=	"available RAM=";
	prog_char	gTextMsg_size_of[]			PROGMEM	=	"size of ";
#if (RAMEND > 1000)
	prog_char	gTextMsg_RAM_SIZE[]			PROGMEM	=	"RAM size      = ";
	prog_char	gTextMsg_FLASH_SIZE[]		PROGMEM	=	"FLASH size    = ";
	prog_char	gTextMsg_EEPROM_SIZE[]		PROGMEM	=	"EEPROM size   = ";
#endif
	prog_char	gTextMsg_UPTIME[]			PROGMEM	=	"Up time       = ";

#if defined(__AVR_ATmega645__)
	prog_char	gTextMsg_I2C_NOT_SUPPORT[]	PROGMEM	=	"This CPU does not support I2C (TWI) ";
#endif
#ifdef _ENABLE_XTENDED_MEMORY_
	prog_char	gTextMsg_EXTENDED_RAM[]		PROGMEM	=	"eXtended RAM  = ";
#endif


//************************************************************************
//*	Help messages
	prog_char	gTextMsg_HELP_MSG_EP[]		PROGMEM	=	"!=Blink all LEDs";
	prog_char	gTextMsg_HELP_MSG_0[]		PROGMEM	=	"0=Zero address counters";
#if (SERIAL_PORT_COUNT > 1)
	prog_char	gTextMsg_HELP_MSG_HASH[]	PROGMEM	=	"#=Open serial1 as terminal";
	prog_char	gTextMsg_OpenSerialPort[]	PROGMEM	=	"Opening Serial Port 1 @ 9600 (<esc><esc> to exit)";
#endif
	prog_char	gTextMsg_HELP_MSG_QM[]		PROGMEM	=	"?=CPU stats";
#ifdef kAnalogPin_Count
	prog_char	gTextMsg_HELP_MSG_A[]		PROGMEM	=	"A=Show Analog Pin";
#endif
	prog_char	gTextMsg_HELP_MSG_B[]		PROGMEM	=	"B=Blink LED";
#ifdef _SUPPORT_RTC_
	prog_char	gTextMsg_HELP_MSG_C[]		PROGMEM	=	"C=Clock Functions";
#endif
#ifdef _SUPPORT_SD_CARD_
	prog_char	gTextMsg_HELP_MSG_D[]		PROGMEM	=	"D=Show 1st sector of SD card";
#endif
	prog_char	gTextMsg_HELP_MSG_E[]		PROGMEM	=	"E=Dump EEPROM";
	prog_char	gTextMsg_HELP_MSG_F[]		PROGMEM	=	"F=Dump FLASH";
#ifdef _SUPPORT_GRAPHICS_
	prog_char	gTextMsg_HELP_MSG_G[]		PROGMEM	=	"G=Graphics Card Test";
#endif
	prog_char	gTextMsg_HELP_MSG_H[]		PROGMEM	=	"H=Help";
#ifdef _SUPPORT_I2C_TESTING_
	prog_char	gTextMsg_HELP_MSG_I[]		PROGMEM	=	"I=I2C scan";
	prog_char	gTextMsg_HELP_MSG_J[]		PROGMEM	=	"J=Send data to I2C device";
#endif
	prog_char	gTextMsg_HELP_MSG_M[]		PROGMEM	=	"M=Map I/O pin assignments";
	prog_char	gTextMsg_HELP_MSG_P[]		PROGMEM	=	"P=Show one digital pin";
	prog_char	gTextMsg_HELP_MSG_R[]		PROGMEM	=	"R=Dump RAM";
	prog_char	gTextMsg_HELP_MSG_S[]		PROGMEM	=	"S=Show pins";
	prog_char	gTextMsg_HELP_MSG_T[]		PROGMEM	=	"T=Play Tone";
	prog_char	gTextMsg_HELP_MSG_V[]		PROGMEM	=	"V=show interrupt Vectors";
#ifdef _SUPPORT_BUTTON_STATE_
	prog_char	gTextMsg_HELP_MSG_W[]		PROGMEM	=	"W=Watch for change on all pins";
#endif
#ifdef _ENABLE_XTENDED_MEMORY_
	prog_char	gTextMsg_HELP_MSG_X[]		PROGMEM	=	"X=Xtended Memory Test";
#endif
	prog_char	gTextMsg_HELP_MSG_Y[]		PROGMEM	=	"Y=Port blink";
	prog_char	gTextMsg_HELP_MSG_Z[]		PROGMEM	=	"Z=Set PWM Pin";



unsigned char	*gRamPtr;
unsigned long	gRamIndex;
unsigned long	gFlashIndex;
uint8_t			gPageLineCount;

#ifdef _SUPPORT_BUTTON_STATE_
	int				gButtonState[kDigitalPin_Count + 1];
#endif
#ifdef _SUPPORT_I2C_TESTING_
	unsigned char	gI2CfoundAddress;
	uint8_t			gI2CtestCounter;
	boolean			gI2CneedsInit;
#endif
#ifdef _SUPPORT_RTC_
	#include "DS1307.h"
	int		gHour;
	int		gMinute;
	int		gYear;
	int		gMonth;
	int		gDay;
	int		gDow;
#endif

//************************************************************************
void	PrintFromPROGMEM(void *dataPtr, byte offset=0)
{
uint8_t	ii;
char	theChar;

	ii			=	offset;
	theChar		=	1;
	
	while (theChar != 0)
	{
		theChar	=	pgm_read_byte_near((long)dataPtr + ii);
		if (theChar != 0)
		{
			Serial.print(theChar);
		}
		ii++;
	}
}

//************************************************************************
void	PrintFromPROGMEMln(void *dataPtr, byte offset=0)
{
	PrintFromPROGMEM(dataPtr, offset);

	Serial.println();
}





#pragma mark -
#pragma mark Memory Functions

//************************************************************************
static int		Get_free_memory()
{
extern char	__bss_end;
extern char	*__brkval;
int			free_memory;

	if ((int)__brkval == 0)
	{
		free_memory	=	((int)&free_memory) - ((int)&__bss_end);
	}
	else
	{
		free_memory	=	((int)&free_memory) - ((int)__brkval);
	}
	
	return (free_memory);
}

//************************************************************************
//*	http://www.nongnu.org/avr-libc/user-manual/malloc.html
//*	thanks to John O for the pointer to this info and the insperation to do it
static void	Ram_TableDisplay(void) 
{
char stack = 1;
extern char *__data_start;
extern char *__data_end;

extern char *__bss_start;
extern char *__bss_end;
//extern char *__brkval;
extern char *__heap_start;
extern char *__heap_end;
//extern char *__malloc_heap_end;
extern size_t __malloc_margin;


	int	data_size	=	(int)&__data_end - (int)&__data_start;
	int	bss_size	=	(int)&__bss_end - (int)&__data_end;
	int	heap_end	=	(int)&stack - (int)&__malloc_margin;
//	int	heap_size	=	(int)__brkval - (int)&__bss_end;
	int	heap_size	=	heap_end - (int)&__bss_end;
	int	stack_size	=	RAMEND - (int)&stack + 1;
	int	available	=	(RAMEND - (int)&__data_start + 1);
	
	available	-=	data_size + bss_size + heap_size + stack_size;

#if (RAMEND > 4000)
	Serial.print("+----------------+  __data_start  =");	Serial.println((int)&__data_start);
	Serial.print("+      data      +");						Serial.println();
	Serial.print("+    variables   +  data_size     =");	Serial.println(data_size);
	Serial.print("+                +");						Serial.println();
	Serial.print("+----------------+  __data_end    =");	Serial.println((int)&__data_end);
	Serial.print("+----------------+  __bss_start   =");	Serial.println((int)&__bss_start);
	Serial.print("+       bss      +");						Serial.println();
	Serial.print("+    variables   +  bss_size      =");	Serial.println(bss_size);
	Serial.print("+                +");						Serial.println();
	Serial.print("+----------------+  __bss_end     =");	Serial.println((int)&__bss_end);
	Serial.print("+----------------+  __heap_start  =");	Serial.println((int)&__heap_start);
	Serial.print("+                +");						Serial.println();
	Serial.print("+       heap     +  heap_size     =");	Serial.println(heap_size);
	Serial.print("+                +");						Serial.println();
	Serial.print("+----------------+  heap_end      =");	Serial.println(heap_end);
	Serial.print("+----------------+  Current STACK =");	Serial.println((int)&stack);
	Serial.print("+                +");						Serial.println();
	Serial.print("+      stack     +  stack_size    =");	Serial.println(stack_size);
	Serial.print("+                +");						Serial.println();
	Serial.print("+----------------+  RAMEND        =");	Serial.println(RAMEND);
#endif

//	Serial.print("__brkval      =");
//	Serial.println((int)__brkval);

	PrintFromPROGMEM(gTextMsg_Available_RAM);
	Serial.println(available);

	Serial.println();
	Serial.println();
}


#pragma mark -

#define	RAMEND_LIMIT	2000
//************************************************************************
static void	PrintCPUstats()
{
unsigned long	memorySize;
unsigned long	upSeconds;
byte			fuseByte;
//#if  (RAMEND > RAMEND_LIMIT)
	char		textString[32];
//#endif

	PrintFromPROGMEM(gTextMsg_CPU_Type);
	PrintFromPROGMEMln(gTextMsg_CPU_Name);


	PrintFromPROGMEM(gTextMsg_AVR_ARCH);
	Serial.println(__AVR_ARCH__);
	
	//*	these can be found in avr/version.h
	PrintFromPROGMEM(gTextMsg_AVR_LIBC);
	PrintFromPROGMEMln(gTextMsg_AVR_LIBC_VER_STR);

	PrintFromPROGMEM(gTextMsg_GCC_VERSION);
	PrintFromPROGMEMln(gTextMsg_GCC_VERSION_STR);

#if defined(SIGNATURE_0)
	PrintFromPROGMEM(gTextMsg_CPU_SIGNATURE);
	//*	these can be found in avr/iomxxx.h
	sprintf(textString, "%02X %02X %02X", SIGNATURE_0, SIGNATURE_1, SIGNATURE_2);
	Serial.println(textString);
#endif

#if !defined(__MWERKS__) && defined(GET_LOW_FUSE_BITS) &&  (RAMEND > RAMEND_LIMIT)
	//*	fuse settings
	fuseByte	=	boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
	sprintf(textString, "Low fuse byte = %02X", fuseByte);
	Serial.println(textString);

	fuseByte	=	boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	sprintf(textString, "High fuse byte= %02X", fuseByte);
	Serial.println(textString);

	fuseByte	=	boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
	sprintf(textString, "Ext  fuse byte= %02X", fuseByte);
	Serial.println(textString);

	fuseByte	=	boot_lock_fuse_bits_get(GET_LOCK_BITS);
	sprintf(textString, "Lock fuse byte= %02X", fuseByte);
	Serial.println(textString);

#endif


#if defined(RAMEND) && (RAMEND > 1000)
	//*	these can be found in avr/iomxxx.h
	memorySize	=	RAMEND - RAMSTART + 1;
	PrintFromPROGMEM(gTextMsg_RAM_SIZE);
	Serial.print(memorySize / 1024);
	PrintFromPROGMEMln(gTextMsg_K);
#endif


#if defined(FLASHEND) && (RAMEND > 1000)
	memorySize	=	(long)FLASHEND;
	memorySize	+=	1;
	PrintFromPROGMEM(gTextMsg_FLASH_SIZE);
	Serial.print(memorySize / 1024);
	PrintFromPROGMEMln(gTextMsg_K);
#endif

#if defined(E2END) && (RAMEND > 1000)
	PrintFromPROGMEM(gTextMsg_EEPROM_SIZE);
	Serial.print(E2END + 1);
	PrintFromPROGMEMln(gTextMsg_bytes);
#endif

#ifdef _ENABLE_XTENDED_MEMORY_
	XtendedMemoryCheck(true);	//*	true means print results
#endif

#if  (RAMEND > RAMEND_LIMIT)
	PrintFromPROGMEM(gTextMsg_size_of);
	Serial.print("char  = ");
	Serial.println(sizeof(char));
	
	PrintFromPROGMEM(gTextMsg_size_of);
	Serial.print("int   = ");
	Serial.println(sizeof(int));
	
	PrintFromPROGMEM(gTextMsg_size_of);
	Serial.print("long  = ");
	Serial.println(sizeof(long));

	PrintFromPROGMEM(gTextMsg_size_of);
	Serial.print("float = ");
	Serial.println(sizeof(float));
	
	PrintFromPROGMEM(gTextMsg_size_of);
	Serial.print("double= ");
	Serial.println(sizeof(double));
	
	PrintFromPROGMEM(gTextMsg_size_of);
	Serial.print("void *= ");
	Serial.println(sizeof(void *));
	
#endif
	upSeconds	=	millis() / 1000;
	PrintFromPROGMEM(gTextMsg_UPTIME);
//	Serial.println(upSeconds);
	sprintf(textString, "%02ld:%02ld:%02ld", (upSeconds / 3600), ((upSeconds / 60) % 60), (upSeconds % 60));
	Serial.println(textString);

	Serial.println();


	
	Ram_TableDisplay();
}


//************************************************************************
static void	DumpHex(byte dumpWhat, unsigned long startAddress, unsigned char numRows)
{
unsigned long	myAddressPointer;
uint8_t			ii;
unsigned char	theValue;
char			textString[10];
char			asciiDump[18];

	myAddressPointer	=	startAddress;
	while (numRows > 0)
	{
		sprintf(textString, "%04lX - ", myAddressPointer);
		Serial.print(textString);
		
		asciiDump[0]		=	0;
		for (ii=0; ii<16; ii++)
		{
			switch(dumpWhat)
			{
				case kDUMP_FLASH:
				#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || (FLASHEND > 0x10000)
					theValue	=	pgm_read_byte_far(myAddressPointer);
				#else
					theValue	=	pgm_read_byte_near(myAddressPointer);
				#endif
					break;

				case kDUMP_EEPROM:
				#ifndef __MWERKS__
					theValue	=	EEPROM.read(myAddressPointer);
				#endif
					break;

				case kDUMP_RAM:
					theValue	=	gRamPtr[myAddressPointer];
					break;

			}

			sprintf(textString, "%02X ", theValue);
			Serial.print(textString);
			if ((theValue >= 0x20) && (theValue < 0x7f))
			{
				asciiDump[ii % 16]	=	theValue;
			}
			else
			{
				asciiDump[ii % 16]	=	'.';
			}
			
			myAddressPointer++;
		}
		asciiDump[16]	=	0;
		Serial.println(asciiDump);
	
		numRows--;
	}
}


//*******************************************************************
static void ShowPins(void)
{
short	ii;
int		pinValue;

	for (ii = kDigitalPin_First; ii < kDigitalPin_Count; ii++)
	{
		PrintFromPROGMEM(gTextMsg_DIGITALPIN);
		Serial.print(ii);
		PrintFromPROGMEM(gTextMsg_SPACE);
		pinValue	=	digitalRead(ii);
		Serial.print(pinValue);
		PrintFromPROGMEM(gTextMsg_SPACE);

		if (ii == kRXD_pin)
		{
			PrintFromPROGMEM(gTextMsg_RX_DATA);
		}
		if (ii == kTXD_pin)
		{
			PrintFromPROGMEM(gTextMsg_TX_DATA);
		}

		Serial.println();
	}
	Serial.println();


#ifdef kAnalogPin_Count
	for (ii = 0; ii < kAnalogPin_Count; ii++)
	{
		pinValue	=	analogRead(ii);
		PrintFromPROGMEM(gTextMsg_ANALOGPIN);
		Serial.print(ii);
		PrintFromPROGMEM(gTextMsg_SPACE);
		Serial.print(pinValue);
		Serial.println();
	}
	Serial.println();
#else
	Serial.println("No analog pins defined");
#endif
}

//*******************************************************************
static void PrintHelp(void)
{
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_EP);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_0);
#if (SERIAL_PORT_COUNT > 1)
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_HASH);
#endif
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_QM);
#ifdef kAnalogPin_Count
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_A);
#endif
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_B);
#ifdef _SUPPORT_RTC_
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_C);
#endif
#ifdef _SUPPORT_SD_CARD_
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_D);
#endif
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_E);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_F);
#ifdef _SUPPORT_GRAPHICS_
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_G);
#endif
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_H);
#ifdef _SUPPORT_I2C_TESTING_
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_I);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_J);
#endif
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_M);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_P);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_R);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_S);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_T);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_V);
#ifdef _SUPPORT_BUTTON_STATE_
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_W);
#endif
#ifdef _ENABLE_XTENDED_MEMORY_
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_X);
#endif
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_Y);
	PrintFromPROGMEMln(gTextMsg_HELP_MSG_Z);
}


#pragma mark -
#pragma mark I2C routines
#ifdef _SUPPORT_I2C_TESTING_

//****************************************************************************
//*	Scan the I2C bus between addresses from_addr and to_addr.
//*	On each address, call the callback function with the address and result.
//*	If result==0, address was found, otherwise, address wasn't found
//*	(can use result to potentially get other status on the I2C bus, see twi.c)
//*	Assumes Wire.begin() has already been called
//*	2009, Tod E. Kurt, http://todbot.com/blog/
//****************************************************************************
static void ScanI2CBus(byte from_addr, byte to_addr) 
{
#if defined(__AVR_ATmega645__)
	PrintFromPROGMEM(gTextMsg_I2C_NOT_SUPPORT);
	PrintFromPROGMEMln(gTextMsg_CPU_Name);
#else

byte	addr;
byte	rc;
byte	data; // not used, just an address to feed to twi_writeTo()
int		foundCount;

	PrintFromPROGMEM(gTextMsg_I2C_SCAN);
	Serial.print(from_addr, DEC);
	PrintFromPROGMEM(gTextMsg_TO);
	Serial.print(to_addr, DEC);
	PrintFromPROGMEMln(gTextMsg_DotDotDot);

	data		=	0;
	foundCount	=	0;
	for(addr = from_addr; addr <= to_addr; addr++ )
	{
		Serial.print("addr:");
		Serial.print(addr,DEC);
		rc	=	twi_writeTo(addr, &data, 0, 1);
		if (rc == 0)
		{
			gI2CfoundAddress	=	addr;
			foundCount++;
		}
		Serial.print( (rc == 0) ? " found!":"       ");
		Serial.print( (addr % 4) ? "\t":"\r\n");
	}
	Serial.println();

	PrintFromPROGMEM(gTextMsg_I2C_DevCnt);
	Serial.println(foundCount);
#endif
}

//****************************************************************************
static void SendDataToI2Cdevice(void)
{
#if defined(__AVR_ATmega645__)
	PrintFromPROGMEM(gTextMsg_I2C_NOT_SUPPORT);
	PrintFromPROGMEMln(gTextMsg_CPU_Name);
#else
char	tempString[16];

	//*	join the I2C bus as the master
	if (gI2CneedsInit)
	{
		Wire.begin();
		gI2CneedsInit	=	false;
	}
	Wire.beginTransmission(gI2CfoundAddress);
	Wire.send("I2C test data ");
	sprintf(tempString, "%d\r", gI2CtestCounter);
	Wire.send(tempString);
	Wire.endTransmission();
	
	gI2CtestCounter++;
#endif
}
#endif

//************************************************************************
static char	GetChar(void)
{
char	theChar;

	//*	wait until there is a char
	while (!Serial.available())
	{
		//*	do nothing
	}
	theChar	=	Serial.read();
	theChar	=	toupper(theChar);
	return(theChar);
}

//************************************************************************
static void	GetInputTextString(char *inputTextString, unsigned char maxLen)
{
boolean			keepGoing;
char			theChar;
unsigned char	cc;

	keepGoing	=	true;
	cc			=	0;
	while (keepGoing && (cc < maxLen))
	{
		theChar	=	GetChar();
		Serial.print(theChar, BYTE);
		if (theChar == 0x08)	//*	backspace
		{
			cc--;
			if (cc < 0)
			{
				cc	=	0;
			}
		}
		else if (theChar >= 0x20)
		{
			inputTextString[cc++]	=	theChar;
		}
		else
		{
			inputTextString[cc]	=	0;
			keepGoing			=	false;
		}
	}
}

//************************************************************************
static void	ShowOneDigitalPin()
{
char	inputTextString[16];
int		pinNumber;
int		pinValue;

	PrintFromPROGMEM(gTextMsg_WHAT_PIN);
	GetInputTextString(inputTextString, 8);
	Serial.println();
	
	pinNumber	=	atoi(inputTextString);
	
	if ((pinNumber >= 0) && (pinNumber < kDigitalPin_Count))
	{
		pinMode(pinNumber, INPUT);
		digitalWrite(pinNumber, HIGH);	//*	turn on pull up resister

		while (!Serial.available())
		{
			pinValue	=	digitalRead(pinNumber);
			PrintFromPROGMEM(gTextMsg_DIGITALPIN);
			Serial.print(pinNumber);
			PrintFromPROGMEM(gTextMsg_SPACE);
			Serial.print(pinValue);
			Serial.print(0x0d, BYTE);
		}
		Serial.read();	//*	throw away the char
		Serial.println();
	}
	else
	{
		PrintFromPROGMEMln(gTextMsg_PIN_OUT_OF_RANGE);
	}
}

#ifdef kAnalogPin_Count
//************************************************************************
static void	ShowOneAnalogPin()
{
char	inputTextString[16];
int		pinNumber;
int		analogValue;

	PrintFromPROGMEM(gTextMsg_WHAT_PIN);
	GetInputTextString(inputTextString, 8);
	Serial.println();
	
	pinNumber	=	atoi(inputTextString);
	
	if (pinNumber < kAnalogPin_Count)
	{
		while (!Serial.available())
		{
			PrintFromPROGMEM(gTextMsg_ANALOGPIN);
			Serial.print(pinNumber);
			PrintFromPROGMEM(gTextMsg_SPACE);

			analogValue	=	analogRead(pinNumber);
			Serial.print(analogValue);
			Serial.print("   ");	//*	this is to make sure it wipes out the trailing data of the previous value
			Serial.print(0x0d, BYTE);
			delay(25);
		}
		Serial.read();	//*	throw away the char
	}
	else
	{
		//*	out of range
	//	PrintFromPROGMEM(gTextMsg_ANALOGPIN);
	}
	Serial.println();
}
#endif

//************************************************************************
static void	SetPWMpin()
{
#if defined(kAnalogPin_Count)
char	inputTextString[16];
int		pinNumber;
int		pwmValue;

	PrintFromPROGMEM(gTextMsg_WHAT_PIN);
	GetInputTextString(inputTextString, 8);
	Serial.println();
	
	if ((strlen(inputTextString) > 0) && isdigit(inputTextString[0]))
	{
		pinNumber	=	atoi(inputTextString);
		
		if (pinNumber < kDigitalPin_Count)
		{
			if ((pinNumber != kRXD_pin) && (pinNumber != kTXD_pin))
			{
			//	Serial.println();
			//	Serial.print(pinNumber);
			//	PrintFromPROGMEM(gTextMsg_SPACE);
			//	Serial.println((int)digitalPinToTimer(pinNumber));

				if (digitalPinToTimer(pinNumber) == NOT_ON_TIMER)
				{
					PrintFromPROGMEMln(gTextMsg_NOT_PWM);
				}
					
				pwmValue	=	128;
				while (!Serial.available())
				{
					analogWrite(pinNumber, pwmValue);
					delay(100);
					pwmValue	+=	10;
					if (pwmValue >= 255)
					{
						pwmValue	=	0;
					}
				}
				Serial.read();	//*	throw away the char
				analogWrite(pinNumber, 0);
				digitalWrite(pinNumber, 0);

			}
			else
			{
				PrintFromPROGMEMln(gTextMsg_SERIAL_PIN);
			}
		}
		else
		{
			PrintFromPROGMEMln(gTextMsg_PIN_OUT_OF_RANGE);
		}
	}
	else
	{
		PrintFromPROGMEMln(gTextMsg_HUH);
	}
#else
	Serial.println("No analog pins defined");
#endif
}




//************************************************************************
static boolean	PrintNewLine_MORE(void)
{
char	theChar;
boolean	keepGoing;

	Serial.println();
	gPageLineCount++;
	keepGoing	=	true;
	
	if (gPageLineCount > 25)
	{
		PrintFromPROGMEM(gTextMsg_MORE);
		theChar	=	GetChar();
		if ((theChar & 0x5f) == 'Q')
		{
			keepGoing	=	false;
		}
		Serial.print(0x0d, BYTE);

		gPageLineCount	=	0;
	}
	
	return(keepGoing);
}

#ifndef NOT_ON_TIMER
	#define	NOT_ON_TIMER 0	
#endif

//************************************************************************
static void MapPins()
{
uint8_t	ii;
uint8_t	portNum;
int		pinMask;
int		pinNum;
int		timerNum;
char	textString[20];
char	portLetter;
boolean	keepGoing;

	PrintFromPROGMEM(gTextMsg_CPU_Type);
	PrintFromPROGMEMln(gTextMsg_CPU_Name);

	for (ii = 0; ii< kDigitalPin_Count; ii++)
	{

	#if defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	
		//*	the TEENSY arduino does the pin mapping macros differently
		//*	so I had to create my own table
		portLetter	=	pgm_read_byte_near(gPinToPortNameTable + ii);
		timerNum	=	0;
	#else
		portNum		=	digitalPinToPort(ii);
		portLetter	=	portNum + 0x40;
		timerNum	=	digitalPinToTimer(ii);
	#endif

		pinMask		=	digitalPinToBitMask(ii);



		//*	figure out the pin number from the mask
		pinNum	=	0;
		while (pinMask != 0)
		{
			pinNum++;
			pinMask	=	pinMask >> 1;
		}
		pinNum--;

		PrintFromPROGMEM(gTextMsg_DIGITAL_PIN);
		sprintf(textString, "%2d = %c %2X  ", ii, portLetter, pinNum);
		Serial.print(textString);
		if (timerNum != NOT_ON_TIMER)
		{
			sprintf(textString, "Timer %2d ", timerNum);
			Serial.print(textString);
		}
		if (ii == kRXD_pin)
		{
			PrintFromPROGMEM(gTextMsg_RX_DATA);
		}
		if (ii == kTXD_pin)
		{
			PrintFromPROGMEM(gTextMsg_TX_DATA);
		}

	#if defined(kLED_pin_First) && defined(kLED_pin_Last)
		if ((ii >= kLED_pin_First) && (ii <= kLED_pin_Last))
		{
			PrintFromPROGMEM(gTextMsg_ON_BOARD_LED);
		}
	#endif
	#ifdef kLED_PIN
		if (ii == kLED_PIN)
		{
			PrintFromPROGMEM(gTextMsg_ON_BOARD_LED);
		}
	#endif
	
	


	#ifdef kDigitalPin_FirstAnalog
		if ((ii >= kDigitalPin_FirstAnalog) && (ii < (kDigitalPin_FirstAnalog + kAnalogPin_Count)))
		{
			PrintFromPROGMEM(gTextMsg_AnalogIN);
			Serial.print(ii - kDigitalPin_FirstAnalog);
		}
	#endif
		keepGoing	=	PrintNewLine_MORE();
		if (keepGoing == false)
		{
			break;
		}
	}
}


//************************************************************************
static void	PlayTone()
{
#if (defined(SPEAKER_PIN1) && defined(SPEAKER_PIN2))
int	notes[]		=	{NOTE_D4, NOTE_E4, NOTE_C4, NOTE_C3, NOTE_G3};
int durations[]	=	{500, 500, 500, 500, 1000};

	pinMode(SPEAKER_PIN1, OUTPUT);
	pinMode(SPEAKER_PIN2, OUTPUT);
	digitalWrite(SPEAKER_PIN1, LOW);
	tone(SPEAKER_PIN2, 600, 500);
	for (int thisNote = 0; thisNote < 5; thisNote ++)
	{
  		tone(SPEAKER_PIN2, notes[thisNote], durations[thisNote]);
  		delay(durations[thisNote]);
	}
#else
char	inputTextString[8];
int		pinNumber;

	PrintFromPROGMEM(gTextMsg_WHAT_PIN);
	GetInputTextString(inputTextString, 4);
	Serial.println();
	
	if ((strlen(inputTextString) > 0) && isdigit(inputTextString[0]))
	{
		pinNumber	=	atoi(inputTextString);
		
		if ((pinNumber != kRXD_pin) && (pinNumber != kTXD_pin))
		{
			Serial.println(pinNumber);
			pinMode(pinNumber, OUTPUT);
			while (!Serial.available())
			{
				digitalWrite(pinNumber, HIGH);
				delayMicroseconds(500);
				digitalWrite(pinNumber, LOW);
				delayMicroseconds(500);
			}
			Serial.read();	//*	throw away the char
		}
		else
		{
			PrintFromPROGMEM(gTextMsg_SERIAL_PIN);
		}
	}

#endif

}


//************************************************************************
static void BlinkAllLEDS()
{
uint8_t	ii;
boolean	keepGoing;

	for (ii = kDigitalPin_First; ii < kDigitalPin_Count; ii++)
	{
		if ((ii != kRXD_pin) && (ii != kTXD_pin))
		{
			pinMode(ii, OUTPUT);
			digitalWrite(ii, LOW);
		}
	}

	keepGoing	=	true;
	while (keepGoing)
	{
		for (ii = kDigitalPin_First; ii < kDigitalPin_Count; ii++)
		{
			if ((ii != kRXD_pin) && (ii != kTXD_pin))
			{
				digitalWrite(ii, HIGH);
				delay(75);
				digitalWrite(ii, LOW);
			}
		}

		if (Serial.available())
		{
			keepGoing	=	false;
		}
	}
}

//************************************************************************
static void BlinkLED()
{
#if defined(kLED_pin_First) && defined(kLED_pin_Last)
uint8_t	ii;

	PrintFromPROGMEMln(gTextMsg_MULTIPLE_LED);
	PrintFromPROGMEM(gTextMsg_BlinkLEDpin);
	Serial.print(kLED_pin_First);

	for (ii=kLED_pin_First; ii<=kLED_pin_Last; ii++)
	{
		pinMode(ii, OUTPUT);
	}

	while (!Serial.available())
	{
		for (ii=kLED_pin_First; ii<=kLED_pin_Last; ii++)
		{
			digitalWrite(ii, LOW);
			delay(200);
			digitalWrite(ii, HIGH);
			delay(200);
		}
		delay(500);

	}
	Serial.read();	//*	throw away the char
	Serial.println();


#else
char	inputTextString[8];
int		pinNumber;

#ifdef kLED_PIN
	pinNumber	=	kLED_PIN;
#else
	pinNumber	=	2;
#endif

	PrintFromPROGMEM(gTextMsg_BlinkLEDpin);
	Serial.print(pinNumber);
	Serial.println();
	
	PrintFromPROGMEM(gTextMsg_WHAT_PIN);
	GetInputTextString(inputTextString, 4);
	Serial.println();
	
	
	if ((strlen(inputTextString) > 0) && isdigit(inputTextString[0]))
	{
		pinNumber	=	atoi(inputTextString);
	}
	
	if (pinNumber < kDigitalPin_Count)
	{
		pinMode(pinNumber, OUTPUT);
		while (!Serial.available())
		{
			digitalWrite(pinNumber, HIGH);
			delay(200);
			digitalWrite(pinNumber, LOW);
			delay(200);
		}
		Serial.read();	//*	throw away the char
		Serial.println();
		pinMode(pinNumber, INPUT);
	}
	else
	{
		PrintFromPROGMEMln(gTextMsg_PIN_OUT_OF_RANGE);
	}
#endif

}


#ifdef _SUPPORT_BUTTON_STATE_
//************************************************************************
static void	CheckOneInput(int inputNum)
{
int	inputState;

	inputState	=	digitalRead(inputNum);
	
	if (inputState != gButtonState[inputNum])
	{
		PrintFromPROGMEM(gTextMsg_DIGITALPIN);
		Serial.print(inputNum);
		PrintFromPROGMEM(gTextMsg_IS_NOW);
		Serial.print(inputState);
		Serial.println();
		
		gButtonState[inputNum]	=	inputState;
	}
}


//************************************************************************
//*	Watch for input changes
static void	WatchForInputChange()
{
uint8_t	ii;
#if defined(_SUPPORT_LIQUIDWARE_BUTTONPAD_)
	int	buttonPadStatePrev;
	int	buttonPadState;
#endif

	//*	set all pins as input
	for (ii = kDigitalPin_First; ii < kDigitalPin_Count; ii++)
	{
		//*	we dont want to do this on RXD and TXD
		if ((ii != kRXD_pin) && (ii != kTXD_pin))
		{
			pinMode(ii, INPUT);
			digitalWrite(ii, HIGH);	//*	this turns on the pull up resistor
		}
	}
#if defined(_SUPPORT_LIQUIDWARE_BUTTONPAD_)
	//Initialize Power to the ButtonPad through Arduino MEGA Pins 62 and 63
	pinMode(62, OUTPUT);
	pinMode(63, OUTPUT);
	digitalWrite(62, HIGH);
	digitalWrite(63, LOW);
	for (ii = 64; ii <= 69; ii++)
	{
		digitalWrite(ii, LOW);	//enable internal pulldown
	}
#endif

	while (!Serial.available())
	{
		for (ii = kDigitalPin_First; ii < kDigitalPin_Count; ii++)
		{
			//*	we dont want to do this on RXD and TXD
			if ((ii != kRXD_pin) && (ii != kTXD_pin))
			{
				CheckOneInput(ii);
			}
		}
		
	#if defined(_SUPPORT_LIQUIDWARE_BUTTONPAD_)
		buttonPadState	=	CButtonPad.readButtons();
		if (buttonPadState != buttonPadStatePrev)
		{
			buttonPadStatePrev	=	buttonPadState;
			Serial.print("buttonpad=");
			Serial.println(buttonPadState, HEX);
		}
	#endif
	}
	Serial.read();	//*	throw away the char
}
#endif

#pragma mark -
#pragma mark Duino644 routines

#if defined(_SUPPORT_RTC_)
//*********************************************************************************************************
static void GetTimeFromRTC()
{
int		rtc[7];
int		second;
char	textString[48];

	RTC.get(rtc, true);

	second	=	rtc[0];
	gMinute	=	rtc[1];
	gHour	=	rtc[2];
	gDow	=	rtc[3];
	gDay	=	rtc[4];
	gMonth	=	rtc[5];
	gYear	=	rtc[6];

	sprintf(textString, "Time is %02d/%02d/%02d %02d:%02d:%02d",	gMonth,
																	gDay,
																	gYear,
																	gHour,
																	gMinute,
																	second
																	);
	Serial.println(textString);

}

//*********************************************************************************************************
void SetTime()
{
	RTC.stop();
	RTC.set(DS1307_SEC,		0);
	RTC.set(DS1307_MIN,		gMinute);
	RTC.set(DS1307_HR,		gHour);
	RTC.set(DS1307_DOW,		gDow);
	RTC.set(DS1307_DATE,	gDay);
	RTC.set(DS1307_MTH,		gMonth);
	RTC.set(DS1307_YR,		gYear);
	RTC.start();
}


//*********************************************************************************************************
static void ClockFunctions()
{
	GetTimeFromRTC();
}

//*********************************************************************************************************
static void SetClockBumpMinutes(char clockCommandChar)
{
	GetTimeFromRTC();
	if (clockCommandChar == '=')
	{
		gYear	=	10;
		gMonth	=	7;
		gDay	=	4;
		gDow	=	7;
		gHour	=	19;
		gMinute	=	51;
	}
	else if (clockCommandChar == '+')
	{
		gMinute++;
		if (gMinute >= 60)
		{
			gMinute	=	0;
			gHour++;
		}
	}
	else
	{
		gMinute--;
		if (gMinute < 0)
		{
			gMinute	=	59;
			gHour--;
		}
	}
	SetTime();
}


#if defined(_SUPPORT_RTC_) && defined(_BOARD_DUINO644_ ) && defined(__AVR_ATmega644P__ )
//*********************************************************************************************************
static void PrintTimeToSerail1()
{
int		rtc[7];
int		second;
char	textString[48];

	RTC.get(rtc, true);

	second	=	rtc[0];
	gMinute	=	rtc[1];
	gHour	=	rtc[2];
	gDow	=	rtc[3];
	gDay	=	rtc[4];
	gMonth	=	rtc[5];
	gYear	=	rtc[6];

	sprintf(textString, "%02d/%02d/%02d \r%02d:%02d:%02d",	gMonth,
															gDay,
															gYear,
															gHour,
															gMinute,
															second
															);
	Serial1.println(textString);

}
#endif
#endif

#pragma mark -
#pragma mark AVR routines


//************************************************************************
static void	AVR_PortOutput(void)
{
char	inputTextString[16];
char	portLetter;

	PrintFromPROGMEM(gTextMsg_WHAT_PORT);
	GetInputTextString(inputTextString, 8);
	Serial.println();
	
	portLetter	=	inputTextString[0];

	if ((portLetter >= 'A') && (portLetter <= 'Z'))
	{
		switch(portLetter)
		{
		#ifdef DDRA
			case 'A':
				DDRA	=	0xff;
				while (!Serial.available())
				{
					PORTA	^=	0xff;
					delay(200);
				}
				PORTA	=	0;
				break;
		#endif

		#ifdef DDRB
			case 'B':
				DDRB	=	0xff;
				while (!Serial.available())
				{
					PORTB	^=	0xff;
					delay(200);
				}
				PORTB	=	0;
				break;
		#endif

		#ifdef DDRC
			case 'C':
				DDRC	=	0xff;
				while (!Serial.available())
				{
					PORTC	^=	0xff;
					delay(200);
				}
				PORTC	=	0;
				break;
		#endif

		#ifdef DDRD
			case 'D':
				DDRD	=	0xff;
				while (!Serial.available())
				{
					PORTD	^=	0xff;
					delay(200);
				}
				PORTD	=	0;
				break;
		#endif

		#ifdef DDRE
			case 'E':
				DDRE	=	0xff;
				while (!Serial.available())
				{
					PORTE	^=	0xff;
					delay(200);
				}
				PORTE	=	0;
				break;
		#endif

		#ifdef DDRF
			case 'F':
				DDRF	=	0xff;
				while (!Serial.available())
				{
					PORTF	^=	0xff;
					delay(200);
				}
				PORTF	=	0;
				break;
		#endif

		#ifdef DDRG
			case 'G':
				DDRG	=	0xff;
				while (!Serial.available())
				{
					PORTG	^=	0xff;
					delay(200);
				}
				PORTG	=	0;
				break;
		#endif

		#ifdef DDRH
			case 'H':
				DDRH	=	0xff;
				while (!Serial.available())
				{
					PORTH	^=	0xff;
					delay(200);
				}
				PORTH	=	0;
				break;
		#endif

		#ifdef DDRI
			case 'I':
				DDRI	=	0xff;
				while (!Serial.available())
				{
					PORTI	^=	0xff;
					delay(200);
				}
				PORTI	=	0;
				break;
		#endif

		#ifdef DDRJ
			case 'J':
				DDRJ	=	0xff;
				while (!Serial.available())
				{
					PORTJ	^=	0xff;
					delay(200);
				}
				PORTJ	=	0;
				break;
		#endif

		#ifdef DDRK
			case 'K':
				DDRK	=	0xff;
				while (!Serial.available())
				{
					PORTK	^=	0xff;
					delay(200);
				}
				PORTK	=	0;
				break;
		#endif

		#ifdef DDRL
			case 'L':
				DDRL	=	0xff;
				while (!Serial.available())
				{
					PORTL	^=	0xff;
					delay(200);
				}
				PORTL	=	0;
				break;
		#endif

			default:
				PrintFromPROGMEMln(gTextMsg_PortNotSupported);
				break;
		}
	}
	else
	{
		PrintFromPROGMEMln(gTextMsg_MustBeLetter);
	}
}

//************************************************************************
static void	VectorDisplay(void)
{
unsigned char	byte1;
unsigned char	byte2;
unsigned char	byte3;
unsigned char	byte4;

unsigned int	word1;
unsigned int	word2;
int				vectorIndex;
char			textString[48];
unsigned long	myMemoryPtr;
unsigned long	wordMemoryAddress;
unsigned int	realitiveAddr;
unsigned long	myFullAddress;
boolean			keepGoing;
unsigned long	absoluteAddr;

	myMemoryPtr		=	0;
	vectorIndex		=	0;
	PrintFromPROGMEMln(gTextMsg_CPU_Name);
	PrintFromPROGMEMln(gTextMsg_VECTOR_HEADER);
//	Serial.println(		"V#   ADDR   op code     inst      addr    Interrupt");
	//					 V#   ADDR   op code 
	//					  1 - 0000 = C3 BB 00 00 rjmp 03BB >000776 RESET
	while (vectorIndex < kInterruptVectorCount)
	{
		wordMemoryAddress	=	myMemoryPtr / 2;
		//					 01 - 0000 = 12 34
		sprintf(textString, "%2d - %04X = ", (vectorIndex + 1), wordMemoryAddress);
		Serial.print(textString);
		
		
		//*	the AVR is LITTLE ENDIAN, sqp the byte order
		byte1	=	pgm_read_byte_near(myMemoryPtr++);
		byte2	=	pgm_read_byte_near(myMemoryPtr++);
		word1	=	(byte2 << 8) + byte1;

		byte3	=	pgm_read_byte_near(myMemoryPtr++);
		byte4	=	pgm_read_byte_near(myMemoryPtr++);
		word2	=	(byte4 << 8) + byte3;



		sprintf(textString, "%02X %02X %02X %02X ", byte2, byte1, byte4, byte3);
		Serial.print(textString);

		if ((word1 & 0xc000) == 0xc000)
		{
			//*	rjmp instruction
			realitiveAddr	=	word1 & 0x3FFF;
			absoluteAddr	=	(wordMemoryAddress << 1) + (realitiveAddr << 1);
	//		sprintf(textString, "rjmp %04X ", realitiveAddr);
			sprintf(textString, "rjmp %04X >%06X ", realitiveAddr, absoluteAddr);
		}
		else if ((word1 & 0xfE0E) == 0x940c)
		{
			//*	jmp instruction, this is REALLY complicated, refer to the JMP instruction manual
			myFullAddress	=	word2 +
								((byte1 & 0x01) << 16) +
								((byte1 & 0xf0) << 17) +
								((byte2 & 0x01) << 21);
			myFullAddress	=	myFullAddress << 1;
								
			sprintf(textString, "jmp       >%06X ", myFullAddress);
		}
		while (strlen(textString) < 12)
		{
			strcat(textString, " ");	
		}
		
		Serial.print(textString);

	#if defined(_INTERRUPT_NAMES_DEFINED_)
		strcpy_P(textString, (char*)pgm_read_word(&(gInterruptNameTable[vectorIndex])));
		Serial.print(textString);
	#endif
	
		keepGoing	=	PrintNewLine_MORE();
		if (keepGoing == false)
		{
			break;
		}

		vectorIndex++;
	}
}

#ifdef _DEBUG_INTERRUPT_NAMES_
//************************************************************************
//*	this is a test routine to check to make sure there are only the vector strings
//*	in the intterupt name table that are needed. Just a sanity check on my logic
//************************************************************************
static void	DebugInterruptNames()
{
int		ii;
int		vectorCount;
char	theChar;

	Serial.println();
	ii			=	0;
	vectorCount	=	0;
	while ((theChar = pgm_read_byte(gAvrInt_RESET + ii)) != '*')
	{
		if (theChar == 0)
		{
			Serial.println();
			vectorCount++;
		}
		else
		{
			Serial.print(theChar);
		}
		ii++;
	}
	Serial.print("Vector count=");
	Serial.println(vectorCount);
	if (vectorCount <= kInterruptVectorCount)
	{
		Serial.println("Vector table OK");
	}
	else
	{
		Serial.println("Vector too big");
	}
}
#endif


#ifdef _ENABLE_XTENDED_MEMORY_



#define	xTendedMemoryStart	0x2200
//************************************************************************
//*	returns amount of extended memory
static long	XtendedMemoryCheck(boolean displayMem)
{
long			xtraMemCount;
unsigned char	*memoryPtr;
long			ii;
unsigned char	testValue;

	xtraMemCount	=	0;
	
	//*	Atmega2560 manual, page 37
	XMCRA	=	(1 << SRE);			//*	External RAM Enable
	XMCRB	=	0;					//*	we want all the address bits enabled
	
	//*	Atmega2560 manual, page 33
	DDRC	=	0xff;
	PORTC	=	0x00;
	
	//*	write to memory
	memoryPtr	=	0x0000;
	for (ii = xTendedMemoryStart; ii<0x0ffff; ii+= 256)
	{
		testValue		=	(ii >> 8) + (ii & 0x0ff);
		memoryPtr[ii]	=	testValue;
	}
	//*	no go back through and test
	for (ii = xTendedMemoryStart; ii<0x0ffff; ii+= 256)
	{
		testValue		=	(ii >> 8) + (ii & 0x0ff);
		if (memoryPtr[ii] == testValue)
		{
			xtraMemCount	+=	256;
		}
	}
	if (displayMem)
	{
		PrintFromPROGMEM(gTextMsg_EXTENDED_RAM);
		Serial.print(xtraMemCount / 1024);
		PrintFromPROGMEMln(gTextMsg_K);
	}

	XMCRA	=	0;			//*	External RAM DISABLE

	return(xtraMemCount);
}

//************************************************************************
static void	XtendedMemoryTest()
{
unsigned char	*memoryPtr;
long			ii;
long			errorCount;
long			memorySize;
unsigned char	testOffset;
unsigned char	testValue;
boolean			keepGoing;

	memorySize	=	XtendedMemoryCheck(true);	//*	true means print results

	if (memorySize > 0)
	{
		gRamIndex	=	xTendedMemoryStart;
		
		//*	Cerebot high order address bit
		DDRG	|=	(1 << 3);
		PORTG	=	0;

		//*	Atmega2560 manual, page 37
		XMCRA	=	(1 << SRE);			//*	External RAM Enable
		XMCRB	=	0;					//*	we want all the address bits enabled
		
		//*	Atmega2560 manual, page 33
		DDRC	=	0xff;
		PORTC	=	0x00;

		errorCount	=	0;
		keepGoing	=	true;
		testOffset	=	0;
		while (keepGoing)
		{
			
			memoryPtr	=	0x0000;
			//*	write to memory
			ii	=	xTendedMemoryStart;
			while (ii < (xTendedMemoryStart + memorySize))
			{
				testValue		=	testOffset + (ii >> 8) + (ii & 0x0ff);
				memoryPtr[ii]	=	testValue;
				if (memoryPtr[ii] != testValue)
				{
					errorCount++;
					Serial.print("Write err ");
					Serial.println(ii, HEX);
					keepGoing	=	false;
				}
				ii++;
			}
			//*	no go back through and test
			ii	=	xTendedMemoryStart;
			while (ii < (xTendedMemoryStart + memorySize))
			{
				testValue		=	testOffset + (ii >> 8) + (ii & 0x0ff);
				if (memoryPtr[ii] != testValue)
				{
					errorCount++;
					Serial.print('-');
					keepGoing	=	false;
				}
				ii++;
			}
			
			Serial.print('+');
			
			testOffset++;
			
			if (Serial.available())
			{
				Serial.read();	//*	throw away the char
				keepGoing	=	false;
			}
		}
		Serial.println();
		Serial.print("Error count=");
		Serial.println(errorCount);
	}
	
}
#endif


#pragma mark -

#ifdef _SUPPORT_SD_CARD_
//************************************************************************
static void	ShowSDcardDirectroy()
{
//#ifdef __AVR_ATmega644__
	SD.ls("quotes.txt");
	SD.readBytes("quotes.txt", 0);
//#endif
}
#endif

#pragma mark -

#if (SERIAL_PORT_COUNT > 1)
//************************************************************************
static void SerialPortComm()
{
uint8_t	escapeCount;
char	theChar;

	PrintFromPROGMEMln(gTextMsg_OpenSerialPort);
	escapeCount	=	0;
	
	Serial1.begin(9600);
	
	while (escapeCount < 2)
	{
		if (Serial.available())
		{
			theChar	=	Serial.read();
			Serial1.print(theChar);
			if (theChar == 0x1B)
			{		
				escapeCount++;
			}
			else
			{
				escapeCount	=	0;
			}
			
		}
		if (Serial1.available())
		{
			theChar	=	Serial1.read();
			Serial.print(theChar);
		}
	}
}
#endif


#pragma mark -
#ifdef _SUPPORT_GRAPHICS_
#include	"UnivGraphicsLib.h"

//*******************************************************************************
void	DisplayMoriePattern(RECT *graphicsRect, char iterations)
{
int			xx, yy;
int			ii, jj;
int			delta;
RGBColor	myColor;

	//*	lets try a morie pattern
	SetRGBcolor(&myColor, 0, 0, 0);
	RGBBackColor(&myColor);
	xx	=	gWidth / 2;
	yy	=	gHeight / 2;
	for (jj = 0; jj < iterations; jj++)
	{
		EraseRect(graphicsRect);
		EraseScreen();
		delta	=	random(15, 40);
		SetRGBcolor(&myColor, random(50, 255),random(50, 255),random(50, 255));
		RGBForeColor(&myColor);
		for (ii = 10; ii < (gHeight / 2); ii += 3)
		{
			FrameOvalFromCenter(xx + delta, yy, ii, ii);
			FrameOvalFromCenter(xx - delta, yy, ii, ii);
		}
		delay(400);
	}
}


//************************************************************************
static void	GraphicsDemo(void)
{
int			ii;
char		theFontName[32];
RECT		graphicsRect;
	
	UnivGraphicsLib_Init();

	graphicsRect.top	=	0;
	graphicsRect.left	=	0;
	graphicsRect.bottom	=	gHeight - 1;
	graphicsRect.right	=	gWidth - 1;

	for (ii = kHershey_FirstFontNumber; ii <kHersheyFont_last; ii++)
	{
		GetHersheyFontName(ii, theFontName);
		Serial.println(theFontName);
	}
	EraseScreen();
//	HersheyDisplayFontList(&graphicsRect, false);
	HersheyDisplayFontSamples(&graphicsRect);
	delay(1400);
	
	DisplayMoriePattern(&graphicsRect, 2);
}

#endif



#pragma mark -
//************************************************************************
void setup()
{
uint8_t	ii;

#if defined(_BOARD_LIQUIDWARE_TOUCH_SLIDE_)
	//*	arduino in reset
	//*	the Liquidware Touch slide uses the same 2 pins (0,1) for tx,rx.
	//*	It can put the Arduino in reset and thus take over these 2 pins
	SETBIT(ARDUINO_RESET_DDR, ARDUINO_RESET_PNUM);
	CLRBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PNUM);
	delay(100);
#endif

#ifdef _SUPPORT_I2C_TESTING_
	gI2CneedsInit		=	true;
	gI2CfoundAddress	=	0;
	gI2CtestCounter		=	0;
#endif
	gRamPtr				=	0;
	gRamIndex			=	0;
	gFlashIndex			=	0;
	gPageLineCount		=	0;


#if defined(__AVR_ATmega2560__) && !defined(_BOARD_LIQUIDWARE_TOUCH_SLIDE_)
//*	this is for Cerebot Plus
	DDRE	=	0xff;
	PORTE	=	0xff;
#endif
	

	Serial.begin(9600);
//	Serial.begin(115200);

	Serial.println();


	PrintFromPROGMEM(gTextMsg_Explorer);
	Serial.println();
	PrintFromPROGMEM(gTextMsg_CPU_Type);
	PrintFromPROGMEMln(gTextMsg_CPU_Name);
	PrintHelp();


//kDigitalPin_FirstAnalog
	for (ii = kDigitalPin_First; ii < kDigitalPin_Count; ii++)
	{
		if ((ii != kRXD_pin) && (ii != kTXD_pin))
		{
	//		pinMode(ii, INPUT);
	//		digitalWrite(ii, HIGH);	//*	turn on pull up resister
		}
	}

#ifdef _SUPPORT_BUTTON_STATE_
	//*	get the current state of the inputs
	for (ii=0; ii < kDigitalPin_Count; ii++)
	{
		gButtonState[ii]	=	digitalRead(ii);
	}
#endif


	PrintFromPROGMEM(gTextMsg_Prompt);

#if defined(_BOARD_DUINO644_ ) && defined(__AVR_ATmega644P__ )
	Serial1.begin(9600);
#endif

#if (defined(SPEAKER_PIN1) && defined(SPEAKER_PIN2))
//	PlayTone();
#endif

#ifdef kLED_PIN
	pinMode(kLED_PIN, OUTPUT);
	while (!Serial.available())
	{
		digitalWrite(kLED_PIN, HIGH);
		delay(200);
		digitalWrite(kLED_PIN, LOW);
		delay(200);
	}
#endif
}


//************************************************************************
void loop()
{
char	theChar;
	
	if (Serial.available())
	{
		theChar	=	GetChar();
		if (theChar >= 0x20)
		{
			Serial.print(theChar);
		}
		PrintFromPROGMEM(gTextMsg_SPACE);
		gPageLineCount	=	0;
		switch(theChar)
		{
			case '!':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_EP, 2);
				BlinkAllLEDS();
				break;

		#if (SERIAL_PORT_COUNT > 1)
			case '#':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_HASH, 2);
				SerialPortComm();
				break;
		#endif

			case '0':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_0, 2);
				gFlashIndex	=	0;
				gRamIndex	=	0;
				break;
			
			//*	1,2,3, 4 are for use with LCDserialShield
			case '1':
				PrintFromPROGMEMln(gTextMsg_CPU_Name);
				break;

			case '2':
				Serial.println(__AVR_ARCH__);
				break;

			case '3':
				PrintFromPROGMEMln(gTextMsg_AVR_LIBC_VER_STR);
				break;

			case '?':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_QM, 2);
				PrintCPUstats();
				break;
			

		#ifdef kAnalogPin_Count
			case 'A':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_A, 2);
				ShowOneAnalogPin();
				break;
		#endif	
			case '4':
			case 'B':
				BlinkLED();
				break;
				
		#if defined(_SUPPORT_RTC_)
			case 'C':
				ClockFunctions();
				break;

			case '=':
			case '+':
			case '-':
				SetClockBumpMinutes(theChar);
				break;

		#endif
			
		#ifdef _SUPPORT_SD_CARD_
			case 'D':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_D, 2);
				ShowSDcardDirectroy();
				break;
		#endif
				
			case 'E':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_E, 2);
				DumpHex(kDUMP_EEPROM, 0, 32);
				break;
			
			case 'F':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_F, 2);
				DumpHex(kDUMP_FLASH, gFlashIndex, 16);
				gFlashIndex	+=	256;
				break;

		#ifdef _SUPPORT_GRAPHICS_
			case 'G':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_G, 2);
				GraphicsDemo();
				break;
		#endif
			
			case 'H':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_H, 2);
				PrintHelp();
				break;

		#ifdef _SUPPORT_I2C_TESTING_
			case 'I':
				if (gI2CneedsInit)
				{
					Wire.begin();
					gI2CneedsInit	=	false;
				}
				ScanI2CBus(1, 127);
				break;
			
			case 'J':
				PrintFromPROGMEM(gTextMsg_HELP_MSG_J, 2);
				PrintFromPROGMEM(gTextMsg_SPACE);
				Serial.println(gI2CfoundAddress, DEC);
				SendDataToI2Cdevice();
				break;
		#endif
					
			case 'M':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_M, 2);
				MapPins();
				break;
				
			case 'P':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_P, 2);
				ShowOneDigitalPin();
				break;
				
			case 'R':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_R, 2);
				DumpHex(kDUMP_RAM, gRamIndex, 16);
				gRamIndex	+=	256;
				break;
			
			case 'S':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_S, 2);
				ShowPins();
				break;
			
			case '5':
			case 'T':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_T, 2);
				PlayTone();
				break;
				
		#ifdef _SUPPORT_BUTTON_STATE_
			case '6':
			case 'W':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_W, 2);
				WatchForInputChange();
				break;
		#endif

			case 'V':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_V, 2);
				VectorDisplay();
				break;
				
		#ifdef _ENABLE_XTENDED_MEMORY_
			case 'X':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_X, 2);
				XtendedMemoryTest();
				break;
		#endif
				
			case 'Y':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_Y, 2);
				AVR_PortOutput();
				break;
				
			case 'Z':
				PrintFromPROGMEMln(gTextMsg_HELP_MSG_Z, 2);
				SetPWMpin();
				break;

		#ifdef _DEBUG_INTERRUPT_NAMES_
			case '/':
				DebugInterruptNames();
				break;
		#endif


			default:
				PrintFromPROGMEMln(gTextMsg_HUH);
				break;
				
		}
		PrintFromPROGMEM(gTextMsg_Prompt);
	}
#if defined(_SUPPORT_RTC_) && defined(_BOARD_DUINO644_ ) && defined(__AVR_ATmega644P__ )
	else
	{
		if ((millis() % 1000) == 0)
		{
			PrintTimeToSerail1();
		}
	}
#endif
}



