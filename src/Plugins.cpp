#include <Plugins.hpp>

VisualizerPluginInputSettings::VisualizerPluginInputSettings(int Channel,float Volume):channel(Channel),volume(Volume){}
VisualizerPluginDisplaySettings::VisualizerPluginDisplaySettings(sf::FloatRect Position,float Rotation):position(Position),rotation(Rotation){}

VisualizerPlugin::VisualizerPlugin() {}
void VisualizerPlugin::render(sf::Time ElapsedTime) {}
void VisualizerPlugin::inputSamples(std::vector<sf::Int16>& Samples) {}
void VisualizerPlugin::setRenderSize(sf::Vector2u Size) { 
    sf::ContextSettings Settings;
    Settings.antialiasingLevel=8;
    m_Texture.create(Size.x,Size.y,Settings);
}
void VisualizerPlugin::setStreamProperties(int SampleRate,int ChannelCount) { m_SampleRate=SampleRate,m_ChannelCount=ChannelCount; }
const sf::Texture& VisualizerPlugin::getTexture() { return m_Texture.getTexture(); }



namespace Plugin {

    VolumeCircle::VolumeCircle(float Speed,sf::Color FillColor,sf::Color OutlineColor,int OutlineThickness):m_Speed(Speed),m_TargetSize(0) {
        m_Circle.setPointCount(99);
        m_Circle.setFillColor(FillColor);
        m_Circle.setOutlineColor(OutlineColor);
        m_Circle.setOutlineThickness(OutlineThickness);
        m_Circle.setRadius(0);
    }
    void VolumeCircle::render(sf::Time ElapsedTime) {
        m_Texture.clear(sf::Color::Transparent);
        m_Circle.setRadius(m_Circle.getRadius()-(m_Circle.getRadius()-m_TargetSize)*m_Speed*ElapsedTime.asSeconds());
        m_Circle.setOrigin(m_Circle.getRadius(),m_Circle.getRadius());
        m_Texture.draw(m_Circle);
        m_Texture.display();
    }
    void VolumeCircle::setRenderSize(sf::Vector2u Size) {
        VisualizerPlugin::setRenderSize(Size);
        m_Circle.setPosition(Size.x/2,Size.y/2);
    }
    void VolumeCircle::inputSamples(std::vector<sf::Int16>& Samples) {
        float Avg=0,Multiplier=(float)std::min(m_Texture.getSize().x,m_Texture.getSize().y)/(float)(32768.f*Samples.size());
        for(int i=0;i<Samples.size();i++) Avg+=abs(Samples[i])*Multiplier;
        m_TargetSize=Avg;
    }

    ConfettiCircle::ConfettiCircle(){}
    ConfettiCircle::ConfettiCircle(sf::Vector2f Position,int StartSize,int EndSize,int FadeTime,sf::Color FillColor,sf::Color OutlineColor,int OutlineThickness,int Sides,float GrowRate):
        Destruct(false),m_EndScale(EndSize/StartSize),m_GrowRate(GrowRate),m_FadeTime(FadeTime) {
        m_Circle.setPointCount(Sides);
        m_Circle.setRadius(StartSize);
        m_Circle.setPosition(Position);
        m_Circle.setOrigin(StartSize,StartSize);
        m_Circle.setScale(1.02,1.02);
        m_Circle.setFillColor(FillColor);
        m_Circle.setOutlineColor(OutlineColor);
        m_Circle.setOutlineThickness(OutlineThickness);
    }
    void ConfettiCircle::render(sf::Time ElapsedTime,sf::RenderTexture& Texture) {
        if(m_Circle.getScale().x<=m_EndScale-.1&&
           (m_Circle.getGlobalBounds().left<0||m_Circle.getGlobalBounds().top<0||m_Circle.getGlobalBounds().left+m_Circle.getGlobalBounds().width>Texture.getSize().x||m_Circle.getGlobalBounds().top+m_Circle.getGlobalBounds().height>Texture.getSize().y)==false)
            m_Circle.setScale(m_Circle.getScale().x-(m_Circle.getScale().x-m_EndScale)*m_GrowRate*ElapsedTime.asSeconds(),
                              m_Circle.getScale().x-(m_Circle.getScale().x-m_EndScale)*m_GrowRate*ElapsedTime.asSeconds());
        else if(m_Circle.getFillColor().a>(255.f/m_FadeTime)*ElapsedTime.asSeconds())
            m_Circle.setFillColor({m_Circle.getFillColor().r,
                                   m_Circle.getFillColor().g,
                                   m_Circle.getFillColor().b,
                                   m_Circle.getFillColor().a-(255.f/m_FadeTime)*ElapsedTime.asSeconds()});
        else
            Destruct=true;
        Texture.draw(m_Circle);
    }

