#include "stdafx.h"

#include "framebuffer.h"
#include "math.h"
#include <iostream>
#include "scene.h"

#include <tiffio.h>


using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0,
	int _w, int _h, unsigned int _id) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w*h];
	pix1 = new unsigned int[w * h];
	zb = new float[w*h];
	zbL1 = new float[w * h];
	zbL2 = new float[w * h];
	zbL3 = new float[w * h];
	zbL4 = new float[w * h];


}

void FrameBuffer::ClearZB() {

	for (int uv = 0; uv < w*h; uv++)
		zb[uv] = -5000.0f;

}

void FrameBuffer::ClearZB(float* zb1) {

	for (int uv = 0; uv < w * h; uv++)
		zb1[uv] = -5000.0f;

}

int FrameBuffer::getPixelIndex(int u, int v)
{
	if (u >= w) {
		//	cout << u << endl;
		u = u % w;
		//cout << "new U:" << u << endl;
	}
	if (v >= h)
	{
		//	cout << v << endl;
		v = v % h;
		//	cout << "new U:" << v << endl;
	}

	int id = u + v * w;
	return id;

}

void FrameBuffer::copyPix(unsigned int* pix, unsigned int* pix1) {

	for (int uv = 0; uv < w * h; uv++)
		pix1[uv] = pix[uv];
	
}

void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}

int FrameBuffer::handle(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	case FL_MOVE: {
		int u = Fl::event_x();
		int v = Fl::event_y();
		cerr << u << " " << v << "        \r";
		return 0;
	}
	default:
		return 0;
	}
	return 0;
}

void FrameBuffer::KeyboardHandle() {

	int key = Fl::event_key();
	switch (key) {
	case FL_Up: {
		cerr << "INFO: pressed up key";
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}


void FrameBuffer::Set(int u, int v, unsigned int color) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return;

	pix[(h - 1 - v)*w + u] = color;

}

void FrameBuffer::Set(int u, int v, unsigned int * pix, unsigned int color) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return;

	pix[(h - 1 - v) * w + u] = color;

}

void FrameBuffer::SetBGR(unsigned int bgr) {

	for (int uv = 0; uv < w*h; uv++)
		pix[uv] = bgr;

}

// load a tiff image to pixel buffer
void FrameBuffer::LoadTiff(char* fname) {
	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (w != width || h != height) {
		w = width;
		h = height;
		delete[] pix;
		pix = new unsigned int[w*h];
		size(w, h);
		glFlush();
		glFlush();
	}

	if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}

// save as tiff image
void FrameBuffer::SaveAsTiff(char *fname) {

	TIFF* out = TIFFOpen(fname, "w");

	if (out == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	for (uint32 row = 0; row < (unsigned int)h; row++) {
		TIFFWriteScanline(out, &pix[(h - row - 1) * w], row);
	}

	TIFFClose(out);
}

void FrameBuffer::SetChecker(unsigned int col0, unsigned int col1, int csize) {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int cv = v / csize;
			int cu = u / csize;
			if ((cu + cv) % 2) {
				Set(u, v, col0);
			}
			else {
				Set(u, v, col1);
			}
		}
	}

}

int FrameBuffer::Farther(int u, int v, float currz) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 1;
	int uv = (h - 1 - v)*w + u;
	if (currz < zb[uv])
		return 1;
	zb[uv] = currz;
	return 0;

}

int FrameBuffer::FartherLightZ(float* zb1, int u, int v, float currz) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 1;
	int uv = (h - 1 - v) * w + u;
	if (currz < zbL1[uv])
		return 1;
	zbL1[uv] = currz;
	return 0;

}
int FrameBuffer::FartherLightZCompare(float* zb1, int u, int v, float currz) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 1;
	int uv = (h - 1 - v) * w + u;
	//if (currz+2 < zbL1[uv] )
	if (currz +02.25 < zbL1[uv])
		return 1;
	else {
		return 0;
	}

}



void FrameBuffer::Draw2DSegment(V3 p0, V3 c0, V3 p1, V3 c1) {

	float du = fabsf((p0 - p1)[0]);
	float dv = fabsf((p0 - p1)[1]);
	int stepsN;
	if (du < dv) {
		stepsN = 1+(int)dv;
	}
	else {
		stepsN = 1+(int)du;
	}
	for (int i = 0; i <= stepsN; i++) {
		V3 cp, cc;
		cp = p0 + (p1 - p0) * (float)i / (float)stepsN;
		// cp[2] depth (one over w) at current pixel
		int u = (int)cp[0], v = (int)cp[1];
		if (Farther(u, v, cp[2]))
			continue;
		cc = c0 + (c1 - c0) * (float)i / (float)stepsN;
		Set(u, v, cc.GetColor());
	}

}

void FrameBuffer::DrawSquarePoint(float uf, float vf, int psize, unsigned int color) {

	int u = (int)uf;
	int v = (int)vf;
	for (int cv = v - psize / 2; cv <= v + psize / 2; cv++)
		for (int cu = u - psize / 2; cu <= u + psize / 2; cu++)
			Set(cu, cv, color);
}


void FrameBuffer::Draw3DSegment(V3 P0, V3 P1, PPC *ppc, V3 c0, V3 c1) {

	V3 p0, p1;
	if (!ppc->Project(P0, p0))
		return;
	if (!ppc->Project(P1, p1))
		return;
	cout << p0 << endl;
	Draw2DSegment(p0, c0, p1, c1);

}

void FrameBuffer::Draw3DPoint(V3 P, PPC *ppc, unsigned int color, int psize) {


	V3 pP;
	if (!ppc->Project(P, pP))
		return;

	int u = (int)pP[0];
	int v = (int)pP[1];
	for (int cv = v - psize / 2; cv <= v + psize / 2; cv++) {
		for (int cu = u - psize / 2; cu <= u + psize / 2; cu++) {
			if (Farther(cu, cv, pP[2]))
				continue;
			Set(cu, cv, color);
		}
	}

}

unsigned int FrameBuffer::Get(int u, int v) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 0;
	return pix[(h - 1 - v)*w + u];

}

unsigned int FrameBuffer::Get(int u, int v,unsigned int * pix) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 0;
	return pix[(h - 1 - v) * w + u];

}

float FrameBuffer::GetZ(int u, int v) {
	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 0;
	return zb[(h - 1 - v)*w + u];

}

void FrameBuffer::showTextureImageAsUploaded(texture* t1) {

	
	for (int i = 0; i < t1->w; i++)
	{
		for (int j = 0; j < t1->h; j++)
		{
			unsigned int color_ij = i + j * t1->w;
			Set(i, j, t1->pix[color_ij]);

		}

	}

}

void FrameBuffer::copy2tex(texture* t1) {
	copyPix(pix, t1->pix);
}