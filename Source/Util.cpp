#include "Util.hpp"
#include <iostream>
namespace selbaward {
    Hexahedron::Hexahedron(sf::Vector3f Size,int Depth):m_Size(Size) {
        m_Shapes.resize(4);
        for(int i=0;i<m_Shapes.size();i++) {
            m_Shapes[i].setOrigin3d(Size/2.f);
            m_Shapes[i].setDepth(Depth);
        }
        setRotation({0,0,0});
    }
    void Hexahedron::setPosition(sf::Vector2f Position) {
        for(int i=0;i<m_Shapes.size();i++) m_Shapes[i].setPosition(Position);
    }
    void Hexahedron::rotate(sf::Vector3f Rotation) {
        for(int i=0;i<m_Shapes.size();i++) m_Shapes[i].setRotation3d(m_Shapes[i].getRotation3d()+Rotation);
    }
    void Hexahedron::setRotation(sf::Vector3f Rotation) {
        m_Shapes[0].setRotation3d({0,0,0});
        m_Shapes[1].setRotation3d({180,0,0});
        m_Shapes[2].setRotation3d({90,0,0});
        m_Shapes[3].setRotation3d({-90,0,0});
        rotate(Rotation);
    }
    sf::Vector3f Hexahedron::getRotation() { return m_Shapes[0].getRotation3d(); }
    void Hexahedron::setTexture(Face Face,Side Side,const sf::Texture& Texture) {
        if(Side==Outside) m_Shapes[Face].setTexture(Texture);
        else m_Shapes[Face].setBackTexture(Texture);
    }
    void Hexahedron::setTextureRect(Face Face,Side Side,sf::IntRect Rect) {
        if(Side==Outside) m_Shapes[Face].setTextureRect({Rect.left,Rect.top,m_Size.x,Side<2?m_Size.y:m_Size.z});
        else m_Shapes[Face].setBackTextureOffset({Rect.left,Rect.top});
    }
    void Hexahedron::setInsideTextureFlipped(Face Face,bool Flip) {
        m_Shapes[Face].setFlipBack(Flip);
    }
    void Hexahedron::setColor(Face Face,sf::Color Color) {
        m_Shapes[Face].setColor(Color);
    }
    void Hexahedron::draw(sf::RenderTarget* Target) {
        sf::Vector3f Rotation[4];
        for(int i=0;i<4;i++) Rotation[i]=m_Shapes[i].getRotation3d();
        for(int i=0;i<4;i++)
                if(((isInRange(Rotation[i].x,90,-90)?1:0)+(isInRange(Rotation[i].y,90,-90)?1:0))%2==1)
                    Target->draw(m_Shapes[i]);
        if((Rotation[0].y<45&&Rotation[0].y>-45))
        {
            for(int i=0;i<4;i++)
                if(((((Rotation[i].x<90&&Rotation[i].x>45)||(Rotation[i].x<-45&&Rotation[i].x>-90))?1:0)+(((Rotation[i].y<90&&Rotation[i].y>45)||(Rotation[i].y<-45&&Rotation[i].y>-90))?1:0))%2==1)
                    Target->draw(m_Shapes[i]);
            for(int i=0;i<4;i++)
                if((((Rotation[i].x<=45&&Rotation[i].x>=-45)?1:0)+((Rotation[i].y<=45&&Rotation[i].y>=-45)?1:0))%2==0)
                    Target->draw(m_Shapes[i]);
        }
        else if(Rotation[0].y>135||Rotation[0].y<-135)
        {
            for(int i=0;i<4;i++)
                if(((((Rotation[i].x>90&&Rotation[i].x<135)||(Rotation[i].x<-90&&Rotation[i].x>-135))?1:0)+(((Rotation[i].y<135&&Rotation[i].y>90)||(Rotation[i].y<-90&&Rotation[i].y>-135))?1:0))%2==1)
                    Target->draw(m_Shapes[i]);
            for(int i=0;i<4;i++)
                if((((Rotation[i].x<=-135||Rotation[i].x>=135)?1:0)+((Rotation[i].y<=-135||Rotation[i].y>=135)?1:0))%2==0)
                    Target->draw(m_Shapes[i]);
        }
        else
            for(int i=0;i<4;i++)
                if((((Rotation[i].x<=90&&Rotation[i].x>=-90)?1:0)+((Rotation[i].y<=90&&Rotation[i].y>=-90)?1:0))%2==0)
                    Target->draw(m_Shapes[i]);
    }
    bool Hexahedron::isInRange(float Number,float UpperBound,float LowerBound) {
        return LowerBound<Number&&Number<UpperBound;
    }

