/**********************************************************/
/**				Pontificia Universidad Católica del Perú   		 **/
/**		   							Sistemas Digitales			 				   **/
/**********************************************************/
/**	Archivo: Nokia5110.c				 												 **/
/**	Alumnos:Natalia Muñiz Herrera			 									 **/
/**		Genaro Junior Chuqui Sosa    	         						 **/ 
/**	Microcontrolador: TM4C123GH6PM			 								 **/
/**	Programa Principal  				 											 	 **/
/**	Descripción: 																				 **/
/** Programa que simula un juego similar a un juego de 	 **/
/**	Ping Pong, donde habrán dos jugadores que mediante   **/
/**	el uso de los SW1 y SW2 de la tarjeta TivaLaunchPad  **/
/** y el uso del teclado de la computadora pueda mover   **/
/** una paletita que será, parecida a una de ping pong   **/
/** y tratarán de evitar que esta caiga fuera de ella    **/
/** la pelotita se irá moviendo de un lado a otro de la  **/
/** pantalla. Gana el jugador que acumule 5 puntos.      **/
/** Se gana un punto cuando la pelotita cae fuera de la  **/
/** paletita del jugador contrario. 										 **/
/**	Fecha: Julio, 2020				 													 **/
/**********************************************************/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TivaEs.h"

#define TRUE 1 
#define FALSE 0 

#define IZQUIERDO 0 
#define DERECHO 1 


/**********************************************************/
/*****  								CONSTANTES 							      *****/
/**********************************************************/


uint8_t xP1 = 0, yP11, yP12, xP2, yP21, yP22, xPp, yPp, jugador, lado;


//Configuración del SYSTICK a 500ms 
void ConfigurarSYSTICK_500ms(void)
{
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
	/* TiempoDeseado=500ms; FreqCPU=16MHZ
	* valor que debe ir en el registro RELOAD=X
	* X=(TiempoDeseado)*(FreqCPU) -1
	* X= [500ms] * [16MHz] -1 =7999999
	*15999999 en hexadecimal es 007A11FF*/
	NVIC_ST_RELOAD_R = (NVIC_ST_RELOAD_R&0xFF000000)|0x007A11FF;
	// Iniciamos el contador con cero (escribiendo cualquier valor)
	NVIC_ST_CURRENT_R &= ~(0x00FFFFFF);
	// Habilitamos el módulo SysTick y a su vez la interrupcion 
	NVIC_ST_CTRL_R |= (NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE);
}

void JuegoP1(){
	xP1 = 0;
	yP11 = 2;
	yP12 = 3;
	xP2 = 9;
	yP21 = 2;
	yP22 = 3;
	
	xPp = 1;
	yPp = 0;
	
	lado = IZQUIERDO;
	
	Nokia5110_SetCursor(xP1,yP11);
	Nokia5110_OutChar('|');
	Nokia5110_SetCursor(xP1,yP12);
	Nokia5110_OutChar('|');
	
	Nokia5110_SetCursor(xP2,yP21);
	Nokia5110_OutChar('|');
	Nokia5110_SetCursor(xP2,yP22);
	Nokia5110_OutChar('|');
	
	Nokia5110_SetCursor(xPp,yPp);
	Nokia5110_OutChar('O');
}

void JuegoP2(){
	xP1 = 0;
	yP11 = 2;
	yP12 = 3;
	xP2 = 9;
	yP21 = 2;
	yP22 = 3;
	xPp = 8;
	yPp = 2;
	
	lado = DERECHO; 
	
	Nokia5110_SetCursor(xP1,yP11);
	Nokia5110_OutChar('|');
	Nokia5110_SetCursor(xP1,yP12);
	Nokia5110_OutChar('|');
	
	Nokia5110_SetCursor(xP2,yP21);
	Nokia5110_OutChar('|');
	Nokia5110_SetCursor(xP2,yP22);
	Nokia5110_OutChar('|');
	
	Nokia5110_SetCursor(xPp,yPp);
	Nokia5110_OutChar('O');
}

void Iniciajuego(uint8_t jugador){
	if(jugador == 1 ){
		JuegoP1();
	}else if (jugador == 2){
		JuegoP2();
	}
}


void MoverPelotita(uint8_t lado){
	if(lado == IZQUIERDO){
		while(yPp > 0){
			Nokia5110_SetCursor(xPp, yPp);
			Nokia5110_OutChar(' ');
			xPp ++;
			yPp --; 
			Nokia5110_SetCursor(xPp, yPp);
			Nokia5110_OutChar('O');	
		}
		while(yPp < 5 && xPp < 9){
			Nokia5110_SetCursor(xPp, yPp);
			Nokia5110_OutChar(' ');
			xPp ++;
			yPp ++; 
			Nokia5110_SetCursor(xPp, yPp);
			Nokia5110_OutChar('O');				
		}
		while(xPp < 9 ){
			Nokia5110_SetCursor(xPp, yPp);
			Nokia5110_OutChar(' ');
			xPp ++;
			yPp --; 
			Nokia5110_SetCursor(xPp, yPp);
			Nokia5110_OutChar('O');				
		}
		lado = DERECHO; 
	}
}


int main (void){
	uint8_t estado;
	Nokia5110_Inicializa();
	TivaEs_Inicializa();
	
	
	Iniciajuego(1);
	MoverPelotita(IZQUIERDO);
	
//Se muestra la pantalla Principal del juego 	
	Nokia5110_Clear();
	Nokia5110_OutString(" Ping Pong");
	Nokia5110_SetCursor(0,2);
	Nokia5110_OutString(" Continue");
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString(" Press SW1");
	Nokia5110_SetCursor(0,5);
	Nokia5110_OutString("P1      P2");

	while(1){
		if ((GPIO_PORTF_DATA_R & 0x11) == SW2){ // Se espera que se presione el SW1
			while((GPIO_PORTF_DATA_R & 0x11) == SW2);
			Nokia5110_Clear();//Limpiamos la pantalla 
			
			
			
			

		}
	}
}




