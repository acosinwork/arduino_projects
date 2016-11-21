#include "iarduino_IR.h"
#include "iarduino_IR_RX.h"
#include "iarduino_IR_TX.h"

volatile	iarduino_IR_VV IRVV;
			iarduino_IR_XX IRXX;

			iarduino_IR_RX::iarduino_IR_RX(uint8_t i, bool j){IRVV.IRRX_pins_NUM=i; IRVV.IRRX_flag_READ_INVERT=j;}											//	сохраняем номер вывода к которому подключён выход ИК-приёмника и флаг инверсии сигнала с   ИК-приёмника
			iarduino_IR_TX::iarduino_IR_TX(uint8_t i, bool j){IRVV.IRTX_pins_NUM=i; IRVV.IRTX_flag_SEND_INVERT=j;}											//	сохраняем номер вывода к которому подключён вход  ИК-светодиод и флаг инверсии сигнала для ИК-светодиода

void		iarduino_IR_RX::begin(){pinMode(IRVV.IRRX_pins_NUM, INPUT); IRXX.IRXX_func_TIMER2_SETREG(20); IRVV.IRXX_flag_SEND=0;}							//	переводим вывод ИК-приёмника в режим приёма, устанавливаем таймер на частоту 20кГц, устанавливаем режим работы функции ISR в чтение
void		iarduino_IR_TX::begin(){pinMode(IRVV.IRTX_pins_NUM, OUTPUT); digitalWrite(IRVV.IRTX_pins_NUM, IRVV.IRTX_flag_SEND_INVERT); IRTX_flag_TOGGLE_SEND=0;}	//	переводим вывод ИК-светодиода в режим передачи, выводим 0 на светодиод (или 1 если установлен флаг инверсии), сбрасываем флаг последнего состояния отправленного бита toggle

//			проверка наличия принятого пакета
bool		iarduino_IR_RX::check(bool i){
//			отвечаем отсутствием новых не прочитанных пакетов
			if(!IRVV.IRRX_uint_READ_STATUS)	{return false;}																									//	нет принятых пакетов
			if( IRVV.IRRX_flag_CHECK)		{return false;}																									//	все принятые пакеты прочтены
//			ждём окончания получения данных
			if( IRVV.IRRX_uint_READ_STATUS<4){while(IRVV.IRRX_uint_READ_STATUS<4){}}																		//	принимаются первые пакеты, ждём...
//			сохраняем полученные данные (буферизируем)
			for(IRRX_var_I=0; IRRX_var_I<2;  IRRX_var_I++){																									//	проход по пакетам
			for(IRRX_var_J=0; IRRX_var_J<68; IRRX_var_J++){																									//	проход по длительностям полубитов
				IRRX_mass_PACK[IRRX_var_I][IRRX_var_J]=IRVV.IRRX_mass_PACK[IRRX_var_I][IRRX_var_J];															//	сохраняем данные пакетов
			}	IRRX_uint_PACK_LEN[IRRX_var_I]=IRVV.IRRX_uint_PACK_LEN[IRRX_var_I];																			//	сохраняем длину  пакетов
			}
			key_press=IRVV.IRRX_flag_KEY_PRESS?0:1; IRVV.IRRX_flag_KEY_PRESS=1;																				//	устанавливаем флаг key_press, если функция была выполнена для данного удержания кнопки пульта
//			запрещаем повторное чтение уже полученного пакета
			if(i){	if (IRVV.IRRX_uint_READ_STATUS==6)	{IRVV.IRRX_flag_CHECK=1;}																			//	если разрешего реагировать на пакеты повторов (i==1) и пакеты завершены (STATUS==6), устанавливаем флаг выполнения функции check (тогда функция вернёт false при следующем вызове, см. выше)
					if (IRVV.IRRX_flag_READ_REPEAT)	{IRVV.IRRX_flag_READ_REPEAT=0;}else{return false;}														//	если кнопка на пульте удерживается, продолжаем выполнение функции, иначе возвращаем false
			}else{		IRVV.IRRX_flag_CHECK=1;}																											//	если запрещено реагировать на пакеты повторов, то устанавливаем флаг выполнения функции check (тогда функция вернёт false при следующем вызове, см. выше)
//			сброс установленного протокола
			if(!IRRX_uint_CODING){IRRX_flag_SET_PROTOCOL=0;}																								//	если протокол передачи данных был удалён, то сбрасываем флаг установки протокола передачи данных пользователем
//			определяем протокол передачи данных, если он не указан пользователем
			if(!IRRX_flag_SET_PROTOCOL){IRRX_uint_CODING=IRRX_func_PROTOCOL();}
//			раскодируем принятые данные в соответствии с протоколом
			do{	if(IRRX_func_DECODE(0)){data=IRRX_uint_DATA; length=IRRX_uint_LENGTH;}else{return false;}													//	информационный пакет
				if(IRRX_func_DECODE(1)){IRRX_uint_DATA_REPEAT=IRRX_uint_DATA; IRRX_uint_LENGTH_REPEAT=IRRX_uint_LENGTH;}									//	повторный пакет
//				если протокол не указан пользователем и был определён как NRC, но не прошел проверку, то меняем протокол на IR_BIPHASIC и повторяем раскодирование
				IRRX_var_J=0; if(IRRX_uint_CODING==IR_NRC){if(!IRRX_flag_SET_PROTOCOL){if(!IRRX_func_CHECK_NRC()){IRRX_var_J=1; IRRX_uint_CODING==IR_BIPHASIC;}}}
			}	while(IRRX_var_J);
//			определяем тип повторного пакета, если протокол передачи данных не указан пользователем
			if(!IRRX_flag_SET_PROTOCOL){
				if(IRRX_uint_PACK_LEN[1]==0)										{IRRX_uint_PACK_REPEAT_TYPE=0;}else										//	повторного пакета нет
				if(IRRX_uint_CODING==IR_NRC)										{IRRX_uint_PACK_REPEAT_TYPE=3;}else										//	повторный пакет уникален
				if(length==IRRX_uint_LENGTH_REPEAT && data!=IRRX_uint_DATA_REPEAT)	{IRRX_uint_PACK_REPEAT_TYPE=1;}else										//	повторный пакет имеет инверсные биты
				if(length==IRRX_uint_LENGTH_REPEAT && data==IRRX_uint_DATA_REPEAT)	{IRRX_uint_PACK_REPEAT_TYPE=2;}else										//	повторный пакет идентичен информационному
																					{IRRX_uint_PACK_REPEAT_TYPE=3;}											//	повторный пакет уникален
				IRRX_uint_PACK_REPEAT=IRRX_func_CREATE_PACK_REPEAT();																						//	значение или биты инверсии для отправки пакета повтора
			}
//			игнорируем полученные данные (независимо от типа протокола)
			if(	length< 8)															{IRVV.IRRX_uint_READ_STATUS=0; return false;}							//	если длина данных информационного пакета меньше 1 байта
			if((length< IRRX_uint_LENGTH_REPEAT)&&IRRX_uint_PACK_LEN[1]>0)			{IRVV.IRRX_uint_READ_STATUS=0; return false;}							//	если длина данных информационного пакета меньше чем длина данных пакета повтора, при условии, что пакет повтора принят
			if((length!=IRRX_uint_PACK_LENGTH)  &&IRRX_flag_SET_PROTOCOL )			{IRVV.IRRX_uint_READ_STATUS=0; return false;}							//	если длина данных информационного пакета не равна установленной протоколом, при условии, что протокол был установлен
			return true;
}

