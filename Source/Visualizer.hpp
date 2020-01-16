#ifndef Visualizer_h
#define Visualizer_h
#include <vector>
#include <SFML/Graphics.hpp>
#include "Plugins.hpp"

class Visualizer {
    public:
    Visualizer();
    void render();
    void inputSamples(std::vector<sf::Int16> Samples);
    void setStreamProperties(int SampleRate,int ChannelCount);
    void addPlugin(VisualizerPlugin* Plugin,VisualizerPluginSettings Settings);
    void setRenderSize(sf::Vector2u Size);
    void setOutputSize(sf::Vector2u Size,float Rotation,float Scaling);

    const sf::Texture& getTexture();
    protected:
    std::vector<VisualizerPlugin*> m_Plugins;
    std::vector<sf::Sprite> m_PluginSprites;
    std::vector<VisualizerPluginSettings> m_PluginSettings;
    sf::RenderTexture m_Texture;
    sf::View m_View;
    int m_SampleRate,m_ChannelCount;
    sf::Clock m_Clock;
};

#endif