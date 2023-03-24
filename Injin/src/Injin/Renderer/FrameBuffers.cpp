#include "FrameBuffers.h"
#include <iostream>
engin::FrameBuffers::FrameBuffers(uint32_t width, uint32_t height, uint16_t nCol, bool isDepth, bool isStencil, bool depthStencilAsTex)
    :m_width(width),m_height(height)
{
    int32_t col;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &col);
    m_nColAtt = nCol > col ? col : nCol;
    m_colAtt = new uint32_t[m_nColAtt];
    glGenFramebuffers(1, &m_fbo);
    processFrameBuffer(isDepth, isStencil, depthStencilAsTex);
}

engin::FrameBuffers::~FrameBuffers()
{
    delete[] m_colAtt;
}

uint16_t engin::FrameBuffers::getFbo() const
{
    return m_fbo;
}

uint16_t engin::FrameBuffers::getColAtt(uint32_t index) const
{
    return m_colAtt[index];
}

uint16_t engin::FrameBuffers::getDepSenAtt() const
{
    return m_depAndStenAtt;
}

void engin::FrameBuffers::bindFrameBuffer()
{
    //Normally When we initialize viewport we are instructing the GPU to draw with the viewport
    //In this case we are assign the framebuffer size and drawing with the screen viewport 
    //which will end up leaving  some of the pixel to render(ie instead drawing for viewport size we will draw 
    // for framebuffer size)
    //So that is the reason why we change the viewport to framebuffer size and change back to original size
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void engin::FrameBuffers::unbindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glViewport(0, 0, width, height);
}

void engin::FrameBuffers::clrBuffer(const glm::vec4& value)
{
    engin::grpicsInit::clrColrBuffr(value);
    engin::grpicsInit::clrBuffr();
}

void engin::FrameBuffers::renderFrameBuffer(Meshes* mesh, uint16_t colAtt)
{
    glBindTexture(GL_TEXTURE_2D, getColAtt(colAtt));
    mesh->drawMesh();
}

void engin::FrameBuffers::processFrameBuffer(bool isDepth, bool isStencil, bool depthStencilAsTex)
{
    bindFrameBuffer();
    glGenTextures(m_nColAtt, m_colAtt);
    for (uint16_t i = 0; i < m_nColAtt; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_colAtt[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTextureParameteri(m_colAtt[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_colAtt[i], GL_TEXTURE_WRAP_T, GL_REPEAT);
        //If we provide mipmapFilter then when we minimize the window the mipmap lvl will reduce
        //we will not get any sample from it
        glTextureParameteri(m_colAtt[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_colAtt[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colAtt[i], 0);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    if (isDepth && isStencil)
    {
        if (depthStencilAsTex)
        {
            glGenTextures(1, &m_depAndStenAtt);
            glBindTexture(GL_TEXTURE_2D, m_depAndStenAtt);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0,
                GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depAndStenAtt, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            glGenRenderbuffers(1, &m_depAndStenAtt);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depAndStenAtt);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depAndStenAtt);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }
    else if (isDepth)
    {
        if (depthStencilAsTex)
        {
            glGenTextures(1, &m_depAndStenAtt);
            glBindTexture(GL_TEXTURE_2D, m_depAndStenAtt);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0,
                GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depAndStenAtt, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            glGenRenderbuffers(1, &m_depAndStenAtt);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depAndStenAtt);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depAndStenAtt);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }
    else if (isStencil)
    {
        if (depthStencilAsTex)
        {
            glGenTextures(1, &m_depAndStenAtt);
            glBindTexture(GL_TEXTURE_2D, m_depAndStenAtt);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, m_width, m_height, 0,
                GL_STENCIL_COMPONENTS, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depAndStenAtt, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            glGenRenderbuffers(1, &m_depAndStenAtt);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depAndStenAtt);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, m_width, m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depAndStenAtt);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer creation is incomplete. Please check the code.\n";
    unbindFrameBuffer();
}
