#ifndef BURN_H
#define BURN_H

#include "../libburn/libburn.h"
using namespace burn;

class Burn
{
public:
    Burn();
    ~Burn();
    bool initialize();
private:
    struct burn_drive_info *drive_list;
    unsigned int drive_count = 0;
};

#endif // BURN_H
