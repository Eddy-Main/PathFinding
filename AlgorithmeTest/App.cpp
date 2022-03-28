#include "App.h"

// Menu events
BEGIN_EVENT_TABLE(App, wxFrame)
	EVT_MENU(wxID_EXIT, App::OnExit)
	EVT_MENU(wxID_HELP, App::showLegend)
END_EVENT_TABLE()


// Creates the main window of the App, including its components.
App::App() : wxFrame(nullptr, wxID_ANY, "Application A*",wxPoint(0,0),wxSize(1066,511))
{
	// Widgets of the App, includes : Button, TextLabel, ComboBox, SpinCtrl.
	// Buttons 
	wxSize*  startSize = new wxSize(100, 20);
	startBtn = new wxButton(this, wxID_ANY, "Start", wxDefaultPosition, *startSize);
	startBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &App::OnStart, this);

	wxStaticText* mazeGenLabel = new wxStaticText(this, wxID_ANY, "Random Maze Generator");
	wxSize*  mazeSize = new wxSize(100, 20);
	wxButton* mazeBtn = new wxButton(this, wxID_ANY, "Maze Generate", wxDefaultPosition, *mazeSize);
	mazeBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &App::mazeGenerator, this);

	wxStaticText* randomPointsLabel = new wxStaticText(this, wxID_ANY, "Random Points Generator");
	wxSize*  randomPointsSize = new wxSize(100, 20);
	wxButton* randomPointsBtn = new wxButton(this, wxID_ANY, "Random Points", wxDefaultPosition, *randomPointsSize);
	randomPointsBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &App::randomPointsGenerator, this);

	wxSize*  resetSize = new wxSize(100, 20);
	wxButton* resetBtn = new wxButton(this, wxID_ANY, "Reset Board", wxDefaultPosition, *resetSize);
	resetBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &App::resetBoard, this);



	// Spin Ctrl
	wxSize* randomSpinSize = new wxSize(60, 20);
	randomSpin = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, *randomSpinSize);
	randomSpin->SetMax(100);

	wxStaticText* sleepTimeLabel = new wxStaticText(this, wxID_ANY, "Update Rate (ms)");
	wxSize*  sleepTimeSize = new wxSize(60, 20);
	wxSpinCtrl* sleepTimeSpin = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, *sleepTimeSize);
	sleepTimeSpin->SetMax(40);
	sleepTimeSpin->Bind(wxEVT_TEXT, &App::changeSleepTime, this);

	wxStaticText* nodeSizeLabel = new wxStaticText(this, wxID_ANY, "Cell Size (px)");
	wxSize*  nodeSizeSize = new wxSize(60, 20);
	wxSpinCtrl* nodeSizeSpin = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, *nodeSizeSize);
	nodeSizeSpin->SetValue(20);
	nodeSizeSpin->SetMin(10);
	nodeSizeSpin->SetMax(40);
	nodeSizeSpin->Bind(wxEVT_TEXT, &App::changeNodeSize, this);



	// ComboBox

	wxStaticText* algoLabelA = new wxStaticText(this, wxID_ANY, "Algorithm");
	wxSize*  comboASize = new wxSize(100, 20);
	wxArrayString* algorithmAList = new wxArrayString();
	algorithmAList->Add("A*");	 // A Star
	algorithmAList->Add("BFS");   // Breadth First Search
	algorithmAList->Add("Dijkstra");   // Depth First Search

	wxComboBox* algorithmABox = new wxComboBox(this, wxID_ANY, "--Select", wxDefaultPosition, *comboASize, *algorithmAList);
	algorithmABox->SetSelection(0);


	wxStaticText* algoLabelB = new wxStaticText(this, wxID_ANY, "Algorithm");
	wxSize*  comboBSize = new wxSize(100, 20);
	wxArrayString* algorithmBList = new wxArrayString();
	algorithmBList->Add("A*");	 // A Star
	algorithmBList->Add("BFS");   // Breadth First Search
	algorithmBList->Add("Dijkstra");   // Depth First Search
	algorithmBList->Add("None");   // For Solo board run.

	wxComboBox* algorithmBBox = new wxComboBox(this, wxID_ANY, "--Select", wxDefaultPosition, *comboBSize, *algorithmBList);
	algorithmBBox->SetSelection(1);

	algorithmBoxes.push_back(algorithmABox);
	algorithmBoxes.push_back(algorithmBBox);

	wxStaticText* directionLabelA = new wxStaticText(this, wxID_ANY, "Direction");
	wxSize*  directionComboASize = new wxSize(100, 20);
	wxArrayString* directionList = new wxArrayString();
	directionList->Add("4-way");	 // + shaped directions
	directionList->Add("8-way");	 // 8 way directions

	wxComboBox* directionABox = new wxComboBox(this, wxID_ANY, "--Select", wxDefaultPosition, *directionComboASize, *directionList);
	directionABox->SetSelection(1);


	wxStaticText* directionLabelB = new wxStaticText(this, wxID_ANY, "Direction");
	wxSize*  directionComboBSize = new wxSize(100, 20);
	wxComboBox* directionBBox = new wxComboBox(this, wxID_ANY, "--Select", wxDefaultPosition, *directionComboBSize, *directionList);
	directionBBox->SetSelection(1);

	directionBoxes.push_back(directionABox);
	directionBoxes.push_back(directionBBox);


	// Lone Text Labels

	wxSize*  labelSize = new wxSize(131, 30);
	resultsLabelA = new wxStaticText(this, wxID_ANY, "Exec Time A : \nCell Count A :", wxDefaultPosition, *labelSize);// , *nodeSizePos, *nodeSizeSize);
	resultsLabelB = new wxStaticText(this, wxID_ANY, "Exec Time B : \nCell Count B :", wxDefaultPosition, *labelSize);// , *nodeSizePos, *nodeSizeSize);



	// Widget Sizers
	// Boards
	wxStaticBoxSizer* boardASetupBox = new wxStaticBoxSizer(wxVERTICAL, this, "Board A");
	boardASetupBox->Add(algoLabelA, 0, wxWEST | wxNORTH | wxEAST, 5);
	boardASetupBox->Add(algorithmABox, 0, wxSOUTH, 10);
	boardASetupBox->Add(directionLabelA, 0, wxWEST | wxEAST, 5);
	boardASetupBox->Add(directionABox, 0, wxSOUTH, 5);

	wxStaticBoxSizer* boardBSetupBox = new wxStaticBoxSizer(wxVERTICAL, this, "Board B");
	boardBSetupBox->Add(algoLabelB, 0, wxWEST | wxNORTH | wxEAST, 5);
	boardBSetupBox->Add(algorithmBBox, 0, wxSOUTH, 10);
	boardBSetupBox->Add(directionLabelB, 0, wxWEST | wxEAST, 5);
	boardBSetupBox->Add(directionBBox, 0, wxSOUTH, 5);


	// Launch Setup
	wxBoxSizer* sleepTimeSizer = new wxBoxSizer(wxVERTICAL);
	sleepTimeSizer->Add(sleepTimeLabel);
	sleepTimeSizer->Add(sleepTimeSpin);

	wxBoxSizer* nodeSizeSizer = new wxBoxSizer(wxVERTICAL);
	nodeSizeSizer->Add(nodeSizeLabel);
	nodeSizeSizer->Add(nodeSizeSpin);

	wxBoxSizer* setupCol1Layout = new wxBoxSizer(wxVERTICAL);
	setupCol1Layout->Add(boardASetupBox, 0, wxALL, 5);
	setupCol1Layout->Add(sleepTimeSizer, 0, wxALL, 5);

	wxBoxSizer* setupCol2Layout = new wxBoxSizer(wxVERTICAL);
	setupCol2Layout->Add(boardBSetupBox, 0, wxALL, 5);
	setupCol2Layout->Add(nodeSizeSizer, 0, wxALL, 5);

	wxStaticBoxSizer* mainSetupBox = new wxStaticBoxSizer(wxHORIZONTAL, this, "Launch Setup");
	mainSetupBox->Add(setupCol1Layout, 1, wxALIGN_CENTER);
	mainSetupBox->Add(setupCol2Layout, 1, wxALIGN_CENTER);


	// Optional Function
	wxBoxSizer* mazeGenSizer = new wxBoxSizer(wxVERTICAL);
	mazeGenSizer->Add(mazeGenLabel);
	mazeGenSizer->Add(mazeBtn);

	wxBoxSizer* randomSpinSizer = new wxBoxSizer(wxHORIZONTAL);
	randomSpinSizer->Add(randomPointsBtn);
	randomSpinSizer->Add(randomSpin, 0, wxWEST, 30);

	wxBoxSizer* randomPointSizer = new wxBoxSizer(wxVERTICAL);
	randomPointSizer->Add(randomPointsLabel);
	randomPointSizer->Add(randomSpinSizer);

	wxStaticBoxSizer* optionFuncBox = new wxStaticBoxSizer(wxVERTICAL, this, "Optional Function");
	optionFuncBox->Add(mazeGenSizer, 0, wxALL, 5);
	optionFuncBox->Add(randomPointSizer, 0, wxALL, 5);


	// Execution Results

	wxStaticBoxSizer* resultsBox = new wxStaticBoxSizer(wxHORIZONTAL, this, "Execution Results");
	resultsBox->Add(resultsLabelA, 0, wxALL, 5);
	resultsBox->Add(resultsLabelB, 0, wxALL, 5);

	wxBoxSizer* startCancelBtnsSizer = new wxBoxSizer(wxHORIZONTAL);
	startCancelBtnsSizer->Add(startBtn, 1, wxALL, 5);
	startCancelBtnsSizer->Add(resetBtn, 1, wxALL, 5);

	wxColor frameBgColor = this->GetBackgroundColour();
	wxSize*  defaultBoardSize = new wxSize(421, 421);
	Board* myBoardA = new Board(21, 21, nodeSizeSpin->GetValue(), frameBgColor, this, wxID_ANY, wxDefaultPosition, *defaultBoardSize);
	Board* myBoardB = new Board(21, 21, nodeSizeSpin->GetValue(), frameBgColor, this, wxID_ANY, wxDefaultPosition, *defaultBoardSize);
	myBoardA->Bind(wxEVT_SIZE, &App::OnResize, this, wxID_ANY);


	wxBoxSizer* allWidgetsSizer = new wxBoxSizer(wxVERTICAL);
	allWidgetsSizer->Add(mainSetupBox, 0, wxEXPAND | wxALL, 5);
	allWidgetsSizer->Add(optionFuncBox, 0, wxEXPAND | wxALL, 5);
	allWidgetsSizer->Add(resultsBox, 0, wxEXPAND | wxALL, 5);
	allWidgetsSizer->Add(startCancelBtnsSizer, 0, wxALIGN_CENTER|wxALL, 5);


	// App's main sizer
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	mainSizer->Add(myBoardA, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(myBoardB, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(allWidgetsSizer, 0, wxALIGN_CENTER | wxRIGHT, 5);


	SetSizer(mainSizer);
	myBoards.push_back(myBoardA);
	myBoards.push_back(myBoardB);

	// Would have loved to use the '|' operator, but can't.
	// As EVT_LEFT/RIGHT_DOWN is only executed once when the user press the MouseBtn,
	// had to add wxEVT_MOTION as well.
	for (Board* board : myBoards) {
		board->Bind(wxEVT_LEFT_DOWN, &App::OnBoardMousePress, this);
		board->Bind(wxEVT_RIGHT_DOWN, &App::OnBoardMousePress, this);
		board->Bind(wxEVT_MOTION, &App::OnBoardMousePress, this);
	}

	// Menu bar
	wxMenuBar* menuBar = new wxMenuBar();
	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_HELP, L"&Commands", L"Shows the legend of the application");
	helpMenu->AppendSeparator();
	helpMenu->Append(wxID_EXIT, L"&Quit", L"Quit the application");
	menuBar->Append(helpMenu, L"&Info");

	SetMenuBar(menuBar);

	// Window parameters
	wxSize*  minSize = new wxSize(550, 511);
	this->SetMinSize(*minSize);

	// Deletes
	delete labelSize;
	delete algorithmAList;
	delete algorithmBList;
	delete directionList;
	delete defaultBoardSize;
	delete minSize;

	delete startSize;
	delete mazeSize;
	delete  randomSpinSize;
	delete  randomPointsSize;

	delete resetSize;
	delete comboASize;
	delete comboBSize;
	delete directionComboASize;
	delete directionComboBSize;
	delete sleepTimeSize;
	delete nodeSizeSize;
}


