#ifndef Input_h
#define Input_h
#include <SFML/Audio.hpp>
#include "Visualizer.hpp"

class LiveVisualizer : public Visualizer,public sf::SoundBufferRecorder {
    public:
    void Play(unsigned int sampleRate=44100);
    void Stop();
    private:
    virtual bool onProcessSamples(const sf::Int16* samples,std::size_t sampleCount);
    std::vector<sf::Int16> m_Samples;
};

#endif