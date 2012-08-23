/**
	@file MainWindow.cpp
	@author Andrew D. Zonenberg
	@brief Main application window
 */

#include "MainWindow.h"
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <stdint.h>

#include "AddCategoryDialog.h"

using namespace std;

MainWindow::MainWindow()
{
	//Initial setup
	set_title("Penguin Electronics Inventory");
	set_reallocate_redraws(true);
	set_default_size(1280, 800);
	
	try
	{
		//Add widgets
		CreateWidgets();
		
		//TODO: Load categories from server
	}
	catch(std::string err)
	{
		cerr << err.c_str();
		exit(-1);
	}
	
	//Done adding widgets
	show_all();
	
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateWidgets()
{	
	m_catmodel = Gtk::TreeStore::create(m_catcols);
	
	add(m_rootsplitter);
		m_rootsplitter.add1(m_catbrowser);
			m_catbrowser.set_model(m_catmodel);
			m_catbrowser.append_column("Category", m_catcols.name);
			m_catbrowser.get_column(0)->set_sort_column(m_catcols.name);
			m_catbrowser.get_column(0)->clicked();
			m_catbrowser.enable_model_drag_source();
			m_catbrowser.enable_model_drag_dest();
			m_catbrowser.get_selection()->set_mode(Gtk::SELECTION_SINGLE);
		m_rootsplitter.add2(m_itemlist);
		m_rootsplitter.set_position(300);
	
	m_catbrowser_popup.add(m_catbrowser_popup_addcat);
		m_catbrowser_popup_addcat.set_label("Add Category");
		m_catbrowser_popup_addcat.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::OnAddCategory), true);
	m_catbrowser_popup.add(m_catbrowser_popup_delcat);
		m_catbrowser_popup_delcat.set_label("Delete Category");
		m_catbrowser_popup_delcat.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::OnDeleteCategory), true);
	m_catbrowser_popup.show_all();
		
	m_catbrowser.signal_button_press_event().connect(sigc::mem_fun(*this, &MainWindow::OnClickCatBrowser), false);
}

bool MainWindow::OnClickCatBrowser(GdkEventButton* event)
{
	if(event->type == GDK_BUTTON_PRESS)
	{
		Gtk::TreeModel::Path path;
		if(!m_catbrowser.get_path_at_pos(event->x, event->y, path))		//TODO: bin window check
			m_catbrowser.get_selection()->unselect_all();
		
		//Popup menu
		if(event->button == 3)
		{
			//Show all menu items
			m_catbrowser_popup.show_all();
			
			//Don't show menu items that don't make sense in this context
			Glib::RefPtr<Gtk::TreeSelection> sel = m_catbrowser.get_selection();	
			if(sel->count_selected_rows() == 0)
				m_catbrowser_popup_delcat.hide();
			
			m_catbrowser_popup.popup(event->button, event->time);
		}
	}
	
	//call the existing handler
	return false;
}

void MainWindow::OnAddCategory()
{
	AddCategoryDialog dlg;
	int result = dlg.run();
	if(result != Gtk::RESPONSE_ACCEPT)
		return;
		
	Glib::RefPtr<Gtk::TreeSelection> sel = m_catbrowser.get_selection();
		
	if(sel->count_selected_rows() == 0)
	{		
		//Add the new category to the end of the list
		Gtk::TreeStore::iterator it = m_catmodel->append();
		it->set_value(0, dlg.m_catname.get_text());
	}
	else
	{
		//Add as a child of the current selection 
		Gtk::TreeStore::iterator it = m_catmodel->append(sel->get_selected()->children());
		it->set_value(0, dlg.m_catname.get_text());
		
		//and expand the parent so the new category is visible
		Gtk::TreePath path(sel->get_selected());
		m_catbrowser.expand_row(path, false);
	}
	
	//TODO: Sync with server
}

void MainWindow::OnDeleteCategory()
{	
	Gtk::MessageDialog dlg(	"Are you sure you want to delete this category and all of its subcategories?\n\n"
							"This operation will fail if any components are still assigned to the category.",
							false,
							Gtk::MESSAGE_QUESTION,
							Gtk::BUTTONS_YES_NO,
							true);
	int response = dlg.run();
	if(response != Gtk::RESPONSE_YES)
		return;
		
	//Delete the node
	Glib::RefPtr<Gtk::TreeSelection> sel = m_catbrowser.get_selection();
	m_catmodel->erase(sel->get_selected());
	
	//TODO: sync with server
}