App::~App()
{
	delete myBoards[0];
	delete myBoards[1];
	myBoards.clear();
	delete startBtn;
	delete algorithmBoxes[0];
	delete algorithmBoxes[1];
	algorithmBoxes.clear();
	delete directionBoxes[0];
	delete directionBoxes[1];
	directionBoxes.clear();
	stopwatch.clear();
	threadsDone.clear();
	delete resultsLabelA;
	delete resultsLabelB;
}

// Launch the pathfinding algorithms according to the set.
void App::OnStart(wxCommandEvent &evt)
{
	// Sends 2 threads, one for both the boards.
	vector<thread> threads;
	for (int i = 0; i < 2; i++) {
		threadsDone[i] = false;
		int algorithm = algorithmBoxes[i]->GetSelection();
		int direction = directionBoxes[i]->GetSelection();

		threads.push_back(thread(&App::callAlgorithm, this, i, algorithm, direction));

	}

	// Update both boards at a set interval.
	concurrentUpdate();

	// Wait until both algorithm finishes.
	for (int i = 0; i < 2; i++)
		threads[i].join();

	// Update timer Text
	resultsLabelA->SetLabel("Exec Time A : " + stopwatch[0] + "\nCell Count A : " + to_string(visitedCellCounts[0]));
	resultsLabelB->SetLabel("Exec Time B : " + stopwatch[1] + "\nCell Count B : " + to_string(visitedCellCounts[1]));

	for (Board* board : myBoards) {
		board->Update();
		board->Refresh();
	}

	evt.Skip();
}

