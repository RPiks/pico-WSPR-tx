# pico-WSPR-tx
WSPR beacon for Raspberry Pi Pico, based on pico-hf-oscillator PLL DCO library as submodule.

![Screenshot_2023-11-16_22-42-12](https://github.com/RPiks/pico-WSPR-tx/assets/47501785/9e2d0a9a-fe95-4e67-8b72-acd1011ed353)

# *NO* additional hardware
The WSPR beacon provides the output signal on the GPIO pin of Raspberry Pi Pico. No externall PLL, analog oscillators!
You should only provide a lowpass filter of appropriate frequency if you want to use this module extensively.

# For what?
This is an experimental project of amateur radio hobby and it is devised by me in order to experiment with QRP narrowband digital modes.
I am licensed radio amateur who is keen on experiments in the area of the digital modes on HF. 
My QRZ page is https://www.qrz.com/db/R2BDY

# Quick-start
1. Install Raspberry Pi Pico SDK. Configure environment variables. Test whether it is built successfully.

2. git clone https://github.com/RPiks/pico-WSPR-tx
3. cd pico-WSPR-tx
4. git submodule init
5. ./build.sh
6. Check whether output file ./build/pico-WSPR-tx.uf2 appears.

7. Load the .uf2 file (2) into the Pico.

8. Initialy the operating HF band is 40 meter (WSPR dial frequency is 7040 kHz).

9. Provide the feedback by clicking like on the github page of the project.

Cheers,
Roman Piksaykin, amateur radio callsign R2BDY
https://www.qrz.com/db/R2BDY
piksaykin@gmail.com
