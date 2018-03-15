#ifndef MK_SPRITE_H
#define MK_SPRITE_H

// Include Cocos
#include "cocos2d.h"

// Include STL
#include <string>
#include <cmath>

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKMathsHelper.h"
#include "../Common/MKAssertions.h"

USING_NS_CC;
using namespace std;

NS_MK_BEGIN

class MKSprite : public cocos2d::Sprite
{
    typedef cocos2d::Sprite Super;

public:
    enum TextureWrapMethod
    {
        REPEAT,
        CLAMP_TO_EDGE,
    };

protected:
    GLProgram* m_GLProgram = nullptr;
    GLProgramState* m_GLProgramState = nullptr;

    const TextureWrapMethod m_TextureWrapMethod;
	Vec2 m_TextureOffset;
    Vec2 m_TextureScale;
    mkF32 m_TextureRotation;

    // Internal Function(s)
    void InitGLProgram(const mkString& _vertexShader, const mkString& _fragmentShader);
	void InitTextureParameters();

public:
    // Static Variable(s)
    static const mkString DEFAULT_VERTEX_SHADER;
    static const mkString DEFAULT_FRAGMENT_SHADER;

    // Static Function(s)
    static MKSprite* Create(const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);
    // If _textureWrapMethod is REPEAT, the size MUST be a power of 2. I don't set the rules, it's OpenGL and Cocos' dumb systems.
    // Cocos stupidly making the texture size the object size is the most infuriating thing.
	static MKSprite* CreateWithFile(const mkString& _fileName, TextureWrapMethod _textureWrapMethod = CLAMP_TO_EDGE, const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);
	static MKSprite* CreateWithTexture(Texture2D *texture, TextureWrapMethod _textureWrapMethod = CLAMP_TO_EDGE, const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);

    // Interface Function(s)
    TextureWrapMethod GetTextureWrapMethod() const
	{
		return m_TextureWrapMethod;
	}

	// The below functions only work if m_TextureRepeat is true.
	Vec2 GetTextureOffset() const
	{
		return m_TextureOffset;
	}

    void SetTextureOffset(const cocos2d::Vec2& _textureOffset)
    {
        SetTextureOffset(_textureOffset.x, _textureOffset.y);
    }

    void SetTextureOffset(mkF32 _x, mkF32 _y)
    {
        m_TextureOffset.x = _x;
        m_TextureOffset.y = _y;

        m_GLProgramState->setUniformVec2("u_textureOffset", m_TextureOffset);
        m_GLProgram->updateUniforms();
	}

	void OffsetTexture(mkF32 _x, mkF32 _y)
	{
		SetTextureOffset(m_TextureOffset.x + _x, m_TextureOffset.y + _y);
	}

    Vec2 GetTextureScale() const
    {
        return m_TextureScale;
    }

    void SetTextureScale(const cocos2d::Vec2& _textureScale)
    {
        SetTextureScale(_textureScale.x, _textureScale.y);
    }

    void SetTextureScale(mkF32 _u, mkF32 _v)
    {
        m_TextureScale.x = _u;
        m_TextureScale.y = _v;

        m_GLProgramState->setUniformVec2("u_textureScale", m_TextureScale);
        m_GLProgram->updateUniforms();
    }

    void ScaleTexture(mkF32 _u, mkF32 _v)
    {
        SetTextureScale(m_TextureScale.x + _u, m_TextureScale.y + _v);
    }

    mkF32 GetTextureRotation() const
    {
        return m_TextureRotation;
    }

    void SetTextureRotation(mkF32 _degrees)
    {
        m_TextureRotation = _degrees;

        mkF32 rotationRadians = m_TextureRotation * MKMathsHelper::Deg2Rad;
        m_GLProgramState->setUniformVec2("u_textureRotationCosSin", Vec2(cos(rotationRadians), sin(rotationRadians)));
        m_GLProgram->updateUniforms();
    }

    void RotateTexture(mkF32 _degrees)
    {
        SetTextureRotation(m_TextureRotation + _degrees);
    }

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    MKSprite(TextureWrapMethod _textureWrapMethod = TextureWrapMethod::CLAMP_TO_EDGE)
        : m_TextureWrapMethod(_textureWrapMethod)
    {
        // Default Texture Offset
        m_TextureOffset.x = 0.0f;
        m_TextureOffset.y = 0.0f;

        m_TextureScale.x = 1.0f;
        m_TextureScale.y = 1.0f;

        m_TextureRotation = 0.0f;
    }
    virtual ~MKSprite() {}

    // Virtual Function(s)
    virtual mkBool initSprite(const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);
    virtual mkBool initSpriteWithFile(const mkString& _fileName, const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);
    virtual mkBool initSpriteWithTexture(Texture2D* _texture, const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);
};

NS_MK_END

#endif // MK_SPRITE_H