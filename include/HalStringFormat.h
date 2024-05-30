#ifndef HALSTRINGFORMAT_H
#define HALSTRINGFORMAT_H

struct HalStringFormatChar {
    unsigned long alphabet:18;
    unsigned int symbol:12;
    unsigned int parameters:2; //First bit = is capital letter, Second bit = 0-consonant:1-vowel; Первый бит = заглавная ли буква, Второй бит = 0-гласная:1-согласная
} __attribute__((packed,aligned(4)));

typedef struct HalStringFormatChar HalStringFormatChar;
//HalUTF8

typedef struct HalUTF8Char {
    HalStringFormatChar hsfc;
    unsigned int utf8c;
} HalUTF8Char;

#ifndef UTF8HalStringFormatCharacters
static const HalUTF8Char UTF8HalStringFormatCharacters[]={
/*ASCII*/
    {{0,0,0b00},0x00},{{0,1,0b00},0x01},{{0,2,0b00},0x02},{{0,3,0b00},0x03},{{0,4,0b00},0x04},
    {{0,5,0b00},0x05},{{0,6,0b00},0x06},{{0,7,0b00},0x07},{{0,8,0b00},0x08},{{0,9,0b00},0x09},
    {{0,10,0b00},0x0a},{{0,11,0b00},0x0b},{{0,12,0b00},0x0c},{{0,13,0b00},0x0d},{{0,14,0b00},0x0e},
    {{0,15,0b00},0x0f},{{0,16,0b00},0x10},{{0,17,0b00},0x11},{{0,18,0b00},0x12},{{0,19,0b00},0x13},
    {{0,20,0b00},0x14},{{0,21,0b00},0x15},{{0,22,0b00},0x16},{{0,23,0b00},0x17},{{0,24,0b00},0x18},
    {{0,25,0b00},0x19},{{0,26,0b00},0x1a},{{0,27,0b00},0x1b},{{0,28,0b00},0x1c},{{0,29,0b00},0x1d},
    {{0,30,0b00},0x1e},{{0,31,0b00},0x1f},{{0,32,0b00},0x20},{{0,33,0b00},0x21},{{0,34,0b00},0x22},
    {{0,35,0b00},0x23},{{0,36,0b00},0x24},{{0,37,0b00},0x25},{{0,38,0b00},0x26},{{0,39,0b00},0x27},
    {{0,40,0b00},0x28},{{0,41,0b00},0x29},{{0,42,0b00},0x2a},{{0,43,0b00},0x2b},{{0,44,0b00},0x2c},
    {{0,45,0b00},0x2d},{{0,46,0b00},0x2e},{{0,47,0b00},0x2f},{{0,48,0b00},0x30},{{0,49,0b00},0x31},
    {{0,50,0b00},0x32},{{0,51,0b00},0x33},{{0,52,0b00},0x34},{{0,53,0b00},0x35},{{0,54,0b00},0x36},
    {{0,55,0b00},0x37},{{0,56,0b00},0x38},{{0,57,0b00},0x39},{{0,58,0b00},0x3a},{{0,59,0b00},0x3b},
    {{0,60,0b00},0x3c},{{0,61,0b00},0x3d},{{0,62,0b00},0x3e},{{0,63,0b00},0x3f},{{0,64,0b00},0x40},
    {{0,65,0b11},0x41},{{0,66,0b01},0x42},{{0,67,0b01},0x43},{{0,68,0b01},0x44},{{0,69,0b11},0x45},
    {{0,70,0b01},0x46},{{0,71,0b01},0x47},{{0,72,0b01},0x48},{{0,73,0b11},0x49},{{0,74,0b01},0x4a},
    {{0,75,0b01},0x4b},{{0,76,0b01},0x4c},{{0,77,0b01},0x4d},{{0,78,0b01},0x4e},{{0,79,0b11},0x4f},
    {{0,80,0b01},0x50},{{0,81,0b01},0x51},{{0,82,0b01},0x52},{{0,83,0b01},0x53},{{0,84,0b01},0x54},
    {{0,85,0b11},0x55},{{0,86,0b01},0x56},{{0,87,0b01},0x57},{{0,88,0b01},0x58},{{0,89,0b11},0x59},
    {{0,90,0b01},0x5a},{{0,91,0b00},0x5b},{{0,92,0b00},0x5c},{{0,93,0b00},0x5d},{{0,94,0b00},0x5e},
    {{0,95,0b00},0x5f},{{0,96,0b00},0x60},{{0,65,0b10},0x61},{{0,66,0b00},0x62},{{0,67,0b00},0x63},
    {{0,68,0b00},0x64},{{0,69,0b10},0x65},{{0,70,0b00},0x66},{{0,71,0b00},0x67},{{0,72,0b00},0x68},
    {{0,73,0b10},0x69},{{0,74,0b00},0x6a},{{0,75,0b00},0x6b},{{0,76,0b00},0x6c},{{0,77,0b00},0x6d},
    {{0,78,0b00},0x6e},{{0,79,0b10},0x6f},{{0,80,0b00},0x70},{{0,81,0b00},0x71},{{0,82,0b00},0x72},
    {{0,83,0b00},0x73},{{0,84,0b00},0x74},{{0,85,0b10},0x75},{{0,86,0b00},0x76},{{0,87,0b00},0x77},
    {{0,88,0b00},0x78},{{0,89,0b10},0x79},{{0,90,0b00},0x7a},{{0,97,0b00},0x7b},{{0,98,0b00},0x7c},
    {{0,99,0b00},0x7d},{{0,100,0b00},0x7e},{{0,101,0b00},0x7f},
/*Russian*/
    {{1,0,0b11},0xd090},{{1,1,0b01},0xd091},{{1,2,0b01},0xd092},{{1,3,0b01},0xd093},{{1,4,0b01},0xd094},
    {{1,5,0b11},0xd095},{{1,6,0b11},0xd081},{{1,7,0b01},0xd096},{{1,8,0b01},0xd097},{{1,9,0b11},0xd098},
    {{1,10,0b01},0xd099},{{1,11,0b01},0xd09a},{{1,12,0b01},0xd09b},{{1,13,0b01},0xd09c},{{1,14,0b01},0xd09d},
    {{1,15,0b11},0xd09e},{{1,16,0b01},0xd09f},{{1,17,0b01},0xd0a0},{{1,18,0b01},0xd0a1},{{1,19,0b01},0xd0a2},
    {{1,20,0b11},0xd0a3},{{1,21,0b01},0xd0a4},{{1,22,0b01},0xd0a5},{{1,23,0b01},0xd0a6},{{1,24,0b01},0xd0a7},
    {{1,25,0b01},0xd0a8},{{1,26,0b01},0xd0a9},{{1,27,0b01},0xd0aa},{{1,28,0b11},0xd0ab},{{1,29,0b01},0xd0ac},
    {{1,30,0b11},0xd0ad},{{1,31,0b11},0xd0ae},{{1,32,0b11},0xd0af},{{1,0,0b10},0xd0b0},{{1,1,0b00},0xd0b1},
    {{1,2,0b00},0xd0b2},{{1,3,0b00},0xd0b3},{{1,4,0b00},0xd0b4},{{1,5,0b10},0xd0b5},{{1,6,0b10},0xd191},
    {{1,7,0b00},0xd0b6},{{1,8,0b00},0xd0b7},{{1,9,0b10},0xd0b8},{{1,10,0b00},0xd0b9},{{1,11,0b00},0xd0ba},
    {{1,12,0b00},0xd0bb},{{1,13,0b00},0xd0bc},{{1,14,0b00},0xd0bd},{{1,15,0b10},0xd0be},{{1,16,0b00},0xd0bf},
    {{1,17,0b00},0xd180},{{1,18,0b00},0xd181},{{1,19,0b00},0xd182},{{1,20,0b10},0xd183},{{1,21,0b00},0xd184},
    {{1,22,0b00},0xd185},{{1,23,0b00},0xd186},{{1,24,0b00},0xd187},{{1,25,0b00},0xd188},{{1,26,0b00},0xd189},
    {{1,27,0b00},0xd18a},{{1,28,0b10},0xd18b},{{1,29,0b00},0xd18c},{{1,30,0b10},0xd18d},{{1,31,0b10},0xd18e},
    {{1,32,0b10},0xd18f}
};
#define UTF8HalStringFormatSize sizeof(UTF8HalStringFormatCharacters)/sizeof(HalUTF8Char)
#endif

