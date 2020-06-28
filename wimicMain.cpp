
#include "wimicMain.h"
#include <wx/msgdlg.h>

#include "typedef_ext.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ifaddrs.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ARGVCNT 4

extern wmdevices_t wmdev;

bool wimicDialog::_started = false;
bool wimicDialog::_server_started = false;

struct _argmain {
    char *argv[ARGVCNT];
};

struct _status_system {
    bool conected;
    bool close_app;
    bool server_running;
    bool server_shuttingdown;
};

_status_system status_system;

int main_start(int argc, char *argv[]);

namespace WIMIC {
    bool get_connected();
    void stop_threading();
    void detect_devices();
}

namespace UMSERVER {
extern "C" {
    int server_main_start(int argc, char **argv);
    void Server_shutdown();
    bool server_is_running();
    char *get_default_certkey_path();
}
}

//(*InternalHeaders(wimicDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)


//(*IdInit(wimicDialog)
const long wimicDialog::ID_BUTTON1 = wxNewId();
const long wimicDialog::ID_BUTTON2 = wxNewId();
const long wimicDialog::ID_BUTTON3 = wxNewId();
const long wimicDialog::ID_LISTBOX1 = wxNewId();
const long wimicDialog::ID_BUTTON4 = wxNewId();
const long wimicDialog::ID_STATICTEXT2 = wxNewId();
const long wimicDialog::ID_STATICTEXT3 = wxNewId();
const long wimicDialog::ID_STATICTEXT1 = wxNewId();
const long wimicDialog::ID_LED1 = wxNewId();
const long wimicDialog::ID_STATICTEXT4 = wxNewId();
const long wimicDialog::ID_STATICTEXT5 = wxNewId();
const long wimicDialog::ID_PANEL1 = wxNewId();
const long wimicDialog::ID_TEXTCTRL1 = wxNewId();
const long wimicDialog::ID_HYPERLINKCTRL1 = wxNewId();
const long wimicDialog::ID_PANEL2 = wxNewId();
const long wimicDialog::ID_NOTEBOOK1 = wxNewId();
const long wimicDialog::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wimicDialog,wxDialog)
    //(*EventTable(wimicDialog)
    //*)
END_EVENT_TABLE()

