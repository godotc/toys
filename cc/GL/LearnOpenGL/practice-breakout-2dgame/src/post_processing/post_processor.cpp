#include "./post_processor.h"

PostProcessor::PostProcessor(Shader shader, unsigned int w, unsigned int h)
{
    PostProcessingShader = shader;
    m_Width              = w;
    m_Height             = h;
    bChaos               = false;
    bConfuse             = false;
    bShake               = false;

    glGenFramebuffers(1, &this->MSFBO);
    glGenFramebuffers(1, &this->FBO);
    glGenRenderbuffers(1, &this->RBO);

    // initialize renderbuffer storeage with a multisampled color buffer (don't need depth /stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    {
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        // allocate  storage for render
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, m_Width, m_Height);
        // attach MS render buffer objcet to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOG_ERROR("POSTPROCESSOR: Failed to initialize MSFBD");
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    {
        this->Texture.Generate(m_Width, m_Height, NULL);
        // attach texture to frambuffer as its color attachment
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, RBO);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOG_ERROR("POSTPROCESSOR: Failed to initialize FBO");
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // initialize initial data and unifroms
    initRenderData();
    this->PostProcessingShader.Use().SetInteger("scene", 0);

    float offset        = 1.f / 300.f;
    float offsets[9][2] = {
        {-offset,  offset}, // top-left
        {   0.0f,  offset}, // top-center
        { offset,  offset}, // top-right
        {-offset,    0.0f}, // center-left
        {   0.0f,    0.0f}, // center-center
        { offset,    0.0f}, // center - right
        {-offset, -offset}, // bottom-left
        {   0.0f, -offset}, // bottom-center
        { offset, -offset}  // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (float *)offsets);


}

void PostProcessor::BeginRender() {}
void PostProcessor::EndRender() {}
void PostProcessor::Render(float time) {}

void PostProcessor::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float        vertices[] =
        {-1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
         -1.0f, 1.0f, 0.0f, 1.0f,

         -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
