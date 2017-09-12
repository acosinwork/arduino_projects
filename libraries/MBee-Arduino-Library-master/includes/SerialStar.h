/* "Системы модули и компоненты" ("СМК"). 2017. Москва.
Библиотека MBee-Arduino.
Распространяется свободно. Надеемся, что программные продукты, созданные
с помощью данной библиотеки будут полезными, однако никакие гарантии, явные или
подразумеваемые не предоставляются. */

#ifndef SerialStar_h
#define SerialStar_h

//Режим последовательного интерфейса должен быть установлен в пакетный с escape-символами в направлении Модуль->Хост. В направлении хост->модуль пакетный режим без escape-символов.
#define ATAP 2 

//Escape-символы.
#define START_BYTE 0x7E
#define ESCAPE 0x7D
#define XON 0x11
#define XOFF 0x13

//Коды ошибок при разборе API-фрейма.
#define NO_ERROR 0
#define CHECKSUM_FAILURE 1
#define PACKET_EXCEEDS_BYTE_ARRAY_LENGTH 2
#define UNEXPECTED_START_BYTE 3

//Статус локального ответа API-фрейма.
#define OK_COMMAND_STATUS 0
#define ERROR_COMMAND_STATUS  1 //Недостаточно памяти для размещения пакета.
#define INVALID_CODE_COMMAND_STATUS 2 //Недопустимый код команды.
#define INVALID_PARAMETER_COMMAND_STATUS 3 //Недопустимое значение параметра.
#define TX_FAILURE_COMMAND_STATUS 4 //Пакет в эфир не отправлен вследствие занятости рабочей частоты.

//Параметры поля Receive options.
#define PACKET_ACKNOWLEDGED_BIT 0
#define PACKET_WAS_BROADCAST_BIT  1

#define API_ID_INDEX 3 //Тип API-фрейма всегда третий байт фрейма (считая с 0).
#define AT_COMMAND_API_LENGTH 2 //2 байта символы AT-команды.
#define REMOTE_AT_COMMAND_API_LENGTH 5 //Адрес удаленного модема 2 байта + Опции 1 байт + Символы AT-команды 2 байта.
#define DEFAULT_FRAME_ID 1

//Константы ответа TRANSMIT_STATUS_API_FRAME.
#define	SUCCESS 0x0

//Биты регистра DM (Device Mode).
#define DEVICE_MODE_BROADCAST_RECEIVE_BIT 0 //Если данный бит равен 1, то прием широковещательных пакетов запрещен.
#define DEVICE_MODE_CCA_BIT               4 //Бит, управляющий режимом CCA при передаче пакетов в прозрачном режиме UART. Если равен 1, то CCA отключено.
#define DEVICE_MODE_ENCRYPTION_BIT        5 //Бит, управляющий шифрованием трафика в прозрачном режиме UART. Если равен 1, то шифрование включено.

//Параметры поля transmitOptions.
#define ACKNOWLEDGE_DISABLE_TX_OPTIONS_BIT 0
#define CCA_DISABLE_TX_OPTIONS_BIT DEVICE_MODE_CCA_BIT //Номер бита, для удобства совпадает с аналогичным битом поля регистра mode.
#define ENCRYPTION_ENABLE_TX_OPTIONS_BIT DEVICE_MODE_ENCRYPTION_BIT //Номер бита, для удобства совпадает с аналогичным битом поля регистра mode.
#define SLEEP_DEVICE_TX_OPTIONS_BIT 6 //Бит, равный 1, если сообщение предназначено для спящего узла и помещается в буфер, а не передается сразу в эфир.
   
//Параметры поля Remote command options.
#define ACKNOWLEDGE_DISABLE_COMMAND_OPTIONS_BIT   ACKNOWLEDGE_DISABLE_TX_OPTIONS_BIT
#define APPLY_CHANGES_COMMAND_OPTION_BIT          1 //Применяет сделанные изменения и записывает во флеш обновленные параметры.
#define APPLY_CHANGES_NO_SAVE_COMMAND_OPTION_BIT  2 //Применяет сделанные изменения без сохранения их во флеш.
#define CCA_DISABLE_COMMAND_OPTION_BIT            DEVICE_MODE_CCA_BIT //Для удобства совпадает с битом аналогичного назначения поля transmitOptions и регистра Device Mode.
#define ENCRYPTION_ENABLE_COMMAND_OPTION_BIT      DEVICE_MODE_ENCRYPTION_BIT //Для удобства совпадает с битом аналогичного назначения поля transmitOptions и регистра Device Mode.
#define SLEEP_DEVICE_COMMAND_OPTIONS_BIT          SLEEP_DEVICE_TX_OPTIONS_BIT //Для удобства совпадает с битом аналогичного назначения поля transmitOptions и регистра Device Mode.

