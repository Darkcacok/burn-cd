#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libburn/libburn.h"

using namespace burn;
using namespace std;

void burn_iso_to_disc(struct burn_drive *drive, char *iso_path)
{
    struct burn_source *data_src = NULL, *fifo_src = NULL;
    struct burn_disc *target_disc = NULL;
    struct burn_session *session = NULL;
    struct burn_write_opts *burn_options = NULL;
    enum burn_disc_status disc_state;
    struct burn_track *track;
    struct burn_progress progress;
    int fd = 0;
    off_t fixed_size;
    int fifo_chunksize = 2048;
    int fifo_chunks = 2048;
    int padding = 300 * 1024; //a padding of 300 kiB helps to avoid the read-ahead bug
    char reasons[BURN_REASONS_LEN];
    struct stat stbuf;
    time_t start_time;

    target_disc = burn_disc_create();
    session = burn_session_create();
    burn_disc_add_session(target_disc, session, BURN_POS_END);

    track = burn_track_create();
    burn_track_define_data(track, 0, padding, 1, BURN_MODE1);

    /* Open file descriptor to source of track data */
    fd = open(iso_path, O_RDONLY);
    if (fd >= 0)
        if (fstat(fd, &stbuf) != -1)
            if ((stbuf.st_mode & S_IFMT) == S_IFREG)
                fixed_size = stbuf.st_size;
            else
            {
                cout << "error to open  filedescriptor\n"
                     << endl;
                return;
            }

    /*Convert this filedescriptor into a burn_source object*/
    data_src = burn_fd_source_new(fd, -1, fixed_size);
    if (data_src == NULL)
    {
        cout << "Could not open data source " << iso_path << endl;
        return;
    }

    /* Install a fifo object on top of that data source object */
    fifo_src = burn_fifo_source_new(data_src, fifo_chunksize, fifo_chunks, 0);
    if (fifo_src == NULL)
    {
        cout << "Could not create fifo object of 4 MB\n"
             << iso_path << endl;
        return;
    }

    /* Use the fifo object as data source for the track */
    if (burn_track_set_source(track, fifo_src) != BURN_SOURCE_OK)
    {
        cout << "Cannot attach source object to track object\n";
        return;
    }

    burn_session_add_track(session, track, BURN_POS_END);

    burn_source_free(data_src);

    /*Evaluate drive and media*/
    disc_state = burn_disc_get_status(drive);
    if (disc_state != BURN_DISC_BLANK && disc_state != BURN_DISC_APPENDABLE)
    {
        return;
    }

    /*Create burn options*/
    burn_options = burn_write_opts_new(drive);
    burn_write_opts_set_perform_opc(burn_options, 0); //opc - Optimum Power Calibration
    burn_write_opts_set_multi(burn_options, 0);
    burn_write_opts_set_simulate(burn_options, 0);
    burn_write_opts_set_underrun_proof(burn_options, 1);
    if (burn_write_opts_auto_write_type(burn_options, target_disc, reasons, 0) == BURN_WRITE_NONE)
    {
        cout << "FATAL: Failed to find a suitable write mode with this media.\n"
             << "Reasons given:" << endl
             << reasons << endl;
        return;
    }
    burn_drive_set_speed(drive, 0, 0);

    burn_set_signal_handling((void*)"libburner : ", NULL, 0x30);

    cout << "Burning starts. With e.g. 4x media expect up to a minute of zero progress.\n";
    //start_time = time(0);
    burn_disc_write(burn_options, target_disc);

    while (burn_drive_get_status(drive, NULL) == BURN_DRIVE_SPAWNING)
		usleep(100002);
    while (burn_drive_get_status(drive, &progress) != BURN_DRIVE_IDLE)
    {
        int size, free_bytes, ret;
        char *status_text;

        ret = burn_fifo_inquire_status(fifo_src, &size, &free_bytes, &status_text);
        if(ret > 0)
            cout << "fifo " << status_text << " %% " << 100 - (100.0*free_bytes/size) << " fill" << endl;

        sleep(1);
    }

    burn_write_opts_free(burn_options);
    burn_source_free(fifo_src);
    burn_track_free(track);
    burn_disc_free(target_disc);
}

int main(int argc, char const *argv[])
{
    struct burn_drive_info *drive_list;
    unsigned int drive_count = 0;
    int ret;

    if (burn_initialize())
		cout << "Done\n";
	else {
		cout << "FAILED\n";
		return -1;
	}

    while(!burn_drive_scan(&drive_list, &drive_count));
        usleep(100002);
    if(drive_count <=0)
    {
        cout << "error to scan drive" << endl;
        return -1;
    }

    ret = burn_drive_grab(drive_list[0].drive, 1);
    if(!ret)
    {
        cout << "error to grab the drive";
        return -1;
    }

    burn_iso_to_disc(drive_list[0].drive, "myiso.iso");

    burn_drive_release(drive_list[0].drive, 1);

    burn_finish();

    return 0;
}
