#include "util.h"
#include "radio.h"
#include "delay.h"
#include "usart.h"
#include "si446x_ctrl.h"
#include "si446x_defs.h"
#include "si446x_hal.h"
#include "mini_morse.h"
#include<stdio.h>

// const uint8_t radio_msg[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ,.!:;()\"@&?-+/=*\\";
const uint8_t radio_msg[] = "Namaste!";

int main()
{
  led_init();
  delay_init();
  usart_init(115200);
  usart_txln("Namaste!");

  while (!radio_init())
  {
    usart_txln("Radio error");
    delay_ms(1000);
  }
  usart_txln("Radio successfull..");

  gfsk_rx();
  RX_Interrupt_Handler();
  
  
  
  
  
  // while (radio_init())
  // {

    // receive_data();
    // delay_ms(10);

  // }
  

  // usart_txln("Radio success!");

  // radio_init_morse();
  // // usart_txln("Morse success!");
  // mini_morse_tx(radio_msg, sizeof(radio_msg));

  while(0x221b)
  {
    // radio_tx(radio_msg, sizeof(radio_msg));
    // radio_set_state(SLEEP);
    // delay_ms(2000);
  }
  // while(1){
  //   usart_txln("Radio error");


  // }

  return 0;
}

inline void mini_morse_start_tx(const enum MorseLetter m)
{
  radio_set_state(START_TX);
  si446x_morse_high();
}

inline void mini_morse_stop_tx(const enum MorseLetter m)
{
  si446x_morse_low();
  radio_set_state(READY);
}

inline void mini_morse_delay(const uint16_t delay)
{
  delay_ms(delay);
}