// Close the application.
void App::OnExit(wxCommandEvent &evt) {
	Close(false);
	evt.Skip();
}


// This function updates both board at a set interval.
// It is required to update boards with this function, as
// wxWidgets strongly recommand not using GUI functions in threads.
// The uses of events between thread is instead recommanded.
// But for a task like this, I deemed that this way was good enough to make it work.
void App::concurrentUpdate() {
	if (sleepTime == 0) return;

#ifdef accurateSleep
	duration<double> minSleepTime(0);
#endif
	while(!threadsDone[0] || !threadsDone[1]){
		for (Board* board : myBoards) {
			board->Update();
			board->Refresh();
		}

#ifdef accurateSleep
		auto start = high_resolution_clock::now();
		while (duration<double>(high_resolution_clock::now() - start).count() < sleepTime / 1e3) {
			sleep_for(minSleepTime);
		}
#else
		sleep_for(milliseconds(sleepTime));
#endif
	}
}

// Execution of an algorithm.
// It counts its execution time and its number of visited cells.
// @params : 
//		boardIndex : 0 or 1		(resp. Board A or Board B)
//		algorithm  : 0, 1 or 2	(resp. A*, BFS or Dijkstra)
//		direction  : 0 or 1		(resp. Four-way or Eight-way)
void App::callAlgorithm(int boardIndex, int algorithm, int direction) {
	// For Single Board Run, save some time.
	myBoards[boardIndex]->resetBoardParams();
	myBoards[boardIndex]->initNeighbours(direction);
		
	int visitedCellCount = 0;
	auto start = high_resolution_clock::now();
	switch (algorithm) {
	case 0:  //A*
		algorithmAStar(myBoards[boardIndex], &visitedCellCount);
		break;
	case 1:  //BFS
		algorithmBFS(myBoards[boardIndex], &visitedCellCount);
		break;
	case 2:  //Dijkstra
		algorithmDijkstra(myBoards[boardIndex], &visitedCellCount);
		break;
	default :
		stopwatch[boardIndex] = "None";
		visitedCellCounts[boardIndex] = 0;
		threadsDone[boardIndex] = true;
		return;
	}

	visitedCellCounts[boardIndex] = visitedCellCount;

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	string timeMsg;

#ifdef timerWithoutSleep
	// Still need to take out the execution time of Sleep().
	int time = duration.count() - ((visitedCellCount * sleepTime)*1e3);
#else
	int time = duration.count();
#endif

	// From microseconds to milliseconds.
	int nbMs = time / 1000;
	int nbUs = time % 1000;

	string strNbUs = to_string(nbUs);
	string decimals(3 - strNbUs.length(), '0');
	decimals += strNbUs;
	
	if (time / 1000000 > 0) 
		timeMsg = to_string(nbMs) + " ms";
	else
		timeMsg = to_string(nbMs) + "." + decimals +" ms";
	

	stopwatch[boardIndex] = timeMsg;

	colorPath(myBoards[boardIndex]);
	threadsDone[boardIndex] = true;
}

