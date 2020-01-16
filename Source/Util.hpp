#ifndef Util_h
#define Util_h
#include <SFML/Graphics.hpp>
#include "../SelbaWard/SelbaWard.hpp"

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
}
#endif