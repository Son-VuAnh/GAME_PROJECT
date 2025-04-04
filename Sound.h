#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>

class Sound
{
public:
    Sound();
    ~Sound();

    void init();

    void play_button_sound();
    void play_move_sound();
    void play_illegal_move_sound();

    void play_winning_sound();
    void play_drawing_sound();

private:
    Mix_Chunk* button_sound;
    Mix_Chunk* move_sound;
    Mix_Chunk* illegal_move_sound;

    Mix_Chunk* winning_sound;
    Mix_Chunk* drawing_sound;
};

#endif
