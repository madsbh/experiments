/*
Copyright (c) 2012 Advanced Micro Devices, Inc.  

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
//Originally written by Erwin Coumans

//
//#include "vld.h"
#ifndef __APPLE__
#include <GL/glew.h>
#endif


#ifdef __APPLE__
#include "MacOpenGLWindow.h"
#else
#include "Win32OpenGLRenderManager.h"
#endif

#include "fontstash.h"


#include "LinearMath/btQuickprof.h"
#include "LinearMath/btQuaternion.h"

#include "../../opencl/gpu_rigidbody_pipeline/CommandlineArgs.h"

bool printStats = false;
bool pauseSimulation = false;
bool shootObject = false;

int m_glutScreenWidth;
int m_glutScreenHeight;

bool useInterop = false;




GLuint m_texturehandle;
GLuint shaderProgram;
GLuint positionUniform;
GLint colourAttribute, positionAttribute,textureAttribute;
GLuint vertexArrayObject,vertexBuffer;
GLuint  indexBuffer;




void loadShader();
unsigned int indexData[6] = {0,1,2,0,2,3};

void loadBufferData(){
    Vertex vertexDataOrg[4] = {
        { vec4(-0.5, -0.5, 0.0, 1.0 ), vec4( 1.0, 0.0, 0.0, 1.0 ) ,vec2(0,0)},
        { vec4(-0.5,  0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0,1)},
        { vec4( 0.5,  0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(1,1)},
        { vec4( 0.5, -0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(1,0)}
    };

    Vertex vertexData[4] = {
        { vec4(-0.5, -0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.0078125,0.015625)},
        { vec4(-0.5,  0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.101562,0.015625)},
        { vec4( 0.5,  0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.101562,0.105469)},
        { vec4( 0.5, -0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.0078125,0.105469)}
    };
    
    Vertex vertexData2[4] = {
        { vec4(0, 0.901042, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.0078125,0.015625)},
        { vec4(0.0234375, 0.901042, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.101562,0.015625)},
        { vec4( 0.0234375,  0.871094, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.101562,0.105469)},
        { vec4( 0., 0.871094, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) ,vec2(0.0078125,0.105469)}
    };

    
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
    GLuint err = glGetError();
    assert(err==GL_NO_ERROR);

  
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(int), indexData,GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(colourAttribute);
	err = glGetError();
    assert(err==GL_NO_ERROR);
    
	glEnableVertexAttribArray(textureAttribute);
    
    glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
    glVertexAttribPointer(colourAttribute  , 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)sizeof(vec4));
    glVertexAttribPointer(textureAttribute , 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(sizeof(vec4)+sizeof(vec4)));
	err = glGetError();
    assert(err==GL_NO_ERROR);
    
}

void initTestTexture()
{
    //	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,(GLuint*)&m_texturehandle);
	
    GLint err = glGetError();
    assert(err==GL_NO_ERROR);
    
    glBindTexture(GL_TEXTURE_2D,m_texturehandle);
    
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
	int width=256;
	int height=256;
	unsigned char* image = (unsigned char*)malloc(width*height);
	memset(image,0,width*height);
	for (int i=0;i<width;i++)
	{
		for (int j=0;j<height;j++)
		{
			if (i==j)
				image[i+width*j]=0;
			else
				image[i+width*j]=255;
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width,height,0,GL_RED,GL_UNSIGNED_BYTE,image);
	
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
	
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
    free(image);

}

// Load the shader from the source text
void gltLoadShaderSrc(const char *szShaderSrc, GLuint shader)
{
	GLchar *fsStringPtr[1];
    
	fsStringPtr[0] = (GLchar *)szShaderSrc;
	glShaderSource(shader, 1, (const GLchar **)fsStringPtr, NULL);
}

static const char* vertexShader= \
"#version 150   \n"
"\n"
"uniform vec2 p;\n"
"\n"
"in vec4 position;\n"
"in vec4 colour;\n"
"out vec4 colourV;\n"
"\n"
"in vec2 texuv;\n"
"out vec2 texuvV;\n"
"\n"
"\n"
"void main (void)\n"
"{\n"
"    colourV = colour;\n"
"	gl_Position = vec4(p.x+position.x, p.y+position.y,0.f,1.f);\n"
"	texuvV=texuv;\n"
"}\n";

static const char* fragmentShader= \
"#version 150\n"
"\n"
"in vec4 colourV;\n"
"out vec4 fragColour;\n"
"in vec2 texuvV;\n"
"\n"
"uniform sampler2D Diffuse;\n"
"\n"
"void main(void)\n"
"{\n"
"	vec4 texcolorred = texture(Diffuse,texuvV);\n"
"//	vec4 texcolor = vec4(texcolorred.x,texcolorred.x,texcolorred.x,texcolorred.x);\n"
"	vec4 texcolor = vec4(1,1,1,texcolorred.x);\n"
"\n"
"    fragColour = colourV*texcolor;\n"
"}\n";

GLuint gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg)
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader;
	GLuint hReturn = 0;
	GLint testVal;
    
	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
	gltLoadShaderSrc(vertexShader, hVertexShader);
	gltLoadShaderSrc(fragmentShader, hFragmentShader);
	
	// Compile them
	glCompileShader(hVertexShader);
	GLuint err = glGetError();
    assert(err==GL_NO_ERROR);
    
    glCompileShader(hFragmentShader);
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
	// Check for errors
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
        char temp[256] = "";
        glGetShaderInfoLog( hVertexShader, 256, NULL, temp);
        fprintf( stderr, "Compile failed:\n%s\n", temp);
        assert(0);
        exit(0);
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}
    
	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
        char temp[256] = "";
        glGetShaderInfoLog( hFragmentShader, 256, NULL, temp);
        fprintf( stderr, "Compile failed:\n%s\n", temp);
        assert(0);
        exit(0);
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}
    
	// Link them - assuming it works...
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);
    
	glLinkProgram(hReturn);
    
	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);
    
	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(hReturn);
		return (GLuint)NULL;
	}
    
	return hReturn;  
}

void loadShader(){
	shaderProgram = gltLoadShaderPair(vertexShader,fragmentShader);
    
   positionUniform = glGetUniformLocation(shaderProgram, "p");
    if (positionUniform < 0) {
		assert(0);
	}
	colourAttribute = glGetAttribLocation(shaderProgram, "colour");
	if (colourAttribute < 0) {
        assert(0);
   }
	positionAttribute = glGetAttribLocation(shaderProgram, "position");
	if (positionAttribute < 0) {
		assert(0);
  	}
	textureAttribute = glGetAttribLocation(shaderProgram,"texuv");
	if (textureAttribute < 0) {
		assert(0);
	}
    
}

void display() {
   
    glClearColor(.4, .4, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    GLint err = glGetError();
    assert(err==GL_NO_ERROR);
    
	const float timeScale = 0.008f;
	
    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindVertexArray(vertexArrayObject);
    
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
    
 //   glBindTexture(GL_TEXTURE_2D,m_texturehandle);
    
    
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
    vec2 p( 0.f,0.f);//?b?0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
    glUniform2fv(positionUniform, 1, (const GLfloat *)&p);
    
    err = glGetError();
    assert(err==GL_NO_ERROR);
	err = glGetError();
    assert(err==GL_NO_ERROR);
    
    glEnableVertexAttribArray(positionAttribute);
	err = glGetError();
    assert(err==GL_NO_ERROR);

    glEnableVertexAttribArray(colourAttribute);
	err = glGetError();
    assert(err==GL_NO_ERROR);
    
	glEnableVertexAttribArray(textureAttribute);
    
    glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
    glVertexAttribPointer(colourAttribute  , 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)sizeof(vec4));
    glVertexAttribPointer(textureAttribute , 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(sizeof(vec4)+sizeof(vec4)));
	err = glGetError();
    assert(err==GL_NO_ERROR);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    
    //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    int indexCount = 6;
    err = glGetError();
    assert(err==GL_NO_ERROR);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
//	glutSwapBuffers();
}




const char* fileName="../../bin/1000 stack.bullet";
void Usage()
{
	printf("\nprogram.exe [--pause_simulation=<0 or 1>] [--load_bulletfile=test.bullet] [--enable_interop=<0 or 1>] [--enable_gpusap=<0 or 1>] [--enable_convexheightfield=<0 or 1>] [--enable_static=<0 or 1>] [--x_dim=<int>] [--y_dim=<num>] [--z_dim=<int>] [--x_gap=<float>] [--y_gap=<float>] [--z_gap=<float>]\n"); 
};

int main(int argc, char* argv[])
{
	GLint err;
   CommandLineArgs args(argc,argv);

	if (args.CheckCmdLineFlag("help"))
	{
		Usage();
		return 0;
	}

	args.GetCmdLineArgument("enable_interop", useInterop);
	printf("useInterop=%d\n",useInterop);



	args.GetCmdLineArgument("pause_simulation", pauseSimulation);
	printf("pause_simulation=%d\n",pauseSimulation);
	

	
	char* tmpfile = 0;
	args.GetCmdLineArgument("load_bulletfile", tmpfile );
	if (tmpfile)
		fileName = tmpfile;

	printf("load_bulletfile=%s\n",fileName);

	
	printf("\n");
#ifdef __APPLE__
	MacOpenGLWindow* window = new MacOpenGLWindow();
#else
	Win32OpenGLWindow* window = new Win32OpenGLWindow();
#endif
	
	int width = 512;
	int height= 512;

	window->init(width,height);
#ifndef __APPLE__
	err = glewInit();
#endif
    window->runMainLoop();
    
    loadShader();
    
    loadBufferData();
    
    initTestTexture();
    
	window->startRendering();
	window->endRendering();

	
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
		
//	render.InitShaders();
  
//	render.writeTransforms();

    window->runMainLoop();

	//window->setMouseCallback(btDefaultMouseCallback);
	//window->setKeyboardCallback(btDefaultKeyboardCallback);


    err = glGetError();
    assert(err==GL_NO_ERROR);


		int done;
	struct sth_stash* stash = 0;
	FILE* fp = 0;
	int datasize;
	unsigned char* data;
	float sx,sy,dx,dy,lh;
	int droidRegular, droidItalic, droidBold, droidJapanese, dejavu;
	GLuint texture;

	stash = sth_create(512,512);//256,256);//,1024);//512,512);
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
	if (!stash)
	{
		fprintf(stderr, "Could not create stash.\n");
		return -1;
	}

	// Load the first truetype font from memory (just because we can).
#ifdef _WIN32
    const char* fontPath = "../../bin/";
#else
    const char* fontPath = "./";
#endif
    
    char fullFontFileName[1024];
    sprintf(fullFontFileName,"%s%s",fontPath,"DroidSerif-Regular.ttf");//cour.ttf");//times.ttf");//DroidSerif-Regular.ttf");
    
	fp = fopen(fullFontFileName, "rb");

    err = glGetError();
    assert(err==GL_NO_ERROR);
    
    assert(fp);
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        datasize = (int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        data = (unsigned char*)malloc(datasize);
        if (data == NULL)
        {
            assert(0);
            return -1;
        }
        else
            fread(data, 1, datasize, fp);
        fclose(fp);
        fp = 0;
    }
	if (!(droidRegular = sth_add_font_from_memory(stash, data)))
    {
        assert(0);
        return -1;
    }
    err = glGetError();
    assert(err==GL_NO_ERROR);

	// Load the remaining truetype fonts directly.
    sprintf(fullFontFileName,"%s%s",fontPath,"DroidSerif-Italic.ttf");

	if (!(droidItalic = sth_add_font(stash,fullFontFileName)))
	{
        assert(0);
        return -1;
    }
     sprintf(fullFontFileName,"%s%s",fontPath,"DroidSerif-Bold.ttf");

	if (!(droidBold = sth_add_font(stash,fullFontFileName)))
	{
        assert(0);
        return -1;
    }
    err = glGetError();
    assert(err==GL_NO_ERROR);
    
     sprintf(fullFontFileName,"%s%s",fontPath,"DroidSansJapanese.ttf");
    if (!(droidJapanese = sth_add_font(stash,fullFontFileName)))
	{
        assert(0);
        return -1;
    }
    err = glGetError();
    assert(err==GL_NO_ERROR);


	while (!window->requestedExit())
	{
		CProfileManager::Reset();
        GLint err = glGetError();
        assert(err==GL_NO_ERROR);
        
       // glClearColor(0.5f,0.5f,0.5f,1.f);
        
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
        window->startRendering();
		
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		    glClearColor(1,1,1,1);//.4, .4, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //display();
      
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
    if (1)
	{
		BT_PROFILE("font stash rendering");
				// Update and render
		glEnable(GL_BLEND);
        err = glGetError();
        assert(err==GL_NO_ERROR);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        err = glGetError();
        assert(err==GL_NO_ERROR);

        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		glDisable(GL_DEPTH_TEST);
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		//glColor4ub(255,0,0,255);
		
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
        glEnable(GL_BLEND);
		
        err = glGetError();
        assert(err==GL_NO_ERROR);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		sx = 0; sy = height;
		
		sth_begin_draw(stash);
		
		dx = sx; dy = sy;
		static int once=0;

		for (int i=0;i<1;i++)
		{
			dx = sx;
			if (once!=1)
			{
				//need to save this file as UTF-8 without signature, codepage 650001 in Visual Studio
			    err = glGetError();
                assert(err==GL_NO_ERROR);

				sth_draw_text(stash, droidJapanese,32.f, dx, dy-36, (const char*) "\xE7\xA7\x81\xE3\x81\xAF\xE3\x82\xAC\xE3\x83\xA9\xE3\x82\xB9\xE3\x82\x92\xE9\xA3\x9F\xE3\x81\xB9\xE3\x82\x89\xE3\x82\x8C\xE3\x81\xBE\xE3\x81\x99\xE3\x80\x82",&dx,
                              width,height);//はabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=?/\][{}.,<>`~@#$%^", &dx);
//				sth_draw_text(stash, droidJapanese,32.f, dx, dy, (const char*) "私はガラスを食べられます。それは私を傷つけません。",&dx);//はabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=?/\][{}.,<>`~@#$%^", &dx);
				
				dx = sx;

                err = glGetError();
                assert(err==GL_NO_ERROR);
				sth_flush_draw(stash);
				dx=0;			
				sth_draw_text(stash, droidRegular,30.f, dx, dy-60, "How does this OpenGL True Type font look? ", &dx,width,height);
				dx=0;
				dy-=30;
				sth_draw_text(stash, droidRegular,30.f, dx, dy-60, "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890", &dx,width,height);
				dx=0;
				dy-=30;
				sth_draw_text(stash, droidRegular,30.f, dx, dy-60, "!@#$%^abcdefghijklmnopqrstuvwxyz", &dx,width,height);

				dx=0;
			//	sth_draw_text(stash, droidRegular,16.f, dx, dy-42, "aph OpenGL Profile aCABCabdabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^", &dx,width,height);
				//sth_draw_text(stash, droidRegular,16.f, dx, dy-42, "aph OpenGL Profile aCABCabdabcdefghijlkmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^", &dx,width,height);

				sth_flush_draw(stash);
                err = glGetError();
                assert(err==GL_NO_ERROR);


			}	else
			{
				dx = sx;
				dy = height;
			
                err = glGetError();
                assert(err==GL_NO_ERROR);

                
				sth_draw_texture(stash, droidRegular, 32.f, 0, 0,width,height, "a", &dx);
                err = glGetError();
                assert(err==GL_NO_ERROR);

			}
			once++;
		}
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		sth_end_draw(stash);
		
		glEnable(GL_DEPTH_TEST);
        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		//glFinish();
	}
        err = glGetError();
        assert(err==GL_NO_ERROR);
        

		window->endRendering();

        err = glGetError();
        assert(err==GL_NO_ERROR);
        
		{
			BT_PROFILE("glFinish");
			glFinish();
		}

		CProfileManager::Increment_Frame_Counter();

		static bool printStats  = true;

		
		
		 if (printStats && !pauseSimulation)
		 {
			static int count = 0;
			count--;
			if (count<0)
			{
				count = 100;
//				CProfileManager::dumpAll();
				//printStats  = false;
			} else
			{
//				printf(".");
			}
		 }
		

	}

#ifdef _WIN32
	sth_delete(stash);
	free(data);
#endif

//	render.CleanupShaders();
	window->exit();
	delete window;
	
	return 0;

}