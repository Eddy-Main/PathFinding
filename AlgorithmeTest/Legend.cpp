#include "Legend.h"



BEGIN_EVENT_TABLE(Legend, wxDialog)
EVT_CLOSE(Legend::OnExit)
EVT_PAINT(PaintAll)
EVT_ERASE_BACKGROUND(OnEraseBackground)
END_EVENT_TABLE()


// Creates the Legend pannel window.
Legend::Legend(
	wxWindow *parent,
	wxWindowID winid,
	const wxPoint& pos,
	const wxSize& size) : wxDialog(parent,winid,"Legend", wxDefaultPosition, size)
{
	// Positions of the colored square.
	vector<int> squareXList = { 90, 90, 370, 370 , 40, 290, 440 };
	vector<int> squareYList = { 40, 140, 40, 140, 240, 240, 240 };
	
	// Color of squares.
	squareColorList = { *wxGREEN,*wxRED,*wxBLACK,*wxLIGHT_GREY,*wxYELLOW, wxColor(255,215,0),*wxBLUE };

	// Description of each colors.
	vector<string> squareLabel =
	{
	"Starting point of our algorithms.\nCtrl + LeftClick",
	"Ending point of our algorithms\nAlt + LeftClick",
	"Place an Obstacle on the current cell.\nLeftClick",
	"Available cell for the algorithm to move on.\nRightClick",
	"Neighbours of visited cells, which hasn't been checked yet",
	"Visited cells.",
	"Calculated shortest path from starting point to ending point."
	};

	// Size of text descriptions.
	vector<wxSize> squareLabelSize = { wxSize(240, 50), wxSize(240, 50),
								  wxSize(240, 50), wxSize(240, 50),
							wxSize(170, 50), wxSize(70,50), wxSize(170, 50) };
	
	

	vector<wxStaticText*> squareLabels;
	for (int i = 0; i < squareXList.size(); i++) {
		squareList.push_back(new wxRect(squareXList[i], squareYList[i], 21, 21));
		
		squareLabels.push_back(new wxStaticText(this, wxID_ANY, squareLabel[i],
		wxPoint(squareXList[i] + 30, squareYList[i]-5), squareLabelSize[i]));
	}

	wxStaticText* noteResize = new wxStaticText(this, wxID_ANY, 
		"Note : You can resize the Application's window to add or remove cells from both boards.",
		wxPoint(30, 320), wxSize(500, 30));


	OkBtn = new wxButton(this, wxID_ANY, "Ok", wxPoint(550,320), wxSize(100, 20));
	OkBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Legend::OnOkBtn, this);
}


Legend::~Legend()
{
	delete OkBtn; 
	for (wxRect* square : squareList)
		delete square;

	squareList;
	squareColorList;

}


// Paint the Legend window.
void Legend::PaintAll(wxPaintEvent& evt) {

	wxBufferedPaintDC dc(this);

	dc.Clear();
	for (int i = 0; i < squareList.size(); i++) {
		dc.SetBrush(squareColorList[i]);
		dc.SetPen(*wxBLACK);
		dc.DrawRectangle(*squareList[i]);
	}

	evt.Skip();
}


// Prevent the paint event from having any flickering effect.
void Legend::OnEraseBackground(wxEraseEvent &WXUNUSED(evt)) {
}




// Close window when OK button is pressed.
void Legend::OnOkBtn(wxCommandEvent &evt)
{
	EndModal(wxID_OK);
	evt.Skip();
}

// Close window when the window is closed.
void Legend::OnExit(wxCloseEvent& evt)
{
	EndModal(wxID_OK);
	evt.Skip();
}