
/* ******************************************************************************************************************
   *** BasicBoss Cross Compiler  (ISO C99)                                                                        ***
   *** ------------------------                                                                                   ***
   *** author : Marco Timm                                                                                        ***
   *** date   : 10.01.2008                                                                                        ***
   *** version: v0.2                                                                                              ***
   *** contact: marco-onscreen(at)gmx.de                                                                          ***
   *** licence: FREEWARE                                                                                          ***
   ***                                                                                                            ***
   *** Anwendung: bbcompiler.exe infile [outfile]                                                                 ***
   *** -infile muss prg-format sein                                                                               ***
   *** -outfile wird im prg-format geschrieben                                                                    ***
   ***                                                                                                            ***
   *** WICHTIG!!!:                                                                                                ***
   ***  PROTOCOL muss in der 1.Zeile im Quelltext stehen, damit die Fehler angezeigt werden koennen!!!            ***
   ***  (Quelltext: 0 PfundzeichenPROTOCOL )                                                                      ***
   ***                                                                                                            ***
   *** Der BasicBoss Cross Compiler ist ein 6502 emulator, der den BasicBoss V2.4 ausfuehrt.                      ***
   *** BasicBoss ist ein BasicCompiler für den C64, geschrieben von Thilo Herrmann und Veroeffendlicht 1988.      ***
   ***	                                                                                                          ***
   ***	 Quelle: 64'er Extra Nr. 11                                                                               ***
   ***	 (c) 1988 by Markt&Technik Verlag Aktiengesellschaft,                                                     ***
   ***	 Hans-Pinsel-Straße 2, D-8013 Haar bei München/West-Germany                                               ***
   ******************************************************************************************************************/

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "boss_abbild.inc"

unsigned char mem_readpc();

char poke2asc[257]= "\
@abcdefghijklmno\
pqrstuvwxyz[°]??\
 !\"#$%&'()*+,-./\
0123456789:;<=>?\
?ABCDEFGHIJHLMNO\
PQRSTUVWXYZ?????\
????????????????\
????????????????\
@abcdefghijklmno\
pqrstuvwxyz[°]??\
 !\"#$%&'()*+,-./\
0123456789:;<=>?\
?ABCDEFGHIJHLMNO\
PQRSTUVWXYZ?????\
????????????????\
????????????????\
";


//OPCODES
#define   ADC_Immediate   0x69
#define   ADC_ZeroPage    0x65
#define   ADC_ZeroPageX   0x75
#define   ADC_Absolute    0x6d   
#define   ADC_AbsoluteX   0x7d  
#define   ADC_AbsoluteY   0x79
#define   ADC_IndirectX   0x61
#define   ADC_IndirectY   0x71

#define   AND_Immediate   0x29
#define   AND_ZeroPage    0x25
#define   AND_ZeroPageX   0x35
#define   AND_Absolute    0x2d   
#define   AND_AbsoluteX   0x3d  
#define   AND_AbsoluteY   0x39
#define   AND_IndirectX   0x21
#define   AND_IndirectY   0x31

#define   ASL_Accumulator 0x0a
#define   ASL_ZeroPage    0x06
#define   ASL_ZeroPageX   0x16
#define   ASL_Absolute    0x0e
#define   ASL_AbsoluteX   0x1e  

#define   BIT_ZeroPage    0x24
#define   BIT_Absolute    0x2c

#define   BPL   0x10
#define   BMI   0x30
#define   BVC   0x50
#define   BVS   0x70
#define   BCC   0x90
#define   BCS   0xB0
#define   BNE   0xD0
#define   BEQ   0xF0

#define   BRK   0x00

#define   CMP_Immediate   0xc9
#define   CMP_ZeroPage    0xc5
#define   CMP_ZeroPageX   0xd5
#define   CMP_Absolute    0xcd
#define   CMP_AbsoluteX   0xdd  
#define   CMP_AbsoluteY   0xd9
#define   CMP_IndirectX   0xc1
#define   CMP_IndirectY   0xd1

#define   CPX_Immediate   0xe0
#define   CPX_ZeroPage    0xe4
#define   CPX_Absolute    0xec   

#define   CPY_Immediate   0xc0
#define   CPY_ZeroPage    0xc4
#define   CPY_Absolute    0xcc   

#define   DEC_ZeroPage    0xc6
#define   DEC_ZeroPageX   0xd6
#define   DEC_Absolute    0xce   
#define   DEC_AbsoluteX   0xde 

#define   EOR_Immediate   0x49
#define   EOR_ZeroPage    0x45
#define   EOR_ZeroPageX   0x55
#define   EOR_Absolute    0x4d   
#define   EOR_AbsoluteX   0x5d  
#define   EOR_AbsoluteY   0x59
#define   EOR_IndirectX   0x41
#define   EOR_IndirectY   0x51

#define   CLC   0x18
#define   SEC   0x38
#define   CLI   0x58
#define   SEI   0x78
#define   CLV   0xb8
#define   CLD   0xd8
#define   SED   0xf8

#define   INC_ZeroPage    0xe6
#define   INC_ZeroPageX   0xf6
#define   INC_Absolute    0xee   
#define   INC_AbsoluteX   0xfe 

#define   JMP_Absolute    0x4c 
#define   JMP_Indirect    0x6c

#define   JSR   0x20 

#define   LDA_Immediate   0xa9
#define   LDA_ZeroPage    0xa5
#define   LDA_ZeroPageX   0xb5
#define   LDA_Absolute    0xad   
#define   LDA_AbsoluteX   0xbd  
#define   LDA_AbsoluteY   0xb9
#define   LDA_IndirectX   0xa1
#define   LDA_IndirectY   0xb1

#define   LDX_Immediate   0xa2
#define   LDX_ZeroPage    0xa6
#define   LDX_ZeroPageY   0xb6
#define   LDX_Absolute    0xae   
#define   LDX_AbsoluteY   0xbe  

#define   LDY_Immediate   0xa0
#define   LDY_ZeroPage    0xa4
#define   LDY_ZeroPageX   0xb4
#define   LDY_Absolute    0xac   
#define   LDY_AbsoluteX   0xbc  

#define   LSR_Accumulator 0x4a
#define   LSR_ZeroPage    0x46
#define   LSR_ZeroPageX   0x56
#define   LSR_Absolute    0x4e
#define   LSR_AbsoluteX   0x5e  

