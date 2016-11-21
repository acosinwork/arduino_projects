#include "iarduino_IR.h"
#ifndef iarduino_IR_RX_h
#define iarduino_IR_RX_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class iarduino_IR_RX{
	public:			iarduino_IR_RX					(uint8_t, bool=true);			//	подключение 						(№ вывода подключённого к ИК-приемнику, инверсия сигнала с ИК-приёмника)
	/**	пользовательские функции **/
		void		begin							(void);							//	инициализация ИК-приёмника			(без параметра)
		bool		check							(bool=false);					//	проверка наличия принятого пакета	(реагировать на пакеты повтора)
		bool		protocol						(const char*);					//	указать  протокол передачи данных	(строка протокола)
		char*		protocol						(void);							//	получить протокол передачи данных	(без параметра)
		int			protocol						(int);							//	получить протокол передачи данных	(номер получаемых данных или IR_CLEAN для сброса, ранее установленного, протокола передачи данных)
	/**	переменные доступные для пользователя **/
		uint32_t	data							=	0;							//	переменная для чтения данных
		uint8_t		length							=	0;							//	переменная для чтения длинны данных (количество информационных бит)
		bool		key_press						=   0;							//  флаг указвающий на то, что кнопка пульта нажимается а не удерживается
	private:
	/**	внутренние функции **/
		uint8_t		IRRX_func_PROTOCOL				(void);							//	определение протокола передачи данных
		bool		IRRX_func_DECODE				(uint8_t);						//	раскодирование в IRRX_uint_DATA		(номер пакета начиная с 0)
		bool		IRRX_func_DECODE_BIPHASIC		(uint8_t, bool);				//	декодирование бифазного кода		(номер пакета начиная с 0, true если _/¯ = 1  ¯\_ = 0   false если _/¯ = 0  ¯\_ = 1 )
		void		IRRX_func_DECODE_BIPHASIC_SHIFT	(bool);							//	сдвиг данных на 1 бит				(true - влево, false - вправо)
		bool		IRRX_func_CHECK_NRC				(void);							//	проверка протокола NRC
		uint32_t	IRRX_func_CREATE_PACK_REPEAT	(void);							//	создание повторного пакета для протокола
		bool		IRRX_func_COMPARE				(uint8_t, uint8_t, uint8_t);	//	сравнение чисел с указанием допуска	(число, число, допуск±)
	/**	внутренние переменные **/
		uint8_t		IRRX_var_I						=	0;							//	переменная
		uint8_t		IRRX_var_J						=	0;							//	переменная
		uint8_t		IRRX_var_K						=	0;							//	переменная
		char		IRRX_char_PROTOCOL[26];											//	строка с подробной информацией о протоколе
		bool		IRRX_flag_SET_PROTOCOL			=	0;							//	флаг установки протокола передачи данных пользователем
		bool		IRRX_flag_START					=	0;							//	флаг наличия сигнала старт
		bool		IRRX_flag_STOP					=	0;							//	флаг наличия сигнала стоп
		bool		IRRX_flag_TOGGLE				=	0;							//	флаг наличия бита toggle/рестарт
		uint8_t		IRRX_uint_CODING				=	0;							//	кодировка (определённый или установленный протокол передачи данных)
		uint8_t		IRRX_uint_FREQUENCY				=	37;							//	несущая частота (в кГц)
		uint8_t		IRRX_uint_BIT_PULSE_MAX			=	0;							//	максимальная длительность импульса в мкс/50
		uint8_t		IRRX_uint_BIT_PULSE_MIN			=	0;							//	минимальная  длительность импульса в мкс/50
		uint8_t		IRRX_uint_BIT_PAUSE_MAX			=	0;							//	максимальная длительность паузы в мкс/50
		uint8_t		IRRX_uint_BIT_PAUSE_MIN			=	0;							//	минимальная  длительность паузы в мкс/50
		uint8_t		IRRX_uint_START_PULSE			=	0;							//	длительность импульса сигнала старт в мкс/50
		uint8_t		IRRX_uint_START_PAUSE			=	0;							//	длительность паузы сигнала старт в мкс/50
		uint8_t		IRRX_uint_STOP_PULSE			=	0;							//	длительность импульса сигнала стоп в мкс/50
		uint8_t		IRRX_uint_STOP_PAUSE			=	0;							//	длительность паузы сигнала стоп в мкс/50
		uint8_t		IRRX_uint_TOGGLE_PULSE			=	0;							//	длительность импульса бита toggle/рестарт в мкс/50
		uint8_t		IRRX_uint_TOGGLE_PAUSE			=	0;							//	длительность паузы бита toggle/рестарт в мкс/50
		uint8_t		IRRX_uint_TOGGLE_POSITION		=	0;							//	позиция бита toggle/рестарт в пакете
		uint8_t		IRRX_uint_TOGGLE_PULSE_POS		=	0;							//	позиция импульса бита toggle/рестарт в пакете
		uint8_t		IRRX_uint_TOGGLE_PAUSE_POS		=	0;							//	позиция паузы бита toggle/рестарт в пакете
		uint8_t		IRRX_uint_TOGGLE_PULSE_SUM		=	0;							//	кол-во битов toggle/рестарт в пакете
		uint8_t		IRRX_uint_TOGGLE_PAUSE_SUM		=	0;							//	кол-во битов toggle/рестарт в пакете
		uint8_t		IRRX_uint_PACK_PAUSE			=	0;							//	пауза между 1 и 2 пакетами в мс
		uint32_t	IRRX_uint_PACK_REPEAT			=	0;							//	значение или биты инверсии для отправки пакета повтора
		uint8_t		IRRX_uint_PACK_REPEAT_TYPE		=	0;							//	тип повторного пакета (0-нет, 1-с инверсными битами, 2-идентичен информационному, 3-уникален)
		uint8_t		IRRX_uint_PACK_LENGTH			=	0;							//	установленная протоколом длинна данных 1 пакета в битах
		uint8_t		IRRX_uint_PACK_LENGTH_REPEAT	=	0;							//	установленная протоколом длинна данных 2 пакета в битах
		uint8_t		IRRX_uint_LENGTH				=	0;							//	длинна данных 1 или 2 пакета в битах (временная переменная, её значение присваивается переменным length или IRRX_uint_LENGTH_REPEAT)
		uint8_t		IRRX_uint_LENGTH_REPEAT			=	0;							//	длинна данных 2 пакета в битах
		uint32_t	IRRX_uint_DATA					=	0;							//	значение данных в 1 или 2 пакете (временная переменная, её значение присваивается переменным data или IRRX_uint_DATA_REPEAT)
		uint32_t	IRRX_uint_DATA_REPEAT			=	0;							//	значение данных во 2 пакете
		uint16_t	IRRX_mass_BIPHASIC_BIN[4];										//	массив с данными бифазного кода в двоичном виде
		uint8_t		IRRX_uint_PACK_LEN[2];											//	длинна массива с данными 1 и 2 пакета
		uint8_t		IRRX_mass_PACK[2][68];											//	массив с данными 1 и 2 пакета (длительность импульсов и паузы, в мкс/50)
};
#endif