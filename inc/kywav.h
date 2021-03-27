#ifndef KYWAV_H
#define KYWAV_H

#include <stdio.h>
#include <stdlib.h>

/**
 * assign function to function pointer is dirty work
 * 
 */

enum walvsolver_get_type
{
	NUM_CHANNELS,
	SAMPLE_RATE,
	BPS
};

typedef struct {
	unsigned int chunk_id;
	unsigned int chunk_size;
	unsigned int format;

	unsigned int subchunk_id;
	unsigned int subchunk_size;
	unsigned short audio_format;
	unsigned short num_channels;
	unsigned int sample_rate;
	unsigned int byte_rate;
	unsigned short block_align;
	unsigned short bps;

	unsigned int subchuck2_id;
	unsigned int subchunck2_size;
	void* data;
} wavinfo;

typedef struct wavsolver {
	FILE* file;
	wavinfo* wavinfo;
	struct wavsolver* self;
	int (*open)(struct wavsolver* ws, const char* filename);
	int (*get)(struct wavsolver* ws, enum walvsolver_get_type type, void* value);
	int (*read)(struct wavsolver* ws, void* buff, unsigned int num_samples);
	int (*useless)(struct wavsolver* ws);
	int (*release)(struct wavsolver* ws);
} wavsolver;


int wavsolver_open(wavsolver* wavsolver, const char* filename);
int wavsolver_get(wavsolver* wavsolver, enum walvsolver_get_type type, int* value);
int wavsolver_read(wavsolver* wavsolver, void* buff, unsigned int num_samples);
int wavsolver_useless(wavsolver* wavsolver);
int wavsolver_release(wavsolver* wavsolver);

#define GEN_WAVSOLVER(name) \
	wavsolver name = { NULL, NULL, &name, \
						wavsolver_open, \
						wavsolver_get, \
						wavsolver_read, \
						wavsolver_useless, \
						wavsolver_release }

#endif
