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

/// @author Vsevolod Vlaskine, Cedric Wohlleber

#ifndef SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_VIEWER_H_
#define SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_VIEWER_H_

#ifdef WIN32
#include <winsock2.h>

//#include <windows.h>
#endif

#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <snark/graphics/qt3d/view.h>
#include "./CameraReader.h"
#include "./Reader.h"

namespace snark { namespace graphics { namespace View {

class Viewer : public qt3d::view
{
    Q_OBJECT
public:
    std::vector< boost::shared_ptr< Reader > > readers;
    Viewer( const QColor4ub& background_color, double fov, bool z_up, bool orthographic = false,
            boost::optional< comma::csv::options > cameracsv = boost::optional< comma::csv::options >(),
            boost::optional< Eigen::Vector3d > cameraposition = boost::optional< Eigen::Vector3d >(),
            boost::optional< Eigen::Vector3d > cameraorientation = boost::optional< Eigen::Vector3d >()
          );
    void shutdown();

private slots:
    void read();

private:
    
    void initializeGL( QGLPainter *painter );
    void paintGL( QGLPainter *painter );
    void setCameraPosition( const Eigen::Vector3d& position, const Eigen::Vector3d& orientation );
    
    bool m_shutdown;
    bool m_lookAt;
    boost::scoped_ptr< CameraReader > m_cameraReader;
    boost::optional< Eigen::Vector3d > m_cameraposition;
    boost::optional< Eigen::Vector3d > m_cameraorientation;
    bool m_cameraFixed;
};

} } } // namespace snark { namespace graphics { namespace View {

#endif /*SNARK_GRAPHICS_APPLICATIONS_VIEWPOINTS_VIEWER_H_*/
