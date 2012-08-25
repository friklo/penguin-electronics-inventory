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
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treestore.h>
#include <gtkmm/widget.h>
#include <gtkmm/window.h>

#include <string>
#include <vector>
#include <map>

#include <curl/curl.h>
#include <json/json.h>

class CatBrowserColumns : public Gtk::TreeModel::ColumnRecord
{
public:
	Gtk::TreeModelColumn<Glib::ustring> name;
	Gtk::TreeModelColumn<int> id;
	
	CatBrowserColumns()
	{
		add(name);
		add(id);
	}
};

class PackageListColumns : public Gtk::TreeModel::ColumnRecord
{
public:
	Gtk::TreeModelColumn<int> id;
	Gtk::TreeModelColumn<Glib::ustring> shortname;
	
	PackageListColumns()
	{
		add(id);
		add(shortname);
	}
};

class ManufacturerListColumns : public Gtk::TreeModel::ColumnRecord
{
public:
	Gtk::TreeModelColumn<int> id;
	Gtk::TreeModelColumn<Glib::ustring> name;
	Gtk::TreeModelColumn<Glib::ustring> website;
	Gtk::TreeModelColumn<Glib::ustring> logo_url;
	
	ManufacturerListColumns()
	{
		add(id);
		add(name);
		add(website);
		add(logo_url);
	}
};

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	~MainWindow();

protected:
	////////////////////////////////////////////////////////////////////////////////////////////////
	//UI stuff
	
	Gtk::HPaned m_rootsplitter;
		Gtk::ScrolledWindow m_catscroller;
			Gtk::TreeView m_catbrowser;
				Glib::RefPtr<Gtk::TreeStore> m_catmodel;
				CatBrowserColumns m_catcols;
				Gtk::Menu m_catbrowser_popup;
					Gtk::MenuItem m_catbrowser_popup_addcat;
					Gtk::MenuItem m_catbrowser_popup_delcat;
		Gtk::Notebook m_itemtabs;
			Gtk::TreeView m_itemlist;
			//TODO: List of item properties tabs
			Gtk::TreeView m_bomlist;
			Gtk::VBox m_packagemanager;
				Gtk::Frame m_packageframe;
					Gtk::ScrolledWindow m_packagescroller;
						Gtk::TreeView m_packagelist;
							Glib::RefPtr<Gtk::TreeStore> m_packagemodel;
							PackageListColumns m_packagecols;
				Gtk::Frame m_packageclassframe;
			Gtk::TreeView m_manufacturerList;
				Glib::RefPtr<Gtk::TreeStore> m_manufacturermodel;
				ManufacturerListColumns m_manufacturercols;
				Gtk::Menu m_manufacturerList_popup;
					Gtk::MenuItem m_manufacturerList_popup_add;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//Client-server communication
	CURL* m_hCurl;
	std::string m_curldata;
	static size_t CurlRxCallback(char* ptr, size_t size, size_t nmemb, void* pThis);
	size_t CurlRxCallback_real(char* ptr, size_t size, size_t nmemb);
	
	std::string PostRequest(std::string action, std::map<std::string, std::string> postdata);
	
	void RefreshCategoryList();
	void ImportCategory(const Json::Value& cat);
	void ImportCategory(const Json::Value& cat, Gtk::TreeStore::iterator parent);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//Message handlers
	
	bool OnClickCatBrowser(GdkEventButton* ev);
	bool OnClickManufacturerList(GdkEventButton* ev);
	
	void OnAddCategory();
	void OnDeleteCategory();
	void OnCategoryEditStarted(Gtk::CellEditable* cell, const Glib::ustring& path);
	void OnCategoryEditDone();
	
	void OnAddManufacturer();

	////////////////////////////////////////////////////////////////////////////////////////////////
	//Initialization
	void CreateWidgets();
};

#endif
