#include "Board.h"

wxBEGIN_EVENT_TABLE(Board, wxPanel)
EVT_PAINT(PaintAll)
EVT_ERASE_BACKGROUND(OnEraseBackground)
wxEND_EVENT_TABLE()

// Creates a nWidth x nHeight array of cells.
Board::Board(
	const int nWidth,
	const int nHeight,
	const int nodeSizePixel,
	wxColor frameBgColor,
	wxWindow *parent,
	wxWindowID winid,
	const wxPoint& pos,
	const wxSize& size) : wxPanel(parent, winid, pos, size)
{
	// Default parameters
	wxSize*  minSize = new wxSize(0, -1);
	this->SetMinSize(*minSize);
	width = nWidth;
	height = nHeight;
	this->SetBackgroundColour(frameBgColor);
	nodeSize = nodeSizePixel;

	// Initialization of the board.
	initBoard();
	initMazeNeighbours();

	// Default starting and ending point.
	beginNode = listNodes[width / 4][height / 2];
	beginNode->brushColor = *wxGREEN;
	endNode = listNodes[3 * width / 4][height / 2];
	endNode->brushColor = *wxRED;

	delete minSize;

}


Board::~Board()
{
	deallocateNodes();
}


//Initialize the Board with Nodes.
void Board::initBoard() {
	
	for (int x = 0; x < width; x++)
	{
		vector<Node*> boardRow;
		for (int y = 0; y < height; y++)
		{
			Node* newNode = new Node();
			newNode->x = x;
			newNode->y = y;
			boardRow.push_back(newNode);
		}
		listNodes.push_back(boardRow);
	}
}


// Initialize each node's neighbours.
// 
// i.e. : 
//	Four ways :					Eight ways :
//				N						N N N
//			  N C N						N C N
//				N						N N N

void Board::initNeighbours(int directions) {

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Neigbour for Pathfinding Algorithms
			if(!listNodes[x][y]->vecNeighbours.empty())
				listNodes[x][y]->vecNeighbours.clear();

			switch (directions) {
				// Four way directions
				case 0: {
					if (y > 0)			 // Up
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x][y - 1]);
					if (x < width - 1)	 // Right
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x + 1][y]);
					if (y < height - 1)  // Down
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x][y + 1]);
					if (x > 0)			 // Left
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x - 1][y]);
					break;
				}

				// Eight way directions
				case 1: {
					if (y > 0)	// Up
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x][y - 1]);
					if (x < width - 1)	// Right
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x + 1][y]);
					if (y < height - 1)	// Down
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x][y + 1]);
					if (x > 0)  // Left
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x - 1][y]);
					if (y > 0 && x < width - 1) // Up Right
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x + 1][y - 1]);
					if (y < height - 1 && x < width - 1) // Down Right
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x + 1][y + 1]);
					if (y < height - 1 && x>0)	// Down Left
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x - 1][y + 1]);
					if (y > 0 && x > 0)	 // Up Left
						listNodes[x][y]->vecNeighbours.push_back(listNodes[x - 1][y - 1]);
					break;
				}
			}
		}
	}
}


// Initialize each node's maze neighbours.
// It differentiate itself from Board::initNeighbours(), 
// as two neighbour nodes have another node between them as a
// placeholder for a possible Obstacle, creating the maze's walls.
//
// i.e. : 
//
//				N
//				W
//		    N 0 C 0 N
//				W
//				N
//
// C = Current Node, W = Wall, N = Neighbour Node, 0 = Placeholder Node
void Board::initMazeNeighbours() {

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Neigbour for Maze Generation
			if (!listNodes[x][y]->vecMazeNeighbours.empty())
				listNodes[x][y]->vecMazeNeighbours.clear();

			if (y < height - 2) { // Down
				vector<Node*> downVec = { listNodes[x][y + 1], listNodes[x][y + 2] };
				listNodes[x][y]->vecMazeNeighbours.push_back(downVec);
			}
			if (x < width - 2) {  // Right
				vector<Node*> rightVec = { listNodes[x + 1][y], listNodes[x + 2][y] };
				listNodes[x][y]->vecMazeNeighbours.push_back(rightVec);
			}
			if (y > 1) {		  // Up
				vector<Node*> upVec = { listNodes[x][y - 1], listNodes[x][y - 2] };
				listNodes[x][y]->vecMazeNeighbours.push_back(upVec);
			}
			if (x > 1) {		  // Left
				vector<Node*> leftVec = { listNodes[x - 1][y], listNodes[x - 2][y] };
				listNodes[x][y]->vecMazeNeighbours.push_back(leftVec);
			}

		}
	}
}


