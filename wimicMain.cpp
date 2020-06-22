
#include "wimicMain.h"
#include <wx/msgdlg.h>

#include <pthread.h>

#define ARGVCNT 4

bool wimicDialog::_started = false;
bool wimicDialog::_server_started = false;

namespace MUMPI {
    void stop_threading();
    bool get_connected();
}

struct _argmain {
    char *argv[ARGVCNT];
};

struct _status_system {
    bool conected;
    bool close_app;
};

_status_system status_system;

int main_start(int argc, char *argv[]);

extern "C" {
    int server_main_start(int argc, char **argv);
    void Server_shutdown();
}

//(*InternalHeaders(wimicDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)


//(*IdInit(wimicDialog)
const long wimicDialog::ID_STATICTEXT1 = wxNewId();
const long wimicDialog::ID_LED1 = wxNewId();
const long wimicDialog::ID_BUTTON1 = wxNewId();
const long wimicDialog::ID_BUTTON2 = wxNewId();
const long wimicDialog::ID_BUTTON3 = wxNewId();
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
    Create(parent, id, _("WiMic Server/Client"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX|wxTAB_TRAVERSAL|wxMINIMIZE_BOX, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_NOTEBOOK1"));
    Notebook1->SetFocus();
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    lblstatconnection = new wxStaticText(Panel1, ID_STATICTEXT1, _("Server:"), wxDefaultPosition, wxSize(62,15), 0, _T("ID_STATICTEXT1"));
    BoxSizer3->Add(lblstatconnection, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    Led1 = new wxLed(Panel1,ID_LED1,wxColour(128,128,128),wxColour(0,255,0),wxColour(255,0,0),wxDefaultPosition,wxDefaultSize);
    Led1->SwitchOff();
    BoxSizer3->Add(Led1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(BoxSizer3, wxGBPosition(0, 0), wxGBSpan(2, 1), wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    close = new wxButton(Panel1, ID_BUTTON1, _("close"), wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(close, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(BoxSizer2, wxGBPosition(2, 1), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    start_client = new wxButton(Panel1, ID_BUTTON2, _("start"), wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer4->Add(start_client, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    stop_server = new wxButton(Panel1, ID_BUTTON3, _("stop"), wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer4->Add(stop_server, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(BoxSizer4, wxGBPosition(2, 0), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->AddGrowableCol(1);
    GridBagSizer1->AddGrowableRow(1);
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
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&wimicDialog::Ontimer_connect_statusTrigger);
    //*)

    stop_server->Enable(false);
    status_system.close_app = false;
}

wimicDialog::~wimicDialog()
{
    //(*Destroy(wimicDialog)
    //*)
}

void wimicDialog::OnQuit(wxCommandEvent& event)
{
    status_system.conected = MUMPI::get_connected();
    MUMPI::stop_threading();

    status_system.close_app = true;

    if (!status_system.conected) {
        Server_shutdown();
        usleep(200);
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

    server_main_start(ARGVCNT, argtmp->argv);

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
    sprintf(argmain->argv[3], "%s", "-p1234");

    pthread_t main_start_thread;
    pthread_attr_t thread_attr_main_start;

    pthread_attr_init(&thread_attr_main_start);
    pthread_attr_setstacksize(&thread_attr_main_start, 2048);

    pthread_attr_setschedpolicy(&thread_attr_main_start, SCHED_FIFO);

    pthread_create(&main_start_thread, &thread_attr_main_start, _main_start, argmain);
}

void wimicDialog::Onstart_clientClick(wxCommandEvent& event)
{
    _start_server();
    usleep(200);
    _start_client();

    Led1->SwitchOff();

    if (!timer_connect_status.IsRunning()) {
        timer_connect_status.Start(1000);
    }
}

void wimicDialog::Onstop_clientClick(wxCommandEvent& event)
{
    MUMPI::stop_threading();
    usleep(200);
    Server_shutdown();
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
    status_system.conected = MUMPI::get_connected();

    if (!status_system.conected) {
        if (status_system.close_app) {
            Close();
        }
    }

    Led1->SetOnOrOff(status_system.conected);
    stop_server->Enable(status_system.conected);
    start_client->Enable(!status_system.conected);
}

