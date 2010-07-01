/**********************************************************************

  Audacity: A Digital Audio Editor

  AboutDialog.cpp

  Dominic Mazzoni
  Vaughan Johnson

**********************************************************************/

#include <wx/dialog.h>
#include <wx/html/htmlwin.h>
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/intl.h>

#include "AboutDialog.h"
#include "Audacity.h"
#include "AudacityApp.h"
#include "AudacityBranding.h"

#if (AUDACITY_BRANDING == BRAND_UMIXIT)
   #define nBrandHeight 64
   #include "../images/Branding/UmixIt.xpm" // 162x64
   #include "../images/Branding/powered_by_Audacity_162x64.xpm" // 162x64
#elif (AUDACITY_BRANDING == BRAND_THINKLABS)
   #define nBrandHeight 42
   #include "../images/Branding/Thinklabs.xpm" // 182x42
   #include "../images/Branding/powered_by_Audacity.xpm" // 106x42
#else
   #include "../images/AudacityLogo.xpm"
#endif

#ifdef __WXMSW__
# define DLOG_HEIGHT 430
#else
# if defined(__WXMAC__) && ((wxMAJOR_VERSION == 2) && (wxMINOR_VERSION >= 5))
#  define DLOG_HEIGHT 430
# else
#  define DLOG_HEIGHT 400
# endif
#endif


// ----------------------------------------------------------------------------
// icons
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
   EVT_BUTTON(wxID_OK, AboutDialog::OnOK)
END_EVENT_TABLE()

IMPLEMENT_CLASS(AboutDialog, wxDialog)

