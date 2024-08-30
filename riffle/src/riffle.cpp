/////////////////////////////////////////////////////////////////////////////
// Name:        riffle.cpp
// Purpose:     A little image viewer application
// Author:      Julian Smart
// Modified by: 
// Created:     03/26/05 12:33:53
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "riffle.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/fs_zip.h"

////@begin includes
////@end includes

#include "riffle.h"
#include "riffleutils.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( RiffleApp )
////@end implement app

/*!
 * RiffleApp type definition
 */

IMPLEMENT_CLASS( RiffleApp, wxApp )

/*!
 * RiffleApp event table definition
 */

BEGIN_EVENT_TABLE( RiffleApp, wxApp )

////@begin RiffleApp event table entries
////@end RiffleApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for RiffleApp
 */

RiffleApp::RiffleApp()
{
////@begin RiffleApp member initialisation
////@end RiffleApp member initialisation
    m_helpController = nullptr;
    m_config = nullptr;
}

/*!
 * Initialisation for RiffleApp
 */

bool RiffleApp::OnInit()  //: helpFilePath(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES)
{    
    wxImage::AddHandler( new wxXPMHandler );
    wxImage::AddHandler( new wxPNGHandler );
    wxImage::AddHandler( new wxJPEGHandler );
    wxImage::AddHandler( new wxGIFHandler );

    // Needed for wxHtmlHelpController
    wxFileSystem::AddHandler( new wxZipFSHandler );

    wxString currentDir = wxGetCwd();

    // Use argv to get current app directory
    m_appDir = wxFindAppPath(argv[0], currentDir, _T("RIFFLEDIR"), wxT("Riffle"));


    // Go up a directory if debugging (since exe is in subdirectory)
#ifdef __WXDEBUG__
    m_appDir = wxPathOnly(m_appDir);
#endif
    
    

    // Create a config object that persists for the life of the application
    m_config = new wxConfig(wxT("Riffle"), wxT("Anthemion Software"));

    // Set initial values, if absent
    InitConfig();
    
    wxPathList pathlist;
    bool ret;
    wxString helpFilePath(GetFullAppPath(_T("riffle")));
#if defined(WIN_USE_CHM) && defined(WIN32)
    m_helpController = new wxCHMHelpController();
    m_helpController->Initialize( ((wxFileName)argv[0]).GetPath() + "\\RiffleManual\\riffle.chm");
    m_helpController->LoadFile(((wxFileName)argv[0]).GetPath() + "\\RiffleManual\\riffle.chm");
#else
    m_helpController = new wxHtmlHelpController(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES); //new wxHelpController;
#endif


#if !defined(WIN_USE_CHM) && defined(WIN32)
    helpFilePath = ((wxFileName)argv[0]).GetPath()  + "\\RiffleManual\\riffle.hhp";
    pathlist.Add(((wxFileName)argv[0]).GetPath() + "\\RiffleManual");


#elif !defined(WIN32) 
    helpFilePath = ((wxFileName)argv[0]).GetPath() + "/RiffleManual/riffle.hhp";
    pathlist.Add(((wxFileName)argv[0]).GetPath() + "/RiffleManual");
#endif


#if !defined(WIN_USE_CHM)
    m_helpController->Initialize(helpFilePath);

    m_helpController->UseConfig(wxConfig::Get());
    //bool ret;
    m_helpController->SetTempDir(".");

    //ret = m_helpController->AddBook(helpFilePath);
    //if (!ret)
    //    wxMessageBox("Failed adding book " + helpFilePath);
#endif


    // Get the window position and size
    wxPoint windowPos = wxDefaultPosition;
    wxSize windowSize = wxDefaultSize;
    GetConfig()->Read(wxT("Window/FrameX"), & windowPos.x);
    GetConfig()->Read(wxT("Window/FrameY"), & windowPos.y);
    GetConfig()->Read(wxT("Window/FrameWidth"), & windowSize.x);
    GetConfig()->Read(wxT("Window/FrameHeight"), & windowSize.y);

    RiffleFrame* mainWindow = new RiffleFrame( NULL, ID_FRAME, wxT("Riffle"), windowPos, windowSize );
    mainWindow->Show(true);

    return true;
}

/// Initialize config values
void RiffleApp::InitConfig()
{
    wxConfigInit init(GetConfig());

    init.Init(_T("Window/FrameX"), (long) -1);
    init.Init(_T("Window/FrameY"), (long) -1);
    init.Init(_T("Window/FrameWidth"), (long) -1);
    init.Init(_T("Window/FrameHeight"), (long) -1);
    init.Init(_T("Window/SashPos"), (long) 100);

    init.Init(_T("General/Scale"), (long) 100);

#ifdef __WXWINCE__
    int defaultThumbnailSize = 50;
#else
    int defaultThumbnailSize = 100;
#endif

    init.Init(_T("General/ThumbnailSize"), (long) defaultThumbnailSize);
    init.Init(_T("General/RestoreLastPath"), (bool) true);
    init.Init(_T("General/ZoomIncrement"), (long) 10);
    init.Init(_T("General/CurrentPath"), wxString(wxEmptyString));
}


/*!
 * Cleanup for RiffleApp
 */
int RiffleApp::OnExit()
{
    delete m_config ;
    delete m_helpController;

    return wxApp::OnExit();
}

/// Show help
void RiffleApp::ShowHelp(const wxString& helpTopic)
{
    if (m_helpController)
    {
        if (helpTopic.IsEmpty())
            m_helpController->DisplayContents();
        else
            m_helpController->DisplaySection(helpTopic);
    }
}
// Prepend the current program directory to the name
wxString RiffleApp::GetFullAppPath(const wxString& filename) const
{
    wxString path(m_appDir);
    if (path.Last() != wxFILE_SEP_PATH && filename[0] != wxFILE_SEP_PATH)
        path += wxFILE_SEP_PATH;
    path += filename;
    
    return path;
}

