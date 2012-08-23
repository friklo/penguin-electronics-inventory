#ifndef AddCategoryDialog_h
#define AddCategoryDialog_h

#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>

class AddCategoryDialog : public Gtk::Dialog
{
public:
	AddCategoryDialog();
	
	Gtk::HBox m_catstuff;
		Gtk::Label m_catlabel;
		Gtk::Entry m_catname;
};

#endif
