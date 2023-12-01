# What is this?
Air quality station to detect PM2.5 concentration, this setup is for demonstration purpose.


# Design Requirement
- Air circulation
-- fan to circulate airflow
- battery powered / USB powered 
- Portable
- WIFI transmission


# Selected components:
ZPH02 (Low cost)
ESP8226 Dev board
5V to 3V3 logic level shifter


# Encountered issue
- mismatched working voltage and output voltage
- flooded and hanged serial buffer 


# Technical requirement
Logic level shifter
- ZPH02 requires 5V while ESP provides 3V3 
- 5V power supply (You can use Vin from the board, but mine doesnt work)
- Serial overflow on my arduino uno
- Transmission: PWM or Serial (Serial for this repo)
- Data comes in every 9 bytes, first byte is 0xFF
- baudrate is 9600
- data is at [3] and [4], value in percentage should be [3].[4]%


# Result
Burn paper will generate about 15 - 18 percent, which can be refer to the graph.