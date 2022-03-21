#ifndef GET_ASSET_H
#define GET_ASSET_H

#include <stdint.h>

typedef struct asset_type_data_s {
	int data1;
	int data2;
	int data3;
	int data4;
} asset_type_data_t;

typedef struct asset_image_type_data_s {
	int width;
	int height;
	int data3;
	int data4;
} asset_image_type_data_t;

typedef struct asset_s {
	uint8_t* data;
	int id;
	int len;
	int type;
	asset_type_data_t assetData;
} asset_t;

int get_asset_by_id(asset_t* asset, int id);

#endif