// Include MK
#include "MKSprite.h"

NS_MK_BEGIN

// Static Variable(s)
const mkString MKSprite::DEFAULT_VERTEX_SHADER = "Shaders/MKShader_Default.vsh";
const mkString MKSprite::DEFAULT_FRAGMENT_SHADER = "Shaders/MKShader_Default.fsh";

// Static Function(s)
void MKSprite::InitGLProgram(const mkString& _vertexShader, const mkString& _fragmentShader)
{
    // Create GLProgram
    m_GLProgram = new GLProgram();
    m_GLProgram->initWithFilenames(_vertexShader, _fragmentShader);
    m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    //m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD1, GLProgram::VERTEX_ATTRIB_TEX_COORD1);
    //m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD2, GLProgram::VERTEX_ATTRIB_TEX_COORD2);
    //m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD3, GLProgram::VERTEX_ATTRIB_TEX_COORD3);
    //m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_NORMAL, GLProgram::VERTEX_ATTRIB_NORMAL);
    //m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT, GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT);
    //m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_BLEND_INDEX, GLProgram::VERTEX_ATTRIB_BLEND_INDEX);
    m_GLProgram->link();
    m_GLProgram->updateUniforms();

    // Create GLProgramState
    m_GLProgramState = GLProgramState::getOrCreateWithGLProgram(m_GLProgram);
    setGLProgram(m_GLProgram);
    setGLProgramState(m_GLProgramState);

    // Update Uniforms
    SetTextureOffset(m_TextureOffset);
    SetTextureScale(m_TextureScale);
    SetTextureRotation(m_TextureRotation);
    m_GLProgram->updateUniforms();
}

void MKSprite::InitTextureParameters()
{
	// Set the texture parameters to GL_REPEAT.
	Texture2D::TexParams textureParameters;
	textureParameters.minFilter = GL_NEAREST;
	textureParameters.magFilter = GL_NEAREST;

    switch (m_TextureWrapMethod)
    {
    case CLAMP_TO_EDGE:
        textureParameters.wrapS = GL_CLAMP_TO_EDGE;
        textureParameters.wrapT = GL_CLAMP_TO_EDGE;
        break;
    case REPEAT:
        textureParameters.wrapS = GL_REPEAT;
        textureParameters.wrapT = GL_REPEAT;
        break;
    default:
        textureParameters.wrapS = GL_CLAMP_TO_EDGE;
        textureParameters.wrapT = GL_CLAMP_TO_EDGE;
        break;
    }

	getTexture()->setTexParameters(textureParameters);
}

// Static Function(s)
MKSprite* MKSprite::Create(const mkString& _vertexShader, const mkString& _fragmentShader)
{
    MKSprite *sprite = new (std::nothrow) MKSprite(CLAMP_TO_EDGE);
    if (sprite && sprite->initSprite(_vertexShader, _fragmentShader))
    {
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MKSprite* MKSprite::CreateWithFile(const mkString& _fileName, TextureWrapMethod _textureWrapMethod, const mkString& _vertexShader, const mkString& _fragmentShader)
{
    MKSprite *sprite = new (std::nothrow) MKSprite(_textureWrapMethod);
    if (sprite && sprite->initSpriteWithFile(_fileName, _vertexShader, _fragmentShader))
    {
        sprite->autorelease();
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MKSprite* MKSprite::CreateWithTexture(Texture2D *texture, TextureWrapMethod _textureWrapMethod, const mkString& _vertexShader, const mkString& _fragmentShader)
{
	MKSprite *sprite = new (std::nothrow) MKSprite(_textureWrapMethod);
	if (sprite && sprite->initSpriteWithTexture(texture, _vertexShader, _fragmentShader))
	{
        sprite->autorelease();
        return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return nullptr;
}

// Virtual Override(s)
mkBool MKSprite::initSprite(const mkString& _vertexShader, const mkString& _fragmentShader)
{
    if (!Super::init()) { return false; }

    InitGLProgram(_vertexShader, _fragmentShader);
    InitTextureParameters();

    return true;
}

mkBool MKSprite::initSpriteWithFile(const mkString& _fileName, const mkString& _vertexShader, const mkString& _fragmentShader)
{
    if (!Super::initWithFile(_fileName)) { return false; }

    InitGLProgram(_vertexShader, _fragmentShader);
    InitTextureParameters();

    return true;
}

mkBool MKSprite::initSpriteWithTexture(Texture2D* _texture, const mkString& _vertexShader, const mkString& _fragmentShader)
{
    if (!Super::initWithTexture(_texture)) { return false; }

    InitGLProgram(_vertexShader, _fragmentShader);
    InitTextureParameters();

    return true;
}

NS_MK_END