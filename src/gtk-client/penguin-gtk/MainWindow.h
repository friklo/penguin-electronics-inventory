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
	/*
	Gtk::VBox m_vbox;
		Gtk::Frame m_vframe;
			Gtk::HBox m_vhbox;
				Graph m_vgraph;
				Gtk::VBox m_vvbox;
					Gtk::Label m_v33label;
					Gtk::Label m_v25label;
					Gtk::Label m_v12label;
		Gtk::Frame m_iframe;
			Gtk::HBox m_ihbox;
				Graph m_igraph;
				Gtk::VBox m_ivbox;
					Gtk::Label m_i33label;
					Gtk::Label m_i25label;
					Gtk::Label m_i12label;
		Gtk::Frame m_pframe;
			Gtk::HBox m_phbox;
				Graph m_pgraph;
				Gtk::VBox m_pvbox;
					Gtk::Label m_p33label;
					Gtk::Label m_p25label;
					Gtk::Label m_p12label;
		Gtk::Frame m_tframe;
			Gtk::HBox m_thbox;
				Graph m_tgraph;
				Gtk::VBox m_tvbox;
					Gtk::Label m_tpsulabel;
					Gtk::Label m_tamblabel;
	*/
	
protected:

	//Initialization
	void CreateWidgets();
};

#endif
