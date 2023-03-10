/************************************/
/*	Author	  :	ESLAM_HOSNY			*/
/*	SWC		  : MTWI				*/
/*  Layer	  : MCAL				*/
/*	Version	  :	1.0		  			*/
/*	Date	  :	January 1, 2023		*/
/*	Last Edit : N/A					*/
/************************************/
#include "LSTD_types.h"
#include "LBIT_math.h"
#include "MTWI_config.h"
#include "MTWI_private.h"
#include "MTWI_interface.h"

void MTWI_voidMasterInit(u8 ARG_u8Prescaler, u32 ARG_SCLFrequency)
{
	/* CLEAR And SET Prescaler Value by masking*/
	MTWI_TWSR &= 0b11111100;
	MTWI_TWSR |= ARG_u8Prescaler;
	/*SET The Value Of TWBR */
	switch(ARG_u8Prescaler)
	{
	case MTWI_PRECALER_1:	{MTWI_TWBR = (((F_CPU/ARG_SCLFrequency)-16)/(2*1));  break; }
	case MTWI_PRECALER_4:	{MTWI_TWBR = (((F_CPU/ARG_SCLFrequency)-16)/(2*4));  break;	}
	case MTWI_PRECALER_16:	{MTWI_TWBR = (((F_CPU/ARG_SCLFrequency)-16)/(2*16)); break;	}
	case MTWI_PRECALER_64:	{MTWI_TWBR = (((F_CPU/ARG_SCLFrequency)-16)/(2*64)); break;	}
	default: break; //error
	}
	/* SET Ack_bit And Enable TWI */
	MTWI_TWCR = (1<<TWCR_TWEA)|(1<<TWCR_TWEN);

}

ErrorState	MTWI_enuSlaveInit(u8 ARG_u8Address)
{
	ErrorState L_enuErrorState = MTWI_OK;
	if(127 >= ARG_u8Address)
	{
		/* CLEAR And SET SLAVE_ADDERSS Value */
		MTWI_TWAR &= 0b00000001;
		MTWI_TWAR |= ARG_u8Address<<1;
		/* SET Ack_bit And Enable TWI */
		MTWI_TWCR = (1<<TWCR_TWEA)|(1<<TWCR_TWEN);
	}
	else
	{
		L_enuErrorState = MTWI_INVALID_ADDRESS;
	}

	return L_enuErrorState;
}

void MTWI_voidEnable(void)
{
	/* Enable TWI */
	SET_BIT(MTWI_TWCR,TWCR_TWEN);
}

void MTWI_voidDisable(void)
{
	/* Disable TWI */
	CLEAR_BIT(MTWI_TWCR,TWCR_TWEN);
}

ErrorState MTWI_enuStartCondition(void)
{
	ErrorState L_enuErrorState = MTWI_OK;
	/* CLEAR Flag And Enable TWI And Start Condition */
	MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN)|(1<<TWCR_TWSTA);
	/* Waiting Until The Flag is SET (Polling) */
	while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
	/* Check For The start_ACK */
	if(MTWI_START_ACK != (MTWI_TWSR & 0xF8))
	{
		L_enuErrorState = MTWI_SC_ERROR;
	}

	return L_enuErrorState;
}

ErrorState MTWI_enuReStartCondition(void)
{
	ErrorState L_enuErrorState = MTWI_OK;
	/* CLEAR Flag And Enable TWI And Start Condition */
	MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN)|(1<<TWCR_TWSTA);
	/* Waiting Until The Flag is SET (Polling) */
	while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
	/* Check For The REstart_ACK */
	if(MTWI_RESTART_ACK != (MTWI_TWSR & 0xF10))
	{
		L_enuErrorState = MTWI_RSC_ERROR;
	}

	return L_enuErrorState;
}

void MTWI_voidStopCondition(void)
{
	/* CLEAR Flag And Stop Condition */
	MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWSTO)|(1<<TWCR_TWEN);
}

