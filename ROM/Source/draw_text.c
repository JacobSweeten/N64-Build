#include "structs.h"
#include "get_asset.h"

int get_idx(char c)
{
	if(c >= '0' && c <= '9')
		return c - 48;
	else if(c >= 'A' && c <= 'Z')
		return c - 55;
	else
		return -1;
}

void draw_text(char* text, short* frameBuffer)
{
	asset_t spriteSheet;
	int res = get_asset_by_id(&spriteSheet, 1);

	// Intentionally crash if we can't find the asset
	if(res != 0)
	{
		int a = *(int*)(0xFFFFFFFF);
	}

	asset_image_type_data_t imgData = *(asset_image_type_data_t*)&spriteSheet.assetData;
	short* img = (short*)spriteSheet.data;

	// Get length
	char c = text[0];
	int len = 0;
	while(c != 0)
	{
		len++;
		c = text[len];
	}

	// Convert case
	for(int i = 0; i < len; i++)
	{
		if(text[i] >= 'a' && text[i] <= 'z')
			text[i] = text[i] & 0b11011111;
	}

	int x = 0;
	int y = 0;

	for(int i = 0; i < len; i++)
	{
		int index = get_idx(text[i]);

		if(x == 40)
		{
			x = 0;
			y++;
		}

		if(index != -1)
		{
			for(int j = 0; j < 8; j++)
			{
				for(int k = 0; k < 16; k++)
				{
					// TODO: Check this math
					int yDst = (y * 16 * 320) + (k * 320);
					int xDstRow = (x * 8) + j;
					int dstPos = yDst + xDstRow;
					int srcPos = (index * 8 + j) + (imgData.width * k);

					frameBuffer[dstPos] = img[srcPos];
				}
			}
			x++;
		}
		else if(text[i] == '\n')
		{
			y++;
			x = 0;
		}
		else
		{
			x++;
		}
	}
}