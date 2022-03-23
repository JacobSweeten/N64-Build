#include "structs.h"
#include "vi.h"
#include "get_asset.h"
#include "draw_text.h"

#define DEMO_TEXT

void __start()
{
	FrameBuffers_t fbs;
	init_vi(2, &fbs);
	clear_buffer(fbs.fb1, 0x07C1);
	clear_buffer(fbs.fb2, 0x0001);

#ifdef DEMO_TEXT
	//draw_text("0", fbs.fb1);
	draw_text("\nFINISHED LOADING VI", fbs.fb1);

	
	volatile int a = 0;
	while(1)
		a++;
	
#endif

#ifdef DEMO_FLASH
	while(1)
	{
		volatile int a = 0;
		while(a < 1000000)
			a++;
		
		swap_buffers(&fbs);
	}
#endif

}