// Calculate the distance between two Node.
float App::distance(Node* A, Node* B)
{
	return sqrtf((B->x - A->x)*(B->x - A->x) + (B->y - A->y)*(B->y - A->y));
}

// Heuristique used in the A Star algorithm.
float App::heuristique(Node* A, Node* B)
{
	return distance(A, B);
}

// The A star algorithm, which uses the distance between each node and the end node to
// find the shortest path.
// @params :
//		board				: the board on which the algorithm will be executed.
//		visitedCellCount	: the number of cells visited during the execution of the algorithm.
void App::algorithmAStar(Board* board, int *visitedCellCount)
{
	int nbVisitedCell = 0;
	Node *currentNode = board->beginNode;
	board->beginNode->localGoal = 0;
	board->beginNode->globalGoal = heuristique(board->beginNode, board->endNode);

	// Add the starting node to the list.
	list<Node*> listNotTestedNode;
	listNotTestedNode.push_back(currentNode);

	while (!listNotTestedNode.empty() && currentNode != board->endNode)
	{
		// Sort the list of nodes by distance from current node to the end node.
		listNotTestedNode.sort([](Node* A, Node* B) {
			return A->globalGoal < B->globalGoal; 
		});

		// In case there is no solutions.
		if (listNotTestedNode.empty()) 
			break;

		currentNode = listNotTestedNode.front();
		currentNode->isVisited = true;

		// Remove the all occurence of Node in the list.
		listNotTestedNode.remove(currentNode);

		if (currentNode != board->beginNode && currentNode != board->endNode)
			currentNode->brushColor = wxColor(255,215,0); //ORANGE

		for (Node* neighbourNode : currentNode->vecNeighbours)
		{
			if (!neighbourNode->isObstacle && !neighbourNode->isVisited){
				if(neighbourNode != board->beginNode && neighbourNode != board->endNode)
					neighbourNode->brushColor = *wxYELLOW;

				// Evaluate current path.
				float possibleLowerGoal = currentNode->localGoal + distance(currentNode, neighbourNode);

				// if the current path is shorter than the previous one, update the value
				// And add the node to the list.
				if (possibleLowerGoal < neighbourNode->localGoal)
				{
					neighbourNode->parent = currentNode;
					neighbourNode->localGoal = possibleLowerGoal;

					neighbourNode->globalGoal = neighbourNode->localGoal + heuristique(neighbourNode, board->endNode);
					listNotTestedNode.push_back(neighbourNode);
				}
			}
		}
		nbVisitedCell++;
		
		// Sleep time at each visited node.
		board->Sleep(sleepTime);
	}
	*visitedCellCount = nbVisitedCell;
}