    ConfettiCircles::ConfettiCircles(RandomSeed<int> StartSize,RandomSeed<int> EndSize,RandomSeed<float> FadeTime,RandomSeed<sf::Color> FillColor,RandomSeed<sf::Color> OutlineColor,RandomSeed<int> OutlineThickness,RandomSeed<int> Sides,RandomSeed<float> GrowRate,float AppearanceThreshhold,int Persistance):
        m_StartSize(StartSize),m_EndSize(EndSize),m_FadeTime(FadeTime),m_FillColor(FillColor),m_OutlineColor(OutlineColor),m_OutlineThickness(OutlineThickness),m_Sides(Sides),m_GrowRate(GrowRate),m_AppearanceThreshhold(AppearanceThreshhold),m_BaseThreshhold(AppearanceThreshhold),m_Persistance(Persistance),m_UsingSamples(false) {
        
    }
    void ConfettiCircles::render(sf::Time ElapsedTime) {
        float LocalAverage=0,ChunkSize=ElapsedTime.asSeconds()*m_SampleRate,GlobalAverage=0;
        while(m_UsingSamples) {sf::sleep(sf::seconds(.001));}
        m_UsingSamples=true;
        if(m_Samples.size()<ChunkSize) m_Samples.resize(ChunkSize,0);
        for(int i=0;i<ChunkSize;i++) LocalAverage+=abs((float)m_Samples[i]/ChunkSize);
        m_Averages.push_back(LocalAverage);
        for(int i=0;i<m_Averages.size();i++) GlobalAverage+=m_Averages[i]/m_Averages.size();
        if(LocalAverage>GlobalAverage*m_AppearanceThreshhold) m_Circles.push_back({{rand()%m_Texture.getSize().x+1,rand()%(m_Texture.getSize().y+1)},
                                                                                   m_StartSize.fixed+rand()%(m_StartSize.variable+1),
                                                                                   m_EndSize.fixed+rand()%(m_EndSize.variable+1),
                                                                                   m_FadeTime.fixed+((float)rand()/(float)RAND_MAX)*m_FadeTime.variable+1,
                                                                                   {m_FillColor.fixed.r+rand()%(m_FillColor.variable.r+1),m_FillColor.fixed.b+rand()%(m_FillColor.variable.b+1),m_FillColor.fixed.g+rand()%(m_FillColor.variable.g+1),m_FillColor.fixed.a+rand()%(m_FillColor.variable.a+1)},
                                                                                   {m_OutlineColor.fixed.r+rand()%(m_OutlineColor.variable.r+1),m_OutlineColor.fixed.b+rand()%(m_OutlineColor.variable.b+1),m_OutlineColor.fixed.g+rand()%(m_OutlineColor.variable.g+1),m_OutlineColor.fixed.a+rand()%(m_OutlineColor.variable.a+1)},
                                                                                   m_OutlineThickness.fixed+rand()%(m_OutlineThickness.variable+1),
                                                                                   m_Sides.fixed+rand()%(m_Sides.variable+1),
                                                                                   m_GrowRate.fixed+((float)rand()/(float)RAND_MAX)*m_GrowRate.variable+1}),
                                                              m_AppearanceThreshhold*=1.1;
        else m_AppearanceThreshhold-=(m_AppearanceThreshhold-m_BaseThreshhold)*ElapsedTime.asSeconds();
        if(m_Averages.size()>m_Persistance) m_Averages.erase(m_Averages.begin(),m_Averages.begin()+(m_Averages.size()-m_Persistance));
        m_Samples.erase(m_Samples.begin(),m_Samples.begin()+ChunkSize);
        m_UsingSamples=false;
        m_Texture.clear(sf::Color::Transparent);
        for(int i=0;i<m_Circles.size();i++) {
            if(m_Circles[i].Destruct) m_Circles.erase(m_Circles.begin()+i), i--;
            else m_Circles[i].render(ElapsedTime,m_Texture);
        }
        m_Texture.display();
    }
    void ConfettiCircles::setRenderSize(sf::Vector2u Size) {
        VisualizerPlugin::setRenderSize(Size);
        m_Circles.clear();
    }
    void ConfettiCircles::inputSamples(std::vector<sf::Int16>& Samples) {
        while(m_UsingSamples) {sf::sleep(sf::seconds(.001));}
        m_UsingSamples=true;
        m_Samples.insert(m_Samples.end(),Samples.begin(),Samples.end());
        m_UsingSamples=false;
    }

