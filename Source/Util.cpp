#include "Util.hpp"
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
}