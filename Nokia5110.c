/**********************************************************/
/**				Pontificia Universidad Católica del Perú   		 **/
/**		   							Sistemas Digitales			 				   **/
/**********************************************************/
/**	Archivo: Nokia5110.c				 												 **/
/**	Alumnos:Natalia Muñiz Herrera			 									 **/
/**		Genaro Junior Chuqui Sosa    	         						 **/ 
/**	Microcontrolador: TM4C123GH6PM			 								 **/
/**	Módulo (s) uC: SIS0 				 											 	 **/
/**     Obtenido de: 				         										 **/
/**	https://github.com/EngMahmoudMagdy/c4game        		 **/
/**	Fecha: Julio, 2020				 													 **/
/**********************************************************/
/**	Funciones para pra el uso de la pantalla 	 					 **/
/**	LCD Nokia 5110, mediante el TivaLaunchPad        		 **/
/**	Mediante la interfaz Serial Síncrona (SIS0)	 				 **/
/**********************************************************/
/**	Conexiones: 					 															 **/
/**		PA7 - RST - Reset - pin1		 											 **/
/**		PA3 - CE - SIS0Fss - pin2		 											 **/
/**		PA6 - DC - Data/Command	- pin3		 								 **/
/**		PA5 - DIN - SIS0Tx - pin4		 											 **/
/**		PA2 - CLK - SIS0Clk - pin5		 										 **/
/**		PA4 - BL - no conectado			 											 **/
/**********************************************************/
/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
 
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"

/**********************************************************/
/*****  								FUNCIONES 							      *****/
/**********************************************************/

/**********************************************************/
/**		  				Nokia5110_typeOfWrite() 								 **/
/**********************************************************/
/**	Nos servirá para definir el tipo de salida       		 **/
/** 	de datos, ya sea DATOS o comandos para LCD	 			 **/
/**********************************************************/

enum Nokia5110_typeOfWrite{
  COMMAND,                           // la transmisión es un comando LCD
  DATA                               // la transmisión son datos
};

/**********************************************************/
/**		 							 Nokia5110_lcdwrite()			 					 **/
/**********************************************************/
/** 	Función auxiliar para poder enviar un mensaje	 		 **/
/**		de 8 bits a la pantalla LCD 			 							   **/
/**********************************************************/
/**	Entradas: COMMAND o DATA, mensaje de 8 bits 	 			 **/ 
/**	Salidas: Ninguna 				 														 **/ 
/**********************************************************/

void static Nokia5110_lcdwrite(enum Nokia5110_typeOfWrite type, char message){
  if(type == COMMAND){
                                        // Espere hasta que SSI0 no esté ocupado / transmita FIFO vacío
    while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY);
    DC = DC_COMMAND;
    SSI0_DR_R = message;                // salida de comando 
                                        // Espere hasta que SSI0 no esté ocupado / transmita FIFO vacío
    while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY);
  } else{
    while((SSI0_SR_R&SSI_SR_TNF)==0); //espere hasta que la transmisión FIFO no esté llena
    DC = DC_DATA;
    SSI0_DR_R = message;                //salida de datos 
  }
}

