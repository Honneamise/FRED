#include "funcs.h"
#include "bus.h"

#include <stdlib.h>
#include <math.h>
#include <OPENAL/al.h>
#include <OPENAL/alc.h>

#define M_PI 3.14159265358979323846264338327950288

#define NUM_TONES           48
#define DEFAULT_VOLUME      0.50f
#define TONE_SAMPLE_RATE    22050

static const float TONE_FREQS[NUM_TONES] = 
{
    268.0f, 284.0f, 301.0f, 318.0f, 337.0f, 358.0f, 379.0f, 401.0f, 425.0f, 451.0f, 477.0f, 506.0f, 
    536.0f, 568.0f, 602.0f, 637.0f, 675.0f, 716.0f, 758.0f, 803.0f, 851.0f, 902.0f, 955.0f, 1012.0f, 
    1072.0f, 1136.0f, 1204.0f, 1275.0f, 1351.0f, 1432.0f, 1517.0f, 1607.0f, 1703.0f, 1804.0f, 1911.0f, 2025.0f, 
    2145.0f, 2273.0f, 2408.0f, 2551.0f, 2703.0f, 2864.0f, 3034.0f, 3215.0f, 3406.0f, 3608.0f, 3823.0f, 4050.0f, 
    // 4291.0f, 4547.0f, 4817.0f, 5103.0f, 5407.0f, 5728.0f, 6069.0f, 6430.0f, 6812.0f, 7217.0f, 7647.0f, 8101.0f, 
    // 8583.0f, 9094.0f, 9634.0f, 10207.0f, 10814.0f, 11457.0f, 12139.0f, 12860.0f, 13625.0f, 14435.0f, 15294.0f, 16203.0f, 
    // 17167.0f, 18188.0f, 19269.0f, 20415.0f, 21629.0f, 22915.0f, 24278.0f, 25721.0f, 27251.0f, 28871.0f, 30588.0f, 32407.0f, 
    // 34334.0f, 36376.0f, 38539.0f, 40833.0f, 43258.0f, 45830.0f, 48556.0f, 51443.0f, 54502.0f, 57743.0f, 61176.0f, 64814.0f
};

typedef struct AUDIO
{
    ALCdevice *adev;
    ALCcontext *actx;
    ALuint asrc_snd;
    ALuint asrc_tone;
    ALuint asnd;
    ALuint atones[NUM_TONES];

} AUDIO;

static AUDIO *audio = NULL;

/**********/
static char *openal_error(int32_t err)
{
    switch(err) 
    {
        case AL_NO_ERROR :          return NULL;
        case AL_INVALID_NAME:       return "AL_INVALID_NAME";
        case AL_INVALID_ENUM:       return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE:      return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION:  return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY:      return "AL_OUT_OF_MEMORY";
    }

	return "UNKNOW OPEN AL ERROR";
}

/**********/
static void openal_check_error(void)
{
    int32_t err = alGetError();

	if(err!=0)
    {
        error("[%s][OPENAL][%s]", __func__, openal_error(err));
    }
}

/**********/
static void openal_generate_tone(uint8_t *buf, int32_t sample_rate, float freq)
{
    for(int32_t i=0;i<sample_rate;i++)
    {
        buf[i] = (uint8_t)( 0xFF * sin( M_PI*freq/sample_rate*i ) );
    }
}

/**********/
static void openal_init(void)
{
    char *name = (char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
   
    audio->adev = alcOpenDevice(name);

    if(!audio->adev){ error("[%s][FAILED OPENING AUDIO DEVICE]", __func__); };

    audio->actx = alcCreateContext(audio->adev, NULL);

    if(!audio->actx){ error("[%s][FAILED CREATING AUDIO CONTEXT]", __func__); }

    if(!alcMakeContextCurrent(audio->actx)){ error("[%s][FAILED TO SET AUDIO CONTEXT]", __func__); }

    alGenSources(1, &audio->asrc_snd);
    openal_check_error();

    alGenSources(1, &audio->asrc_tone);
    openal_check_error();

    alGenBuffers(1, &audio->asnd);
    openal_check_error();

    alGenBuffers(NUM_TONES, audio->atones);
    openal_check_error();

    //tones generation
    uint8_t *buf = alloc(TONE_SAMPLE_RATE, sizeof(uint8_t));

    for(int32_t i=0;i<NUM_TONES;i++)
    {
        openal_generate_tone(buf, TONE_SAMPLE_RATE, TONE_FREQS[i]);
        alBufferData(audio->atones[i], AL_FORMAT_MONO8, buf, TONE_SAMPLE_RATE, TONE_SAMPLE_RATE);
        openal_check_error();
    } 

    free(buf); 

    alSourcef(audio->asrc_snd, AL_GAIN, DEFAULT_VOLUME);
    openal_check_error();

    alSourcef(audio->asrc_tone, AL_GAIN, DEFAULT_VOLUME);
    openal_check_error();

}

/**********/
static void openal_close(void)
{
    alSourceStop(audio->asrc_snd); 

    alSourceStop(audio->asrc_tone);

    alDeleteBuffers(1, &audio->asnd);

    alDeleteBuffers(7, audio->atones);

    alDeleteSources(1, &audio->asrc_snd);

    alDeleteSources(1, &audio->asrc_tone);

    alcMakeContextCurrent(NULL);

    alcDestroyContext(audio->actx);

    alcCloseDevice(audio->adev);
}

/**********/
static void stop_tone(void)
{
    int32_t status = 0;

    alGetSourcei(audio->asrc_tone,AL_SOURCE_STATE,&status);
    openal_check_error();

    if(status==AL_PLAYING) 
    { 
        alSourceStop(audio->asrc_tone); 
        openal_check_error();

        alSourcei(audio->asrc_tone, AL_LOOPING, 0);
        openal_check_error();
    }
}

/**********/
static void play_tone(int32_t tone)
{ 
    stop_tone();

    int32_t _tone = tone % NUM_TONES;

    alSourcei(audio->asrc_tone, AL_BUFFER, audio->atones[_tone]);
    openal_check_error();

    alSourcei(audio->asrc_tone, AL_LOOPING, 1);
    openal_check_error();

    alSourcePlay(audio->asrc_tone);
    openal_check_error();
}

/**********/
void audio_init(void)
{
    audio = alloc(1, sizeof(AUDIO));

    openal_init();
}

/**********/
void audio_close(void)
{
    openal_close();

    free(audio);
}

/**********/
void audio_output_func(void)
{
    uint8_t d = bus_read();

    if(d>0 && d<NUM_TONES){ play_tone(d-1); }
    else { stop_tone(); }
}
