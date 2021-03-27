#if defined(_WIN32) && defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS //for fopen...not sure if this is only needed in vs
#pragma message("Run in windows vs!")
#endif
#if defined(__linux__)
#pragma message("Run in Linux!")
#endif

#include "kywav.h"
#include "string.h"

int wavsolver_open(wavsolver* wavsolver, const char* filename)
{
	FILE* _file=NULL;
	wavinfo* _wavinfo=NULL;


	_wavinfo = (wavinfo*)calloc(1, sizeof(wavinfo));

	_file = fopen(filename, "rb");
	if (!_file)
		return -2;


	if (fread(&_wavinfo->chunk_id, 4, 1, _file)!=1) return -1;
	if (strncmp(&_wavinfo->chunk_id,"RIFF",4)) return -1;
	if (fread(&_wavinfo->chunk_size, 4, 1, _file)!=1)return -1;
	if (fread(&_wavinfo->format, 4, 1, _file) != 1) return -1;
	if (strncmp(&_wavinfo->format, "WAVE", 4)) return -1;
#if 0
	for (int i = 0; i < 4; i++)
		printf("%c\n", ((char*)&_wavinfo->format)[i]);
#endif

	// read out the chunk we don't need until reach the fmt chunk
	while (1) {
		if (fread(&_wavinfo->subchunk_id, 4, 1, _file) != 1)return -1;
		if (strncmp(&_wavinfo->subchunk_id, "fmt ", 4)) {
			if (fread(&_wavinfo->subchunk_size, 4, 1, _file) != 1) return -1;			
			unsigned char* dummy_buf = malloc(_wavinfo->subchunk_size);
			if (fread(dummy_buf, _wavinfo->subchunk_size, 1, _file) != 1) return -1;
			free(dummy_buf);			
		}
		else {
			break;
		}
	}
	
	if (fread(&_wavinfo->subchunk_size, 4, 1, _file)!= 1)return -1;
	if (fread(&_wavinfo->audio_format, 2, 1, _file) != 1)return -1;
	if (fread(&_wavinfo->num_channels, 2, 1, _file) != 1)return -1;
	if (fread(&_wavinfo->sample_rate, 4, 1, _file) != 1)return -1;
	if (fread(&_wavinfo->byte_rate, 4, 1, _file) != 1)return -1;
	if (fread(&_wavinfo->block_align, 2, 1, _file) != 1)return -1;
	if (fread(&_wavinfo->bps, 2, 1, _file) != 1)return -1;
	
	int offset=0;
	if ((short)_wavinfo->audio_format == -2) {
		fseek(_file, 8, SEEK_CUR);
		if(fread(&_wavinfo->audio_format, 2, 1, _file) != 1) return -1;
		fseek(_file, 14, SEEK_CUR);
		offset = _wavinfo->subchunk_size - 40;
	}
	else {
		offset = _wavinfo->subchunk_size - 16;
	}
	if (_wavinfo->audio_format != 1) return -1;

	
	if (offset != 0) {
		unsigned char* dummy_buf = malloc(offset);
		if (fread(dummy_buf, offset, 1, _file) != 1) return -1;
		free(dummy_buf);
	}

	if (fread(&_wavinfo->subchuck2_id, 4, 1, _file) != 1)return -1;
	if (strncmp(&_wavinfo->subchuck2_id, "data", 4)) return -1;
	if (fread(&_wavinfo->subchunck2_size, 4, 1, _file) != 1)return -1;
	
#if 0
	printf("num_channels = %d\n", _wavinfo->num_channels);
	printf("sample_rate = %d\n", _wavinfo->sample_rate);
	printf("bps = %d\n", _wavinfo->bps);
	printf("data_length = %d\n", _wavinfo->subchunck2_size);
#endif

	wavsolver->file = _file;
	wavsolver->wavinfo = _wavinfo;

	return 0;
}

int wavsolver_get(wavsolver* wavsolver, enum walvsolver_get_type type, int* value) 
{
	int tmp = 0;
	switch(type) 
	{
		case NUM_CHANNELS:			
			tmp = wavsolver->wavinfo->num_channels;
			break;
		case SAMPLE_RATE:			
			tmp = wavsolver->wavinfo->sample_rate;
			break;
		case BPS:			
			tmp = wavsolver->wavinfo->bps;
			break;
		default:
			return -1;
	}

	if (value == NULL) {
		return tmp;
	}
	else {
		*value = tmp;
		return 0;
	}
}

int wavsolver_read(wavsolver* wavsolver, void* buff, unsigned int num_samples) 
{
	unsigned short num_channels = wavsolver->wavinfo->num_channels;
	unsigned short bps = wavsolver->wavinfo->bps;
	FILE* _file = wavsolver->file;
	unsigned int num_samples_ch = num_samples * num_channels;
	unsigned char sample_bytes = bps / 8;
	if (fread(buff, sample_bytes, num_samples_ch, _file) != num_samples_ch) return -1;
	return 0;
}

int wavsolver_useless(wavsolver* wavsolver)
{
	printf("Kaiden Yu says hello :D\n");
}

int wavsolver_release(wavsolver* wavsolver)
{
	if(wavsolver->wavinfo)
		free(wavsolver->wavinfo);

	fclose(wavsolver->file);
}