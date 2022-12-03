#include "cmdhandler.h"

#include "util.h"
using namespace std;

// Complete
QuitHandler::QuitHandler() {}

// Complete
QuitHandler::QuitHandler(Handler* next) : Handler(next) {}

// Complete
bool QuitHandler::canHandle(const std::string& cmd) const { return cmd == "QUIT"; }

// Complete
Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    return HANDLER_QUIT;
}

// Complete
PrintHandler::PrintHandler() {}

// Complete
PrintHandler::PrintHandler(Handler* next) : Handler(next) {}

// Complete
bool PrintHandler::canHandle(const std::string& cmd) const { return cmd == "PRINT"; }

// Complete
Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    } catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

// Add code for other handler class implementations below

AndHandler::AndHandler() {}

AndHandler::AndHandler(Handler* next) : Handler(next) {}

bool AndHandler::canHandle(const std::string& cmd) const { return cmd == "AND"; }

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    string temp;
    WebPageSetCombiner* comb;
    comb = new AndWebPageSetCombiner;
    std::vector<std::string> word;
    WebPageSet::iterator itr;
    WebPageSet search;

    while (instr >> temp) {
        word.push_back(conv_to_lower(temp));
    }

    search = eng->search(word, comb);
    ostr << search.size() << endl;

    itr = search.begin();

    while (itr != search.end())

    {
        ostr << (*itr)->filename() << endl;
        ++itr;
    }
    delete comb;
    return HANDLER_OK;
}

OrHandler::OrHandler() {}

OrHandler::OrHandler(Handler* next) : Handler(next) {}

bool OrHandler::canHandle(const std::string& cmd) const { return cmd == "OR"; }
Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    string temp;
    WebPageSetCombiner* comb;
    comb = new OrWebPageSetCombiner;
    std::vector<std::string> word;
    WebPageSet::iterator itr;
    WebPageSet search;

    while (instr >> temp) {
        word.push_back(conv_to_lower(temp));
    }

    search = eng->search(word, comb);
    ostr << search.size() << endl;

    itr = search.begin();

    while (itr != search.end())

    {
        ostr << (*itr)->filename() << endl;
        ++itr;
    }
    delete comb;
    return HANDLER_OK;
}

DiffHandler::DiffHandler() {}

DiffHandler::DiffHandler(Handler* next) : Handler(next) {}

bool DiffHandler::canHandle(const std::string& cmd) const { return cmd == "DIFF"; }
Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {
    string temp;
    WebPageSetCombiner* comb;
    comb = new DiffWebPageSetCombiner;
    std::vector<std::string> word;
    WebPageSet::iterator itr;
    WebPageSet search;

    while (instr >> temp) {
        word.push_back(conv_to_lower(temp));
    }

    search = eng->search(word, comb);
    ostr << search.size() << endl;

    itr = search.begin();

    while (itr != search.end())

    {
        ostr << (*itr)->filename() << endl;
        ++itr;
    }
    delete comb;
    return HANDLER_OK;
}

IncomingHandler::IncomingHandler() {}
IncomingHandler::IncomingHandler(Handler* next) : Handler(next) {}
bool IncomingHandler::canHandle(const std::string& cmd) const { return cmd == "INCOMING"; }
Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {

    string name;
    WebPageSet display;
    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->read_page(name);
        display = (eng->retrieve_page(name))->incoming_links();
        display_hits(display, ostr);
    }

    catch (std::exception& e) {
        return HANDLER_ERROR;
    }

    return HANDLER_OK;
}

OutgoingHandler::OutgoingHandler() {}
OutgoingHandler::OutgoingHandler(Handler* next) : Handler(next) {}
bool OutgoingHandler::canHandle(const std::string& cmd) const { return cmd == "OUTGOING"; }
Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr) {

    string name;
    WebPageSet display;
    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->read_page(name);
        display = (eng->retrieve_page(name))->outgoing_links();
        display_hits(display, ostr);
    } catch (std::exception& e) {
        return HANDLER_ERROR;
    }

    return HANDLER_OK;
}