//			отправка пакета данных на светодиод
void		iarduino_IR_TX::send(uint32_t i, bool j){
			IRTX_var_I=IRTX_uint_PACK_PAUSE<(IR_INTERVAL_PACK+2+(128000000/F_CPU))?(IR_INTERVAL_PACK+2+(128000000/F_CPU)):IRTX_uint_PACK_PAUSE;				//	определяем длительность задержки между пакетами
			uint32_t k=(IRTX_var_I-2-(128000000/F_CPU))*20;																									//	-2мс на задержки в функции IRXX_func_TIMER2_SETREG, -8мс для F_CPU=16МГц на установку регистров таймера в функции IRXX_func_TIMER2_SETREG, *20 преобразуем мс в мкс/50
			if(IRVV.IRTX_uint_CALL_PAUSE>10){j=false;}																										//	если пауза между вызовами данной функции больше 500мкс, то считаем что удержания не было
			if(IRTX_uint_DATA_PREVIOUS!=i){j=false;}																										//	если команда для передачи отличается от предыдущей команды, то считаем что удержания не было
			if(j){	IRTX_func_DELAY(0,(IRTX_uint_CODING==IR_NRC)?90:k);																						//	задержка перед повторным пакетом
					IRTX_flag_EVEN_SEND=!IRTX_flag_EVEN_SEND;																								//	определяем чётность пакета (1-чётный/0-нечётный)
					if(IRTX_uint_CODING==IR_NRC||(IRTX_uint_PACK_REPEAT_TYPE==1 && !IRTX_flag_EVEN_SEND)){													//	для NRC повторным является информационный пакет, а для пакетов с инверсными битами - каждый нечётный пакет отправляется без инверсии
										IRTX_func_SEND(i,length);																							//	отправка повторного пакета 
					}else{				IRTX_func_SEND(IRTX_func_CREATE_PACK_REPEAT(i),IRTX_uint_PACK_LENGTH_REPEAT);}										//	отправка повторного пакета
			}else{	IRTX_flag_TOGGLE_SEND=!IRTX_flag_TOGGLE_SEND;																							//	инвертируем состояние бита toggle
					IRTX_flag_EVEN_SEND=0;																													//	указываем, что отправлен нечётный пакет
					if(IRTX_uint_CODING==IR_NRC){	IRTX_func_SEND(IRTX_func_CREATE_PACK_REPEAT(i),IRTX_uint_PACK_LENGTH_REPEAT); IRTX_func_DELAY(0,k);}	//	для NRC перед информационным отправляется стартовый пакет
										IRTX_func_SEND(i,length);																							//	отправка информационного пакета
			}		IRTX_uint_DATA_PREVIOUS=i;																												//	сохраняем значение отправленной команды
}

//			загрузка протокола передачи данных
bool		iarduino_IR_RX::protocol(const char *i){
			for(IRRX_var_I=0; IRRX_var_I<25; IRRX_var_I++){if((i[IRRX_var_I] & 0xC0) != 0x40){return false;}}												//  возвращаем false если первые два бита любого символа в протоколе не равны "01"
			if(IRXX.IRXX_func_DECODING(i,0)>10){return false;}																								//  возвращаем false если тип кодировки больше 10
			IRRX_flag_SET_PROTOCOL		=	1;
			IRRX_uint_CODING			=	IRXX.IRXX_func_DECODING(i,0);																					//	тип кодировки
			IRRX_uint_FREQUENCY			=	IRXX.IRXX_func_DECODING(i,1);																					//	несущая частота
			IRRX_uint_PACK_LENGTH		=	IRXX.IRXX_func_DECODING(i,2);																					//	длина первого (информационного) пакета (бит)
			IRRX_uint_PACK_LENGTH_REPEAT=	IRXX.IRXX_func_DECODING(i,3);																					//	длина второго (повторного) пакета (бит)
			IRRX_uint_PACK_PAUSE		=	IRXX.IRXX_func_DECODING(i,4);																					//	длительность паузы между пакетами (мс)
			IRRX_uint_START_PULSE		=	IRXX.IRXX_func_DECODING(i,5);																					//	длительность импульса старт (мкс/50)
			IRRX_uint_START_PAUSE		=	IRXX.IRXX_func_DECODING(i,6);																					//	длительность паузы старт (мкс/50)
			IRRX_uint_STOP_PULSE		=	IRXX.IRXX_func_DECODING(i,7);																					//	длительность импульса стоп (мкс/50)
			IRRX_uint_STOP_PAUSE		=	IRXX.IRXX_func_DECODING(i,8);																					//	длительность паузы стоп (мкс/50)
			IRRX_uint_TOGGLE_PULSE		=	IRXX.IRXX_func_DECODING(i,9);																					//	длительность импульса рестарт или toggle (мкс/50)
			IRRX_uint_TOGGLE_PAUSE		=	IRXX.IRXX_func_DECODING(i,10);																					//	длительность паузы рестарт или toggle (мкс/50)
			IRRX_uint_TOGGLE_POSITION	=	IRXX.IRXX_func_DECODING(i,11);																					//	позиция бита toggle или рестарт в пакете
			IRRX_uint_BIT_PULSE_MAX		=	IRXX.IRXX_func_DECODING(i,12);																					//	максимальная длительность импульса (мкс/50)
			IRRX_uint_BIT_PULSE_MIN		=	IRXX.IRXX_func_DECODING(i,13);																					//	минимальная  длительность импульса (мкс/50)
			IRRX_uint_BIT_PAUSE_MAX		=	IRXX.IRXX_func_DECODING(i,14);																					//	максимальная длительность паузы (мкс/50)
			IRRX_uint_BIT_PAUSE_MIN		=	IRXX.IRXX_func_DECODING(i,15);																					//	минимальная  длительность паузы (мкс/50)
			IRRX_flag_START				=	IRXX.IRXX_func_DECODING(i,16);																					//	флаг наличия стартового импульса
			IRRX_flag_STOP				=	IRXX.IRXX_func_DECODING(i,17);																					//	флаг наличия стопового импульса
			IRRX_flag_TOGGLE			=	IRXX.IRXX_func_DECODING(i,18);																					//	флаг наличия бита toggle
			IRRX_uint_PACK_REPEAT_TYPE	=	IRXX.IRXX_func_DECODING(i,19); IRRX_uint_PACK_REPEAT  =0;														//	тип повторного пакета (0-нет, 1-с инверсными битами, 2-идентичен информационному, 3-уникален)
			IRRX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,20); IRRX_uint_PACK_REPEAT<<=8;														//	1ый байт значения или битов инверсии пакета повтора
			IRRX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,21); IRRX_uint_PACK_REPEAT<<=8;														//	2ой байт значения или битов инверсии пакета повтора
			IRRX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,22); IRRX_uint_PACK_REPEAT<<=8;														//	3ий байт значения или битов инверсии пакета повтора
			IRRX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,23); IRRX_uint_PACK_REPEAT<<=8;														//	4ый байт значения или битов инверсии пакета повтора
			return true;
}

//			загрузка протокола передачи данных
bool		iarduino_IR_TX::protocol(const char *i){
			for(IRTX_var_I=0; IRTX_var_I<25; IRTX_var_I++){if((i[IRTX_var_I] & 0xC0) != 0x40){return false;}}												//  возвращаем false если первые два бита любого символа в протоколе не равны "01"
			if(IRXX.IRXX_func_DECODING(i,0)>10){return false;}																								//  возвращаем false если тип кодировки больше 10
			IRTX_uint_CODING			=	IRXX.IRXX_func_DECODING(i,0);																					//	тип кодировки
			frequency					=	IRXX.IRXX_func_DECODING(i,1);																					//	несущая частота
			length						=	IRXX.IRXX_func_DECODING(i,2);																					//	длина первого (информационного) пакета (бит)
			IRTX_uint_PACK_LENGTH_REPEAT=	IRXX.IRXX_func_DECODING(i,3);																					//	длина второго (повторного) пакета (бит)
			IRTX_uint_PACK_PAUSE		=	IRXX.IRXX_func_DECODING(i,4);																					//	длительность паузы между пакетами (мс)
			IRTX_uint_START_PULSE		=	IRXX.IRXX_func_DECODING(i,5);																					//	длительность импульса старт (мкс/50)
			IRTX_uint_START_PAUSE		=	IRXX.IRXX_func_DECODING(i,6);																					//	длительность паузы старт (мкс/50)
			IRTX_uint_STOP_PULSE		=	IRXX.IRXX_func_DECODING(i,7);																					//	длительность импульса стоп (мкс/50)
			IRTX_uint_STOP_PAUSE		=	IRXX.IRXX_func_DECODING(i,8);																					//	длительность паузы стоп (мкс/50)
			IRTX_uint_TOGGLE_PULSE		=	IRXX.IRXX_func_DECODING(i,9);																					//	длительность импульса рестарт или toggle (мкс/50)
			IRTX_uint_TOGGLE_PAUSE		=	IRXX.IRXX_func_DECODING(i,10);																					//	длительность паузы рестарт или toggle (мкс/50)
			IRTX_uint_TOGGLE_POSITION	=	IRXX.IRXX_func_DECODING(i,11);																					//	позиция бита toggle или рестарт в пакете
			IRTX_uint_BIT_PULSE_MAX		=	IRXX.IRXX_func_DECODING(i,12);																					//	максимальная длительность импульса (мкс/50)
			IRTX_uint_BIT_PULSE_MIN		=	IRXX.IRXX_func_DECODING(i,13);																					//	минимальная  длительность импульса (мкс/50)
			IRTX_uint_BIT_PAUSE_MAX		=	IRXX.IRXX_func_DECODING(i,14);																					//	максимальная длительность паузы (мкс/50)
			IRTX_uint_BIT_PAUSE_MIN		=	IRXX.IRXX_func_DECODING(i,15);																					//	минимальная  длительность паузы (мкс/50)
			IRTX_flag_START				=	IRXX.IRXX_func_DECODING(i,16);																					//	флаг наличия стартового импульса
			IRTX_flag_STOP				=	IRXX.IRXX_func_DECODING(i,17);																					//	флаг наличия стопового импульса
			IRTX_flag_TOGGLE			=	IRXX.IRXX_func_DECODING(i,18);																					//	флаг наличия бита toggle
			IRTX_uint_PACK_REPEAT_TYPE	=	IRXX.IRXX_func_DECODING(i,19); IRTX_uint_PACK_REPEAT  =0;														//	тип повторного пакета (0-нет, 1-с инверсными битами, 2-идентичен информационному, 3-уникален)
			IRTX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,20); IRTX_uint_PACK_REPEAT<<=8;														//	1ый байт значения или битов инверсии пакета повтора
			IRTX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,21); IRTX_uint_PACK_REPEAT<<=8;														//	2ой байт значения или битов инверсии пакета повтора
			IRTX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,22); IRTX_uint_PACK_REPEAT<<=8;														//	3ий байт значения или битов инверсии пакета повтора
			IRTX_uint_PACK_REPEAT		=	IRXX.IRXX_func_DECODING(i,23); IRTX_uint_PACK_REPEAT<<=8;														//	4ый байт значения или битов инверсии пакета повтора
			return true;
}

