
/*
 * driver.c
 *
 * Created: 23.08.2023 11:19:43
 *  Author: Vanya
 */ 
#include "driver.h"
#include "driver_init.h"
#include "utils.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>



struct spi_m_sync_descriptor EXT_SPI;
struct spi_m_sync_descriptor RF_SPI;
struct i2c_m_sync_desc EXT_I2C;


struct io_descriptor *io;
struct io_descriptor *I2C_io;

struct io_descriptor *lcd_spi;
struct io_descriptor *rf_spi;



void RF_IRQ_Enable(void){
	ext_irq_register(RF_IRQ, RF_int_Handler);
	spi_m_sync_enable(&RF_SPI);
}


uint8_t int_count = 0;
bool rf_isReady(){
	if (int_count != 0){
		data_ready();
		int_count = 0;
		return true;
	}
	return false;
}

void RF_int_Handler(void){
	int_count++;
	//gpio_toggle_pin_level(LED);
	
}



uint8_t RFM69_read_reg(uint8_t addr){
	//uint8_t *addr_ptr = &addr;
	//struct io_descriptor *io;
	//spi_m_sync_get_io_descriptor(&RF_SPI, &io);
	//spi_m_sync_enable(&VFD_SPI);
	io_write(rf_spi, &addr, 1);
	io_read(rf_spi,&addr,1);
	return addr;
}

void RFM69_ReadBuff(uint8_t* buff, uint16_t len){
	//spi_m_sync_get_io_descriptor(&RF_SPI, &io);
	//spi_m_sync_enable(&VFD_SPI);
	io_read(rf_spi,buff,len);
}

void RFM69_WriteBuff(uint8_t* buff, uint16_t len){
	//spi_m_sync_get_io_descriptor(&RF_SPI, &io);
	//spi_m_sync_enable(&VFD_SPI);
	io_write(rf_spi,buff,len);
}



void RFM_69_sel(bool arg){
	gpio_set_pin_level(RF_CS , arg);
}

void SPI_write(uint8_t arg){
	uint8_t *addr_ptr = &arg;
	//struct io_descriptor *io;
	//spi_m_sync_get_io_descriptor(&RF_SPI, &io); ///<<< EXP
	//spi_m_sync_enable(&RF_SPI);
	io_write(rf_spi, &arg, 1);	
}


uint8_t SPI_read(){
	uint8_t data_byte = 0;
	//spi_m_sync_get_io_descriptor(&RF_SPI, &io); ///<<< EXP
	io_read(rf_spi,&data_byte,1);
	return data_byte;
}

bool get_io0(){
	return gpio_get_pin_level(RF_IRQ);
}
void toggle_led(){
	gpio_toggle_pin_level(LED_G);
}

void set_led(){
	gpio_set_pin_level(LED_G, false);
}
void reset_led(){
	gpio_set_pin_level(LED_G, true);
}

//Display section
void EXT_SPI_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	
	
	spi_m_sync_init(&EXT_SPI, SERCOM0);
	spi_m_sync_get_io_descriptor(&EXT_SPI, &lcd_spi);
	spi_m_sync_enable(&EXT_SPI);
	
	gpio_set_pin_level(PA04, false);
	gpio_set_pin_direction(PA04, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PA04, PINMUX_PA04D_SERCOM0_PAD0);
	gpio_set_pin_level(PA05, false);
	gpio_set_pin_direction(PA05, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PA05, PINMUX_PA05D_SERCOM0_PAD1);
	gpio_set_pin_direction(PA06, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PA06, GPIO_PULL_OFF);
	gpio_set_pin_function(PA06, PINMUX_PA06D_SERCOM0_PAD2);
	gpio_set_pin_level(VFD_RST, true);
	gpio_set_pin_level(FILAMENT_EN, true);
	
	//io_write(io, example_VFD_SPI, 12);

	//VFD additional IO setup
	gpio_set_pin_level(VFD_RST, true);
	gpio_set_pin_direction(VFD_RST, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(VFD_RST, GPIO_PIN_FUNCTION_OFF);
	
	gpio_set_pin_level(FILAMENT_EN, true);
	gpio_set_pin_direction(FILAMENT_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(FILAMENT_EN, GPIO_PIN_FUNCTION_OFF);	
	
	gpio_set_pin_level(VFD_CS, true);
	gpio_set_pin_direction(VFD_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(VFD_CS, GPIO_PIN_FUNCTION_OFF);
	
		
}
void vfd_write_data(uint8_t* buff, uint16_t len){
	//struct io_descriptor *io;
	//spi_m_sync_get_io_descriptor(&EXT_SPI, &io);
	//spi_m_sync_enable(&EXT_SPI);
	io_write(lcd_spi, buff, len);
	
}

void vfd_cs_set(bool state){
	gpio_set_pin_level(VFD_CS, !state);
}

uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){
	//uint8_t cycles;

	switch(msg)
	{
		case U8X8_MSG_DELAY_NANO:     // delay arg_int * 1 nano second
		break;
		case U8X8_MSG_DELAY_100NANO:    // delay arg_int * 100 nano seconds
		break;
		case U8X8_MSG_DELAY_10MICRO:    // delay arg_int * 10 micro seconds
		break;
		case U8X8_MSG_DELAY_MILLI:      // delay arg_int * 1 milli second
		break;
		default:
		return 0;
	}
	return 0;
}
uint8_t vfd_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	
	switch(msg)
	{
		case U8X8_MSG_BYTE_SEND:
		vfd_write_data(arg_ptr, arg_int);
		break;
		case U8X8_MSG_BYTE_INIT:
		/* disable chipselect */
		vfd_cs_set(false);
		break;
		case U8X8_MSG_BYTE_SET_DC:
		u8x8_gpio_SetDC(u8x8, arg_int);
		break;
		case U8X8_MSG_BYTE_START_TRANSFER:
		vfd_cs_set(true);
		break;
		case U8X8_MSG_BYTE_END_TRANSFER:
		vfd_cs_set(false);
		break;
		default:
		return 0;
	}
	return 1;
}

