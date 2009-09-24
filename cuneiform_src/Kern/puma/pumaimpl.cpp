/*
 * pumaimpl.cpp
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#include "pumaimpl.h"
#include "puma.h"
#include "mpuma.h"
#include "pumadef.h"

#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

#ifndef NDEBUG
#define DBG(msg) cerr << msg << endl;
#else
#define DBG(msg) ;
#endif

static Handle ghStorage = NULL;

namespace CIF {

unsigned char * PumaImpl::main_buffer_ = 0;
unsigned char * PumaImpl::work_buffer_ = 0;

static int32_t s_ConsoleLine = 0;

bool PumaImpl::preOpenInitialize() {
	bool rc = true;
	// Удалим предыдущие окна отладки.
	Handle hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_DELLINE);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_BINARIZE);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_INPUT);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(PUMA_IMAGE_TURN);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_ORTOMOVE);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);

	PumaImpl::close();
	ResetPRGTIME();
	if (LDPUMA_Skip(hDebugRoot)) {
		if (s_ConsoleLine)
			LDPUMA_ConsoleClear(s_ConsoleLine);
		s_ConsoleLine = LDPUMA_ConsoleGetCurLine();
	} else {

	}
	SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
	SetReturnCode_puma(IDS_ERR_NO);
	return rc;
}

bool PumaImpl::postOpenInitialize(const char * lpFileName) {
	bool rc = true;
	CIMAGEBITMAPINFOHEADER info;
	if (lpFileName)
		LDPUMA_SetFileName(NULL, lpFileName);
	if (!CIMAGE_GetImageInfo((puchar) PUMA_IMAGE_USER, &info)) {
		SetReturnCode_puma(CIMAGE_GetReturnCode());
		rc = false;
	} else {
		gRectTemplate.left = 0;
		gRectTemplate.right = info.biWidth;
		gRectTemplate.top = 0;
		gRectTemplate.bottom = info.biHeight;
	}
	if (lpFileName) {
		szInputFileName = lpFileName;
		strcpy(szLayoutFileName, lpFileName);
		char * s = strrchr(szLayoutFileName, '.');
		if (s)
			*s = 0;
		strcat(szLayoutFileName, ".bin");
	} else
		szInputFileName = "";
	hCPAGE = CreateEmptyPage();
	return rc;
}

PumaImpl::PumaImpl() {
	initMainBuffer();
	initWorkBuffer();
	if (!ModulesInit((void*) ghStorage))
		throw PumaException("Puma init failed");
}

PumaImpl::~PumaImpl() {
	ModulesDone();
	freeMainBuffer();
	freeWorkBuffer();
}

void PumaImpl::analyze() {
	if (!PUMA_XPageAnalysis())
		throw PumaException("Puma analyze failed");
}

void PumaImpl::close() {
	DBG("Puma close")
	CLINE_Reset();
	ClearAll();
	// clean
	CIMAGE_Reset();
	CPAGE_DeleteAll();
	RIMAGE_Reset();
	hCPAGE = NULL;
	gpRecogDIB = gpInputDIB = NULL;
}

void PumaImpl::freeMainBuffer() {
	if (!main_buffer_)
		return;
	delete[] main_buffer_;
	main_buffer_ = NULL;
}

void PumaImpl::freeWorkBuffer() {
	if (!work_buffer_)
		return;
	delete[] work_buffer_;
	work_buffer_ = NULL;
}

void PumaImpl::initMainBuffer() {
	if (main_buffer_) {
		DBG("PumaImpl: main buffer is not empty")
		delete[] main_buffer_;
	}
	main_buffer_ = new unsigned char[MainBufferSize];
}

void PumaImpl::initWorkBuffer() {
	if (work_buffer_) {
		DBG ("PumaImpl: work buffer is not empty")
		delete[] work_buffer_;
	}
	work_buffer_ = new uchar[WorkBufferSize];
}

void PumaImpl::open(char * dib) {
	DBG("Puma open")
	assert(dib);
	preOpenInitialize();
	gpInputDIB = (unsigned char*) dib;
	// write image
	if (!CIMAGE_WriteDIB((puchar) PUMA_IMAGE_USER, dib, 1))
		throw PumaException("PumaImpl::open can't write DIB");

	if (!postOpenInitialize("none.txt"))
		throw PumaException("Puma post open init failed");
}

void PumaImpl::recognize() {
	if (!PUMA_XFinalRecognition())
		throw PumaException("Puma recognition failed");
	DBG("Puma recognize")
}

void PumaImpl::save(const std::string& filename, int Format) const {
#ifndef NDEBUG
	cerr << "Puma save to: " << filename << endl;
#endif

	if (!PUMA_XSave(filename, static_cast<puma_format_t> (Format),
			PUMA_CODE_UTF8))
		throw PumaException("Puma save failed");
}

unsigned char * PumaImpl::mainBuffer() {
	assert(main_buffer_);
	return main_buffer_;
}

unsigned char * PumaImpl::workBuffer() {
	assert(work_buffer_);
	return work_buffer_;
}

}
