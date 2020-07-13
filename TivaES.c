/**********************************************************/
/**				Pontificia Universidad Católica del Perú   		 **/
/**		   							Sistemas Digitales			 				   **/
/**********************************************************/
/**	Archivo: TivaES.c				 														 **/
/**	Alumnos:Natalia Muñiz Herrera			 									 **/
/**					Genaro Junior Chuqui Sosa        						 **/ 
/**	Microcontrolador: TM4C123GH6PM			 								 **/
/**	Módulo (s) uC: GPIO PORTF		 											 	 **/
/**	Fecha: Julio, 2020				 													 **/
/**********************************************************/
/**	Funciones para el control de los pulsadores					 **/
/**	SW1 y SW2 de la tarjeta de evaluación EK-TM4C123GXL	 **/
/**********************************************************/
/*****	Conexiones:                              			*****/
/*****          Led rojo        -       PF1           *****/
/*****          Led azul        -       PF2           *****/
/*****          Led verde       -       PF3						*****/
/*****          Pulsador SW1    -       PF4           *****/
/*****          Pulsador SW2    -       PF0  	 				*****/
/**********************************************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "TivaES.h"

/**********************************************************/
/****************TivaEs_Inicializa()*********************/
/**********************************************************/
/** 	Funciones para el control del led RGB y de los 		 **/
/**		pulsadores SW1 y SW2 															 **/
/**********Puertos de entrada:PF0, PF4 ********************/
/**********Puertos de salida:LED RGB***********************/
/**********************************************************/

void TivaEs_Inicializa(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Activar reloj PORTF
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0); //Se espera la activación
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B; //Desbloqueamos PF0
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_DIR_R |= 0x0E; //Se activan salidas
	GPIO_PORTF_DIR_R &= ~(0x11); //Se activan entradas
	GPIO_PORTF_PUR_R |= 0x11; //Activamos resistencias para SW1y SW2
	GPIO_PORTF_DR8R_R |= 0x0E; //Se activa 8mA para LED's
	GPIO_PORTF_AFSEL_R &= ~(0x1F); //Se desactivan funciones alternas
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFF00000)|0x00000000; //Se configuran pines para control 
	GPIO_PORTF_AMSEL_R &= ~(0x1F); //Se desactivan funciones analógicas
	GPIO_PORTF_DEN_R |= 0x1F; //Se activan funciones digitales
	GPIO_PORTF_DATA_R &= ~(0x0E); //Se inicia con LED's OFF
}

/***********************************************************/
/**		  					  TivaES_LeePulsador()		 						  **/
/***********************************************************/
/** 	Nos permite leer el estado actual del pulsador		  **/
/**		el cual nos dará un valor LOW en caso de estar		  **/
/**		presionado y HIGH si no lo está 		 					 		  **/ 
/***********************************************************/
/**		Entradas: Máscara del Switch a leer (SW1 o SW2)		  **/ 
/**		Salidas: estado (HIGH o LOW)											  **/ 
/***********************************************************/

uint8_t TivaES_LeePulsador(uint8_t maskSW){
	uint8_t estado;
	if((GPIO_PORTF_DATA_R & 0x11)!=maskSW){
		estado=LOW;
	}else{
		estado=HIGH;
	}
	return estado;
}

/***********************************************************/
/**		  					  TivaES_SondeaPulsador()	 						  **/
/***********************************************************/
/** 	Nos permite sondear el valor del pulsador      		  **/
/**		el cual nos dará un valor LOW en caso de estar		  **/
/**		presionado y HIGH si no lo está 		 					 		  **/ 
/***********************************************************/
/**		Entradas: Máscara del Switch a leer (SW1 o SW2)		  **/ 
/**		Salidas: estado (HIGH o LOW)											  **/ 
/***********************************************************/

uint8_t TivaES_SondeaPulsador(uint8_t maskSW, uint8_t *antNivelPtr){
	uint8_t flancoBajadaFlag, actNivel;
	
	actNivel = TivaES_LeePulsador(maskSW);
	flancoBajadaFlag = FALSE; 
	
	if((*antNivelPtr == HIGH)&&(actNivel == LOW)){
		flancoBajadaFlag = TRUE;
	}
	*antNivelPtr = actNivel;
	return flancoBajadaFlag;
}

/***********************************************************/
/**		  					  TivaEs_PresAndSolt()	 					  	  **/
/***********************************************************/
/** 	Evaluaremos si uno de los pulsadores a sido    		  **/
/**		presionado y soltado															  **/
/***********************************************************/
/**		Entradas: Máscara del Switch a leer (SW1 o SW2)		  **/ 
/**		Salidas: Ninguna 					 												  **/ 
/***********************************************************/

void TivaEs_PresAndSolt(uint8_t maskSW){
	if((GPIO_PORTF_DATA_R & 0x11)==maskSW){
		while((GPIO_PORTF_DATA_R & 0x11)==maskSW);
	}
}

