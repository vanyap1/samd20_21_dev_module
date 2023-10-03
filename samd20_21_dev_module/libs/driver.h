
/*
 * driver.h
 *
 * Created: 23.08.2023 11:19:52
 *  Author: Vanya
 */ 

#include <stdint.h>
#include <stdbool.h>
#include <u8g2.h>

#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
	#endif
	#include <hal_atomic.h>
	#include <hal_delay.h>
	#include <hal_gpio.h>
	#include <hal_init.h>
	#include <hal_io.h>
	#include <hal_sleep.h>

	#include <hal_ext_irq.h>

	#include <hal_spi_m_sync.h>
	#include <hal_spi_m_sync.h>

	#include <hal_usart_sync.h>

	#include <hal_delay.h>

	extern struct spi_m_sync_descriptor EXT_SPI;
	extern struct spi_m_sync_descriptor RF_SPI;

	
	void RF_IRQ_Enable(void);
	void RF_int_Handler(void);
	uint8_t rf_isReady();
	uint8_t RFM69_read_reg(uint8_t addr);
	void RFM69_ReadBuff(uint8_t* buff, uint16_t len);
	void RFM_69_sel(bool arg);
	void SPI_write(uint8_t);
	uint8_t SPI_read();
	bool get_io0();
	void toggle_led();
	void set_led();
	void reset_led();
	
	
	
	//Display section
	void EXT_SPI_init(void);
	void vfd_write_data(uint8_t* buff, uint16_t len);
	void vfd_cs_set(bool state);
	uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
	uint8_t vfd_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
	
	//RF_Section
	void RF_SPI_init(void);

	
	
	
	
	#ifdef __cplusplus
}
#endif
#endif // DRIVER_EXAMPLES_H_INCLUDED
