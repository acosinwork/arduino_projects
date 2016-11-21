#include "iarduino_IR.h"
#ifndef iarduino_IR_TX_h
#define iarduino_IR_TX_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class iarduino_IR_TX{
	public:			iarduino_IR_TX				(uint8_t, bool=false);	//	подключение 							(№ вывода подключённого к ИК-светодиоду, инверсия сигнала для ИК-светодиода)
	/**	пользовательские функции **/
		void		begin						(void);					//	инициализация ИК-светодиода				(без параметра)
		void		send						(uint32_t, bool=false);	//	отправить пакет данных на светодиод		(данные, флаг передачи повторных пакетов при удержании)
		bool		protocol					(const char*);			//	указать  протокол передачи данных		(строка протокола)
	/**	переменные доступные для пользователя **/
		uint8_t		frequency					=	37;					//	несущая частота для передачи сигнала
		uint8_t		length						=	32;					//	длина данных информационного пакета		(количество информационных бит)
	private:
	/**	внутренние функции **/
		uint32_t	IRTX_func_CREATE_PACK_REPEAT(uint32_t=0);			//	создание пакета повтора					(данные)
		void		IRTX_func_DELAY				(bool, uint32_t);		//	передача сигнала на светодиод			(0/1, удержание)
		void		IRTX_func_SEND				(uint32_t, uint8_t);	//	передача пакета  на светодиод			(данные, длинна)
	/**	внутренние переменные **/
		uint8_t		IRTX_var_I					=	0;					//	переменная
		uint8_t		IRTX_var_J					=	0;					//	переменная
		uint8_t		IRTX_var_K					=	0;					//	переменная
		bool		IRTX_flag_START				=	1;					//	флаг наличия сигнала старт
		bool		IRTX_flag_STOP				=	1;					//	флаг наличия сигнала стоп
		bool		IRTX_flag_TOGGLE			=	0;					//	флаг наличия бита toggle/рестарт
		bool		IRTX_flag_TOGGLE_SEND		=	0;					//	флаг последнего состояния бита toggle
		bool		IRTX_flag_EVEN_SEND			=	0;					//	чётность последнего отправленного пакета
		uint8_t		IRTX_uint_CODING			=	IR_PAUSE_LENGTH;	//	установленная кодировка
		uint8_t		IRTX_uint_BIT_PULSE_MAX		=	12;					//	максимальная длительность импульса в мкс/50
		uint8_t		IRTX_uint_BIT_PULSE_MIN		=	11;					//	минимальная  длительность импульса в мкс/50
		uint8_t		IRTX_uint_BIT_PAUSE_MAX		=	31;					//	максимальная длительность паузы в мкс/50
		uint8_t		IRTX_uint_BIT_PAUSE_MIN		=	8;					//	минимальная  длительность паузы в мкс/50
		uint8_t		IRTX_uint_START_PULSE		=	179;				//	длительность импульса сигнала старт в мкс/50
		uint8_t		IRTX_uint_START_PAUSE		=	86;					//	длительность паузы сигнала старт в мкс/50
		uint8_t		IRTX_uint_STOP_PULSE		=	12;					//	длительность импульса сигнала стоп в мкс/50
		uint8_t		IRTX_uint_STOP_PAUSE		=	0;					//	длительность паузы сигнала стоп в мкс/50
		uint8_t		IRTX_uint_TOGGLE_PULSE		=	12;					//	длительность импульса бита toggle/рестарт в мкс/50
		uint8_t		IRTX_uint_TOGGLE_PAUSE		=	31;					//	длительность паузы бита toggle/рестарт в мкс/50
		uint8_t		IRTX_uint_TOGGLE_POSITION	=	60;					//	позиция бита toggle/рестарт в пакете
		uint8_t		IRTX_uint_PACK_PAUSE		=	20;					//	пауза между 1 и 2 пакетами в мс
		uint32_t	IRTX_uint_PACK_REPEAT		=	0;					//	значение или биты инверсии для отправки пакета повтора
		uint8_t		IRTX_uint_PACK_REPEAT_TYPE	=	2;					//	тип повторного пакета (0-нет, 1-с инверсными битами, 2-идентичен информационному, 3-уникален)
		uint32_t	IRTX_uint_DATA_PREVIOUS		=	0;					//	значение данных отправленных последними функцией send
		uint8_t		IRTX_uint_PACK_LENGTH_REPEAT=	32;					//	длина данных пакета повтора (количество информационных бит)
};
#endif
