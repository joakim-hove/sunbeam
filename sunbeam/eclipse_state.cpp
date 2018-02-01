#include <opm/parser/eclipse/EclipseState/EclipseState.hpp>
#include <opm/parser/eclipse/EclipseState/Grid/FaultCollection.hpp>

#include "sunbeam.hpp"


namespace {

    py::list getNNC( const EclipseState& state ) {
        py::list l;
        for( const auto& x : state.getInputNNC().nncdata() )
            l.append( py::make_tuple( x.cell1, x.cell2, x.trans )  );
        return l;
    }
    py::list faultNames( const EclipseState& state ) {
        py::list l;
        const auto& fc = state.getFaults();
        for (size_t i = 0; i < fc.size(); i++) {
            const auto& f = fc.getFault(i);
            l.append(f.getName());
        }
        return l;
    }
    py::dict jfunc( const EclipseState& s) {
      const auto& tm = s.getTableManager();
      if (!tm.useJFunc())
        return py::dict();
      const auto& j = tm.getJFunc();
      std::string flag = "BOTH";
      std::string dir  = "XY";
      if (j.flag() == JFunc::Flag::WATER)
        flag = "WATER";
      else if (j.flag() == JFunc::Flag::GAS)
        flag = "GAS";

      if (j.direction() == JFunc::Direction::X)
        dir = "X";
      else if (j.direction() == JFunc::Direction::Y)
        dir = "Y";
      else if (j.direction() == JFunc::Direction::Z)
        dir = "Z";

      py::dict ret;
      ret["FLAG"] = flag;
      ret["DIRECTION"] = dir;
      ret["ALPHA_FACTOR"] = j.alphaFactor();
      ret["BETA_FACTOR"] = j.betaFactor();
      if (j.flag() == JFunc::Flag::WATER || j.flag() == JFunc::Flag::BOTH)
        ret["OIL_WATER"] = j.owSurfaceTension();
      if (j.flag() == JFunc::Flag::GAS || j.flag() == JFunc::Flag::BOTH)
        ret["GAS_OIL"] = j.goSurfaceTension();
      return ret;
    }


    const std::string faceDir( FaceDir::DirEnum dir ) {
      switch (dir) {
      case FaceDir::DirEnum::XPlus:  return "X+";
      case FaceDir::DirEnum::XMinus: return "X-";
      case FaceDir::DirEnum::YPlus:  return "Y+";
      case FaceDir::DirEnum::YMinus: return "Y-";
      case FaceDir::DirEnum::ZPlus:  return "Z+";
      case FaceDir::DirEnum::ZMinus: return "Z-";
      }
      return "Unknown direction";
    }

    py::list faultFaces( const EclipseState& state, const std::string& name ) {
        py::list l;
        const auto& gr = state.getInputGrid(); // used for global -> IJK
        const auto& fc = state.getFaults();
        const Fault& f = fc.getFault(name);
        for (const auto& ff : f) {
            // for each fault face
            for (size_t g : ff) {
                // for global index g in ff
                const auto ijk = gr.getIJK(g);
                l.append(py::make_tuple(ijk[0], ijk[1], ijk[2], faceDir(ff.getDir())));
            }
        }
        return l;
    }

}

void sunbeam::export_EclipseState() {

    py::class_< EclipseState >( "EclipseState", py::no_init )
        .add_property( "title", &EclipseState::getTitle )
        .def( "_props",         &EclipseState::get3DProperties, ref() )
        .def( "_grid",          &EclipseState::getInputGrid,    ref() )
        .def( "_cfg",           &EclipseState::cfg,             ref() )
        .def( "_tables",        &EclipseState::getTableManager, ref() )
        .def( "has_input_nnc",  &EclipseState::hasInputNNC )
        .def( "simulation",     &EclipseState::getSimulationConfig, ref())
        .def( "input_nnc",      &getNNC )
        .def( "faultNames",     &faultNames )
        .def( "faultFaces",     &faultFaces )
        .def( "jfunc",          &jfunc )
        ;

}
