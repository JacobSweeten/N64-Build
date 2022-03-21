#include "PackAssets.h"

void appendAsset(asset_link_t** assetList, asset_t* asset)
{
	asset_link_t* current = *assetList;
	asset_link_t* newLink = (asset_link_t*)malloc(sizeof(asset_link_t));
	newLink->asset = asset;
	newLink->next = NULL;

	if(current == NULL)
	{
		*assetList = newLink;
		return;
	}

	while(current->next != NULL)
	{
		current = current->next;
	}

	current->next = newLink;
}

asset_t* getNewAsset(asset_link_t** assetList)
{
	asset_t* newAsset = (asset_t*)malloc(sizeof(asset_t));
	appendAsset(assetList, newAsset);

	return newAsset;
}

void getFileName(char* buff, char* s)
{
	int lastDot = -1;
	for(int i = strlen(s) - 1; i >= 0; i--)
	{
		if(s[i] == '.')
		{
			lastDot = i;
		}
	}

	memcpy(buff, s, lastDot);
}

bool endsWith(char* s, char* end)
{
	int sLen = strlen(s);
	int endLen = strlen(end);

	for(int i = sLen - endLen; i < sLen; i++)
	{
		if(s[i] != end[i - sLen + endLen])
		{
			return false;
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	asset_link_t* assetList = NULL;

	DIR* assetDir = opendir(argv[1]);
	struct dirent* subDir;
	subDir = readdir(assetDir);
	while(subDir != NULL)
	{
		if(strncmp(subDir->d_name, ".", 1) && strncmp(subDir->d_name, "..", 1))
		{
			char* fullFileName = (char*)malloc(strlen(argv[1] + strlen(subDir->d_name) + 1));
			sprintf(fullFileName, "%s/%s", argv[1], subDir->d_name);

			char* fileName = (char*)malloc(strlen(subDir->d_name));
			getFileName(fileName, subDir->d_name);

			asset_t* newAsset = getNewAsset(&assetList);
			FILE* assetFile = fopen(fullFileName, "r");

			fseek(assetFile, 0, SEEK_END);
			int assetFileLen = ftell(assetFile);
			fseek(assetFile, 0, SEEK_SET);
			
			if(endsWith(subDir->d_name, ".IMG"))
			{
				newAsset->id = atoi(fileName);
				newAsset->len = assetFileLen - 4;

				asset_image_type_data_t img_data;
				uint16_t height;
				uint16_t width;
				fread(&height, 2, 1, assetFile);
				fread(&width, 2, 1, assetFile);

				img_data.height = height << 16;
				img_data.width = width << 16;

				newAsset->assetTypeData = *(asset_type_data_t*)(&img_data);

				newAsset->rawData = (uint8_t*)malloc(assetFileLen - 4);
				fread(newAsset->rawData, 1, assetFileLen - 4, assetFile);
			}

			fclose(assetFile);
			free(fullFileName);
			free(fileName);
		}

		subDir = readdir(assetDir);
	}

	closedir(assetDir);

	printf("Done enumerating assets.\n");

	int assetFileLen = 0;
	
	asset_link_t* currentLink = assetList;
	while(currentLink != NULL)
	{
		assetFileLen += 28;
		assetFileLen += currentLink->asset->len;

		currentLink = currentLink->next;
	}

	uint8_t* assetFile = (uint8_t*)malloc(assetFileLen);
	int assetFileIdx = 0;

	currentLink = assetList;
	while(currentLink != NULL)
	{
		asset_t* asset = currentLink->asset;
		*(uint32_t*)(&assetFile[assetFileIdx]) = swapEndianness32(asset->len);
		*(uint32_t*)(&assetFile[assetFileIdx + 4]) = swapEndianness32(asset->id);
		*(uint32_t*)(&assetFile[assetFileIdx + 8]) = swapEndianness32(asset->assetType);
		*(asset_type_data_t*)(&assetFile[assetFileIdx + 12]) = asset->assetTypeData;
		memcpy(&assetFile[assetFileIdx + 28], asset->rawData, asset->len);

		assetFileIdx += 28;
		assetFileIdx += asset->len;

		currentLink = currentLink->next;
	}

	FILE* outFile = fopen(argv[2], "w");
	fwrite(assetFile, 1, assetFileLen, outFile);
	fclose(outFile);

	return 0;
}