/**********************************************************/
/**		  					Nokia5110_Inicializa()		 						 **/
/**********************************************************/
/** 	Nos permite inicializar los puertos de la 	 			 **/
/**		tarjeta TivaLaunchPad que nos permitirán  	 			 **/
/**		controlar la pantalla LCD Nokia5110 					 		 **/ 
/**********************************************************/
/**		Entradas: Ninguna			 	 													 **/ 
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/
void Nokia5110_Inicializa(void){
  volatile unsigned long delay;
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;  // activa SSI0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activa port A
  delay = SYSCTL_RCGC2_R;               // espera la señal de reloj que se termine de activar 
  GPIO_PORTA_DIR_R |= 0xC0;             // colocamos PA6,7 como salidas
  GPIO_PORTA_AFSEL_R |= 0x2C;           // habilitamos las funciones alternas PA2,3,5
  GPIO_PORTA_AFSEL_R &= ~0xC0;          // deshabilitamos funciones alternas en PA6,7
  GPIO_PORTA_DEN_R |= 0xEC;             // habilitamos las funciones digitales PA2,3,5,6,7
                                        // configuramos PA2,3,5 como
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;
                                        // configuramos PA6,7 como GPIO 
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0x00FFFFFF)+0x00000000;
  GPIO_PORTA_AMSEL_R &= ~0xEC;          // deshabilitamos funciones analógicas PA2,3,5,6,7
  SSI0_CR1_R &= ~SSI_CR1_SSE;           // deshabilitamos SSI
  SSI0_CR1_R &= ~SSI_CR1_MS;            // deshabilitamos modo master 
                                        // configurar para el reloj del sistema / fuente de reloj en baudios PLL
  SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL;
 					 // divisor de reloj para 3.33 MHz SSIClk (80 MHz PLL / 24)
                                         // SysClk / (CPSDVSR * (1 + SCR))
                                         // 80 / (24 * (1 + 0)) = 3.33 MHz (más lento que 4 MHz)

  SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+24; // debe ser un número par
  SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (3.33 Mbps velocidad de datos)
                  SSI_CR0_SPH |         // SPH = 0
                  SSI_CR0_SPO);         // SPO = 0
                                        // FRF = Formato de escala libre
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
                                        // DSS = 8-bit data
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;
  SSI0_CR1_R |= SSI_CR1_SSE;            // habilitamos SSI

  RESET = RESET_LOW;                    // reseteamos LCD 
  for(delay=0; delay<10; delay=delay+1);// delay minimo 100 ns
  RESET = RESET_HIGH;                   // lógica negativa

  Nokia5110_lcdwrite(COMMAND, 0x21);              // chip activo; modo de direccionamiento horizontal (V = 0); usar conjunto de instrucciones extendido (H = 1)
                                        	  // configure LCD Vop (contraste), que puede requerir algunos ajustes:
  Nokia5110_lcdwrite(COMMAND, CONTRAST);          // pruebe 0xB1 (para 3.3V rojo SparkFun), 0xB8 (para 3.3V azul SparkFun), 0xBF si su pantalla es demasiado oscura, o 0x80 a 0xFF si experimenta
  Nokia5110_lcdwrite(COMMAND, 0x04);              // establecer coeficiente de temperatura
  Nokia5110_lcdwrite(COMMAND, 0x14);              // Modo de polarización de LCD 1:48: intente 0x13 o 0x14

  Nokia5110_lcdwrite(COMMAND, 0x20);              // debemos enviar 0x20 antes de modificar el modo de control de visualización
  Nokia5110_lcdwrite(COMMAND, 0x0C);              // establecer el control de visualización en modo normal: 0x0D para inversa
}

/**********************************************************/
/**	   							Nokia5110_OutChar()		 							 **/
/**********************************************************/
/** 	Imprime un caracter en la Nokia 5110 LCD  				 **/
/**		el caractér se imprimirá en la posición  	 				 **/
/**		actual del cursor, el cual se actualizará	 				 **/
/**		automáticamente. 	 	 		 													 **/ 
/**********************************************************/
/**		Entradas: Caracter a imprimir 		 	 							 **/ 
/**		Salidas: Ninguna 																	 **/ 
/**********************************************************/
void Nokia5110_OutChar(unsigned char data){
  int i;

  Nokia5110_lcdwrite(DATA, 0x00);         
  for(i=0; i<5; i=i+1){
    Nokia5110_lcdwrite(DATA, ASCII[data - 0x20][i]); //Se imprime el caracter 
  }
	  Nokia5110_lcdwrite(DATA, 0x00);
	  Nokia5110_lcdwrite(DATA, 0x00);
}


/**********************************************************/
/**		Nokia5110_OutString()			 												 **/
/**********************************************************/
/** 	Imprime un caracter en la Nokia 5110 LCD  	 			 **/
/**	el caractér se imprimirá en la posición  	 					 **/
/**	actual del cursor, el cual se actualizará	 					 **/
/**	automáticamente. 	 	 		 														 **/ 
/**********************************************************/
/**	Entradas: Caracter a imprimir 		 	 								 **/ 
/**	Salidas: Ninguna 				 														 **/ 
/**********************************************************/

void Nokia5110_OutString(char *ptr){
  while(*ptr){
    Nokia5110_OutChar((unsigned char)*ptr);
    ptr = ptr + 1;
  }
}

