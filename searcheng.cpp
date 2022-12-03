#include "searcheng.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// Helper function that will extract the extension of a filename
std::string extract_extension(const std::string& filename);

std::string extract_extension(const std::string& filename) {
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}

// To be updated as needed
SearchEng::SearchEng() {}

// To be completed
SearchEng::~SearchEng() {
    std::map<std::string, WebPage*>::iterator itr;
    std::map<std::string, PageParser*>::iterator p;
    for (itr = file_name.begin(); itr != file_name.end(); ++itr) {
        delete (itr->second);
    }
    for (p = parsers_.begin(); p != parsers_.end(); ++p) {
        delete (p->second);
    }
}

// Complete
void SearchEng::register_parser(const std::string& extension, PageParser* parser) {
    if (parsers_.find(extension) != parsers_.end()) {
        throw std::invalid_argument("parser for provided extension already exists");
    }
    parsers_.insert(make_pair(extension, parser));
}

// Complete
void SearchEng::read_pages_from_index(const std::string& index_file) {
    ifstream ifile(index_file.c_str());
    if (ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }
    // Parse all the files
    string filename;
    while (ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

// To be completed
void SearchEng::read_page(const string& filename) {

    string ext = extract_extension(filename);
    std::set<std::string> input_terms, input_links;
    WebPage* link_page;
    if (ext != "") {
        if (parsers_.find(ext) == parsers_.end()) {
            throw std::logic_error("There is no registered parser!");
        }
    }
    ifstream ifile(filename);  // read from a file
    if (ifile.fail()) {
        throw std::invalid_argument("Invalid filename");
    }

    std::map<std::string, PageParser*>::iterator page_it;
    page_it = parsers_.find(ext);                      // find the matching type
    PageParser* pars = page_it->second;                // set the type
    WebPage* store_page = newretrieve_page(filename);  // update and/or add the webpage

    // if the page doesn't exist create a new one
    if (store_page == NULL) {
        store_page = new WebPage(filename);
        file_name.insert(make_pair(filename, store_page));
    }
    // call the parse function for the extension
    pars->parse(ifile, input_terms, input_links);

    // update/store the page into the webpage
    std::set<std::string>::const_iterator it_temp;
    store_page->all_terms(input_terms);
    it_temp = input_terms.begin();
    while (it_temp != input_terms.end()) {
        search_terms[*it_temp].insert(store_page);
        ++it_temp;
    }
    /* Or I can us
    store_page->all_terms(input_terms);
    for(std::set<std::string>::const_iterator it_temp;it_temp != input_terms.end();++it_temp)
    {search_terms[*it_temp].insert(store_page);}*/

    for (std::set<std::string>::const_iterator it_temp = input_links.begin(); it_temp != input_links.end(); ++it_temp) {
        link_page = newretrieve_page(*it_temp);
        if (link_page == NULL) {
            link_page = new WebPage(*it_temp);
            file_name.insert(make_pair(*it_temp, link_page));
        }
        store_page->add_outgoing_link(link_page);
        link_page->add_incoming_link(store_page);
    }
}

// To be completed
WebPage* SearchEng::retrieve_page(const std::string& page_name) const {

    // get pointer to a webpage object
    std::map<std::string, WebPage*>::const_iterator itr;
    itr = file_name.find(page_name);
    if (itr != file_name.end()) {
        return itr->second;
    } else {

        return (NULL);
    }
}

// To be completed
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const {

    string ext = extract_extension(page_name);
    if (ext != "") {
        if (parsers_.find(ext) == parsers_.end()) {
            throw std::logic_error("There is no registered parser!");
        }
    }
    ifstream ifile(page_name);
    if (ifile.fail()) {
        throw std::invalid_argument(" Invalid filename");
    }

    std::map<std::string, PageParser*>::const_iterator itr;
    itr = parsers_.find(ext);
    PageParser* page = itr->second;
    ostr << page->display_text(ifile);
}

// To be completed
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const {
    WebPageSet matchingPages;
    std::map<std::string, WebPageSet>::const_iterator p;
    // init matchingPage to the to the pages that corresponding to the term[0]
    p = search_terms.find(terms[0]);
    matchingPages = p->second;
    // and itarate through the terms starting from i=1
    long unsigned int i = 1;
    if (terms.size() >= 1) {
        // set the matching page to "" is the term is not found or the terms gives an empty set
        while (i < terms.size()) {
            p = search_terms.find(terms[i]);
            if (p != search_terms.end()) {
                matchingPages = combiner->combine(matchingPages, p->second);
            }
            // else not found
            else {
                matchingPages = combiner->combine(matchingPages, WebPageSet());
            }
            i++;
        }
    }
    return matchingPages;
}
// Add private helper function implementations here
// Helper retrive page, because the given retrive page is const
WebPage* SearchEng::newretrieve_page(const std::string& page_name) {
    // get pointer to a webpage object
    std::map<std::string, WebPage*>::const_iterator itr;
    itr = file_name.find(page_name);
    if (itr != file_name.end()) {
        return itr->second;
    } else {

        return (NULL);
    }
}
