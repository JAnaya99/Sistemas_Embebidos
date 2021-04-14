#ifndef RTCCONTROL_H_
#define RTCCONTROL_H_

#define SECONDS_ADDR	0X00
#define MINUTES_ADDR	0X01
#define HOURS_ADDR		0x02
#define DAY_ADDR		0X03
#define DATE_ADDR		0X04
#define MONTH_ADDR		0x05
#define YEAR_ADDR		0X06

union SecondsType {
	struct {
		uint8_t un_seconds	: 4,
				dec_seconds : 3,
				RESERVED	: 1;
	} field;
	uint8_t reg;
};

union MinutesType {
	struct {
		uint8_t un_minutes	: 4,
				dec_minutes : 3,
				RESERVED	: 1;
	} field;
	uint8_t reg;
};

union HoursType {
	struct {
		uint8_t un_hours	: 4,
				dec_hours	: 1,
				is_pm		: 1,
				is_24_fmt	: 1,
				RESERVED	: 1;
	} field;
	uint8_t reg;
};

union DayType {
	struct {
		uint8_t day;
	} field;
	uint8_t reg;
};

union DateType {
	struct {
		uint8_t un_date		: 4,
				dec_date	: 2,
				RESERVED	: 2;
	} field;
	uint8_t reg;
};

union MonthType {
	struct {
		uint8_t un_month	: 4,
				dec_month	: 1,
				RESERVED	: 3;
	} field;
	uint8_t reg;
};

union YearType {
	struct {
		uint8_t un_year		: 4,
				dec_year	: 4;
	} field;
	uint8_t reg;
};

struct RTCType {
	union SecondsType seconds_type;
	union MinutesType minutes_type;
	union HoursType hours_type;
	union DayType day_type;
	union DateType date_type;
	union MonthType month_type;
	union YearType year_type;
};

void I2CInit(void);
void StopCond();
void SendI2CData(uint8_t data);
void sendI2CDataArray(uint32_t slave_addr, uint8_t* dataArray, uint32_t buffer_size);
void receiveI2CDataArray(uint32_t slave_addr, uint8_t* dataArray, uint32_t buffer_size);


#endif /* RTCCONTROL_H_ */