/**********************************************************/
/**		Nokia5110_OutUDec()			 													 **/
/**********************************************************/
/** 	Imprime un número de 16 bits decimal sin   	 			 **/
/**		signo, con un tamaño fijo, justificado a   	 			 **/
/**		la derecha 					 															 **/
/**********************************************************/
/**		Entradas: Número sin signo de 16 bits 	 	 				 **/ 
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/
void Nokia5110_OutUDec(unsigned short n){
  if(n < 10){
    Nokia5110_OutChar(n+'0'); // n entre 0 y 9  
  } else if(n<100){
    Nokia5110_OutChar(n/10+'0'); // decimales 
    Nokia5110_OutChar(n%10+'0'); // enteros 
  } else if(n<1000){
    Nokia5110_OutChar(n/100+'0'); // cien digitos 
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); // 10 digitos
    Nokia5110_OutChar(n%10+'0'); // 1 digito 
  }
  else if(n<10000){
    Nokia5110_OutChar(n/1000+'0'); // mil 
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); // ciens
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); // decenas 
    Nokia5110_OutChar(n%10+'0'); //enteros 
  }
  else {
    Nokia5110_OutChar(n/10000+'0'); // diez mil  
    n = n%10000;
    Nokia5110_OutChar(n/1000+'0'); // miles 
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); // cientos 
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); // decenas  
    Nokia5110_OutChar(n%10+'0'); // enteros 
  }
}



/**********************************************************/
/**		  					Nokia5110_SetCursor()								 	 **/
/**********************************************************/
/** 	Mueve el cursos a la posición X,Y desea 	 				 **/
/**		y el siguiente caracter se escribe en esa 	 			 **/
/**		posición, X=0 columna izquierda, 		 							 **/
/**		posición, Y=0 fila superior. 	 		 								 **/ 
/**********************************************************/
/**		Entradas: newX, posición en X (0-11)	 	 					 **/ 
/**		  				newY, posición en Y (0-5)		 						 **/
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/
void Nokia5110_SetCursor(unsigned char newX, unsigned char newY){
  if((newX > 11) || (newY > 5)){        // mala entrada
    return;                             // no hacemos nada 
  }
// multiplica newX por 7 porque cada caracter tiene 7 columnas de ancho
  Nokia5110_lcdwrite(COMMAND, 0x80|(newX*7));     // el bit de configuración 7 actualiza la posición X
  Nokia5110_lcdwrite(COMMAND, 0x40|newY);         // el bit de configuración 6 actualiza la posición Y
}

/**********************************************************/
/**		  						Nokia5110_Clear()			 						   **/
/**********************************************************/
/** 	Limpia la LCD escribiendo ceros en la  		 				 **/
/**		pantalla y resetenado el cursor a (0,0) 	 				 **/
/**********************************************************/
/**		Entradas: Ninguna 			 	 												 **/ 		
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/
void Nokia5110_Clear(void){
  int i;
  for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
    Nokia5110_lcdwrite(DATA, 0x00); //Se limpia la pantalla
  }
  Nokia5110_SetCursor(0,0); //Colocamos el cursor en (0,0)
}

/**********************************************************/
/**	        			Nokia5110_DrawFullImage()		 					 **/
/**********************************************************/
/** 	Llena toda la pantalla, dibujando una  		 				 **/
/**		imagen de mapa de bits de 48x84		 	 							 **/
/**********************************************************/
/**		Entradas: Puntero ptr a mapa de bits 504bytes 	 	 **/ 		
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/
void Nokia5110_DrawFullImage(const char *ptr){
  int i;
  Nokia5110_SetCursor(0, 0);
  for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){ //Recorremos el arreglo almacenado el ptr
    Nokia5110_lcdwrite(DATA, ptr[i]); //Imprimimos la imagen en pantalla 
  }
}

char Screen[SCREENW*SCREENH/8]; //buffer almacena la siguiente imagen que se imprimirá en la pantalla

