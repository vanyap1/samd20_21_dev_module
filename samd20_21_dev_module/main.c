#include <atmel_start.h>
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"

#include "driver_examples.h"
#include "driver.h"
#include "u8g2.h"

#include "RFM69.h"
#include "RFM69registers.h"

#define NETWORKID 33
#define NODEID    3
#define TONODEID  4
#define RX_MODE 1

u8g2_t u8g2;

uint16_t seco, mine, hour;
uint32_t pack_counter = 0;
uint32_t last_pkg_time;
uint8_t rf_tx_str[32] = "\0";

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	EXT_SPI_init();
	RF_SPI_init();
	EXT_I2C_init();
	
	rfm69_init(868, NODEID, NETWORKID);
	setHighPower(true);
	
	
	
	//EXTERNAL_IRQ_0_example();
	
	uint8_t debug_str[] = "                    \n\r";
	uint8_t rf_str[] =    "                    \n\r";
	
	
	
	//LED_Panel
		uint8_t led_val;
		uint8_t panel_addr = 0x64;
		#define  r_part 2
		#define  g_part 1
		#define  b_part 3
		
		uint8_t reset_cmd[] = {0xa5, 0x5a};
		uint8_t init_cmd[] = {0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xAA, 0xAA, 0xAA, 0xAA};
		uint8_t led_data[] = {0xa2, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f};
		
		uint8_t led1[]= {0xa2, 0, 0, 0};
		uint8_t led2[]= {0xa5, 0, 0, 0};
		
		uint8_t led3[]= {0xa8, 0, 0, 0};
		uint8_t led4[]= {0xab, 0, 25, 0};
		uint8_t led5[]= {0xae, 0, 25, 0};
		uint8_t ext_led[] = {0xb1, 64};
	//End LED Panel
	
	
	
	u8g2_Setup_gp1287ai_256x50_f(&u8g2, U8G2_R0, vfd_spi, u8x8_avr_gpio_and_delay);
	u8g2_InitDisplay(&u8g2);
	
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetContrast(&u8g2, 5);
	//WDT_CLEAR_CLEAR(0x5a);
	
	u8g2_ClearBuffer(&u8g2);
	//WDT->CLEAR.reg=0x5a;
	//u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
	//u8g2_SetFont(&u8g2, u8g2_font_courR08_tr);
	//u8g2_SetFont(&u8g2, u8g2_font_cu12_t_symbols);
	u8g2_SetFont(&u8g2, u8g2_font_Terminal_tr);
	//u8g2_SetFont(&u8g2, u8g2_font_battery19_tn); //battery icons
	
	/* Replace with your application code */
	 
		
	 GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_WDT |
	 GCLK_CLKCTRL_CLKEN |
	 GCLK_CLKCTRL_GEN_GCLK3;	
	//WDT->INTENSET.bit.EW  = 0;      // Enable early warning interrupt
	//WDT->EWCTRL.bit.EWOFFSET = 0x0B;  // Early Warning Interrupt Time Offset 0xA
	//WDT->CONFIG.bit.PER    = 0xB;  // Set period for chip reset 0xB 16384 clock cycles
	WDT->CLEAR.bit.CLEAR = 0xA5;
	//WDT->CTRL.bit.WEN      = 0;      // Disable window mode
	//while(WDT->STATUS.bit.SYNCBUSY); // Sync CTRL write
	//WDT->CTRL.bit.ENABLE = 1; // Start watchdog now!
	//while(WDT->STATUS.bit.SYNCBUSY);

	//I2C_write_batch(panel_addr, reset_cmd, sizeof(reset_cmd));
	//delay_ms(10);
	//I2C_write_batch(panel_addr, init_cmd, sizeof(init_cmd));
	//delay_ms(10);
	
	
	#define  input_reg		0x00
	#define  output_reg		0x02
	#define  polarity_reg	0x04
	#define  config_reg		0x06
	
	uint8_t port_expander_off[3] = {output_reg,0x00, 0x00};
	uint8_t port_expander_on[3] = {output_reg,0x01, 0x00};
	uint8_t port_config[3] = {config_reg, 0x02, 0x00};
	uint8_t port_polarity[3] = {polarity_reg, 0x00, 0x00};
	
	uint8_t port_state[2];
	uint8_t addr_expander = 0x20; 
	
	
	I2C_write_batch(addr_expander, port_config, sizeof(port_config));
	I2C_write_batch(addr_expander, port_polarity, sizeof(port_polarity));
	
	
	while (1) {
	
		WDT->CLEAR.bit.CLEAR = 0xA5;
		while(WDT->STATUS.bit.SYNCBUSY);
		
		
		
		
		
		
		//gpio_toggle_pin_level(LED_G);
		//gpio_toggle_pin_level(LED_SD);
		//gpio_toggle_pin_level(LED_R);
		
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawRFrame(&u8g2, 0, 0, 256 ,48, 5);
		u8g2_DrawStr(&u8g2, 60, 11, (char *)"ATMEL SAMD20 (48MHz)");
		u8g2_DrawLine(&u8g2, 3,12, 252,12);
		
		
		sprintf(debug_str, "INT:%04d; RSSI: %04d; tim:%03d:%02d:%02d; %01d " , rf_isReady(), lastRSSI(),  hour,mine,seco, get_io0());
		//sprintf((char *)rf_str , "%02d/%02d/%02d/%02d/%02d    ",  readReg(REG_IRQFLAGS1), readReg(REG_IRQFLAGS2),  readReg(REG_TEMP1),  last_pkg_time,  rx_header[4]);
		
		
		
		
		
		u8g2_DrawStr(&u8g2, 3, 23, (char *)debug_str);
		u8g2_DrawStr(&u8g2, 3, 34, (char *)rf_str);
		u8g2_DrawStr(&u8g2, 3, 44, (char *)DATA);
		u8g2_SendBuffer(&u8g2);
		
		//seco == 999 ? seco=0 : seco++;
		seco++;
		if(seco==60){
			u8g2_InitDisplay(&u8g2);
			u8g2_SetPowerSave(&u8g2, 0);
			u8g2_SetContrast(&u8g2, 5);
			
			mine++;
			seco=0;
		}
		if(mine==60){
			hour++;
			mine=0;
		}
		last_pkg_time++;
		
		
		//if(last_pkg_time > 10){
			//delay_ms(3000);
			//last_pkg_time=0;
			////RF_HW_Reset();
			//rfm69_init(868, NODEID, NETWORKID);
			//setHighPower(true);
		//}
		
		
		sprintf(rf_tx_str, "%03d:%02d:%02d test_ ; %010d\0",  hour,mine,seco, pack_counter);
		gpio_set_pin_level(LED_R, true);
		sendFrame(0, &rf_tx_str, sizeof(rf_tx_str));
		gpio_set_pin_level(LED_R, false);
		

		pack_counter++;
		//delay_ms(100);
		
		if (rf_isReady()) {
			
			sprintf((char *)rf_str , "%02d/%02d/%02d/%02d/%02d    ",  rx_header[0], rx_header[1], rx_header[2],  rx_header[3],  rx_header[4]);
			
			//u8g2_ClearBuffer(&u8g2);
			//u8g2_DrawRFrame(&u8g2, 200, 4, 40 ,40, 5);
			//u8g2_DrawStr(&u8g2, 3, 34, (char *)debug_str);
			//u8g2_DrawStr(&u8g2, 3, 44, (char *)DATA);
			
			//u8g2_SendBuffer(&u8g2);
			//rx_ready = 0;
			last_pkg_time=0;
		}
		
	}
}
