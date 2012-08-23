#include "AddCategoryDialog.h"

AddCategoryDialog::AddCategoryDialog()
{
	//Initial setup
	set_title("Add Category");
	set_default_size(500, 300);

	get_vbox()->pack_start(m_catstuff, Gtk::PACK_SHRINK);
		m_catstuff.pack_start(m_catlabel, Gtk::PACK_SHRINK);
		m_catlabel.set_text("Category name");
		m_catstuff.pack_start(m_catname);
	
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	add_button(Gtk::Stock::OK, Gtk::RESPONSE_ACCEPT);
	
	show_all();
}
