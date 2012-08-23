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
		m_rootsplitter.add2(m_itemlist);
		m_rootsplitter.set_position(300);
	
	m_catbrowser_popup.add(m_catbrowser_popup_addcat);
		m_catbrowser_popup_addcat.set_label("Add Category");
		m_catbrowser_popup_addcat.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::OnAddCategory), true);
	m_catbrowser_popup.show_all();
		
	m_catbrowser.signal_button_press_event().connect(sigc::mem_fun(*this, &MainWindow::OnClickCatBrowser), false);
}

bool MainWindow::OnClickCatBrowser(GdkEventButton* event)
{
	if(event->type == GDK_BUTTON_PRESS)
	{
		//Popup menu
		if(event->button == 3)
			m_catbrowser_popup.popup(event->button, event->time);
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
		
	//Add the new category
	Gtk::TreeStore::iterator it = m_catmodel->append();
	it->set_value(0, dlg.m_catname.get_text());
}
