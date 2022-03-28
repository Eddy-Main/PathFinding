#pragma once

#include "App.h"

class Main : public wxApp
{
public:
	Main();
	~Main();

private :
	App* m_frame = nullptr;

public:
	virtual bool OnInit();
};

