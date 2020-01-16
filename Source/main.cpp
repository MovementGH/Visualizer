#include <SFML/Graphics.hpp>
#include "Platform.hpp"
#include "Inputs.hpp"
#include "../SelbaWard/SelbaWard.hpp"

int main() {
    sf::Vector2u Size(1280,1024);

    sf::RenderWindow Window;
    sf::ContextSettings Settings;
    Settings.antialiasingLevel=8;
    Window.create(sf::VideoMode(Size.x,Size.y),"Test",sf::Style::Default,Settings);
    Window.setVerticalSyncEnabled(true);

    LiveVisualizer Visualizer;
    Visualizer.setRenderSize(Size/(unsigned)2);
    Visualizer.setOutputSize({Size.y/2,Size.x/2},90,1);
    Visualizer.addPlugin(new Plugin::ConfettiCircles({1,10},{15,85},{.1,1},{sf::Color::White,{0,0,0,0}},{{255,255,255},{0,0,0,0}},{0,0},{99,0},{1,4},1.2,10),{0,1,{0,0,Size.x/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::Lasers(1.2,{1,0},{1,0},{2,0},{2,0},{sf::Color::White,{0,0,0,0}},10,"Product/Grad.png"),{0,2,{0,0,Size.x/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(4,sf::Color::Black,{255,255,255,255},2),{0,1,{-(int)Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(2,sf::Color::Black,{255,255,255,212},2),{0,.5,{-(int)Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(1,sf::Color::Black,{255,255,255,170},2),{0,.25,{-(int)Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(.5,sf::Color::Black,{255,255,255,128},2),{0,.125,{-(int)Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(4,sf::Color::Black,{255,255,255,255},2),{0,1,{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(2,sf::Color::Black,{255,255,255,212},2),{0,.5,{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(1,sf::Color::Black,{255,255,255,170},2),{0,.25,{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::VolumeCircle(.5,sf::Color::Black,{255,255,255,128},2),{0,.125,{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0});
    Visualizer.addPlugin(new Plugin::Amplitude(4,1280,1,{128,128,128},{128,128,128},0,0.05,1),{0,1,{0,.75*Size.y/2,Size.x/2,Size.y/8},0});
    Visualizer.addPlugin(new Plugin::Amplitude(4,1280,1,{128,128,128},{128,128,128},0,0.05,1),{0,1,{0,0,Size.x/2,Size.y/8},180});
    Visualizer.addPlugin(new Plugin::Hanning(512,sf::Color::White),{0,1,{0,Size.y/8,Size.x/2,Size.y/4},0});
    Visualizer.addPlugin(new Plugin::Pitch(2048,sf::Color::White,sf::Color::White,true,3),{0,1,{0,0,Size.x/2,Size.y/2},0});
    Visualizer.Play();
    
    sw::Hexahedron Hexahedron({Size.y/2.f,Size.x/2.f,Size.x/2.f},2);
    Hexahedron.rotate({90,0,90});
    for(int i=0;i<4;i++)
        Hexahedron.setTexture(sw::Hexahedron::Face(i),sw::Hexahedron::Side::Outside,Visualizer.getTexture());
    Hexahedron.setPosition({Size.x/2,Size.y/2});
    
    while(Window.isOpen()) {
        sf::Event Event;
        while(Window.pollEvent(Event)) {
            if(Event.type==sf::Event::Closed) Window.close();
        }
        Hexahedron.rotate({0.5,.1,0});
        Window.clear();
        Visualizer.render();
        Hexahedron.draw(&Window);
        Window.display();
    }
}