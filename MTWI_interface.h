/************************************/
/*	Author	  :	ESLAM_HOSNY			*/
/*	SWC		  : MTWI				*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	January 1, 2023		*/
/*	Last Edit : N/A					*/
/************************************/
#ifndef _MTWI_INTERFACE_H_
#define _MTWI_INTERFACE_H_

#ifndef F_CPU
#define F_CPU	1000000UL
#endif

#define MTWI_MASTER_MODE		0
#define MTWI_SLAVE_MODE			1

#define MTWI_SLA_W				0
#define MTWI_SLA_R				1

#define MTWI_REC_WITH_SEND_ACK	0
#define MTWI_REC_WITH_SEND_NACK	1

#define MTWI_PRECALER_1			0
#define MTWI_PRECALER_4			1
#define MTWI_PRECALER_16		2
#define MTWI_PRECALER_64		3

#define MTWI_START_ACK			0x08
#define MTWI_RESTART_ACK		0x10
#define MTWI_SLA_W_ACK			0x18
#define MTWI_SLA_W_NACK			0x20
#define MTWI_MT_DBYTE_ACK		0x28
#define MTWI_MT_DBYTE_NACK		0x30
#define MTWI_ARBITRITION_ACK	0x38
#define MTWI_SLA_R_ACK			0x40
#define MTWI_SLA_R_NACK			0x48
#define MTWI_MR_DBYTE_ACK		0x50
#define MTWI_MR_DBYTE_NACK		0x58
#define MTWI_SR_DBYTE_ACK		0x80
#define MTWI_SR_DBYTE_NACK		0x88
#define MTWI_ST_DBYTE_ACK		0xB8
#define MTWI_ST_DBYTE_NACK		0xC0

void		MTWI_voidMasterInit(u8 ARG_u8Prescaler, u32 ARG_SCLFrequency);

void		MTWI_voidStopCondition(void);

void 		MTWI_voidEnable(void);

void		MTWI_voidDisable(void);

ErrorState	MTWI_enuStartCondition(void);

ErrorState	MTWI_enuReStartCondition(void);

ErrorState	MTWI_enuSlaveInit(u8 ARG_u8Address);

ErrorState	MTWI_enuSendSlaveAddresswithR_W(u8 ARG_u8SlaveAddress, u8 ARG_u8R_W);

ErrorState	MTWI_enuSendDataByte(u8 ARG_u8DataByte);

ErrorState	MTWI_enuReadDataByte(u8 *ARG_u8DataByte, u8 ARG_u8ACK_STATE);


#endif
