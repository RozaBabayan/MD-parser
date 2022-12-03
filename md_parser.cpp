#include "md_parser.h"

#include "util.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

// To be completed
void MDParser::parse(
        std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks) {

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // Complete your code below
    // Initialize the current term and link as empty strings.
    string term = "";
    string link = "";
    // initialize the current state
    PARSE_STATE_T currState = NORMALTEXT;

    // Get the first character from the file.
    char c = istr.get();
    // Continue reading from the file until input fails.
    while (!istr.fail()) {
        // for anchor text consider '&' that will split the text into two terms
        // Is c a character to split terms?
        if (!isalnum(c)) {

            /* Determine new state */
            if (currState == NORMALTEXT) {
                if (c != '[') {
                    currState = NORMALTEXT;
                } else if (c == '[') {
                    currState = LINKTEXT;
                }

            } else if (currState == LINKTEXT) {
                if (c != ']') {
                    currState = LINKTEXT;
                } else if (c == ']') {
                    currState = ISLINK;
                }

            } else if (currState == ISLINK) {
                if (c != '(' && c != '[') {
                    currState = NORMALTEXT;

                } else if (c == '[') {
                    currState = LINKTEXT;
                } else if (c == '(') {
                    currState = LINKURL;
                }
            } else if (currState == LINKURL) {
                if (c != ')') {
                    currState = LINKURL;

                } else if (c == ')') {

                    if (link != "") {
                        allOutgoingLinks.insert(link);
                        link = "";
                    }
                    currState = NORMALTEXT;
                }
            }
            if (term != "") {
                if (currState != LINKURL) {
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                }
            }
            term = "";
        }
        // Otherwise we continually add to the end of the current term or link.
        if (isalnum(c) && (currState == NORMALTEXT || currState == LINKTEXT || currState == ISLINK)) {
            term += c;
        }
        if (c != '(' && (currState == LINKURL)) {

            link += c;
        }

        // Attempt to get another character from the file.
        c = istr.get();
    }

    // Since the last term in the file may not have punctuation, there may be a valid term in
    // the "term" variable, so we need to insert it into the allSearchableTerms set.
    if (term != "") {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }
    if (link != "") {

        allOutgoingLinks.insert(link);
    }
}

// To be completed
std::string MDParser::display_text(std::istream& istr) {

    // generate a displayable text strings
    // This function strips out URL/links from the text contents of a file
    // and only shows the anchor text
    std::string retval;
    // initialize the current state
    PARSE_STATE_T currState = NORMALTEXT;
    string term = "";
    string link = "";
    char c = istr.get();

    // // Continue reading from the file until input fails.
    while (!istr.fail()) {
        bool flag = false;
        if (!isalnum(c)) {

            if (currState == NORMALTEXT) {

                if (c != '[') {
                    currState = NORMALTEXT;
                } else if (c == '[') {
                    currState = LINKTEXT;
                }

            } else if (currState == LINKTEXT) {
                if (c != ']') {
                    currState = LINKTEXT;
                } else if (c == ']') {
                    currState = ISLINK;
                }

            } else if (currState == ISLINK) {
                if (c != '(' && c != '[') {
                    currState = NORMALTEXT;

                } else if (c == '[') {
                    currState = LINKTEXT;
                } else if (c == '(') {
                    currState = LINKURL;
                }

            } else if (currState == LINKURL) {

                if (c != ')') {
                    currState = LINKURL;

                } else if (c == ')') {
                    flag = true;

                    currState = NORMALTEXT;
                }
            }
        }

        if (currState != LINKURL && !flag) {
            retval += c;
        }

        c = istr.get();
    }

    return retval;
}
