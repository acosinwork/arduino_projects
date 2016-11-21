#ifndef iarduino_IR_h
#define iarduino_IR_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
								
#define	IR_UNDEFINED		0										//			тип кодирования не определён
#define	IR_PAUSE_LENGTH		1										//			кодирование длинной паузы
#define	IR_PULSE_LENGTH		2										//			кодирование шириной импульса (ШИМ)
#define	IR_BIPHASIC			3										//			бифазное кодирование
#define	IR_BIPHASIC_INV		4										//			бифазное кодирование с инверсными битами
#define	IR_NRC				5										//	NOKIA	бифазное кодирование с сигналом старт, пакеты повтора идентичны, первый и последний пакеты специальные
#define	IR_RS5				6										//	PHILIPS	бифазное кодирование с битом toggle (третий бит в пакете), пакеты повтора идентичны первому
#define	IR_RS5X				7										//	PHILIPS	бифазное кодирование с битом toggle (второй бит в пакете), пакеты повтора идентичны первому
#define	IR_RS6				8										//	PHILIPS	бифазное кодирование с битом toggle (пятый бит в пакете) и сигналом старт, пакеты повтора идентичны первому
#define	IR_CLEAN			255										//	сброс, ранее установленного, протокола передачи данных

#define	IR_INTERVAL_PACK	7										//	минимальный интервал между повторными пакетами в мс
#define	IR_INTERVAL_PRESS	150										//	минимальный интервал между нажатиями клавиш в мс

class iarduino_IR_VV{												//	класс volatile variable
	public:
	/**	переменные изменяемые в прерываниях **/
		volatile	uint8_t		IRRX_pins_NUM			=	0;		//	номер вывода к которому подключён ИК-приёмник
		volatile	uint8_t		IRTX_pins_NUM			=	0;		//	номер вывода к которому подключён ИК-светодиод
		volatile	uint8_t		IRRX_uint_READ_STATUS	=	0;		//	состояние приёма пакетов
																	//	0 - нет пакетов
																	//	1 - принимается первый пакет	3 - принимается второй пакет
																	//	2 - принят первый пакет			4 - принят второй пакет
																	//	5 - принимаются или приняты следующие пакеты
																	//	6 - пауза после последнего пакета превышает IR_INTERVAL_PRESS мс
		volatile	bool		IRTX_pins_SEND_STATUS	=	0;		//	состояние передачи несущей частоты (не передаётся/передаётся)
		volatile	bool		IRRX_pins_READ_DATA		=	0;		//	состояние на выводе к которому подключён ИК приёмник
		volatile	bool		IRTX_pins_SEND_DATA		=	0;		//	состояние на выводе к которому подключён светодиод (0/1)
		volatile	bool		IRRX_flag_CHECK			=	0;		//	флаг выполнения функции check (для реализации опции НЕреагирования на повторные пакеты)
		volatile	bool		IRRX_flag_KEY_PRESS		=	0;		//	флаг выполнения функции check (для установки флага key_press доступного пользователю)
		volatile	bool		IRRX_flag_READ_REPEAT	=	0;		//	флаг наличия повторного пакета старше второго (удерживается клавиша на пульте)
		volatile	bool		IRRX_flag_READ_PULSE	=	0;		//	флаг состояния сигнала в данный момент времени (1-PULSE/0-PAUSE)
		volatile	bool		IRRX_flag_READ_INVERT	=	0;		//	флаг инвертирования сигналов принимаемых от ИК приёмника
		volatile	bool		IRTX_flag_SEND_INVERT	=	0;		//	флаг инвертирования сигналов передаваемых на светодиод
		volatile	bool		IRXX_flag_SEND			=	0;		//	флаг передачи данных
		volatile	uint16_t	IRRX_uint_PACK_PAUSE	=	0;		//	пауза между 1 и 2 пакетами
		volatile	uint8_t		IRTX_uint_CALL_PAUSE	=	0;		//	пауза между вызовами функции send (в мкс/50)
		volatile	uint16_t	IRRX_uint_PACK_LENGTH	=	0;		//	длинна сигнала или паузы принимаемой в данный момент времени (в мкс/50)
		volatile	uint8_t		IRRX_uint_PACK_INDEX	=	0;		//	индекс в массиве с данными, в который сейчас записывается время
		volatile	uint8_t		IRRX_uint_PACK_NUM		=	0;		//	номер массива в который записывается пакет
		volatile	uint8_t		IRRX_uint_PACK_LEN[2]	=	{0,0};	//	длинна   массива с данными 1 и 2 пакета
		volatile	uint8_t		IRRX_mass_PACK[2][68];				//	массив с данными 1 и 2 пакета (длительность импульсов и паузы, в мкс/50)
};

class iarduino_IR_XX{												//	класс общих функций
	public:
					void		IRXX_func_TIMER2_SETREG(uint32_t);	//	установка значений регистров таймера под нужную частоту (частота в кГц)
					uint8_t		IRXX_func_DECODING(const char *i,uint8_t);
};

#endif