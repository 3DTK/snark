// This file is part of snark, a generic and flexible library for robotics research
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. All advertising materials mentioning features or use of this software
//    must display the following acknowledgement:
//    This product includes software developed by the University of Sydney.
// 4. Neither the name of the University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


/// @author Vsevolod Vlaskine

#include <limits>
#include <map>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include "./Action.h"
#include "./MainWindow.h"

namespace snark { namespace graphics { namespace View {

MainWindow::MainWindow( const std::string& title, Viewer* viewer )
    : m_viewer( *viewer )
    , m_fileFrameVisible( m_viewer.readers.size() > 1 )
{
    QMenu* fileMenu = menuBar()->addMenu( "File" );
    menuBar()->addMenu( fileMenu );

    m_fileFrame = new QFrame;
    m_fileFrame->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
    m_fileFrame->setFixedWidth( 300 );
    m_fileFrame->setContentsMargins( 0, 0, 0, 0 );
    m_fileLayout = new QGridLayout;
    m_fileLayout->setSpacing( 0 );
    m_fileLayout->setContentsMargins( 5, 5, 5, 0 );

    QLabel* filenameLabel = new QLabel( "<b>filename</b>" );
    QLabel* visibleLabel = new QLabel( "<b>view</b>" );
    m_fileLayout->addWidget( filenameLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop );
    m_fileLayout->addWidget( visibleLabel, 0, 1, Qt::AlignRight | Qt::AlignTop );
    m_fileLayout->setRowStretch( 0, 0 );
    m_fileLayout->setColumnStretch( 0, 10 ); // quick and dirty
    m_fileLayout->setColumnStretch( 1, 1 );
    m_fileFrame->setLayout( m_fileLayout );

    QFrame* frame = new QFrame;
    frame->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
    frame->setContentsMargins( 0, 0, 0, 0 );
    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );
    layout->addWidget( m_fileFrame, 0, 0 );
#if QT_VERSION >= 0x050000
    layout->addWidget( QWidget::createWindowContainer( viewer ), 0, 1 );
#else
  layout->addWidget( viewer, 0, 1 );
#endif
    layout->setColumnStretch( 0, 0 );
    layout->setColumnStretch( 1, 1 );
    frame->setLayout( layout );
    setCentralWidget( frame );
    resize( 640, 480 );

    m_viewMenu = menuBar()->addMenu( "View" );
    ToggleAction* action = new ToggleAction( "File Panel", boost::bind( &MainWindow::toggleFileFrame, this, _1 ) );
    action->setChecked( m_fileFrameVisible );
    m_viewMenu->addAction( action );
    updateFileFrame();
    toggleFileFrame( m_fileFrameVisible );
    setWindowTitle( title.c_str() );
}

CheckBox::CheckBox( boost::function< void( bool ) > f ) : m_f( f )
{
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( action( bool ) ) );
}

void CheckBox::action( bool checked ) {    m_f( checked ); }

void MainWindow::showFileGroup( std::string name, bool shown )
{
    FileGroupMap::iterator it = m_fileGroups.find( name );
    if( it == m_fileGroups.end() ) { std::cerr << "view-points: warning: file group \"" << name << "\" not found" << std::endl; }
    for( std::size_t i = 0; i < it->second.size(); ++i ) { it->second[i]->setCheckState( shown ? Qt::Checked : Qt::Unchecked ); }
}

void MainWindow::updateFileFrame() // quick and dirty
{
    for( std::size_t i = 0; i < m_viewer.readers.size(); ++i ) // quick and dirty: clean
    {
        for( unsigned int k = 0; k < 2; ++k )
        {
            if( m_fileLayout->itemAtPosition( i + 1, k ) == NULL ) { continue; }
            QWidget* widget = m_fileLayout->itemAtPosition( i + 1, k )->widget();
            m_fileLayout->removeWidget( widget );
            delete widget;
        }
    }
    bool sameFields = true;
    std::string fields;
    for( std::size_t i = 0; sameFields && i < m_viewer.readers.size(); ++i )
    {
        if( i == 0 ) { fields = m_viewer.readers[0]->options.fields; } // quick and dirty
        else { sameFields = m_viewer.readers[i]->options.fields == fields; }
    }
    m_fileGroups.clear();
    for( std::size_t i = 0; i < m_viewer.readers.size(); ++i )
    {
        static const std::size_t maxLength = 30; // arbitrary
        std::string filename = m_viewer.readers[i]->options.filename;
        if( filename.length() > maxLength )
        {
            // quick and dirty to avoid boost::filesystem 2 vs boost::filesystem 3 clash
            //std::string leaf = boost::filesystem::path( filename ).leaf();
            #ifdef WIN32
            std::string leaf = comma::split( filename, '\\' ).back();
            #else
            std::string leaf = comma::split( filename, '/' ).back();
            #endif
            filename = leaf.length() >= maxLength ? leaf : std::string( "..." ) + filename.substr( filename.length() - maxLength );
        }
        if( !sameFields ) { filename += ": \"" + m_viewer.readers[i]->options.fields + "\""; }
        m_fileLayout->addWidget( new QLabel( filename.c_str() ), i + 1, 0, Qt::AlignLeft | Qt::AlignTop );
        CheckBox* viewBox = new CheckBox( boost::bind( &Reader::show, boost::ref( *m_viewer.readers[i] ), _1 ) );
        viewBox->setCheckState( m_viewer.readers[i]->show() ? Qt::Checked : Qt::Unchecked );
        connect( viewBox, SIGNAL( toggled( bool ) ), &m_viewer, SLOT( update() ) ); // redraw when box is toggled
        viewBox->setToolTip( ( std::string( "check to make " ) + filename + " visible" ).c_str() );
        m_fileLayout->addWidget( viewBox, i + 1, 1, Qt::AlignRight | Qt::AlignTop );
        m_fileLayout->setRowStretch( i + 1, i + 1 == m_viewer.readers.size() ? 1 : 0 );
        m_fileGroups[ m_viewer.readers[i]->options.fields ].push_back( viewBox );
    }
    std::size_t i = 1 + m_viewer.readers.size();
    QLabel* filenameLabel = new QLabel( "<b>groups</b>" );
    m_fileLayout->addWidget( filenameLabel, i++, 0, Qt::AlignLeft | Qt::AlignTop );
    for( FileGroupMap::const_iterator it = m_fileGroups.begin(); it != m_fileGroups.end(); ++it, ++i )
    {
        m_fileLayout->addWidget( new QLabel( ( "\"" + it->first + "\"" ).c_str() ), i, 0, Qt::AlignLeft | Qt::AlignTop );
        CheckBox* viewBox = new CheckBox( boost::bind( &MainWindow::showFileGroup, this, it->first, _1 ) );
        viewBox->setToolTip( ( std::string( "check to make files with fields \"" ) + it->first + "\" visible" ).c_str() );
        m_fileLayout->addWidget( viewBox, i, 1, Qt::AlignRight | Qt::AlignTop );
        m_fileLayout->setRowStretch( i, i + 1 == m_viewer.readers.size() + fields.size() ? 1 : 0 );
    }
}

void MainWindow::toggleFileFrame( bool visible )
{
    m_fileFrameVisible = visible;
    if( visible ) { m_fileFrame->show(); } else { m_fileFrame->hide(); }
}

void MainWindow::closeEvent( QCloseEvent * )
{
    m_viewer.shutdown();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

} } } // namespace snark { namespace graphics { namespace View {
