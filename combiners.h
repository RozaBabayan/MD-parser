#ifndef COMBINERS_H
#define COMBINERS_H

#include "searcheng.h"

/*** Define Derived WebPageSetCombiners (for AND, OR, DIFF) classes below ***/

class AndWebPageSetCombiner : public WebPageSetCombiner {
public:
    AndWebPageSetCombiner();
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
};
class OrWebPageSetCombiner : public WebPageSetCombiner {
public:
    OrWebPageSetCombiner();
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
};
class DiffWebPageSetCombiner : public WebPageSetCombiner {
public:
    DiffWebPageSetCombiner();
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
};
#endif