//			выгрузка протокола передачи данных или его сброс
int			iarduino_IR_RX::protocol(int i){ int j=0;
			if(i==IR_CLEAN){IRRX_flag_SET_PROTOCOL=0; return j;}																							//	сбрасываем установленный ранеее протокол
			if(i<=19){j=IRXX.IRXX_func_DECODING(protocol(),i);																								//	получаем запрошенное значение
			if(i==11){j/=2;}																																//	переводим позицию бита toggle из полубитов в биты
			if(i>4 && i<16 && i!=11){j*=50;}}																												//	переводим мкс/50 в мкс
			return j;																																		//	выводим запрошенное значение
}

//			выгрузка протокола передачи данных
char*		iarduino_IR_RX::protocol(){
			IRRX_char_PROTOCOL[ 0]		=	0x40 | (0x3F & IRRX_uint_CODING				);																	//	всего		6 бит номера кодировки
			IRRX_char_PROTOCOL[ 1]		=	0x40 | (0x3F & IRRX_uint_FREQUENCY			);																	//	всего		6 бит частоты в кГц
			IRRX_char_PROTOCOL[ 2]		=	0x40 | (0x3F & IRRX_flag_SET_PROTOCOL?IRRX_uint_PACK_LENGTH       :length					);					//	всего		6 бит длинны информационного пакета в битах
			IRRX_char_PROTOCOL[ 3]		=	0x40 | (0x3F & IRRX_flag_SET_PROTOCOL?IRRX_uint_PACK_LENGTH_REPEAT:IRRX_uint_LENGTH_REPEAT	);					//	всего		6 бит длинны пакета повтора в битах
			IRRX_char_PROTOCOL[ 4]		=	0x40 | (0x3F & IRRX_uint_PACK_PAUSE			);																	//	последние	6 бит длительности паузы между пакетами в мс
			IRRX_char_PROTOCOL[ 5]		=	0x40 | (0x3F & IRRX_uint_START_PULSE		);																	//	последние	6 бит длительности импульса бита старт в мкс/50
			IRRX_char_PROTOCOL[ 6]		=	0x40 | (0x3F & IRRX_uint_START_PAUSE		);																	//	последние	6 бит длительности паузы бита страт в мкс/50
			IRRX_char_PROTOCOL[ 7]		=	0x40 | (0x3F & IRRX_uint_STOP_PULSE			);																	//	последние	6 бит длительности импульса бита стоп в мкс/50
			IRRX_char_PROTOCOL[ 8]		=	0x40 | (0x3F & IRRX_uint_STOP_PAUSE			);																	//	последние	6 бит длительности паузы бита стоп в мкс/50
			IRRX_char_PROTOCOL[ 9]		=	0x40 | (0x3F & IRRX_uint_TOGGLE_PULSE		);																	//	последние	6 бит длительности импульса бита toggle/рестарт в мкс/50
			IRRX_char_PROTOCOL[10]		=	0x40 | (0x3F & IRRX_uint_TOGGLE_PAUSE		);																	//	последние	6 бит длительности паузы бита toggle/рестарт в мкс/50
			IRRX_char_PROTOCOL[11]		=	0x40 | (0x3F & IRRX_uint_TOGGLE_POSITION	);																	//	всего		6 бит позиции бита toggle/рестарт в пакете
			IRRX_char_PROTOCOL[12]		=	0x40 | (0x3F & IRRX_uint_BIT_PULSE_MAX		);																	//	всего		6 бит максимальной длительности импульса бита в мкс/50
			IRRX_char_PROTOCOL[13]		=	0x40 | (0x3F & IRRX_uint_BIT_PULSE_MIN		);																	//	всего		6 бит минимальной длительности импульса бита в мкс/50
			IRRX_char_PROTOCOL[14]		=	0x40 | (0x3F & IRRX_uint_BIT_PAUSE_MAX		);																	//	всего		6 бит максимальной длительности паузы бита в мкс/50
			IRRX_char_PROTOCOL[15]		=	0x40 | (0x3F & IRRX_uint_BIT_PAUSE_MIN		);																	//	всего		6 бит минимальной длительности паузы бита в мкс/50
			IRRX_char_PROTOCOL[16]		=	0x40 | (0x3F & IRRX_uint_PACK_REPEAT >> 26	);																	//	первые		6 бит данных для отправки пакета повтора
			IRRX_char_PROTOCOL[17]		=	0x40 | (0x3F & IRRX_uint_PACK_REPEAT >> 20	);																	//	вторые		6 бит данных для отправки пакета повтора
			IRRX_char_PROTOCOL[18]		=	0x40 | (0x3F & IRRX_uint_PACK_REPEAT >> 14	);																	//	третие		6 бит данных для отправки пакета повтора
			IRRX_char_PROTOCOL[19]		=	0x40 | (0x3F & IRRX_uint_PACK_REPEAT >>  8	);																	//	четвёртые	6 бит данных для отправки пакета повтора
			IRRX_char_PROTOCOL[20]		=	0x40 | (0x3F & IRRX_uint_PACK_REPEAT >>  2	);																	//	пятые		6 бит данных для отправки пакета повтора
			IRRX_char_PROTOCOL[21]		=	0x40 | (0x03 & IRRX_uint_PACK_REPEAT)<<4 | (IRRX_uint_PACK_PAUSE  >>6)<<2 | (IRRX_uint_START_PULSE>>6);			//	последние 2 бита IRRX_uint_PACK_REPEAT, первые 2 бита IRRX_uint_PACK_PAUSE, первые 2 бита IRRX_uint_START_PULSE
			IRRX_char_PROTOCOL[22]		=	0x40 | (IRRX_uint_START_PAUSE    >>6)<<4 | (IRRX_uint_STOP_PULSE  >>6)<<2 | (IRRX_uint_STOP_PAUSE >>6);			//	первые 2 бита IRRX_uint_START_PAUSE, первые 2 бита IRRX_uint_STOP_PULSE, первые 2 бита IRRX_uint_STOP_PAUSE
			IRRX_char_PROTOCOL[23]		=	0x40 | (IRRX_uint_TOGGLE_PULSE   >>6)<<4 | (IRRX_uint_TOGGLE_PAUSE>>6)<<2 | IRRX_uint_PACK_REPEAT_TYPE;			//	первые 2 бита IRRX_uint_TOGGLE_PULSE, первые 2 бита IRRX_uint_TOGGLE_PAUSE, 2 бита IRRX_uint_PACK_REPEAT_TYPE
			IRRX_char_PROTOCOL[24]		=	0x40 | IRRX_flag_START<<5 | IRRX_flag_STOP<<4 | IRRX_flag_TOGGLE<<3;											//	флаги
			IRRX_char_PROTOCOL[25]		=	0;
			return IRRX_char_PROTOCOL;
}
//			определение протокола передачи данных
uint8_t		iarduino_IR_RX::IRRX_func_PROTOCOL(){
			IRRX_var_J					=	0;																												//	переменная для вывода номера протокола
			IRRX_uint_BIT_PULSE_MAX		=	0;																												//	максимальная длинна импульсов
			IRRX_uint_BIT_PULSE_MIN		=	255;																											//	минимальная  длинна импульсов
			IRRX_uint_BIT_PAUSE_MAX		=	0;																												//	максимальная длинна пауз
			IRRX_uint_BIT_PAUSE_MIN		=	255;																											//	минимальная  длинна пауз
			IRRX_flag_START				=	0;																												//	флаг наличия сигнала старт
			IRRX_uint_START_PULSE		=	IRRX_mass_PACK[0][0];																							//	предположим, что первый бит это сигнал старт
			IRRX_uint_START_PAUSE		=	IRRX_mass_PACK[0][1];																							//	предположим, что первый бит это сигнал старт
			IRRX_flag_STOP				=	0;																												//	флаг наличия сигнала стоп
			IRRX_uint_STOP_PULSE		=	IRRX_mass_PACK[0][IRRX_uint_PACK_LEN[0]-1];																		//	предположим, что последний бит это сигнал стоп
			IRRX_uint_STOP_PAUSE		=	IRRX_mass_PACK[0][IRRX_uint_PACK_LEN[0]-2];																		//	предположим, что последний бит это сигнал стоп
			IRRX_flag_TOGGLE			=	0;																												//	флаг наличия импульса toggle/рестарт
			IRRX_uint_TOGGLE_PULSE_POS	=	0;																												//	позиция бита toggle/рестарт в пакете
			IRRX_uint_TOGGLE_PAUSE_POS	=	0;																												//	позиция бита toggle/рестарт в пакете
			IRRX_uint_TOGGLE_PULSE_SUM	=	0;																												//	кол-во битов toggle/рестарт в пакете
			IRRX_uint_TOGGLE_PAUSE_SUM	=	0;																												//	кол-во битов toggle/рестарт в пакете
			IRRX_uint_TOGGLE_PULSE		=	0;																												//	длинна импульса toggle/рестарт
			IRRX_uint_TOGGLE_PAUSE		=	0;																												//	длинна паузы toggle/рестарт
			IRRX_uint_PACK_REPEAT		=	0;																												//	значение или биты инверсии для отправки пакета повтора
			IRRX_uint_PACK_REPEAT_TYPE	=	0;																												//	тип повторного пакета
			IRRX_uint_PACK_PAUSE		=	IRVV.IRRX_uint_PACK_PAUSE/20;																					//	пауза между 1 и 2 пакетами в мс
			IRRX_uint_FREQUENCY			=	37;																												//	несущая частота сигнала
//			определяем максимальные и минимальные значения импульсов и пауз (без первого и последнего)
			for(IRRX_var_I=2; IRRX_var_I<IRRX_uint_PACK_LEN[0]-2; IRRX_var_I++){
				if(IRRX_var_I%2){	IRRX_uint_BIT_PAUSE_MAX=max(IRRX_uint_BIT_PAUSE_MAX, IRRX_mass_PACK[0][IRRX_var_I]);
									IRRX_uint_BIT_PAUSE_MIN=min(IRRX_uint_BIT_PAUSE_MIN, IRRX_mass_PACK[0][IRRX_var_I]);}
				else{				IRRX_uint_BIT_PULSE_MAX=max(IRRX_uint_BIT_PULSE_MAX, IRRX_mass_PACK[0][IRRX_var_I]);
									IRRX_uint_BIT_PULSE_MIN=min(IRRX_uint_BIT_PULSE_MIN, IRRX_mass_PACK[0][IRRX_var_I]);}
			}
//			проверяем наличие импульса toggle/рестарт
			IRRX_uint_TOGGLE_PULSE=IRRX_uint_BIT_PULSE_MAX; IRRX_uint_TOGGLE_PAUSE=IRRX_uint_BIT_PAUSE_MAX; IRRX_uint_BIT_PULSE_MAX=0; IRRX_uint_BIT_PAUSE_MAX=0;
			for(IRRX_var_I=2; IRRX_var_I<IRRX_uint_PACK_LEN[0]-2; IRRX_var_I++){
				if(IRRX_var_I%2){	if(IRRX_func_COMPARE(IRRX_mass_PACK[0][IRRX_var_I], IRRX_uint_TOGGLE_PAUSE, 6)){IRRX_uint_TOGGLE_PAUSE_POS=IRRX_var_I; IRRX_uint_TOGGLE_PAUSE_SUM++;}else{IRRX_uint_BIT_PAUSE_MAX=max(IRRX_uint_BIT_PAUSE_MAX, IRRX_mass_PACK[0][IRRX_var_I]);}}
				else{				if(IRRX_func_COMPARE(IRRX_mass_PACK[0][IRRX_var_I], IRRX_uint_TOGGLE_PULSE, 6)){IRRX_uint_TOGGLE_PULSE_POS=IRRX_var_I; IRRX_uint_TOGGLE_PULSE_SUM++;}else{IRRX_uint_BIT_PULSE_MAX=max(IRRX_uint_BIT_PULSE_MAX, IRRX_mass_PACK[0][IRRX_var_I]);}}
			}						if(IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_BIT_PULSE_MIN, 6)||!IRRX_uint_BIT_PULSE_MAX||IRRX_uint_TOGGLE_PULSE_SUM!=1){IRRX_uint_BIT_PULSE_MAX=IRRX_uint_TOGGLE_PULSE;}else{IRRX_flag_TOGGLE=1;}
									if(IRRX_func_COMPARE(IRRX_uint_BIT_PAUSE_MAX, IRRX_uint_BIT_PAUSE_MIN, 6)||!IRRX_uint_BIT_PAUSE_MAX||IRRX_uint_TOGGLE_PAUSE_SUM!=1){IRRX_uint_BIT_PAUSE_MAX=IRRX_uint_TOGGLE_PAUSE;}else{IRRX_flag_TOGGLE=1;}
