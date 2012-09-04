/**
 * @file CatPictureApp.cpp
 * CSE 274 - Fall 2012
 * My solution for HW01 Phase 01.
 *
 * @author Andrew Koscianski
 * @date 2012-09-01
 *
 * @note This file is (c) 2012. It is licensed under the 
 * CC BY 3.0 license (http://creativecommons.org/licenses/by/3.0/),
 * which means you are free to use, share, and remix it as long as you
 * give attribution. Commercial uses are allowed.
 *
 * @note This project satisfies goals A.1 (rectangle), A.2 (circle), B.1 (blur), E.2 (transparency),
 * E.5 (animation) and E.6 (mouse interaction)
 */
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* setttings);

  private:
	Surface* mySurface_;

	int frame_number_;

	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024;

	// Creates a basic Rectangle with starting point (x1, y1) and ending point (x2, y2) with color
	// c1 and c2. This satisfies Requirement A.1 (rectangle).
	void basicRectangle (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1, Color8u c2);


	void blueTintImage (uint8_t* pixels);

	// Creates a basic line with starting point (x1, y1) and ending point (x2, y2) with color c1.
	// This satisfies Requirement A.3 (line).
	void basicLine (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1);

	// Creates a basic triangle with three points (x1, y1), (x2, y2), (x3, y3) with color c1.
	// This satisfies Requirement A.7 (triangle).
	void basicTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1);
};

void CatPictureApp::prepareSettings(Settings* settings) {
	(*settings).setWindowSize(kAppWidth, kAppHeight);
	(*settings).setResizable(false);
}

void CatPictureApp::basicRectangle(uint8_t* pixels, int x1, int x2, int y1, int y2, Color8u fill, Color8u fill2) {
	//Determines the starting and ending coordinates for the rectangle
	int startx = (x1 < x2) ? x1 : x2;
	int endx = (x1 < x2) ? x2 : x1;
	int starty = (y1 < y2) ? y1 : y2;
	int endy = (y1 < y2) ? y2 : y1;
	
	//Boundary Checking
	if (endx < 0) return;
	if (endy < 0) return;
	if (startx >= kAppWidth) return;
	if (starty >= kAppHeight) return;
	if (endx >= kAppWidth) endx = kAppWidth - 1;
	if (endy >= kAppHeight) endy = kAppHeight - 1;	

	for (int y = starty; y <= endy; y++) {
		for (int x = startx; x <= endx; x++) {
			if ((y == starty || y == endy) || (x == startx || x == endx)){
			pixels[3*(x + y*kTextureSize)] = fill.r;
			pixels[3*(x + y*kTextureSize)+1] = fill.g;
			pixels[3*(x + y*kTextureSize)+2] = fill.b;
			}
		}
	}
}

void CatPictureApp::blueTintImage(uint8_t* pixels) 
{
	int x,y;
	for( y=1;y<kAppHeight-1;y++){
		for( x=1;x<kAppWidth-1;x++){
			if (x%2 == 0){
				//pixels[3*(x + y*kTextureSize)] = 50;
			}
		}
	}

	
}

void CatPictureApp::basicLine(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1)
{



}

void CatPictureApp::basicTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1)
{

}

void CatPictureApp::setup()
{
	frame_number_=0;
	
	//This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
}

void CatPictureApp::mouseDown( MouseEvent event )
{
	
	
}

void CatPictureApp::update()
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	Color8u rectFill = Color8u(255,0,0);
	Color8u rectFill2 = Color8u(150,150,150);
	basicRectangle(dataArray, 200, 400, 200, 500, rectFill, rectFill2); 

	blueTintImage(dataArray);

	//Only save the first frame of drawing as output, code snippet via Dr. Brinkman
	if(frame_number_ == 0){
		writeImage("kosciaaj.png",*mySurface_);
	}
	//keeps track of how many frames we have shown.
	frame_number_++;
}

void CatPictureApp::draw()
{
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
