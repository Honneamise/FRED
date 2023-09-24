#include "funcs.h"
#include "memory.h"

#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#define WINDOW_W        960
#define WINDOW_H        600

#define TEXTURE_W       320
#define TEXTURE_H       200

#define ROWS            25
#define COLS            40

#define OPENGL_MAJOR    4
#define OPENGL_MINOR    3

typedef struct VIDEO
{
    //glfw
    GLFWwindow *window;

    //opengl
    GLuint vao;
    GLuint vbo;
    GLuint vert_shader;
    GLuint frag_shader;
    GLuint prog_shader;

    GLuint texture;
    uint8_t *texture_buffer;

} VIDEO;

static VIDEO *video = NULL;

/***************/
/* STATIC DATA */
/***************/
static const float TEXTURE_VERTICES[16] = 
{
    -1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
    1.0f,  1.0f, 1.0f, 0.0f
};

//vertex shader
static const char *VERT_SRC = "\n\
#version 450\n\
layout (location = 0) in vec2 Pos;\n\
layout (location = 1) in vec2 Uv;\n\
out vec2 Tex;\n\
void main()\n\
{\n\
    gl_Position = vec4(Pos.x, Pos.y, 0.0f, 1.0f);\n\
    Tex = Uv;\n\
}";

//fragment shader
static const char *FRAG_SRC = "\n\
#version 450\n\
uniform sampler2D Texture;\n\
in vec2 Tex;\n\
out vec4 Color;\n\
const vec3 Palette[16] = vec3[]\n\
(\n\
    vec3(0.0,  0.0,  0.0),  // #000000\n\
    vec3(0.5,  0.0,  0.0),  // #800000\n\
    vec3(0.0,  0.5,  0.0),  // #008000\n\
    vec3(0.5,  0.5,  0.0),  // #808000\n\
    vec3(0.0,  0.0,  0.5),  // #000080\n\
    vec3(0.5,  0.0,  0.5),  // #800080\n\
    vec3(0.0,  0.5,  0.5),  // #008080\n\
    vec3(0.75, 0.75, 0.75), // #C0C0C0\n\
    vec3(0.5,  0.5,  0.5),  // #808080\n\
    vec3(1.0,  0.0,  0.0),  // #FF0000\n\
    vec3(0.0,  1.0,  0.0),  // #00FF00\n\
    vec3(1.0,  1.0,  0.0),  // #FFFF00\n\
    vec3(0.0,  0.0,  1.0),  // #0000FF\n\
    vec3(1.0,  0.0,  1.0),  // #FF00FF\n\
    vec3(0.0,  1.0,  1.0),  // #00FFFF\n\
    vec3(1.0,  1.0,  1.0)   // #FFFFFF\n\
);\n\
void main()\n\
{\n\
    int index = int( texture2D(Texture, Tex).r * 255.0);\n\
    Color = vec4( Palette[index], 1.0);\n\
}";

//log
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = NULL;

//program
static PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
static PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
static PFNGLUSEPROGRAMPROC glUseProgram = NULL;
static PFNGLATTACHSHADERPROC glAttachShader = NULL;
static PFNGLDETACHSHADERPROC glDetachShader = NULL;
static PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
static PFNGLUNIFORM3FPROC glUniform3f = NULL;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
 
//shader
static PFNGLCREATESHADERPROC glCreateShader = NULL;
static PFNGLDELETESHADERPROC glDeleteShader = NULL;
static PFNGLSHADERSOURCEPROC glShaderSource = NULL;
static PFNGLCOMPILESHADERPROC glCompileShader = NULL;
static PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
 
//VBO
static PFNGLGENBUFFERSPROC glGenBuffers = NULL;
static PFNGLBINDBUFFERPROC glBindBuffer = NULL;
static PFNGLBUFFERDATAPROC glBufferData = NULL;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
static PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
 
//VAO
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;


/********/
/* GLFW */
/********/
static void glfw_resize_callback(GLFWwindow *window, int width, int height)
{
    int x = 0;
	int y = 0;
    int view_w = 0;
    int view_h = 0;

	float ratio = (float)TEXTURE_W/(float)TEXTURE_H;

	if( width/ratio < height)
	{
		view_w = width;
		view_h = (int)(view_w/ratio);
	}
	else
	{
		view_h = height;
		view_w = (int)(view_h*ratio);
	}

	x = (width - view_w)/2;
	y = (height - view_h)/2;
		
    glViewport(x, y, view_w, view_h);	

}

/**********/
static void glfw_init()
{
    glfwInit();

    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
    
    video->window = glfwCreateWindow(WINDOW_W, WINDOW_H, "", NULL, NULL);

    glfwMakeContextCurrent(video->window);

    glfwSwapInterval(0);
    glfwSetWindowSizeCallback(video->window, glfw_resize_callback);
}

/**********/
static void glfw_close()
{
    glfwDestroyWindow(video->window);
 
    glfwTerminate();
}

/**********/
/* OPENGL */
/**********/
static void opengl_load_extensions()
{
    //log
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
    glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)glfwGetProcAddress("glDebugMessageCallback");

    //program
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
    glDetachShader = (PFNGLDETACHSHADERPROC)glfwGetProcAddress("glDetachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
    glUniform3f = (PFNGLUNIFORM3FPROC)glfwGetProcAddress("glUniform3f");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glDisableVertexAttribArray");

    //shader
    glCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");

    //VBO
    glGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glfwGetProcAddress("glBufferSubData");

    //VAO
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");  
}

