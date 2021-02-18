#ifndef Util_h
#define Util_h
#include <random>
#include <functional>
#include <SFML/Graphics.hpp>
#include <Sprite3d.hpp>

//RandomSeeds
template<typename Type>
struct RandomSeed {
    RandomSeed(Type Fixed,Type Variable):fixed(Fixed),variable(Variable){};
    RandomSeed(Type& Copy):fixed(Copy.fixed),variable(Copy.variable){};
    RandomSeed();
    Type fixed,variable;
};

//SelbaWard Extentions
namespace selbaward {
    class Hexahedron {
        public:
        
        enum Face {
            Front=0,
            Back=1,
            Top=2,
            Bottom=3
        };
        enum Side { 
            Inside=0,
            Outside=1
        };

        Hexahedron(sf::Vector3f Size,int Depth);
        void setPosition(sf::Vector2f Position);
        void rotate(sf::Vector3f Rotation);
        void setRotation(sf::Vector3f Rotation);
        sf::Vector3f getRotation();
        void setTexture(Face Face,Side Side,const sf::Texture& Texture);
        void setTextureRect(Face Face,Side Side,sf::IntRect Rect);
        void setInsideTextureFlipped(Face Face,bool Flip);
        void setColor(Face Face,sf::Color Color);
        void draw(sf::RenderTarget* Target);

        protected:
        bool isInRange(float Number,float LowerBound,float UpperBound);

        sf::Vector3f m_Size;
        std::vector<sw::Sprite3d> m_Shapes;
    };
    class WarpField : public sf::Drawable, public sf::Transformable {
        public:
        WarpField();

        void reset();
        void setSize(sf::Vector2f Size);
        void setStarCount(unsigned int StarCount);

        void setColor(sf::Color Color);

        void move(float Distance);

        protected:
        inline float random(float Low,float High);
        virtual void draw(sf::RenderTarget& Target,sf::RenderStates States) const;

        std::vector<sf::Vector2f> m_StarMovementSpeed;
        std::vector<float> m_StarAlpha,m_StarMaxAlpha,m_StarMA;
        std::vector<bool> m_StarMovingRight,m_StarMovingDown;
        
        sf::Vector2f m_Size;
        sf::Color m_Color;

        std::vector<sf::Vertex> m_Vertices;

        std::mt19937 m_RandomGenerator;
        std::uniform_int_distribution<unsigned short> m_RandomDistribution;
        std::function <unsigned short int()> m_Random;
    };
}
#endif