#define   NOP             0xea

#define   ORA_Immediate   0x09
#define   ORA_ZeroPage    0x05
#define   ORA_ZeroPageX   0x15
#define   ORA_Absolute    0x0d   
#define   ORA_AbsoluteX   0x1d  
#define   ORA_AbsoluteY   0x19
#define   ORA_IndirectX   0x01
#define   ORA_IndirectY   0x11

#define   TAX   0xaa
#define   TXA   0x8a
#define   DEX   0xca
#define   INX   0xe8
#define   TAY   0xa8
#define   TYA   0x98
#define   DEY   0x88
#define   INY   0xc8

#define   ROL_Accumulator 0x2a
#define   ROL_ZeroPage    0x26
#define   ROL_ZeroPageX   0x36
#define   ROL_Absolute    0x2e
#define   ROL_AbsoluteX   0x3e  

#define   ROR_Accumulator 0x6a
#define   ROR_ZeroPage    0x66
#define   ROR_ZeroPageX   0x76
#define   ROR_Absolute    0x6e
#define   ROR_AbsoluteX   0x7e  

#define   RTI   0x40

#define   RTS   0x60

#define   SBC_Immediate   0xe9
#define   SBC_ZeroPage    0xe5
#define   SBC_ZeroPageX   0xf5
#define   SBC_Absolute    0xed   
#define   SBC_AbsoluteX   0xfd  
#define   SBC_AbsoluteY   0xf9
#define   SBC_IndirectX   0xe1
#define   SBC_IndirectY   0xf1

#define   STA_ZeroPage    0x85
#define   STA_ZeroPageX   0x95
#define   STA_Absolute    0x8d   
#define   STA_AbsoluteX   0x9d  
#define   STA_AbsoluteY   0x99
#define   STA_IndirectX   0x81
#define   STA_IndirectY   0x91

#define TXS  0x9a      // Transfer X to Stack ptr
#define TSX  0xba      // Transfer Stack ptr to X
#define PHA  0x48      // PusH Accumulator
#define PLA  0x68      // PuLl Accumulator
#define PHP  0x08      // PusH Processor status
#define PLP  0x28      // PuLl Processor status

#define STX_ZeroPage   0x86
#define STX_ZeroPageY  0x96
#define STX_Absolute   0x8e 

#define STY_ZeroPage   0x84
#define STY_ZeroPageX  0x94
#define STY_Absolute   0x8c 


// bool werte
#define FALSE 0
#define TRUE  1

// variablen
char errorausgabe_flag = FALSE;   //ausgabe auf drucker
char save_flag = FALSE;           //compilat speichern

#define NONE 255
unsigned char TALK = 0;
unsigned char LISTEN = 1;
unsigned char LISTEN_TALK = NONE;         //bus

unsigned char SEK_ADRESS[2]= {NONE,NONE};   //(open|... , kanal)


char MESSAGE[0x10000];
int MESSAGE_LEN = 0;


#define ROUT 0
#define DATA 1
#define PROG 2
#define NORMAL 3

unsigned char SAVE_FILE[4][0x10000];
int SAVE_LEN[4] = {0,0,0,0};
unsigned char SAVE_STATE = NONE;


unsigned char LOAD_FILE[0x10000];
int LOAD_MAX = 0;
int LOAD_POS = 0;

unsigned char ram[0x10000];      
unsigned char rom[0x10000];

int PC=0;
int SP=255;

int A=0;          //Register
int X=0;
int Y=0;

char DECIMAL    = FALSE;
char INTERRUPT  = FALSE;
char BREAK      = FALSE;
char ZERO       = FALSE;
char NEGATIVE   = FALSE;
char OVERFLOW   = FALSE;
char CARRY      = FALSE;

char BASIC      = FALSE;
char IO         = FALSE;
char KERNAL     = FALSE;
char ZEICHENROM = FALSE;

char exitflag = FALSE;       //prg beenden



// speicher laden und register setzen
void init_ram_rom(char* in)
{
	FILE *fp;
	unsigned char *ptr = BOSS_ABBILD;
	int i,zw;
	
	// laden
	for(i=0; i<0x10000; i++) ram[i] = *ptr++;       //ram laden
	for(i=0; i<0x10000; i++) rom[i] = *ptr++;       //rom laden
    
        // 0x63,0x09,
	PC = 0x0963;

        // 0xf6,0x00,0x00,0x00,
	SP = 0xf6;
	A  = 0x00;
	X  = 0x00;
	Y  = 0x00;

        // 0x00,0x00,0x00,0x00,0x00,0x00,0x00
	DECIMAL   =FALSE;
	INTERRUPT =FALSE;
	BREAK     =FALSE;
	ZERO      =FALSE;
	NEGATIVE  =FALSE;
	OVERFLOW  =FALSE;
	CARRY     =FALSE;

        // 0x01,0x01,0x01,0x00
	BASIC      =TRUE;
	IO         =TRUE;
        KERNAL     =TRUE;
	ZEICHENROM =FALSE;
	
	//assert(ptr == (BOSS_ABBILD + sizeof(BOSS_ABBILD)));

        // Basicprogramm laden
	fp = fopen(in,"rb");

	//Fehlerbearbeitung
	if (fp==NULL) {
		printf("Error: Konnte '%s' nicht laden.\n",in);
		//int dumy = getchar();
		exit(1);
	}

	LOAD_MAX=0;
	LOAD_POS=0;
	while (TRUE){
		zw = getc(fp);
		if (zw == EOF) break;
		LOAD_FILE[LOAD_MAX]=(char)zw;
		LOAD_MAX++;
	}

	fclose(fp);
	return;
}










