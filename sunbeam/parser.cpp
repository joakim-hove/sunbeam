#include <opm/json/JsonObject.hpp>
#include <opm/parser/eclipse/Parser/Parser.hpp>
#include <opm/parser/eclipse/Deck/Deck.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Schedule.hpp>
#include <opm/parser/eclipse/EclipseState/SummaryConfig/SummaryConfig.hpp>
#include <pybind11/stl.h>

#include "sunbeam_state.hpp"
#include "sunbeam.hpp"


namespace {

    Deck create_deck( const std::string& deckStr,
                      const ParseContext& pc,
                      const Parser& parser) {
        return parser.parseFile( deckStr, pc);
    }


    Deck create_deck_string( const std::string& deckStr,
                             const ParseContext& pc,
                             const Parser& parser) {
        return parser.parseString( deckStr, pc);
    }


    EclipseState * old_parse(const std::string& filename, const ParseContext& context) {
        Parser p;
        const auto deck = p.parseFile(filename, context);
        return new EclipseState(deck,context);
    }

    EclipseState * parseData(const std::string& deckStr, const ParseContext& context) {
        Parser p;
        const auto deck = p.parseString(deckStr, context);
        return new EclipseState(deck,context);
    }


    Schedule * create_schedule(const std::string& filename, const ParseContext& context) {
        Parser p;
        const auto deck = p.parseFile(filename, context);
        const EclipseState ecl_state(deck, context);
        return new Schedule(deck, ecl_state, context);
    }

    SummaryConfig * create_smry_config(const std::string& filename) {
        Parser p;
        ParseContext context;

        const auto deck = p.parseFile(filename, context);
        const TableManager tables(deck);
        const EclipseState ecl_state(deck, context);
        const Schedule schedule(deck, ecl_state, context);

        return new SummaryConfig(deck, schedule, tables, context);
    }

    void (ParseContext::*ctx_update)(const std::string&, InputError::Action) = &ParseContext::update;

    SunbeamState * parse_file(const std::string& filename, const ParseContext& context, const Parser& parser) {
        return new SunbeamState(true, filename, context, parser);
    }

    SunbeamState * parse_string(const std::string& filename, const ParseContext& context, const Parser& parser) {
        return new SunbeamState(false, filename, context, parser);
    }

    void add_keyword(Parser* parser, const std::string& json_string) {
        const Json::JsonObject keyword(json_string);
        parser->addParserKeyword(keyword);
    }
}

void sunbeam::export_Parser(py::module& module) {

    module.def( "parse", parse_file );
    module.def( "parse_string", parse_string);
    module.def( "create_deck", &create_deck );
    module.def( "create_deck_string", &create_deck_string);
    module.def( "create_schedule", create_schedule);
    module.def( "create_smry_config", create_smry_config);


    py::class_<SunbeamState>(module, "SunbeamState")
        .def("_schedule", &SunbeamState::getSchedule, ref_internal)
        .def("_state", &SunbeamState::getEclipseState, ref_internal)
        .def("_deck", &SunbeamState::getDeck, ref_internal)
        .def("_summary_config", &SunbeamState::getSummmaryConfig, ref_internal);

    py::class_< ParseContext >(module, "ParseContext" )
        .def(py::init<>())
        .def(py::init<const std::vector<std::pair<std::string, InputError::Action>>>())
        .def( "update", ctx_update );

    py::enum_< InputError::Action >( module, "action" )
      .value( "throw",  InputError::Action::THROW_EXCEPTION )
      .value( "warn",   InputError::Action::WARN )
      .value( "ignore", InputError::Action::IGNORE );


    py::class_<Parser>(module, "Parser")
        .def(py::init<>())
        .def("add_keyword", add_keyword);
}