//HalUTF8

//HalUTF16

typedef struct HalUTF16Char {
    HalStringFormatChar hsfc;
    unsigned int utf16c;
} HalUTF16Char;

#ifndef UTF16HalStringFormatCharacters
static const HalUTF16Char UTF16HalStringFormatCharacters[]={
/*ASCII*/
    {{0,0,0b00},0x0000},{{0,1,0b00},0x0001},{{0,2,0b00},0x0002},{{0,3,0b00},0x0003},{{0,4,0b00},0x0004},
    {{0,5,0b00},0x0005},{{0,6,0b00},0x0006},{{0,7,0b00},0x0007},{{0,8,0b00},0x0008},{{0,9,0b00},0x0009},
    {{0,10,0b00},0x000a},{{0,11,0b00},0x000b},{{0,12,0b00},0x000c},{{0,13,0b00},0x000d},{{0,14,0b00},0x000e},
    {{0,15,0b00},0x000f},{{0,16,0b00},0x0010},{{0,17,0b00},0x0011},{{0,18,0b00},0x0012},{{0,19,0b00},0x0013},
    {{0,20,0b00},0x0014},{{0,21,0b00},0x0015},{{0,22,0b00},0x0016},{{0,23,0b00},0x0017},{{0,24,0b00},0x0018},
    {{0,25,0b00},0x0019},{{0,26,0b00},0x001a},{{0,27,0b00},0x001b},{{0,28,0b00},0x001c},{{0,29,0b00},0x001d},
    {{0,30,0b00},0x001e},{{0,31,0b00},0x001f},{{0,32,0b00},0x0020},{{0,33,0b00},0x0021},{{0,34,0b00},0x0022},
    {{0,35,0b00},0x0023},{{0,36,0b00},0x0024},{{0,37,0b00},0x0025},{{0,38,0b00},0x0026},{{0,39,0b00},0x0027},
    {{0,40,0b00},0x0028},{{0,41,0b00},0x0029},{{0,42,0b00},0x002a},{{0,43,0b00},0x002b},{{0,44,0b00},0x002c},
    {{0,45,0b00},0x002d},{{0,46,0b00},0x002e},{{0,47,0b00},0x002f},{{0,48,0b00},0x0030},{{0,49,0b00},0x0031},
    {{0,50,0b00},0x0032},{{0,51,0b00},0x0033},{{0,52,0b00},0x0034},{{0,53,0b00},0x0035},{{0,54,0b00},0x0036},
    {{0,55,0b00},0x0037},{{0,56,0b00},0x0038},{{0,57,0b00},0x0039},{{0,58,0b00},0x003a},{{0,59,0b00},0x003b},
    {{0,60,0b00},0x003c},{{0,61,0b00},0x003d},{{0,62,0b00},0x003e},{{0,63,0b00},0x003f},{{0,64,0b00},0x0040},
    {{0,65,0b11},0x0041},{{0,66,0b01},0x0042},{{0,67,0b01},0x0043},{{0,68,0b01},0x0044},{{0,69,0b11},0x0045},
    {{0,70,0b01},0x0046},{{0,71,0b01},0x0047},{{0,72,0b01},0x0048},{{0,73,0b11},0x0049},{{0,74,0b01},0x004a},
    {{0,75,0b01},0x004b},{{0,76,0b01},0x004c},{{0,77,0b01},0x004d},{{0,78,0b01},0x004e},{{0,79,0b11},0x004f},
    {{0,80,0b01},0x0050},{{0,81,0b01},0x0051},{{0,82,0b01},0x0052},{{0,83,0b01},0x0053},{{0,84,0b01},0x0054},
    {{0,85,0b11},0x0055},{{0,86,0b01},0x0056},{{0,87,0b01},0x0057},{{0,88,0b01},0x0058},{{0,89,0b11},0x0059},
    {{0,90,0b01},0x005a},{{0,91,0b00},0x005b},{{0,92,0b00},0x005c},{{0,93,0b00},0x005d},{{0,94,0b00},0x005e},
    {{0,95,0b00},0x005f},{{0,96,0b00},0x0060},{{0,65,0b10},0x0061},{{0,66,0b00},0x0062},{{0,67,0b00},0x0063},
    {{0,68,0b00},0x0064},{{0,69,0b10},0x0065},{{0,70,0b00},0x0066},{{0,71,0b00},0x0067},{{0,72,0b00},0x0068},
    {{0,73,0b10},0x0069},{{0,74,0b00},0x006a},{{0,75,0b00},0x006b},{{0,76,0b00},0x006c},{{0,77,0b00},0x006d},
    {{0,78,0b00},0x006e},{{0,79,0b10},0x006f},{{0,80,0b00},0x0070},{{0,81,0b00},0x0071},{{0,82,0b00},0x0072},
    {{0,83,0b00},0x0073},{{0,84,0b00},0x0074},{{0,85,0b10},0x0075},{{0,86,0b00},0x0076},{{0,87,0b00},0x0077},
    {{0,88,0b00},0x0078},{{0,89,0b10},0x0079},{{0,90,0b00},0x007a},{{0,97,0b00},0x007b},{{0,98,0b00},0x007c},
    {{0,99,0b00},0x007d},{{0,100,0b00},0x007e},{{0,101,0b00},0x007f},
/*Russian*/
    {{1,0,0b11},0x0410},{{1,1,0b01},0x0411},{{1,2,0b01},0x0412},{{1,3,0b01},0x0413},{{1,4,0b01},0xd0414},
    {{1,5,0b11},0x0415},{{1,6,0b11},0xd0401},{{1,7,0b01},0xd0416},{{1,8,0b01},0x0417},{{1,9,0b11},0x0418},
    {{1,10,0b01},0x0419},{{1,11,0b01},0x041a},{{1,12,0b01},0x041b},{{1,13,0b01},0x041c},{{1,14,0b01},0x041d},
    {{1,15,0b11},0x041e},{{1,16,0b01},0x041f},{{1,17,0b01},0x0420},{{1,18,0b01},0x0421},{{1,19,0b01},0x0422},
    {{1,20,0b11},0x0423},{{1,21,0b01},0x0424},{{1,22,0b01},0x0425},{{1,23,0b01},0x0426},{{1,24,0b01},0x0427},
    {{1,25,0b01},0x0428},{{1,26,0b01},0x0429},{{1,27,0b01},0x042a},{{1,28,0b11},0x042b},{{1,29,0b01},0x042c},
    {{1,30,0b11},0x042d},{{1,31,0b11},0x042e},{{1,32,0b11},0x042f},{{1,0,0b10},0x0430},{{1,1,0b00},0x0431},
    {{1,2,0b00},0x0432},{{1,3,0b00},0x0433},{{1,4,0b00},0x0434},{{1,5,0b10},0x0435},{{1,6,0b10},0x0451},
    {{1,7,0b00},0x0436},{{1,8,0b00},0x0437},{{1,9,0b10},0x0438},{{1,10,0b00},0x0439},{{1,11,0b00},0x043a},
    {{1,12,0b00},0x043b},{{1,13,0b00},0x043c},{{1,14,0b00},0x043d},{{1,15,0b10},0x043e},{{1,16,0b00},0x043f},
    {{1,17,0b00},0x0440},{{1,18,0b00},0x0441},{{1,19,0b00},0x0442},{{1,20,0b10},0x0443},{{1,21,0b00},0x0444},
    {{1,22,0b00},0x0445},{{1,23,0b00},0x0446},{{1,24,0b00},0x0447},{{1,25,0b00},0x0448},{{1,26,0b00},0x0449},
    {{1,27,0b00},0x044a},{{1,28,0b10},0x044b},{{1,29,0b00},0x044c},{{1,30,0b10},0x044d},{{1,31,0b10},0x044e},
    {{1,32,0b10},0x044f}
};
#define UTF16HalStringFormatSize sizeof(UTF16HalStringFormatCharacters)/sizeof(HalUTF16Char)
#endif

