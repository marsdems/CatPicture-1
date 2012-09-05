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
	void mouseDown(MouseEvent event);	
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

	void CatPictureApp::blur(uint8_t* pixels);


    void basicCircle (uint8_t* pixels, int x, int y, int r, Color8u col);

	// Creates a basic line with starting point (x1, y1) and ending point (x2, y2) with color c1.
	// This satisfies Requirement A.3 (line).
	void basicLine (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u col);

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
	//My attempt at concentric rectangles with alternating colors. Only works properly with squares. 
	for (int y = starty; y <= endy; y++) {
		for (int x = startx; x <= endx; x++) {
			if ((y == starty || y == endy) || (x == startx || x == endx) || (y%2==0 && x>y && x+y <= endx) || (x%2==0 && x>y && x+y >= endx) 
			|| (x%2==0 && x<y && x+y <= endx) || (y%2==0 && x<y && x+y >= endx) || (x==y) && x%2==0){
			pixels[3*(x + y*kTextureSize)] = fill.r;
			pixels[3*(x + y*kTextureSize)+1] = fill.g;
			pixels[3*(x + y*kTextureSize)+2] = fill.b;
			}
		}

	}
	for (int y = starty; y <= endy; y++) {
		for (int x = startx; x <= endx; x++) {
			if (((y+1)%2==0 && x>y && x+y <= endx) || ((x+1)%2==0 && x>y && x+y >= endx) 
			|| ((x+1)%2==0 && x<y && x+y <= endx) || ((y+1)%2==0 && x<y && x+y >= endx) || (x==y) && (x+1)%2==0){
			pixels[3*(x + y*kTextureSize)] = fill2.r;
			pixels[3*(x + y*kTextureSize)+1] = fill2.g;
			pixels[3*(x + y*kTextureSize)+2] = fill2.b;
			}
		}

	}
}

void CatPictureApp::basicCircle (uint8_t* pixels, int x, int y, int r, Color8u col){
	for(int y1 = y - r; y1 <= y + r; y1++)
	{
		for(int x1 = x - r; x1 <= x + r; x1++)
		{
			//Bounds test, to make sure we don't access array out of bounds
			if(y1 < 0 || x1 < 0 || x1 >= kAppWidth || y1 >= kAppHeight) continue;
			int dist = (int)sqrt((double)((x1-x)*(x1-x) + (y1-y)*(y1-y)));
			if(dist <= r)
			{
		    pixels[3*(x1 + y1*kTextureSize)] = col.r;
			pixels[3*(x1 + y1*kTextureSize)+1] =col.g;
			pixels[3*(x1 + y1*kTextureSize)+2] = col.b;	
			}
		}
	}
}

