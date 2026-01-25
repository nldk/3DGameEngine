//
// Created by niel on 1/24/26.
//

#include "soundEngine.h"

Engine::Sound::Sound(std::string fileName, bool looping) {
    if (!buffer.loadFromFile(fileName)) {
        throw std::runtime_error("Failed to load sound file: " + fileName);
    }
    sound.setBuffer(buffer);
    sound.setLoop(looping);
}
void Engine::AudioPlayerG::Update(double deltaTime) {
    if (loopCount > 0) {
        // Only play if the sound is stopped
        if (soundToPlay.getStatus() == sf::Sound::Stopped) {
            soundToPlay.play();
            loopCount--;
        }
    } else if (playing) {
        // Play the sound once if needed
        soundToPlay.play();
        playing = false;
    }
}