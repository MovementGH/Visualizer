#include "Visualizer.hpp"

Visualizer::Visualizer():m_SampleRate(44100),m_ChannelCount(2){ m_Clock.restart(); }
void Visualizer::render() {
    sf::Time ElapsedTime=m_Clock.restart();
    m_Texture.clear(sf::Color::Black);
    for(int i=0;i<m_Plugins.size();i++) {
        m_Plugins[i]->render(ElapsedTime);
        m_Texture.draw(m_PluginSprites[i]);
    }
    m_Texture.display();
}
void Visualizer::setRenderSize(sf::Vector2u Size) {
    for(int i=0;i<m_Plugins.size();i++) m_Plugins[i]->setRenderSize(Size);
    m_Texture.create(Size.x,Size.y);
}
void Visualizer::inputSamples(std::vector<sf::Int16> Samples) {
    std::vector<sf::Int16> ModifiedSamples(Samples.size()/m_ChannelCount);
    for(int i=0;i<m_Plugins.size();i++) {
        for(int i2=m_PluginSettings[i].inputChannel,i3=0;i2<Samples.size();i2+=m_ChannelCount,i3++) ModifiedSamples[i3]=Samples[i2]*m_PluginSettings[i].inputVolume;
        m_Plugins[i]->inputSamples(ModifiedSamples);
    }
}
void Visualizer::setStreamProperties(int SampleRate,int ChannelCount) {
    m_SampleRate=SampleRate;
    m_ChannelCount=ChannelCount;
}
void Visualizer::addPlugin(VisualizerPlugin* Plugin,VisualizerPluginSettings Settings) {
    sf::Sprite PluginSprite;
    Plugin->setRenderSize({(uint)Settings.displayPosition.width,(uint)Settings.displayPosition.height});
    Plugin->setStreamProperties(m_SampleRate,m_ChannelCount);
    PluginSprite.setTexture(Plugin->getTexture());
    PluginSprite.setPosition(Settings.displayPosition.left+Settings.displayPosition.width/2,Settings.displayPosition.top+Settings.displayPosition.height/2);
    PluginSprite.setOrigin(Settings.displayPosition.width/2,Settings.displayPosition.height/2);
    PluginSprite.setRotation(Settings.displayRotation);
    m_Plugins.push_back(Plugin);
    m_PluginSettings.push_back(Settings);
    m_PluginSprites.push_back(PluginSprite);
}
const sf::Texture& Visualizer::getTexture() { return m_Texture.getTexture(); }

