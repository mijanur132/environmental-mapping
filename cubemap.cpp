#include "stdafx.h"
#include "cubemap.h"
#include"matrix.h"

cubemap::cubemap(int _w, int _h)
{
	w = _w;
	h = _h;	
	facel = w / 3;
	faceh = h / 4;
	defaultCam =new PPC(hfov, w/3, h / 4);
	cam0=new PPC(hfov, w / 3, h / 4);
	cam1 = new PPC(hfov, w / 3, h / 4);
	cam2 = new PPC(hfov, w / 3, h / 4);
	cam3 = new PPC(hfov, w / 3, h / 4);
	cam4 = new PPC(hfov, w / 3, h / 4);
	cam5 = new PPC(hfov, w / 3, h / 4);
	cam0->Tilt(90.0f);
	cam4->Tilt(-90.0f);
	cam1->PanLeftRight(90.0f);
	cam3->PanLeftRight(-90.0f);
	cam4->PanLeftRight(180.0f);
	for (int i = 0; i < 6; i++)
	{	

		matrix face(w/3,h/4);
		cubefaces.push_back(face);
		
	}

	
}

cubemap::cubemap(FrameBuffer* fb)
{
	w = fb->w;
	h = fb->h;
	facel = w / 3;
	faceh = h / 4;
	defaultCam = new PPC(hfov, w / 3, h / 4);
	cam0 = new PPC(hfov, w / 3, h / 4);
	cam1 = new PPC(hfov, w / 3, h / 4);
	cam2 = new PPC(hfov, w / 3, h / 4);
	cam3 = new PPC(hfov, w / 3, h / 4);
	cam4 = new PPC(hfov, w / 3, h / 4);
	cam5 = new PPC(hfov, w / 3, h / 4);
	cam0->Tilt(90.0f);
	cam4->Tilt(-90.0f);
	cam1->PanLeftRight(90.0f);
	cam3->PanLeftRight(-90.0f);
	cam5->PanLeftRight(180.0f);
	matrix temp(w, h);
	temp.fbPix2mat(fb);
	int index = 0;
	int index2 = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (index==1 || index==3|| index==4 || index==5 || index==7 || index==10)
			{
				
				matrix face = temp.split(4, 3, i, j);
				if (index2 == 5) {
				
					matrix reartemp(facel, faceh);
					reartemp.copyDatafrom(&face);
					for (int ih = 0; ih < faceh; ih++)
					{
						face.imageVec[ih] = reartemp.imageVec[faceh-1 - ih];
						
					}

				
				}
				cubefaces.push_back(face);
				
				index2++;
			}
			index++;
			
		}

	}	


}
void cubemap::cmface2fb(FrameBuffer* fb, int faceI) {

	cubefaces[faceI].mat2fbPix(fb);

}

matrix cubemap::envmap(PPC* viewCam) {
	matrix temp(viewCam->w, viewCam->h);
	
	for (int ih = 0; ih < viewCam->h; ih++)
	{
		for (int iw = 0; iw < viewCam->w; iw++)
		{
			V3 rayVec=viewCam->GetRayVector(iw, ih);
			V3 pp = cam0->lookAtRayVecDir(rayVec);	
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp.imageVec[ih][iw] = cubefaces[0].imageVec[pp[1]][pp[0]];
				continue;
			}
					
			
			pp = cam1->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp.imageVec[ih][iw] = cubefaces[1].imageVec[pp[1]][pp[0]];
				continue;
			}

			
			pp = cam2->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp.imageVec[ih][iw] = cubefaces[2].imageVec[pp[1]][pp[0]];
				continue;
			}
			
			pp = cam3->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp.imageVec[ih][iw] = cubefaces[3].imageVec[pp[1]][pp[0]];
				continue;
			}
			
			pp = cam4->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp.imageVec[ih][iw] = cubefaces[4].imageVec[pp[1]][pp[0]];
				continue;
			}
			
			pp = cam5->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp.imageVec[ih][iw] = cubefaces[5].imageVec[pp[1]][pp[0]];				
			}

		}

	}

	return temp;

}
unsigned int cubemap::envmap4mRay(V3 rayVec) {
	

	unsigned int temp;
			
			V3 pp = cam0->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp = cubefaces[0].imageVec[pp[1]][pp[0]];				
			}


			pp = cam1->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp = cubefaces[1].imageVec[pp[1]][pp[0]];
				
			}


			pp = cam2->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp = cubefaces[2].imageVec[pp[1]][pp[0]];
			
			}

			pp = cam3->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp = cubefaces[3].imageVec[pp[1]][pp[0]];
				
			}

			pp = cam4->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp = cubefaces[4].imageVec[pp[1]][pp[0]];
				
			}

			pp = cam5->lookAtRayVecDir(rayVec);
			if (pp[0] > 0 && pp[1] > 0 && pp[0] < faceh && pp[1] < facel)
			{
				temp = cubefaces[5].imageVec[pp[1]][pp[0]];
			}

			return temp;



}
