#ifndef Plugins_h
#define Plugins_h
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <complex>
#include <valarray>
#include "Util.hpp"

struct VisualizerPluginInputSettings {
    VisualizerPluginInputSettings(int Channel,float Volume);
    int channel;
    float volume;
};
struct VisualizerPluginDisplaySettings {
    VisualizerPluginDisplaySettings(sf::FloatRect Position,float Rotation);
    sf::FloatRect position;
    float rotation;
};
class VisualizerPlugin {
    public:
    VisualizerPlugin();
    virtual void render(sf::Time ElapsedTime);
    virtual void setRenderSize(sf::Vector2u Size);
    virtual void setStreamProperties(int SampleRate,int ChannelCount);
    virtual void inputSamples(std::vector<sf::Int16>& Samples);

    const sf::Texture& getTexture();
    
    protected:
    sf::RenderTexture m_Texture;
    int m_SampleRate,m_ChannelCount;
};

namespace Plugin {
    class VolumeCircle : public VisualizerPlugin {
        public:
        VolumeCircle(float Speed,sf::Color FillColor,sf::Color OutlineColor=sf::Color::Transparent,int OutlineThickness=0);

        virtual void render(sf::Time ElapsedTime);
        virtual void setRenderSize(sf::Vector2u Size);
        virtual void inputSamples(std::vector<sf::Int16>& Samples);

        protected:
        sf::CircleShape m_Circle;
        float m_Speed;
        float m_TargetSize;
    };

    class ConfettiCircle {
        public:
        ConfettiCircle();
        ConfettiCircle(sf::Vector2f Position,int StartSize,int EndSize,int FadeTime,sf::Color FillColor,sf::Color OutlineColor,int OutlineThickness,int Sides,float GrowRate);
        void render(sf::Time ElapsedTime,sf::RenderTexture& Texture);
        bool Destruct;
        protected:
        sf::CircleShape m_Circle;
        float m_GrowRate,m_FadeTime,m_EndScale;
    };
    class ConfettiCircles : public VisualizerPlugin {
        public:
        ConfettiCircles(RandomSeed<int> StartSize,RandomSeed<int> EndSize,RandomSeed<float> FadeTime,RandomSeed<sf::Color> FillColor,RandomSeed<sf::Color> OutlineColor,RandomSeed<int> OutlineThickness,RandomSeed<int> Sides,RandomSeed<float> GrowRate,float AppearanceThreshhold,int Persistance);
        
        virtual void render(sf::Time ElapsedTime);
        virtual void setRenderSize(sf::Vector2u Size);
        virtual void inputSamples(std::vector<sf::Int16>& Samples);

        protected:
        std::vector<ConfettiCircle> m_Circles;
        RandomSeed<int> m_StartSize,m_EndSize,m_OutlineThickness,m_Sides;
        RandomSeed<float> m_GrowRate,m_FadeTime;
        RandomSeed<sf::Color> m_FillColor,m_OutlineColor;
        float m_AppearanceThreshhold,m_BaseThreshhold;
        int m_Persistance;
        bool m_UsingSamples;

        std::vector<sf::Int16> m_Samples;
        std::vector<float> m_Averages;

    };

    class Laser {
        public:
        Laser();
        Laser(float LifeTime,float FadeTime,sf::Vector2f Point1,sf::Vector2f Point2,int Size,float ScaleRate,sf::Color Color,sf::Texture& Texture);
        void render(sf::Time ElapsedTime,float TargetScale,sf::RenderTexture& Texture);
        bool Destruct;
        protected:
        sf::Sprite m_Sprite;
        sf::Time m_Life;
        float m_LifeTime,m_FadeTime,m_BaseScale,m_ScaleRate;
    };
    class Lasers : public VisualizerPlugin {
        public:
        Lasers(float AppearanceThreshhold,RandomSeed<float> LifeTime,RandomSeed<float> FadeTime,RandomSeed<int> MeanSize,RandomSeed<float> ScaleRate,RandomSeed<sf::Color> Color,int Persistance,std::string TexturePath);

        virtual void render(sf::Time ElapsedTime);
        virtual void setRenderSize(sf::Vector2u Size);
        virtual void inputSamples(std::vector<sf::Int16>& Samples);
        
        protected:
        std::vector<Laser> m_Lasers;
        RandomSeed<int> m_MeanSize;
        RandomSeed<float> m_LifeTime,m_FadeTime,m_ScaleRate;
        RandomSeed<sf::Color> m_Color;
        float m_AppearanceThreshhold,m_BaseThreshhold,m_TargetScale;
        int m_Persistance;
        bool m_UsingSamples;

        std::vector<sf::Int16> m_Samples;
        std::vector<float> m_Averages;

        sf::Texture m_LaserTexture;
    };

    class Amplitude : public VisualizerPlugin {
        public:
        Amplitude(float ScaleSpeed,int Rects,int SamplesPerRect,sf::Color BaseColor,sf::Color PeakColor,int BaseHeight,float RefreshSpeed,int Smoothing);

        virtual void render(sf::Time ElapsedTime);
        virtual void inputSamples(std::vector<sf::Int16>& Samples);
        virtual void setRenderSize(sf::Vector2u Size);

        protected:
        float m_ScaleSpeed,m_RefreshSpeed;
        int m_BaseHeight,m_Smoothing;
        bool m_UsingSamples;
        sf::Color m_BaseColor,m_PeakColor;
        sf::Clock m_RefreshClock;

        std::vector<float> m_RectSize,m_RectTarget;
        std::vector<sf::Int16> m_Samples;
        
        sf::VertexArray m_Rects;
    };
    
    class Hanning : public VisualizerPlugin {
        public:
        Hanning(int Width,sf::Color Color);

        virtual void render(sf::Time ElapsedTime);
        virtual void inputSamples(std::vector<sf::Int16>& Samples);

        protected:
        bool m_UsingSamples;
        int m_Width;
        sf::Color m_Color;

        std::vector<sf::Int16> m_Samples;
        std::vector<float> m_HanningCache;

        sf::VertexArray m_Line;
    };

    class Pitch : public VisualizerPlugin {
        public:
        Pitch(int Width,sf::Color BaseColor,sf::Color PeakColor,bool Logarithmic,float Logarithm);

        virtual void render(sf::Time ElapsedTime);
        virtual void inputSamples(std::vector<sf::Int16>& Samples);
        virtual void setRenderSize(sf::Vector2u Size);

        protected:
        bool m_Logarithmic,m_UsingSamples;
        int m_Width,m_FFTWidth;
        float m_Logarithm;
        sf::Color m_BaseColor,m_PeakColor;

        void FFT(std::complex<float>* Samples,int SampleCount,int SampleCountLogarithm);

        std::complex<float> m_Even,m_Temp;
        std::vector<sf::Int16> m_Samples;
        std::vector<float> m_HanningCache;
        std::vector<std::complex<float>> m_TempV,m_HanningSamples;
        std::vector<std::vector<std::complex<float>>> m_Polars;

        sf::VertexArray m_Bars;
    };

    
    
}

#endif