//Параметры поля Receive options.
#define NO_RECEIVE_OPTIONS 0
#define PACKET_ACKNOWLEDGED_BIT 0
#define PACKET_WAS_BROADCAST_BIT  1

#define MAX_FRAME_DATA_SIZE 45 //2 байта адреса + 1 байт RSSI + 1 байт options + 40 байт данные)
#define BROADCAST_ADDRESS 0xFFFF

//Значения "по умолчанию".
#define TRANSMIT_OPTION_DEFAULT 0

//Линии ввода/вывода.

//Состояние цифрового входа/выхода.
#define LOW  0
#define HIGH 1

//Идентификаторы линий ввода/вывода. Идентификаторы соответствуют номерам физических выводов модуля MBee-868-x.0.
#define L0_ID   2
#define L1_ID   3
#define L2_ID   4
#define L3_ID   6
#define L4_ID   7
#define L5_ID   9
#define L6_ID   11
#define L7_ID   12
#define L8_ID   13

#define B0_ID   14
#define B1_ID   15
#define B2_ID   16
#define B3_ID   17
#define B4_ID   18
#define B5_ID   19

#define R0_ID   35
#define R1_ID   34
#define R2_ID   33
#define R3_ID   32
#define R4_ID   31
#define R5_ID   30
#define R6_ID   29
#define R7_ID   28
#define R8_ID   27
#define R9_ID   24

//Режимы линий ввода/вывода.
#define	IO_DISABLED	0
#define IO_NOT_AVAILABLE	1
#define IO_ADC	2
#define IO_DIGITAL_INPUT	3
#define IO_DIGITAL_OUTPUT_LO	4
#define IO_DIGITAL_OUTPUT_HI	5
#define IO_UART_TX	6 
#define IO_UART_RX	7
#define IO_UART_CTS	8 
#define IO_UART_RTS 9
#define IO_SYSTEM_LED	10
#define IO_SLEEP_REQUEST	11
#define IO_SLEEP_STATUS	12
#define IO_COUNTER_INPUT1	13
#define IO_COUNTER_INPUT2	14
#define IO_WAKEUP_INPUT_FALLING_EDGE	15
#define IO_WAKEUP_INPUT_RISING_EDGE	16
#define IO_PWM1	17
#define IO_PWM2	18
#define IO_PWM3 19
#define IO_PWM4 20
#define IO_RS485_DIRECTION	21

//Макросы.
#define BV(n)	(1 << (n))
	
/**
 * Идентификаторы API-фрейма в мнемониках проекта SerialStar.
 */
#define	MODEM_STATUS_API_FRAME															0x8A //ex. MODEM_STATUS_RESPONSE.
#define	TRANSMIT_STATUS_API_FRAME														0x8B //ex. ZB_TX_STATUS_RESPONSE.
#define	AT_COMMAND_IMMEDIATE_APPLY_API_FRAME 								0x07 //Идентификатор введен SysMC. AT-команда, содержащаяся во фрейме, применяется непосредственно после получения без сохранения измененного параметра во флеше.
#define	AT_COMMAND_RESPONSE_IMMEDIATE_APPLY_API_FRAME 			0x87
#define	AT_COMMAND_API_FRAME																0x08 //ex. AT_COMMAND_REQUEST.
#define	AT_COMMAND_RESPONSE_API_FRAME												0x88 //ex. AT_COMMAND_RESPONSE.
#define	AT_COMMAND_QUEUE_PARAMETER_VALUE_API_FRAME					0x09 //ex. AT_COMMAND_QUEUE_REQUEST.
#define AT_COMMAND_RESPONSE_QUEUE_PARAMETER_VALUE_API_FRAME 0x89 //ex. TX_STATUS_RESPONSE. Идентификатор не имеет аналога в проекте SerialStar.
#define	REMOTE_AT_COMMAND_REQUEST_API_FRAME									0x17 //ex. REMOTE_AT_REQUEST.
#define	REMOTE_AT_COMMAND_RESPONSE_API_FRAME								0x97 //ex. REMOTE_AT_COMMAND_RESPONSE.
#define	TRANSMIT_REQUEST_NO_OPTIONS_API_FRAME 							0x0F //Идентификатор, введенный SysMC, предназначенный для передачи данных без байта опций. Нужен для увеличения полезной нагрузки.
#define TRANSMIT_REQUEST_API_FRAME													0x10	//ex. ZB_TX_REQUEST.
#define TRANSMIT_REQUEST_PRO_API_FRAME											0x01 //ex. TX_16_REQUEST.
#define REMOTE_ACKNOWLEDGE_API_FRAME												0x8C //Идентификатор, введенный SysMC, предназначенный для подтверждения приема пакетов удаленным модемом. 
#define RECEIVE_PACKET_API_FRAME														0x81 //ex. RX_16_RESPONSE.
#define RECEIVE_PACKET_NO_OPTIONS_API_FRAME									0x8F //Идентификатор, введенный SysMC, используемый при выдаче в UART сообщений, переданных удаленным модемом в прозрачном режиме или в пакетном режиме без опций (TRANSMIT_REQUEST_NO_OPTIONS_API_FRAME).
#define IO_DATA_SAMPLE_API_FRAME														0x83 //ex. RX_16_IO_RESPONSE.