//			проверяем наличие сигналов старт и стоп
			if( IRRX_uint_START_PULSE>(IRRX_uint_BIT_PULSE_MAX+9) || (IRRX_uint_START_PULSE+9)<IRRX_uint_BIT_PULSE_MIN || IRRX_uint_START_PAUSE>(IRRX_uint_BIT_PAUSE_MAX+9) || (IRRX_uint_START_PAUSE+9)<IRRX_uint_BIT_PAUSE_MIN){IRRX_flag_START=1;}
			if( IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_BIT_PULSE_MIN, 6) && !IRRX_func_COMPARE(IRRX_uint_BIT_PAUSE_MAX, IRRX_uint_BIT_PAUSE_MIN, 6)){IRRX_flag_STOP=1; if(IRRX_uint_STOP_PAUSE>IRRX_uint_BIT_PAUSE_MAX){IRRX_uint_STOP_PAUSE-=IRRX_uint_BIT_PAUSE_MAX;}else{IRRX_uint_STOP_PAUSE=0;}}else{IRRX_uint_BIT_PAUSE_MAX=max(IRRX_uint_BIT_PAUSE_MAX, IRRX_uint_STOP_PAUSE); IRRX_uint_BIT_PAUSE_MIN=min(IRRX_uint_BIT_PAUSE_MIN, IRRX_uint_STOP_PAUSE); IRRX_uint_BIT_PULSE_MAX=max(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_STOP_PULSE); IRRX_uint_BIT_PULSE_MIN=min(IRRX_uint_BIT_PULSE_MIN, IRRX_uint_STOP_PULSE);}
//			проверяем наличие протоколов кодирования длинной импульса/паузы или бифазного кодирования
			if( IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_BIT_PULSE_MIN, 6) && !IRRX_func_COMPARE(IRRX_uint_BIT_PAUSE_MAX, IRRX_uint_BIT_PAUSE_MIN, 6))																			{IRRX_var_J=IR_PAUSE_LENGTH;}	//	если длительность всех импульсов приблизительно равна, а длительность пауз      отличается,        то устанавливаем протокол: кодирование длинной паузы
			if(!IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_BIT_PULSE_MIN, 6) &&  IRRX_func_COMPARE(IRRX_uint_BIT_PAUSE_MAX, IRRX_uint_BIT_PAUSE_MIN, 6))																			{IRRX_var_J=IR_PULSE_LENGTH;}	//	если длительность всех пауз      приблизительно равна, а длительность импульсов отличается,        то устанавливаем протокол: кодирование шириной импульса (ШИМ)
			if( IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MIN, IRRX_uint_BIT_PAUSE_MIN, 3) &&  IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_BIT_PAUSE_MAX, 3))																			{IRRX_var_J=IR_BIPHASIC;}		//	если максимальные или минимальные значения длительностей и пауз приблизительно равны,              то устанавливаем протокол: кодирование длинной паузы
			if( IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MIN, IRRX_uint_BIT_PAUSE_MIN, 3) && (IRRX_func_COMPARE(IRRX_uint_BIT_PAUSE_MAX, IRRX_uint_BIT_PAUSE_MIN*2, 3) || IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MAX, IRRX_uint_BIT_PULSE_MIN*2, 3)))	{IRRX_var_J=IR_BIPHASIC;}		//	если минимальные значения импульсов и пауз приблизительно равны, а максимальные в два раза больше, то устанавливаем протокол: кодирование длинной паузы
