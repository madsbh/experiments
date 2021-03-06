/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "akVertexGroup.h"

akVertexGroup::akVertexGroup(UTuint32 size)
{
	m_size = size;
	m_indices.resize(size);
	m_weights.resize(size);
}

akVertexGroup::~akVertexGroup()
{
	m_indices.clear();
	m_weights.clear();
}

void akVertexGroup::add(UTuint32 vertexIndex, akScalar weight)
{
//	printf("%d,%f\n",vertexIndex,weight);
	m_indices.push_back(vertexIndex);
	m_weights.push_back(weight);
	m_size++;
}

void akVertexGroup::set(UTuint32 idx, UTuint32 vertexIndex, akScalar weight)
{
	if(idx < m_size)
	{
		m_indices[idx] = vertexIndex;
		m_weights[idx] = weight;
	}
}
