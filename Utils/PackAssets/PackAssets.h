#ifndef PACKASSETS_H
#define PACKASSETS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <dirent.h>

#include "common.h"

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
	int len;
	int id;
	int assetType;
	asset_type_data_t assetTypeData;
	uint8_t* rawData;
} asset_t;

typedef struct asset_link_s asset_link_t;
typedef struct asset_link_s {
	asset_t* asset;
	asset_link_t* next;
} asset_link_t;

asset_t* getNewAsset(asset_link_t**);

#endif