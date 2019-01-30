#ifndef BURN_H
#define BURN_H

#include "vector"
#include "iostream"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;


#include "../libburn/libburn.h"
using namespace burn;

#include <QtWidgets/QProgressBar>

class Burn
{
public:
    Burn();
    ~Burn();
    bool initialize();
    vector<string> *scan();
    void write_iso_image(int n, char *iso_path, QProgressBar *progress_burn);

    vector<string> path_to_drive;

private:
    struct burn_drive_info *drive_list;
    unsigned int drive_count = 0;
};

#endif // BURN_H