//RF_Section
void RF_SPI_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
	_gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	
	spi_m_sync_init(&RF_SPI, SERCOM1);
	spi_m_sync_get_io_descriptor(&RF_SPI, &rf_spi);
	spi_m_sync_enable(&RF_SPI);
	
	gpio_set_pin_level(PA00, false);
	gpio_set_pin_direction(PA00, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PA00, PINMUX_PA00D_SERCOM1_PAD0);
	gpio_set_pin_level(PA01, false);
	gpio_set_pin_direction(PA01, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PA01, PINMUX_PA01D_SERCOM1_PAD1);
	gpio_set_pin_direction(PA18, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA18, GPIO_PULL_OFF);
	gpio_set_pin_function(PA18, PINMUX_PA18C_SERCOM1_PAD2);
	//Gpio setup
	
	gpio_set_pin_level(RF_RST, false);
	gpio_set_pin_direction(RF_RST, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(RF_RST, GPIO_PIN_FUNCTION_OFF);
	RF_HW_Reset();
	//Radio additional IO setup
	gpio_set_pin_level(RF_CS, true);
	gpio_set_pin_direction(RF_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(RF_CS, GPIO_PIN_FUNCTION_OFF);
	
	//SPI Flash additional IO setup
	gpio_set_pin_level(ROM_CS, true);
	gpio_set_pin_direction(ROM_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(ROM_CS, GPIO_PIN_FUNCTION_OFF);
	
	//MicroSD Flash additional IO setup
	gpio_set_pin_level(LED_SD, false);
	gpio_set_pin_direction(LED_SD, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(LED_SD, GPIO_PIN_FUNCTION_OFF);
	
	gpio_set_pin_level(SD_CS, true);
	gpio_set_pin_direction(SD_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(SD_CS, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_direction(SD_DETECT, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(SD_DETECT, GPIO_PULL_OFF);
	gpio_set_pin_function(SD_DETECT, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_direction(RF_IRQ, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(RF_IRQ, GPIO_PULL_DOWN);
	gpio_set_pin_function(RF_IRQ, PINMUX_PB30A_EIC_EXTINT14);
	RF_IRQ_Enable();
	
}


void RF_HW_Reset(void){
	gpio_set_pin_level(RF_RST, true);
	delay_ms(1);
	gpio_set_pin_level(RF_RST, false);
	delay_ms(20);
}

//External I2C port


void EXT_I2C_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM4);
	_gclk_enable_channel(SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC);
	_gclk_enable_channel(SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC);
	
	i2c_m_sync_init(&EXT_I2C, SERCOM4);
	
	gpio_set_pin_pull_mode(PB12, GPIO_PULL_OFF);
	gpio_set_pin_function(PB12, PINMUX_PB12C_SERCOM4_PAD0);
	gpio_set_pin_pull_mode(PB13, GPIO_PULL_OFF);
	gpio_set_pin_function(PB13, PINMUX_PB13C_SERCOM4_PAD1);
	
	i2c_m_sync_get_io_descriptor(&EXT_I2C, &I2C_io);
	i2c_m_sync_enable(&EXT_I2C);
}


bool I2C_write_batch(uint8_t addres, uint8_t *data, uint8_t data_len){
	i2c_m_sync_set_slaveaddr(&EXT_I2C, addres, I2C_M_SEVEN);
	return (io_write(I2C_io, (uint8_t *)data, data_len) >= 0) ? true : false;  
}

bool I2C_read_batch(uint8_t addres ,uint8_t *data, uint8_t data_len){
	i2c_m_sync_set_slaveaddr(&EXT_I2C, addres, I2C_M_SEVEN);
	
	i2c_m_sync_cmd_read(&EXT_I2C, 0x00, data, data_len);
	
	//return (io_read(I2C_io, (uint8_t *)data, data_len) >= 0) ? true : false;
}