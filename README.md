# pico-WSPR-tx
WSPR beacon for Raspberry Pi Pico, based on pico-hf-oscillator PLL DCO library as submodule.

![pico-wspr-rx](https://github.com/RPiks/pico-WSPR-tx/assets/47501785/a69103e7-d766-4554-b288-aa0f82a74da9)
The example of this WSPR beacon (QRPX power level) reception.

![Screenshot_2023-11-16_22-42-12](https://github.com/RPiks/pico-WSPR-tx/assets/47501785/dcc20f80-17c8-4c37-84e6-c162bffd8bbb)
The spectrum of this WSPR beacon received on SSB transceiver.

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
4. ./build.sh
5. Check whether output file ./build/pico-WSPR-tx.uf2 appears.

6. Load the .uf2 file (2) into the Pico.

7. Initialy the operating HF band is 40 meter (WSPR dial frequency is 7040 kHz).

8. Provide the feedback by clicking like on the github page of the project :).

Cheers,
Roman Piksaykin, amateur radio callsign R2BDY
https://www.qrz.com/db/R2BDY
piksaykin@gmail.com