/**
Супер класс для всех пакетов, передаваемых по UART модулями MBee-868-x.0 в проекте SerialStar.
Запрещается создавать объекты непосредственно этого класса. Необходимо использовать подклассы
для соответствующих проектов.
Для экономии памяти рекомендуется повторное использование объектов подклассов.
*/ 
class MBeeResponse
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	MBeeResponse();
	
	/**
	Возвращает тип API-фрейма.
	*/
	uint8_t getApiId();
	
	void setApiId(uint8_t apiId);
	
	/**
	Возвращает MSB длины поля данных.
	*/
	uint8_t getMsbLength();
	
	void setMsbLength(uint8_t msbLength);
	/**
	Возвращает LSB длины поля данных.
	*/
	uint8_t getLsbLength();
	
	void setLsbLength(uint8_t lsbLength);
	/**
	 Подсчет контрольной суммы.
	*/
	uint8_t getChecksum();
	
	/**
	 Запись контрольной суммы в соответствующее поле пакета.
	*/
	void setChecksum(uint8_t checksum);
	
	/**
	Возвращает длину данных в пакете между полем типа API-фрейма и контрольной суммой. 
	*/
	uint8_t getFrameDataLength();
	
	/**
	Запись поля длины в пакете. 
	*/
	void setFrameLength(uint8_t frameLength);
	
	/**
	Устанавливает указатель на поле данных пакета. 
	*/
	void setFrameData(uint8_t* frameDataPtr);
	
	/**
	Возвращает указатель на буфер, который содержит сообщение. Первым байтом сообщения является байт, следующий 
	непосредственно за байтом типа API-фрейма (API ID). Последним байтом является байт перед контрольной суммой.
	Такое трактование отличается от определения длины API-фрейма, которое включает в себя поле типа API-фрейма.
	Однако, поскольку формат каждого API-фрейма предполагает наличие на 4-ой позиции байта API ID, то смысла хранить
	его еще раз (а поле API ID хранится в особой переменной apiId) нет.
	*/
	uint8_t* getFrameData();

	/**
	Возвращает длину пакета.
	*/
	uint16_t getPacketLength();
	
	/**
	Сбрасывает все поля пакета.
	*/
	void reset();
	
	/**
	Инициализация пакета. 
	*/
	void init();
	
	/**
	Вызов TxStatusResponse при getApiId() == TRANSMIT_STATUS_API_FRAME 0x8B(ex. ZB_TX_STATUS_RESPONSE).
	*/
	void getTxStatusResponse(MBeeResponse &response);
	
	/**
	Вызов RxResponse при getApiId() == RECEIVE_PACKET_API_FRAME 0x81(ex. RX_16_RESPONSE). 
	*/
	void getRxResponse(MBeeResponse &response);
	
	/**
	Вызов RxIoSampleResponse при getApiId() == IO_DATA_SAMPLE_API_FRAME 0x83 (ex. RX_16_IO_RESPONSE).
	*/
	void getRxIoSampleResponse(MBeeResponse &response);
	
	/**
	Вызов ModemStatusResponse при getApiId() == MODEM_STATUS_API_FRAME 0x8A (ex. MODEM_STATUS_RESPONSE).
	*/
	void getModemStatusResponse(MBeeResponse &response);
		
	/**
	Вызов AtCommandResponse при getApiId() == AT_COMMAND_RESPONSE_API_FRAME 0x88 (ex. AT_COMMAND_RESPONSE).
	*/
	void getAtCommandResponse(MBeeResponse &responses);
	
	/**
	Вызов RemoteAtCommandResponse при getApiId() == REMOTE_AT_COMMAND_RESPONSE_API_FRAME 0x97 (ex. REMOTE_AT_COMMAND_RESPONSE).
	*/
	void getRemoteAtCommandResponse(MBeeResponse &response);
	
	/**
  Вызов RxAcknowledgeResponse при getApiId() == REMOTE_ACKNOWLEDGE_API_FRAME 0x8C.
	*/
	void getRxAcknowledgeResponse(MBeeResponse &response);

	/**
	Возвращает true, если пакет успешно принят.
	*/ 
	bool isAvailable();
	
	/**
	Устанавливает признак доступности пакета.
	*/ 
	void setAvailable(bool complete);
	
	/**
	Возвращает true если при разборе пакета произошли ошибки.
	*/
	bool isError();
	
	/**
	Возвращает код ошибки или 0, если их нет.
	Возможные коды ошибок: CHECKSUM_FAILURE, PACKET_EXCEEDS_BYTE_ARRAY_LENGTH, UNEXPECTED_START_BYTE.
	*/
	uint8_t getErrorCode();
	
	/**
	Устанавливает код ошибки.
	*/
	void setErrorCode(uint8_t errorCode);
	
