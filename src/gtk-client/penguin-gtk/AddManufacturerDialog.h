#ifndef AddManufacturerDialog_h
#define AddManufacturerDialog_h

#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>

class AddManufacturerDialog : public Gtk::Dialog
{
public:
	AddManufacturerDialog();
	
	Gtk::HBox m_compstuff;
		Gtk::Label m_compnamelabel;
		Gtk::Entry m_compname;
	Gtk::HBox m_webstuff;
		Gtk::Label m_weblabel;
		Gtk::Entry m_website;
	Gtk::HBox m_logostuff;
		Gtk::Label m_logolabel;
		Gtk::Entry m_logourl;
};

#endif

