#pragma once

#include <wx/spinctrl.h>

#include <algorithm>
using std::min;

#include <string>
using std::string;
using std::to_string;

#include <list>
using std::list;
   

#include "Board.h"
#include "Legend.h"

// Tried to remove the execution time of Board::SleepTime from the chrono,
// but I wasn't able to accurately measure it.

// #define timerWithoutSleep


class App : public wxFrame
{
public:
	App();
	~App();

public :
	vector<Board*> myBoards;
	wxButton* startBtn;
	vector<wxComboBox*> algorithmBoxes;
	vector<wxComboBox*> directionBoxes;
	wxSpinCtrl* randomSpin;
	int sleepTime = 0;
	vector<string> stopwatch = { "", "" };
	vector<int> visitedCellCounts = { 0,0 };
	vector<bool> threadsDone = { false, false };
	wxStaticText* resultsLabelA;
	wxStaticText* resultsLabelB;


	void OnStart(wxCommandEvent &evt);
	void OnExit(wxCommandEvent &evt);

	void concurrentUpdate();
	void callAlgorithm(int boardIndex, int algorithm, int direction);
	float distance(Node* A, Node* B);
	float heuristique(Node* A, Node* B);
	void algorithmAStar(Board* board, int *visitedCellCounts);
	void algorithmBFS(Board* board, int *visitedCellCounts);
	void algorithmDijkstra(Board* board, int *visitedCellCounts);
	void colorPath(Board* board);

	void mazeGenerator(wxCommandEvent &evt);
	void randomPointsGenerator(wxCommandEvent &evt);

	void OnResize(wxSizeEvent& evt);
	void changeNodeSize(wxCommandEvent &evt);
	void changeSleepTime(wxCommandEvent &evt);

	void OnBoardMousePress(wxMouseEvent &evt);

	void resetBoard(wxCommandEvent &evt);

	void showLegend(wxCommandEvent &evt);




	wxDECLARE_EVENT_TABLE();
};