protected:
	uint8_t* _frameDataPtr;
private:
	void setCommon(MBeeResponse &target);
	uint8_t _apiId;
	uint8_t _msbLength;
	uint8_t _lsbLength;
	uint8_t _checksum;
	uint8_t _frameLength;
	bool _complete;
	uint8_t _errorCode;
};

/**
Класс расширяет базовый класс MBeeResponse для всех сообщений, которые имеют поле frame id.
*/ 
class FrameIdResponse : public MBeeResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	FrameIdResponse();
	
	/**
	Возвращает идентификатор фрейма.
	*/	
	uint8_t getFrameId();
private:
	uint8_t _frameId;
};

/**
Класс для API-фрейма со статусом модема после инициализации.
*/
class ModemStatusResponse : public MBeeResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	ModemStatusResponse();
	
	/**
	Возвращает статус модема после рестарта.
	*/
	uint8_t getStatus();
	static const uint8_t API_ID = MODEM_STATUS_API_FRAME;
};

/**
Базовый класс для всех пакетов, принятых по эфиру и передаваемых модулем на UART.
*/
class RxCommonResponse : public MBeeResponse
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	RxCommonResponse();
	
	/**
	Возвращает адрес модема, отправившего пакет.
	*/
	uint16_t getRemoteAddress();
	
	/**
	Возвращает RSSI на антенном входе модуля, зафиксированный в момент приема пакета. Число со знаком в дополнительном коде.
	*/
	uint8_t getRssi();
	
	/**
	Возвращает байт опций.
	*/
	uint8_t getOption();
};
 
/**
Класс для ответа на локальный командный API-фрейм.
*/
class AtCommandResponse : public FrameIdResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	AtCommandResponse();
	/**
	Возвращает указатель на массив, содержащий 2 ASCII символа AT-команды.
	*/ 
	uint8_t* getCommand();

	/**
	 Возвращает статус команды. Если 0, значит команда выполнена успешно.
	*/		 
	uint8_t getStatus();

	/**
	Возвращает указатель на массив, содержащий параметр команды.
	Применяется только для команд запроса.
	*/		 
	uint8_t* getValue();

	/**
	Возвращает длину поля параметра команды.
	*/
	uint8_t getValueLength();
	
	/**
	Возвращает true, если статус эквивалентен AT_OK.
	*/ 
	bool isOk();
};

/**
Класс для локального ответа модуля со статусом передачи в эфир или размещением в буфере.
*/
class TxStatusResponse : public FrameIdResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	TxStatusResponse();
	
	/**
	Возвращает статус локальной команды.
	*/
	uint8_t getStatus();
	
	/**
	Возвращает true, если команда исполнена.
	*/
	bool isSuccess();
	
	/**
	Возвращает API ID выполненной команды.
	*/
	uint8_t getApiId();
};


