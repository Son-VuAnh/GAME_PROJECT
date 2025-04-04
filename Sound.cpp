#include "Sound.h"

Sound::Sound() : button_sound(nullptr), move_sound(nullptr) {}

Sound::~Sound()
{

    if (button_sound)
    {
        Mix_FreeChunk(button_sound);
        button_sound = nullptr;
    }

    if (move_sound)
    {
        Mix_FreeChunk(move_sound);
        move_sound = nullptr;
    }

    if (illegal_move_sound)
    {
        Mix_FreeChunk(illegal_move_sound);
        illegal_move_sound = nullptr;
    }

    if (winning_sound)
    {
        Mix_FreeChunk(winning_sound);
        winning_sound = nullptr;
    }

    if (drawing_sound)
    {
        Mix_FreeChunk(drawing_sound);
        drawing_sound = nullptr;
    }

    Mix_CloseAudio();
    Mix_Quit();
}

void Sound::init()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Volume(-1, 15);

    button_sound = Mix_LoadWAV("assets/sound/button_click.wav");
    move_sound = Mix_LoadWAV("assets/sound/move.wav");
    illegal_move_sound = Mix_LoadWAV("assets/sound/illegal.wav");

    winning_sound = Mix_LoadWAV("assets/sound/winning.wav");
    drawing_sound = Mix_LoadWAV("assets/sound/drawing.wav");
}

void Sound::play_button_sound()
{
    if (button_sound)
    {
        Mix_PlayChannel(-1, button_sound, 0);
    }
}

void Sound::play_move_sound()
{
    if (move_sound)
    {
        Mix_PlayChannel(-1, move_sound, 0);
    }
}

void Sound::play_illegal_move_sound()
{
    if (illegal_move_sound)
    {
        Mix_PlayChannel(-1, illegal_move_sound, 0);
    }
}

void Sound::play_winning_sound()
{
    if (winning_sound)
    {
        Mix_PlayChannel(-1, winning_sound, 0);
    }
}

void Sound::play_drawing_sound()
{
    if (button_sound)
    {
        Mix_PlayChannel(-1, drawing_sound, 0);
    }
}