/**********/
static void APIENTRY opengl_debug_callback(
                GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* param)
{
    if(severity!=GL_DEBUG_SEVERITY_NOTIFICATION)//too much spam
    {
        fprintf(stdout, 
            "[OPENGL]: source=0x%X, type=0x%X, id=0x%X, severity=0x%X\nmessage=%s\n",
            source,
            type,
            id,
            severity,
            message);
    }
	
}

/**********/
static GLuint opengl_shader(GLenum type, const uint8_t *src)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const GLchar**)&src, NULL);

	glCompileShader(shader);

	GLint check = GL_FALSE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &check);

	if (check == GL_FALSE)
	{
		uint32_t len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		char *msg = alloc(len, sizeof(char));

		glGetShaderInfoLog(shader, len, &len, msg);

		error("[%s][%s]", __func__, msg);
	}

	return shader;
}

/**********/
static GLuint opengl_program_shader(GLuint vert_shader, GLuint frag_shader)
{
	GLuint prog_shader = glCreateProgram();

	glAttachShader(prog_shader, vert_shader);

	glAttachShader(prog_shader, frag_shader);

	glLinkProgram(prog_shader);

	GLint check = GL_FALSE;

	glGetProgramiv(prog_shader, GL_LINK_STATUS, &check);

	if (check == GL_FALSE)
	{
		uint32_t len = 0;

		glGetProgramiv(prog_shader, GL_INFO_LOG_LENGTH, &len);

		char *msg = alloc(len, sizeof(char));

		glGetProgramInfoLog(prog_shader, len, &len, msg);

		error("[%s][%s]", __func__, msg);
	}

	return prog_shader;
}

/**********/
static void opengl_init()
{
    opengl_load_extensions();

    #ifdef _DEBUG
	    glEnable(GL_DEBUG_OUTPUT);
	    glDebugMessageCallback(opengl_debug_callback, NULL);
    #endif

    glGenVertexArrays(1, &video->vao);
    glBindVertexArray(video->vao); 

	glGenBuffers(1, &video->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, video->vbo);

	glEnableVertexAttribArray(0);//vertex 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
    glEnableVertexAttribArray(1);//texture 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)) );

	glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), TEXTURE_VERTICES, GL_STATIC_DRAW);	
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);//fix for W if not multiple of 4

	glGenTextures(1, &video->texture);

	glBindTexture(GL_TEXTURE_2D, video->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEXTURE_W, TEXTURE_H, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    uint32_t len = 0;
    
    video->vert_shader = opengl_shader(GL_VERTEX_SHADER, VERT_SRC);
    
    video->frag_shader = opengl_shader(GL_FRAGMENT_SHADER, FRAG_SRC);
    
    video->prog_shader = opengl_program_shader(video->vert_shader, video->frag_shader);

	glUseProgram(video->prog_shader);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
}

/**********/
static void opengl_close()
{
    glDeleteTextures(1, &video->texture);

    glDisableVertexAttribArray(0);

    glDisableVertexAttribArray(1);

    glDeleteVertexArrays(1, &video->vao);

    glDeleteBuffers(1, &video->vbo);

	glDetachShader(video->prog_shader, video->vert_shader);
	glDeleteShader(video->vert_shader);

	glDetachShader(video->prog_shader, video->frag_shader);
	glDeleteShader(video->frag_shader);

	glDeleteProgram(video->prog_shader);
}

/*********/
/* VIDEO */
/*********/
void video_init()
{
    video = alloc(1, sizeof(VIDEO));

    video->texture_buffer = alloc(TEXTURE_W*TEXTURE_H,sizeof(uint8_t));

    glfw_init();

    opengl_init();

    glfw_resize_callback(video->window, WINDOW_W, WINDOW_H);
}

/**********/
void video_close()
{
    opengl_close();

    glfw_close();

    free(video->texture_buffer);

    free(video);
}

/**********/
void video_poll()
{
    glfwPollEvents();
}

/**********/
int video_run()
{
    return !glfwWindowShouldClose(video->window);
}

/**********/
void video_update_texture()
{
    uint8_t *text = memory_get_buffer();

    uint8_t *charset_base = memory_get_buffer() + CHARSET_BASE;
      
    //pixel pos in texture
    uint16_t posx = 0;
    uint16_t posy = 0;

    for(uint16_t count=0;count<ROWS*COLS*2;count+=2)
    {
        uint8_t *glyph_base = charset_base + text[(count)] * 8  ;

        posy = ( (count/2) / COLS) * 8;
        
        //for each byte of the glyph
        for(uint8_t i=0;i<8;i++)
        {
            posx = ( (count/2) % COLS) * 8;

            uint8_t byte = glyph_base[i];
            
            for(uint8_t j=0;j<8;j++)
            {
                uint16_t index = posy * TEXTURE_W + posx;

                if( (byte & 0x80) == 0x80) 
                {
                    video->texture_buffer[index] = (text[(count+1)] & 0xF0) >> 4;
                }
                else
                {
                    video->texture_buffer[index] = text[(count+1)] & 0x0F;
                }

                byte = (uint8_t)(byte << 1);

                //increase posx
                posx++;
            }

            //increase posy
            posy++;
        }
    }
}

/**********/
void video_update()
{
    video_update_texture();

    glClear(GL_COLOR_BUFFER_BIT);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TEXTURE_W, TEXTURE_H, GL_RED, GL_UNSIGNED_BYTE, video->texture_buffer);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glfwSwapBuffers(video->window);
}

/**********/
GLFWwindow *video_get_window()
{
    return video->window;
}