/* Header Files */
#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <math.h>
/* define some constent */
typedef unsigned long DWORD;                                      // define "unsigned long" alias is DWORD                  
typedef unsigned int WORD;
typedef unsigned char BYTE;
#define MK_PDI (WORD)((bus << 8) | (device << 3) | (function))
#define PCI_ADDRESS (DWORD) (0x80000000 | (DWORD) MK_PDI << 8)  
#define PCI_ADDRESS_OFFSET (DWORD) (PCI_ADDRESS | (offset << 2))  //PCI address
#define PCI_CONFIG_INDEX_PORT 0xCF8                               //PCI index port
#define PCI_CONFIG_DATA_PORT 0xCFC                                //PCI data port
/* keyboard defination */
#define ESC   0x11B
#define UP    0x4800
#define DOWN  0x5000
#define LEFT  0x4B00
#define RIGHT 0x4D00
#define ENTER 0x1C0D
#define WAIT_PRESS bioskey (0)
#define F1 0x3B00
#define F2 0x3C00
#define F3 0x3D00
/* Function Declaration */
void PCI_write (int index, DWORD address);
DWORD PCI_read (int read);
void PCI_scan (void);
int PCI_scan_keyboard (void);
void PCI_read_in_word (int bus, int device, int function);
void PCI_read_in_byte (int bus, int device, int function);
void PCI_read_in_dword (int bus, int device, int function);
int PCI_read_keyboard (int k);
void PCI_modify (int length, int offset, int k);
/* Other */
int a [30], b [30], c [30];                                   //define a[],b[],c[] to store device's bus device function
int i;

/* main function */
int main (void) {
	PCI_scan ();
	PCI_scan_keyboard ();
	return 0;
}

/* PCI_scan Function */
void PCI_scan (void) {
	int bus, device, function;                               //define variates for PCI bus, device, function 
	DWORD data;                                              //define data to receive data from PCI config data port
	DWORD address;                                           //define address to write PCI address into PCI config index port
  system ("cls");
	printf("Bus   Device   Function   VDID");
	printf("\n");
  i = 1;                                                   //must be initialized here

	for (bus = 0; bus <= 0x63; ++bus) {
		for (device = 0; device <= 0x1F; ++device) {
			for (function = 0; function <= 0x7; ++function) {
				PCI_write (PCI_CONFIG_INDEX_PORT,PCI_ADDRESS);
				data = PCI_read (PCI_CONFIG_DATA_PORT);
				if ((WORD)data != 0xFFFF ) {
					i++;
					a [i] = bus;                                     //a[] receive bus device function values for global use
					b [i] = device;                                  
					c [i] = function;
					printf ("%02X    %02X       %02X         %X", bus, device, function, data);
					printf ("\n");
				}
			}
		}
	}

  printf ("\nEsc: quit");
  printf ("\nEnter: read device");
  printf ("\nUP_arrow and DOWN_arrow: select device");
}

/* PCI_write function */
void PCI_write (int index,DWORD data) {
	asm mov dx,index;
	asm lea bx,data;
	__emit__(
	0x66,0x50,                                               // push EAX
	0x66,0x8B,0x07,                                          // in EAX,DX
	0x66,0xEF,                                               // mov [BX],EAX
	0x66,0x58);                                              // pop EAX
}

/* PCI_read function */
DWORD PCI_read (int read) {
	DWORD data;
	asm mov dx,read;
	asm lea bx,data;
	__emit__(
	0x66,0x50,                                               // push EAX
	0x66,0xED,                                               // mov EAX,[BX]
	0x66,0x89,0x07,                                          // out DX,EAX
	0x66,0x58);                                              // pop EAX
	return data;
}