/**
Класс для ответа на командный API-фрейм для удаленного узла.
*/
class RemoteAtCommandResponse : public RxCommonResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	RemoteAtCommandResponse();
	
	/**
	Возвращает указатель на массив, содержащий 2 ASCII символа AT-команды.
	*/ 
	uint8_t* getCommand();
	
	/**
	 Возвращает статус команды. Если статус 0, значит команда выполнена успешно.
	*/	
	uint8_t getStatus();
	
	/**
	Возвращает указатель на массив, содержащий параметр команды.
	Применяется только для команд запроса параметра.
	*/	
	uint8_t* getValue();
	
	/**
	Возвращает длину поля параметра команды.
	*/
	uint8_t getValueLength();
	
	/**
	Возвращает true, если команда успешно выполнена удаленным модулем.
	*/
	bool isOk();
};

/**
Класс для пакета с подтверждением получения неструктурированных данных удаленным модемом.
*/
class RxAcknowledgeResponse : public RxCommonResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	RxAcknowledgeResponse();
	/**
	Возвращает frameId пакета, полученного удаленным модулем на который отправлено это подтверждение.
	Может использоваться хостом для проверки доставки пакетов.
	*/
	uint8_t getFrameId();
};

/**
Класс для всех пакетов, принятых по эфиру, передаваемых модулем на UART и содержащих поле данных.
*/
class RxDataResponse : public RxCommonResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	RxDataResponse();
	
	/**
	Возвращает байт из поля данных принятого пакета в соответствии с номером, определяемом 
	переменной index. Значение index может находится в диапазоне 0 to getDataLength() - 1.
	*/
	uint8_t getData(uint8_t index);
	
	/**
	Возвращает указатель на поле данных принятого пакета.
	*/
	uint8_t* getData();
	
	/**
	Возвращает true, если принятый пакет был широковещательный.
	*/
	bool isAddressBroadcast();
	
	/**
	Возвращает длину поля данных.
	*/
	virtual uint8_t getDataLength() = 0;
	
	/**
	Возвращает номер байта в принятом пакете с которого начинается поле данных.
	*/
	virtual uint8_t getDataOffset() = 0;
};

class RxIoSampleResponse : public RxDataResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	RxIoSampleResponse();
	
	/**
	Возвращает длину поля данных, не включая 2 байт с температурой и напряжением источника питания удаленного модема.
	*/
	uint8_t getDataLength();
	
	uint8_t getDataOffset();
	
	/**
	Возвращает число линий ввода/вывода данные о текущем состоянии которых имеются в принятом пакете.
	*/
	uint8_t getSampleSize();
	
	/**
	Возвращает температуру, измеренную на удаленном модеме. Температура измеряется с помощью встроенного датчика. Значение 
	представляет собой число в дополнительном коде со знаком. Если число меньше, чем 128, то это и есть текущая температура.
	Если число больше 128-и, то его нужно вычесть из 256 и поменять знак.
	*/
	uint8_t getTemperature();
	
	/**
	Возвращает напряжение источника питания удаленного модема. Для расчета значения напряжения в вольтах, необходимо
	полученное число разделить на 51.
	*/
	uint8_t getVbatt();
	
	/**
	Возвращает режим линии ввода/ввода по порядковому номеру выборки в принятом пакете.
	*/
	uint8_t getMode(uint8_t number);
	
	/**
	Возвращает номер вывода модуля, соответствующий порядковому номеру выборки в принятом пакете.
	*/
	uint8_t getPin(uint8_t number);
	
	
	/**
	Возвращает true, если информация о текущем состоянии данного вывода модуля имеется в принятом пакете.
	*/
	bool isAvailable(uint8_t pin);
	
	/**
	Возвращает текущее состояние аналогового входа, соответствующего порядковому номеру выборки в принятом пакете.
	*/
	uint16_t getAnalog(uint8_t number);
	
	/**
	Возвращает текущее состояние цифрового входа/выхода, соответствующего порядковому номеру выборки в принятом пакете.
	*/
	uint8_t getDigital(uint8_t number);
	
	/**
	Возвращает число подсчитанных импульсов для счетного входа, соответствующего порядковому номеру выборки в принятом пакете.
	*/
	uint32_t getCounter(uint8_t number);

private:
	void getNextSample(uint8_t &sample); //Увеличивает sample на число байт, соответствующее длине данных о текущем состоянии, которая зависит от типа входа/выхода.
	uint8_t getSample(uint8_t number); //Возвращает смещение первого байта (идентификатора линии ввода/вывода) относительно начала поля данных для данного номера выборки в пакете. 
};


/**
Класс для выдачи в UART принятых по эфиру пакетов, содержаших неструктированные данные.
*/
class RxResponse : public RxDataResponse 
{
public:
	/**
	Конструктор "по умолчанию".
	*/
	RxResponse();
	
