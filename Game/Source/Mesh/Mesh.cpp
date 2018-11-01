#include "GamePCH.h"

#include "Mesh/Mesh.h"

Mesh::Mesh()
{
    m_VBO = 0;
    m_pShader = 0;

    m_PrimitiveType = -1;
    m_NumVerts = 0;
}
    
Mesh::~Mesh()
{
    glDeleteBuffers( 1, &m_VBO );
}

void SetUniform1f(GLuint shader, const char* uniformName, float value)
{
    GLint loc = glGetUniformLocation( shader, uniformName );
    if( loc != -1 )
    {
        glUniform1f( loc, value );
    }
}

void SetUniform2f(GLuint shader, const char* uniformName, vec2 value)
{
    GLint loc = glGetUniformLocation( shader, uniformName );
    if( loc != -1 )
    {
        glUniform2f( loc, value.x, value.y );
    }
}

void Mesh::Draw(vec2 objectPos, float objectAngle, vec2 objectScale, vec2 cameraPos, vec2 projectionScale)
{
    assert( m_PrimitiveType != -1 );
    assert( m_NumVerts != 0 );
    assert( m_pShader != 0 );
    assert( m_pShader->GetProgram() != 0 );

    // Bind buffer and set up attributes.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    GLint loc = glGetAttribLocation( m_pShader->GetProgram(), "a_Position" );
    if( loc != -1 )
    {
        glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Pos) );
        glEnableVertexAttribArray( loc );
    }

    loc = glGetAttribLocation( m_pShader->GetProgram(), "a_Color" );
    if( loc != -1 )
    {
        glVertexAttribPointer( loc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, m_Color) );
        glEnableVertexAttribArray( loc );
    }

    // Set up shader.
    GLuint shader = m_pShader->GetProgram();
    glUseProgram( shader );

    // Set up uniforms.
    SetUniform2f( shader, "u_ObjectScale", objectScale );
    SetUniform1f( shader, "u_ObjectAngleRadians", objectAngle / 180.0f * PI );
    SetUniform2f( shader, "u_ObjectPosition", objectPos );
    SetUniform2f( shader, "u_CameraTranslation", cameraPos * -1 );
    SetUniform2f( shader, "u_ProjectionScale", projectionScale );

    CheckForGLErrors();

    // Draw.
    glDrawArrays( m_PrimitiveType, 0, m_NumVerts );

    CheckForGLErrors();
}

void Mesh::GenerateTriangle()
{
    // ATM this can only be called once, so we assert if it's called again with a VBO already allocated.
    assert( m_VBO == 0 );

    // Vertex info for a diamond.
    VertexFormat vertexAttributes[] = {
        VertexFormat( vec2(  0.0f,  1.0f ), MyColor( 255, 255, 255, 255 ) ),
        VertexFormat( vec2( -0.5f, -1.0f ), MyColor( 255, 255, 255, 255 ) ),
        VertexFormat( vec2(  0.5f, -1.0f ), MyColor( 255, 255, 255, 255 ) ),
    };

    // Gen and fill buffer with our attributes.
    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat) * 3, vertexAttributes, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    m_PrimitiveType = GL_TRIANGLES;
    m_NumVerts = 3;

    // Check for errors.
    CheckForGLErrors();
}

void Mesh::GenerateCircle()
{
    // ATM this can only be called once, so we assert if it's called again with a VBO already allocated.
    assert( m_VBO == 0 );

    // Vertex position info for a diamond.
    VertexFormat vertexAttributes[] = {
        VertexFormat( vec2(  0.0f,  1.0f ), MyColor(   0, 255,   0, 255 ) ),
        VertexFormat( vec2( -1.0f,  0.0f ), MyColor(   0, 255,   0, 255 ) ),
        VertexFormat( vec2(  1.0f,  0.0f ), MyColor(   0, 255,   0, 255 ) ),
        VertexFormat( vec2(  0.0f, -1.0f ), MyColor(   0, 255,   0, 255 ) ),
    };

    // Gen and fill buffer with our attributes.
    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat) * 4, vertexAttributes, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    m_PrimitiveType = GL_TRIANGLE_STRIP;
    m_NumVerts = 4;

    // Check for errors.
    CheckForGLErrors();
}
