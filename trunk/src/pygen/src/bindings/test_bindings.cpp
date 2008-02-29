// This file has been generated by Py++.

#include "boost/python.hpp"

#include "app/generatorapp.h"
#include "../app/pyapp.h"

using namespace boost::python;
using namespace ft;

BOOST_PYTHON_MODULE(pygen){

    def("runApp", runApp);
    def("printSomeText", printSomeText);

    class_< GeneratorApp >( "GeneratorApp" )    
        .def( init< >() )    
        .def( "printSomeText", &GeneratorApp::printSomeText );
}

