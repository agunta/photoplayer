// MAF_PP.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "..\PhotoPlayer\src\getimage.h"
#include <fstream>
#include <string>



using namespace MAF_PP;
using namespace std;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	
	
	// Create the main window and run it

	Application::Run(gcnew Form1());
	return 0;
	

	
}
