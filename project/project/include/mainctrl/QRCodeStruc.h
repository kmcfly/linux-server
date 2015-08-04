//global variable
const unsigned int gQRSpecVerMax=40; //the max QR version number
const int    gMaskNum = 8;
const int    gModeIndicatorSize=4;
const int    gQrSpecWidthMax= 177;
//Mask value
const int MaskN1= 3;
const int MaskN2= 3;
const int MaskN3= 40;
const int MaskN4= 10;

const int QR_INT_MAX= 2147483647;    /* maximum (signed) int value */

//Spec
const int QrspecModeIDEci = 7;
const int QrspecModeIDNum = 1;
const int QrspecModeIDAn = 2;
const int QrspecModeID8 = 4;
const int QrspecModeIDKanJi = 8;
const int QrspecModeIDFnc1First = 5;
const int QrspecModeIDFnc1Second = 9;
const int QrspecModeIDStructure	 = 3;
const int QrspecModeIDTerminator = 0;

//#define MODE_INDICATOR_SIZE 4

//function point
typedef int MaskMaker(int, const unsigned char *, unsigned char *);

//enum variable
typedef enum {
	QR_MODE_NUL = -1,  ///< Terminator (NUL character). Internal use only
	QR_MODE_NUM = 0,   ///< Numeric mode
	QR_MODE_AN,        ///< Alphabet-numeric mode
	QR_MODE_8,         ///< 8-bit data mode
	QR_MODE_KANJI,     ///< Kanji (shift-jis) mode
	QR_MODE_STRUCTURE, ///< Internal use only
	QR_MODE_ECI,       ///< ECI mode
	QR_MODE_FNC1FIRST,  ///< FNC1, first position
	QR_MODE_FNC1SECOND, ///< FNC1, second position
}EQREncodeMode;

typedef enum
{
	QR_ECLEVEL_L=0,
	QR_ECLEVEL_M,
	QR_ECLEVEL_Q,
	QR_ECLEVEL_H
}EQRErrorLevel;

typedef enum
{
	QR_TYPE_DATA_ERROR=0,
	QR_TYPE_FINDER,
	QR_TYPE_SEPARATOR,
	QR_TYPE_TIMING_PATTERNS,
	QR_TYPE_ALIGNMENT_PATTERNS,
	QR_TYPE_FORMAT_INFO,
	QR_TYPE_VERSION_INFO
}EQrDataType;


//struct define
typedef struct {
	int width; //< Edge length of the symbol
	int words;  //< Data capacity (bytes)
	int remainder; //< Remainder bit (bits)
	int ec[4];  //< Number of ECC code (bytes)
}QRCodeCapacity;


//bitstream block
typedef struct {
	int length;
	unsigned char *data;
}BitStream;

/**
* Reed-Solomon codec control block
*/
struct QR_RS {
	int mm;              /* Bits per symbol */
	int nn;              /* Symbols per block (= (1<<mm)-1) */
	unsigned char *alpha_to;     /* log lookup table */
	unsigned char *index_of;     /* Antilog lookup table */
	unsigned char *genpoly;      /* Generator polynomial */
	int nroots;     /* Number of generator roots = number of parity symbols */
	int fcr;        /* First consecutive root, index form */
	int prim;       /* Primitive element, index form */
	int iprim;      /* prim-th root of 1, index form */
	int pad;        /* Padding bytes in shortened block */
	int gfpoly;
	struct QR_RS *next;
};

typedef struct QRinputListStruc {
	EQREncodeMode mode;
	int size;				///< Size of data chunk (byte).
	unsigned char *data;	///< Data chunk.
	BitStream *bstream;
	struct QRinputListStruc *next;
}QRinput_List;

/******************************************************************************
* Input Data
*****************************************************************************/
typedef struct _QRinput {
	int version;
	EQRErrorLevel level;
	QRinput_List *head;
	QRinput_List *tail;
	int mqr;
	int fnc1;
	unsigned char appid;
}QRInputData;

typedef struct {
	int dataLength;
	unsigned char *data;
	int eccLength;
	unsigned char *ecc;
}RSblock;

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	int b1;
	int blocks;
	RSblock *rsblock;
	int count;
}QRRawCode;




/******************************************************************************
* Frame filling
*****************************************************************************/

typedef struct {
	int width;
	unsigned char *frame;
	int x, y;
	int dir;
	int bit;
	int mqr;
} FrameFiller;