    WarpField::WarpField():
        m_RandomDistribution(1,255) {
        std::random_device Device;
        m_RandomGenerator.seed(Device());
        m_Random=std::bind(m_RandomDistribution,m_RandomGenerator);
    }
    void WarpField::move(float Distance) {
        float FadeAmount=1+(0.003*Distance);
        std::size_t StarCount=m_Vertices.size();
        for(int i=0;i<StarCount;i++) {
            if(m_StarAlpha[i]<m_StarMaxAlpha[i])
                m_StarAlpha[i]*=FadeAmount,
                m_Vertices[i].color.a=m_StarAlpha[i],
                m_StarMA[i]=m_Vertices[i].color.a*Distance;
            m_Vertices[i].position.x+=(m_StarMovingRight[i]?-m_StarMA[i]:m_StarMA[i])*m_StarMovementSpeed[i].x;
            m_Vertices[i].position.y+=(m_StarMovingDown[i]?-m_StarMA[i]:m_StarMA[i])*m_StarMovementSpeed[i].y;
            m_StarMovementSpeed[i]*=1.003f;
            if(m_Vertices[i].position.x<0||m_Vertices[i].position.x>m_Size.x||m_Vertices[i].position.y<0||m_Vertices[i].position.y>m_Size.y)
                m_StarMovementSpeed[i].x=random(0.00004,0.006),
                m_StarMovementSpeed[i].y=0.006-m_StarMovementSpeed[i].x,
                m_Vertices[i].position.x=m_Size.x/2+random(-1,1),
                m_Vertices[i].position.y=m_Size.y/2+random(-1,1),
                m_Vertices[i].color.a=0,
                m_StarMovingRight[i]=m_Vertices[i].position.x<m_Size.x/2,
                m_StarMovingDown[i]=m_Vertices[i].position.y<m_Size.y/2,
                m_StarAlpha[i]=2.55,
                m_StarMaxAlpha[i]=250;
        }
    }
    void WarpField::reset() {
        for(int i=0;i<m_Vertices.size();i++) {
            m_Vertices[i].position={random(0.f,m_Size.x),random(0.f,m_Size.y)};
            m_Vertices[i].color=m_Color;
            float MoveEffect=pow(1.002,sqrt(pow(m_Vertices[i].position.x-m_Size.x/2,2)+pow(m_Vertices[i].position.y-m_Size.y/2,2)));
            m_StarMovementSpeed[i].x=random(0.00004,0.006);
            m_StarMovementSpeed[i].y=0.006-m_StarMovementSpeed[i].x;
            m_StarMovementSpeed[i]*=MoveEffect;
            m_StarMaxAlpha[i]=250;
            m_StarAlpha[i]=std::min(2.55f*MoveEffect,m_StarMaxAlpha[i]);
            m_Vertices[i].color.a=m_StarAlpha[i];
            m_StarMovingRight[i]=m_Vertices[i].position.x<m_Size.x/2;
            m_StarMovingDown[i]=m_Vertices[i].position.y<m_Size.y/2;
        }
        for(int i=0;i<500;i++)
            move(3);
    }
    void WarpField::setSize(sf::Vector2f Size) {
        m_Size=Size;
        reset();
    }
    void WarpField::setStarCount(unsigned StarCount) {
        m_Vertices.resize(StarCount);
        m_StarMovementSpeed.resize(StarCount);
        m_StarAlpha.resize(StarCount);
        m_StarMaxAlpha.resize(StarCount);
        m_StarMovingRight.resize(StarCount);
        m_StarMovingDown.resize(StarCount);
        m_StarMA.resize(StarCount,100);
        reset();
    }
    void WarpField::setColor(sf::Color Color) {
        m_Color=Color;
        for(auto& Vertex:m_Vertices)
            Vertex.color={Color.r,Color.g,Color.b,Vertex.color.a};
    }
    void WarpField::draw(sf::RenderTarget& Target,sf::RenderStates States) const {
        States.transform*=getTransform();
        if(m_Vertices.size()>0) Target.draw(&m_Vertices.front(),m_Vertices.size(),sf::Points,States);
    }
    float WarpField::random(float Low,float High) {
        return std::uniform_real_distribution<float>{Low,High}(m_RandomGenerator);
    }
}