/* read PCI */
/* read PCI in byte */
void PCI_read_in_byte (int bus, int device, int function) {
	DWORD data;
	WORD data_0, data_1, data_2, data_3;                     //receive splited data from PCI_data
	int offset = 0;
	int i = 0;
  textcolor (GREEN);
	system ("cls");
	printf ("**************************************************\n");
	printf ("*********Bus: %02X Device: %02X Function: %02X**********\n", bus,device,function);
	printf ("**************************************************\n");
	cprintf ("   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
  printf ("\n");
  cprintf ("%X0 ", i);

	for (offset = 0; offset <= 0x3F; offset++) {
		PCI_write (PCI_CONFIG_INDEX_PORT,PCI_ADDRESS_OFFSET);
		data = PCI_read (PCI_CONFIG_DATA_PORT);
		data_0 = (BYTE) (data);                                //split PCI_data 
		data_1 = (BYTE) (data >> 8);
		data_2 = (BYTE) (data >> 16);
		data_3 = (BYTE) (data >> 24);
		printf ("%02X %02X %02X %02X ", data_0, data_1, data_2, data_3);
    if (offset < 63 && (offset+1) % 4 == 0) {
      i = i + 1;
      printf ("\n");
			cprintf ("%X0 ", i);
    }
	}

	printf ("\n\nF1: BYTE  F2: WORD  F3: DWORD  ESC: RETURN ENTER: MODIFY");
}

/* read PCI in word */
void PCI_read_in_word (int bus, int device, int function) {
	DWORD data;
	WORD data_0, data_1;
	int offset = 0;
	int i = 0;
  textcolor (GREEN);
	system ("cls");
	printf ("******************************************\n");
	printf ("***Bus: %02X   Device: %02X   Function: %02X****\n", bus,device,function);
	printf ("******************************************\n");
	cprintf ("   0100 0302 0504 0706 0908 0B0A 0D0C 0F0E");
  printf ("\n");
  cprintf ("%X0 ", i);

	for (offset = 0; offset <= 0x3F; offset++) {
		PCI_write (PCI_CONFIG_INDEX_PORT,PCI_ADDRESS_OFFSET);
		data = PCI_read (PCI_CONFIG_DATA_PORT);
		data_0 = (WORD) (data);                                 //low 16 bit data
		data_1 = (WORD) (data >> 16);                           //high 16 bit data
		printf ("%04X %04X ", data_0, data_1);
		if (offset < 63 && (offset+1) % 4 == 0) {
      i = i + 1;
			printf ("\n");
			cprintf ("%X0 ", i);
    }
	}
	printf ("\n\nF1: BYTE  F2: WORD  F3: DWORD  ESC: RETURN ENTER: MODIFY");
}

/* read PCI in dword */
void PCI_read_in_dword (int bus, int device, int function) {
	DWORD data;
	int offset = 0;
	int i = 0;
  textcolor (GREEN);
	system ("cls");
	printf ("**************************************\n");
	printf ("  Bus: %02X   Device: %02X   Function: %02X\n", bus,device,function);
	printf ("**************************************\n");
	cprintf ("   03020100 07060504 0B0A0908 0F0E0D0C");
  printf ("\n");
  cprintf ("%X0 ", i);

	for (offset = 0; offset <= 0x3F; offset++) {
		PCI_write (PCI_CONFIG_INDEX_PORT,PCI_ADDRESS_OFFSET);
		data = PCI_read (PCI_CONFIG_DATA_PORT);
		printf ("%08lX ", data);
		if (offset < 63 && (offset+1) % 4 == 0) {
      i = i + 1;
			printf ("\n");
			cprintf ("%X0 ", i);
    }
	}

	printf ("\n\nF1: BYTE  F2: WORD  F3: DWORD  ESC: RETURN ENTER: MODIFY");
}


/* keyboard */
/* PCI_scan_keyboard Function */
int PCI_scan_keyboard (void) {
	int x = 1, y = 2;

	for (;;) {
		gotoxy (x, y);
		switch (WAIT_PRESS) {
			case ESC:
				return 0;
			case UP:
				if (y > 2 && y <= i)
					y = y - 1;
				break;
			case DOWN:
				if (y > 1 && y < i)
					y = y + 1;
				break;
			case ENTER:
				PCI_read_in_byte (a [y], b [y], c [y]);
				PCI_read_keyboard (y);
				PCI_scan ();
				x = 1;
				y = 2;
				break;

		}
	}

}

/* PCI_read_keyboard */
int PCI_read_keyboard (int k) {    //k receive
	int x = 4, y = 5;
	int x_start_limit = 4, y_start_limit = 5;
	int x_end_limit = 49, y_end_limit = 20;
	int x_change = 3, y_change = 1;
	int length = 2;  //byte,word,dword character length
	int offset = 0;
	int x_offset_change = 1;

	for (;;) {
		gotoxy (1, 4);
    printf ("%02X",offset);
		gotoxy (x, y);
		switch (WAIT_PRESS) {
			case ESC:
        return 0;

			case UP:
				if (y > y_start_limit && y <= y_end_limit) {
					y = y - y_change;
          offset = offset - 16;
        }
				break;
			case DOWN:
				if (y >= y_start_limit && y < y_end_limit) {
					y = y + y_change;
          offset = offset + 16;
        }
        break; 
      case LEFT:
        if (x > x_start_limit && x <= x_end_limit) {
          x = x - x_change;
          offset = offset - x_offset_change;
        }
				break;
			case RIGHT:
				if (x >= x_start_limit && x < x_end_limit) {
					x = x + x_change;
          offset = offset + x_offset_change;
        }
				break;

			case F1:
				PCI_read_in_byte (a [k], b [k], c [k]);
        x = 4;
        y = 5;
        x_end_limit = 49, y_end_limit = 20;
        x_change = 3, y_change = 1; 
        length = 2;
        offset = 0;
        x_offset_change = 1;
        break;
			case F2:
				PCI_read_in_word (a[k], b[k], c[k]); 
        x = 4;
        y = 5;
        x_end_limit = 39, y_end_limit = 20;
        x_change = 5, y_change = 1; 
        length = 4;
        offset = 0;
        x_offset_change = 2;
        break;
			case F3:
				PCI_read_in_dword (a [k], b[k], c[k]);
        x = 4;
        y = 5;
        x_end_limit = 31, y_end_limit = 20;
        x_change = 9, y_change = 1;
        length = 8; 
        offset = 0;
        x_offset_change = 4;
        break;

      case ENTER:       
        PCI_modify (length, offset, k);
        break;
    }
  }

}


/* Modify PCI */
void PCI_modify (int length, int offset, int k) {
  DWORD modify_input [10];                                 //define modify_input to receiive midified value
  DWORD input_change = 0;                                  //input_change receive that modify_input was transformed value
  DWORD data_read;                                         //data_read receive the value that get from PCI config data port
  DWORD data_change;                                       //data_change store the value witch is written into PCI
  int i = 1;
  int bus = a [k];
  int device = b [k];
  int function = c [k];
  int select_offset;                                       
  int input_judgement = 0;                                 //judgement condition whether to excute if()
	select_offset = offset % 4;
  offset = offset/4;

	for (i = 1; (modify_input [i] = getchar ()) != 10 && i <= length; i++) {       
    if (modify_input [i] >= 48 && modify_input [i] <=57) {
      modify_input [i] = (DWORD) ((modify_input [i] - 48) * pow (16, (length - i)));
      input_change = input_change + modify_input [i];
      input_judgement ++;
    } 
    else if (modify_input [i] >= 65 && modify_input [i] <= 70) {
      modify_input [i] = (DWORD) ((modify_input [i] - 55) * pow (16, (length - i))); 
      input_change = (input_change + modify_input [i]);
      input_judgement ++;
    }
  }

  if (length == 2)                                         //judge the length of PCI_data
    PCI_read_in_byte (bus, device, function); 
  else if (length == 4)
    PCI_read_in_word (bus, device, function);   
  else if (length == 8)
    PCI_read_in_dword (bus, device, function);
  
  i = i - 1;
  if (length == 2 && i == 2 && input_judgement == 2) {
    PCI_write (PCI_CONFIG_INDEX_PORT, PCI_ADDRESS_OFFSET);
    data_read = PCI_read (PCI_CONFIG_DATA_PORT);                          //read PCI data
    if (select_offset == 0)
      data_change = ((data_read & 0xFFFFFF00) | input_change);
    else if (select_offset == 1)
      data_change = ((data_read & 0xFFFF00FF) | (input_change << 8));
    else if (select_offset == 2)
      data_change = ((data_read & 0xFF00FFFF) | (input_change << 16));
    else
      data_change = ((data_read & 0x00FFFFFF) | (input_change << 24));
    PCI_write (PCI_CONFIG_INDEX_PORT, PCI_ADDRESS_OFFSET);
    PCI_write (PCI_CONFIG_DATA_PORT, data_change);
    PCI_read_in_byte (bus, device, function);     
  }
  if (length == 4 && i == 4 && input_judgement == 4) {
    PCI_write (PCI_CONFIG_INDEX_PORT, PCI_ADDRESS_OFFSET);
    data_read = PCI_read (PCI_CONFIG_DATA_PORT);
    if (select_offset == 0)
      data_change = ((data_read & 0xFFFF0000) | input_change);
    else
      data_change = ((data_read & 0x0000FFFF) | (input_change << 16));
    PCI_write (PCI_CONFIG_INDEX_PORT, PCI_ADDRESS_OFFSET);
    PCI_write (PCI_CONFIG_DATA_PORT, data_change);
    PCI_read_in_word (bus, device, function);     
  }
  if (length == 8 && i == 8 && input_judgement == 8) {
    PCI_write (PCI_CONFIG_INDEX_PORT, PCI_ADDRESS_OFFSET);
    data_read = PCI_read (PCI_CONFIG_DATA_PORT);
    data_change = ((data_read & 0x00000000) | input_change);
    PCI_write (PCI_CONFIG_INDEX_PORT, PCI_ADDRESS_OFFSET);
    PCI_write (PCI_CONFIG_DATA_PORT, data_change);
    PCI_read_in_dword (bus, device, function);    
  }

}



