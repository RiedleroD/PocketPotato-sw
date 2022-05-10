//MENUSTRING(test,"first","second"); will generate ↓

//const char _test1[] PROGMEM = "first";
//const char _test2[] PROGMEM = "second";
//
//const char* const tests[] PROGMEM = {_test1,_test2};
//
//const int testcount PROGMEM = sizeof(tests)/sizeof(char*);

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
	#define MENUSTRINGS7(name,s1,s2,s3,s4,s5,s6,s7)\
		MENUSTRING(name ## 1,s1);\
		MENUSTRING(name ## 2,s2);\
		MENUSTRING(name ## 3,s3);\
		MENUSTRING(name ## 4,s4);\
		MENUSTRING(name ## 5,s5);\
		MENUSTRING(name ## 6,s6);\
		MENUSTRING(name ## 7,s7);\
		const char* const name ## s[] PROGMEM = {_ ## name ## 1,_ ## name ## 2,_ ## name ## 3,_ ## name ## 4,_ ## name ## 5,_ ## name ## 6,_ ## name ## 7};\
		MENUCOUNT(name)
#endif

#ifndef FUNCS_REPLACE
	#define FUNCS_REPLACE
	#define WHITE SSD1306_WHITE
	#define BLACK SSD1306_BLACK
	#define INVERT SSD1306_INVERSE
	#define TRANSPARENT 42 //very hacky, but works.
	#define drawHLine(x,y,l,c) fillRect(x,y,l,1,c)
	#define drawVLine(x,y,l,c) fillRect(x,y,1,l,c)
	#define drawTexture(x,y,texture,width,height) drawTextureWithPalette(x,y,texture,width,height,WHITE,BLACK)
#endif

//actual function headers

extern void drawTextureWithPalette(const uint8_t _x,const uint8_t _y,const uint8_t texture[],const uint8_t width,const uint8_t height,const uint16_t color1,const uint16_t color2);
extern void drawZoomedPixel(const uint8_t x,const uint8_t y,uint8_t zoom);
extern void fillRect(const uint8_t x,const uint8_t y,const uint8_t width,const uint8_t height,const uint8_t color);
extern void drawRect(const uint8_t x,const uint8_t y,const uint8_t width,const uint8_t height,const uint8_t color);
extern void smartSleep(const uint32_t want_dt);