//speicherzugriff schreiben
void mem_write(int adr, unsigned char wert){
	if (adr==0xdd00 && IO==TRUE) return;           //Hardware verbindung zu disk

	// ram rom zugriff
	if (adr == 0x0001){
		unsigned char zw = wert & 0x07;
		// basic, IO, Zeichenrom, kernal 
		char port[8][4]={{FALSE, FALSE, FALSE, FALSE},    // 0x000 
		                 {FALSE, FALSE, TRUE , FALSE},    // 0x001 
		                 {FALSE, FALSE, TRUE , TRUE },    // 0x010 
		                 {TRUE , FALSE, TRUE , TRUE },    // 0x011 
		                 {FALSE, FALSE, FALSE, FALSE},    // 0x100 
		                 {FALSE, TRUE , FALSE, FALSE},    // 0x101 
		                 {FALSE, TRUE , FALSE, TRUE },    // 0x110 
	                     {TRUE , TRUE , FALSE, TRUE }};   // 0x111

		BASIC      =port[zw][0];
		IO         =port[zw][1];
		ZEICHENROM =port[zw][2];
		KERNAL     =port[zw][3];
        
		ram[1] = (ram[1] & 0xf8) | zw;    //nur die bit 2..0 ersetzen
		return;
	}

	// IO zugriff
	if (0xd000 <= adr && adr < 0xe000 && IO==TRUE) {
		rom[adr]=wert;
		return;
	}

	// sontiges
	ram[adr]=wert;
	return;
}
	

//speicherzugriff lesen
unsigned char mem_read(int adr){
	if (adr==0xdd00 && IO==TRUE) return 0;          //Hardware verbindung zu disk
	if (0xa000 <= adr && adr < 0xc000 && BASIC==TRUE)       return rom[adr];
	if (0xd000 <= adr && adr < 0xe000 && IO==TRUE)          return rom[adr];
	if (0xd000 <= adr && adr < 0xe000 && ZEICHENROM==TRUE)  return rom[adr - 0x1000];  // Zeichen rom ist bei 0xc000 gespeichert
	if (0xe000 <= adr && adr < 0x10000 && KERNAL==TRUE)     return rom[adr];
	return ram[adr];
}


// word lesen
int mem_read_word(int adr){
	return mem_read(adr) + mem_read(adr+1)*256;
}


// byte bei PC lesen
unsigned char mem_readpc(){
	unsigned char back = mem_read(PC);
	PC++;
	return back;
}


// word bei PC lesen
int mem_readpc_word(){
	int back = mem_read(PC) + mem_read(PC+1)*256;
	PC=PC+2;
	return back;
}