void gfsk_rx(void)
{
  uint8_t buffer[7];

  set_properties(Si446x_PROP_GLOBAL_XO_TUNE, (const uint8_t[]){0x4B}, 1);

  set_properties(Si446x_PROP_INT_CTL_ENABLE, (const uint8_t[]){0x03}, 1);
  set_properties(Si446x_PROP_INT_CTL_PH_ENABLE, (const uint8_t[]){0x10}, 1);
  set_properties(Si446x_PROP_INT_CTL_MODEM_ENABLE, (const uint8_t[]){0x02}, 1);


  buffer[0] = 0x00;
  buffer[1] = 0x00;
  buffer[2] = 0x00;
  si446x_ctrl_send_cmd_stream(Si446x_CMD_GET_INT_STATUS, buffer, 3);

  set_properties(Si446x_PROP_MODEM_MOD_TYPE, (const uint8_t[]){0x03}, 1);
  set_properties(Si446x_PROP_MODEM_MAP_CONTROL, (const uint8_t[]){0x00}, 1);
  set_properties(Si446x_PROP_MODEM_CLKGEN_BAND, (const uint8_t[]){0x08}, 1);

  set_properties(Si446x_PROP_FREQ_CONTROL_INTE, (const uint8_t[]){0x3C}, 1);
  set_properties(Si446x_PROP_FREQ_CONTROL_FRAC_2, (const uint8_t[]){0x08}, 1);
  set_properties(Si446x_PROP_FREQ_CONTROL_FRAC_1, (const uint8_t[]){0x00}, 1);
  set_properties(Si446x_PROP_FREQ_CONTROL_FRAC_0, (const uint8_t[]){0x00}, 1);
  set_properties(Si446x_PROP_FREQ_CONTROL_W_SIZE, (const uint8_t[]){0x20}, 1);

  //RX Parameters
  set_properties(Si446x_PROP_FREQ_CONTROL_VCOCNT_RX_ADJ,(const uint8_t[]){0xFF},1);
  set_properties(Si446x_PROP_MODEM_MDM_CTRL,(const uint8_t[]){0x00},1); 
  set_properties(Si446x_PROP_MODEM_IF_CONTROL,(const uint8_t[]){0x08},1);  
  set_properties(Si446x_PROP_MODEM_IF_FREQ_2,(const uint8_t[]){0x03},1);  
  set_properties(Si446x_PROP_MODEM_IF_FREQ_1,(const uint8_t[]){0xC0},1);  
  set_properties(Si446x_PROP_MODEM_IF_FREQ_0,(const uint8_t[]){0x00},1);  
  set_properties(Si446x_PROP_MODEM_DECIMATION_CFG1,(const uint8_t[]){0xB0},1);
  set_properties(Si446x_PROP_MODEM_DECIMATION_CFG0,(const uint8_t[]){0x20},1); 
  set_properties(Si446x_PROP_MODEM_BCR_OSR_1,(const uint8_t[]){0x00},1);
  set_properties(Si446x_PROP_MODEM_BCR_OSR_0,(const uint8_t[]){0x5E},1); 
  set_properties(Si446x_PROP_MODEM_BCR_NCO_OFFSET_2,(const uint8_t[]){0x05},1);  
  set_properties(Si446x_PROP_MODEM_BCR_NCO_OFFSET_1,(const uint8_t[]){0x76},1);  
  set_properties(Si446x_PROP_MODEM_BCR_NCO_OFFSET_0,(const uint8_t[]){0x1A},1); 
  set_properties(Si446x_PROP_MODEM_BCR_GAIN_1,(const uint8_t[]){0x07},1);  
  set_properties(Si446x_PROP_MODEM_BCR_GAIN_0,(const uint8_t[]){0xFF},1);  
  set_properties(Si446x_PROP_MODEM_BCR_GEAR,(const uint8_t[]){0x02},1);  
  set_properties(Si446x_PROP_MODEM_BCR_MISC1,(const uint8_t[]){0x00},1);  
  set_properties(Si446x_PROP_MODEM_BCR_MISC0,(const uint8_t[]){0x00},1);  
  set_properties(Si446x_PROP_MODEM_AFC_GEAR,(const uint8_t[]){0x00},1);  
  set_properties(Si446x_PROP_MODEM_AFC_WAIT,(const uint8_t[]){0x12},1);  
  set_properties(Si446x_PROP_MODEM_AFC_GAIN_1,(const uint8_t[]){0x80},1);  
  set_properties(Si446x_PROP_MODEM_AFC_GAIN_0,(const uint8_t[]){0x16},1);  
  set_properties(Si446x_PROP_MODEM_AFC_LIMITER_1,(const uint8_t[]){0x01},1);  
  set_properties(Si446x_PROP_MODEM_AFC_LIMITER_0,(const uint8_t[]){0x76},1);  
  set_properties(Si446x_PROP_MODEM_AFC_MISC,(const uint8_t[]){0xE0},1);  
  set_properties(Si446x_PROP_MODEM_AGC_CONTROL,(const uint8_t[]){0xE2},1);  
  set_properties(Si446x_PROP_MODEM_AGC_WINDOW_SIZE,(const uint8_t[]){0x11},1);  
  set_properties(Si446x_PROP_MODEM_AGC_RFPD_DECAY,(const uint8_t[]){0x15},1);  
  set_properties(Si446x_PROP_MODEM_AGC_IFPD_DECAY,(const uint8_t[]){0x15},1);  
  set_properties(Si446x_PROP_MODEM_OOK_CNT1,(const uint8_t[]){0xA4},1);  
  set_properties(Si446x_PROP_MODEM_OOK_MISC,(const uint8_t[]){0x03},1);  
  set_properties(Si446x_PROP_MODEM_RAW_SEARCH,(const uint8_t[]){0xD6},1);  
  set_properties(Si446x_PROP_MODEM_RAW_CONTROL,(const uint8_t[]){0x03},1);  
  set_properties(Si446x_PROP_MODEM_RAW_EYE_1,(const uint8_t[]){0x00},1);  
  set_properties(Si446x_PROP_MODEM_RAW_EYE_0,(const uint8_t[]){0xDE},1);  
  set_properties(Si446x_PROP_MODEM_RSSI_COMP,(const uint8_t[]){0x40},1);  
  set_properties(Si446x_PROP_MODEM_RSSI_CONTROL,(const uint8_t[]){0x00},1);  
  set_properties(Si446x_PROP_MODEM_RSSI_THRESH,(const uint8_t[]){0xFF},1);  
  set_properties(Si446x_PROP_MODEM_RSSI_JUMP_THRESH,(const uint8_t[]){0x0C},1); 
  // yaha hernu feri

  //WB Filter 
  const uint8_t WB_filter0 = {0x00, 0x5B, 0x47, 0x0F, 0xC0, 0x6D, 0x25, 0xF4, 0xDB, 0xD6, 0xDF, 0xEC, 0xF7};
  const uint8_t WB_fiilter1 = {0x0C, 0xFE, 0x01, 0x15, 0xF0, 0xFF, 0x03};
  set_properties(Si446x_FILTER_0,WB_filter0,sizeof(WB_filter0));
  set_properties(Si446x_FILTER_1,WB_fiilter1,sizeof(WB_fiilter1));

  // //NB Filter 
  // const uint8_t NB_filter0 = {0x12, 0x5B, 0x47, 0x0F, 0xC0, 0x6D, 0x25, 0xF4, 0xDB, 0xD6, 0xDF, 0xEC, 0xF7};
  // const uint8_t NB_fiilter1 = {0x1E, 0xFE, 0x01, 0x15, 0xF0, 0xFF, 0x03};
  // set_properties(Si446x_FILTER_0,NB_filter0,sizeof(NB_filter0));
  // set_properties(Si446x_FILTER_1,NB_fiilter1,sizeof(NB_fiilter1));



  // set_properties(Si446x_PROP_PA_MODE, (const uint8_t[]){0x08}, 1);
  // set_properties(Si446x_PROP_PA_PWR_LVL, (const uint8_t[]){0x7F}, 1);
  // set_properties(Si446x_PROP_PA_BIAS_CLKDUTY, (const uint8_t[]){0x00}, 1);
  // set_properties(Si446x_PROP_PA_TC, (const uint8_t[]){0x3D}, 1);

  // set_properties(Si446x_PROP_MODEM_DATA_RATE_2, (const uint8_t[]){0x00}, 1);
  // set_properties(Si446x_PROP_MODEM_DATA_RATE_1, (const uint8_t[]){0x4E}, 1);
  // set_properties(Si446x_PROP_MODEM_DATA_RATE_0, (const uint8_t[]){0x20}, 1);
  // set_properties(Si446x_PROP_MODEM_TX_NCO_MODE_3, (const uint8_t[]){0x04}, 1);
  // set_properties(Si446x_PROP_MODEM_TX_NCO_MODE_2, (const uint8_t[]){0x2D}, 1);
  // set_properties(Si446x_PROP_MODEM_TX_NCO_MODE_1, (const uint8_t[]){0xC6}, 1);
  // set_properties(Si446x_PROP_MODEM_TX_NCO_MODE_0, (const uint8_t[]){0xC0}, 1);
  // set_properties(Si446x_PROP_MODEM_FREQ_DEV_2, (const uint8_t[]){0x00}, 1);
  // set_properties(Si446x_PROP_MODEM_FREQ_DEV_1, (const uint8_t[]){0x00}, 1);
  // set_properties(Si446x_PROP_MODEM_FREQ_DEV_0, (const uint8_t[]){0x39}, 1);
  // set_properties(Si446x_PROP_MODEM_TX_RAMP_DELAY, (const uint8_t[]){0x01}, 1);
  set_properties(Si446x_PROP_PREAMBLE_CONFIG,(const uint8_t[]){0x02},1);
  set_properties(Si446x_PROP_PREAMBLE_CONFIG_STD_1,(const  uint8_t[]){0x14},1);
  set_properties(Si446x_PROP_PREAMBLE_CONFIG_STD_2,(const  uint8_t[]){0x08},1);


  // set_properties(Si446x_PROP_PREAMBLE_TX_LENGTH, (const uint8_t[]){0x05}, 1);
  // set_properties(Si446x_PROP_PREAMBLE_CONFIG, (const uint8_t[]){0x12}, 1);

  set_properties(Si446x_PROP_SYNC_CONFIG, (const uint8_t[]){0x01}, 1);
  set_properties(Si446x_PROP_SYNC_BITS_31_24, (const uint8_t[]){0xB4}, 1);
  set_properties(Si446x_PROP_SYNC_BITS_23_16, (const uint8_t[]){0x2B}, 1);
  
  set_properties(Si446x_PROP_PKT_CRC_CONFIG,(const uint8_t[]){0x00}, 1);
  set_properties(Si446x_PROP_PKT_CONFIG1,(const uint8_t[]){0x00},1);

  // set_properties(Si446x_PROP_PKT_LEN, (const uint8_t[]){0x00}, 1);
  set_properties(Si446x_PROP_PKT_FIELD_1_LENGTH_12_8, (const uint8_t[]){0x00}, 1);
  set_properties(Si446x_PROP_PKT_FIELD_1_LENGTH_7_0, (const uint8_t[]){0x0A}, 1);
  // set_properties(Si446x_PROP_PKT_FIELD_1_CONFIG, (const uint8_t[]){0x00}, 1);
  // set_properties(Si446x_PROP_PKT_FIELD_1_CRC_CONFIG, (const uint8_t[]){0xA2}, 1);

  // const uint8_t GPIO_PIN[] = {0x14, 0x11, 0x18, 0x1A, 0x00, 0x00, 0x00};
  // si446x_ctrl_send_cmd_stream(Si446x_CMD_GPIO_PIN_CFG, GPIO_PIN, sizeof(GPIO_PIN));

  buffer[0] = 0x00;
  buffer[1] = 0x00;
  buffer[2] = 0x00;
  buffer[3] = 0x00;
  buffer[4] = 0x00;
  buffer[5] = 0x30;
  buffer[6] = 0x00;
  si446x_ctrl_send_cmd_stream(Si446x_CMD_START_RX, buffer, 4);
}


