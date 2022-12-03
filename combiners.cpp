#include "combiners.h"

#include "searcheng.h"
#include <set>

// Complete the necessary code

AndWebPageSetCombiner::AndWebPageSetCombiner() {}
WebPageSet AndWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) {
    std::set<WebPage*>::iterator itr;
    WebPageSet setIntSec;

    for (itr = setA.begin(); itr != setA.end(); ++itr) {

        if (setB.find(*itr) != setB.end()) {
            setIntSec.insert(*itr);
        }
    }
    return (setIntSec);
}

OrWebPageSetCombiner::OrWebPageSetCombiner() {}
WebPageSet OrWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) {
    std::set<WebPage*>::iterator itr;
    WebPageSet setUnSec;
    for (itr = setA.begin(); itr != setA.end(); ++itr) {
        setUnSec.insert(*itr);
    }
    for (itr = setB.begin(); itr != setB.end(); ++itr) {
        setUnSec.insert(*itr);
    }
    return (setUnSec);
}
DiffWebPageSetCombiner::DiffWebPageSetCombiner() {}
WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) {
    std::set<WebPage*>::iterator itr;
    WebPageSet setDiffSec;

    for (itr = setA.begin(); itr != setA.end(); ++itr) {

        if (setB.find(*itr) == setB.end()) {
            setDiffSec.insert(*itr);
        }
    }
    return (setDiffSec);
}