    Laser::Laser(){}
    Laser::Laser(float LifeTime,float FadeTime,sf::Vector2f Point1,sf::Vector2f Point2,int Size,float ScaleRate,sf::Color Color,sf::Texture& Texture):
        Destruct(false),m_Life(sf::Time::Zero),m_LifeTime(LifeTime),m_FadeTime(FadeTime),m_ScaleRate(ScaleRate),m_BaseScale((float)Size/(float)Texture.getSize().y) {
        m_Sprite.setTexture(Texture);
        m_Sprite.setTextureRect({0,0,sqrtf(std::pow(Point1.x-Point2.x,2)+std::pow(Point1.y-Point2.y,2))+500,Texture.getSize().y});
        m_Sprite.setRotation(atan2(Point2.y-Point1.y,Point2.x-Point1.x)*180/M_PI);
        m_Sprite.scale(1,m_BaseScale);
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width/2,Texture.getSize().y/2);
        m_Sprite.setPosition(Point1.x-(Point1.x-Point2.x)/2,Point1.y-(Point1.y-Point2.y)/2);
        m_Sprite.setColor(Color);
    }
    void Laser::render(sf::Time ElapsedTime,float TargetScale,sf::RenderTexture& Texture) {
        if(m_Life<sf::seconds(m_LifeTime)) m_Life+=ElapsedTime;
        else if(m_Sprite.getColor().a>255.f/m_FadeTime*ElapsedTime.asSeconds())
            m_Sprite.setColor({m_Sprite.getColor().r,m_Sprite.getColor().g,m_Sprite.getColor().b,m_Sprite.getColor().a-255.f/m_FadeTime*ElapsedTime.asSeconds()});
        else Destruct=true;
        m_Sprite.setScale(1,m_Sprite.getScale().y-(m_Sprite.getScale().y-TargetScale*m_BaseScale)*m_ScaleRate*ElapsedTime.asSeconds());
        Texture.draw(m_Sprite);
    }
    
    Lasers::Lasers(float AppearanceThreshhold,RandomSeed<float> LifeTime,RandomSeed<float> FadeTime,RandomSeed<int> MeanSize,RandomSeed<float> ScaleRate,RandomSeed<sf::Color> Color,int Persistance,std::string TexturePath):
        m_AppearanceThreshhold(AppearanceThreshhold),m_BaseThreshhold(AppearanceThreshhold),m_LifeTime(LifeTime),m_FadeTime(FadeTime),m_MeanSize(MeanSize),m_ScaleRate(ScaleRate),m_Color(Color),m_Persistance(Persistance),m_UsingSamples(false),m_TargetScale(1) {
        m_LaserTexture.loadFromFile(TexturePath);
        m_LaserTexture.setRepeated(true);
        m_LaserTexture.setSmooth(true);
    }
    void Lasers::render(sf::Time ElapsedTime) {
        float LocalAverage=0,ChunkSize=ElapsedTime.asSeconds()*m_SampleRate,GlobalAverage=0;
        while(m_UsingSamples) {sf::sleep(sf::seconds(.001));}
        m_UsingSamples=true;
        if(m_Samples.size()<ChunkSize) m_Samples.resize(ChunkSize,0);
        for(int i=0;i<ChunkSize;i++) LocalAverage+=abs((float)m_Samples[i]/ChunkSize);
        m_Averages.push_back(LocalAverage);
        for(int i=0;i<m_Averages.size();i++) GlobalAverage+=m_Averages[i]/m_Averages.size();
        if(LocalAverage>GlobalAverage*m_AppearanceThreshhold) {
            int Side1=rand()%4,Side2=rand()%4;
            Side2=(Side2==Side1)?(Side2+1)%4:Side2;
            m_Lasers.push_back({m_LifeTime.fixed+((float)rand()/(float)RAND_MAX)*m_LifeTime.variable+1,
                                m_FadeTime.fixed+((float)rand()/(float)RAND_MAX)*m_FadeTime.variable+1,
                                {(Side1<2)?rand()%m_Texture.getSize().x:((Side1==2)?m_Texture.getSize().x:0),(Side1<2)?((Side1==1)?m_Texture.getSize().y:0):rand()%m_Texture.getSize().y},
                                {(Side2<2)?rand()%m_Texture.getSize().x:((Side2==2)?m_Texture.getSize().x:0),(Side2<2)?((Side2==1)?m_Texture.getSize().y:0):rand()%m_Texture.getSize().y},
                                m_MeanSize.fixed+rand()%(m_MeanSize.variable+1),
                                m_ScaleRate.fixed+((float)rand()/(float)RAND_MAX)*m_ScaleRate.variable+1,
                                {m_Color.fixed.r+rand()%(m_Color.variable.r+1),m_Color.fixed.g+rand()%(m_Color.variable.g+1),m_Color.fixed.b+rand()%(m_Color.variable.b+1),m_Color.fixed.a+rand()%(m_Color.variable.a+1)},
                                m_LaserTexture});
            m_AppearanceThreshhold*=1.1;
        }
        else m_AppearanceThreshhold-=(m_AppearanceThreshhold-m_BaseThreshhold)*ElapsedTime.asSeconds();
        if(m_Averages.size()>m_Persistance) m_Averages.erase(m_Averages.begin(),m_Averages.begin()+(m_Averages.size()-m_Persistance));
        m_Samples.erase(m_Samples.begin(),m_Samples.begin()+ChunkSize);
        m_UsingSamples=false;
        m_Texture.clear(sf::Color::Transparent);
        for(int i=0;i<m_Lasers.size();i++) {
            if(m_Lasers[i].Destruct) m_Lasers.erase(m_Lasers.begin()+i), i--;
            else m_Lasers[i].render(ElapsedTime,m_TargetScale,m_Texture);
        }
        m_Texture.display();
    }
    void Lasers::setRenderSize(sf::Vector2u Size) {
        VisualizerPlugin::setRenderSize(Size);
        m_Lasers.clear();
    }
    void Lasers::inputSamples(std::vector<sf::Int16>& Samples) {
        while(m_UsingSamples) sf::sleep(sf::seconds(.001));
        m_UsingSamples=true;
        m_Samples.insert(m_Samples.end(),Samples.begin(),Samples.end());
        float Avg=0;
        for(int i=0;i<Samples.size();i++) Avg+=(float)(abs(Samples[i])/(float)(1024.f*Samples.size()));
        m_TargetScale=Avg;
        m_UsingSamples=false;
    }

    Amplitude::Amplitude(float ScaleSpeed,int Rects,int SamplesPerRect,sf::Color BaseColor,sf::Color PeakColor,int BaseHeight,float RefreshSpeed,int Smoothing):
        m_ScaleSpeed(ScaleSpeed),m_BaseColor(BaseColor),m_PeakColor(PeakColor),m_BaseHeight(BaseHeight),m_RefreshSpeed(RefreshSpeed),m_Smoothing(Smoothing),m_UsingSamples(false) {
        m_RefreshClock.restart();
        m_RectTarget.resize(Rects);
        m_RectSize.resize(Rects);
        m_Rects.setPrimitiveType(sf::Lines);
        m_Samples.resize(Rects*SamplesPerRect,0);
    }
    void Amplitude::setRenderSize(sf::Vector2u Size) {
        VisualizerPlugin::setRenderSize(Size);
        m_Rects.resize(m_Texture.getSize().x*2);
        for(int i=0;i<m_Texture.getSize().x;i++)
            m_Rects[i*2]={{i+.5f,m_Texture.getSize().y+.5f},m_BaseColor};
    }
    void Amplitude::render(sf::Time ElapsedTime) {
        if(m_RefreshClock.getElapsedTime().asSeconds()>m_RefreshSpeed) {
            while(m_UsingSamples) sf::sleep(sf::seconds(0.001));
            m_UsingSamples=true;
            m_RefreshClock.restart();
            float ChunkSize=m_Samples.size()/m_RectTarget.size();
            for(int i=0;i<m_RectTarget.size();i++) {
                float Avg=0;
                for(int i2=i*ChunkSize;i2<(i+1)*ChunkSize;i2++) Avg+=(float)abs(m_Samples[i2])/ChunkSize;
                m_RectTarget[i]=(Avg/32768.f)*m_Texture.getSize().y;
            }
            m_UsingSamples=false;
        }
        for(int i=0;i<m_RectTarget.size();i++) m_RectSize[i]-=(m_RectSize[i]-m_RectTarget[i])*m_ScaleSpeed*ElapsedTime.asSeconds();
        for(int i=0;i<m_Texture.getSize().x;i++) {
            int Size=0;
            for(int i2=-m_Smoothing;i2<=m_Smoothing;i2++) Size+=m_RectSize[std::max(0.f,std::min(m_RectSize.size()-1.f,(float)(i*m_RectSize.size())/(float)m_Texture.getSize().x+i2))]/(m_Smoothing*2+1);
            sf::Vector2f Base={i+.5f,m_Texture.getSize().y+.5f},Peak={i+.5f,m_Texture.getSize().y-m_BaseHeight-Size+.5f};
            float ColorFade=1-(Peak.y/m_Texture.getSize().y);
            m_Rects[i*2+1]={Peak,{m_BaseColor.r-(m_BaseColor.r-m_PeakColor.r)*ColorFade,
                                  m_BaseColor.g-(m_BaseColor.g-m_PeakColor.g)*ColorFade,
                                  m_BaseColor.b-(m_BaseColor.b-m_PeakColor.b)*ColorFade,
                                  m_BaseColor.a-(m_BaseColor.a-m_PeakColor.a)*ColorFade}};
        }
        m_Texture.clear(sf::Color::Transparent);
        m_Texture.draw(m_Rects);
        m_Texture.display();
    }
    void Amplitude::inputSamples(std::vector<sf::Int16>& Samples) {
        while(m_UsingSamples) sf::sleep(sf::seconds(0.001));
        m_UsingSamples=true;
        std::size_t Size=m_Samples.size();
        m_Samples.insert(m_Samples.end(),Samples.begin(),Samples.end());
        m_Samples.erase(m_Samples.begin(),m_Samples.begin()+Samples.size());
        m_UsingSamples=false;
    }

    Hanning::Hanning(int Width,sf::Color Color):
        m_Color(Color),m_Width(Width),m_UsingSamples(false) {
        m_Line.setPrimitiveType(sf::LineStrip);
        m_Line.resize(m_Width);
        m_HanningCache.resize(m_Width);
        for(int i=0;i<m_Width;i++)
            m_HanningCache[i]=(0.54f-0.46f*cos(2*M_PI*i/(float)m_Width));
    }
    void Hanning::render(sf::Time ElapsedTime) {
        while(m_UsingSamples) sf::sleep(sf::seconds(0.001));
        m_UsingSamples=true;
        if(m_Samples.size()<m_Width) m_Samples.resize(m_Width,0);
        for(int i=0;i<m_Width;i++) m_Line[i]={{(float)(i*m_Texture.getSize().x)/(float)m_Width+.5f,(m_Texture.getSize().y/2)+(float)(m_Samples[i]*m_HanningCache[i]*m_Texture.getSize().y/2)/32768.f+.5f},m_Color};
        m_Samples.erase(m_Samples.begin(),m_Samples.begin()+std::min(m_SampleRate*ElapsedTime.asSeconds()+10,(float)m_Samples.size()));
        m_UsingSamples=false;
        m_Texture.clear(sf::Color::Transparent);
        m_Texture.draw(m_Line);
        m_Texture.display();
    }
    void Hanning::inputSamples(std::vector<sf::Int16>& Samples) {
        while(m_UsingSamples) sf::sleep(sf::seconds(.001));
        m_UsingSamples=true;
        m_Samples.insert(m_Samples.end(),Samples.begin(),Samples.end());
        m_UsingSamples=false;
    }

    Pitch::Pitch(int Width,sf::Color BaseColor,sf::Color PeakColor,bool Logarithmic,float Logarithm):
        m_Logarithmic(Logarithmic),m_Logarithm(Logarithm),m_Width(Width),m_BaseColor(BaseColor),m_PeakColor(PeakColor),m_UsingSamples(false) {
        m_Bars.setPrimitiveType(sf::Lines);
        m_HanningSamples.resize(m_Width);
        m_TempV.resize(m_Width/2);
        m_Polars.resize(log2(m_Width),std::vector<std::complex<float>>(m_Width/2));
        for(int i=0;i<log2(m_Width);i++)
            for(int i2=0;i2<m_Width/2;i2++)
                m_Polars[i][i2]=(exp(std::complex<float>(0,-2*M_PI/(pow(2,i+1))*i2)));
        m_HanningCache.resize(m_Width);
        for(int i=0;i<m_Width;i++)
            m_HanningCache[i]=(0.54f-0.46f*cos(2*M_PI*i/(float)m_Width));
    }
    void Pitch::setRenderSize(sf::Vector2u Size) {
        VisualizerPlugin::setRenderSize(Size);
        m_Bars.resize(m_Texture.getSize().x*2);
        for(int i=0;i<m_Texture.getSize().x;i++)
            m_Bars[i*2]={{i+.5f,m_Texture.getSize().y-.5f},m_BaseColor};
    }
    void Pitch::FFT(std::complex<float>* Samples,int SampleCount,int SampleCountLogarithm) {
        if(SampleCount>1) {
            SampleCount/=2;
            for(int i=0;i<SampleCount;i++) m_TempV[i]=Samples[i*2+1];
            for(int i=0;i<SampleCount;i++) Samples[i]=Samples[i*2];
            for(int i=0;i<SampleCount;i++) Samples[i+SampleCount]=m_TempV[i];
            FFT(&Samples[0],SampleCount,SampleCountLogarithm-1);
            FFT(&Samples[0]+SampleCount,SampleCount,SampleCountLogarithm-1);
            for(int i=0;i<SampleCount;i++) {
                m_Even=Samples[i];
                m_Temp=m_Polars[SampleCountLogarithm-1][i]*Samples[i+SampleCount];
                Samples[i]+=m_Temp;
                Samples[i+SampleCount]=m_Even-m_Temp;
            }
        }
    }
    void Pitch::render(sf::Time ElapsedTime) {
        //Hanning Window
        while(m_UsingSamples) sf::sleep(sf::seconds(.001));
        m_UsingSamples=true;
        if(m_Samples.size()<m_Width) m_Samples.resize(m_Width,0);
        for(int i=0;i<m_Width;i++) m_HanningSamples[i]={m_Samples[i]*m_HanningCache[i],0};
        m_Samples.erase(m_Samples.begin(),m_Samples.begin()+std::min(m_SampleRate*ElapsedTime.asSeconds()+10,(float)m_Samples.size()));
        m_UsingSamples=false;
        //FFT
        FFT(m_HanningSamples.data(),m_Width,log2(m_Width));
        for(int i=0;i<m_HanningSamples.size();i++)
            m_HanningSamples[i]*=log2(i)*1.709511291351455d;
        
        if(m_Logarithmic) {
            float Height;
            for(float i=0;i<m_Texture.getSize().x;i++) {
                Height=2*pow(abs(m_HanningSamples[(int)(m_HanningSamples.size()/2.f*pow(2,1+i*6/m_Texture.getSize().x)/1024.f)])/160000000.f,2);
                m_Bars[i*2+1]={{i+.5f,m_Texture.getSize().y-(Height*m_Texture.getSize().y+.5f)},
                               {m_BaseColor.r-(float)(m_BaseColor.r-m_PeakColor.r)*Height,
                                m_BaseColor.g-(float)(m_BaseColor.g-m_PeakColor.g)*Height,
                                m_BaseColor.b-(float)(m_BaseColor.b-m_PeakColor.b)*Height,
                                m_BaseColor.a-(float)(m_BaseColor.a-m_PeakColor.a)*Height}};
            }
        } else std::cout<<"Not supported yet"<<std::endl;
        m_Texture.clear(sf::Color::Transparent);
        m_Texture.draw(m_Bars);
        m_Texture.display();
    }
    void Pitch::inputSamples(std::vector<sf::Int16>& Samples) {
        while(m_UsingSamples) sf::sleep(sf::seconds(.001));
        m_UsingSamples=true;
        m_Samples.insert(m_Samples.end(),Samples.begin(),Samples.end());
        m_UsingSamples=false;
    }
}
