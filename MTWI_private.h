/************************************/
/*	Author	  :	ESLAM_HOSNY			*/
/*	SWC		  : MTWI				*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	January 1, 2023		*/
/*	Last Edit : N/A					*/
/************************************/
#ifndef _MTWI_PRIVATE_H_
#define _MTWI_PRIVATE_H_


#define MTWI_TWCR		*((volatile u8*)0x56)
#define TWCR_TWIE		0
#define TWCR_TWEN		2
#define TWCR_TWWC		3
#define TWCR_TWSTO		4
#define TWCR_TWSTA		5
#define TWCR_TWEA		6
#define TWCR_TWINT		7


#define MTWI_TWSR		*((volatile u8*)0x21)
#define TWSR_TWPS0		0
#define TWSR_TWPS1		1

#define MTWI_TWAR		*((volatile u8*)0x22)
#define TWAR_TWGCE		0

#define MTWI_TWBR		*((volatile u8*)0x20)

#define MTWI_TWDR		*((volatile u8*)0x23)


#endif

