////////////////////////////////////////////////////
// Plugin.cpp
// Standard implementation file for all AmiBroker plug-ins
//
// Copyright (C)2001-2006 Tomasz Janeczko, amibroker.com
// All rights reserved.
//
// Last modified: 2006-07-21 TJ
// 
// You may use this code in your own projects provided that:
//
// 1. You are registered user of AmiBroker
// 2. The software you write using it is for personal, noncommercial use only
//
// For commercial use you have to obtain a separate license from Amibroker.com
//
////////////////////////////////////////////////////

#include "pch.h"
#include "Plugin.h"
#include "resource.h"
#include "PluginManager.h"
#include "Application.h"
#include "IntervalType.h"
#include <cpprest/http_client.h>
#include "Utils.h"
#include <time.h>

// These are the only two lines you need to change
#define PLUGIN_NAME "CB180080-SOICT-BKHN Data Plug-in"
#define PLUGIN_NAME_AFL "CB180080-SOICT-BKHN AFL Plug-in"
#define VENDOR_NAME "Amibroker.com"
#define PLUGIN_VERSION 10400
#define PLUGIN_ID PIDCODE( 'C', 'B', '1', '8' )

// NOTE: we don't use
// "ODBC" id code because some 3rd party group used it in their plugin
// and we don't need conflicts if someone installed that before

#define THIS_PLUGIN_TYPE PLUGIN_TYPE_DATA

////////////////////////////////////////
// Data section
////////////////////////////////////////
static struct PluginInfo oPluginInfo =
{
		sizeof( struct PluginInfo ),
		PLUGIN_TYPE_DATA,		
		PLUGIN_VERSION,
		PLUGIN_ID,
		PLUGIN_NAME,
		VENDOR_NAME,
		0,
		387000
};

static struct PluginInfo oPluginInfoAfl =
{
		sizeof( struct PluginInfo ),
		PLUGIN_TYPE_AFL,		
		PLUGIN_VERSION,
		PLUGIN_ID,
		PLUGIN_NAME_AFL,
		VENDOR_NAME,
		0,
		387000
};


// the site interface for callbacks
struct SiteInterface gSite;


///////////////////////////////////////////////////////////
// Basic plug-in interface functions exported by DLL
///////////////////////////////////////////////////////////

PLUGINAPI int GetPluginInfo( struct PluginInfo *pInfo ) 
{ 
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );
	*pInfo = oPluginInfo;

	return TRUE;
	//char szThisPluginName[ _MAX_PATH ];

	//GetModuleFileName( AfxGetInstanceHandle(), szThisPluginName, sizeof( szThisPluginName ) );

	

	return TRUE;
}



PLUGINAPI int Init(void) 
{ 
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	return 1;
}	 

PLUGINAPI int Release(void) 
{ 
	return 1; 	  // default implementation does nothing
} 

PLUGINAPI int SetSiteInterface( struct SiteInterface *pInterface )
{
	gSite = *pInterface;

	return TRUE;
}


//PLUGINAPI int GetFunctionTable( FunctionTag **ppFunctionTable )
//{
//	*ppFunctionTable = gFunctionTable;
//
//	// must return the number of functions in the table
//	return gFunctionTableSize;
//}

#define MY_TIMER_ID 5678

VOID CALLBACK TimerFunc(	HWND hwnd,     // handle of window for timer messages
							UINT uMsg,     // WM_TIMER message
							UINT idEvent,  // timer identifier
							DWORD dwTime   // current system time
)
{
	/*if( idEvent == MY_TIMER_ID )
	{
		PostMessage( g_oData.m_hAmiBrokerWnd, WM_USER_STREAMING_UPDATE, 0, 0 );
	}*/

}

PLUGINAPI int Notify(struct PluginNotification *pn) 
{ 
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	///////////////////
	// Check if Data manager is running
	//////////////////
	if( ( pn->nReason & REASON_DATABASE_LOADED ) )
	{
		PluginManager::GetInstance()->SetHwnd(pn->hMainWnd);
	}

	if( pn->nReason & REASON_DATABASE_UNLOADED ) 
	{
		
	}
	return 1;
}	 

// GetQuotes wrapper for LEGACY format support
// convert back and forth between old and new format
//
// WARNING: it is highly inefficient and should be avoided
// So this is left just for maintaning compatibility,
// not for performance
// 
PLUGINAPI int GetQuotes( LPCTSTR pszTicker, int nPeriodicity, int nLastValid,   int nSize, struct QuotationFormat4 *pQuotes )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Quotation* pQuote5 = (struct Quotation*)malloc(nSize * sizeof(Quotation));

	QuotationFormat4* src = pQuotes;
	Quotation* dst = pQuote5;

	int i;

	for (i = 0; i <= nLastValid; i++, src++, dst++)
	{
		ConvertFormat4Quote(src, dst);
	}

	int nQty = GetQuotesEx(pszTicker, nPeriodicity, nLastValid, nSize, pQuote5, NULL);

	dst = pQuote5;
	src = pQuotes;

	for (i = 0; i < nQty; i++, dst++, src++)
	{
		ConvertFormat5Quote(dst, src);
	}

	free(pQuote5);

	return nQty;
}

