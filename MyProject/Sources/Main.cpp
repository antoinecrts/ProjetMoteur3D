/*
*	(c) XLim, UMR-CNRS
*	Authors: G.Gilet
*/

#pragma comment(lib,"glew32.lib")

#include <iostream>
#include <cstdlib>
#include <sstream>

#include "GoblimApplication.h"

using namespace std;

int main(int argc, char ** argv) 
{
    try
    {
        GobLimApplication app;
        app.mainLoop();
    } 
    catch(const std::runtime_error &e) 
    {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        cout << error_msg << endl;
        return -1;
    }
    return 0;
}
