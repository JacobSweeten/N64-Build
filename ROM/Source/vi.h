#ifndef VI_H
#define VI_H

typedef struct FrameBuffers_s {
	int count;
	int currentBuffer;
	short* fb1;
	short* fb2;
	short* fb3;
} FrameBuffers_t;

void init_vi(int, FrameBuffers_t*);
void swap_buffers(FrameBuffers_t*);
void clear_buffer(short*, short);

#endif