//table define
/**
* Table of the capacity of symbols
* See Table 1 (pp.13) and Table 12-16 (pp.30-36), JIS X0510:2004.
*/
const QRCodeCapacity QrcodeCapacity[gQRSpecVerMax + 1] = {
	{  0,    0, 0, {   0,    0,    0,    0}},
	{ 21,   26, 0, {   7,   10,   13,   17}}, // 1
	{ 25,   44, 7, {  10,   16,   22,   28}},
	{ 29,   70, 7, {  15,   26,   36,   44}},
	{ 33,  100, 7, {  20,   36,   52,   64}},
	{ 37,  134, 7, {  26,   48,   72,   88}}, // 5
	{ 41,  172, 7, {  36,   64,   96,  112}},
	{ 45,  196, 0, {  40,   72,  108,  130}},
	{ 49,  242, 0, {  48,   88,  132,  156}},
	{ 53,  292, 0, {  60,  110,  160,  192}},
	{ 57,  346, 0, {  72,  130,  192,  224}}, //10
	{ 61,  404, 0, {  80,  150,  224,  264}},
	{ 65,  466, 0, {  96,  176,  260,  308}},
	{ 69,  532, 0, { 104,  198,  288,  352}},
	{ 73,  581, 3, { 120,  216,  320,  384}},
	{ 77,  655, 3, { 132,  240,  360,  432}}, //15
	{ 81,  733, 3, { 144,  280,  408,  480}},
	{ 85,  815, 3, { 168,  308,  448,  532}},
	{ 89,  901, 3, { 180,  338,  504,  588}},
	{ 93,  991, 3, { 196,  364,  546,  650}},
	{ 97, 1085, 3, { 224,  416,  600,  700}}, //20
	{101, 1156, 4, { 224,  442,  644,  750}},
	{105, 1258, 4, { 252,  476,  690,  816}},
	{109, 1364, 4, { 270,  504,  750,  900}},
	{113, 1474, 4, { 300,  560,  810,  960}},
	{117, 1588, 4, { 312,  588,  870, 1050}}, //25
	{121, 1706, 4, { 336,  644,  952, 1110}},
	{125, 1828, 4, { 360,  700, 1020, 1200}},
	{129, 1921, 3, { 390,  728, 1050, 1260}},
	{133, 2051, 3, { 420,  784, 1140, 1350}},
	{137, 2185, 3, { 450,  812, 1200, 1440}}, //30
	{141, 2323, 3, { 480,  868, 1290, 1530}},
	{145, 2465, 3, { 510,  924, 1350, 1620}},
	{149, 2611, 3, { 540,  980, 1440, 1710}},
	{153, 2761, 3, { 570, 1036, 1530, 1800}},
	{157, 2876, 0, { 570, 1064, 1590, 1890}}, //35
	{161, 3034, 0, { 600, 1120, 1680, 1980}},
	{165, 3196, 0, { 630, 1204, 1770, 2100}},
	{169, 3362, 0, { 660, 1260, 1860, 2220}},
	{173, 3532, 0, { 720, 1316, 1950, 2310}},
	{177, 3706, 0, { 750, 1372, 2040, 2430}} //40
};

/******************************************************************************
* Length indicator
*****************************************************************************/
/*4 encode data mode:number, number and alphabetic,8 bits,Japan*/
static const int QrCodeLengthTableBits[4][3] = {
	/*ver 1-9 */ /*ver 10-26*/ /*ver 27-40*/
	{10, 12, 14}, 
	{ 9, 11, 13},
	{ 8, 16, 16},
	{ 8, 10, 12}
};

/******************************************************************************
* Error correction code
*****************************************************************************/

/**
* Table of the error correction code (Reed-Solomon block)
* See Table 12-16 (pp.30-36), JIS X0510:2004.
*/

