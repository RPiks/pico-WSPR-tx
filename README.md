# pico-WSPR-tx
WSPR beacon for Raspberry Pi Pico, based on pico-hf-oscillator PLL DCO library as submodule.
The GPS reference is used to compensate Pico's clock drift. GPS is also used to schedule WSPR transmissions.

![image](https://github.com/RPiks/pico-WSPR-tx/assets/47501785/3f835a9d-fa42-4eb8-ba93-ea72033d9e62)
The example of this WSPR beacon (QRPX power level) reception. Last max QRB is ~3400 km on 40m band.

![image](https://github.com/RPiks/pico-WSPR-tx/assets/47501785/a86280b9-71cb-4bb2-8b3c-0e33d2499aca)
High spectrum quality and less than 1Hz frequency drift.

# *NO* additional hardware
The WSPR beacon provides the output signal on the GPIO pin of Raspberry Pi Pico. No externall PLL, analog oscillators!
You should only provide a lowpass filter of appropriate frequency if you want to use this module extensively. 
The power transmitted using GPIO pin is sufficient only when using full-size dipole as antenna. If there is no such option available, you need to boost the signal using simple 1 transistor amplifier.

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

7. Initialy the operating HF band is 17 meter (WSPR dial frequency is 18106 kHz).

8. Provide the feedback by clicking like on the github page of the project :).

Cheers,
Roman Piksaykin, amateur radio callsign R2BDY
https://www.qrz.com/db/R2BDY
piksaykin@gmail.com