// The BFS algorithm, which is using the first in first out way of thinking.
// @params :
//		board				: the board on which the algorithm will be executed.
//		visitedCellCount	: the number of cells visited during the execution of the algorithm.
void App::algorithmBFS(Board* board, int *visitedCellCount) {
	int sleepCalledTimes = 0;

	Node* currentNode = board->beginNode;
	currentNode->isVisited = true;
	list<Node*> listNotTestedNode;
	listNotTestedNode.push_back(currentNode);

	while (!listNotTestedNode.empty() && currentNode != board->endNode) {
		// Take and remove the first node from the list.
		currentNode = listNotTestedNode.front(); 
		listNotTestedNode.pop_front();

		if (currentNode != board->beginNode && currentNode != board->endNode)
			currentNode->brushColor = wxColor(255, 215, 0); //ORANGE
		

		// Add neighbours at the end of the list.
		for (Node* neighbourNode : currentNode->vecNeighbours)
		{
			if (!neighbourNode->isObstacle && !neighbourNode->isVisited) {
				neighbourNode->parent = currentNode;
				neighbourNode->isVisited = true;
				listNotTestedNode.push_back(neighbourNode);
				if (neighbourNode != board->beginNode && neighbourNode != board->endNode)
					neighbourNode->brushColor = *wxYELLOW; //YELLOW
			}
		}
		sleepCalledTimes++;

		// Sleep time at each visited node.
		board->Sleep(sleepTime);
	}
	*visitedCellCount = sleepCalledTimes;
}

