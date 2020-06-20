
#include "wimicMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(wimicDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };


//(*IdInit(wimicDialog)
//*)

BEGIN_EVENT_TABLE(wimicDialog,wxDialog)
    //(*EventTable(wimicDialog)
    //*)
END_EVENT_TABLE()

wimicDialog::wimicDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wimicDialog)
    Create(parent, id, _("Urus GUI app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(367,265));
    Center();
    //*)
}

wimicDialog::~wimicDialog()
{
    //(*Destroy(wimicDialog)
    //*)
}

void wimicDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void wimicDialog::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("Ok"), _("Welcome to..."));
}
