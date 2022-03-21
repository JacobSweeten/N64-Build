#include "vi.h"

void swap_buffers(FrameBuffers_t* fbs)
{
	if(fbs->count == 2)
	{
		if(fbs->currentBuffer == 1)
		{
			((int*)0xA4400000)[1] = (unsigned int)fbs->fb2;
			fbs->currentBuffer = 2;
		}
		else
		{
			((int*)0xA4400000)[1] = (unsigned int)fbs->fb1;
			fbs->currentBuffer = 1;
		}
	}
	
}

void clear_buffer(short* buffer, short color)
{
	for(int i = 0; i < 320; i++)
	{
		for(int j = 0; j < 240; j++)
		{
			buffer[i + (j * 320)] = color;
		}
	}
}

void init_vi(int frameBufferCount, FrameBuffers_t* fbs)
{
	fbs->count = frameBufferCount;
	fbs->fb1 = (short*)0x80100000;
	fbs->fb2 = (short*)0x80125800;
	fbs->currentBuffer = 1;

	int* viBase = (int*)0xA4400000;
	viBase[0] =  0x0000320E;
	viBase[1] =  (int)fbs->fb1;
	viBase[2] =  0x00000140;
	viBase[3] =  0x00000200;
	viBase[4] =  0x00000000;
	viBase[5] =  0x03E52293;
	viBase[6] =  0x0000020D;
	viBase[7] =  0x00000C15;
	viBase[8] =  0x0C150C15;
	viBase[9] =  0x006C02EC;
	viBase[10] = 0x002501FF;
	viBase[11] = 0x000E0204;
	viBase[12] = 0x00000200;
	viBase[13] = 0x000000400;
}