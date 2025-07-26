- clone into circle/app  
- in `include/circle/sysconfig.h` uncomment `#define USE_PWM_AUDIO_ON_ZERO`
- GPIO 12/13 are the PWM output, use a RC filter and a dc-blocking cap.
- GPIO 14/15 are TX/RX for logging.
  
`faust -lang c -o test-tone.h test-tone.dsp`  
`make`
