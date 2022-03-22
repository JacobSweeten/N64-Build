#include "get_asset.h"

int get_asset_by_id(asset_t* asset, int id)
{
	int* ptr1 = (int*)0xB0001004;
	int* ptr2 = (int*)ptr1[0];
	char* data = (char*)ptr2;
	int idx = 0;

	do
	{
		asset->len = *(int*)&data[idx];
		asset->id = *(int*)&data[idx + 4];
		asset->type = *(int*)&data[idx + 8];
		asset->assetData = *(asset_type_data_t*)&data[idx + 12];
		asset->data = &data[idx + 28];

		idx += 28 + asset->len;
	} while(asset->id != id && asset->len != 0);

	if(asset->len == 0)
		return -1;
	else
		return 0;
}