// The Dijkstra algorithm, which uses the distance between each node and the starting node to
// find the shortest path.
// @params :
//		board				: the board on which the algorithm will be executed.
//		visitedCellCount	: the number of cells visited during the execution of the algorithm.
void App::algorithmDijkstra(Board* board, int *visitedCellCount) {
	int sleepCalledTimes = 0;
	Node *currentNode = board->beginNode;
	board->beginNode->localGoal = 0;

	// Add the starting node to the list.
	list<Node*> listNotTestedNode;
	listNotTestedNode.push_back(currentNode);

	while (!listNotTestedNode.empty() && currentNode != board->endNode)
	{
		// Sort the list of nodes by distance from current node to the starting node.
		listNotTestedNode.sort([](Node* A, Node* B) {
			return A->localGoal < B->localGoal;
		});

		// In the case where there is no solution.
		if (listNotTestedNode.empty())
			break;

		currentNode = listNotTestedNode.front();
		currentNode->isVisited = true;

		// Remove the all occurence of Node in the list.
		listNotTestedNode.remove(currentNode);

		if (currentNode != board->beginNode && currentNode != board->endNode)
			currentNode->brushColor = wxColor(255, 215, 0); //ORANGE

		for (Node* neighbourNode : currentNode->vecNeighbours)
		{
			if (!neighbourNode->isObstacle && !neighbourNode->isVisited) {
				if (neighbourNode != board->beginNode && neighbourNode != board->endNode)
					neighbourNode->brushColor = *wxYELLOW; //YELLOW

				// Evaluate current path.
				float possibleLowerGoal = currentNode->localGoal + distance(currentNode, neighbourNode);

				// if the current path is shorter than the previous one, update the value
				// And add the node to the list.
				if (possibleLowerGoal < neighbourNode->localGoal)
				{
					neighbourNode->parent = currentNode;
					neighbourNode->localGoal = possibleLowerGoal;
					listNotTestedNode.push_back(neighbourNode);
				}
			}
		}
		sleepCalledTimes++;

		// Sleep time at each visited node.
		board->Sleep(sleepTime);
	}
	*visitedCellCount = sleepCalledTimes;
}

