#include "burn.h"

Burn::Burn()
{

}

Burn::~Burn()
{
    burn_finish();
}

bool Burn::initialize()
{
    if(burn_initialize())
        return true;
    else
        return false;
}