//			проверяем наличие протоколов бифазного кодирования со специальными битами (toggle) или битами (старт/стоп)
			if( IRRX_var_J==IR_BIPHASIC){ IRRX_flag_STOP=0; IRRX_flag_TOGGLE=0;
				if(IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MIN, 17, 4) && !IRRX_flag_START){IRRX_var_J=IR_RS5; IRRX_uint_FREQUENCY=36;}
				if(IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MIN,  9, 4) &&  IRRX_flag_START && IRRX_func_COMPARE(IRRX_uint_START_PULSE, 54, 4) && IRRX_func_COMPARE(IRRX_uint_START_PAUSE, 17, 4)){IRRX_var_J=IR_RS6; IRRX_uint_FREQUENCY=36;}
				if(IRRX_func_COMPARE(IRRX_uint_BIT_PULSE_MIN, 10, 4) &&  IRRX_flag_START && IRRX_func_COMPARE(IRRX_uint_START_PULSE, 10, 4) && IRRX_func_COMPARE(IRRX_uint_START_PAUSE, 50, 4)){IRRX_var_J=IR_NRC; IRRX_uint_FREQUENCY=38;}
			}
			if( IRRX_var_J==IR_PAUSE_LENGTH){IRRX_uint_TOGGLE_POSITION=IRRX_uint_TOGGLE_PAUSE_POS-1;}
			if( IRRX_var_J==IR_PULSE_LENGTH){IRRX_uint_TOGGLE_POSITION=IRRX_uint_TOGGLE_PULSE_POS;}
			return IRRX_var_J;
}
//			раскодирование принятых данных в переменную IRRX_uint_DATA
bool		iarduino_IR_RX::IRRX_func_DECODE(uint8_t i){
			IRRX_uint_DATA=0; IRRX_uint_LENGTH=0;
			if(IRRX_uint_CODING==IR_PAUSE_LENGTH		){for(IRRX_var_I=IRRX_flag_START?2:0; IRRX_var_I<IRRX_uint_PACK_LEN[i]-1; IRRX_var_I+=2){if(!(IRRX_flag_TOGGLE&&(IRRX_uint_TOGGLE_POSITION==IRRX_var_I))){IRRX_uint_DATA=(IRRX_uint_DATA<<1)+(IRRX_var_I<IRRX_uint_PACK_LEN[i]-3?(IRRX_mass_PACK[i][IRRX_var_I+1]>IRRX_uint_BIT_PAUSE_MIN+6):(IRRX_mass_PACK[i][IRRX_var_I+1]-IRRX_uint_STOP_PAUSE>IRRX_uint_BIT_PAUSE_MIN+6));	IRRX_uint_LENGTH++;}} return true;}
			if(IRRX_uint_CODING==IR_PULSE_LENGTH		){for(IRRX_var_I=IRRX_flag_START?2:0; IRRX_var_I<IRRX_uint_PACK_LEN[i];   IRRX_var_I+=2){if(!(IRRX_flag_TOGGLE&&(IRRX_uint_TOGGLE_POSITION==IRRX_var_I))){IRRX_uint_DATA=(IRRX_uint_DATA<<1)+                                    (IRRX_mass_PACK[i][IRRX_var_I  ]>IRRX_uint_BIT_PULSE_MIN+6);                                                                                   IRRX_uint_LENGTH++;}} return true;}
			if(IRRX_uint_CODING==IR_NRC					){if( IRRX_func_DECODE_BIPHASIC(i?0:1,false)){return true;}else{if(IRRX_flag_SET_PROTOCOL || i){return false;}else{IRRX_uint_CODING=IR_BIPHASIC;}}}
			if(IRRX_uint_CODING==IR_RS5					){if( IRRX_func_DECODE_BIPHASIC(i    ,true )){return true;}else{if(IRRX_flag_SET_PROTOCOL || i){return false;}else{IRRX_uint_CODING=IR_BIPHASIC;}}}
			if(IRRX_uint_CODING==IR_RS5X				){if( IRRX_func_DECODE_BIPHASIC(i    ,true )){return true;}else{if(IRRX_flag_SET_PROTOCOL || i){return false;}else{IRRX_uint_CODING=IR_BIPHASIC;}}}
			if(IRRX_uint_CODING==IR_RS6					){if( IRRX_func_DECODE_BIPHASIC(i    ,false)){return true;}else{if(IRRX_flag_SET_PROTOCOL || i){return false;}else{IRRX_uint_CODING=IR_BIPHASIC;}}}
			if(IRRX_uint_CODING==IR_BIPHASIC			){if( IRRX_func_DECODE_BIPHASIC(i    ,true )){return true;}else{if(IRRX_flag_SET_PROTOCOL || i){return false;}else{IRRX_uint_CODING=IR_BIPHASIC_INV;}}}
			if(IRRX_uint_CODING==IR_BIPHASIC_INV		){if( IRRX_func_DECODE_BIPHASIC(i    ,false)){return true;}else{return false;}}
			return false;
}
//			декодирование бифазного кода
bool		iarduino_IR_RX::IRRX_func_DECODE_BIPHASIC(uint8_t i, bool j){
			IRRX_uint_DATA=0; IRRX_uint_LENGTH=0;
//			определяем наличие удвоенного бита toggle
			bool k=0; for(IRRX_var_I=IRRX_flag_START?2:0; IRRX_var_I<IRRX_uint_PACK_LEN[i]; IRRX_var_I+=2){if(IRRX_mass_PACK[i][IRRX_var_I]>IRRX_uint_BIT_PULSE_MAX+4 || IRRX_mass_PACK[i][IRRX_var_I+1]>IRRX_uint_BIT_PAUSE_MAX+4){k=1;}}
//			преобразуем данные в последовательность «1» и «0»
			IRRX_mass_BIPHASIC_BIN[0]=0; IRRX_mass_BIPHASIC_BIN[1]=0; IRRX_mass_BIPHASIC_BIN[2]=0; IRRX_mass_BIPHASIC_BIN[3]=0;
			for(IRRX_var_I=IRRX_flag_START?2:0, IRRX_var_J=0, IRRX_var_K=15; IRRX_var_I<IRRX_uint_PACK_LEN[i]; IRRX_var_I+=2){
				IRRX_mass_BIPHASIC_BIN[IRRX_var_J]|=1<<IRRX_var_K; if(IRRX_var_K){IRRX_var_K--;}else{IRRX_var_K=15; IRRX_var_J++;} IRRX_uint_LENGTH++; if(IRRX_mass_PACK[i][IRRX_var_I  ]>IRRX_uint_BIT_PULSE_MIN+5){if(IRRX_uint_CODING!=IR_RS6 || !(IRRX_uint_LENGTH==9 || (IRRX_uint_LENGTH==10 && !k))){IRRX_mass_BIPHASIC_BIN[IRRX_var_J]|=1<<IRRX_var_K; if(IRRX_var_K){IRRX_var_K--;}else{IRRX_var_K=15; IRRX_var_J++;} IRRX_uint_LENGTH++;}}
				IRRX_mass_BIPHASIC_BIN[IRRX_var_J]|=0<<IRRX_var_K; if(IRRX_var_K){IRRX_var_K--;}else{IRRX_var_K=15; IRRX_var_J++;} IRRX_uint_LENGTH++; if(IRRX_mass_PACK[i][IRRX_var_I+1]>IRRX_uint_BIT_PAUSE_MIN+5){if(IRRX_uint_CODING!=IR_RS6 || !(IRRX_uint_LENGTH==9 || (IRRX_uint_LENGTH==10 && !k))){IRRX_mass_BIPHASIC_BIN[IRRX_var_J]|=0<<IRRX_var_K; if(IRRX_var_K){IRRX_var_K--;}else{IRRX_var_K=15; IRRX_var_J++;} IRRX_uint_LENGTH++;}}
			}
//			сдвигаем данные на 1 бит вправо (если _/¯ = 1   ¯\_ = 0 )
			if(j){IRRX_func_DECODE_BIPHASIC_SHIFT(false); IRRX_uint_LENGTH++;}
//			корректируем количество битов до чётного числа
			if(IRRX_uint_LENGTH%2){IRRX_uint_LENGTH++;} if(!((IRRX_mass_BIPHASIC_BIN[(IRRX_uint_LENGTH-1)/16]&(1<<(16-(IRRX_uint_LENGTH-(((IRRX_uint_LENGTH-1)/16)*16)))))||(IRRX_mass_BIPHASIC_BIN[(IRRX_uint_LENGTH-1)/16]&(1<<(17-(IRRX_uint_LENGTH-(((IRRX_uint_LENGTH-1)/16)*16))))))){IRRX_uint_LENGTH-=2;}
//			убираем бит toggle
			IRRX_var_I=IRRX_mass_BIPHASIC_BIN[0]>>8; // сохраняем биты start и mode
			if(IRRX_uint_CODING==IR_RS5	){IRRX_func_DECODE_BIPHASIC_SHIFT(true); IRRX_func_DECODE_BIPHASIC_SHIFT(true); IRRX_uint_LENGTH-=2; IRRX_var_I&=0b11110000; IRRX_mass_BIPHASIC_BIN[0]&=0b0000111111111111; IRRX_mass_BIPHASIC_BIN[0]|=IRRX_var_I<<8;}
			if(IRRX_uint_CODING==IR_RS5X){IRRX_func_DECODE_BIPHASIC_SHIFT(true); IRRX_func_DECODE_BIPHASIC_SHIFT(true); IRRX_uint_LENGTH-=2; IRRX_var_I&=0b11000000; IRRX_mass_BIPHASIC_BIN[0]&=0b0011111111111111; IRRX_mass_BIPHASIC_BIN[0]|=IRRX_var_I<<8;}
			if(IRRX_uint_CODING==IR_RS6	){IRRX_func_DECODE_BIPHASIC_SHIFT(true); IRRX_func_DECODE_BIPHASIC_SHIFT(true); IRRX_uint_LENGTH-=2; IRRX_var_I&=0b11111111; IRRX_mass_BIPHASIC_BIN[0]&=0b0000000011111111; IRRX_mass_BIPHASIC_BIN[0]|=IRRX_var_I<<8;}
//			преобразуем последовательность «1» и «0» в число
			for(IRRX_var_I=0, IRRX_var_J=0, IRRX_var_K=15; IRRX_var_I<IRRX_uint_LENGTH; IRRX_var_I+=2){IRRX_uint_DATA<<=1;
				if(((IRRX_mass_BIPHASIC_BIN[IRRX_var_J]&(1<<IRRX_var_K))?1:0) && ((IRRX_mass_BIPHASIC_BIN[IRRX_var_J]&(1<<(IRRX_var_K-1)))?0:1)){IRRX_uint_DATA|=j?0:1;}else
				if(((IRRX_mass_BIPHASIC_BIN[IRRX_var_J]&(1<<IRRX_var_K))?0:1) && ((IRRX_mass_BIPHASIC_BIN[IRRX_var_J]&(1<<(IRRX_var_K-1)))?1:0)){IRRX_uint_DATA|=j?1:0;}else{return false;}
				if(IRRX_var_K>1){IRRX_var_K-=2;}else{IRRX_var_K=15; IRRX_var_J++;}
			}	IRRX_uint_LENGTH/=2;
			return true;
}
//			сдвиг данных на 1 бит (true - влево, false - вправо)
void		iarduino_IR_RX::IRRX_func_DECODE_BIPHASIC_SHIFT(bool i){
			if(i){	IRRX_mass_BIPHASIC_BIN[0]<<=1; IRRX_mass_BIPHASIC_BIN[0]|=IRRX_mass_BIPHASIC_BIN[1]>>15;
					IRRX_mass_BIPHASIC_BIN[1]<<=1; IRRX_mass_BIPHASIC_BIN[1]|=IRRX_mass_BIPHASIC_BIN[2]>>15;
					IRRX_mass_BIPHASIC_BIN[2]<<=1; IRRX_mass_BIPHASIC_BIN[2]|=IRRX_mass_BIPHASIC_BIN[3]>>15;
					IRRX_mass_BIPHASIC_BIN[3]<<=1; 
			}else{	IRRX_mass_BIPHASIC_BIN[3]>>=1; IRRX_mass_BIPHASIC_BIN[3]|=IRRX_mass_BIPHASIC_BIN[2]<<15;
					IRRX_mass_BIPHASIC_BIN[2]>>=1; IRRX_mass_BIPHASIC_BIN[2]|=IRRX_mass_BIPHASIC_BIN[1]<<15;
					IRRX_mass_BIPHASIC_BIN[1]>>=1; IRRX_mass_BIPHASIC_BIN[1]|=IRRX_mass_BIPHASIC_BIN[0]<<15;
					IRRX_mass_BIPHASIC_BIN[0]>>=1;
			}
}
//			проверка протокола передачи данных NRC по стартовому пакету
bool		iarduino_IR_RX::IRRX_func_CHECK_NRC(){				if(IRRX_uint_LENGTH_REPEAT<3){				return false;}
			for(IRRX_var_I=1; IRRX_var_I<=IRRX_uint_LENGTH_REPEAT; IRRX_var_I++){
				if(IRRX_var_I==2 &&  (IRRX_uint_DATA_REPEAT & 1<< (IRRX_uint_LENGTH_REPEAT-IRRX_var_I))){	return false;}
				if(IRRX_var_I!=2 && !(IRRX_uint_DATA_REPEAT & 1<< (IRRX_uint_LENGTH_REPEAT-IRRX_var_I))){	return false;}
			}																								return true;
}
//			создание повторного пакета для протокола
uint32_t	iarduino_IR_RX::IRRX_func_CREATE_PACK_REPEAT(void){ uint32_t k=0;
			switch(IRRX_uint_PACK_REPEAT_TYPE){
				case 1:  for(IRRX_var_I=1; IRRX_var_I<=length; IRRX_var_I++){k<<=1; k+=((data>>(length-IRRX_var_I)&1)==(IRRX_uint_DATA_REPEAT>>(length-IRRX_var_I)&1));} return k; break;
				case 2:  return 0; break;
				case 3:  return IRRX_uint_DATA_REPEAT; break;
				default: return 0;
			}
}

