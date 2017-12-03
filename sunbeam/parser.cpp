#include <opm/json/JsonObject.hpp>
#include <opm/parser/eclipse/Parser/Parser.hpp>
#include <opm/parser/eclipse/Deck/Deck.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Schedule.hpp>

#include "sunbeam.hpp"


namespace {

    Deck parseDeck( const std::string& deckStr,
                    const boost::python::list& keywords,
                    bool isFile,
                    const ParseContext& pc ) {
        Parser p;
        size_t len = py::extract<size_t>(keywords.attr("__len__")());
        for (size_t i = 0; i < len; i++) {
            const std::string kw = py::extract<const std::string>(py::str(keywords[i]));
            const Json::JsonObject jkw(kw);
            p.addParserKeyword(jkw);
        }
        return isFile ? p.parseFile(deckStr, pc) : p.parseString(deckStr, pc);
    }

    EclipseState (*parse)( const std::string&, const ParseContext& ) = &Parser::parse;
    EclipseState (*parseData) (const std::string &data, const ParseContext& context) = &Parser::parseData;

    void (ParseContext::*ctx_update)(const std::string&, InputError::Action) = &ParseContext::update;



    Schedule parse_scheduleX(const std::string& deck_file, const Opm::EclipseState& es, const Opm::ParseContext& pc) {
        Parser p;
        Deck deck = p.parseFile(deck_file, pc);
        return Schedule(deck, es, pc);
    }

    Schedule (*parse_schedule)(const std::string&, const EclipseState&, const ParseContext& ) = &parse_scheduleX;
}


void sunbeam::export_Parser() {

    py::def( "parse", parse );
    py::def( "parse_data", parseData );
    py::def( "parse_deck", &parseDeck );
    py::def( "parse_schedule", parse_schedule);
    py::class_< ParseContext >( "ParseContext" )
        .def( "update", ctx_update )
        ;

    py::enum_< InputError::Action >( "action" )
        .value( "throw",  InputError::Action::THROW_EXCEPTION )
        .value( "warn",   InputError::Action::WARN )
        .value( "ignore", InputError::Action::IGNORE )
        ;

}
