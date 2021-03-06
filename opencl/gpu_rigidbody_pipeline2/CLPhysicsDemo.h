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

#ifndef CL_PHYSICS_DEMO_H
#define CL_PHYSICS_DEMO_H

#include "LinearMath/btAlignedObjectArray.h"

#ifdef _WIN32
class Win32OpenGLWindow;
#else
class MacOpenGLWindow;
#endif

#include "../gpu_rigidbody_pipeline/btCollidable.h"

struct CLPhysicsDemo
{
#ifdef _WIN32
	Win32OpenGLWindow* m_renderer;
#else
    MacOpenGLWindow* m_renderer;
#endif
    
	

	int m_numPhysicsInstances;
	int m_numDynamicPhysicsInstances;
	int m_maxShapeBufferCapacity;
	
	



	struct InternalData* m_data;
#ifdef _WIN32
	CLPhysicsDemo(Win32OpenGLWindow*	renderer, int maxShapeBufferCapacity);
#else
    CLPhysicsDemo(MacOpenGLWindow*	renderer, int maxShapeBufferCapacity);
#endif
    
	virtual ~CLPhysicsDemo();

	//btOpenCLGLInteropBuffer*	m_interopBuffer;
	
	void	init(int preferredDevice, int preferredPlatform, bool useInterop);
	
	void	setupInterop();

	int		registerCollisionShape(const float* vertices, int strideInBytes, int numVertices, const float* scaling, bool noHeightField);
	int		registerConvexShape(class btConvexUtility* utilPtr , bool noHeightField);
	int		registerConcaveMesh(class objLoader* obj, const float* scaling);

	int		registerPhysicsInstance(float mass, const float* position, const float* orientation, int collisionShapeIndex, int userPointer);

	void	setObjectTransform(const float* position, const float* orientation, int objectIndex);

	void	setObjectLinearVelocity(const float* velocityosition, int objectIndex);

	void	writeVelocitiesToGpu();
	void	writeBodiesToGpu();

	void	cleanup();

	void	stepSimulation();
};

#endif//CL_PHYSICS_DEMO_H