//			создание повторного пакета для отправки на светодиод (данные)
uint32_t	iarduino_IR_TX::IRTX_func_CREATE_PACK_REPEAT(uint32_t j){ uint32_t k=0;
			switch(IRTX_uint_PACK_REPEAT_TYPE){
				case 1:  for(IRTX_var_I=1; IRTX_var_I<=length; IRTX_var_I++){k<<=1; k+=(j>>(length-IRTX_var_I)&1)?((IRTX_uint_PACK_REPEAT>>(length-IRTX_var_I)&1)?1:0):((IRTX_uint_PACK_REPEAT>>(length-IRTX_var_I)&1)?0:1);} return k; break;
				case 2:  return j; break;
				case 3:  return IRTX_uint_PACK_REPEAT; break;
				default: return 0;
			}
}

//			сравнение чисел с указанием допуска	(число, число, допуск±)
bool		iarduino_IR_RX::IRRX_func_COMPARE(uint8_t i, uint8_t j, uint8_t k){
			int result = i - j; if(result<0){result*=-1;}
			return result>k? false:true;
}

//			передача пакета данных на светодиод
void		iarduino_IR_TX::IRTX_func_SEND(uint32_t i, uint8_t j){
//			определяем позицию бита toggle/рестарт
			IRTX_var_K=255; if(IRTX_uint_CODING==IR_PAUSE_LENGTH || IRTX_uint_CODING==IR_PULSE_LENGTH){if(IRTX_flag_TOGGLE){IRTX_var_K=IRTX_uint_TOGGLE_POSITION/2;} if(IRTX_flag_START){IRTX_var_K--;}}else if(IRTX_uint_CODING==IR_RS5){IRTX_var_K=2;}else if(IRTX_uint_CODING==IR_RS5X){IRTX_var_K=1;}else if(IRTX_uint_CODING==IR_RS6){IRTX_var_K=4;}
//			устанавливаем таймер на частоту передачи
			if(frequency){IRVV.IRXX_flag_SEND=1; IRXX.IRXX_func_TIMER2_SETREG(frequency*2);}
//			отправляем сигнал старт
			if(IRTX_flag_START){IRTX_func_DELAY(1,IRTX_uint_START_PULSE); IRTX_func_DELAY(0,IRTX_uint_START_PAUSE);}
//			отправляем биты данных
			for(IRTX_var_I=0; IRTX_var_I<j; IRTX_var_I++){
//				определяем значение отправляемого бита
				IRTX_var_J = i>>(j-IRTX_var_I-1) & 1;
//				передаём бит toggle/рестарт
				if(IRTX_var_I==IRTX_var_K){
					switch(IRTX_uint_CODING){
						case IR_PAUSE_LENGTH:							IRTX_func_DELAY(1						, IRTX_uint_TOGGLE_PULSE);
																		IRTX_func_DELAY(0						, IRTX_uint_TOGGLE_PAUSE);	break;
						case IR_PULSE_LENGTH:							IRTX_func_DELAY(0						, IRTX_uint_TOGGLE_PAUSE);
																		IRTX_func_DELAY(1						, IRTX_uint_TOGGLE_PULSE);	break;
						case IR_RS5: case IR_RS5X:						IRTX_func_DELAY(!IRTX_flag_TOGGLE_SEND	, IRTX_uint_BIT_PAUSE_MIN);
																		IRTX_func_DELAY( IRTX_flag_TOGGLE_SEND	, IRTX_uint_BIT_PAUSE_MIN);	break;
						case IR_RS6:									IRTX_func_DELAY( IRTX_flag_TOGGLE_SEND	, IRTX_uint_BIT_PAUSE_MIN*2);
																		IRTX_func_DELAY(!IRTX_flag_TOGGLE_SEND	, IRTX_uint_BIT_PAUSE_MIN*2);	break;
				}	}
//				передаем информационный бит
					switch(IRTX_uint_CODING){
						case IR_PAUSE_LENGTH:							IRTX_func_DELAY(1			, IRTX_uint_BIT_PULSE_MIN);
																		IRTX_func_DELAY(0			, IRTX_var_J?IRTX_uint_BIT_PAUSE_MAX:IRTX_uint_BIT_PAUSE_MIN); break;
						case IR_PULSE_LENGTH:							IRTX_func_DELAY(0			, IRTX_var_J?IRTX_uint_BIT_PULSE_MAX:IRTX_uint_BIT_PULSE_MIN);
																		IRTX_func_DELAY(1			, IRTX_uint_BIT_PAUSE_MIN); break;
						case IR_BIPHASIC: case IR_RS5: case IR_RS5X:	IRTX_func_DELAY(!IRTX_var_J	, IRTX_uint_BIT_PAUSE_MIN);
																		IRTX_func_DELAY( IRTX_var_J	, IRTX_uint_BIT_PAUSE_MIN); break;
						case IR_BIPHASIC_INV: case IR_NRC: case IR_RS6:	IRTX_func_DELAY( IRTX_var_J	, IRTX_uint_BIT_PAUSE_MIN);
																		IRTX_func_DELAY(!IRTX_var_J	, IRTX_uint_BIT_PAUSE_MIN); break;
					}
			}
//			отправляем сигнал стоп
			if(IRTX_flag_STOP){IRTX_func_DELAY(0,IRTX_uint_STOP_PAUSE); IRTX_func_DELAY(1,IRTX_uint_STOP_PULSE);}
//			устанавливаем на выходе уровень не активного состояния
			IRVV.IRTX_pins_SEND_STATUS=0; digitalWrite(IRVV.IRTX_pins_NUM, IRVV.IRTX_flag_SEND_INVERT?!i:i);
//			устанавливаем таймер на частоту приёма
			IRXX.IRXX_func_TIMER2_SETREG(20); IRVV.IRXX_flag_SEND=0;
}