// Clear the board's node list.
void Board::deallocateNodes() {

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			delete listNodes[x][y];
		}
		listNodes[x].clear();
	}
	listNodes.clear();
}


// Reset the board's all available node paremeters.
// Thus leaving all obstacles on the board.
void Board::resetBoardParams() {

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			listNodes[x][y]->isVisited = false;
			listNodes[x][y]->parent = nullptr;
			listNodes[x][y]->localGoal = INFINITY;
			listNodes[x][y]->globalGoal = INFINITY;

			if (!listNodes[x][y]->isObstacle && listNodes[x][y] != beginNode && listNodes[x][y] != endNode)
				listNodes[x][y]->brushColor = *wxLIGHT_GREY;
		}
	}
}

// Reset the board completely and set default starting and ending node positions.
void Board::resetBoard() {

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			clearBoard();

			beginNode = listNodes[width / 4][height / 2];
			beginNode->brushColor = *wxGREEN;
			endNode = listNodes[3 * width / 4][height / 2];
			endNode->brushColor = *wxRED;

		}
	}
}

// Reset the board completely.
void Board::clearBoard(wxColor color) {

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			listNodes[x][y]->isObstacle = false;
			listNodes[x][y]->brushColor = color;
			listNodes[x][y]->isVisited = false;
			listNodes[x][y]->parent = nullptr;
			listNodes[x][y]->localGoal = INFINITY;
			listNodes[x][y]->globalGoal = INFINITY;

		}
	}
}

// Change all the non-visited nodes to obstacles.
void Board::fillMazeSpots() {

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!listNodes[x][y]->isVisited) {
				listNodes[x][y]->isVisited = true;
				listNodes[x][y]->isObstacle = true;
				listNodes[x][y]->brushColor = *wxBLACK;
			}

		}
	}
}	

// Get the total number of available cells.
int Board::getAvailableCells() {

	int nbAvailableCells = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!listNodes[x][y]->isObstacle && listNodes[x][y]!=beginNode && listNodes[x][y]!=endNode)
				nbAvailableCells++;

		}
	}
	return nbAvailableCells;
}

// Draws the board.
void Board::PaintAll(wxPaintEvent& evt ) {

	wxBufferedPaintDC dc(this);

	dc.Clear();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			dc.SetBrush(listNodes[x][y]->brushColor);
			dc.SetPen(*wxMEDIUM_GREY_PEN);
			dc.DrawRectangle(wxRect(x * nodeSize, y * nodeSize, nodeSize+1, nodeSize+1));
		}
	}

	evt.Skip();
}