//HalUTF16

#define utf8_is_single_byte(c)((c[0]&0x80)==0x0)
#define utf8_is_double_byte(c)((c[0]&0xe0)==0xc0&&utf8_is_continuation(c[1]))
#define utf8_is_triple_byte(c)((c[0]&0xf0)==0xe0&&utf8_is_continuation(c[1])&&utf8_is_continuation(c[2]))
#define utf8_is_quadruple_byte(c)((c[0]&0xf8)==0xf0&&utf8_is_continuation(c[1])&&utf8_is_continuation(c[2])&&utf8_is_continuation(c[3]))
#define utf8_is_continuation(c)((c&0xc0)==0x80)

void* hsf_malloc(unsigned long long int x);
void HalStringFormat_init(void*(*hmf)(unsigned long long int));

unsigned long long int HalStringFormat_length(HalStringFormatChar* str);
int HalUTF8_indexByUTF8Char(unsigned int chr);
int HalUTF8_indexByHalStringFormatChar(HalStringFormatChar chr);
unsigned long long int UTF8String_length(char* str,unsigned long long int size);
HalStringFormatChar U8CH(unsigned int chr);
HalStringFormatChar ASCIIH(char chr);
HalStringFormatChar* UTF8String_toHalStringFormat(HalStringFormatChar* out,char* str,unsigned long long int size);
char* HalStringFormat_toUTF8(char* out,HalStringFormatChar* str,unsigned long long int size);
unsigned long long int HalStringFormat_UTF8GetLength(HalStringFormatChar* str,unsigned long long int size);
HalStringFormatChar* U8H(char* str);
char* HU8(HalStringFormatChar* str);
unsigned char HSFCharCompare(HalStringFormatChar a,HalStringFormatChar b);
unsigned char HSFCompare(HalStringFormatChar* a,HalStringFormatChar* b);
char* HSF2ASCII(HalStringFormatChar* str);

