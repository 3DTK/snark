// This file is part of snark, a generic and flexible library
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// snark is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// snark is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with snark. If not, see <http://www.gnu.org/licenses/>.

/// @author Cedric Wohlleber

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_H_

#include <Qt3D/qgltexture2d.h>
#include <Qt3D/qglscenenode.h>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qglabstractscene.h>
#include <Qt3D/qglpainter.h>

namespace snark { namespace graphics { namespace View {

class Quad
{
public:
    Quad( const QImage& image );
    QGLSceneNode* node() const;

private:
    QGLBuilder m_builder;
    QGeometryData m_geometry;
    QGLTexture2D m_texture;
    QGLMaterial m_material;
    QGLSceneNode* m_sceneNode;
};

class Texture
{
public:
    Texture( const QString& string, const QColor4ub& color );

    void draw( QGLPainter* painter );
    
private:    
    QImage m_image;
    
};


} } } // namespace snark { namespace graphics

#endif /*SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_TEXTURE_H_*/