//////////////////////////////////////
// GetQuotesEx is a most important function
// for every data plugin
// it is called by AmiBroker everytime AmiBroker
// needs new data for given symbol.
//
// Internally AmiBroker caches response obtained
// from GetQuotes function but you may force it to 
// get new data by sending appropriate message to AmiBroker
// main window.
//
//
// When AmiBroker calls GetQuotes function it allocates
// array of quotations of size equal to default number of bars as set in
// File->Database Settings,
// and fills the array with quotes that are already present in the
// database.
// Filled area covers array elements from zero to nLastValid
//
// In your DLL you can update the array with more recent quotes.
// Depending on the data source you can either fill entire array
// from the scratch (Metastock, TC2000, QP2 plugins do that)
// or just add/update a few recent bars and leave the remaining bars
// untouched (eSignal, myTrack, QuoteTracker plugins do that)
static void extract_quotation(struct Quotation* p, const web::json::value& json) {
	if (json.has_field(U("highest"))) p->High = json.at(U("highest")).as_double();
	if (json.has_field(U("open"))) p->Open = json.at(U("open")).as_double();
	if (json.has_field(U("close"))) p->Price = json.at(U("close")).as_double();
	if (json.has_field(U("lowest"))) p->Low = json.at(U("lowest")).as_double();
	if (json.has_field(U("volume"))) p->Volume = json.at(U("volume")).as_double();
	if (json.has_field(U("tradingdate")))
	{
		const web::json::value td = json.at(U("tradingdate"));
		time_t secsSinceEpoch = td.at(U("time")).as_number().to_int64() /1000;
		tm time;
		auto error = localtime_s(&time, &secsSinceEpoch);
		p->DateTime.PackDate.Year = time.tm_year+1900;
		p->DateTime.PackDate.Month = time.tm_mon+1;
		p->DateTime.PackDate.Day = time.tm_mday;
		p->DateTime.PackDate.Hour = time.tm_hour;
		p->DateTime.PackDate.Minute = time.tm_min;
		p->DateTime.PackDate.Second = time.tm_sec;
	}
}
static std::map<std::pair<std::string, EIntervalType>, web::json::array> mData;
PLUGINAPI int GetQuotesEx(LPCTSTR pszTicker, int nPeriodicity, int nLastValid, int nSize, struct Quotation* pQuotes, GQEContext* pContext)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CB180080::Application::GetInstance()->StartOLE();
	auto document = Application::GetInstance()->GetActiveDocument();
	int period = nPeriodicity;
	if (document)
		period = document->GetInterval();
	delete document;
	CB180080::Application::GetInstance()->EndOLE();
	if (period == 0)
		period = nPeriodicity;
	EIntervalType interval = EIntervalType::Daily;
	if (period < 24 * 60 * 60)
		interval = EIntervalType::Intraday;
	auto it_data = mData.find(std::make_pair(pszTicker, interval));
	if (it_data != mData.end())
	{
		auto data = it_data->second;
		int i = 0;
		for (auto it = data.begin(); it != data.end(); it++) {
			if (i >= nSize)
				break;
			struct Quotation* p = &pQuotes[i];
			extract_quotation(p, *it);
			i++;
		}
		return i;
	}
	web::http::client::http_client client(U("http://128.199.197.161:8888/"));
	web::uri_builder builder(U("/trading/amibroker"));
	web::json::value json_v;
	json_v[U("symbol")] = web::json::value::string(Utils::S2WS(pszTicker));
	json_v[U("interval")] = web::json::value::number(period);
	json_v[U("from")] = web::json::value::string(U("01/01/1990"));
	json_v[U("to")] = web::json::value::string(U("01/01/2100"));
	try {
		auto res = client.request(web::http::methods::POST, builder.to_string(), json_v).get();
		if (res.status_code() == web::http::status_codes::OK)
		{
			auto rs = res.extract_json().get();
			int code = -1;
			if (rs.has_field(U("code"))) code = rs.at(U("code")).as_integer();
			if (code == 0 && rs.has_field(U("data"))) {
				auto data = rs.at(U("data")).as_array();
				mData.insert_or_assign(std::make_pair(pszTicker, interval), data);
				int i = 0;
				for (auto it = data.begin(); it != data.end(); it++) {
					if (i >= nSize)
						break;
					struct Quotation* p = &pQuotes[i];
					extract_quotation(p, *it);
					i++;
				}
				return i;
			}
		}
		
	}
	catch (std::exception ex) {

	}
	return nLastValid+1;
}
PLUGINAPI int Configure( LPCTSTR pszPath, struct InfoSite *pSite )
{
	// AFX_MANAGE_STATE is needed because dialog boxes call MFC and use resources
	// similar line should be added to any other function exported by the plugin
	// if only the function calls MFC or uses DLL resources
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  
	

	return 1;
}

PLUGINAPI AmiVar GetExtraData( LPCTSTR pszTicker, LPCTSTR pszName, int nArraySize, int nPeriodicity, void* (*pfAlloc)(unsigned int nSize) )
{
	// default implementation does nothing

	AmiVar var;

	var.type = VAR_NONE;
	var.val = 0;
	
	return var;
}

PLUGINAPI int SetTimeBase( int nTimeBase )
{
	// only intraday intervals are supported
	return 1;//return ( nTimeBase < ( 24 * 60 * 60 ) )  ? 1 : 0;
}

