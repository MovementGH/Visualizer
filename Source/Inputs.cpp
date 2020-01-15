#include "Inputs.hpp"

void LiveVisualizer::Play(unsigned int sampleRate) {
    setProcessingInterval(sf::milliseconds(5));
    setChannelCount(2);
    setStreamProperties(sampleRate,2);
    start(sampleRate);
}
void LiveVisualizer::Stop() { stop(); }
bool LiveVisualizer::onProcessSamples(const sf::Int16* samples,std::size_t sampleCount) {
    m_Samples.resize(sampleCount);
    for(int i=0;i<sampleCount;i++) m_Samples[i]=samples[i];
    inputSamples(m_Samples);
    return true;
}