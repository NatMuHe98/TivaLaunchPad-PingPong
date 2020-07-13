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

#ifndef _TIVAES_H_
#define _TIVAES_H_

/**********************************************************/
/*****  								CONSTANTES 							      *****/
/**********************************************************/
#define SW1 0x00000010
#define SW2 0x00000001

/*****              CONSTANTES BOOLEANAS              *****/
#ifndef FALSE 
	#define FALSE 0
#endif
#ifndef TRUE 
	#define TRUE  1
#endif
#ifndef LOW 
	#define LOW   0
#endif 
#ifndef HIGH 
	#define HIGH  1
#endif 

/**********************************************************/
/*****  								FUNCIONES 							      *****/
/**********************************************************/

/**********************************************************/
/****************TivaEs_Inicializa()***********************/
/**********************************************************/
/** 	Funciones para el control del led RGB y de los 		 **/
/**		pulsadores SW1 y SW2 															 **/
/**********Puertos de entrada:PF0, PF4 ********************/
/**********Puertos de salida:LED RGB***********************/
/**********************************************************/

void TivaEs_Inicializa(void);

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

uint8_t TivaES_LeePulsador(uint8_t maskSW);

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

uint8_t TivaES_SondeaPulsador(uint8_t maskSW, uint8_t *antNivelPtr);

/***********************************************************/
/**		  					  TivaEs_PresAndSolt()	 					  	  **/
/***********************************************************/
/** 	Evaluaremos si uno de los pulsadores a sido    		  **/
/**		presionado y soltado															  **/
/***********************************************************/
/**		Entradas: Máscara del Switch a leer (SW1 o SW2)		  **/ 
/**		Salidas: Ninguna 					 												  **/ 
/***********************************************************/

void TivaEs_PresAndSolt(uint8_t maskSW);

#endif