// Color the path found by the algorithm.
void App::colorPath(Board* board)
{
	int nWidth = board->width;
	int nHeight = board->height;

	if (board->endNode != nullptr)
	{
		Node *path = board->endNode;

		while (path->parent != board->beginNode && path->parent != nullptr)
		{
			path = path->parent;
			board->listNodes[path->x][path->y]->brushColor = *wxBLUE;
			board->Sleep(sleepTime);
		}
	}
}


// Create a random maze.
void App::mazeGenerator(wxCommandEvent &evt) {
	int nWidth = myBoards[0]->width;
	int nHeight = myBoards[0]->height;


	srand(time(NULL));
	// It seems like, the first rand() always give similar numbers for Windows users...
	// Just wasting the first call might not be a greeeeeat solution, but it seems to work somewhat. 
	rand();


	int randBeginNodeX;
	int randBeginNodeY;
	int randEndNodeX;
	int randEndNodeY;
	int oddOrEven;
	do {
		randBeginNodeX = rand() % nWidth;
		randBeginNodeY = rand() % nHeight;
		randEndNodeX = rand() % nWidth;
		randEndNodeY = rand() % nHeight;

		if (((randBeginNodeX + randEndNodeX) % 2) == 1 || ((randBeginNodeY + randEndNodeY) % 2) == 1)
			oddOrEven = 1;
		else oddOrEven = 0;
	} while ((oddOrEven == 1) ||
		((randBeginNodeX == randEndNodeX) && (randBeginNodeY == randEndNodeY))); // BeginNode and EndNode must be different.
	// I require a random beginNode and EndNode which are separated by an odd number of rows and column.
	// So that it doesn't mess up the maze generation.


	// Reset both board
	for (Board* board : myBoards) {
		board->clearBoard();// *wxBLACK);

		board->beginNode = board->listNodes[randBeginNodeX][randBeginNodeY];
		board->endNode = board->listNodes[randEndNodeX][randEndNodeY];
		board->beginNode->brushColor = *wxGREEN;
		board->endNode->brushColor = *wxRED;
	}

	// Start at the begin Node.
	vector<Node*> stackNode;
	stackNode.push_back(myBoards[0]->beginNode);

	while (!stackNode.empty()) {
		Node* currentNode = stackNode.back();

		for (Board* board : myBoards)
			board->listNodes[currentNode->x][currentNode->y]->isVisited = true;

		// Add all non visited neighbours to the vector.
		vector<vector<Node*>> randomNeighbourYetVisited;
		for (vector<Node*> vecNeighbourNode : currentNode->vecMazeNeighbours) {
			if (vecNeighbourNode[1]->isVisited == false) {
				randomNeighbourYetVisited.push_back(vecNeighbourNode);
			}
		}

		// In the case of a dead end.
		if (randomNeighbourYetVisited.empty())
			stackNode.pop_back();
		else {
			// Randomize the neighbours.
			random_shuffle(randomNeighbourYetVisited.begin(), randomNeighbourYetVisited.end());

			// Add neighbours to the stack.
			stackNode.push_back(randomNeighbourYetVisited.back()[1]);

			// "Marking" the walls of the maze.
			Node* visitedNode = randomNeighbourYetVisited.back()[0];
			for (Board* board : myBoards)
				board->listNodes[visitedNode->x][visitedNode->y]->isVisited = true;
		}

	}

	for (Board* board : myBoards) {
		//"Filling" the walls of the maze.
		board->fillMazeSpots();
		board->Update();
		board->Refresh();
	}
	evt.Skip();

}

