/**
	@file MainWindow.h
	@author Andrew D. Zonenberg
	@brief Main application window
 */

#ifndef MainWindow_h
#define MainWindow_h
#include <gtkmm/actiongroup.h>
#include <gtkmm/box.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/frame.h>
#include <gtkmm/liststore.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/main.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treestore.h>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>

#include <vector>
#include <map>

class CatBrowserColumns : public Gtk::TreeModel::ColumnRecord
{
public:
	Gtk::TreeModelColumn<Glib::ustring> name;
	
	CatBrowserColumns()
	{ add(name); }
};

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	~MainWindow();
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	//UI stuff
	
	Gtk::HPaned m_rootsplitter;
		Gtk::TreeView m_catbrowser;
			Glib::RefPtr<Gtk::TreeStore> m_catmodel;
			CatBrowserColumns m_catcols;
		Gtk::TreeView m_itemlist;
		
	Gtk::Menu m_catbrowser_popup;
		Gtk::MenuItem m_catbrowser_popup_addcat;
		Gtk::MenuItem m_catbrowser_popup_delcat;
	
protected:

	bool OnClickCatBrowser(GdkEventButton* ev);
	
	void OnAddCategory();
	void OnDeleteCategory();

	//Initialization
	void CreateWidgets();
};

#endif
