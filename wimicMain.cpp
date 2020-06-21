
#include "wimicMain.h"
#include <wx/msgdlg.h>

#include <pthread.h>

#define ARGVCNT 4

bool wimicDialog::_started = false;

namespace MUMPI {
    void stop_threading();
    bool get_connected();
}

int main_start(int argc, char *argv[]);

//(*InternalHeaders(wimicDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)


//(*IdInit(wimicDialog)
const long wimicDialog::ID_PANEL3 = wxNewId();
const long wimicDialog::ID_PANEL2 = wxNewId();
const long wimicDialog::ID_BUTTON1 = wxNewId();
const long wimicDialog::ID_BUTTON2 = wxNewId();
const long wimicDialog::ID_BUTTON3 = wxNewId();
const long wimicDialog::ID_PANEL1 = wxNewId();
const long wimicDialog::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wimicDialog,wxDialog)
    //(*EventTable(wimicDialog)
    //*)
END_EVENT_TABLE()

wimicDialog::wimicDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wimicDialog)
    wxNotebook* Notebook1;

    Create(parent, id, _("Urus GUI app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    GridBagSizer1 = new wxGridBagSizer(0, 0);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    Panel2 = new wxPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel3 = new wxPanel(Panel2, ID_PANEL3, wxPoint(0,0), wxSize(296,120), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    BoxSizer3->Add(Panel2, 1, wxALL|wxALIGN_TOP, 5);
    GridBagSizer1->Add(BoxSizer3, wxGBPosition(0, 0), wxGBSpan(2, 2), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    close = new wxButton(Panel1, ID_BUTTON1, _("close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(close, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridBagSizer1->Add(BoxSizer2, wxGBPosition(2, 1), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    start_server = new wxButton(Panel1, ID_BUTTON2, _("Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer4->Add(start_server, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    stop_server = new wxButton(Panel1, ID_BUTTON3, _("stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
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
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::OnQuit);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::Onstart_serverClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wimicDialog::Onstop_serverClick);
    //*)
}

wimicDialog::~wimicDialog()
{
    //(*Destroy(wimicDialog)
    //*)
}

void wimicDialog::OnQuit(wxCommandEvent& event)
{
    bool conected = MUMPI::get_connected();
    MUMPI::stop_threading();

    if (!conected) {
        Close();
    }
}

struct _argmain {
    char *argv[ARGVCNT];
};

void *wimicDialog::_main_start(void *arg)
{
    if (_started) {
        return NULL;
    }

    _started = true;
    _argmain *argtmp = (_argmain*)arg;

    main_start(ARGVCNT, argtmp->argv);

    _started = false;
    printf("Exited\n");
    pthread_exit(NULL);
}

void wimicDialog::Onstart_serverClick(wxCommandEvent& event)
{
    if (_started) {
        wxMessageBox(_("Allready running!"),_("Alert"));
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

void wimicDialog::Onstop_serverClick(wxCommandEvent& event)
{
    MUMPI::stop_threading();
}
