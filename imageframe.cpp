
#include "imageframe.h"

ImageWindow::ImageWindow(wxFrame *parent)
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{

}

void ImageWindow::OnPaint(wxPaintEvent &WXUNUSED(event)) {
	wxPaintDC pdc(this);

    wxDC &dc = pdc ;

    if ( wxDynamicCast(&pdc, wxPaintDC) )
    {
        PrepareDC(dc);
    }

	dc.Clear();

	wxBitmap bm("test.bmp",wxBITMAP_TYPE_BMP);
	dc.DrawBitmap(bm,0,0);
    dc.DrawText(wxT("You should see ffthis "), 10, 10);
}

wxBEGIN_EVENT_TABLE(ImageWindow, wxScrolledWindow)
    EVT_PAINT  (ImageWindow::OnPaint)
//    EVT_MOTION (MyCanvas::OnMouseMove)
//    EVT_LEFT_DOWN (MyCanvas::OnMouseDown)
//    EVT_LEFT_UP (MyCanvas::OnMouseUp)
wxEND_EVENT_TABLE()


