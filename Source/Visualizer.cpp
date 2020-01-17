#include "Visualizer.hpp"

Visualizer::Visualizer():m_SampleRate(44100),m_ChannelCount(2){ m_Clock.restart(); }
void Visualizer::render() {
    sf::Time ElapsedTime=m_Clock.restart();
    m_Texture.clear(sf::Color::Black);
    for(int i=0;i<m_Plugins.size();i++) {
        m_Plugins[i]->render(ElapsedTime);
        for(int i2=0;i2<m_PluginSprites[i].size();i2++) m_Texture.draw(m_PluginSprites[i][i2]);
    }
    m_Texture.display();
}
void Visualizer::setRenderSize(sf::Vector2u Size) {
    for(int i=0;i<m_Plugins.size();i++) m_Plugins[i]->setRenderSize(Size);
    m_View.setCenter({Size.x/2,Size.y/2});
    m_Texture.setView(m_View);
}
void Visualizer::setOutputSize(sf::Vector2u Size,float Rotation,float Scaling) {
    m_View.setRotation(Rotation);
    m_View.setSize({Size.x,Size.y});
    m_View.zoom(Scaling);
    m_Texture.create(Size.x,Size.y);
    m_Texture.setView(m_View);
}
void Visualizer::inputSamples(std::vector<sf::Int16>& Samples) {
    std::vector<sf::Int16> ModifiedSamples(Samples.size()/m_ChannelCount);
    for(int i=0;i<m_Plugins.size();i++) {
        for(int i2=m_InputSettings[i].channel,i3=0;i2<Samples.size();i2+=m_ChannelCount,i3++) ModifiedSamples[i3]=Samples[i2]*m_InputSettings[i].volume;
        m_Plugins[i]->inputSamples(ModifiedSamples);
    }
}
void Visualizer::setStreamProperties(int SampleRate,int ChannelCount) {
    m_SampleRate=SampleRate;
    m_ChannelCount=ChannelCount;
}
void Visualizer::addPlugin(VisualizerPlugin* Plugin,VisualizerPluginInputSettings Input,std::vector<VisualizerPluginDisplaySettings> Display) {
    sf::Vector2u LargestRender(0,0);
    for(int i=0;i<Display.size();i++) {
        if(Display[i].position.width>LargestRender.x) LargestRender.x=Display[i].position.width;
        if(Display[i].position.height>LargestRender.y) LargestRender.y=Display[i].position.height;
    }
    Plugin->setRenderSize(LargestRender);
    Plugin->setStreamProperties(m_SampleRate,m_ChannelCount);
    m_Plugins.push_back(Plugin);
    m_PluginSprites.push_back(std::vector<sf::Sprite>(Display.size()));
    std::vector<sf::Sprite>& PluginSprites=m_PluginSprites[m_PluginSprites.size()-1];
    for(int i=0;i<Display.size();i++) {
        PluginSprites[i].setTexture(Plugin->getTexture());
        PluginSprites[i].setTextureRect({0,0,LargestRender.x,LargestRender.y});
        PluginSprites[i].setScale((float)Display[i].position.width/(float)LargestRender.x,(float)Display[i].position.height/(float)LargestRender.y);
        PluginSprites[i].setOrigin(LargestRender.x/2,LargestRender.y/2);
        PluginSprites[i].setRotation(Display[i].rotation);
        PluginSprites[i].setPosition(Display[i].position.left+Display[i].position.width/2,Display[i].position.top+Display[i].position.height/2);
    }
    m_InputSettings.push_back(Input);
}
const sf::Texture& Visualizer::getTexture() { return m_Texture.getTexture(); }

