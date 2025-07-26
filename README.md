- clone into circle/app  
- in `include/circle/sysconfig.h` uncomment `#define USE_PWM_AUDIO_ON_ZERO`
  
`faust -lang c -o test-tone.h test-tone.dsp`  
`make`
