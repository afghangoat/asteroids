#include "audiomanager.h"

int init_audio(AudioManager* aman) {
    aman->audio_device = 0;
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("A az SDL nem tudta inicializálni a hanglejátszót megfelelően. Hiba: %s\n", SDL_GetError());
        return -1;
    }

    SDL_AudioSpec desired_spec;
    SDL_zero(desired_spec);
    desired_spec.format = AUDIO_U8;     // Wav formátum
	desired_spec.channels = 1;          //1 Channel
	desired_spec.freq = 44100;          //44100 Hz
    desired_spec.samples = 4096;

    aman->audio_device = SDL_OpenAudioDevice(NULL, 0, &desired_spec, NULL, 0);
    if (aman->audio_device == 0) {
        printf("A hanglejátszó nem indult el megfelelően: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_PauseAudioDevice(aman->audio_device, 0);
    return 0;
}

int play_sound(AudioManager* aman, const char* file_path) {
    if (aman->audio_device == 0) {
        printf("A hanglejátszó nem indult el megfelelően.\n");
        return -1;
    }

    SDL_AudioSpec wav_spec;
    Uint8* wav_buffer;
    Uint32 wav_length;

    if (SDL_LoadWAV(file_path, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        printf("Audio fájl nem lett betöltve: %s\n", SDL_GetError());
        return -1;
    }

    if (SDL_QueueAudio(aman->audio_device, wav_buffer, wav_length) < 0) {
        printf("Audio nem lett berakva: %s\n", SDL_GetError());
        SDL_FreeWAV(wav_buffer);
        return -1;
    }

	//Hang fájl lejátszása
    SDL_PauseAudioDevice(aman->audio_device, 0);

    SDL_FreeWAV(wav_buffer);

    return 0;
}

void cleanup_audio(AudioManager* aman) {
    if (aman->audio_device != 0) {
        SDL_CloseAudioDevice(aman->audio_device);
    } else {
        printf("Az audio manager nem állt le megfelelően!");
    }
}

