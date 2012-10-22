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

/// @author Vsevolod Vlaskine

#include <iostream>
#include "./Action.h"


namespace snark { namespace graphics { namespace View {

Action::Action( const std::string& name, boost::function< void() > f )
    : QAction( name.c_str(), NULL )
    , m_action( f )
{
    connect( this, SIGNAL( triggered() ), this, SLOT( action() ) );
}

void Action::action() { m_action(); }

ToggleAction::ToggleAction( const std::string& name, boost::function< void( bool ) > f, const std::string& key )
    : QAction( tr( name.c_str() ), NULL )
    , m_functor( f )
{
    setCheckable( true );
    if( key != "" ) { setShortcut( QKeySequence( tr( key.c_str() ) ) ); }
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( action( bool ) ) );
}

ToggleAction::ToggleAction( const QIcon& icon, const std::string& name, boost::function< void( bool ) > f, const std::string& key )
    : QAction( icon, tr( name.c_str() ), NULL )
    , m_functor( f )
{
    setCheckable( true );
    if( key != "" ) { setShortcut( QKeySequence( tr( key.c_str() ) ) ); }
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( action( bool ) ) );
}

void ToggleAction::action( bool checked ) { m_functor( checked ); }

} } } // namespace snark { namespace graphics { namespace View {
