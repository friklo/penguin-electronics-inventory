#include "AddManufacturerDialog.h"

AddManufacturerDialog::AddManufacturerDialog()
{
	//Initial setup
	set_title("Add Manufacturer");
	set_default_size(500, 300);

	get_vbox()->pack_start(m_compstuff, Gtk::PACK_SHRINK);
		m_compstuff.pack_start(m_compnamelabel, Gtk::PACK_SHRINK);
		m_compnamelabel.set_text("Manufacturer name");
		m_compstuff.pack_start(m_compname);
		m_compnamelabel.set_width_chars(20);
	get_vbox()->pack_start(m_webstuff, Gtk::PACK_SHRINK);
		m_webstuff.pack_start(m_weblabel, Gtk::PACK_SHRINK);
		m_weblabel.set_text("Website");
		m_webstuff.pack_start(m_website);
		m_weblabel.set_width_chars(20);
	get_vbox()->pack_start(m_logostuff, Gtk::PACK_SHRINK);
		m_logostuff.pack_start(m_logolabel, Gtk::PACK_SHRINK);
		m_logolabel.set_text("Logo URL");
		m_logostuff.pack_start(m_logourl);
		m_logolabel.set_width_chars(20);
	
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	add_button(Gtk::Stock::OK, Gtk::RESPONSE_ACCEPT);
	
	m_compname.set_activates_default(true);
	m_logourl.set_activates_default(true);
	m_website.set_activates_default(true);
	set_default_response(Gtk::RESPONSE_ACCEPT);
	
	show_all();
}