wimicDialog::wimicDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wimicDialog)
    Create(parent, wxID_ANY, _("WiMic Server/Client"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL1"));
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    GridBagSizer1->AddGrowableCol(3);
    GridBagSizer1->AddGrowableRow(3);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    close = new wxButton(Panel1, ID_BUTTON1, _("close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(close, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(BoxSizer2, wxGBPosition(2, 1), wxDefaultSpan, wxALIGN_RIGHT|wxALIGN_BOTTOM, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    start_client = new wxButton(Panel1, ID_BUTTON2, _("start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer4->Add(start_client, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    stop_server = new wxButton(Panel1, ID_BUTTON3, _("stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer4->Add(stop_server, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(BoxSizer4, wxGBPosition(2, 0), wxDefaultSpan, wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    ListBox1 = new wxListBox(Panel1, ID_LISTBOX1, wxDefaultPosition, wxSize(300,100), 0, 0, wxVSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_LISTBOX1"));
    BoxSizer5->Add(ListBox1, 1, wxALL|wxALIGN_LEFT, 5);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    select_dev = new wxButton(Panel1, ID_BUTTON4, _("select"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer6->Add(select_dev, 0, wxALL|wxALIGN_TOP, 5);
    BoxSizer7 = new wxBoxSizer(wxVERTICAL);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Device selected:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    wxFont StaticText1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    StaticText1->SetFont(StaticText1Font);
    BoxSizer7->Add(StaticText1, 0, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT, 5);
    dev_label_sel = new wxStaticText(Panel1, ID_STATICTEXT3, _("dev_label_sel"), wxDefaultPosition, wxSize(200,15), 0, _T("ID_STATICTEXT3"));
    dev_label_sel->SetForegroundColour(wxColour(0,0,0));
    wxFont dev_label_selFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
    dev_label_sel->SetFont(dev_label_selFont);
    BoxSizer7->Add(dev_label_sel, 1, wxALL|wxALIGN_LEFT, 5);
    BoxSizer6->Add(BoxSizer7, 1, wxALIGN_TOP, 5);
    BoxSizer5->Add(BoxSizer6, 0, wxALIGN_LEFT, 5);
    GridBagSizer1->Add(BoxSizer5, wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    lblstatconnection = new wxStaticText(Panel1, ID_STATICTEXT1, _("Server:"), wxDefaultPosition, wxSize(51,15), 0, _T("ID_STATICTEXT1"));
    BoxSizer3->Add(lblstatconnection, 1, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_TOP, 5);
    Led1 = new wxLed(Panel1,ID_LED1,wxColour(255,0,0),wxColour(0,255,0),wxColour(255,0,0),wxDefaultPosition,wxDefaultSize);
    Led1->SwitchOff();
    BoxSizer3->Add(Led1, 1, wxALL|wxSHAPED, 5);
    FlexGridSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT4, _("IP:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    BoxSizer8->Add(StaticText2, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_TOP, 5);
    local_ip_label = new wxStaticText(Panel1, ID_STATICTEXT5, _("local_ip_label"), wxDefaultPosition, wxSize(89,13), 0, _T("ID_STATICTEXT5"));
    local_ip_label->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HOTLIGHT));
    BoxSizer8->Add(local_ip_label, 1, wxALL|wxALIGN_TOP, 5);
    FlexGridSizer1->Add(BoxSizer8, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    GridBagSizer1->Add(FlexGridSizer1, wxGBPosition(0, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    Panel1->SetSizer(GridBagSizer1);
    GridBagSizer1->Fit(Panel1);
    GridBagSizer1->SetSizeHints(Panel1);
    about_panel = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL2"));
    BoxSizer9 = new wxBoxSizer(wxVERTICAL);
    GridSizer1 = new wxGridSizer(1, 1, 0, 0);
    text_about = new wxTextCtrl(about_panel, ID_TEXTCTRL1, _("text_about"), wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_CENTRE|wxNO_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    text_about->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUBAR));
    GridSizer1->Add(text_about, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 1);
    hyper_link_license = new wxHyperlinkCtrl(about_panel, ID_HYPERLINKCTRL1, _("hyper_link_license"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE|wxNO_BORDER, _T("ID_HYPERLINKCTRL1"));
    GridSizer1->Add(hyper_link_license, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 1);
    BoxSizer9->Add(GridSizer1, 1, wxALL|wxEXPAND, 5);
    about_panel->SetSizer(BoxSizer9);
    BoxSizer9->Fit(about_panel);
    BoxSizer9->SetSizeHints(about_panel);
    Notebook1->AddPage(Panel1, _("Main"), true);
    Notebook1->AddPage(about_panel, _("About"), false);
    BoxSizer1->Add(Notebook1, 1, wxALL|wxALIGN_TOP, 5);
    SetSizer(BoxSizer1);
    timer_connect_status.SetOwner(this, ID_TIMER1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::OnQuit);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::Onstart_clientClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::Onstop_clientClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::OnButton1Click);
    Panel1->Connect(wxEVT_KILL_FOCUS,(wxObjectEventFunction)&wimicDialog::OnPanel1KillFocus,0,this);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&wimicDialog::Ontimer_connect_statusTrigger);
    Connect(wxEVT_PAINT,(wxObjectEventFunction)&wimicDialog::OnPaint);
    Connect(wxEVT_KILL_FOCUS,(wxObjectEventFunction)&wimicDialog::OnKillFocus);
    //*)

    stop_server->Enable(false);
    status_system.close_app = false;
    timer_connect_status.Stop();
    Led1->SetOnOrOff(false);
    _detect_devices();
    _make_about();
}

wimicDialog::~wimicDialog()
{
    //(*Destroy(wimicDialog)
    //*)
}

void wimicDialog::OnQuit(wxCommandEvent& event)
{
    status_system.conected = WIMIC::get_connected();
    WIMIC::stop_threading();

    status_system.server_shuttingdown = true;
    status_system.close_app = true;

    if (!status_system.conected) {
        Close();
    }
}

void *wimicDialog::_main_start(void *arg)
{
    if (_started) {
        return NULL;
    }

    _started = true;
    _argmain *argtmp = (_argmain*)arg;

    main_start(ARGVCNT, argtmp->argv);

    _started = false;
    printf("Exit client\n");
    pthread_exit(NULL);
}

void *wimicDialog::_server_main_start(void *arg)
{
    if (_server_started) {
        return NULL;
    }

    _server_started = true;
    _argmain *argtmp = (_argmain*)arg;

    UMSERVER::server_main_start(ARGVCNT, argtmp->argv);

    printf("Exit server\n");

    _server_started = false;
    pthread_exit(NULL);
}

void wimicDialog::_start_client()
{
    if (_started) {
        wxMessageBox(_("Already running!"),_("Alert"));
    }

    _argmain *argmain = new _argmain;

    for (uint8_t i = 0; i < ARGVCNT; i++) {
        argmain->argv[i] = new char[20];
    }

    sprintf(argmain->argv[0], "%s", "wimic");
    sprintf(argmain->argv[1], "%s", "-s127.0.0.1");
    sprintf(argmain->argv[2], "%s", "-uwimic_server");
    sprintf(argmain->argv[3], "%s", "-padmin");

    pthread_t main_start_thread;
    pthread_attr_t thread_attr_main_start;

    pthread_attr_init(&thread_attr_main_start);
    pthread_attr_setstacksize(&thread_attr_main_start, 2048);

    pthread_attr_setschedpolicy(&thread_attr_main_start, SCHED_FIFO);

    pthread_create(&main_start_thread, &thread_attr_main_start, _main_start, argmain);
}

void wimicDialog::Onstart_clientClick(wxCommandEvent& event)
{
    char *drssl = UMSERVER::get_default_certkey_path();
    _make_dir(drssl, 0775);
    _start_server();

    Led1->SwitchOff();

    if (!timer_connect_status.IsRunning()) {
        timer_connect_status.Start(1000);
    }
}

void wimicDialog::Onstop_clientClick(wxCommandEvent& event)
{
    WIMIC::stop_threading();
    status_system.server_shuttingdown = true;
}

void wimicDialog::_start_server()
{
    if (_server_started) {
        wxMessageBox(_("Server already running!"),_("Alert"));
    }

    _argmain *argmain = new _argmain;

    for (uint8_t i = 0; i < ARGVCNT; i++) {
        argmain->argv[i] = new char[20];
    }

    sprintf(argmain->argv[0], "%s", "wimic");
    sprintf(argmain->argv[1], "%s", "-d");
    sprintf(argmain->argv[2], "%s", "-r");
    sprintf(argmain->argv[3], "%s", "-b1234");

    pthread_t server_main_start_thread;
    pthread_attr_t thread_attr_server_main_start;

    pthread_attr_init(&thread_attr_server_main_start);
    pthread_attr_setstacksize(&thread_attr_server_main_start, 2048);

    pthread_attr_setschedpolicy(&thread_attr_server_main_start, SCHED_FIFO);

    pthread_create(&server_main_start_thread, &thread_attr_server_main_start, _server_main_start, argmain);
}

void wimicDialog::Ontimer_connect_statusTrigger(wxTimerEvent& event)
{
    if (!status_system.server_running) {
        if (UMSERVER::server_is_running()) {
            status_system.server_running = true;
            _start_client();
        }
    }

    if (!status_system.conected) {
        if (status_system.server_shuttingdown) {
            status_system.server_shuttingdown = false;
            UMSERVER::Server_shutdown();
            status_system.server_running = false;
            _detect_devices();
            if (status_system.close_app) {
                Close();
            }
        }
    }

    status_system.conected = WIMIC::get_connected();

    Led1->SetOnOrOff(status_system.conected);
    stop_server->Enable(status_system.conected);
    start_client->Enable(!status_system.conected);
    select_dev->Enable(!status_system.conected);
}

void wimicDialog::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
}

void wimicDialog::_make_dir(const char *dir, int perm)
{
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        mkdir(dir, perm);
    }
}

void wimicDialog::OnKillFocus(wxFocusEvent& event)
{
    event.Skip();
}

void wimicDialog::OnPanel1KillFocus(wxFocusEvent& event)
{
    event.Skip();
}

void wimicDialog::OnButton1Click(wxCommandEvent& event)
{
    uint8_t sel = ListBox1->GetSelection();
    wmdev.default_dev = wmdev.index_tmp_out[sel];
    dev_label_sel->SetLabel(wxString::FromAscii(wmdev.name[wmdev.default_dev]));
    printf("Device Nr:%d Name: %s\n", wmdev.default_dev, wmdev.name[wmdev.default_dev]);
}

void wimicDialog::_detect_devices()
{
    wxArrayString s1;

    WIMIC::detect_devices();

    uint8_t indextmp = 0;
    for (uint8_t i = 0; i < wmdev.dev_count; i++) {
        if (wmdev.inout_dev[i] == INOUT_DEV::OUTPUT_DEV) {
            s1.Add(wxString::FromAscii(wmdev.name[i]));
            wmdev.index_tmp_out[indextmp] = i;
            indextmp++;
        }
    }

    ListBox1->Clear();
    ListBox1->InsertItems(s1, 0);
    ListBox1->SetSelection(indextmp - 1);
    dev_label_sel->SetLabel(wxString::FromAscii(wmdev.name[wmdev.default_dev]));

    const char *localip = _get_local_ip();
    local_ip_label->SetLabel(wxString::FromAscii(localip));
}

const char *wimicDialog::_get_local_ip()
{
    struct ifaddrs *ifaddr, *ifa;
    int s;
    static char hostip[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), hostip, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        switch(ifa->ifa_addr->sa_family) {
            case AF_INET:
                printf("Device: %s Family: IPV4 %s\n", ifa->ifa_name, hostip);
                break;

            case AF_INET6:
                printf("Device: %s Family: IPV6 %s\n", ifa->ifa_name, hostip);
                break;

            default:
                printf("Unknown AF\n");
        }

        if (!(strstr(hostip, "127.")) && (ifa->ifa_addr->sa_family == AF_INET)) {
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            printf("\n\tInterface : <%s>\n", ifa->ifa_name);
            printf("\tAddress   : <%s>\n\n", hostip);
            return hostip;
        }
    }

    freeifaddrs(ifaddr);
    return hostip;
}

void wimicDialog::_make_about()
{
    text_about->SetLabel(_("\n\n\n\nWiMic, remote wireless microphone server and client.\n"
                           "Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>\n"
                           "Licensed under GPLv3."));
    text_about->SetSize(Notebook1->GetSize().GetWidth() - 10, -1);
    hyper_link_license->SetLabel(_("https://github.com/hiro2233/wimic/"));
    hyper_link_license->SetSize(Notebook1->GetSize().GetWidth() - 10, -1);
}
