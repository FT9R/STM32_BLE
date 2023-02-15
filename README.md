# Decsription
This is an example of usage [`E104-BT52`](https://www.ebyte.com/en/product-view-news.html?id=1061)
serial to BLE module, which can provide wireless data transmission between microcontroller units,
personal computers, mobile phones and etc.
## Quick start
It is assumed that you have at least two the same BLE modules.
First module (Slave) should be configured as follows:
* Connect one module to COM port via USB to TTL adapter (Vcc = 3.3V)
* Configure the serial port according to BLE module default parameters
  - BaudRate - 115200
  - DataBits - 8
  - StopBits - 1
  - Parity - none
* Send "AT" throught terminal. You should get "+OK" as a response from module.
* Send "AT+ROLE=0" in order to choose slave role for this module.
* Send "AT+LOGMSG=1" if you want to see the connection status
* Send "AT+MAC?" to get 6 byte slave address. 
Paste it to **/Include/main.h 
instead of predefined values in 'const uint8_t SlaveMAC[6] = {0x32, 0x00, 0x00, 0xB1, 0x9A, 0x01};'