void receive_data(void)
{
    uint8_t buffer[7];  
    uint8_t rx_fifo[64]; 

    // Clear interrupts
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    si446x_ctrl_send_cmd_stream(Si446x_CMD_GET_INT_STATUS, buffer, 3);

    // Set RX mode
    buffer[0] = 0x00; 
    buffer[1] = 0x30; 
    buffer[2] = 0x00; 
    buffer[3] = 0x00; 
    si446x_ctrl_send_cmd_stream(Si446x_CMD_START_RX, buffer, 4);

    while (1) // Continuous reception loop
    {
        // Check interrupt status
        si446x_ctrl_send_cmd_stream(Si446x_CMD_GET_INT_STATUS, buffer, 3);

        // Check if a packet is received
        if (buffer[2] & 0x10) // Packet RX interrupt
        {
            // Read RX FIFO
            si446x_ctrl_send_cmd_stream(Si446x_CMD_READ_RX_FIFO, rx_fifo, sizeof(rx_fifo));

            // Process received data
            usart_tx("Received Data: ");
            for (int i = 0; i < sizeof(rx_fifo); i++)
            {
                usart_tx(rx_fifo[i]);
            }
            usart_txln("");

            // Clear RX FIFO
            si446x_ctrl_send_cmd_stream(Si446x_CMD_FIFO_INFO, buffer, 2);
            if (buffer[1] & 0x01) // RX FIFO Overflow
            {
                si446x_ctrl_send_cmd_stream(Si446x_CMD_FIFO_INFO, (const uint8_t[]){0x02}, 1);
            }

            // Restart RX
            si446x_ctrl_send_cmd_stream(Si446x_CMD_START_RX, buffer, 4);
        }

        delay_ms(100);
    }
}


//optional choice
void RX_Interrupt_Handler(void)
{
    uint8_t buffer[7];
    uint8_t rx_fifo[64];
    
    si446x_ctrl_send_cmd_stream(Si446x_CMD_GET_INT_STATUS, buffer, 3);

    if (buffer[2] & 0x10) // Packet RX interrupt
    {
        // Read RX FIFO
        si446x_ctrl_send_cmd_stream(Si446x_CMD_READ_RX_FIFO, rx_fifo, sizeof(rx_fifo));

        usart_tx("Received Data: ");
        for (int i = 0; i < sizeof(rx_fifo); i++)
        {
            usart_tx(rx_fifo[i]);
            if(rx_fifo[1]==0x01){
              usart_tx("0x01");

            }
        }
        usart_txln("");
    }

    si446x_ctrl_send_cmd_stream(Si446x_CMD_START_RX, buffer, 4);
}

