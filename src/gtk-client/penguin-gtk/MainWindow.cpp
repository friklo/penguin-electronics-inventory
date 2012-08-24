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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization

MainWindow::MainWindow()
{
	//Initial setup
	set_title("Penguin Electronics Inventory");
	set_reallocate_redraws(true);
	set_default_size(1280, 800);
	
	//Add widgets
	CreateWidgets();
	
	//Initialize libcurl
	m_hCurl = curl_easy_init();
	if(!m_hCurl)
	{
		printf("curl_easy_init() failed\n");
		exit(-1);
	}
	curl_easy_setopt(m_hCurl, CURLOPT_VERBOSE, 0);												//no verbose debug output
	curl_easy_setopt(m_hCurl, CURLOPT_HEADER, 0);												//we don't want to see the headers
	curl_easy_setopt(m_hCurl, CURLOPT_NOPROGRESS, 1);											//don't show progress of operations
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, &MainWindow::CurlRxCallback);				//callback for data coming from server to us
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_hCurl, CURLOPT_NOPROXY, "*");											//do not use proxy server
	
	//Load categories from server
	RefreshCategoryList();

	//Done adding widgets
	show_all();
	
}

MainWindow::~MainWindow()
{
	curl_easy_cleanup(m_hCurl);
}

void MainWindow::CreateWidgets()
{	
	m_catmodel = Gtk::TreeStore::create(m_catcols);
	
	add(m_rootsplitter);
		m_rootsplitter.add1(m_catbrowser);
			m_catbrowser.set_model(m_catmodel);
			m_catbrowser.append_column_editable("Category", m_catcols.name);
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
	m_catbrowser.get_column_cell_renderer(0)->signal_editing_started().connect(sigc::mem_fun(*this, &MainWindow::OnCategoryEditStarted));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General client-server communication

size_t MainWindow::CurlRxCallback(char* ptr, size_t size, size_t nmemb, void* pThis)
{
	return reinterpret_cast<MainWindow*>(pThis)->CurlRxCallback_real(ptr, size, nmemb);
}

size_t MainWindow::CurlRxCallback_real(char* ptr, size_t size, size_t nmemb)
{
	size_t ksize = size*nmemb;
	for(size_t i=0; i<ksize; i++)
		m_curldata += ptr[i];
	return ksize;
}

string MainWindow::PostRequest(string action, std::map<string, string> args)
{
	m_curldata = "";
	
	//Generate the URL
	char url[1024];
	snprintf(url, 1023, "%s/penguinserver.php?action=%s", "http://inventory-dev.drawersteak.com", action.c_str());
	curl_easy_setopt(m_hCurl, CURLOPT_URL, url);
		
	//Generate the POST data
	curl_httppost* postdata = NULL;
	curl_httppost* last = NULL;
	for(std::map<string, string>::iterator it=args.begin(); it != args.end(); it++)
		curl_formadd(&postdata, &last, CURLFORM_COPYNAME, it->first.c_str(), CURLFORM_COPYCONTENTS, it->second.c_str(), CURLFORM_END);
	curl_easy_setopt(m_hCurl, CURLOPT_HTTPPOST, postdata);
		
	//Send the request
	if(0 != curl_easy_perform(m_hCurl))
	{
		printf("foobaz\n");
		printf("WARNING: curl_easy_perform() failed\n");
		return "";
	}
	curl_formfree(postdata);
	
	return m_curldata;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Category list

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
	
	//Get ID of parent
	int parent_id = -1;
	
	//Ask the server to add the category	
	std::map<string, string> args;
	args["catname"] = dlg.m_catname.get_text();
	char sparent[16];
	snprintf(sparent, 15, "%d", parent_id);
	args["parent"] = sparent;
	string server_result = PostRequest("add_category", args);
	
	//Parse the result
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(server_result, root, false))
	{
		printf("Couldn't parse JSON data\n");
		return;
	}
	
	//Check status
	if(root.get("status", "fail").asString() != "ok")
	{
		Gtk::MessageDialog dlg(	string("Server-side error: ") + root.get("error_code", "Unspecified error").asString(),
							false,
							Gtk::MESSAGE_ERROR,
							Gtk::BUTTONS_OK,
							true);
		dlg.run();
		return;
	}
	
	//All OK, get category ID
	int catid = root.get("catid", 1).asInt();
	
	if(sel->count_selected_rows() == 0)
	{		
		//Add the new category to the end of the list
		Gtk::TreeStore::iterator it = m_catmodel->append();
		it->set_value(0, dlg.m_catname.get_text());
		it->set_value(1, catid);
	}
	else
	{
		//Add as a child of the current selection 
		Gtk::TreeStore::iterator it = m_catmodel->append(sel->get_selected()->children());
		it->set_value(0, dlg.m_catname.get_text());
		it->set_value(1, catid);
		
		//and expand the parent so the new category is visible
		Gtk::TreePath path(sel->get_selected());
		m_catbrowser.expand_row(path, false);
	}
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

void MainWindow::OnCategoryEditStarted(Gtk::CellEditable* cell, const Glib::ustring& /*path*/)
{
	cell->signal_editing_done().connect(sigc::mem_fun(*this, &MainWindow::OnCategoryEditDone));
}

void MainWindow::OnCategoryEditDone()
{
	Glib::RefPtr<Gtk::TreeSelection> sel = m_catbrowser.get_selection();
	string newval;
	sel->get_selected()->get_value(0, newval);
	
	printf("editing done (new value = %s)\n", newval.c_str());
	
	//TODO: sync with server
}

void MainWindow::RefreshCategoryList()
{
	std::map<string, string> args;
	string server_result = PostRequest("list_categories", args);
	
	//Parse the result
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(server_result, root, false))
	{
		printf("Couldn't parse JSON data\n");
		exit(-1);
	}
	
	//Check status
	if(root.get("status", "fail").asString() != "ok")
	{
		Gtk::MessageDialog dlg(	string("Server-side error: ") + root.get("error_code", "Unspecified error").asString(),
							false,
							Gtk::MESSAGE_ERROR,
							Gtk::BUTTONS_OK,
							true);
		dlg.run();
		return;
	}
	
	Json::Value cats = root.get("cats", Json::Value::null);
	if(cats.isNull())
	{
		Gtk::MessageDialog dlg(	"No category list provided by server",
							false,
							Gtk::MESSAGE_ERROR,
							Gtk::BUTTONS_OK,
							true);
		dlg.run();
		return;
	}
	
	//Load categories into tree view
	for(Json::Value::iterator it = cats.begin(); it != cats.end(); it++)
		ImportCategory(*it);
}

void MainWindow::ImportCategory(const Json::Value& cat)
{
	//Add the new category to the end of the list
	Gtk::TreeStore::iterator it = m_catmodel->append();
	it->set_value(0, cat.get("name", "Unnamed Category").asString());
	it->set_value(1, cat.get("id", -1).asInt());
}

