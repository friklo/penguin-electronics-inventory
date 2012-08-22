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

using namespace std;

MainWindow::MainWindow()
{
	//Initial setup
	set_title("Penguin Electronics Inventory");
	set_reallocate_redraws(true);
	set_default_size(1024, 768);
	
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
	/*
	const int label_width = 14;
	
	add(m_vbox);
		m_vbox.pack_start(m_vframe);
			m_vframe.set_label("Voltage");
				m_vframe.add(m_vhbox);
					m_vhbox.pack_start(m_vgraph);
					m_vhbox.pack_end(m_vvbox, Gtk::PACK_SHRINK);
						m_vvbox.pack_start(m_v33label);
						m_vvbox.pack_start(m_v25label);
						m_vvbox.pack_start(m_v12label);
						m_v33label.set_width_chars(label_width);
						m_v25label.set_width_chars(label_width);
						m_v12label.set_width_chars(label_width);
		m_vbox.pack_start(m_iframe);
			m_iframe.set_label("Current");
				m_iframe.add(m_ihbox);
					m_ihbox.pack_start(m_igraph);
					m_ihbox.pack_end(m_ivbox, Gtk::PACK_SHRINK);
						m_ivbox.pack_start(m_i33label);
						m_ivbox.pack_start(m_i25label);
						m_ivbox.pack_start(m_i12label);
						m_i33label.set_width_chars(label_width);
						m_i25label.set_width_chars(label_width);
						m_i12label.set_width_chars(label_width);
		m_vbox.pack_start(m_pframe);
			m_pframe.set_label("Power");
				m_pframe.add(m_phbox);
					m_phbox.pack_start(m_pgraph);
					m_phbox.pack_end(m_pvbox, Gtk::PACK_SHRINK);
						m_pvbox.pack_start(m_p33label);
						m_pvbox.pack_start(m_p25label);
						m_pvbox.pack_start(m_p12label);
						m_p33label.set_width_chars(label_width);
						m_p25label.set_width_chars(label_width);
						m_p12label.set_width_chars(label_width);
		m_vbox.pack_start(m_pframe);
		m_vbox.pack_start(m_tframe);
			m_tframe.set_label("Temperature");
				m_tframe.add(m_thbox);
					m_thbox.pack_start(m_tgraph);
					m_thbox.pack_end(m_tvbox, Gtk::PACK_SHRINK);
						m_tvbox.pack_start(m_tpsulabel);
						m_tvbox.pack_start(m_tamblabel);
						m_tpsulabel.set_width_chars(label_width);
						m_tamblabel.set_width_chars(label_width);
	
	//Set up scales for graphs
	m_vgraph.m_units = "mV";
	m_vgraph.m_minScale = 0;
	m_vgraph.m_maxScale = 4000;
	m_vgraph.m_scaleBump = 1000;
	
	m_igraph.m_units = "mA";
	m_igraph.m_minScale = 0;
	m_igraph.m_maxScale = 1000;
	m_igraph.m_scaleBump = 250;
	
	m_pgraph.m_units = "mW";
	m_pgraph.m_minScale = 0;
	m_pgraph.m_maxScale = 2500;
	m_pgraph.m_scaleBump = 500;
	
	m_tgraph.m_units = "C";
	m_tgraph.m_minScale = 0;
	m_tgraph.m_maxScale = 100;
	m_tgraph.m_scaleBump = 20;
	
	//Add data series to graphs
	m_vgraph.m_seriesName = "voltage";
	m_vgraph.m_series.push_back(&m_3v3);
	m_vgraph.m_series.push_back(&m_2v5);
	m_vgraph.m_series.push_back(&m_1v2);
	
	m_igraph.m_seriesName = "current";
	m_igraph.m_series.push_back(&m_3v3);
	m_igraph.m_series.push_back(&m_2v5);
	m_igraph.m_series.push_back(&m_1v2);
	
	m_pgraph.m_seriesName = "power";
	m_pgraph.m_series.push_back(&m_3v3);
	m_pgraph.m_series.push_back(&m_2v5);
	m_pgraph.m_series.push_back(&m_1v2);
	
	m_tgraph.m_seriesName = "temperature";
	m_tgraph.m_series.push_back(&m_tpsu);
	m_tgraph.m_series.push_back(&m_tamb);
	
	//Set up colors on graphs
	m_3v3.m_color.set_rgb_p(0.0f, 0.5f, 0.0f);
	m_2v5.m_color.set_rgb_p(0.0f, 0.0f, 1.0f);
	m_1v2.m_color.set_rgb_p(0.5f, 0.0f, 0.0f);
	m_tpsu.m_color.set_rgb_p(0.5f, 0.0f, 0.0f);
	m_tamb.m_color.set_rgb_p(0.0f, 0.0f, 1.0f);
	
	pthread_t thread;
	pthread_create(&thread, NULL, UartThreadProc, this);
		
	//Set up viewport
	show_all();
	*/
}

