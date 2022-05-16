#pragma once

#include "wx/wx.h"
#include "wx/dcbuffer.h"

#include <thread>
using std::thread;

#include <vector>
using std::vector;

#include <chrono>
using std::this_thread::sleep_for;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;


#define accurateSleep true


struct Node
{
	int x;
	int y;
	vector<Node*> vecNeighbours;

	vector<vector<Node*>> vecMazeNeighbours;

	bool isObstacle = false;
	bool isVisited = false;
	Node* parent;
	float localGoal;
	float globalGoal;


	wxColor brushColor = *wxLIGHT_GREY;

	bool operator !=(const Node B)
	{
		if (x != B.x || y != B.y)
			return true;
		return false;
	}

	bool operator ==(const Node B)
	{
		if (x != B.x || y != B.y)
			return false;
		return true;
	}
};


class Board : public wxPanel
{
public :
	Board(
		int nWidth,
		int nHeight,
		int nodeSizePixel,
		wxColor frameBgColor,
		wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize
	);

	~Board();

	int width;
	int height;

	int nodeSize;
	vector<vector<Node*>> listNodes;
	Node* beginNode;
	Node* endNode;

public :
	void initBoard();
	void initNeighbours(int directions);
	void initMazeNeighbours();

	void deallocateNodes();
	void resetBoardParams();
	void resetBoard();
	void clearBoard(wxColor color = *wxLIGHT_GREY);

	void fillMazeSpots();
	int getAvailableCells();


	void PaintAll(wxPaintEvent& evt);
	void mouseColorNode(int x, int y, bool isLeftDown, bool isRightDown);
	void OnResize(wxSize* newSize);
	void Sleep(int time);
	void OnEraseBackground(wxEraseEvent &event);

	wxDECLARE_EVENT_TABLE();
};
