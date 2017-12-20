#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class ImageWindow: public wxScrolledWindow
{
public:
    ImageWindow(wxFrame *parent);
private:
	void OnPaint(wxPaintEvent &WXUNUSED(event));
	wxDECLARE_EVENT_TABLE();

};