//			передача импульса или паузы на светодиод
void		iarduino_IR_TX::IRTX_func_DELAY(bool i, uint32_t j){
			uint32_t k=micros();																															//	сохраняем время вызова функции
			if(frequency)	{IRVV.IRTX_pins_SEND_STATUS=i; IRVV.IRTX_pins_SEND_DATA=1;}																		//	если несущая частота присутствует, то устанавливаем флаг наличия несущей частоты (IRTX_pins_SEND_STATUS) и флаг состояния на выходе IRTX_pins_NUM (IRTX_pins_SEND_DATA)
			else			{digitalWrite(IRVV.IRTX_pins_NUM, IRVV.IRTX_flag_SEND_INVERT?!i:i);}															//	если передача данных осуществляется без несущей частоты, то просто устанавливаем состояние на выходе IRTX_pins_NUM
			j*=50; k+=j; while(micros()<=k){}																												//	переводим мкс/50 в мкс, приращаем время вызова данной функции на время ожидания и ждём пока оно не истечёт
}

//			установка значений регистров таймера под нужную частоту
void		iarduino_IR_XX::IRXX_func_TIMER2_SETREG(uint32_t i){i*=1000; uint16_t j; uint8_t k;
			if(F_CPU/255/  1<i){j=   1; k=1;}else																											//	определяем значение предделителя j
			if(F_CPU/255/  8<i){j=   8; k=2;}else																											//	и значение битов регистра TCCR2B: CS22-CS20 = k
			if(F_CPU/255/ 32<i){j=  32; k=3;}else
			if(F_CPU/255/ 64<i){j=  64; k=4;}else
			if(F_CPU/255/128<i){j= 128; k=5;}else
			if(F_CPU/255/256<i){j= 256; k=6;}else
			                   {j=1024; k=7;}
			TCCR2A	= 0<<COM2A1	| 0<<COM2A0	| 0<<COM2B1	| 0<<COM2B0	| 1<<WGM21	| 0<<WGM20;																	//	биты COM2... = «0» (каналы A и B таймера отключены), биты WGM21 и WGM20 = «10» (таймер 2 в режиме CTC)
			TCCR2B	= 0<<FOC2A	| 0<<FOC2B	| 0<<WGM22	| k;																								//	биты FOC2... = «0» (без принудительной установки результата сравнения), бит WGM22 = «0» (таймер 2 в режиме CTC), биты CS22-CS20 = k (значение предделителя)
			OCR2A	= (uint8_t)(F_CPU/(j*i))-1;																												//	значение регистра сравнения OCR2A настраивается под частоту переполнения счётного регистра TCNT2=i.  i=F_CPU/(предделитель*(OCR2A+1)) => OCR2A = (F_CPU/(предделитель*i))-1
			TIMSK2	= 0<<OCIE2B	| 1<<OCIE2A	| 0<<TOIE2;																										//	разрешаем прерывание по совпадению счётного регистра TCNT2 и регистра сравнения OCR2A
			SREG	= 1<<7;																																	//	устанавливаем флаг глобального разрешения прерываний 
			delay(1); IRVV.IRRX_pins_READ_DATA=IRVV.IRTX_pins_SEND_DATA=IRVV.IRRX_uint_READ_STATUS=IRVV.IRTX_pins_SEND_STATUS=IRVV.IRRX_flag_CHECK=IRVV.IRRX_flag_KEY_PRESS=IRVV.IRRX_flag_READ_REPEAT=IRVV.IRRX_flag_READ_PULSE=IRVV.IRRX_uint_PACK_LENGTH=IRVV.IRTX_uint_CALL_PAUSE=IRVV.IRRX_uint_PACK_PAUSE=IRVV.IRRX_uint_PACK_INDEX=IRVV.IRRX_uint_PACK_NUM=IRVV.IRRX_uint_PACK_LEN[0]=IRVV.IRRX_uint_PACK_LEN[1]=0;
}

