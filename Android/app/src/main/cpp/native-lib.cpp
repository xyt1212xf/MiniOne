#include <jni.h>
#include <string>
#include "com_example_myapplication_RendererJNI.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <GLES2/gl2.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

GLint	g_programObject;
jint	g_width;
jint	g_height;


GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader ( type );

    if ( shader == 0 )
    {
        return 0;
    }

    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );

    // Compile the shader
    glCompileShader ( shader );

    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled )
    {
        GLint infoLen = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            void *infoLog = malloc ( sizeof ( char ) * infoLen );

            glGetShaderInfoLog ( shader, infoLen, NULL, (char*)infoLog );
           // LOGE("Error compiling shader:[%s]", infoLog );

            free ( infoLog );
        }

        glDeleteShader ( shader );
        return 0;
    }

    return shader;

}


//*********************************************************************************
//
JNIEXPORT void JNICALL Java_com_example_myapplication_RendererJNI_glesInit(JNIEnv *pEnv, jobject obj)
{
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vPosition;              \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
            "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
    fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    // Create the program object
    programObject = glCreateProgram ( );

    if ( programObject == 0 )
    {
        return;
    }

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    // Link the program
    glLinkProgram ( programObject );

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked )
    {
        GLint infoLen = 0;

        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            void *infoLog = malloc ( sizeof ( char ) * infoLen );

            glGetProgramInfoLog ( programObject, infoLen, NULL, (char*)infoLog );
           // LOGE("Error linking program:[%s]", infoLog );

            free ( infoLog );
        }

        glDeleteProgram ( programObject );
        return;
    }

    // Store the program object
    g_programObject = programObject;

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );

}

//*********************************************************************************
//
JNIEXPORT void JNICALL Java_com_example_myapplication_RendererJNI_glesRender(JNIEnv *pEnv, jobject obj)
{
    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                             -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f
    };

    // Set the viewport
    glViewport ( 0, 0, g_width, g_height );

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( g_programObject );

    // Load the vertex data
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( 0 );

    glDrawArrays ( GL_TRIANGLES, 0, 3 );
}

//*********************************************************************************
//
JNIEXPORT void JNICALL Java_com_example_myapplication_RendererJNI_glesResize(JNIEnv *pEnv, jobject obj, jint width, jint height) {
    g_width = width;
    g_height = height;
}