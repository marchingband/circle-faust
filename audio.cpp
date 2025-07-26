#include <circle/timer.h>
#include <circle/synchronize.h>
#include <circle/memory.h>
#include <circle/logger.h>
#include <assert.h>
#include <circle/sound/pwmsoundbasedevice.h>
#include <circle/logger.h>

#include "audio.h"
#include "dsp.h"
#include "test-tone.h"

static const char FromPDDevice[] = "pd_device";

mydsp *dsp;

void extract(const char *name, float *p, float init, float min, float max, float step){};
void declare(struct UIGlue *uiInterface, float *p, const char* key, const char* val){};
void openVerticalBox(struct UIGlue *uiInterface, char* key){};
void openHorizontalBox(struct UIGlue *uiInterface, char* key){};
void closeBox(struct UIGlue *uiInterface){};
void addVerticalBargraph(struct UIGlue *uiInterface, const char *name, float *p, float f1, float f2){};
void addButton(struct UIGlue *uiInterface, const char *name, float *p){extract(name, p, 0, 0, 0, 0);};
void addVerticalSlider(struct UIGlue *uiInterface, const char *name, float *p, float init, float min, float max, float step){extract(name, p, init, min, max, step);};
void addHorizontalSlider(struct UIGlue *uiInterface, const char *name, float *p, float init, float min, float max, float step){extract(name, p, init, min, max, step);};
void addNumEntry(struct UIGlue *uiInterface, const char *name, float *p, float init, float min, float max, float step){extract(name, p, init, min, max, step);};

UIGlue ui_glue = {
    NULL,
    openVerticalBox,
    openHorizontalBox,
    closeBox,
    declare,
    addVerticalSlider,
    addHorizontalSlider,
    addNumEntry,
    addVerticalBargraph,
    addButton
};

float **outBuffers = NULL;

CPDPWM::CPDPWM(CInterruptSystem *pInterrupt)
:   CPWMSoundBaseDevice (pInterrupt)
{
    dsp = newmydsp();
    initmydsp(dsp, 44100);
    buildUserInterfacemydsp(dsp, &ui_glue);
}

CPDPWM::~CPDPWM(void)
{
}


unsigned CPDPWM::GetChunk (u32 *pBuffer, unsigned nChunkSize)
{
    int min = GetRangeMin();
    int max = GetRangeMax();
    int dc = max / 2;
    float scale = 0.1;
    
    int numFrames = nChunkSize / 2;
    
    if(outBuffers == NULL){
        outBuffers = (float **) malloc(2 * sizeof(float *));
        for (int i = 0; i < 2; ++i) {
            outBuffers[i] = (float *) malloc(numFrames * sizeof(float));
        }
        CLogger::Get ()->Write (FromPDDevice, LogDebug, "Allocated buffers");
    }
    
    computemydsp(dsp, numFrames, NULL, outBuffers);
    for(int i=0; i<nChunkSize/2; i++){
        s16 left = (outBuffers[0][i] * scale * dc) + dc;
        s16 right = (outBuffers[1][i] * scale * dc) + dc;
        pBuffer[i * 2] = left;
        pBuffer[i * 2 + 1] = right;
    }
    // CLogger::Get ()->Write (FromPDDevice, LogDebug, "Wrote %d bytes, min %d, max %d", nChunkSize, min ,max);

    return nChunkSize;
}