/*Ecc different blocks in different version and ecc level*/
/*the total version is 40,and max ecc block num is 2,in one ecc level */
static const int QrEccTable[gQRSpecVerMax+1][4][2] = {
	{{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}},
	{{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}}, // 1
	{{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}},
	{{ 1,  0}, { 1,  0}, { 2,  0}, { 2,  0}},
	{{ 1,  0}, { 2,  0}, { 2,  0}, { 4,  0}},
	{{ 1,  0}, { 2,  0}, { 2,  2}, { 2,  2}}, // 5
	{{ 2,  0}, { 4,  0}, { 4,  0}, { 4,  0}},
	{{ 2,  0}, { 4,  0}, { 2,  4}, { 4,  1}},
	{{ 2,  0}, { 2,  2}, { 4,  2}, { 4,  2}},
	{{ 2,  0}, { 3,  2}, { 4,  4}, { 4,  4}},
	{{ 2,  2}, { 4,  1}, { 6,  2}, { 6,  2}}, //10
	{{ 4,  0}, { 1,  4}, { 4,  4}, { 3,  8}},
	{{ 2,  2}, { 6,  2}, { 4,  6}, { 7,  4}},
	{{ 4,  0}, { 8,  1}, { 8,  4}, {12,  4}},
	{{ 3,  1}, { 4,  5}, {11,  5}, {11,  5}},
	{{ 5,  1}, { 5,  5}, { 5,  7}, {11,  7}}, //15
	{{ 5,  1}, { 7,  3}, {15,  2}, { 3, 13}},
	{{ 1,  5}, {10,  1}, { 1, 15}, { 2, 17}},
	{{ 5,  1}, { 9,  4}, {17,  1}, { 2, 19}},
	{{ 3,  4}, { 3, 11}, {17,  4}, { 9, 16}},
	{{ 3,  5}, { 3, 13}, {15,  5}, {15, 10}}, //20
	{{ 4,  4}, {17,  0}, {17,  6}, {19,  6}},
	{{ 2,  7}, {17,  0}, { 7, 16}, {34,  0}},
	{{ 4,  5}, { 4, 14}, {11, 14}, {16, 14}},
	{{ 6,  4}, { 6, 14}, {11, 16}, {30,  2}},
	{{ 8,  4}, { 8, 13}, { 7, 22}, {22, 13}}, //25
	{{10,  2}, {19,  4}, {28,  6}, {33,  4}},
	{{ 8,  4}, {22,  3}, { 8, 26}, {12, 28}},
	{{ 3, 10}, { 3, 23}, { 4, 31}, {11, 31}},
	{{ 7,  7}, {21,  7}, { 1, 37}, {19, 26}},
	{{ 5, 10}, {19, 10}, {15, 25}, {23, 25}}, //30
	{{13,  3}, { 2, 29}, {42,  1}, {23, 28}},
	{{17,  0}, {10, 23}, {10, 35}, {19, 35}},
	{{17,  1}, {14, 21}, {29, 19}, {11, 46}},
	{{13,  6}, {14, 23}, {44,  7}, {59,  1}},
	{{12,  7}, {12, 26}, {39, 14}, {22, 41}}, //35
	{{ 6, 14}, { 6, 34}, {46, 10}, { 2, 64}},
	{{17,  4}, {29, 14}, {49, 10}, {24, 46}},
	{{ 4, 18}, {13, 32}, {48, 14}, {42, 32}},
	{{20,  4}, {40,  7}, {43, 22}, {10, 67}},
	{{19,  6}, {18, 31}, {34, 34}, {20, 61}},//40
};

//Alignment pattern
static const int QrAlignmentPattern[gQRSpecVerMax+1][2] = {
	{ 0,  0},
	{ 0,  0}, {18,  0}, {22,  0}, {26,  0}, {30,  0}, // 1- 5
	{34,  0}, {22, 38}, {24, 42}, {26, 46}, {28, 50}, // 6-10
	{30, 54}, {32, 58}, {34, 62}, {26, 46}, {26, 48}, //11-15
	{26, 50}, {30, 54}, {30, 56}, {30, 58}, {34, 62}, //16-20
	{28, 50}, {26, 50}, {30, 54}, {28, 54}, {32, 58}, //21-25
	{30, 58}, {34, 62}, {26, 50}, {30, 54}, {26, 52}, //26-30
	{30, 56}, {34, 60}, {30, 58}, {34, 62}, {30, 54}, //31-35
	{24, 50}, {28, 54}, {32, 58}, {26, 54}, {30, 58}, //35-40
};

//version pattern
/*if version  < 6,it is not need version pattern*/
static const unsigned int QrVersionPattern[gQRSpecVerMax- 6] = {
	0x07c94, 0x085bc, 0x09a99, 0x0a4d3, 0x0bbf6, 0x0c762, 0x0d847, 0x0e60d,
	0x0f928, 0x10b78, 0x1145d, 0x12a17, 0x13532, 0x149a6, 0x15683, 0x168c9,
	0x177ec, 0x18ec4, 0x191e1, 0x1afab, 0x1b08e, 0x1cc1a, 0x1d33f, 0x1ed75,
	0x1f250, 0x209d5, 0x216f0, 0x228ba, 0x2379f, 0x24b0b, 0x2542e, 0x26a64,
	0x27541, 0x28c69
};


//format information
//4 ecc level and 8 different mask code mode
static const unsigned int QrFormatInfo[4][8] = {
	{0x77c4, 0x72f3, 0x7daa, 0x789d, 0x662f, 0x6318, 0x6c41, 0x6976},
	{0x5412, 0x5125, 0x5e7c, 0x5b4b, 0x45f9, 0x40ce, 0x4f97, 0x4aa0},
	{0x355f, 0x3068, 0x3f31, 0x3a06, 0x24b4, 0x2183, 0x2eda, 0x2bed},
	{0x1689, 0x13be, 0x1ce7, 0x19d0, 0x0762, 0x0255, 0x0d0c, 0x083b}
};

/******************************************************************************
* Alphabet-numeric data
*****************************************************************************/
const signed char QrInputAnTable[128] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	36, -1, -1, -1, 37, 38, -1, -1, -1, -1, 39, 40, -1, 41, 42, 43,
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 44, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};