//OPERATIONEN
void adc(unsigned char wert){
	// A=A+OP+CARRY
	int zw = A;
	int zw2 = wert;
	int tmp;

	if (!DECIMAL) {
		if (zw > 127) zw= -(256-zw);
		if (zw2 > 127) zw2= -(256-zw2);

		A = A + wert;
		tmp = zw + zw2;
		if (CARRY)  { A = A + 1; tmp = tmp+1; }

		if ((A & 0x0100) != 0){ CARRY=TRUE; A = A & 0xff;} else CARRY=FALSE;
		if ((A & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
		if (A == 0) ZERO=TRUE; else ZERO= FALSE;

		if (tmp < -128 || tmp > 127) OVERFLOW = TRUE; else OVERFLOW = FALSE;
             
	}else printf("!!! adc decimal nicht impl. !!!");
	return;
}


void And(unsigned char wert){
	// A = A & wert
	A = A & wert;          //bitweises und
	if ((A & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE= FALSE;
	if (A == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}


void asl(int adr){
	//
	int wert=A;
	if (adr != -1) wert = mem_read(adr);

	wert = wert << 1;
	if ((wert & 0x0100) != 0) {CARRY=TRUE; wert = wert & 0xff;} else CARRY=FALSE;
	if ((wert & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	if (wert == 0) ZERO=TRUE; else ZERO=FALSE;

	if (adr == -1) A=wert; else mem_write(adr,wert);
	return;
}



void bit(unsigned char wert){
	//
	if ((wert & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	if ((wert & 64) != 0) OVERFLOW=TRUE; else OVERFLOW=FALSE;
	if ((wert & A)==0) ZERO=TRUE; else ZERO=FALSE;
	return;
}



void branch(char flag, unsigned char wert){
	//
	int wert2 = wert;
	if (!flag) return;
	if (wert2 > 127) wert2 = -(256-wert2); 
	PC = PC + wert2;
	return;
}


void compare(int reg , unsigned char wert){
	//
	int tmp;
	
	if (reg == wert) ZERO=TRUE; else ZERO=FALSE;
	if (reg >= wert) CARRY=TRUE; else CARRY=FALSE;

	tmp = reg - wert;
	if (tmp < 0) tmp = tmp + 256;
	if ((tmp & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}



void dec(int adr){
	//
	int wert = mem_read(adr)-1;
	if (wert == -1) wert=255;
	mem_write(adr,wert);
	if (wert==0) ZERO=TRUE; else ZERO=FALSE;
	if ((wert & 0x80)!=0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}


void eor(unsigned char wert){
	//
	A = A ^ wert;         //xor
	if (A==0) ZERO=TRUE; else ZERO=FALSE;
	if ((A & 0x80)!=0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}


void inc(int adr){
	//
	int wert = mem_read(adr)+1;
	if (wert==256) wert=0;
	mem_write(adr,wert);
	if (wert==0) ZERO=TRUE; else ZERO=FALSE;
	if ((wert & 0x80)!=0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}


void push(unsigned char wert){
	//    
	if (SP == -1){        //stacküberlauf
		printf ("SP < 0 on $%x (%d) not impl.",PC-1, PC-1);
		exitflag = TRUE;
		return;
	}
	mem_write(0x0100 + SP,wert);
	SP--;
	return;
}

unsigned char pop(){
	//
	SP ++;
	if (SP == 256){
		printf ("SP > 255 on $%x (%d) not impl.",PC-1, PC-1);
		exitflag = TRUE;
	}
	return mem_read(0x0100 + SP);
}

void pushpc(){
	//
	int wert = PC-1;
	int lo = wert & 0xff;
	int hi = wert >> 8;
	push(hi);
	push(lo);
	return;
}

void poppc(){
	//
	int lo = pop();
	int hi = pop();
	PC = (hi << 8) + lo + 1;
	return;
}


void lda(unsigned char wert){
	//
	A = wert;
	if (wert==0) ZERO=TRUE; else ZERO=FALSE;
	if ((wert & 0x80)!=0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}


void ldx(unsigned char wert){
	//
	X = wert;
	if (wert==0) ZERO=TRUE; else ZERO=FALSE;
	if ((wert & 0x80)!=0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}


void ldy(unsigned char wert){
	//
	Y = wert;
	if (wert==0) ZERO=TRUE; else ZERO=FALSE;
	if ((wert & 0x80)!=0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	return;
}



void lsr(int adr){
	//
	int wert;
	if (adr == -1) wert=A; else wert = mem_read(adr);
	if ((wert & 0x01) == 1) CARRY=TRUE; else CARRY=FALSE;
	wert = wert >> 1;
	NEGATIVE=TRUE;
	if (wert == 0) ZERO=TRUE; else ZERO=FALSE;
	if (adr == -1) A=wert; else mem_write(adr,wert);
	return;
}


void ora(unsigned char wert){
	A = A | wert;         //or
	if ((A & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	if (A == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}


void inx(){
	//
	X++;
	if (X==256) X=0;
	if ((X & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE= FALSE;
	if (X == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}


void iny(){
	//
	Y++;
	if (Y==256) Y=0;
	if ((Y & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE= FALSE;
	if (Y == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}


void dex(){
	//
	X--;
	if (X==-1) X=255;
	if ((X & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE= FALSE;
	if (X == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}


void dey(){
	//
	Y--;
	if (Y==-1) Y=255;
	if ((Y & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE= FALSE;
	if (Y == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}

void transfer_test(unsigned char wert){
	//
	if ((wert & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE= FALSE;
	if (wert == 0) ZERO=TRUE; else ZERO= FALSE;
	return;
}


void rol(int adr){
	//
	int wert;
	if (adr == -1) wert=A; else wert = mem_read(adr);
	wert = wert << 1;
	if (CARRY) wert = wert | 0x0001;
	if ((wert & 0x0100) != 0) {CARRY=TRUE; wert = wert & 0x00ff;} else CARRY= FALSE;
	if ((wert & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	if (wert == 0) ZERO=TRUE; else ZERO= FALSE;
	if (adr == -1) A=wert; else mem_write(adr,wert);
	return;
}


void ror(int adr){
	//
	int wert;
	if (adr == -1) wert=A; else wert = mem_read(adr);
	NEGATIVE = CARRY;
	if (CARRY) wert = wert | 0x0100;
	if ((wert & 0x01) == 1) CARRY=TRUE; else CARRY=FALSE;
	wert = wert >> 1;
	if (wert == 0) ZERO=TRUE; else ZERO= FALSE;
	if (adr == -1) A=wert; else mem_write(adr,wert);
	return;
}


void pushstatus(){
	int status = 0;
	if (NEGATIVE)  status += (1 << 7);
	if (OVERFLOW)  status += (1 << 6);
	if (BREAK)     status += (1 << 4);
	if (DECIMAL)   status += (1 << 3);
	if (INTERRUPT) status += (1 << 2);
	if (ZERO)      status += (1 << 1);
	if (CARRY)     status += (1 << 0);
	push(status);
	return;
}

void popstatus(){
	int status = pop();
	NEGATIVE= FALSE;
	OVERFLOW= FALSE;
	DECIMAL = FALSE;
	INTERRUPT=FALSE;
	ZERO    = FALSE;
	CARRY   = FALSE;
	if ((status & (1 << 7)) != 0) NEGATIVE = TRUE;
	if ((status & (1 << 6)) != 0) OVERFLOW = TRUE;
	if ((status & (1 << 3)) != 0) DECIMAL = TRUE; 
	if ((status & (1 << 2)) != 0) INTERRUPT = TRUE;
	if ((status & (1 << 1)) != 0) ZERO = TRUE;      
	if ((status & (1 << 0)) != 0) CARRY = TRUE;
	return;
}

void sbc(unsigned char wert){
	//
	int cwert = wert;
	int zw = A;        
	int zw2 = wert;
	int tmp;
	
	if (!DECIMAL){
		if (!CARRY) cwert++;
		if (A == (cwert & 0xff)) ZERO=TRUE; else ZERO=FALSE;
		if (A >= cwert) CARRY=TRUE; else CARRY=FALSE;
		if (zw > 127) zw = -(256-zw);
		if (zw2 > 127) zw = -(256-zw2);
		tmp= zw - zw2;
		if (!CARRY) tmp = tmp - 1;
		if (wert > 127) OVERFLOW = TRUE; else OVERFLOW = FALSE;
		A = A - cwert; 
		if (A<0) A=A+256;
		if ((A & 0x80) != 0) NEGATIVE=TRUE; else NEGATIVE=FALSE;
	}else printf ("!!! sbc decimal nicht impl. !!!");
	return;
}



//adressen und werte
int Accumulator(){
	return -1;
}

unsigned char Immediate(){
	return mem_readpc();
}

unsigned char ZeroPage(){
	int adr = mem_readpc();
	return mem_read(adr);
}

unsigned char ZeroPageX(){
	int adr = mem_readpc()+X;
	if  (adr > 255) adr -=256;    //6502 bug
	return mem_read(adr);
}

unsigned char ZeroPageY(){
	int adr = mem_readpc()+Y;
	if  (adr > 255) adr -=256;     //6502 bug
	return mem_read(adr);
}

unsigned char Absolute(){
	int adr = mem_readpc_word();
	return mem_read(adr);
}

unsigned char AbsoluteX(){
	int adr = mem_readpc_word()+X;
	return mem_read(adr);
}

unsigned char AbsoluteY(){
	int adr = mem_readpc_word()+Y;
	return mem_read(adr);
}

unsigned char IndirectX(){
	int adr = mem_readpc() + X;
	int indadr = mem_read_word(adr);
	return mem_read(indadr);
}

unsigned char IndirectY(){
	int adr = mem_readpc();
	int indadr = mem_read_word(adr)+Y;
	return mem_read(indadr);
}

int ZeroPage_adr(){
	return mem_readpc();
}

int ZeroPageX_adr(){
	int adr = mem_readpc()+X;
	if  (adr > 255) adr -=256;      //6502 bug
	return adr;
}

int ZeroPageY_adr(){
	int adr = mem_readpc()+Y;
	if  (adr > 255) adr -=256;      //6502 bug
	return adr;
}

int Absolute_adr(){
	int adr = mem_readpc_word();
	return adr;
}

int AbsoluteX_adr(){
	int adr = mem_readpc_word()+X;
	return adr;
}

int AbsoluteY_adr(){
	int adr = mem_readpc_word()+Y;
	return adr;
}

int IndirectX_adr(){
	int adr = mem_readpc()+ X;
	int indadr = mem_read_word(adr);
	return indadr;
}

int IndirectY_adr(){
	int adr = mem_readpc();
	int indadr = mem_read_word(adr)+Y;
	return indadr;
}

int Indirect_adr(){
	int adr = mem_readpc_word();
	int indadr = mem_read_word(adr);
	return indadr;
}



// 6502 opcodes emulation
void opcode(){
	unsigned char op = mem_readpc();
	
	if ((op & 0x0f) == 0){
		if ((op & 0x80) == 0){
			if (op == BRK) {printf("BRK"); exitflag= TRUE;         return;}
			if (op == BPL) {branch( !NEGATIVE,Immediate() );       return;}    //Branch on PLus
			if (op == JSR) {int adr = mem_readpc_word(); pushpc(); PC=adr; return;}
			if (op == BMI) {branch( NEGATIVE,Immediate());         return;}    //Branch on MInus
			if (op == RTI) {popstatus(); poppc();                  return;}
			if (op == BVC) {branch( !OVERFLOW,Immediate() );       return;}    //Branch on oVerflow Clear
			if (op == RTS) {poppc();                               return;}
			if (op == BVS) {branch( OVERFLOW,Immediate() );        return;}    //Branch on oVerflow Set
		}else{
			if (op == BCC) {branch( !CARRY,Immediate());           return;}    //Branch on Carry Clear
			if (op == LDY_Immediate) {ldy( Immediate() );          return;}
			if (op == BCS) {branch( CARRY,Immediate() );           return;}    //Branch on Carry Set
			if (op == CPY_Immediate) {compare( Y,Immediate() );    return;}
			if (op == BNE) {branch( !ZERO,Immediate() );           return;}    //Branch on Not Equal
			if (op == CPX_Immediate) {compare( X,Immediate() );    return;}
			if (op == BEQ) {branch( ZERO,Immediate() );            return;}    //Branch on EQual
		}
		printf("!!! OP0 FEHLER !!!");
		exitflag = TRUE;
		return;
	}

	if ((op & 0x0f) == 1){
		if ((op & 0x80) == 0){
			if (op == ORA_IndirectX) {ora( IndirectX() ); return;}
			if (op == ORA_IndirectY) {ora( IndirectY() ); return;}
			if (op == AND_IndirectX) {And( IndirectX() ); return;}
			if (op == AND_IndirectY) {And( IndirectY() ); return;}
			if (op == EOR_IndirectX) {eor( IndirectX() ); return;}
			if (op == EOR_IndirectY) {eor( IndirectY() ); return;}
			if (op == ADC_IndirectX) {adc( IndirectX() ); return;}
			if (op == ADC_IndirectY) {adc( IndirectY() ); return;}
		}else{
			if (op == STA_IndirectX) {mem_write( IndirectX_adr(),A  ); return;}
			if (op == STA_IndirectY) {mem_write( IndirectY_adr(),A  ); return;}
			if (op == LDA_IndirectX) {lda( IndirectX() );          return;}
			if (op == LDA_IndirectY) {lda( IndirectY() );          return;}
			if (op == CMP_IndirectX) {compare( A,IndirectX() );    return;}
			if (op == CMP_IndirectY) {compare( A,IndirectY() );    return;}
			if (op == SBC_IndirectX) {sbc( IndirectX() );          return;}
			if (op == SBC_IndirectY) {sbc( IndirectY() );          return;}
		}
		printf("!!! OP1 FEHLER !!!");
		exitflag = TRUE;
		return;
	}

	
	// if (op & 0x0f) == 2
	if (op == LDX_Immediate) {ldx( Immediate() ); return;}


	//# if (op & 0x0f) == 3:


	if ((op & 0x0f) == 4){
		if (op == BIT_ZeroPage)  {bit( ZeroPage()  );          return;}
		if (op == STY_ZeroPage)  {mem_write( ZeroPage_adr() ,Y  ); return;}
		if (op == STY_ZeroPageX) {mem_write( ZeroPageX_adr(),Y  ); return;}
		if (op == LDY_ZeroPage)  {ldy( ZeroPage()  );          return;}
		if (op == LDY_ZeroPageX) {ldy( ZeroPageX() );          return;}
		if (op == CPY_ZeroPage)  {compare( Y,ZeroPage()  );    return;}
		if (op == CPX_ZeroPage)  {compare( X,ZeroPage()  );    return;}

		printf("!!! OP4 FEHLER !!! %x",op);
		exitflag = TRUE;
		return;
	}


	if ((op & 0x0f) == 5){
		if ((op & 0x80) == 0){
			if (op == ORA_ZeroPage)  {ora( ZeroPage()  ); return;}
			if (op == ORA_ZeroPageX) {ora( ZeroPageX() ); return;}
			if (op == AND_ZeroPage)  {And( ZeroPage()  ); return;}
			if (op == AND_ZeroPageX) {And( ZeroPageX() ); return;}
			if (op == EOR_ZeroPage)  {eor( ZeroPage()  ); return;}
			if (op == EOR_ZeroPageX) {eor( ZeroPageX() ); return;}
			if (op == ADC_ZeroPage)  {adc( ZeroPage()  ); return;}
			if (op == ADC_ZeroPageX) {adc( ZeroPageX() ); return;}
		}else{
			if (op == STA_ZeroPage)  {mem_write( ZeroPage_adr() ,A  ); return;}
			if (op == STA_ZeroPageX) {mem_write( ZeroPageX_adr(),A  ); return;}
			if (op == LDA_ZeroPage)  {lda( ZeroPage()  );          return;}
			if (op == LDA_ZeroPageX) {lda( ZeroPageX() );          return;}
			if (op == CMP_ZeroPage)  {compare( A,ZeroPage()  );    return;}
			if (op == CMP_ZeroPageX) {compare( A,ZeroPageX() );    return;}
			if (op == SBC_ZeroPage)  {sbc( ZeroPage()  );          return;}
			if (op == SBC_ZeroPageX) {sbc( ZeroPageX() );          return;}
		}
		printf("!!! OP5 FEHLER !!!");
		exitflag = TRUE;
		return;
	}


	if ((op & 0x0f) == 6){
		if ((op & 0x80) == 0){
			if (op == ASL_ZeroPage)  {asl( ZeroPage_adr()  ); return;}
			if (op == ASL_ZeroPageX) {asl( ZeroPageX_adr() ); return;}
			if (op == ROL_ZeroPage)  {rol( ZeroPage_adr()  ); return;}
			if (op == ROL_ZeroPageX) {rol( ZeroPageX_adr() ); return;}
			if (op == LSR_ZeroPage)  {lsr( ZeroPage_adr()  ); return;}
			if (op == LSR_ZeroPageX) {lsr( ZeroPageX_adr() ); return;}
			if (op == ROR_ZeroPage)  {ror( ZeroPage_adr()  ); return;}
			if (op == ROR_ZeroPageX) {ror( ZeroPageX_adr() ); return;}
		}else{
			if (op == STX_ZeroPage)  {mem_write( ZeroPage_adr() ,X  ); return;}
			if (op == STX_ZeroPageY) {mem_write( ZeroPageY_adr(),X  ); return;}
			if (op == LDX_ZeroPage)  {ldx( ZeroPage()  );          return;}
			if (op == LDX_ZeroPageY) {ldx( ZeroPageY() );          return;}
			if (op == DEC_ZeroPage)  {dec( ZeroPage_adr()  );      return;}
			if (op == DEC_ZeroPageX) {dec( ZeroPageX_adr() );      return;}
			if (op == INC_ZeroPage)  {inc( ZeroPage_adr()  );      return;}
			if (op == INC_ZeroPageX) {inc( ZeroPageX_adr() );      return;}
		}
		printf("!!! OP6 FEHLER !!!");
		exitflag = TRUE;
		return;
	}

	//if (op & 0x0f) == 7:


	if ((op & 0x0f) == 8){
		if ((op & 0x80) == 0){
			if (op == PHP) {pushstatus();                  return;}
			if (op == CLC) {CARRY=FALSE;                   return;}
			if (op == PLP) {popstatus();                   return;}
			if (op == SEC) {CARRY=TRUE;                    return;}
			if (op == PHA) {push( A );                     return;}
			if (op == CLI) {INTERRUPT=FALSE;               return;}
			if (op == PLA) {A = pop(); transfer_test( A ); return;}
			if (op == SEI) {INTERRUPT=TRUE;                return;}
		}else{
			if (op == DEY) {dey();                   return;}
			if (op == TYA) {A=Y; transfer_test( A ); return;}
			if (op == TAY) {Y=A; transfer_test( A ); return;}
			if (op == CLV) {OVERFLOW=FALSE;          return;}
			if (op == INY) {iny();                   return;}
			if (op == CLD) {DECIMAL=FALSE;           return;}
			if (op == INX) {inx();                   return;}
			if (op == SED){
				DECIMAL=TRUE;
				printf("!!! SED (set DECIMAL) not impl. !!!");
				exitflag=TRUE;
				return;
			}
		}
		printf("!!! OP8 FEHLER !!!");
		exitflag = TRUE;
		return;
	}

	if ((op & 0x0f) == 9){
		if ((op & 0x80) == 0){        
			if (op == ORA_Immediate) {ora( Immediate() ); return;}
			if (op == ORA_AbsoluteY) {ora( AbsoluteY() ); return;}
			if (op == AND_Immediate) {And( Immediate() ); return;}
			if (op == AND_AbsoluteY) {And( AbsoluteY() ); return;}
			if (op == EOR_Immediate) {eor( Immediate() ); return;}
			if (op == EOR_AbsoluteY) {eor( AbsoluteY() ); return;}
			if (op == ADC_Immediate) {adc( Immediate() ); return;}
			if (op == ADC_AbsoluteY) {adc( AbsoluteY() ); return;}
		}else{
			if (op == STA_AbsoluteY) {mem_write( AbsoluteY_adr(),A  ); return;}
			if (op == LDA_Immediate) {lda( Immediate() );          return;}
			if (op == LDA_AbsoluteY) {lda( AbsoluteY() );          return;}
			if (op == CMP_Immediate) {compare( A,Immediate() );    return;}
			if (op == CMP_AbsoluteY) {compare( A,AbsoluteY() );    return;}
			if (op == SBC_Immediate) {sbc( Immediate() );          return;}
			if (op == SBC_AbsoluteY) {sbc( AbsoluteY() );          return;}
		}
		printf("!!! OP9 FEHLER !!!");
		exitflag = TRUE;
		return;
	}
  
 
	if ((op & 0x0f) == 0x0a){
		if ((op & 0x80) == 0){
			if (op == ASL_Accumulator) {asl( Accumulator() ); return;}
			if (op == ROL_Accumulator) {rol( Accumulator() ); return;}
			if (op == LSR_Accumulator) {lsr( Accumulator() ); return;}
			if (op == ROR_Accumulator) {ror( Accumulator() ); return;}
		}else{
			if (op == TXA) {A=X; transfer_test( A );  return;}
			if (op == TXS) {SP=X;                     return;}
			if (op == TAX) {X=A; transfer_test( A );  return;}
			if (op == TSX) {X=SP; transfer_test( X ); return;}
			if (op == DEX) {dex();                    return;}
			if (op == NOP) {                          return;}
		}
		printf("!!! OPa FEHLER !!!");
		exitflag = TRUE;
		return;
	}

	//if (op & 0x0f) == 0x0b:


	if ((op & 0x0f) == 0x0c){
		if (op == BIT_Absolute) {bit( Absolute()  );          return;}
		if (op == JMP_Absolute) {PC = mem_readpc_word();          return;} 
		if (op == JMP_Indirect) {PC = Indirect_adr();         return;}
		if (op == STY_Absolute) {mem_write( Absolute_adr() ,Y  ); return;}
		if (op == LDY_Absolute) {ldy( Absolute()  );          return;}
		if (op == LDY_AbsoluteX) {ldy( AbsoluteX() );         return;}
		if (op == CPY_Absolute) {compare( Y,Absolute()  );    return;}
		if (op == CPX_Absolute) {compare( X,Absolute()  );    return;}
		
		printf("!!! OPc FEHLER !!!");
		exitflag = TRUE;
		return;
	}

	if ((op & 0x0f) == 0x0d){
		if ((op & 0x80) == 0){
			if (op == ORA_Absolute)     {ora( Absolute()  ); return;}
			if (op == ORA_AbsoluteX)    {ora( AbsoluteX() ); return;}
			if (op == AND_Absolute)     {And( Absolute()  ); return;}
			if (op == AND_AbsoluteX)    {And( AbsoluteX() ); return;}
			if (op == EOR_Absolute)     {eor( Absolute()  ); return;}
			if (op == EOR_AbsoluteX)    {eor( AbsoluteX() ); return;}
			if (op == ADC_Absolute)     {adc( Absolute()  ); return;}
			if (op == ADC_AbsoluteX)    {adc( AbsoluteX() ); return;}
		}else{
			if (op == STA_Absolute)  {mem_write( Absolute_adr() ,A  ); return;}
			if (op == STA_AbsoluteX) {mem_write( AbsoluteX_adr(),A  ); return;}
			if (op == LDA_Absolute)  {lda( Absolute()  );          return;}
			if (op == LDA_AbsoluteX) {lda( AbsoluteX() );          return;}
			if (op == CMP_Absolute)  {compare( A,Absolute()  );    return;}
			if (op == CMP_AbsoluteX) {compare( A,AbsoluteX() );    return;}
			if (op == SBC_Absolute)  {sbc( Absolute()  );          return;}
			if (op == SBC_AbsoluteX) {sbc( AbsoluteX() );          return;}
		}
		printf("!!! OPd FEHLER !!!");
		exitflag = TRUE;
		return;
	}


	if ((op & 0x0f) == 0x0e){
		if ((op & 0x80) == 0){
			if (op == ASL_Absolute)  {asl( Absolute_adr()  ); return;}
			if (op == ASL_AbsoluteX) {asl( AbsoluteX_adr() ); return;}
			if (op == ROL_Absolute)  {rol( Absolute_adr()  ); return;}
			if (op == ROL_AbsoluteX) {rol( AbsoluteX_adr() ); return;}
			if (op == LSR_Absolute)  {lsr( Absolute_adr()  ); return;}
			if (op == LSR_AbsoluteX) {lsr( AbsoluteX_adr() ); return;}
			if (op == ROR_Absolute)  {ror( Absolute_adr()  ); return;}
			if (op == ROR_AbsoluteX) {ror( AbsoluteX_adr() ); return;}
		}else{
			if (op == STX_Absolute)  {mem_write( Absolute_adr() ,X  ); return;}
			if (op == LDX_Absolute)  {ldx( Absolute()  );          return;}
			if (op == LDX_AbsoluteY) {ldx( AbsoluteY() );          return;}
			if (op == DEC_Absolute)  {dec( Absolute_adr()  );      return;}
			if (op == DEC_AbsoluteX) {dec( AbsoluteX_adr() );      return;}
			if (op == INC_Absolute)  {inc( Absolute_adr()  );      return;}
			if (op == INC_AbsoluteX) {inc( AbsoluteX_adr() );      return;}
		}
		printf("!!! OPe FEHLER !!!\n");
		exitflag = TRUE;
		return;
	}

	//if (op & 0x0f) == 0x0f:

	printf("!!! OP FEHLER (ende)!!! %x %i\n",op,PC);
	exitflag = TRUE;
	return;
}


// ---
void serial_io(char *in, char *out ){
    // serielle io informationen
    
	if (PC==0xED09){
		// send TALK an DEVICE A
		LISTEN_TALK = TALK;
		PC = 0xEDAC; // RTS
	}

	if (PC==0xED0C){
		//sent LISTEN an DEVICE A
		if (A==4) errorausgabe_flag=TRUE;   //error ausgabe ein
		if (A==8) {errorausgabe_flag=FALSE; }   //error ausgabe aus   
		if (A==9) {
			save_flag=TRUE;
			exitflag=TRUE;
		}
		LISTEN_TALK = LISTEN;
		MESSAGE_LEN=0;
		PC = 0xEDAC; // RTS
	}

	if (PC==0xEDB9){
		// Send a Secondary Address A to a Device on the Serial Bus after LISTEN
		SEK_ADRESS[0] = A>>4;
		SEK_ADRESS[1] =	A & 0x0f;
		PC = 0xEDC6;  //RTS
	}

	if (PC==0xEDC7){
		// Send a Secondary Address A to a Device on the Serial Bus after TALK
		SEK_ADRESS[0] = A>>4;
		SEK_ADRESS[1] =	A & 0x0f;
		PC = 0xEDDC;  //RTS
	}

	if (PC==0xEDDD){
		// Send a Byte(X) to an I/O Device over the Serial Bus  (X aus zwischenspeicher ram[0x95] gelesen)
		if (errorausgabe_flag){
			if (A==13){     // wenn enter
				if (MESSAGE_LEN>=0){
					MESSAGE[MESSAGE_LEN]=0;
					if ( strncmp("     PROGRAMM:",MESSAGE,5+9)==0 ) 
						printf( "     PROGRAMM: %s",in );
					else if ( strncmp("     CODEAUSGABE:",MESSAGE,5+11)==0 )
						if (MESSAGE[5+11+2]=='+') printf( "     CODEAUSGABE: %s", out );
						else {
							MESSAGE[5+11+2]=tolower(MESSAGE[5+11+2]);
							printf( "%.19s%s",MESSAGE, out );
						}
					else printf("%s\n",MESSAGE);	//BasicBoss aufgaben
					MESSAGE_LEN=-1;
				}
			}
			if (A==92) MESSAGE[MESSAGE_LEN]='_';  //fehlerausgabe pfund
			if (A!=92 && A!=13) MESSAGE[MESSAGE_LEN]=poke2asc[A];  // fehlerausgabe
		}else{
			MESSAGE[MESSAGE_LEN]=poke2asc[A];
			if (SAVE_STATE!=NONE && !errorausgabe_flag)
			{
				int pos = SAVE_LEN[SAVE_STATE];
				SAVE_FILE[SAVE_STATE][pos]=A;
				SAVE_LEN[SAVE_STATE]++;
			}
		}
		
		MESSAGE_LEN++;    
		ram[0x95] = A;  // Byte A in X zwischenspeichern
		PC = 0xEDEE;  //RTS
	}
  
      
	if (PC==0xEDEF){
		// Send UNTALK to a Device on the Serial Bus
		LISTEN_TALK = NONE;
		PC = 0xEE9F; // RTS
	}

	
	if (PC==0xEDFE){
		// letztes Byte mit EOI senden
		
		if (strncmp("+TEST",MESSAGE,5)==0) {            // ab dieser Nachricht geginnt das schreiben
			if (SAVE_STATE!=NONE) SAVE_LEN[SAVE_STATE] -= 5; 
			SAVE_STATE = NORMAL;
		}
		
		if (strncmp("R+TEST",MESSAGE,6)==0) {         // ab dieser Nachricht geginnt das schreiben des RoutineSegments
			if (SAVE_STATE!=NONE) SAVE_LEN[SAVE_STATE] -= 6; 
			SAVE_STATE = ROUT;
		}
		
		if (strncmp("D+TEST",MESSAGE,6)==0) {         // ab dieser Nachricht geginnt das schreiben des DatenSegments
			if (SAVE_STATE!=NONE) SAVE_LEN[SAVE_STATE] -= 6;
			SAVE_STATE = DATA;
		}

		if (strncmp("P+TEST",MESSAGE,6)==0) {         //  ab dieser Nachricht geginnt das schreiben des ProgrammSegments
			if (SAVE_STATE!=NONE) SAVE_LEN[SAVE_STATE] -= 6;
			SAVE_STATE = PROG;
		}

		MESSAGE[MESSAGE_LEN]=0;
		//MESSAGE[20]=0; printf("MESSAGE: %s---\n",MESSAGE);  // zum testen
		MESSAGE_LEN = 0;
		LISTEN_TALK = NONE;
		PC = 0xEE9F;  // RTS
	}
	return;
}


// --- hauptprg ---

void emu(char* in, char* out){
	int x,pos;
	char name[110] = "x";
	strncat(name,out,100);           // filename fuer die Segmente

	init_ram_rom(in);

	ram[1000]=123; ram[1001]=234;     //auto on
	ram[1002]=8;   // geräte nummer
	ram[1003]=1;   // sek. gerätenummer
	ram[1004]=4;   // Name länge
	ram[1005]='T'; ram[1006]='E'; ram[1007]='S'; ram[1008]='T';   //name
	
	ram[900]=9;   // geräte nummer
	ram[901]=1;   // sek. gerätenummer
	ram[902]=4;   // Name länge
	ram[903]='E'; ram[904]='N'; ram[905]='D'; ram[906]='E';   //name

	ram[0x164a]=NOP;       //Abbruch bei automatik auf press shift umleiten
	ram[0x164b]=NOP;
    
	ram[0x1777]=NOP;       //press shift übergehen
	ram[0x1778]=NOP;

	for(x=0; x<500000000; x++){

		//serielle zugiffe abfangen
		serial_io(in, out);
		
		//prg laden
		if (PC==0xEE13){      // receive byte
			A = LOAD_FILE[LOAD_POS];    // gelesenes Byte
			LOAD_POS++;
			if (LOAD_POS == LOAD_MAX){
				ram[0x90] = ram[0x90] | (1<<6);                 //EOI senden
				LOAD_POS = 0;                                   //fertig für pass2
			}
			PC = 0xEE84; // RTS
		} 

		//asm befehl auführen
		opcode();
		if (exitflag){
			if (save_flag==TRUE){
				// compilat speichern
				FILE *prg;
				
				printf("---------------------------------------\n");

				if (SAVE_LEN[NORMAL]>0) { 
					prg = fopen(out,"wb+"); for(pos=0; pos <SAVE_LEN[NORMAL]; pos++) putc(SAVE_FILE[NORMAL][pos],prg); fclose(prg);	//speichern
					printf("%-13s:%5i bytes , $%.4x bytes     (+2 AdressBytes)\n", out, SAVE_LEN[NORMAL]-2, SAVE_LEN[NORMAL]-2);	
				}
				if (SAVE_LEN[ROUT]>0) {
					name[0] = 'r';
					prg = fopen( name ,"wb+"); for(pos=0; pos <SAVE_LEN[ROUT]; pos++) putc(SAVE_FILE[ROUT][pos],prg); fclose(prg);     // RoutineSegment speichern
					printf("%-13s:%5i bytes , $%.4x bytes     (+2 Adressbytes)\n", name, SAVE_LEN[ROUT]-2, SAVE_LEN[ROUT]-2);		
				}
				if (SAVE_LEN[DATA]>0) { 
					name[0] = 'd';
					prg = fopen( name,"wb+"); for(pos=0; pos <SAVE_LEN[DATA]; pos++) putc(SAVE_FILE[DATA][pos],prg); fclose(prg);     // DatenSegment speichern
					printf("%-13s:%5i bytes , $%.4x bytes     (+2 Adressbytes)\n", name, SAVE_LEN[DATA]-2, SAVE_LEN[DATA]-2);		
				}
				if (SAVE_LEN[PROG]>0) {
					name[0] = 'p';
					prg = fopen( name,"wb+"); for(pos=0; pos <SAVE_LEN[PROG]; pos++) putc(SAVE_FILE[PROG][pos],prg); fclose(prg);     //ProgrammSegment speichern
					printf("%-13s:%5i bytes , $%.4x bytes     (+2 Adressbytes)\n", name, SAVE_LEN[PROG]-2, SAVE_LEN[PROG]-2);		 
				}

				//printf("(Compilat ist bei Quelltextfehlern nur eingeschraenkt funktionsfaehig.)\n",out);
			}else{
				printf("Interner Fehler: kein Compilat geschrieben \n");
				printf("Bitte das orginal BasicBoss mit z.B. WinVice nutzen. \n");
			}	
			return;
		}
	}

	printf("\nBASIC-BOSS abgebrochen. (wegen Endlosschleife)\n");
	printf("Bitte das orginal BasicBoss mit z.B. WinVice nutzen. \n");
	return;
}


int main(int argc, char* argv[])
{
	char* out = "+out.prg";
	//char* in = "basicdemo.prg";

	if ((argc==1)){
		printf ("USE: bbcompiler infile [outfile]\n");
		printf (" Compilat heisst '+out.prg' wenn kein outfile angegeben wird.\n");
		printf (" version: 0.2.2 (Basic-Boss V2.4)\n");
		printf ("\n Wichtig:\n  Erste Zeile im Quelltext muss 'PfundzeichenPROTOCOL' lauten,\n  damit Fehler ausgegeben werden koennen!\n");
		printf ("\n Wenn infile kein Basicprogramm ist wird nur eine leere Huelle erzeugt,\n  aber keine Fehlermeldung ausgegeben.\n");
	}else{ 
		if ( argc==3) out=argv[2];		
		emu(argv[1],out);
	}


	return 0;
}




