#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "ppc.h"
#include "TMesh.h"
#include "texture.h"
#include "cubemap.h"


class Scene {
public:

	GUI *gui;
	FrameBuffer *fb;
	FrameBuffer* fbBB;
	FrameBuffer* fb0, *fb1, *fb3, *fb4, *fbEnv;
	PPC* ppc, *ppc0,*ppc1, *LightSrcPPC, * LightSrcPPC1, * LightSrcPPC2, * LightSrcPPC3;
	PPC* ppcBB;
	TMesh* tmeshes;
	int tmeshesN;
	int globalIndex4dbg = 0;
	texture *t1, *t2, *t3, *t4, *t5;
	//int texesN;
	float vf;
	float hfov = 90.0f;
	Scene();
	void DBG();
	void NewButton();
	void Render();
	void Rendercubemap();
	void Renderenvmap();
	void Render(FrameBuffer* rfb, PPC* rppc);
	void Render(FrameBuffer* rfb, PPC* rppc, cubemap* cm);
	void RenderProjector(FrameBuffer* fb0, FrameBuffer* fb1,PPC* ppc0, PPC* ppc1);
//	void Render(FrameBuffer* fb, PPC* ppc,TMesh* tmesh);
	V3 L,L1,L2,L3,f; // point light source
	float ka; // ambient lighting coefficient
};

extern Scene *scene;