	/**
	Возвращает длину поля данных пакета.
	*/
	uint8_t getDataLength();
	
	/**
	Возвращает смещение поля данных относительно старшего байта адреса.
	*/
	uint8_t getDataOffset();
	
	/**
	Возвращает true, если на принятый пакет было отправлено подтверждение.
	*/
	bool isAcknowledged();
};

/**
Супер класс для всех пакетов, передаваемых от хоста к модулям MBee-868-x.0 в проекте SerialStar.
Запрещается создавать объекты непосредственно этого класса. Необходимо использовать подклассы
для соответствующих проектов.
Для экономии памяти рекомендуется повторное использование объектов подклассов.
*/ 
class MBeeRequest 
{
public:
	/**
	Конструктор, создающий фрейм с заданным API ID и идентификатором.
	*/
	MBeeRequest(uint8_t apiId, uint8_t frameId);
	
	/**
	Устанавливает frame id.  Диапазон значений 1..255, чтобы получить ответ. Если установлен в 0, ответный фрейм не выдается.
	*/
	void setFrameId(uint8_t frameId);
	
	/**
	Возвращает frame id.
	*/
	uint8_t getFrameId();
	
	/**
	Устанавливает тип API-фрейма.
	*/ 
	void setApiId(uint8_t apiId);
		
 	/**
	Возвращает тип API-фрейма.
	*/
	uint8_t getApiId();
	/** Возвращает буфер, который содержит сообщение. Первым байтом сообщения является байт, следующий 
	непосредственно за байтом типа API-фрейма (API ID). Последним байтом является байт перед контрольной суммой.
	Такое трактование отличается от определения длины API-фрейма, которое включает в себя поле типа API-фрейма.
	Однако, поскольку формат каждого API-фрейма предполагает наличие на 4-ой позиции байта API ID, то смысла хранить
	его еще раз (а поле API ID хранится в особой переменной apiId) нет.
	*/
	virtual uint8_t getFrameData(uint8_t pos) = 0; //Приравнивание функции 0 делает ее pure virtual (т.е. она обязательна должна быть определена в производных классах)
	/**
	Возвращает длину поля данных фрейма, не включая frame id или api id и checksum.
	*/ 
	virtual uint8_t getFrameDataLength() = 0;

private:
	uint8_t _apiId;
	uint8_t _frameId;
};

/**
Класс для локального управления модулем с помощью API-фрейма.
*/
class AtCommandRequest : public MBeeRequest 
{
public:
	/**
	Конструктор "по умолчанию"
	*/
	AtCommandRequest();
	
	/**
	Создает локальный командный API-фрейм с заданным кодом команды.
	*/
	AtCommandRequest(uint8_t *command);
	
	/**
	Создает локальный командный API-фрейм для установки параметра с заданным кодом команды, значением параметра и длиной поля параметра команды.
	*/
	AtCommandRequest(uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);
	
	/**
	Создает локальный командный API-фрейм для установки паарметра с заданным кодом команды, значением параметра, длиной поля параметра команды и идентификатором фрейма.
	*/
	AtCommandRequest(uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength, uint8_t frameId);
	
	/**
	Возвращает в зависимости о номера байта либо код AT-команды, содержащейся в фрейме, либо байта параметра команды.
	*/
	uint8_t getFrameData(uint8_t pos);
	
	/**
	Возвращает длину командного фрейма команды.
	*/
	uint8_t getFrameDataLength();
	
	/**
	Возвращает указатель на байты с кодом команды.
	*/
	uint8_t* getCommand();
	
	/**
	Устанавливает команду по указателю на байты на кода.
	*/
	void setCommand(uint8_t* command);
	
	/**
	Возвращает указатель на первый байт параметра команды.
	*/
	uint8_t* getCommandValue();
	
	/**
	Устанавливает указатель на первый байт параметра команды.
	*/
	void setCommandValue(uint8_t* value);
	
	/**
	Возвращает длину поля параметра команды.
	*/
	uint8_t getCommandValueLength();
	
	/**
	Устанавливает длину поля параметра команды.
	*/
	void setCommandValueLength(uint8_t length);
	
	/**
	Присваивает указателю на поле кода команды значение NULL и обнуляет поле длины параметра команды.
	*/
  void clearCommandValue();
private:
	uint8_t *_command;
	uint8_t *_commandValue;
	uint8_t _commandValueLength;
};

