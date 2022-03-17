#ifndef MENUSTRING
	#define MENUSTRING(name,string) const char _ ## name[] PROGMEM = string
#endif

#ifndef MENUCOUNT
	#define MENUCOUNT(name) const int name ## count PROGMEM = sizeof(name ## s)/sizeof(char*)
#endif

#ifndef MENUSTRINGS
	#define MENUSTRINGS
	//TODO: this is a shitty way of doing it
	#define MENUSTRINGS3(name,s1,s2,s3)\
		MENUSTRING(name ## 1,s1);\
		MENUSTRING(name ## 2,s2);\
		MENUSTRING(name ## 3,s3);\
		const char* const name ## s[] PROGMEM = {_ ## name ## 1,_ ## name ## 2,_ ## name ## 3};\
		MENUCOUNT(name)
	#define MENUSTRINGS4(name,s1,s2,s3,s4)\
		MENUSTRING(name ## 1,s1);\
		MENUSTRING(name ## 2,s2);\
		MENUSTRING(name ## 3,s3);\
		MENUSTRING(name ## 4,s4);\
		const char* const name ## s[] PROGMEM = {_ ## name ## 1,_ ## name ## 2,_ ## name ## 3,_ ## name ## 4};\
		MENUCOUNT(name)
	#define MENUSTRINGS5(name,s1,s2,s3,s4,s5)\
		MENUSTRING(name ## 1,s1);\
		MENUSTRING(name ## 2,s2);\
		MENUSTRING(name ## 3,s3);\
		MENUSTRING(name ## 4,s4);\
		MENUSTRING(name ## 5,s5);\
		const char* const name ## s[] PROGMEM = {_ ## name ## 1,_ ## name ## 2,_ ## name ## 3,_ ## name ## 4,_ ## name ## 5};\
		MENUCOUNT(name)
#endif
	
extern void drawTexture(const uint8_t _x,const uint8_t _y,const uint8_t texture[],const uint8_t width,const uint8_t height);
extern void drawZoomedPixel(const uint8_t x,const uint8_t y,uint8_t zoom);
extern void printFromFlash(char* text);