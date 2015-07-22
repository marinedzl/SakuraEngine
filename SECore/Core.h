#pragma once

class Core
{
public:
	Core();
	~Core();
	bool Init();
	void Release();
private:
};

extern Core gCore;