/**
Базовый класс для всех пакетов, предназначенных для передачи по эфиру неструктурированных данных.
*/
class PayloadRequest : public MBeeRequest 
{
public:
	PayloadRequest(uint8_t apiId, uint8_t frameId, uint8_t *payload, uint8_t payloadLength);
	/**
	Возвращает указатель на поле данных. 
	*/
	uint8_t* getPayload();
	
	/**
	 Устанавливает указатель на поле данных.
	*/
	void setPayload(uint8_t* payloadPtr);

	/**
	 Уставливает инлайном одновременно указатель на поле данных и число байт из него, начиная с первого,
	 которое требуется поместить в передаваемое сообщение.
	*/
	void setPayload(uint8_t* payloadPtr, uint8_t payloadLength) 
	{
		setPayload(payloadPtr);
		setPayloadLength(payloadLength);
	}

	/**
	Возвращает установленную длину поля данных.
	*/
	uint8_t getPayloadLength();
	
	/**
	Устанавливает число байт, которое надо поместить в поле данных отправляемого пакета. Например, если
	буфер, на который указывает указатель, установленный функцией setPayload(), содержит 40 байт, а 
	требуется передать только первые 5, то вызывается эта функция с параметром 5. Значение параметра не
	должно превышать длину буфера.
  */
	void setPayloadLength(uint8_t payloadLength);
private:
	uint8_t* _payloadPtr;
	uint8_t _payloadLength;
};

/**
Класс для удаленного управления модулем с помощью API-фрейма.
*/
class RemoteAtCommandRequest : public AtCommandRequest 
{
public:
	/**
	Конструктор "по умолчанию"
	*/
	RemoteAtCommandRequest();
	
	/**
	Создает команду установки параметра на удаленном модеме с установленными значением параметра
	и полем его длины. По умолчанию устанавливаются следущие параметры отправки:
	1. Подтверждение доставки включено.
	2. Измененное значение параметра на удаленном модуле автоматически применяется и сохраняется в
	   энергонезависимой памяти.
	3. Команда отправляется в режиме CCA (Clear Channel Assessment).
	4. Шифрование выключено.
	5. Команда предназначается для модема, находящегося а активном режиме (команда отправляется сразу
	   после получения, а не помещается в буфер.
	*/
	RemoteAtCommandRequest(uint16_t remoteAddress, uint8_t *command, uint8_t *commandValue, uint8_t commandValueLength);
	
	/**
	Создает команду запроса параметра на требуемом удаленном модеме. 
	По умолчанию устанавливаются следущие параметры отправки:
	1. Подтверждение доставки включено.
	2. Команда отправляется в режиме CCA (Clear Channel Assessment).
	3. Шифрование выключено.
	4. Команда предназначается для модема, находящегося а активном режиме (команда отправляется сразу
	   после получения, а не помещается в буфер.
	*/
	RemoteAtCommandRequest(uint16_t remoteAddress, uint8_t *command);
	uint16_t getRemoteAddress();
	void setRemoteAddress(uint16_t remoteAddress);
	bool getAcknowledge();
	void setAcknowledge(bool acknowledge);
	bool getApplyChanges();
	void setApplyChanges(bool applyChanges);
	bool getSaveChanges();
	void setSaveChanges(bool saveChanges);
	bool getCca();
	void setCca(bool cca);
  bool getEncryption();
	void setEncryption(bool encryption);
	bool getSleepingDevice();
	void setSleepingDevice(bool sleepingDevice);	
	uint8_t getFrameData(uint8_t pos);
	uint8_t getFrameDataLength();
	void setDefault();
private:
	uint16_t _remoteAddress;
	bool _acknowledge;
	bool _applyChanges;
	bool _saveChanges;
	bool _cca;
	bool _encryption;
	bool _sleepingDevice;
};

