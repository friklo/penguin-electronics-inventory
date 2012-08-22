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
#include <gtkmm/widget.h>
#include <gtkmm/window.h>

#include <vector>
#include <map>

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	~MainWindow();
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	//UI stuff
	
	Gtk::HPaned m_rootsplitter;
		Gtk::TreeView m_catbrowser;
		Gtk::TreeView m_itemlist;
		
	Gtk::Menu m_catbrowser_popup;
		Gtk::MenuItem m_catbrowser_popup_addcat;
	
protected:

	bool OnClickCatBrowser(GdkEventButton* ev);
	
	void OnAddCategory();

	//Initialization
	void CreateWidgets();
};

#endif
