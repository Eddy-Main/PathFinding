#include "Main.h"
wxIMPLEMENT_APP(Main);

Main::Main()
{
}


Main::~Main()
{
	// Seems like, wxwidgets already delete it by itself.
	// m_frame->Destroy();
}

bool Main::OnInit()
{
	m_frame = new App();
	m_frame->Show();
	return true;
}
