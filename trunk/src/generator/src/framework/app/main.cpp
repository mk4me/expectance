/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "core/mainapp.h"
#include "generatorapp.h"

//#include <boost/python.hpp>
//using namespace boost::python;

void runApp()
{
    //int argc = 2;
    //char *argv[2];
    //std::string myPath = "Empty";
    //std::string model = "cally.cfg";
    //argv[0] = myPath.cc_str();
    //argv[1] = model.c_str();

    //argv[0] = (char*)malloc(20);
    //argv[1] = (char*)malloc(20);
    //strcpy(argv[0],"");
    //strcpy(argv[1],"cally.cfg");

    ft::Application* app = new ft::GeneratorApp();
    //InitGlutApplication(argc, argv, app);
    delete app;

}

//BOOST_PYTHON_MODULE(generator)
//{
//    def("runApp", runApp);
//}


/**
 * \brief Main entry to application
 *
 * \param int argc - number of arguments
 * \param char *[] argv - list of arguments
 * \return int - result of application
 **/
int main(int argc, char *argv[])
{
    ft::Application* app = new ft::GeneratorApp();
    InitGlutApplication(argc, argv, app);
    delete app;
    return 0;
}
