// This file has been generated by Py++.

#include "boost/python.hpp"

#include "app/generatorapp.h"
#include "../app/pyapp.h"

using namespace boost::python;
using namespace ft;

BOOST_PYTHON_MODULE(libpygen){

    def("runApp", runApp);

    def("setPropStr", setPropStr);
    def("setPropInt", setPropInt);
    def("setPropFloat", setPropFloat);
    def("getPropStr", getPropStr);
    def("getPropInt", getPropInt);
    def("getPropFloat", getPropFloat);
    def("dumpProps", dumpProps);

    def("test", test);
}


