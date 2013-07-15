#pragma once

#include <FFGLShader.h>
#include <FFGLPluginSDK.h>

class PoincareFFGL :
public CFreeFrameGLPlugin
{
public:
    PoincareFFGL();
    virtual ~PoincareFFGL() {}

	DWORD SetParameter(const SetParameterStruct* pParam);
	DWORD GetParameter(DWORD dwIndex);
	DWORD ProcessOpenGL(ProcessOpenGLStruct* pGL);
    DWORD InitGL(const FFGLViewportStruct *vp);
    DWORD DeInitGL();


	static DWORD __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
    {
        *ppOutInstance = new PoincareFFGL();
        if (*ppOutInstance != NULL)
            return FF_SUCCESS;
        return FF_FAIL;
    }

protected:	
	float m_Heat;
	float m_HeatY;
	
	int m_initResources;

	GLuint m_heatTextureId;

	FFGLExtensions m_extensions;
    FFGLShader m_shader;
	GLint m_inputTextureLocation;
	GLint m_heatTextureLocation;
	GLint m_maxCoordsLocation;
	GLint m_heatAmountLocation;
};