// Changes a number of available cells into Obstacles.
// The number depends on SpinCtrl randomSpin and the number of available cells.
void App::randomPointsGenerator(wxCommandEvent &evt) {

	// Number from SpinCtrl
	int nbAvailableCells = myBoards[0]->getAvailableCells();

	// Takes the minimum between SpinCtrl's value and the number of available cell.
	int nbPoints = min(nbAvailableCells, randomSpin->GetValue());


	srand(time(NULL));
	// It seems like, the first rand() always give similar numbers for Windows users...
	// Just wasting the first call might not be a great solution, but it seems to work somewhat. 
	rand();

	int i = 0;
	while (i < nbPoints) {
		int randX = rand() % myBoards[0]->width;
		int randY = rand() % myBoards[0]->height;

		Node* randomNode = myBoards[0]->listNodes[randX][randY];

		if (!randomNode->isObstacle && randomNode != myBoards[0]->beginNode && randomNode != myBoards[0]->endNode) {

			for (Board* board : myBoards) {
				board->listNodes[randX][randY]->isObstacle = true;
				board->listNodes[randX][randY]->brushColor = *wxBLACK;
			}

			i++;
		}
	}

	// Refresh board
	for (Board* board : myBoards) {
		board->Update();
		board->Refresh();
	}

	evt.Skip();
}


// When the window is resized, execute Board::OnResize.
void App::OnResize(wxSizeEvent& evt) {

	wxSize newSize = evt.GetSize();

	for (Board* board : myBoards)
		board->OnResize(&newSize);
	evt.Skip();
}

// When boards's cells size is modified, reinitialize boards
// with updated size.
void App::changeNodeSize(wxCommandEvent &evt) {
	int nodeSizePixel = evt.GetInt();

	if (myBoards[0]->nodeSize == nodeSizePixel) return;

	for (Board* board : myBoards) {
		board->nodeSize = nodeSizePixel;
		board->OnResize(nullptr);
	}
	evt.Skip();
}

// Change the sleep amount.
void App::changeSleepTime(wxCommandEvent &evt) {
	sleepTime = evt.GetInt();
	evt.Skip();
}


// This function is called when the user is clicking on one of the boards.
// It translate the clic coordinates and call the Board::mouseColorNode() function.
void App::OnBoardMousePress(wxMouseEvent &evt) {

	if (evt.GetEventType() != wxEVT_LEFT_DOWN && evt.GetEventType() != wxEVT_RIGHT_DOWN
		&& evt.GetEventType() != wxEVT_MOTION && evt.GetEventType() != wxEVT_MIDDLE_DOWN) {
		evt.Skip(); return;
	}

	bool isLeftDown = wxGetMouseState().LeftIsDown();
	bool isRightDown = wxGetMouseState().RightIsDown();
	if (evt.GetEventType() == wxEVT_MOTION && (!isLeftDown && !isRightDown)) { evt.Skip(); return; }
	if (isLeftDown && isRightDown) { evt.Skip(); return; }


	int nWidth = myBoards[0]->width;
	int nHeight = myBoards[0]->height;

	int x_clic = evt.GetX();
	int y_clic = evt.GetY();

	int boardX = x_clic / myBoards[0]->nodeSize;
	int boardY = y_clic / myBoards[0]->nodeSize;
	
	if (boardX >= nWidth || boardY >= nHeight) { evt.Skip(); return; }

	for (Board* board : myBoards)
		board->mouseColorNode(boardX, boardY);

	evt.Skip();

}

// Reset both boards.
void App::resetBoard(wxCommandEvent &evt) {
	int nWidth = myBoards[0]->width;
	int nHeight = myBoards[0]->height;

	for (Board* board : myBoards) {
		board->resetBoard();
		board->Refresh();
	}
	evt.Skip();
}

// Shows a window with the legend of this App.
void App::showLegend(wxCommandEvent &evt) {

	wxSize* legendFixedSize = new wxSize(700, 400);
	Legend* legendDialog = new Legend(this, wxID_ANY, wxDefaultPosition, *legendFixedSize);

	legendDialog->ShowModal();

	delete legendDialog;
	delete legendFixedSize;
	evt.Skip();
}