/**
Класс для пакета,предназначенного для передачи удаленному модему неструктурированных данных, 
которые будут отправлены им на последовательный интерфейс UART. Режим отправки на UART(пакетный
 или прозрачный) зависит от настроек удаленного модуля. 
<p/>
Режимы передачи пакета и получения подтверждений могут определяться байтом option. Для этого 
параметр _optionEnable должен быть установлен в true (режим "по умолчанию").
Если параметр _optionEnable=false, то режимы отправки определяются параметром DM(Device Mode).
Максимальная допустимая длина поля данных пакета зависит от выбранного режима передачи:
1. Если _optionEnable=false, то FrameDataLength <= 40.
2. Если _optionEnable=true, то FrameDataLength <= 39.
3. Если требуется шифрование поля данных(_encryption = true или DEVICE_MODE_ENCRYPTION_BIT = 1),
то FrameDataLength <= 32 независимо от установки _optionEnable.
<p/>
ВНИМАНИЕ!
Каких-либо проверок на превышение допустимой длины поля данных не производится. Если длина поля
данных превышает максимально допустимую для данного режима, то в поле данных помещается макисмально
возможное число. При этом все "лишние" отбрасываются и никакие уведомлений об этом в ответном пакете
не формируются.
*/ 
class TxRequest : public PayloadRequest 
{
public:
	/**
	Конструктор "по умолчанию". Перед отправкой пакета, необходимо как минимум установить указатель
	на буфер с исходными данными, указать количество отправлямых байт, начиная с первого, и адрес
	получателя.
	*/
	TxRequest();
	
	/**
	Класс для адресной отправки данных со значением frameId и опций передачи "по умолчанию".
	*/
	TxRequest(uint16_t addr, uint8_t *payload, uint8_t payloadLength);
	
	/**
	Класс для адресной отправки пакета с заданным frameId и опциями передачи.
	*/
	TxRequest(uint16_t addr, uint8_t option, uint8_t *payload, uint8_t payloadLength, uint8_t frameId);
		
	/**
	Возвращает адрес получателя пакета.
	*/
	uint16_t getRemoteAddress();
	
	/**
	Устанавливает адрес получателя пакета.
	*/
	void setRemoteAddress(uint16_t remoteAddress);
	
	/**
	Возвращает значение байта опций отправки пакета.
	*/
	uint8_t getOption();
	
	/**
	Устанавливает опции отправки пакета.
	*/
	void setOption(uint8_t option);
	
	/**
	Возвращает true, если передача пакета осуществляется в соответствии с байтом опций и 
	false, если в сответствии с настройками Device Mode (DM).
	*/
	bool getOptionEnable();
	
	/**
	Включает/выключает передачу пакетов в соответствии с байтом опций.
	*/
	void setOptionEnable(bool optionEnable);
	
	/**
	Возвращает true, если подтверждение удаленным модемом получения пакета включено.
	*/
	bool getAcknowledge();
	
	/**
	Включает/выключает отправку подтверждения удаленным модемом получения пакета.
	*/
	void setAcknowledge(bool acknowledge);
	
	/**
	Возвращает true, если при отправке пакетов включен режим CCA (Clear Channel Assessment).
	*/
	bool getCca();
	
	/**
	Включает/выключает режим CCA (Clear Channel Assessment) при отправке пакетов.
	*/
	void setCca(bool cca);
	
	/**
	Возвращает true, если включен режим шифрования поля данных.
	*/
  bool getEncryption();
	
	/**
	Включает/выключает шифрование поля данных передаваемого пакета.
	*/
	void setEncryption(bool encryption);
	
	/**
	Возвращает true, если пакет предназначается для спящего модема.
	*/
	bool getSleepingDevice();
	
	/**
	Включает/выключает буферизацию пакетов для спящих модемов.
	*/
	void setSleepingDevice(bool sleepingDevice);	
	
	/**
	Возвращает байт поля данных, номер которого определяется аргументом.
	*/
	uint8_t getFrameData(uint8_t pos);
	
	/**
	Возвращает полную длину поля данных API-фрейма.
	*/
	uint8_t getFrameDataLength();
	
private:
	uint16_t _remoteAddress;
	uint8_t _option;
	bool _optionEnable;
};

/**
Класс для проекта SerialStar диапазона 868 МГц.
*/
class SerialStar : public MBee868
{
public:
	SerialStar();
	void readPacket();
	bool readPacket(int timeout);
	void readPacketUntilAvailable();
	void send(void*);
	void send(MBeeRequest &request);
	
	MBeeResponse& getResponse();
private:
	void sendByte(uint8_t b, bool escape);
	void resetResponse();
	MBeeResponse _response;
	bool _escape;
	uint8_t _pos; //Текущая позиция в буфере. По сути дела представляет собой состояние конечного автомата. 
	uint8_t b; //Последний считанный байт.
	uint8_t _checksumTotal;
	uint8_t _nextFrameId;
	//Буфер входящих сообщений. Содержит только один пакет с данными, начинающимися после поля frameType (api id) и оканчивающимися перед checksum.
	uint8_t _responseFrameData[MAX_FRAME_DATA_SIZE];
	Stream* _serial;
};


#endif //SerialStar_h