ErrorState MTWI_enuSendSlaveAddresswithR_W(u8 ARG_u8SlaveAddress, u8 ARG_u8R_W)
{
	ErrorState L_enuErrorState = MTWI_OK;
	/* Check For R/W Operation */
	if(MTWI_SLA_W == ARG_u8R_W)
	{
		/* SET The VAlue of Called Slave In TWDR(care of The R/W Bit (First Bit) That Be 0 by Shifting*/
		MTWI_TWDR = ARG_u8SlaveAddress << 1;
		CLEAR_BIT(MTWI_TWDR,0);
		/* CLEAR Flag And Enable TWI*/
		MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN);
		/* Waiting Until The Flag is SET (Polling) */
		while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
		/* Check For The SLAVE_ADD_W_ACK */
		if(MTWI_SLA_W_ACK != (MTWI_TWSR & 0xF8))
		{
			L_enuErrorState = MTWI_SEND_SLA_W_ERROR;
		}
	}
	else if(MTWI_SLA_R == ARG_u8R_W)
	{
		/* SET The VAlue of Called Slave In TWDR(care of The R/W Bit (First Bit)*/
		MTWI_TWDR = (ARG_u8SlaveAddress << 1);
		/*SET The First Bit That For Read */
		SET_BIT(MTWI_TWDR,0);
		/* CLEAR Flag And Enable TWI*/
		MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN);
		/* Waiting Until The Flag is SET (Polling) */
		while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
		/* Check For The SLAVE_ADD_R_ACK */
		if(MTWI_SLA_R_ACK != (MTWI_TWSR & 0xF8))
		{
			L_enuErrorState = MTWI_SEND_SLA_R_ERROR;
		}
	}
	else
	{
		L_enuErrorState = MTWI_SEND_SLA_RW_INVALID;
	}

	return L_enuErrorState;
}

ErrorState MTWI_enuSendDataByte(u8 ARG_u8DataByte)
{
	ErrorState L_enuErrorState = MTWI_OK;
	/*Write Byte To TWDR_Register*/
	MTWI_TWDR = ARG_u8DataByte;
	/* CLEAR Flag And Enable TWI*/
	MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	/* Waiting Until The Flag is SET (Polling) */
	while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
	/* Check For Master OR Slave Transmit_ACK */
	if((MTWI_MT_DBYTE_ACK != (MTWI_TWSR & 0xF8)) || (MTWI_ST_DBYTE_ACK != (MTWI_TWSR & 0xF8)))
	{
		L_enuErrorState = MTWI_SEND_DATA_ERROR;
	}

	return L_enuErrorState;
}

ErrorState MTWI_enuReadDataByte(u8 *ARG_u8DataByte, u8 ARG_u8ACK_STATE)
{
	ErrorState L_enuErrorState = MTWI_OK;
	/* Check Of NULL Pointer */
	if(NULL != ARG_u8DataByte)
	{
		if(MTWI_REC_WITH_SEND_ACK == ARG_u8ACK_STATE)
		{
			/* CLEAR Flag And Enable TWI*/
			MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN)|(1<<TWCR_TWEA);
			/* Waiting Until The Flag is SET (Polling) */
			while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
			/*Get The Data From TWDR*/
			*ARG_u8DataByte = MTWI_TWDR;
			/* Check For Master OR Slave Recive_ACK */
			if((MTWI_MR_DBYTE_ACK != (MTWI_TWSR & 0xF8)) || (MTWI_SR_DBYTE_ACK != (MTWI_TWSR & 0xF8)))
			{
				L_enuErrorState = MTWI_READ_DATA_ERROR;
			}
		}
		else if(MTWI_REC_WITH_SEND_NACK == ARG_u8ACK_STATE)
		{
			/*Disable The Acknoladge */
			CLEAR_BIT(MTWI_TWCR,TWCR_TWEA);
			/* CLEAR Flag And Enable TWI*/
			MTWI_TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN);
			/* Waiting Until The Flag is SET (Polling) */
			while((!GET_BIT(MTWI_TWCR,TWCR_TWINT)));
			/*Get The Data From TWDR*/
			*ARG_u8DataByte = MTWI_TWDR;
			/* Check For Master OR Slave Recive_ACK */
			if((MTWI_MR_DBYTE_NACK != (MTWI_TWSR & 0xF8)) || (MTWI_SR_DBYTE_NACK != (MTWI_TWSR & 0xF8)))
			{
				L_enuErrorState = MTWI_READ_DATA_ERROR;
			}
		}
		else
		{
			L_enuErrorState = MTWI_ACK_STATE_INVALID;
		}
	}
	else
	{
		L_enuErrorState = NULL_POINTER;
	}

	return L_enuErrorState;
}

