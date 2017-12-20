

#include "myapp.h"


#include <fits.h>
#include "imageframe.h"

wxIMPLEMENT_APP(CapApp);
bool CapApp::OnInit()
{
    CapFrame *frame = new CapFrame();
    frame->Show( true );
    Fits fit;
    fit.fromFile("300.fit");
    frame->bitmap_ = wxBitmap(fit.getBuffer(),fit.getWidth(),fit.getHeight(),fit.getDepth());
    return true;
}
CapFrame::CapFrame()
        : wxFrame(NULL, wxID_ANY, "CapApp",wxDefaultPosition,wxSize(800,800))
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );
    Bind(wxEVT_MENU, &CapFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &CapFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &CapFrame::OnExit, this, wxID_EXIT);

    this->image_window_ = new ImageWindow( this );
    this->image_window_->SetScrollbars( 10, 10, 100, 240 );
}
void CapFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void CapFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void CapFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}