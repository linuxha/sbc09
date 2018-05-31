/* 6809 Simulator V09.

   Copyright 1994, L.C. Benschop, Eidnhoven The Netherlands.
   This version of the program is distributed under the terms and conditions
   of the GNU General Public License version 2. See the file COPYING.
   THERE IS NO WARRANTY ON THIS PROGRAM!!!
   
   This program simulates a 6809 processor.
   
   System dependencies: short must be 16 bits.
                        char  must be 8 bits.
                        long must be more than 16 bits.
                        arrays up to 65536 bytes must be supported.
                        machine must be twos complement.
   Most Unix machines will work. For MSODS you need long pointers
   and you may have to malloc() the mem array of 65536 bytes.
                 
   Define BIG_ENDIAN if you have a big-endian machine (680x0 etc)              
   
   Special instructions:                     
   SWI2 writes char to stdout from register B.
   SWI3 reads char from stdout to register B, sets carry at EOF.
               (or when no key available when using term control).
   SWI retains its normal function. 
   CWAI and SYNC stop simulator.
   
*/


#include <stdio.h>
#include <stdlib.h>

#define engine extern

#include "v09.h"

#include <string.h>

#define VERSION "v09 1.0.2"
char nom[4096];
extern int iopage;

FILE *tracefile;

void do_trace(void)
{
 Word pc=pcreg;
 Byte ir;
 fprintf(tracefile,"pc=%04x ",pc);
 ir=mem[pc++];
 fprintf(tracefile,"i=%02x ",ir);
 if((ir&0xfe)==0x10)
    fprintf(tracefile,"%02x ",mem[pc]);else fprintf(tracefile,"   ");
     fprintf(tracefile,"x=%04x y=%04x u=%04x s=%04x a=%02x b=%02x cc=%02x\n",
                   xreg,yreg,ureg,sreg,*areg,*breg,ccreg);
} 
 
void usage(void) {
    fprintf(stderr,"%s\nUsage: v09 [-t tracefile [-l addr] [-h addr] ] [-e escchar] [-f filenom.rom] [-a 0xE000 ] \nDefault filename is v09.rom\n", VERSION);
    exit(1); 
}

void version(void) {
    fprintf(stderr,"%s\n", VERSION);
    exit(1); 
}

// @TODO: read_nImage(nom) - better error message
void read_nImage(const char *nom) {
    FILE *image;

    fprintf(stderr, "Opening %s\n", nom);
    if((image = fopen(nom,"rb")) == NULL) {
	perror("v09, image file");
	exit(2);
    }

    // @TODO: Fix the load point
    // Does this always load the image at 0x8000 ?
    fread(mem+0x8000, 0x8000, 1, image);
    fclose(image);
}

read_image()
{
 FILE *image;
 if((image=fopen("v09.rom","rb"))==NULL) 
  if((image=fopen("../v09.rom","rb"))==NULL) 
   if((image=fopen("..\\v09.rom","rb"))==NULL) {
    perror("v09, image file");
    exit(2);
 }
 fread(mem+0x8000,0x8000,1,image);
 fclose(image);
}

#define CHECKARG if(i==argc)usage();else i++;

main(int argc,char *argv[])
{
 Word loadaddr=0x100;
 char *imagename=0;
 int i;
 escchar='\x1d'; 
 tracelo=0;tracehi=0xffff;

 strcpy(nom, "v09.rom");

 for(i=1;i<argc;i++) {
    if (strcmp(argv[i],"-t")==0) {
     i++;
     if((tracefile=fopen(argv[i],"w"))==NULL) {
         perror("v09, tracefile");
         exit(2);
     }
     tracing=1;attention=1;    
   } else if (strcmp(argv[i],"-tl")==0) {
     i++;
     tracelo=strtol(argv[i],(char**)0,0);
   } else if (strcmp(argv[i],"-th")==0) {
     i++;
     tracehi=strtol(argv[i],(char**)0,0);
   } else if (strcmp(argv[i],"-e")==0) {
     i++;
     escchar=strtol(argv[i],(char**)0,0);
   } else if (strcmp(argv[i],"-v") == 0) {
     i++;
     version();
   } else if (strcmp(argv[i],"-f") == 0) {
     i++;
     strcpy(nom, argv[i]);
   } else if (strcmp(argv[i],"-a")==0) {
     i++;
     iopage=strtol(argv[i],(char**)0,0);
   } else {
     usage();
   }
 }   
 #ifdef MSDOS
 if((mem=farmalloc(65535))==0) { 
   fprintf(stderr,"Not enough memory\n");
   exit(2);
 } 
 #endif
 read_nImage(nom); 
 set_term(escchar);
 pcreg=(mem[0xfffe]<<8)+mem[0xffff]; 
 interpr();
}