void CatPictureApp::basicLine(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u col)
{
	//Determines the starting and ending coordinates for the line
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

int F, x, y;

    // Handle trivial cases separately for algorithm speed up.
    // Trivial case 1: m = +/-INF (Vertical line)
    if (x1 == x2)
    {
        if (y1 > y2)  // Swap y-coordinates if p1 is above p2
        {
            swap(y1, y2);
        }

        x = x1;
        y = y1;
        while (y <= y2)
        {
            pixels[3*(x + y*kTextureSize)] = col.r;
			pixels[3*(x + y*kTextureSize)+1] = col.g;
			pixels[3*(x + y*kTextureSize)+2] = col.b;
            y++;
        }
        return;
    }
    // Trivial case 2: m = 0 (Horizontal line)
    else if (y1 == y2)
    {
        x = x1;
        y = y1;

        while (x <= x2)
        {
            pixels[3*(x + y*kTextureSize)] = col.r;
			pixels[3*(x + y*kTextureSize)+1] = col.g;
			pixels[3*(x + y*kTextureSize)+2] = col.b;
            x++;
        }
        return;
    }


    int dy            = y2 - y1;  // y-increment from p1 to p2
    int dx            = x2 - x1;  // x-increment from p1 to p2
    int dy2           = (dy << 1);  // dy << 1 == 2*dy
    int dx2           = (dx << 1);
    int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
    int dy2_plus_dx2  = dy2 + dx2;


    if (dy >= 0)    // m >= 0
    {
        // Case 1: 0 <= m <= 1 (Original case)
        if (dy <= dx)   
        {
            F = dy2 - dx;    // initial F

            x = x1;
            y = y1;
            while (x <= x2)
            {
            pixels[3*(x + y*kTextureSize)] = col.r;
			pixels[3*(x + y*kTextureSize)+1] = col.g;
			pixels[3*(x + y*kTextureSize)+2] = col.b;
                if (F <= 0)
                {
                    F += dy2;
                }
                else
                {
                    y++;
                    F += dy2_minus_dx2;
                }
                x++;
            }
        }
        // Case 2: 1 < m < INF (Mirror about y=x line
        // replace all dy by dx and dx by dy)
        else
        {
            F = dx2 - dy;    // initial F

            y = y1;
            x = x1;
            while (y <= y2)
            {
           pixels[3*(x + y*kTextureSize)] = col.r;
			pixels[3*(x + y*kTextureSize)+1] = col.g;
			pixels[3*(x + y*kTextureSize)+2] = col.b;
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
    else    // m < 0
    {
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy)
        {
            F = -dy2 - dx;    // initial F

            x = x1;
            y = y1;
            while (x <= x2)
            {
            pixels[3*(x + y*kTextureSize)] = col.r;
			pixels[3*(x + y*kTextureSize)+1] = col.g;
			pixels[3*(x + y*kTextureSize)+2] = col.b;
                if (F <= 0)
                {
                    F -= dy2;
                }
                else
                {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror 
        // about y=x line, replace all dx by -dy and dy by dx)
        else    
        {
            F = dx2 + dy;    // initial F

            y = y1;
            x = x1;
            while (y >= y2)
            {
            pixels[3*(x + y*kTextureSize)] = col.r;
			pixels[3*(x + y*kTextureSize)+1] = col.g;
			pixels[3*(x + y*kTextureSize)+2] = col.b;
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }

}

void CatPictureApp::basicTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1)
{
	basicLine(pixels, x1, y1, x2, y2,c1);
	basicLine(pixels, x2, y2, x3, y3,c1);
	basicLine(pixels, x3, y3, x1, y1,c1);
}

void CatPictureApp::setup()
{
	frame_number_=0;
	
	//This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
}

void CatPictureApp::mouseDown(MouseEvent event)
{
	uint8_t* dataArray = (*mySurface_).getData();
	int x = event.getX();
	int y = event.getY();

	basicCircle(dataArray,x,y,20,Color8u(frame_number_+100,frame_number_,frame_number_+200));
	
}

void CatPictureApp::blur(uint8_t* pixels){

}

void CatPictureApp::update()
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData(); 
	Color8u rectFill = Color8u(frame_number_,0,0);
	Color8u rectFill2 = Color8u(0,0,frame_number_);
	Color8u circleCol = Color8u(0,frame_number_,0);
	Color8u gradient = Color8u(frame_number_+100, frame_number_+5, frame_number_+ 180);
	basicRectangle(dataArray, 500, 250, frame_number_+ 100, 200, rectFill, rectFill2); 
	basicLine(dataArray,5,10,150,300, rectFill);
	basicLine(dataArray,5,80,500,20, rectFill2);
	basicLine(dataArray,200,300,150,310, circleCol);
	basicLine(dataArray,(frame_number_)/10 + 700,0, (frame_number_)/10 + 700, 700, gradient);
	basicTriangle(dataArray, 50, frame_number_+100, 150, frame_number_, frame_number_+50, frame_number_+150, circleCol);
	basicCircle(dataArray, 700,250,frame_number_/8, rectFill);
	basicTriangle(dataArray, 0, 0, 0, 500, 150, 450, circleCol);
	//blur(dataArray);

	//Only save the first frame of drawing as output, code snippet via Dr. Brinkman
	if(frame_number_ == 0){
		writeImage("kosciaaj.png",*mySurface_);
	}
	//keeps track of how many frames we have shown.
	if (frame_number_ == 500){
		frame_number_=0;
	}
	frame_number_++;
}

void CatPictureApp::draw()
{
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
