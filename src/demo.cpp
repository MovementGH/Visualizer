#include <SFML/Graphics.hpp>
#include <Inputs.hpp>

int main(int argc,char** argv) {
    sf::Vector2u Size(1920,1080);
    int Input=0;
    if(argc>1)
        Input=std::atoi(argv[1]);

    sf::RenderWindow Window;
    sf::ContextSettings Settings;
    Settings.antialiasingLevel=8;
    Window.create(sf::VideoMode(Size.x,Size.y),"Visualizer Demo",sf::Style::Default,Settings);
    Window.setVerticalSyncEnabled(true);

    LiveVisualizer Visualizer;
    Visualizer.setDevice(Visualizer.getAvailableDevices()[Input]);
    Visualizer.setAmplification(argc>2?std::atoi(argv[2]):1.f);
    Visualizer.setRenderSize(Size/(unsigned)2);
    Visualizer.setOutputSize({Size.y/2,Size.x/2},90,1);
    Visualizer.addPlugin(new Plugin::ConfettiCircles({1,10},{15,85},{.1,1},{sf::Color::White,{0,0,0,0}},{{255,255,255},{0,0,0,0}},{0,0},{99,0},{1,4},1.2,10),{0,1},{{{0,0,Size.x/2,Size.y/2},0}});
    Visualizer.addPlugin(new Plugin::Lasers(1.2,{1,0},{1,0},{2,0},{2,0},{sf::Color::White,{0,0,0,0}},10,"Grad.png"),{0,2},{{{0,0,Size.x/2,Size.y/2},0}});
    Visualizer.addPlugin(new Plugin::VolumeCircle(4,sf::Color::Black,{255,255,255,255},2),{0,1},{{{-(int)Size.y/4,0,Size.y/2,Size.y/2},0},{{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0}});
    Visualizer.addPlugin(new Plugin::VolumeCircle(2,sf::Color::Black,{255,255,255,255},2),{0,.5},{{{-(int)Size.y/4,0,Size.y/2,Size.y/2},0},{{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0}});
    Visualizer.addPlugin(new Plugin::VolumeCircle(1,sf::Color::Black,{255,255,255,255},2),{0,.25},{{{-(int)Size.y/4,0,Size.y/2,Size.y/2},0},{{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0}});
    Visualizer.addPlugin(new Plugin::VolumeCircle(.5,sf::Color::Black,{255,255,255,255},2),{0,.125},{{{-(int)Size.y/4,0,Size.y/2,Size.y/2},0},{{Size.x/2-Size.y/4,0,Size.y/2,Size.y/2},0}});
    Visualizer.addPlugin(new Plugin::Amplitude(4,1280,1,{128,128,128},{128,128,128},.025*Size.y/2,0.05,1),{0,1},{{{0,.75*Size.y/2,Size.x/2,Size.y/8},0},{{0,0,Size.x/2,Size.y/8},180}});
    Visualizer.addPlugin(new Plugin::Hanning(512,sf::Color::White),{0,1},{{{0,Size.y/8,Size.x/2,Size.y/4},0}});
    Visualizer.addPlugin(new Plugin::Pitch(4096,sf::Color::White,sf::Color::Red,true,3),{0,1},{{{0,Size.y/4,Size.x/2,Size.y/4},0},{{0,0,Size.x/2,Size.y/4},180}});
    sw::WarpField Warp;
    Warp.setSize({Size.x,Size.y});
    Warp.setStarCount(20000);
    Warp.setColor(sf::Color::White);
    sw::Hexahedron Hexahedron({Size.y/2.f,Size.x/2.f,Size.x/2.f},2);
    Hexahedron.rotate({90,0,90});
    for(int i=0;i<4;i++)
        Hexahedron.setTexture(sw::Hexahedron::Face(i),sw::Hexahedron::Side::Outside,Visualizer.getTexture());
    Hexahedron.setPosition({Size.x/2,Size.y/2});
    sf::Clock Clock;
    Visualizer.Play();
    while(Window.isOpen()) {
	float Speed=Clock.restart().asSeconds()/0.016f;
        sf::Event Event;
        while(Window.pollEvent(Event)) {
            if(Event.type==sf::Event::Closed) Window.close();
        }
        Hexahedron.rotate({0.5*Speed,0,0});
        Warp.move(3);
        Window.clear();
        Window.draw(Warp);
        Visualizer.render();
        Hexahedron.draw(&Window);
        Window.display();
    }
}
