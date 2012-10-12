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

#ifndef SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_ACTION_H_
#define SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_ACTION_H_

#include <boost/function.hpp>
#include <qaction.h>
#include <qactiongroup.h>
#include <qicon.h>

namespace snark { namespace graphics { namespace View {

class Action : public QAction // quick and dirty
{
    Q_OBJECT

    public:
        Action( const std::string& name, boost::function< void() > f );

    public slots:
        void action();

    private:
        boost::function< void() > m_action;
};

class ToggleAction : public QAction
{
    Q_OBJECT

    public:
        ToggleAction( const std::string& name
                    , boost::function< void( bool ) > f
                    , const std::string& key = "" );

        ToggleAction( const QIcon& icon
                    , const std::string& name
                    , boost::function< void( bool ) > f
                    , const std::string& key = "" );

    public slots:
        void action( bool checked );

    private:
        boost::function< void( bool ) > m_functor;
};

} } } // namespace snark { namespace graphics { namespace View {

#endif // SNARK_GRAPHICS_APPLICATIONS_LABELPOINTS_ACTION_H_
