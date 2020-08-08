
#include "wimicApp.h"

#include "typedef_ext.h"
#include <getopt.h>
#include <signal.h>

wm_system_status_t wmsystem_status;

//(*AppHeaders
#include "wimicMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP_NO_MAIN(wimicApp);

const char *message_logo[] = {"\nWiMic, remote wireless microphone server & client.\n"
                              "Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>\n"
                              "Licensed under GPLv3 License.\n"
                              "This program has absolutely no warranty.\n\n"};

const char *message_usage[] = {"Usage: wimic [options]\n\n"
                               "Options:\n"
                               "-h, --help          Show this help.\n"
                               "-a, --autostart     autostart mode.\n"
                               "-s, --silent        disables the GUI\n"
                               "-d, --device        specify default Nr. output device\n"};

void wimic_help() {
	printf(message_logo[0]);
	printf(message_usage[0]);
#ifdef __WXGTK__
	exit(1);
#endif
}

int wimic_main(int argc, char **argv)
{
    wmsystem_status.silent_mode = false;
    wmsystem_status.autostart_mode = false;
    wmsystem_status.help_opt = false;

	int next_option;
	const char* const short_options = "hasd:";
	const struct option long_options[] =
	{
		{ "help", no_argument, NULL, 'h' },
		{ "autostart", no_argument, NULL, 'a' },
		{ "silent", no_argument, NULL, 's' },
        { "device", required_argument, NULL, 'd'},
		{ NULL, 0, NULL, 0 }
	};

	while(1) {
		// obtain a option
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);

		if(next_option == -1)
			break;  // no more options

		switch(next_option) {

		case 'h':      // -h or --help
		    wmsystem_status.help_opt = true;
			wimic_help();
			break;

		case 'a':      // -a or --autostart
			wmsystem_status.autostart_mode = true;
			break;

		case 's':      // -s or --silent
			wmsystem_status.silent_mode = true;
			break;

		case 'd':
			wmsystem_status.default_dev = std::atoi(optarg);
			break;

		case '?':      // Invalid option
		    wmsystem_status.help_opt = true;
			wimic_help();

		case -1:      // No more options
			break;

		default:      // shouldn't happen :-)
			return(1);
		}
	}

    return 0;
}

int main(int argc, char **argv)
{
    wimic_main(argc, argv);
    return wxEntry(argc, argv);
}

bool wimicApp::OnInit()
{
    if (wmsystem_status.help_opt) {
#ifdef __WXMSW__
        wxMessageBox(wxString::FromAscii(message_logo[0]) + wxString::FromAscii(message_usage[0]));
#endif // __WXMSW__
        exit(1);
    }

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
        Dlg = new wimicDialog(0);
        SetTopWindow(Dlg);
        if (!wmsystem_status.silent_mode) {
            Dlg->Show();
        }
    }
    //*)
    return wxsOK;

}

int wimicApp::OnExit()
{
    return 0;
}
