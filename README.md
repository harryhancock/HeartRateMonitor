# HeartRateMonitor
This is a hardware project which measures the users heartrate and outputs the waveform from a bandpass filter to an oscilloscope. I have included the C code in this repo.

# Construction
I built this my measuring analogue signals using the AVR powered Il Matto development board and using its analogue-to-digital converter to measure voltages.
I applied this to build a proximity detector using a reflectie optical sensor, along with the following components.
Finally I amplified very small variations in intensity from the optical sensor using a bandpass filter circuit with a gain of 50 at the centre freq of 2.3Hz composed of a passive high-pass filter and active low-pass filter.
This allowed me to convert it into a non-invasive system for measuring your heart rate known as Photoplethysmography
The passive high-pass filter was constructed using the 1µF capacitor and 68kΩ resistor in series
The active low-pass filter using the 6.8kΩ resistor, 680kΩ resistor, 100nF capacitor and Op-Amp.

# Learning outcomes
* Configured the ADC to sample from a chosen input
* Captured a single measurement from an input channel
* Configured the ADC to capture multiple samples at a specific smapling rate
* Applied processing to extract the signal of interest

# Components
* TCRT1010 Reflective Optical Sensor
* MCP602 Dual Operational Amplifier IC
* Capacitors: 1x 100nF, 1x 1µF capacitor
* Resistors: 1x 100Ω, 1x 10kΩ resistor, 1x 6.8kΩ resistor, 1x 68kΩ resistor, 1x 680kΩ resistor
* 1x 10kΩ potentiometer

# Updates
* Developed Arduino/C++ code to send a Beats-Per-Minute (BPM) value to a Google cloud server 
* Built using the platform.io plug in for vscode, using the ESP32 Heltec V2 board


## Acknowledgments

* StackOverflow