//UTF16

#define U_IS_SURROGATE(c) (((c)&0xfffff800)==0xd800)

#define U16_IS_SINGLE(c) !U_IS_SURROGATE(c)
 
#define U16_IS_LEAD(c) (((c)&0xfffffc00)==0xd800)
 
#define U16_IS_TRAIL(c) (((c)&0xfffffc00)==0xdc00)
 
#define U16_IS_SURROGATE(c) U_IS_SURROGATE(c)
 
#define U16_IS_SURROGATE_LEAD(c) (((c)&0x400)==0)
 
#define U16_IS_SURROGATE_TRAIL(c) (((c)&0x400)!=0)
 
#define U16_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)
 
#define U16_GET_SUPPLEMENTARY(lead, trail) \
    (((int)(lead)<<10UL)+(int)(trail)-U16_SURROGATE_OFFSET)
 
 
#define U16_LEAD(supplementary) (short)(((supplementary)>>10)+0xd7c0)
 
#define U16_TRAIL(supplementary) (short)(((supplementary)&0x3ff)|0xdc00)
 
#define U16_LENGTH(c) ((unsigned int)(c)<=0xffff ? 1 : 2)

int HalUTF16_indexByUTF16Char(unsigned int chr);
int HalUTF16_indexByHalStringFormatChar(HalStringFormatChar chr);
unsigned long long int UTF16String_length(short* str,unsigned long long int size);
HalStringFormatChar U16CH(unsigned int chr);
HalStringFormatChar* UTF16String_toHalStringFormat(HalStringFormatChar* out,short* str,unsigned long long int size);
short* HalStringFormat_toUTF16(short* out,HalStringFormatChar* str,unsigned long long int size);
unsigned long long int HalStringFormat_UTF16GetLength(HalStringFormatChar* str,unsigned long long int size);
HalStringFormatChar* U16H(short* str);
short* HU16(HalStringFormatChar* str);

//UTF16

#endif