AboutDialog::AboutDialog(wxWindow * parent)
:  wxDialog(parent, -1, _("About Audacity..."),
            wxDefaultPosition, 
            wxSize(400, 
                     #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS) 
                        DLOG_HEIGHT + nBrandHeight + 8),
                     #else
                        DLOG_HEIGHT), 
                     #endif
            wxDEFAULT_DIALOG_STYLE)
{
   wxString versionStr = AUDACITY_VERSION_STRING;

   wxString informationStr;

   #ifdef __i686
   informationStr += _("Intel Mac version");
   #endif

   #ifdef MP3SUPPORT
   informationStr += _("MP3 importing enabled");
   #else
   informationStr += _("MP3 importing disabled");
   #endif
   informationStr += "<br>\n";

   #ifdef USE_LIBVORBIS
   informationStr += _("Ogg Vorbis importing enabled");
   #else
   informationStr += _("Ogg Vorbis importing disabled");
   #endif
   informationStr += "<br>\n";

   #ifdef USE_LIBID3TAG
   informationStr += _("ID3 tag exporting enabled");
   #else
   informationStr += _("ID3 tag exporting disabled");
   #endif
   informationStr += "<br>\n";

   # if USE_LADSPA
   informationStr += _("LADSPA plug-in support enabled");
   # else
   informationStr += _("LADSPA plug-in support disabled");
   # endif
   informationStr += "<br>\n";

   #if USE_LIBRESAMPLE
   informationStr += _("Libresample support enabled");
   #elif USE_LIBSAMPLERATE
   informationStr += _("Libsamplerate support enabled");
   #else
   informationStr += _("No resampling support enabled");
   #endif
   informationStr += "<br>\n";

   // wxWindows version:
   informationStr += wxVERSION_STRING;
   informationStr += "<br>\n";

   // Current date
   informationStr += _("Program build date:");
   informationStr += " " __DATE__ "<br>\n";

   #if (AUDACITY_BRANDING == BRAND_THINKLABS)
      wxString strThinklabsPreamble = _(
         "Thinklabs Phonocardiography, powered by Audacity, "
         "is provided free to researchers and clinicians in the interests of "
         "promoting sound medicine. Thinklabs Phonocardiography is a custom "
         "version of Audacity, for working with heart and lung sounds. It is "
         "designed to complement the power of Thinklabs Digital Stethoscopes."

         "<p>Thinklabs Phonocardiography, powered by Audacity, "
         "is open source software, and researchers are encouraged to "
         "develop it further to enhance the diagnostic power of medical acoustics. "
         "Hopefully the results of such research will be shared freely under the "
         "spirit and letter of the GPL license, under which Audacity is licensed. \n\n"
         
         "<p>Thinklabs Phonocardiography, powered by Audacity, "
         "was developed from Audacity by Vaughan Johnson and Clive Smith, "
         "with funding provided by Thinklabs Medical LLC."
         );
   #endif

   wxString par1Str = _(
     "Audacity is a free program written by a team of volunteer developers "
     "around the world.  Coordination happens thanks to SourceForge.net, "
     "an online service that provides free tools to open-source "
     "software projects.  Audacity is available for Windows 98 and "
     "newer, Mac OS X, Linux, and other Unix-like operating systems.  "
     "Older versions of Audacity are available for Mac OS 9.");

   #if 0 // Not beta anymore
   wxString par2Str = _(
     "This is a beta version of the program.  It may contain "
     "bugs and unfinished features.  We depend on your feedback, so "
     "please visit our website and give us your bug reports and "
     "feature requests." );
   #else
   wxString par2Str = _(
     "This is a stable, completed release of Audacity.  However, if "
     "you find a bug or have a suggestion, please contact us.  We "
     "depend on feedback from users in order to continue to improve "
     "Audacity.  For more information, visit our website.");
   #endif

   wxString translatorCredits;
   /* i18n-hint: The translation of "translator_credits" will appear
      in the credits in the About Audacity window.  Use this to add
      your own name(s) to the credits.

      For example:  "English translation by Dominic Mazzoni."
      */
   if (_("translator_credits") != wxString("translator_credits")) {
      translatorCredits += "<p><center>";
      translatorCredits += _("translator_credits");
      translatorCredits += "</center>";
   }
   wxString localeStr = wxLocale::GetSystemEncodingName();

   #if defined(__WXGTK__) && defined(wxUSE_UNICODE) && !wxCHECK_VERSION(2, 5, 0)
   // HTML Charsets are broken in wxGTK 2.4.x with unicode turned on
   wxString metaTag = "";
   #else
   wxString metaTag = "<META http-equiv=\"Content-Type\" content=\"text/html; charset=" + localeStr + "\">";
   #endif

   wxString creditStr = 
      "<html>"
      "<head>" + metaTag + "</head>"
      "<body bgcolor=\"#ffffff\">"
      "<font size=1>"
      "<center>"
      "<h3>" +  wxGetApp().GetAppName() + "&reg; " + versionStr + "</h3>"
      + _("A Free Digital Audio Editor") +
      "</center>"
      #if (AUDACITY_BRANDING == BRAND_UMIXIT) 
         "<p>The UmixIt method is protected by Patent # 7,343,210 ." +
      #elif (AUDACITY_BRANDING == BRAND_THINKLABS) 
         "<p>"
         + strThinklabsPreamble + 
      #endif
      "<p>"
      + par1Str +
      "<p>"
      + par2Str +
      "<p>" + 
      AUDACITY_URL + 
      "<p>"
      "<center><b>" + _("Information") + "</b></center>"
      "<p><br>"
      + informationStr +
      "<p>"
      "<center><b>" + _("Credits") + "</b></center>"
      + translatorCredits +
      "<p>"
      "<table border=0>"
      "<tr>"
      "<td>Dominic Mazzoni</td>"
      "<td>" + _("Project leader and primary programmer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Matt Brubeck</td>"
      "<td>" + _("Lead developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>James Crook</td>"
      "<td>" + _("Lead developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Roger Dannenberg</td>"
      "<td>" + _("Founding developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Vaughan Johnson</td>"
      "<td>" + _("Lead developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Markus Meyer</td>"
      "<td>" + _("Lead developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Joshua Haberman</td>"
      "<td>" + _("Major developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Shane Mueller</td>"
      "<td>" + _("Major developer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Tony Oetzmann </td>"
      "<td>" + _("Documentation Writer") + "</td>"
      "</tr>"
      "<tr>"
      "<td>Harvey Lubin</td>"
      "<td>" + _("Main Logo") + "</td>"
      "</tr>"
      "</table>"
      "<p>"
      "<center>"
      "<b>" + _("Developers:") + "</b>"
      "<p>"
      "<br>"
      "William Bland<br>"
      "Vince Busam<br>"
      "Brian Gunlogson<br>"
      "Greg Mekkes<br>"
      "Augustus Saunders<br>"
      "<br>"
      "<p>"
      "<center>"
      "<b>" + _("Other contributors:") + "</b>"
      "<p>"
      "<br>"
      "Dave Beydler<br>"
      "Jason Cohen<br>"
      "Dave Fancella<br>"
      "Steve Harris<br>"
      "Daniel James<br>"
      "Steve Jolly<br>"
      "Daniil Kolpakov<br>"
      "Robert Leidle<br>"
      "Logan Lewis<br>" 
      "Tino Meinen<br>"
      "Abe Milde<br>"
      "Monty Montgomery<br>"
      "Paul Nasca<br>"
      "Jason Pepas<br>"
      "Mark Phillips<br>"
      "Alexandre Prokoudine<br>"
      "Jonathan Ryshpan<br>"
      "Juhana Sadeharju<br>"
      "Patrick Shirkey<br>"
      "Mark Tomlinson<br>"
      "David Topper<br>"
      "Rudy Trubitt<br>"
      "Tom Woodhams<br>"
      "Otto Wyss<br>"
      "<p>"
      "<b>" + _("Special thanks:") + "</b>"
      "<p><br>"
      "The wxWidgets Team<br>"
      "The Ogg Vorbis Team<br>"
      "Rob Leslie (libmad)<br>"
      "Ross Bencina and Phil Burk (PortAudio)<br>"
      "Erik de Castro Lopo (libsndfile)<br>"
      "Olli Parviainen (soundtouch)<br>"
      "Verilogix, Inc.<br>"

      "</center>"
      "</font>"
      "</body>"
      "</html>";
   
   Centre();
   this->SetBackgroundColour(wxColour(255, 255, 255));

   wxBoxSizer * pBoxSizer = new wxBoxSizer(wxVERTICAL);

   #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS)
      pBrandLogo = new wxBitmap((const char **)company_logo_xpm);
      int nBrandLogoWidth = pBrandLogo->GetWidth();
      int nBrandLogoHeight = pBrandLogo->GetHeight();
      pBrandIcon =
         new wxStaticBitmap(this, -1, *pBrandLogo, 
                              wxPoint(32, 16),
                              wxSize(nBrandLogoWidth, nBrandLogoHeight));
      pBoxSizer->Add(pBrandIcon, 0, wxALIGN_CENTER | wxALL, 8);
      logo = new wxBitmap((const char **) powered_by_Audacity_xpm);
   #else
      logo = new wxBitmap((const char **) AudacityLogo_xpm);
   #endif
   icon =
       new wxStaticBitmap(this, -1, *logo, 
                          #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS)
                             // powered_by_Audacity logo at right side
                             wxPoint(400 - logo->GetWidth() - 40, 16),
                             wxSize(logo->GetWidth(), logo->GetHeight()));
                          #else
                             wxPoint(93, 10),
                             wxSize(215, 190));
                          #endif
   pBoxSizer->Add(icon, 0, wxALIGN_CENTER | wxALL, 8);

   wxHtmlWindow *html = 
      new wxHtmlWindow(this, -1,
                        #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS)
                           wxPoint(10, nBrandLogoHeight + 32),
                           wxSize(380, 324), 
                        #else
                           wxPoint(10, 210),
                           wxSize(380, 150), 
                        #endif
                        wxHW_SCROLLBAR_AUTO | wxSUNKEN_BORDER);
   html->SetPage(creditStr);
   pBoxSizer->Add(html, 0, wxALIGN_CENTER | wxALL, 8);

   wxButton *ok = new wxButton(this, wxID_OK,
                               _("Audacious!"),
                                 #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS)
                                    wxPoint(150, nBrandLogoHeight + 372),
                                 #else
                                    wxPoint(150, 372),
                                 #endif
                                 wxSize(100, -1));
   ok->SetDefault();
   ok->SetFocus();
   pBoxSizer->Add(ok, 0, wxALIGN_CENTER | wxALL, 8);
}

AboutDialog::~AboutDialog()
{
   delete icon;
   delete logo;
   #if (AUDACITY_BRANDING == BRAND_UMIXIT) || (AUDACITY_BRANDING == BRAND_THINKLABS)
      delete pBrandIcon;
      delete pBrandLogo;
   #endif
}

void AboutDialog::OnOK(wxCommandEvent & WXUNUSED(event))
{
   EndModal(wxID_OK);
}
