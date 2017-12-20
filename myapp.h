

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "imageframe.h"


class CapApp: public wxApp
{
public:
    virtual bool OnInit();
};

/* TODO
	make capframe work with fits objects directly.

*/
class CapFrame: public wxFrame
{
public:
    CapFrame();
        wxBitmap *bitmap_;

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    ImageWindow* image_window_;

};
enum
{
    ID_Hello = 1
};