//			расшифровка строки протокола
uint8_t		iarduino_IR_XX::IRXX_func_DECODING(const char *i, uint8_t j){
			uint8_t k=0;
			if(j<16 ){k = i[j ]&0x3F;    }																													//	0-тип кодировки, 1-несущая частота, 2-длина первого (информационного) пакета (бит), 3-длина второго (повторного) пакета (бит), 11-позиция бита toggle или рестарт в пакете, 12-максимальная длительность импульса (мкс/50), 13-минимальная  длительность импульса (мкс/50), 14-максимальная длительность паузы (мкс/50), 15-минимальная  длительность паузы (мкс/50)
			if(j==4 ){k|=(i[21]&0x0C)<<4;}else																												//	длительность паузы между пакетами (мс)
			if(j==5 ){k|=(i[21]&0x03)<<6;}else																												//	длительность импульса старт (мкс/50)
			if(j==6 ){k|=(i[22]&0x30)<<2;}else																												//	длительность паузы старт (мкс/50)
			if(j==7 ){k|=(i[22]&0x0C)<<4;}else																												//	длительность импульса стоп (мкс/50)
			if(j==8 ){k|=(i[22]&0x03)<<6;}else																												//	длительность паузы стоп (мкс/50)
			if(j==9 ){k|=(i[23]&0x30)<<2;}else																												//	длительность импульса рестарт или toggle (мкс/50)
			if(j==10){k|=(i[23]&0x0C)<<4;}else																												//	длительность паузы рестарт или toggle (мкс/50)
			if(j==16){k = i[24]&0x20?1:0;}else																												//	флаг наличия стартового импульса
			if(j==17){k = i[24]&0x10?1:0;}else																												//	флаг наличия стопового импульса
			if(j==18){k = i[24]&0x08?1:0;}else																												//	флаг наличия бита toggle
			if(j==19){k = i[23]&0x03;    }else																												//	тип повторного пакета (0-нет, 1-с инверсными битами, 2-идентичен информационному, 3-уникален)
			if(j==20){k =(i[16]&0x3F)<<2 | (i[17]&0x30)>>4;}else																							//	1ый байт значения или битов инверсии пакета повтора
			if(j==21){k =(i[17]&0x0F)<<4 | (i[18]&0x3C)>>2;}else																							//	2ой байт значения или битов инверсии пакета повтора
			if(j==22){k =(i[18]&0x03)<<6 | (i[19]&0x3F)>>0;}else																							//	3ий байт значения или битов инверсии пакета повтора
			if(j==23){k =(i[20]&0x3F)<<2 | (i[21]&0x30)>>4;}																								//	4ый байт значения или битов инверсии пакета повтора
			return k;
}

//			запуск по вектору прерывания TIMER2_COMPA_vect - совпадение счётного регистра TCNT2 и регистра сравнения OCR2A
/* ISR */	ISR(TIMER2_COMPA_vect){
			if(IRVV.IRXX_flag_SEND){																														//	Если передаём данные
				if(IRVV.IRTX_pins_SEND_STATUS){IRVV.IRTX_pins_SEND_DATA=!IRVV.IRTX_pins_SEND_DATA;}else{IRVV.IRTX_pins_SEND_DATA=IRVV.IRTX_flag_SEND_INVERT;}	
				digitalWrite(IRVV.IRTX_pins_NUM, IRVV.IRTX_pins_SEND_DATA);																					//	устанавливаем состояние IRTX_pins_SEND_DATA, на выводе передатчика IRTX_pins_NUM
			}else{																																			//	Если принимаем данные
				IRVV.IRRX_pins_READ_DATA=digitalRead(IRVV.IRRX_pins_NUM);																					//	читаем состояние вывода приёмника IRRX_pins_NUM, в переменную IRRX_pins_READ_DATA
				if (IRVV.IRTX_uint_CALL_PAUSE<255){IRVV.IRTX_uint_CALL_PAUSE++;}																			//	инкрементируем паузу между вызовами функции send
				if (IRVV.IRRX_flag_READ_INVERT){IRVV.IRRX_pins_READ_DATA=IRVV.IRRX_pins_READ_DATA?0:1;}														//	инвертируем переменную IRRX_pins_READ_DATA, если установлен флаг инверсии сигналов
				if (IRVV.IRRX_uint_READ_STATUS!=1 && IRVV.IRRX_uint_READ_STATUS!=3){																		//	если пакеты 1 или 2 не принимаются в данный момент
					if (IRVV.IRRX_pins_READ_DATA){																											//	и появился импульс, то он является первым в пакете
						if(IRVV.IRRX_uint_READ_STATUS==2){IRVV.IRRX_uint_PACK_PAUSE=IRVV.IRRX_uint_PACK_LENGTH;}											//	сохраняем паузу между 1 и 2 пакетами
						IRVV.IRRX_flag_READ_PULSE	= 1;																									//	устанавливаем флаг состояния сигнала в данный момент времени в 1 - PULSE; (0-PAUSE)
						IRVV.IRRX_uint_PACK_INDEX	= 0;																									//	устанавливаем индекс в массиве данных в 0 - первый бит пакета
						IRVV.IRRX_uint_PACK_LENGTH	= 0;																									//	устанавливаем длительность текущего импульса в 0
						if(IRVV.IRRX_uint_READ_STATUS==6){IRVV.IRRX_uint_READ_STATUS=0;}																	//	устанавливаем статус 0, считаем что после паузы в IR_INTERVAL_PRESS мс нажата новая кнопка пульта
						if(IRVV.IRRX_uint_READ_STATUS==0){IRVV.IRRX_uint_PACK_NUM=0; IRVV.IRRX_uint_PACK_LEN[0]=0; IRVV.IRRX_uint_PACK_LEN[1]=0; IRVV.IRRX_flag_CHECK=0; IRVV.IRRX_flag_KEY_PRESS=0; IRVV.IRRX_flag_READ_REPEAT=1;}else	//	устанавливаем номер массива в 0 - первый пакет, обнуляем длину первого и второго пакета
						if(IRVV.IRRX_uint_READ_STATUS==2){IRVV.IRRX_uint_PACK_NUM=1;}																		//	устанавливаем номер массива в 1 - второй пакет
						if(IRVV.IRRX_uint_READ_STATUS<5){IRVV.IRRX_uint_READ_STATUS++;}																		//	устанавливаем очередной статус состояния приёма
					}else{																																	//	и импульс не появляется
						if(IRVV.IRRX_uint_READ_STATUS>0){IRVV.IRRX_uint_PACK_LENGTH++;}																		//	инкрементируем данные о длительности паузы
						if(IRVV.IRRX_uint_PACK_LENGTH>IR_INTERVAL_PRESS*20){IRVV.IRRX_uint_READ_STATUS=6;}													//	если длительность паузы превышает минимальный интервал между нажатиями клавиш, устанавливаем статус 6 (IR_INTERVAL_PRESS*20 значит преобразуем мс в мкс/50)
						if(IRVV.IRRX_uint_PACK_LENGTH>60000){IRVV.IRRX_uint_READ_STATUS=0;}																	//	если длительность паузы превышает 3сек устанавливаем статус 0
						if(IRVV.IRRX_uint_READ_STATUS==5){																									//	если уже принято 2 и более пакетов
							if(IRVV.IRRX_uint_PACK_LENGTH==IR_INTERVAL_PACK*20){																			//	если длительность паузы равна минимальному интервалу между пакетами (IR_INTERVAL_PACK*20 значит преобразуем мс в мкс/50)
								IRVV.IRRX_flag_READ_REPEAT=1;																								//	устанавливаем флаг наличия повторного пакета старше второго (удерживается клавиша на пульте)
							}
						}
					}
				}
				if (IRVV.IRRX_uint_READ_STATUS==1 || IRVV.IRRX_uint_READ_STATUS==3){																		//	если принимается 1ый или 2ой пакет
					if (IRVV.IRRX_pins_READ_DATA==IRVV.IRRX_flag_READ_PULSE){																				//	если продолжается прием предыдущего импульса или паузы
						IRVV.IRRX_uint_PACK_LENGTH++;																										//	инкрементируем данные о длительности импульса или паузы
						if (!IRVV.IRRX_flag_READ_PULSE){																									//	если принимается пауза
							if(IRVV.IRRX_uint_PACK_LENGTH>IR_INTERVAL_PACK*20){																				//	и её длительность превышает минимальный интервал между пакетами (IR_INTERVAL_PACK*20 значит преобразуем мс в мкс/50)
								IRVV.IRRX_uint_READ_STATUS++;																								//	считаем что пакет принят
								IRVV.IRRX_uint_PACK_LEN[IRVV.IRRX_uint_PACK_NUM]=IRVV.IRRX_uint_PACK_INDEX;													//	сохраняем длину пакета
							}
						}
					}else{																																	//	если импульс сменился на паузу, или наоборот
						IRVV.IRRX_flag_READ_PULSE=IRVV.IRRX_pins_READ_DATA;																					//	указываем что состояние сигнала в данный момент времени равно состоянию IRRX_pins_READ_DATA
						if (!IRVV.IRRX_uint_PACK_INDEX<68){																									//	если пакет не превышает длину массива
							IRVV.IRRX_mass_PACK[IRVV.IRRX_uint_PACK_NUM][IRVV.IRRX_uint_PACK_INDEX]=IRVV.IRRX_uint_PACK_LENGTH;								//	записываем длину импульса или паузы в массив
							IRVV.IRRX_uint_PACK_LENGTH=0;																									//	обнуляем длительность импульса или паузы
							IRVV.IRRX_uint_PACK_INDEX++;																									//	увеличиваем индекс очередного элемента в массиве
						}
					}
				}
			}
}