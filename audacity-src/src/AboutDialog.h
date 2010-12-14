/**********************************************************************

  Audacity: A Digital Audio Editor

  AboutDialog.h

  Dominic Mazzoni

**********************************************************************/

#ifndef __AUDACITY_ABOUT_DLG__
#define __AUDACITY_ABOUT_DLG__

#include <wx/dialog.h>

class wxBoxSizer;
class wxStaticBitmap;
class wxBitmap;

class AboutDialog:public wxDialog {
   DECLARE_DYNAMIC_CLASS(AboutDialog)

 public:
   AboutDialog(wxWindow * parent);
   virtual ~ AboutDialog();

   void OnOK(wxCommandEvent & event);

   wxBoxSizer *topsizer;
   wxStaticBitmap *icon;
   wxBitmap *logo;
   #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS)
      wxStaticBitmap* pBrandIcon;
      wxBitmap* pBrandLogo;
   #endif
    DECLARE_EVENT_TABLE()
};

#endif
