
#include "wimicApp.h"

#include "typedef_ext.h"
#include <getopt.h>
#include <signal.h>

#ifdef __WXGTK__
#include "resources/logo_wimic.xpm"
#endif

volatile wm_system_status_t wmsystem_status;

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

    m_wimic_taskBarIcon = new wimic_TaskBar();
#if defined(__WXCOCOA__)
    m_wimic_dockIcon = new wimic_TaskBar(wxTaskBarIcon::DOCK);
#endif
    if (!m_wimic_taskBarIcon->SetIcon(wxICON(logo_wimic), wxT("WiMic Server/Client")))
        wxMessageBox(wxT("Could not set icon."));

    bool wxs_ok = true;
    wxInitAllImageHandlers();
    if ( wxs_ok )
    {
        Dlg = new wimicDialog(0);
        SetTopWindow(Dlg);
        if (!wmsystem_status.silent_mode) {
            Dlg->Show();
        }
    }

    return wxs_ok;

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wimicDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)

    return wxsOK;
}

int wimicApp::OnExit()
{
    return 0;
}

//////////////////////////////////////////
//          WiMic TaskBar
//////////////////////////////////////////

bool wimic_TaskBar::_start_enabled = true;
bool wimic_TaskBar::_stop_enabled = false;
bool wimic_TaskBar::_show_enabled = false;

BEGIN_EVENT_TABLE(wimic_TaskBar,wxTaskBarIcon)
    EVT_MENU(TSKBR_SHOW,        wimic_TaskBar::OnMenuShow)
    EVT_MENU(TSKBR_HIDE,        wimic_TaskBar::OnMenuHide)
    EVT_MENU(TSKBR_EXIT,        wimic_TaskBar::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(    wimic_TaskBar::OnLeftButtonDClick)
    EVT_MENU(TSKBR_START,       wimic_TaskBar::OnMenuSubStart)
    EVT_UPDATE_UI(TSKBR_START,  wimic_TaskBar::OnMenuUIStart)
    EVT_MENU(TSKBR_STOP,        wimic_TaskBar::OnMenuSubStop)
    EVT_UPDATE_UI(TSKBR_STOP,   wimic_TaskBar::OnMenuUIStop)
END_EVENT_TABLE()

#if defined(__WXCOCOA__)
wimic_TaskBar::wimic_TaskBar(wxTaskBarIconType iconType = DEFAULT_TYPE)
    : wxTaskBarIcon(iconType)
#else
wimic_TaskBar::wimic_TaskBar()
#endif
{
}

void wimic_TaskBar::OnMenuShow(wxCommandEvent& event)
{
    wimicApp *wimicapp = &wxGetApp();
    wimicapp->Dlg->Show(true);
}

void wimic_TaskBar::OnMenuHide(wxCommandEvent& event)
{
    wimicApp *wimicapp = &wxGetApp();
    wimicapp->Dlg->Show(false);
}

void wimic_TaskBar::OnMenuExit(wxCommandEvent& event)
{
    wimicApp *wimicapp = &wxGetApp();
    wimicapp->Dlg->OnQuit(event);
}

void wimic_TaskBar::OnMenuSubStart(wxCommandEvent& event)
{
    wimicApp *wimicapp = &wxGetApp();
    wimicapp->Dlg->Onstart_clientClick(event);
}

void wimic_TaskBar::OnMenuSubStop(wxCommandEvent& event)
{
    wimicApp *wimicapp = &wxGetApp();
    wimicapp->Dlg->Onstop_clientClick(event);
}

wxMenu *wimic_TaskBar::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(TSKBR_SHOW, _T("&Show"));
    menu->Append(TSKBR_HIDE, _T("&Hide"));
    menu->AppendSeparator();
    wxMenu *submenu = new wxMenu;
    submenu->Append(TSKBR_START, _T("Star&t"));
    submenu->Append(TSKBR_STOP, _T("Sto&p"));
    menu->Append(TSKBR_SUBMAIN, _T("&WiMic"), submenu);

#ifndef __WXMAC_OSX__ // Mac has built-in quit menu
    menu->AppendSeparator();
    menu->Append(TSKBR_EXIT,    _T("&Close"));
#endif

    _start_enabled = !wmsystem_status.conected;
    _stop_enabled = !_start_enabled;

    return menu;
}

void wimic_TaskBar::OnLeftButtonDClick(wxTaskBarIconEvent& event)
{
    wimicApp *wimicapp = &wxGetApp();
    _show_enabled = !_show_enabled;
    wimicapp->Dlg->Show(_show_enabled);
}

void wimic_TaskBar::OnMenuUIStart(wxUpdateUIEvent &event)
{
    event.Enable(_start_enabled);
}

void wimic_TaskBar::OnMenuUIStop(wxUpdateUIEvent &event)
{
    event.Enable(_stop_enabled);
}