// Colors the cell at the (x, y) coordinates according to instructions.
// Instructions :
//		Ctrl + LeftClick --> Moves Starting Node.
//		Alt	 + LeftClick --> Moves Ending Node.
//		LeftClick  --> Place an Obstacle.
//		RightClick --> Remove an Obstacle.
void Board::mouseColorNode(int x, int y) {
	bool isLeftDown = wxGetMouseState().LeftIsDown();
	bool isRightDown = wxGetMouseState().RightIsDown();
	bool isCtrlDown = wxGetKeyState(WXK_CONTROL);
	bool isAltDown = wxGetKeyState(WXK_ALT);


	if (listNodes[x][y] != beginNode && listNodes[x][y] != endNode) {
		if (isCtrlDown && isAltDown)
			wxMessageBox("You can't click a button while pressing both the ctrl and the alt key !!");
		else if (!isCtrlDown && !isAltDown) {
			//LeftClick  --> Place an Obstacle.
			if (isLeftDown) {
				if (listNodes[x][y]->isObstacle == false) {
					listNodes[x][y]->isObstacle = true;
					listNodes[x][y]->brushColor = *wxBLACK;
					RefreshRect(wxRect(x * nodeSize, y * nodeSize, nodeSize + 1, nodeSize + 1));
				}
			}
			//RightClick --> Remove an Obstacle.
			else {
				if (listNodes[x][y]->isObstacle == true) {
					listNodes[x][y]->isObstacle = false;
					listNodes[x][y]->brushColor = *wxLIGHT_GREY;
					RefreshRect(wxRect(x * nodeSize, y * nodeSize, nodeSize + 1, nodeSize + 1));
				}
			}
		}
		//Ctrl + LeftClick --> Moves Starting Node.
		else if (isCtrlDown && isLeftDown) {
			int previousX = beginNode->x;
			int previousY = beginNode->y;

			listNodes[previousX][previousY]->isObstacle = false;
			listNodes[previousX][previousY]->brushColor = *wxLIGHT_GREY;

			beginNode = listNodes[x][y];
			beginNode->isObstacle = false;
			beginNode->brushColor = *wxGREEN;
			RefreshRect(wxRect(previousX * nodeSize, previousY * nodeSize, nodeSize + 1, nodeSize + 1));
			RefreshRect(wxRect(x * nodeSize, y * nodeSize, nodeSize + 1, nodeSize + 1));

		}
		//Alt + LeftClick --> Moves Ending Node.
		else if (isAltDown && isLeftDown) {
			int previousX = endNode->x;
			int previousY = endNode->y;

			listNodes[previousX][previousY]->isObstacle = false;
			listNodes[previousX][previousY]->brushColor = *wxLIGHT_GREY;

			endNode = listNodes[x][y];
			endNode->isObstacle = false;
			endNode->brushColor = *wxRED;

			RefreshRect(wxRect(previousX * nodeSize, previousY * nodeSize, nodeSize + 1, nodeSize + 1));
			RefreshRect(wxRect(x * nodeSize, y * nodeSize, nodeSize + 1, nodeSize + 1));
		}
	}

	return;

}

// Reset board's cells with updates parameters.
// Called when cell or window size is modified.
void Board::OnResize(wxSize* newSize) {
	int nWidth;
	int nHeight;

	// Window resized.
	// Updates number of rows and columns.
	if (newSize)
	{
		nWidth = (newSize->x - 1) / nodeSize;
		nHeight = (newSize->y - 1) / nodeSize;
	}

	// Cell size changed.
	// Updates number of rows and columns. 
	else {
		wxSize currentSize = GetSize();
		nWidth = (currentSize.x - 1) / nodeSize;
		nHeight = (currentSize.y - 1) / nodeSize;
	}

	if (nWidth == width && nHeight == height) return;

	// Reset node list's parameters.
	deallocateNodes();
	width = nWidth;
	height = nHeight;
	initBoard();
	initMazeNeighbours();
	
	beginNode = listNodes[width / 4][height / 2];
	beginNode->brushColor = *wxGREEN;
	endNode = listNodes[3 * width / 4][height / 2];
	endNode->brushColor = *wxRED;

	Update();
	Refresh();
}

// Make the current thread sleep for 'time' milliseconds.
// Used to slow down algorithm display speed.
// However I wasn't able to decrease execution time of high_resolution_clock::now().
// So it may impact on the shown algorithm execution time.
// @params : 
//		time : sleep time in milliseconds
void Board::Sleep(int time) {
	if (time == 0) return;

#ifdef accurateSleep
	duration<double> minSleepTime(0);
	auto start = high_resolution_clock::now();
	while (duration<double>(high_resolution_clock::now() - start).count() < time / 1e3) {
		sleep_for(minSleepTime);
	}
#else
	sleep_for(milliseconds(time));
#endif
}


// Prevent the paint event from having any flickering effect.
void Board::OnEraseBackground(wxEraseEvent &WXUNUSED(evt)) {
}
