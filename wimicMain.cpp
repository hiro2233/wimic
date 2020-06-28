
#include "wimicMain.h"
#include <wx/msgdlg.h>

#include "typedef_ext.h"
#include <pthread.h>
#include <unistd.h>

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
#include <wx/string.h>
//*)


//(*IdInit(wimicDialog)
const long wimicDialog::ID_STATICTEXT1 = wxNewId();
const long wimicDialog::ID_LED1 = wxNewId();
const long wimicDialog::ID_BUTTON1 = wxNewId();
const long wimicDialog::ID_BUTTON2 = wxNewId();
const long wimicDialog::ID_BUTTON3 = wxNewId();
const long wimicDialog::ID_LISTBOX1 = wxNewId();
const long wimicDialog::ID_BUTTON4 = wxNewId();
const long wimicDialog::ID_STATICTEXT2 = wxNewId();
const long wimicDialog::ID_STATICTEXT3 = wxNewId();
const long wimicDialog::ID_PANEL1 = wxNewId();
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
    wxBoxSizer* BoxSizer7;

    Create(parent, wxID_ANY, _("WiMic Server/Client"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_PANEL1"));
    GridBagSizer1 = new wxGridBagSizer(1, 1);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    lblstatconnection = new wxStaticText(Panel1, ID_STATICTEXT1, _("Server:"), wxDefaultPosition, wxSize(51,15), 0, _T("ID_STATICTEXT1"));
    BoxSizer3->Add(lblstatconnection, 1, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_TOP, 5);
    Led1 = new wxLed(Panel1,ID_LED1,wxColour(255,0,0),wxColour(0,255,0),wxColour(255,0,0),wxDefaultPosition,wxDefaultSize);
    Led1->SwitchOff();
    BoxSizer3->Add(Led1, 1, wxALL|wxSHAPED, 5);
    GridBagSizer1->Add(BoxSizer3, wxGBPosition(0, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
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
    GridBagSizer1->AddGrowableCol(0);
    GridBagSizer1->AddGrowableRow(0);
    Panel1->SetSizer(GridBagSizer1);
    GridBagSizer1->Fit(Panel1);
    GridBagSizer1->SetSizeHints(Panel1);
    Notebook1->AddPage(Panel1, _("Main"), true);
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
    struct dirent *de;
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
    wmdev.default_dev = sel;
    dev_label_sel->SetLabel(wxString::FromUTF8(wmdev.name[wmdev.default_dev]));
    printf("Device Nr:%d Name: %s\n", wmdev.default_dev, wmdev.name[wmdev.default_dev]);
}

void wimicDialog::_detect_devices()
{
    wxArrayString s1;

    WIMIC::detect_devices();

    for (uint8_t i = 0; i < wmdev.dev_count; i++) {
        if (wmdev.inout_dev == INOUT_DEV::OUTPUT_DEV) {
            s1.Add(wxString::FromUTF8(wmdev.name[i]));
        }
    }

    ListBox1->Clear();
    ListBox1->InsertItems(s1, 0);
    ListBox1->SetSelection(wmdev.default_dev);
    dev_label_sel->SetLabel(wxString::FromUTF8(wmdev.name[wmdev.default_dev]));
}