/**********************************************************/
/**	         			  Nokia5110_PrintBMP()			 					 **/
/**********************************************************/
/** 	Imprime en pantalla una imagen en una  					   **/
/**		posición definida, además de con una 	 	 					 **/
/**		cierta escala de colores. Toma un mapa de bits   	 **/
/**		y pone sus datos de imagen en la ubicacion 	 			 **/
/**		del búffer, la imagen aparecerá en la 		 				 **/
/**		siguiente llamada a Nokia5110_DisplayBuffer();	 	 **/
/**********************************************************/
/**		Entradas:xpos posicion horizontal, valor 	 			   **/
/**	 	 				 inferior a 84, 0 está a la izquierda 	 	 **/
/**		 				 82 cerca de la derecha 		 							 **/
/**		 				 Puntero ptr a mapa de bits 504bytes 	 		 **/ 
/**		 				 ypos posicion vertical, valor 		 				 **/
/**		 				 inferior a 48, 2 cerca de la parte 	 		 **/
/**		 				 superior, 47 de la parte inferior	 			 **/
/**		 				 ptr es el puntero a la imagen BPM       	 **/
/**		 				 threshold es la escala de gris 	 				 **/
/**		 				 con un valor del 0 a 14 		 							 **/
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/

void Nokia5110_PrintBMP(unsigned char xpos, unsigned char ypos, const unsigned char *ptr, unsigned char threshold){
  long width = ptr[18], height = ptr[22], i, j;
  unsigned short screenx, screeny;
  unsigned char mask;
  // check for clipping
  if((height <= 0) ||              // el mapa de bits se codifica inesperadamente en orden de píxeles de arriba a abajo
     ((width%2) != 0) ||           // debe ser un número par de columnas
     ((xpos + width) > SCREENW) || // corte lado derecho 
     (ypos < (height - 1)) ||      // corte superior
     (ypos > SCREENH))           { //corte inferior
    return;
  }
  if(threshold > 14){
    threshold = 14;             //Solo lleno 'on' activa el pixel
  }
  // los mapas de bits están codificados al revés, así que comience en la esquina inferior izquierda de la imagen
  screeny = ypos/8;
  screenx = xpos + SCREENW*screeny;
  mask = ypos%8;                //fila 0 a 7
  mask = 0x01<<mask;            // ahora almacena una máscara de 0x01 a 0x80
  j = ptr[10];                  // el byte 10 contiene el desplazamiento donde se pueden encontrar datos de imagen
  for(i=1; i<=(width*height/2); i=i+1){
    // the left pixel is in the upper 4 bits
    if(((ptr[j]>>4)&0xF) > threshold){
      Screen[screenx] |= mask;
    } else{
      Screen[screenx] &= ~mask;
    }
    screenx = screenx + 1;
    // the right pixel is in the lower 4 bits
    if((ptr[j]&0xF) > threshold){
      Screen[screenx] |= mask;
    } else{
      Screen[screenx] &= ~mask;
    }
    screenx = screenx + 1;
    j = j + 1;
    if((i%(width/2)) == 0){    // al final de una fila
      if(mask > 0x01){
        mask = mask>>1;
      } else{
        mask = 0x80;
        screeny = screeny - 1;
      }
      screenx = xpos + SCREENW*screeny;
      // los mapas de bits están alineados con palabras de 32 bits
      switch((width/2)%4){      // omite cualquier relleno
        case 0: j = j + 0; break;
        case 1: j = j + 3; break;
        case 2: j = j + 2; break;
        case 3: j = j + 1; break;
      }
    }
  }
}

/**********************************************************/
/**	          		Nokia5110_ClearBuffer()								 **/
/**********************************************************/
/** 	Hay un buffer de RAM que contiene   		 					 **/
/**		una pantalla, borramos ese buffer	 	 							 **/
/**********************************************************/
/**		Entradas: Ninguno			 														 **/ 		
/**		Salidas: Ninguna 				 													 **/ 
/**********************************************************/

void Nokia5110_ClearBuffer(void){
	int i;
  for(i=0; i<SCREENW*SCREENH/8; i=i+1){
    Screen[i] = 0;              // clear buffer
  }
}

/**********************************************************/
/**	         		 Nokia5110_DisplayBuffer()		 					 **/
/**********************************************************/
/** 	Llena toda la pantalla, dibujando una imagen	 		 **/
/**********************************************************/
/**		Entradas: Ninguno			 	 													 **/ 		
/**		Salidas: Ninguna  				 												 **/ 
/**********************************************************/

void Nokia5110_DisplayBuffer(void){
  Nokia5110_DrawFullImage(Screen);
}
