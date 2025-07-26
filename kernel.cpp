//
// kernel.cpp
//

#include "kernel.h"
#include <assert.h>
#include <circle/util.h>
#include <circle/machineinfo.h>
#include <circle/debug.h>
#include <circle/usb/usbhcidevice.h>
#include <circle/usb/usbmidi.h>

static const char FromKernel[] = "kernel";

static CKernel *s_pKernel = nullptr;

CKernel::CKernel (void)
:	
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer),
	m_pUSB (new CUSBHCIDevice (&m_Interrupt, &m_Timer, TRUE)) // TRUE: enable plug-and-play
{
	m_ActLED.Blink (1);	// show we are alive
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		bOK = m_Logger.Initialize (&m_Serial);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	if (bOK)
	{
		assert (m_pUSB);
		bOK = m_pUSB->Initialize ();
	}
	
	if (bOK)
	{
		m_PWMSoundDevice = new CPDPWM(&m_Interrupt);
	}

	return bOK;
}

void CKernel::MIDIPacketHandler (unsigned nCable, u8 *pPacket, unsigned nLength)
{
	for(unsigned i=0; i< nLength; i++){
		s_pKernel->m_Logger.Write (FromKernel, LogNotice, "%d", pPacket[i]);
	}
}

void CKernel::USBDeviceRemovedHandler (CDevice *pDevice, void *pContext)
{
	s_pKernel->m_Logger.Write (FromKernel, LogNotice, "Lost midi device");
}

TShutdownMode CKernel::Run (void)
{
	m_Logger.Write (FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);
	boolean bOK = m_PWMSoundDevice->Start();
	
	while(1){
		assert (m_pUSB);
		if (m_pUSB->UpdatePlugAndPlay ())
		{
			m_pMIDIDevice = (CUSBMIDIDevice *) CDeviceNameService::Get ()->GetDevice ("umidi1", FALSE);
			if (m_pMIDIDevice != 0)
			{
				m_Logger.Write (FromKernel, LogNotice, "Found midi device");
				s_pKernel = this;
				m_pMIDIDevice->RegisterPacketHandler (MIDIPacketHandler);
				m_pMIDIDevice->RegisterRemovedHandler (USBDeviceRemovedHandler);
			}
		}
		m_Timer.MsDelay(100);
	}

	return ShutdownHalt;
}