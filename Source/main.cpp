#include <SFML/Graphics.hpp>
#include "Platform.hpp"
#include "Inputs.hpp"
#include "../SelbaWard/SelbaWard.hpp"

int main() {
    sf::Vector2u Size(1920,1080);

    sf::RenderWindow Window;
    sf::ContextSettings Settings;
    Settings.antialiasingLevel=8;
    Window.create(sf::VideoMode(Size.x,Size.y),"Test",sf::Style::Default,Settings);
    Window.setVerticalSyncEnabled(true);

    LiveVisualizer Visualizer;
    Visualizer.setRenderSize(Size);
    Visualizer.addPlugin(new Plugin::ConfettiCircles({1,10},{15,85},{.1,1},{sf::Color::White,{0,0,0,0}},{{255,255,255},{0,0,0,0}},{0,0},{99,0},{1,4},1.2,10),{0,1,{0,0,Size.x,Size.y},0});
    Visualizer.addPlugin(new Plugin::Lasers(1.2,{1,0},{1,0},{2,0},{2,0},{sf::Color::White,{0,0,0,0}},10,"Product/Grad.png"),{0,2,{0,0,Size.x,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(4,sf::Color::Black,{255,255,255,255},2),{0,1,{-(int)Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(2,sf::Color::Black,{255,255,255,212},2),{0,.5,{-(int)Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(1,sf::Color::Black,{255,255,255,170},2),{0,.25,{-(int)Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(.5,sf::Color::Black,{255,255,255,128},2),{0,.125,{-(int)Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(4,sf::Color::Black,{255,255,255,255},2),{1,1,{Size.x-Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(2,sf::Color::Black,{255,255,255,212},2),{1,.5,{Size.x-Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(1,sf::Color::Black,{255,255,255,170},2),{1,.25,{Size.x-Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(.5,sf::Color::Black,{255,255,255,128},2),{1,.125,{Size.x-Size.y/2,0,Size.y,Size.y},0});
    Visualizer.addPlugin(new Plugin::Amplitude(3,1280,1,{128,128,128},{128,128,128},0,.025,2),{0,1,{0,.75*Size.y,Size.x,Size.y/4},0});
    Visualizer.addPlugin(new Plugin::Amplitude(3,1280,1,{128,128,128},{128,128,128},0,.025,2),{0,1,{0,0,Size.x,Size.y/4},180});
    Visualizer.addPlugin(new Plugin::Hanning(2048,sf::Color::White),{0,1,{0,Size.y/4,Size.x,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::Pitch(2048,sf::Color::White,sf::Color::White,true,3),{0,1,{0,0,Size.x,Size.y},0});
    Visualizer.Play();
    sf::Sprite Sprite;
    Sprite.setTexture(Visualizer.getTexture());
    while(Window.isOpen()) {
        sf::Event Event;
        while(Window.pollEvent(Event)) {
            if(Event.type==sf::Event::Closed) Window.close();
        }
        Window.clear(sf::Color::Black);
        Visualizer.render();
        Window.draw(Sprite);
        Window.display();
    }
}