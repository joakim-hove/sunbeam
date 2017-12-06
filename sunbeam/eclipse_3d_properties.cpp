#include <iostream>

#include <opm/parser/eclipse/EclipseState/Eclipse3DProperties.hpp>

#include "sunbeam.hpp"


namespace {

    py::list getitem( const Eclipse3DProperties& p, const std::string& kw) {
        const auto& ip = p.getIntProperties();
        if (ip.supportsKeyword(kw) && ip.hasKeyword(kw))
            return iterable_to_pylist(p.getIntGridProperty(kw).getData());


        std::cout << "Looking for " << kw << std::endl;
        const auto& dp = p.getDoubleProperties();
        std::cout << "dp returned" << std::endl;
        if (dp.supportsKeyword(kw) && dp.hasKeyword(kw)) {
          std::cout << "Starting to construct list" << std::endl;
          const auto& prop = p.getDoubleGridProperty(kw);
          std::cout << "have get Property" << std::endl;
          return iterable_to_pylist(p.getDoubleGridProperty(kw).getData());
        }
        throw key_error( "no such grid property " + kw );
    }

    bool contains( const Eclipse3DProperties& p, const std::string& kw) {
        return
            (p.getIntProperties().supportsKeyword(kw) &&
             p.getIntProperties().hasKeyword(kw))
            ||
            (p.getDoubleProperties().supportsKeyword(kw) &&
             p.getDoubleProperties().hasKeyword(kw))
            ;
    }
    py::list regions( const Eclipse3DProperties& p, const std::string& kw) {
        return iterable_to_pylist( p.getRegions(kw) );
    }

}

void sunbeam::export_Eclipse3DProperties() {

    py::class_< Eclipse3DProperties >( "Eclipse3DProperties", py::no_init )
        .def( "getRegions",   &regions )
        .def( "__contains__", &contains )
        .def( "__getitem__",  &getitem )
        ;

}
