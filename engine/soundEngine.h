//
// Created by niel on 1/24/26.
//

#ifndef NIELS3DGAMEENGINE_SOUNDENGINE_H
#define NIELS3DGAMEENGINE_SOUNDENGINE_H
#include <SFML/Audio.hpp>

#include "classRegistarion.h"

namespace Engine {
    class Sound {
        sf::SoundBuffer buffer;
        sf::Sound sound;
    public:
        Sound(std::string fileName, bool looping = false);
        void play() {
            sound.play();
        }
        void pause() {
            sound.pause();
        }
        void stop() {
            sound.stop();
        }
        void setVolume(float volume) {
            sound.setVolume(volume);
        }
        sf::Sound::Status getStatus() {
            return sound.getStatus();
        }
        void setLooping(bool looping) {
            sound.setLoop(looping);
        }
    };
    class AudioPlayerG: public System {
        bool playing;
        int loopCount;
    public:
        Sound soundToPlay;
        AudioPlayerG(std::string fileName, bool looping = false) : soundToPlay(fileName,looping) {}
        void playSound(int loops = 1) {
            playing = true;
            loopCount = loops;
        }
        void stopSound() {
            playing = false;
            soundToPlay.stop();
        }
        void setVolume(float volume) {
            soundToPlay.setVolume(volume);
        }
        void setLooping(bool looping) {
            soundToPlay.setLooping(looping);
        }
        void Update(double deltaTime);

    };
};
#endif //NIELS3DGAMEENGINE_SOUNDENGINE_H