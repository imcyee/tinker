Need a fan to move airflow in and out
battery powered
rechargable 
small in size
wifi powered



Logic level shifter requires 
for 5v to 3v

Encountered issue
- mismatched working voltage and output voltage
-- uses logic level shifter

- flooded serial buffer because of tx issue
-- we can time the module
-- using software serial and check for overflow


You can use Vin from your board, unfortunately for mine, it doesnt output 5v, probably caused by the diode beside to prevent two power source go through each others.

