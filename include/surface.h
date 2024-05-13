// Template, BUAS version https://www.buas.nl/games
// IGAD/BUAS(NHTV)/UU - Jacco Bikker - 2006-2020

#pragma once
#include <string_view>
#include "template.h"


namespace Tmpl8 {

constexpr int AlphaMask = 0xff000000;
constexpr int RedMask = 0x00ff0000;
constexpr int GreenMask = 0x0000ff00;
constexpr int BlueMask = 0x000000ff;

typedef unsigned int Pixel; // unsigned int is assumed to be 32-bit, which seems a safe assumption.


inline Pixel BlendColor(Pixel dest, Pixel source)
{
	float alpha = ((source & AlphaMask) >> 24)/255.f;
	float invAlpha = 1 - alpha;

	unsigned int r = static_cast<int>(alpha * ((source & RedMask) >> 16) + invAlpha * ((dest & RedMask) >> 16));
	unsigned int g = static_cast<int>(alpha * ((source & GreenMask) >> 8) + invAlpha * ((dest & GreenMask) >> 8));
	unsigned int b = static_cast<int>(alpha * (source & BlueMask) + invAlpha * (dest & BlueMask));

	return ((r<<16) | (g<<8) | b);
}


inline Pixel AddBlend( Pixel a_Color1, Pixel a_Color2 )
{
	const unsigned int r = (a_Color1 & RedMask) + (a_Color2 & RedMask);
	const unsigned int g = (a_Color1 & GreenMask) + (a_Color2 & GreenMask);
	const unsigned int b = (a_Color1 & BlueMask) + (a_Color2 & BlueMask);
	const unsigned r1 = (r & RedMask) | (RedMask * (r >> 24));
	const unsigned g1 = (g & GreenMask) | (GreenMask * (g >> 16));
	const unsigned b1 = (b & BlueMask) | (BlueMask * (b >> 8));
	return (r1 + g1 + b1);
}

// subtractive blending
inline Pixel SubBlend( Pixel a_Color1, Pixel a_Color2 )
{
	int red = (a_Color1 & RedMask) - (a_Color2 & RedMask);
	int green = (a_Color1 & GreenMask) - (a_Color2 & GreenMask);
	int blue = (a_Color1 & BlueMask) - (a_Color2 & BlueMask);
	if (red < 0) red = 0;
	if (green < 0) green = 0;
	if (blue < 0) blue = 0;
	return static_cast<Pixel>(red + green + blue);
}

class Surface
{
	enum { OWNER = 1 };
public:
	// constructor / destructor
	Surface( int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch );
	Surface( int a_Width, int a_Height );
	Surface( char* a_File );
	~Surface();
	// member data access
	Pixel* GetBuffer() { return m_Buffer; }
	void SetBuffer( Pixel* a_Buffer ) { m_Buffer = a_Buffer; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	int GetPitch() { return m_Pitch; }
	void SetPitch( int a_Pitch ) { m_Pitch = a_Pitch; }

	// Special operations
	void LoadImage(const char* a_File);						// Loads image from disk. Removes all previous data 
	void CopyTo(Surface* a_Dst, int a_X, int a_Y);			// Copies this surface onto another, with an offset (Not sure if clipped) 
	void BlendCopyTo(Surface* a_Dst, int a_X, int a_Y);		// Same as Copy to but blends them with a ADD-blend
	void ScaleColor(unsigned int a_Scale);					// Todo:: 

	// Text methods
	void InitCharset();	
	void SetChar( int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5 );
	void Print(std::string_view text, int x, int y, Pixel color, int width = 1);
	Tmpl8::vec2 GetTextCenterPosition(std::string_view str, int y, int width);
	void PrintCenter(std::string_view str, int y, Pixel color, int width=1);

	// Drawing methods
	void Clear( Pixel a_Color );								// Fills the surface with a_Color
	void Plot(int x, int y, Pixel c);							// Draws one pixel on x.y (Clipped)
	void Line(float x1, float y1, float x2, float y2, Pixel c); // Draws line between x1,y1 to x2,y2 (Clipped)
	
	void Box( int x1, int y1, int x2, int y2, Pixel color );	// Draws a rectangle with a width of 1 pixel
	void Bar( int x1, int y1, int x2, int y2, Pixel color );	// Draws a rectangle that is filled

	void Rectangle(int x1, int y1, int x2, int y2, Pixel color, int width = 0); 
	void Rectangle(const Tmpl8::vec2& topLeft, const Tmpl8::vec2& bottomRight, Pixel color, int width = 0);
	// if width == 0, (default) fill the rectangle, else used for line thickness

	void Circle(int x, int y, int radius, Pixel color, int width = 0); 
	// if width == 0, (default) fill the circle, else used for line thickness

	void Resize( Surface* a_Orig );
private:
	// Attributes
	Pixel* m_Buffer{nullptr};	
	int m_Width{0}, m_Height{0};
	int m_Pitch{0};
	int m_Flags{0};
	// Static attributes for the buildin font
	static char s_Font[51][5][6];
	static bool fontInitialized;

	// translates ascii into position on the s_Font
	int s_Transl[256]{};
};

class Sprite
{
public:
	// Sprite flags
	enum
	{
		FLARE		= (1<< 0),
		OPFLARE		= (1<< 1),	
		FLASH		= (1<< 4),	
		DISABLED	= (1<< 6),	
		GMUL		= (1<< 7),
		BLACKFLARE	= (1<< 8),	
		BRIGHTEST   = (1<< 9),
		RFLARE		= (1<<12),
		GFLARE		= (1<<13),
		NOCLIP		= (1<<14)
	};
	
	// Structors
	Sprite( Surface* a_Surface, unsigned int a_NumFrames );
	~Sprite();
	// Methods
	void Draw( Surface* a_Target, int a_X, int a_Y );
	void DrawScaled( int a_X, int a_Y, int a_Width, int a_Height, Surface* a_Target );
	void SetFlags( unsigned int a_Flags ) { m_Flags = a_Flags; }
	void SetFrame( unsigned int a_Index ) { m_CurrentFrame = a_Index; }
	unsigned int GetFlags() const { return m_Flags; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	Pixel* GetBuffer() { return m_Surface->GetBuffer(); }	
	unsigned int Frames() { return m_NumFrames; }
	Surface* GetSurface() { return m_Surface; }
private:
	// Methods
	void InitializeStartData();
	// Attributes
	int m_Width, m_Height, m_Pitch;
	unsigned int m_NumFrames;          
	unsigned int m_CurrentFrame;       
	unsigned int m_Flags;
	unsigned int** m_Start;
	Surface* m_Surface;
};

class Font
{
public:
	Font();
	Font( char* a_File, char* a_Chars );
	~Font();
	void Print( Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip = false );
	void Centre( Surface* a_Target, char* a_Text, int a_Y );
	int Width( char* a_Text );
	int Height() { return m_Surface->GetHeight(); }
	void YClip( int y1, int y2 ) { m_CY1 = y1; m_CY2 = y2; }
private:
	Surface* m_Surface;
	int* m_Offset, *m_Width, *m_Trans, m_Height, m_CY1, m_CY2;
};

}; // namespace Tmpl8