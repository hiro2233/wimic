
#include "wimicApp.h"

//(*AppHeaders
#include "wimicMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wimicApp);

bool wimicApp::OnInit()
{
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
