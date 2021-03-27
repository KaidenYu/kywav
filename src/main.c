#include <stdio.h>
#include <stdlib.h>

#include "kywav.h"

#if defined(_WIN32)
#define WAV_IN "resources\\A_FS48K.WAV"
//#define WAV_IN "resources\\B_FS44K.WAV"
//#define WAV_IN "resources\\C_FS96K.WAV"
#endif

#if defined(__linux__)
#define WAV_IN "resources/A_FS48K.WAV"
//#define WAV_IN "resources/B_FS44K.WAV"
//#define WAV_IN "resources/C_FS96K.WAV"
#endif


#define FRAME_LENGTH_PER_CHANNEL	10


int main() 
{		
	int channels;
	int samplerate;
	int bps;
	unsigned char* buffer;

	// use macro to delcare wavsolver
	// assign function address to function pointer of the wavsolver struct is a routine stuff
	// unless you want to define your own cutomize function...
	GEN_WAVSOLVER(myws);

	myws.open(&myws, WAV_IN);

	/* .get usage method 1*/
	myws.get(&myws, NUM_CHANNELS, &channels);
	myws.get(&myws, SAMPLE_RATE, &samplerate);
	myws.get(&myws, BPS, &bps);
	printf(".get method 1: channels = %d, samplerate = %d bps = %d\n", channels, samplerate, bps);

	/* reset variable for .get method 2*/
	channels = 0;
	samplerate = 0;
	bps = 0;

	/* .get usage method 2*/
	channels = myws.get(&myws, NUM_CHANNELS, NULL);
	samplerate = myws.get(&myws, SAMPLE_RATE, NULL);
	bps = myws.get(&myws, BPS, NULL);
	printf(".get method 2: channels = %d, samplerate = %d bps = %d\n", channels, samplerate, bps);

	buffer = malloc(FRAME_LENGTH_PER_CHANNEL * channels * (bps / 8));

	myws.read(&myws, buffer, FRAME_LENGTH_PER_CHANNEL);

	myws.release(&myws);

	// ...
	myws.self->self->self->self->self->self->self->self->self->self->self->self->self->self->useless(&myws);
	
	free(buffer);

	return 0;	
}