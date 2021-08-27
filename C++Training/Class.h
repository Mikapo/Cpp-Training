#pragma once

#include <iostream>



class Class
{

public:
	
	int* GetVariablePointer();
	inline void ChangeVariable(int NewVariable) { Variable = NewVariable; };
	inline int GetVariable() const{ return Variable; };

private:

	int Variable = 20;
	
};