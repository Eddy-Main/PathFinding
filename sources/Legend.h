#pragma once

#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include <vector>
using std::vector;

#include <string>
using std::string;
using std::to_string;

class Legend : public wxDialog
{
public:
	Legend(wxWindow *parent,
		wxWindowID winid,
		const wxPoint& pos,
		const wxSize& size);
	~Legend();

	wxButton* OkBtn;
	vector<wxRect*> squareList;
	vector<wxColor> squareColorList;

public :

	void PaintAll(wxPaintEvent& evt);
	void OnOkBtn(wxCommandEvent &evt);
	void OnExit(wxCloseEvent& evt);

	void OnEraseBackground(wxEraseEvent &event);

	wxDECLARE_EVENT_TABLE();
};