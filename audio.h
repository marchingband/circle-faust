#ifndef AUDIO_H
#define AUDIO_H

#include <circle/sound/pwmsoundbasedevice.h>

class CPDPWM : public CPWMSoundBaseDevice
{
public:
    CPDPWM(CInterruptSystem *pInterrupt);
    ~CPDPWM(void);
    unsigned GetChunk (u32 *pBuffer, unsigned nChunkSize);
};

#endif