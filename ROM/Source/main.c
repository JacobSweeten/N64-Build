#include "structs.h"
#include "vi.h"
#include "get_asset.h"

void init_vi(int, FrameBuffers_t*);
void swap_buffers(FrameBuffers_t*);
void draw_text(char*, short*);
void clear_buffer(short*, short);

void __start()
{
	FrameBuffers_t fbs;
	init_vi(2, &fbs);
	clear_buffer(fbs.fb1, 0x07C1);
	clear_buffer(fbs.fb2, 0x0001);

	//draw_text("Test lmaoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo", fbs.fb1);

	/*
	int a = 0;
	while(1)
		a++;
	*/
	
	while(1)
	{
		int a = 0;
		while(a < 1000000)
			a++;
		
		swap_buffers(&fbs);
	}
	
}