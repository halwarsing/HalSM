#ifndef HALSM_H
#define HALSM_H

#include <HalStringFormat.h>

#define NOTHALSMNULLPOINTER ((void*)0)

typedef struct HalSMNull {unsigned char c;} HalSMNull;

typedef struct HalSMErrorIndificator {
    HalStringFormatChar* name;
    unsigned long long int line;
    void* prev;
} HalSMErrorIndificator;

typedef struct HalSMError {
    HalSMErrorIndificator* ind;
    HalStringFormatChar* error;
} HalSMError;

typedef enum HalSMVariableType {
    HalSMVariableType_int,
    HalSMVariableType_double,
    HalSMVariableType_char,
    HalSMVariableType_void,
    HalSMVariableType_HalSMArray,
    HalSMVariableType_str,
    HalSMVariableType_int_array,
    HalSMVariableType_HalSMFunctionC,
    HalSMVariableType_HalSMClassC,
    HalSMVariableType_HalSMRunClassC,
    HalSMVariableType_HalSMSetArg,
    HalSMVariableType_HalSMError,
    HalSMVariableType_HalSMNull,
    HalSMVariableType_HalSMRunFunc,
    HalSMVariableType_HalSMRunFuncC,
    HalSMVariableType_HalSMLocalFunction,
    HalSMVariableType_HalSMCModule,
    HalSMVariableType_HalSMModule,
    HalSMVariableType_HalSMCompiler,
    HalSMVariableType_HalSMRunClass,
    HalSMVariableType_HalSMDoubleGet,
    HalSMVariableType_HalSMClass,
    HalSMVariableType_HalSMVar,
    HalSMVariableType_HalSMMult,
    HalSMVariableType_HalSMDivide,
    HalSMVariableType_HalSMPlus,
    HalSMVariableType_HalSMMinus,
    HalSMVariableType_HalSMEqual,
    HalSMVariableType_HalSMNotEqual,
    HalSMVariableType_HalSMMore,
    HalSMVariableType_HalSMLess,
    HalSMVariableType_HalSMBool,
    HalSMVariableType_HalSMDict,
    HalSMVariableType_HalSMSetVar,
    HalSMVariableType_HalSMReturn,
    HalSMVariableType_HalSMFunctionCTypeDef,
    HalSMVariableType_HalSMFunctionArray,
    HalSMVariableType_unsigned_int,
    HalSMVariableType_custom,
    HalSMVariableType_HalSMMoreEqual,
    HalSMVariableType_HalSMLessEqual,
    HalSMVariableType_HalSMShift,
    HalSMVariableType_HalSMPow,
    HalSMVariableType_HalSMModulo,
    HalSMVariableType_HalSMAND,
    HalSMVariableType_HalSMOR,
    HalSMVariableType_HalSMNegate,
    HalSMVariableType_HalSMNegativeSign,
    //0.1.5
    HalSMVariableType_HalSMGOTO,
    HalSMVariableType_HalSMGOTOSector,
    HalSMVariableType_HalSMInlineIf, //if(a>b) 1 else 0
    HalSMVariableType_HalSMInlineElse,
    HalSMVariableType_HalSMBreak,
    HalSMVariableType_HalSMXOR,
    HalSMVariableType_HalSMBAND,
    HalSMVariableType_HalSMBOR,
    HalSMVariableType_HalSMBNOT,
} HalSMVariableType;

typedef struct HalSMVariable {
    void* value;
    HalSMVariableType type;
} HalSMVariable;

typedef struct HalSMCompiler HalSMCompiler; 

typedef struct DictElement {
    HalSMVariable* key;
    HalSMVariable* value;
} DictElement;

typedef struct DictElementForEach {
    HalSMVariable* key;
    HalSMVariable* value;
    unsigned long long int index;
} DictElementForEach;

typedef struct Dict {
    unsigned long long int size;
    DictElement** elements;
} Dict;

typedef struct HalSMArray {
    unsigned long long int size;
    HalSMVariable** arr;
} HalSMArray;

//VARIABLE CUSTOM TYPE

typedef struct HalSMVariableTypeCustom {
    HalStringFormatChar* name;
    HalStringFormatChar*(*toString)(HalSMCompiler*,HalSMVariable*);
    HalSMVariable*(*add)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*sub)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*mul)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*div)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*pow)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*modulo)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*increment)(HalSMCompiler*,HalSMVariable*);
    HalSMVariable*(*decrement)(HalSMCompiler*,HalSMVariable*);
    unsigned char(*isEqual)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    unsigned char(*isMore)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    unsigned char(*isLess)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    unsigned char(*getBool)(HalSMCompiler*,HalSMVariable*);
    long long int(*indexVariableByName)(HalSMCompiler*,HalSMVariable*,HalStringFormatChar*);
    long long int(*indexFunctionByName)(HalSMCompiler*,HalSMVariable*,HalStringFormatChar*);
    HalSMVariable*(*getVariableByName)(HalSMCompiler*,HalSMVariable*,HalStringFormatChar*);
    HalSMVariable*(*getFunctionByName)(HalSMCompiler*,HalSMVariable*,HalStringFormatChar*);
    HalSMVariable*(*getItem)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*shiftRight)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*shiftLeft)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*convert)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*bxor)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*band)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*bor)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMVariable*(*bnot)(HalSMCompiler*,HalSMVariable*);
    Dict* vars;
    Dict* funcs;
} HalSMVariableTypeCustom;

typedef struct HalSMVariableCustom {
    HalSMVariableTypeCustom* type;
    Dict* vars;
    Dict* funcs;
    void* value;
} HalSMVariableCustom;

//VARIABLE CUSTOM TYPE

typedef enum HalSMFunctionArrayType {
    HalSMFunctionArrayType_function,
    HalSMFunctionArrayType_array,
    HalSMFunctionArrayType_var,
    HalSMFunctionArrayType_dict
} HalSMFunctionArrayType;

typedef struct HalSMFunctionArray {
    HalSMArray* args;
    HalSMFunctionArrayType type;
} HalSMFunctionArray;



typedef struct HalSMFileSystemLibrary {
    unsigned char(*isExistsDir)(HalStringFormatChar*);
    unsigned char(*isExistsFile)(HalStringFormatChar*);
    HalSMArray*(*getListFiles)(HalSMCompiler*,HalStringFormatChar*);
    void*(*openFile)(HalStringFormatChar*,HalStringFormatChar*);
    void(*closeFile)(void*);
    unsigned long long int(*read)(void* buf,unsigned long long int size,unsigned long long int count,void* p);
    unsigned long long int(*write)(void* buf,unsigned long long int size,unsigned long long int count,void* p);
    void(*puts)(void*,HalStringFormatChar*);
    void(*seek)(void*,unsigned long long int,unsigned long long int);
    unsigned long long int(*tell)(void*);
    void(*rewind)(void*);
    void(*flush)(void*);
    int(*eof)(void*);
} HalSMFileSystemLibrary;

typedef struct HalSMStringLibrary {
    HalStringFormatChar*(*Decimal2Str)(long long);
    HalStringFormatChar*(*Decimal2HexStr)(long long);
    HalStringFormatChar*(*Double2Str)(double);
    long long(*ParseDecimal)(HalStringFormatChar*);
    double(*ParseDouble)(HalStringFormatChar*);
} HalSMStringLibrary;

typedef struct HalSMMemoryManagmentLibrary {
    void*(*malloc)(unsigned long long int);
    void*(*calloc)(unsigned long long int,unsigned long long int);
    void*(*realloc)(void*,unsigned long long int);
    void(*free)(void*);
} HalSMMemoryManagmentLibrary;

typedef struct HalSMSystemLibrary {
    void(*exit)(int);
} HalSMSystemLibrary;



typedef struct HalSMInteger {
    unsigned char negative;
    unsigned char* value;
    unsigned long long size;
} HalSMInteger;

typedef struct HalSMDouble {
    unsigned char negative;
    HalSMInteger* a;
    HalSMInteger* b;
} HalSMDouble;

typedef struct HalSMCalculateVars {
    HalStringFormatChar* version;
    HalStringFormatChar*(*addStr)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMInteger*(*addInt)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalSMDouble*(*addDouble)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalStringFormatChar*(*subStr)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMInteger*(*subInt)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalSMDouble*(*subDouble)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalStringFormatChar*(*mulStr)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalSMInteger*(*mulInt)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalSMDouble*(*mulDouble)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalStringFormatChar*(*divStr)(HalSMCompiler*,HalSMVariable*,HalSMVariable*);
    HalSMInteger*(*divInt)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    HalSMDouble*(*divDouble)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
} HalSMCalculateVars;

typedef struct HalSMCompiler {
    HalStringFormatChar* versionName;
    unsigned long long int version;
    HalStringFormatChar* path;
    void* addition_data;
    Dict* functions;
    Dict* sys_modules;
    HalSMCalculateVars calcVars;
    HalSMArray* numbers;
    unsigned long long int line;
    Dict* sys_variables;
    Dict* variables;
    Dict* modules;
    Dict* localFunctions;
    Dict* classes;
    Dict* groupFunctions;
    void(*print)(HalStringFormatChar*);
    void(*printErrorf)(HalStringFormatChar*);
    HalStringFormatChar*(*inputf)(HalStringFormatChar*);
    HalSMVariable*(*getVariable)(HalStringFormatChar*);
    HalSMStringLibrary* stringlibrary;
    HalSMMemoryManagmentLibrary* memorymanagmentlibrary;
    HalSMSystemLibrary* systemlibrary;
    HalSMFileSystemLibrary* filesystemlibrary;
    Dict* sys_custom_variables;
    Dict* GOTOSectors;
    unsigned long long int indexl;
    unsigned char isRun;
    unsigned long long int countLocalFunctions;
    HalSMVariable*(*HalSMLoadModule)(HalSMCompiler*,HalStringFormatChar*);
    HalSMErrorIndificator* ind;
    HalSMErrorIndificator* curInd;
} HalSMCompiler;

typedef HalSMVariable*(*HalSMFunctionCTypeDef)(HalSMCompiler*,HalSMArray*,Dict*);
typedef HalSMVariable*(*HalSMFunctionCTypeDefWithData)(void*,HalSMCompiler*,HalSMArray*,Dict*);

typedef struct HalSMFunctionC {
    void** func;
    HalSMCompiler* hsmc;
    unsigned char type; //0 - standard, 1 - with data
    void* data;
} HalSMFunctionC;

typedef struct HalSMRunClassC {
    HalStringFormatChar* name;
    Dict* vrs;
    Dict* funcs;
} HalSMRunClassC;

typedef struct HalSMClassC {
    Dict* vrs;
    Dict* funcs;
    HalStringFormatChar* name;
} HalSMClassC;

typedef struct HalSMCModule {
    Dict* lfuncs;
    Dict* vrs;
    Dict* classes;
    HalStringFormatChar* name;
} HalSMCModule;

typedef struct HalSMCModule_entry {
    HalStringFormatChar* name;
    HalStringFormatChar* description;
    HalStringFormatChar* version;
    HalStringFormatChar* author;
    Dict* classes;
    Dict* lfuncs;
    Dict* vrs;
} HalSMCModule_entry;

typedef struct HalSMModule {
    HalStringFormatChar* name;
    HalSMCompiler* hsmc;
} HalSMModule;

typedef struct HalSMLocalFunction {
    HalSMCompiler* hsmc;
    HalStringFormatChar* name;
    unsigned long long int id;
    HalSMArray* args;
    HalSMArray* func;
    Dict* vars;
    HalSMVariable* module;
} HalSMLocalFunction;

typedef struct HalSMVar {
    HalSMVariable* name;
} HalSMVar;

typedef struct HalSMPlus {unsigned char c;} HalSMPlus;
typedef struct HalSMMinus {unsigned char c;} HalSMMinus;
typedef struct HalSMMult {unsigned char c;} HalSMMult;
typedef struct HalSMDivide {unsigned char c;} HalSMDivide;
typedef struct HalSMPow {unsigned char c;} HalSMPow;
typedef struct HalSMModulo {unsigned char c;} HalSMModulo;

typedef struct HalSMEqual {unsigned char c;} HalSMEqual;
typedef struct HalSMNotEqual {unsigned char c;} HalSMNotEqual;
typedef struct HalSMMore {unsigned char c;} HalSMMore;
typedef struct HalSMLess {unsigned char c;} HalSMLess;
typedef struct HalSMMoreEqual {unsigned char c;} HalSMMoreEqual;
typedef struct HalSMLessEqual {unsigned char c;} HalSMLessEqual;
typedef struct HalSMAND {unsigned char c;} HalSMAND;
typedef struct HalSMOR {unsigned char c;} HalSMOR;
typedef struct HalSMNegate {unsigned char c;} HalSMNegate;
typedef struct HalSMNegativeSign {unsigned char c;} HalSMNegativeSign;

typedef struct HalSMShift {unsigned char c;} HalSMShift;
typedef struct HalSMInlineIf {unsigned char c;} HalSMInlineIf;
typedef struct HalSMInlineElse {unsigned char c;} HalSMInlineElse;
typedef struct HalSMBreak {unsigned char c;} HalSMBreak;
typedef struct HalSMVoid {unsigned char c;} HalSMVoid;
typedef struct HalSMXOR {unsigned char c;} HalSMXOR;
typedef struct HalSMBAND {unsigned char c;} HalSMBAND;
typedef struct HalSMBOR {unsigned char c;} HalSMBOR;
typedef struct HalSMBNOT {unsigned char c;} HalSMBNOT;

typedef struct HalSMSetArg {
    HalSMVariable* name;
    HalSMVariable* value;
} HalSMSetArg;

typedef struct HalSMRunFuncC {
    HalSMFunctionC* func;
    HalSMArray* args;
    HalSMArray* pointer;
} HalSMRunFuncC;

typedef struct HalSMRunFunc {
    HalSMLocalFunction* func;
    HalSMArray* args;
    HalSMArray* pointer;
} HalSMRunFunc;

typedef struct HalSMRunClass {
    HalStringFormatChar* name;
    Dict* funcs;
    Dict* vars;
} HalSMRunClass;

typedef struct HalSMClass {
    HalStringFormatChar* name;
    Dict* funcs;
    Dict* vars;
} HalSMClass;

typedef struct HalSMDoubleGet {
    HalStringFormatChar* st;
} HalSMDoubleGet;

typedef struct HalSMSetVar {
    HalSMVariable* name;
    HalStringFormatChar* value;
    HalSMVariable* pointer;
} HalSMSetVar;

typedef struct HalSMReturn {
    HalStringFormatChar* value;
    unsigned long long int idLocalFunction;
} HalSMReturn;

HalSMNull* HalSMNull_init(HalSMCompiler* hsmc);
HalSMError* HalSMError_init(HalSMCompiler* hsmc,HalSMErrorIndificator* ind,HalStringFormatChar* error);
HalSMErrorIndificator* HalSMErrorIndificator_init(HalSMCompiler* hsmc,HalStringFormatChar* name,unsigned long long int line,void* prev);

HalSMArray* HalSMArray_init(HalSMMemoryManagmentLibrary* hsmmml);
HalSMArray* HalSMArray_init_with_elements(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable** arr,unsigned long long int size);
HalSMArray* HalSMArray_split_str(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str,HalStringFormatChar* spl);
void HalSMArray_add(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value);
void HalSMArray_set(HalSMArray* harr,HalSMVariable* value,unsigned long long int index);
void HalSMArray_remove(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned long long int index);
void HalSMArray_appendArray(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMArray* t);
void HalSMArray_insert(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value,unsigned long long int index);
HalSMVariable* HalSMArray_get(HalSMArray* harr,unsigned long long int index);
HalSMArray* HalSMArray_reverse(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);
HalStringFormatChar* HalSMArray_join_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalStringFormatChar* join);
HalStringFormatChar* HalSMArray_to_print(HalSMCompiler* hsmc,HalSMArray* harr);
HalStringFormatChar* HalSMArray_chars_to_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);
HalSMArray* HalSMArray_slice(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned long long int s,unsigned long long int e);
unsigned char HalSMArray_compare(HalSMCompiler* hsmc,HalSMArray* harr,HalSMArray* barr);
HalSMArray* HalSMArray_from_str(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str,unsigned long long int size);
HalSMArray* HalSMArray_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);
void HalSMArray_free(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);
long long int HalSMArray_find(HalSMCompiler* hsmc,HalSMArray* harr,HalSMVariable* value);
long long int HalSMArray_index(HalSMCompiler* hsmc,HalSMArray* harr,HalSMVariable* value);
long long int HalSMArray_right_index(HalSMCompiler* hsmc,HalSMArray* harr,HalSMVariable* value);
void HalSMArray_clear(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);

HalSMCalculateVars HalSMCalculateVars_init();
HalStringFormatChar* HalSMCalculateVars_addStr(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalStringFormatChar* HalSMCalculateVars_subStr(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalStringFormatChar* HalSMCalculateVars_mulStr(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalStringFormatChar* HalSMCalculateVars_divStr(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMInteger* HalSMCalculateVars_addInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMInteger* HalSMCalculateVars_subInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMInteger* HalSMCalculateVars_mulInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMInteger* HalSMCalculateVars_divInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMDouble* HalSMCalculateVars_addDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMDouble* HalSMCalculateVars_subDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMDouble* HalSMCalculateVars_mulDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
HalSMDouble* HalSMCalculateVars_divDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);

HalSMVariable* HalSMCompiler_input(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_reversed(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_range(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_print(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMArray* HalSMCompiler_get_print_text(HalSMCompiler* hsmc,HalSMArray* args,unsigned char isToPrint);
HalSMCompiler* HalSMCompiler_init(HalStringFormatChar* path,void(*printf)(HalStringFormatChar*),void(*printErrorf)(HalStringFormatChar*),HalStringFormatChar*(*inputf)(HalStringFormatChar*),HalSMVariable*(*getVariable)(HalStringFormatChar*),HalSMStringLibrary* stringlibrary,HalSMMemoryManagmentLibrary* memorymanagmentlibrary,HalSMSystemLibrary* systemlibrary,HalSMFileSystemLibrary* filesystemlibrary,HalSMVariable*(*HalSMLoadModule)(HalSMCompiler*,HalStringFormatChar*));
HalSMArray* HalSMCompiler_getLines(HalSMCompiler* hsmc,HalStringFormatChar* text);
HalStringFormatChar* HalSMCompiler_removeSpaces(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* text);
void HalSMCompiler_ThrowError(HalSMCompiler* hsmc,HalSMError* err);
HalSMVariable* HalSMCompiler_getNameFunction(HalSMCompiler* hsmc,HalStringFormatChar* l);
HalSMVariable* HalSMCompiler_isSetVar(HalSMCompiler* hsmc,HalStringFormatChar* l,Dict* vars);
unsigned long long int HalSMCompiler_getTabs(HalSMCompiler* hsmc,HalStringFormatChar* l);
unsigned char HalSMCompiler_isNull(HalStringFormatChar* text);
HalSMVariable* HalSMCompiler_compileList(HalSMCompiler* hsmc,unsigned char isConsole,HalSMVariable* module,HalSMArray* allLines);
HalSMVariable* HalSMCompiler_compile(HalSMCompiler* hsmc,HalStringFormatChar* text,unsigned char isConsole,HalSMVariable* module);

unsigned char HalSMCompiler_isGet(HalSMCompiler* hsmc,HalStringFormatChar* l,HalSMVariable* outvar,unsigned char isNewVar,Dict* vars);
HalSMVariable* HalSMCompiler_isGetU(HalSMCompiler* hsmc,HalStringFormatChar* l,unsigned char* isOut,unsigned char isNewVar,Dict* vars);
HalSMVariable* HalSMCompiler_isGetValue(HalSMCompiler* hsmc,HalSMVariable* obuff,unsigned char* isOut,unsigned char isNewVar,Dict* vars,unsigned char* isNotNullOut,HalSMVariable** moduleOut,HalSMVariable* out,HalStringFormatChar* type);
HalSMVariable* HalSMCompiler_getArrayByIndex(HalSMCompiler* hsmc,HalStringFormatChar* obuffs,HalSMVariable* out,unsigned char* isNotNullOut,Dict* vars);
void HalSMCompiler_executeArgs(HalSMCompiler* hsmc,HalSMArray** outArr,Dict* vars);
HalSMArray* HalSMCompiler_getArgs(HalSMCompiler* hsmc,HalStringFormatChar* l,Dict* vars);
void HalSMCompiler_getArgsValue(HalSMCompiler* hsmc,HalSMVariable* buffo,HalStringFormatChar* buffs,HalSMArray* out,Dict* vars);
void HalSMCompiler_getArgsSetVar(HalSMCompiler* hsmc,HalSMArray* value,HalSMVariable* pointer,Dict* vars);

void HalSMCompiler_getArgsFunction(HalSMCompiler* hsmc,HalSMArray* value,HalSMArray* out,Dict* vars);
Dict* HalSMCompiler_getArgsDict(HalSMCompiler* hsmc,HalStringFormatChar* l,Dict* vars);

HalStringFormatChar* HalSMCompiler_getPath(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* a,HalStringFormatChar* b);
HalSMVariable* HalSMCompiler_isExistsPath(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_isExistsDir(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_isExistsFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_getListFiles(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_openFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_closeFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_readFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_putsFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_seekFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_tellFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_rewindFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_flushFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMVariable* HalSMCompiler_additionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_subtractionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_multiplyVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_divideVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_incrementVariable(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HalSMCompiler_decrementVariable(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HalSMCompiler_increment(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_decrement(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMVariable* HalSMCompiler_while(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMLocalFunction* HalSMLocalFunction_init(HalSMCompiler* hsmc,HalStringFormatChar* name,HalStringFormatChar* args,HalSMVariable* module);
HalSMArray* HalSMLocalFunction_getArgs(HalSMCompiler* hsmc,HalStringFormatChar* args);
HalSMVariable* HalSMLocalFunction_run(HalSMLocalFunction* lf,HalSMArray* args,Dict* vars);

unsigned char HalSMCompiler_isMore(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HalSMCompiler_isLess(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);

HalSMVariable* HalSMCompiler_shift(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b,HalSMShift* shift);
HalSMVariable* HalSMCompiler_pow(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HalSMCompiler_powFunc(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_bin(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_hex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_modulo(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);

unsigned char HalSMCompiler_getBool(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HalSMCompiler_negate(HalSMCompiler* hsmc,HalSMVariable* v);

HalSMVariable* HalSMCompiler_replace(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_index(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_rindex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_split(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_substring(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_str_upper(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMVariable* HalSMCompiler_add(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_append(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_count(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_clear(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_insert(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_pop(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_remove(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_reverse(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_copy(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_appendArray(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_slice(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMVariable* HalSMCompiler_len(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMVariable* HalSMCompiler_appendDict(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_indexByKey(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_indexByValue(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_keyByIndex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_valueByIndex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_set(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

HalSMVariable* HalSMCompiler_sqrt(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_int(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_double(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_str(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_floor(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_ceil(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_round(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_abs(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_join(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_array(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_dir(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HalSMCompiler_dict(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

unsigned char HalSMIsInt(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c);
unsigned char HalSMIsDouble(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c);
unsigned char HalSMIsHex(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c);
unsigned char HalSMIsBin(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c);

HalSMDoubleGet* HalSMDoubleGet_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* st);

HalSMCModule* HalSMCModule_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name);
HalSMCModule* HalSMCModule_load(HalSMCompiler* hsmc,HalSMCModule_entry* entry);

HalSMModule* HalSMModule_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,HalSMCompiler* hsmcmodule);

HalSMRunFunc* HalSMRunFunc_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMLocalFunction* func,HalSMArray* args);

HalSMRunFuncC* HalSMRunFuncC_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMFunctionC* func,HalSMArray* args);

HalSMClassC* HalSMClassC_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name);
HalSMRunClassC* HalSMClassC_run(HalSMCompiler* hsmc,HalSMClassC* classc,HalSMArray* args);

HalSMClass* HalSMClass_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,Dict* vrs);
HalSMRunClass* HalSMClass_run(HalSMClass* clas,HalSMCompiler* hsmc,HalSMArray* args);

HalSMRunClass* HalSMRunClass_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,Dict* vrs,Dict* funcs);
HalSMRunClass* HalSMRunClass__init__(HalSMRunClass* runclass,HalSMCompiler* hsmc,HalSMArray* args);

HalSMFunctionC* HalSMFunctionC_init(HalSMCompiler* hsmc,HalSMFunctionCTypeDef func);
HalSMFunctionC* HalSMFunctionC_initWithData(void* data,HalSMCompiler* hsmc,HalSMFunctionCTypeDefWithData func);
HalSMVariable* HalSMFunctionC_run(HalSMFunctionC* hfc,HalSMArray* args,Dict* vars);

HalSMRunClassC* HalSMRunClassC_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,Dict* vrs,Dict* funcs);
HalSMRunClassC* HalSMRunClassC__init__(HalSMCompiler* hsmc,HalSMRunClassC* runclassc,HalSMArray* args);

HalSMVar* HalSMVar_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name);

HalSMSetArg* HalSMSetArg_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* name);
HalSMSetArg* HalSMSetArg_initWithValue(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* name,HalSMVariable* value);

HalSMReturn* HalSMReturn_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* val,unsigned long long int idLocalFunction);

Dict* DictInit(HalSMMemoryManagmentLibrary* hsmmml);
Dict* DictInitWithElements(HalSMMemoryManagmentLibrary* hsmmml,DictElement* elements[],unsigned long long int size);
DictElement* DictElementInit(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* key,HalSMVariable* value);
DictElement* DictElementFindByKey(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* key);
DictElement* DictElementFindByValue(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* value);
DictElement* DictElementFindByIndex(Dict* dict,unsigned long long int index);
long long int DictElementIndexByKey(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* key);
long long int DictElementIndexByValue(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* value);
DictElement* DictElementCopy(HalSMMemoryManagmentLibrary* hsmmml,DictElement* elem);
void PutDictElementToDict(HalSMCompiler* hsmc,Dict* dict,DictElement* elem);
Dict* DictCopy(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict);
unsigned char DictCompare(HalSMCompiler* hsmc,Dict* a,Dict* b);
void DictFree(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict);
void DictClear(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict);
HalStringFormatChar* Dict_to_print(HalSMCompiler* hsmc,Dict* harr);
void DictAppendDict(HalSMMemoryManagmentLibrary* hsmmml,Dict* a,Dict* b);
void DictRemove(HalSMMemoryManagmentLibrary* hsmmml,Dict* a,unsigned long long int index);

HalSMVariable* HalSMVariable_init(HalSMMemoryManagmentLibrary* hsmmml,void* value,HalSMVariableType type);
void HalSMVariable_AsVar(void* var,HalSMVariable* arg);
HalSMVariable* HalSMVariable_init_str(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str);
HalSMVariable* HalSMVariable_init_int(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* n);
HalSMVariable* HalSMVariable_init_double(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* d);
HalSMVariable* HalSMVariable_init_bool(HalSMMemoryManagmentLibrary* hsmmml,unsigned char b);
HalSMVariable* HalSMVariable_init_unsigned_int(HalSMMemoryManagmentLibrary* hsmmml,unsigned long long int n);
HalSMVariable* HalSMVariable_init_char(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar c);
HalStringFormatChar* HalSMVariable_to_str(HalSMCompiler* hsmc,HalSMVariable* a,unsigned char isToPrint);
unsigned char HalSMVariable_Compare(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
void HalSMVariable_free(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* arg);
HalSMVariableCustom* HalSMVariableCustom_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariableTypeCustom* type,void* value,Dict* vars,Dict* funcs);
HalSMVariableTypeCustom* HalSMVariableTypeCustom_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariableTypeCustom type);

HalSMSetVar* HalSMSetVar_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,HalStringFormatChar* value,HalSMVariable* pointer);

HalSMVariable* ParseHalSMVariableInt(HalSMCompiler* hsmc,HalStringFormatChar* c);
HalSMVariable* ParseHalSMVariableDouble(HalSMCompiler* hsmc,HalStringFormatChar* c);

long long int StringIndexOf(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f);
long long int StringLastIndexOf(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f);
HalStringFormatChar* SubString(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,int start,int end);
HalStringFormatChar* SubStringHalSMInteger(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalSMInteger* start,HalSMInteger* end);
HalStringFormatChar* ConcatenateStrings(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f);
HalStringFormatChar* StringReplace(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f,HalStringFormatChar* r);
unsigned char StringEndsWith(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f);
unsigned char StringStartsWith(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f);
void AdditionStrings(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar** c,HalStringFormatChar* f,unsigned long long int sizec,unsigned long long int sizef);



HalSMInteger* HalSMInteger_init(HalSMMemoryManagmentLibrary* hsmmml,unsigned char negative,unsigned char* value,unsigned long long size);
HalSMInteger* HalSMInteger_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
HalSMInteger* HalSMInteger_FromUnsignedInteger(HalSMMemoryManagmentLibrary* hsmmml,unsigned int value);
HalSMInteger* HalSMInteger_FromSignedInteger(HalSMMemoryManagmentLibrary* hsmmml,signed int value);
HalSMInteger* HalSMInteger_FromUnsignedLongLongInteger(HalSMMemoryManagmentLibrary* hsmmml,unsigned long long int value);
unsigned int HalSMInteger_ToUnsignedInteger(HalSMInteger* value);
unsigned long long int HalSMInteger_ToUnsignedLongLongInteger(HalSMInteger* value);
HalSMInteger* HalSMInteger_parse(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str);
HalSMInteger* HalSMInteger_parseHex(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str,unsigned char isZeroX);
HalSMInteger* HalSMInteger_parseBin(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str);
void HalSMInteger_AddSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_Add(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_SubSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_Sub(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_MulSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_Mul(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_DivSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b,HalSMInteger* remainder);
HalSMInteger* HalSMInteger_Div(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b,HalSMInteger* remainder);
HalSMInteger* HalSMInteger_negate(HalSMInteger* a);
HalSMInteger* HalSMInteger_absolute(HalSMInteger* a);
unsigned char HalSMInteger_isMore(HalSMInteger* a,HalSMInteger* b);
unsigned char HalSMInteger_isLess(HalSMInteger* a,HalSMInteger* b);
unsigned char HalSMInteger_isEqual(HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_getValueWithoutNull(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
char* HalSMInteger_Byte2Bits(HalSMMemoryManagmentLibrary* hsmmml,unsigned char byte);
HalStringFormatChar* HalSMInteger_Bytes2Bits(HalSMMemoryManagmentLibrary* hsmmml,unsigned char* bytes,unsigned long long size);
HalStringFormatChar* HalSMInteger_toString(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,unsigned char isHex);
HalStringFormatChar* HalSMInteger_Byte2Hex(HalStringFormatChar* out,unsigned char x);
HalStringFormatChar* HalSMInteger_toStringBytes(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
unsigned char HalSMInteger_getBit(HalSMInteger* a,long long int byte,unsigned char bit);
void HalSMInteger_setBitSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int byte,unsigned char bit,unsigned char value);
HalSMInteger* HalSMInteger_setBit(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int byte,unsigned char bit,unsigned char value);
void HalSMInteger_shiftRightSelf(HalSMInteger* a,long long int bit);
void HalSMInteger_shiftLeftSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int bit);
HalSMInteger* HalSMInteger_shiftRight(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int bit);
HalSMInteger* HalSMInteger_shiftLeft(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int bit);
HalSMInteger* HalSMInteger_clear(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
void HalSMInteger_PowSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_Pow(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_Free(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
unsigned char HalSMInteger_ByteCTZ(unsigned char byte);
long long int HalSMInteger_ctz(HalSMInteger* a);
void HalSMInteger_BitORSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_BitOR(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_BitXORSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_BitXOR(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_BitANDSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
HalSMInteger* HalSMInteger_BitAND(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b);
void HalSMInteger_BitNOTSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
HalSMInteger* HalSMInteger_BitNOT(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a);
HalSMInteger* HalSMInteger_FromHalSMDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
HalSMInteger* HalSMInteger_FloorHalSMDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
HalSMInteger* HalSMInteger_CeilHalSMDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);

HalSMDouble* HalSMDouble_init(HalSMMemoryManagmentLibrary* hsmmml,unsigned char negative,HalSMInteger* a,HalSMInteger* b);
HalSMDouble* HalSMDouble_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
HalSMDouble* HalSMDouble_FromUnsignedInteger(HalSMMemoryManagmentLibrary* hsmmml,unsigned int value);
HalSMDouble* HalSMDouble_FromSignedInteger(HalSMMemoryManagmentLibrary* hsmmml,signed int value);
HalSMDouble* HalSMDouble_FromFloat(HalSMMemoryManagmentLibrary* hsmmml,float value);
HalSMDouble* HalSMDouble_FromDouble(HalSMMemoryManagmentLibrary* hsmmml,double value);
HalSMDouble* HalSMDouble_FromHalSMInteger(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* value);
HalSMDouble* HalSMDouble_parse(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str);
unsigned char HalSMDouble_isMore(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
unsigned char HalSMDouble_isLess(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
unsigned char HalSMDouble_isEqual(HalSMDouble* a,HalSMDouble* b);
void HalSMDouble_AddSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
HalSMDouble* HalSMDouble_Add(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
void HalSMDouble_SubSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
HalSMDouble* HalSMDouble_Sub(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
void HalSMDouble_MulSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
HalSMDouble* HalSMDouble_Mul(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
void HalSMDouble_DivSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
HalSMDouble* HalSMDouble_Div(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b);
HalStringFormatChar* HalSMDouble_toString(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,unsigned char isHex);
HalStringFormatChar* HalSMDouble_toStringRound(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,unsigned long long int round);
double HalSMDouble_ToDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
float HalSMDouble_ToFloat(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
void HalSMDouble_SquareRootSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* q,unsigned long long int crop);
HalSMDouble* HalSMDouble_SquareRoot(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* q,unsigned long long int crop);
void HalSMDouble_AbsSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
HalSMDouble* HalSMDouble_Abs(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
void HalSMDouble_Free(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
void HalSMDouble_PowSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b);
HalSMDouble* HalSMDouble_Pow(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b);
void HalSMDouble_RoundSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b);
HalSMDouble* HalSMDouble_Round(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b);
void HalSMDouble_CropSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,unsigned long long int crop);
HalSMInteger* HalSMDouble_gcdSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* u,HalSMInteger* v);
HalSMInteger* HalSMDouble_gcd(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* u,HalSMInteger* v);
void HalSMDouble_clearSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);
HalSMDouble* HalSMDouble_clear(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a);

HalSMFunctionArray* HalSMFunctionArray_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* args,HalSMFunctionArrayType type);

//GOTO

typedef struct HalSMGOTO {
    HalStringFormatChar* sector;
} HalSMGOTO;

typedef struct HalSMGOTOSector {
    HalStringFormatChar* sector;
    unsigned long long int index;
    HalSMLocalFunction* lf;
} HalSMGOTOSector;

HalSMGOTO* HalSMGOTO_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* sector);
HalSMGOTOSector* HalSMGOTOSector_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* sector,unsigned long long int index,HalSMLocalFunction* lf);

//HalSM File Pointer

HalStringFormatChar* HFP_toString(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HFP_add(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_sub(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_mul(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_div(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_pow(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_modulo(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_increment(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HFP_decrement(HalSMCompiler* hsmc,HalSMVariable* v);
unsigned char HFP_isEqual(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HFP_isMore(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HFP_isLess(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HFP_getBool(HalSMCompiler* hsmc,HalSMVariable* v);
long long int HFP_indexVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
long long int HFP_indexFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
HalSMVariable* HFP_getVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
HalSMVariable* HFP_getFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
HalSMVariable* HFP_getItem(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_shiftRight(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_shiftLeft(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_convert(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_xor(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_and(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_or(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HFP_bnot(HalSMCompiler* hsmc,HalSMVariable* a);
HalSMVariable* HFP_close(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_read(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_get(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_write(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_put(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_eof(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_seek(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_tell(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_rewind(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_flush(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_readUTF8(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_writeUTF8(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_readHSF(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_writeHSF(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_readUTF16(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);
HalSMVariable* HFP_writeUTF16(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars);

//HalSM Byte Array

typedef struct HalSMByteArray {
    unsigned char* bytes;
    unsigned long long int size;
} HalSMByteArray;

HalStringFormatChar* HBA_toString(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HBA_add(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_sub(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_mul(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_div(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_pow(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_modulo(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_increment(HalSMCompiler* hsmc,HalSMVariable* v);
HalSMVariable* HBA_decrement(HalSMCompiler* hsmc,HalSMVariable* v);
unsigned char HBA_isEqual(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HBA_isMore(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HBA_isLess(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
unsigned char HBA_getBool(HalSMCompiler* hsmc,HalSMVariable* v);
long long int HBA_indexVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
long long int HBA_indexFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
HalSMVariable* HBA_getVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
HalSMVariable* HBA_getFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name);
HalSMVariable* HBA_getItem(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_shiftRight(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_shiftLeft(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_convert(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_xor(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_and(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_or(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b);
HalSMVariable* HBA_bnot(HalSMCompiler* hsmc,HalSMVariable* a);

//String

unsigned long long int string_len(HalStringFormatChar* s);
HalSMInteger* string_len_HalSMInteger(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* s);
HalStringFormatChar* string_cat(HalStringFormatChar* s1,HalStringFormatChar* s2);
HalStringFormatChar* string_cpy(HalStringFormatChar* s1,HalStringFormatChar* s2);
HalStringFormatChar* stringncpy(HalStringFormatChar* s1,HalStringFormatChar* s2,unsigned long long int n);

void* memory_cpy(void* dst,const void* src,unsigned long long int n);

#define typevar(x) _Generic((x),HalStringFormatChar*:HalSMVariableType_char,HalSMVoid*:HalSMVariableType_void,HalSMInteger*:HalSMVariableType_int,int**:HalSMVariableType_int_array,\
HalSMDouble*:HalSMVariableType_double,HalSMArray*:HalSMVariableType_HalSMArray,HalStringFormatChar**:HalSMVariableType_str,HalSMFunctionC*:HalSMVariableType_HalSMFunctionC,\
HalSMRunClassC*:HalSMVariableType_HalSMRunClassC,HalSMSetArg*:HalSMVariableType_HalSMSetArg,HalSMError*:HalSMVariableType_HalSMError,\
HalSMNull*:HalSMVariableType_HalSMNull,HalSMRunFunc*:HalSMVariableType_HalSMRunFunc,HalSMLocalFunction*:HalSMVariableType_HalSMLocalFunction,\
HalSMCModule*:HalSMVariableType_HalSMCModule,HalSMModule*:HalSMVariableType_HalSMModule,HalSMClassC*:HalSMVariableType_HalSMClassC,\
HalSMCompiler*:HalSMVariableType_HalSMCompiler,HalSMRunClass*:HalSMVariableType_HalSMRunClass,HalSMDoubleGet*:HalSMVariableType_HalSMDoubleGet,\
HalSMClass*:HalSMVariableType_HalSMClass,HalSMVar*:HalSMVariableType_HalSMVar,HalSMPlus*:HalSMVariableType_HalSMPlus,\
HalSMMinus*:HalSMVariableType_HalSMMinus,HalSMMult*:HalSMVariableType_HalSMMult,HalSMDivide*:HalSMVariableType_HalSMDivide,\
HalSMEqual*:HalSMVariableType_HalSMEqual,HalSMNotEqual*:HalSMVariableType_HalSMNotEqual,HalSMMore*:HalSMVariableType_HalSMMore,\
HalSMLess*:HalSMVariableType_HalSMLess,unsigned char*:HalSMVariableType_HalSMBool,\
Dict*:HalSMVariableType_HalSMDict,HalSMReturn*:HalSMVariableType_HalSMReturn,HalSMSetVar*:HalSMVariableType_HalSMSetVar,\
HalSMFunctionCTypeDef*:HalSMVariableType_HalSMFunctionCTypeDef,HalSMFunctionArray*:HalSMVariableType_HalSMFunctionArray,\
unsigned long long int*:HalSMVariableType_unsigned_int,HalSMRunFuncC*:HalSMVariableType_HalSMRunFuncC,HalSMVariableCustom*:HalSMVariableType_custom,\
HalSMMoreEqual*:HalSMVariableType_HalSMMoreEqual,HalSMLessEqual*:HalSMVariableType_HalSMLessEqual,HalSMShift*:HalSMVariableType_HalSMShift,\
HalSMPow*:HalSMVariableType_HalSMPow,HalSMModulo*:HalSMVariableType_HalSMModulo,HalSMAND*:HalSMVariableType_HalSMAND,HalSMOR*:HalSMVariableType_HalSMOR,\
HalSMNegate*:HalSMVariableType_HalSMNegate,HalSMNegativeSign*:HalSMVariableType_HalSMNegativeSign,HalSMGOTO*:HalSMVariableType_HalSMGOTO,HalSMGOTOSector*:HalSMVariableType_HalSMGOTOSector,\
HalSMInlineIF*:HalSMVariableType_HalSMInlineIf,HalSMInlineElse*:HalSMVariableType_HalSMInlineElse,HalSMBreak*:HalSMVariableType_HalSMBreak,HalSMXOR*:HalSMVariableType_HalSMXOR,\
HalSMBAND*:HalSMVariableType_HalSMBAND,HalSMBOR*:HalSMVariableType_HalSMBOR,HalSMBNOT*:HalSMVariableType_HalSMBNOT)


#define HalSMVariable_auto(val) (HalSMVariable_init((val),typevar(val)))
#define HalSMVariable_AsVarAuto(var,arg) *(var)=*(__typeof__(*(var))*)(arg)->value;
#define HalSMVariable_GetValue(arg) ({\
    void* var;\
    if((arg)->type==HalSMVariableType_str){HalStringFormatChar* var;}\
    else if((arg)->type==HalSMVariableType_int){HalSMInteger var;}\
    else if((arg)->type==HalSMVariableType_char){HalStringFormatChar var;}\
    else if((arg)->type==HalSMVariableType_double){HalSMDouble var;}\
    else if((arg)->type==HalSMVariableType_HalSMArray){HalSMArray var;}\
    else if((arg)->type==HalSMVariableType_int_array){int* var;}\
    else if((arg)->type==HalSMVariableType_HalSMFunctionC){HalSMFunctionC var;}\
    else if((arg)->type==HalSMVariableType_HalSMRunClassC){HalSMRunClassC var;}\
    else if((arg)->type==HalSMVariableType_HalSMSetArg){HalSMSetArg var;}\
    else if((arg)->type==HalSMVariableType_HalSMError){HalSMError var;}\
    else if((arg)->type==HalSMVariableType_HalSMNull){HalSMNull var;}\
    else if((arg)->type==HalSMVariableType_HalSMRunFunc){HalSMRunFunc var;}\
    else if((arg)->type==HalSMVariableType_HalSMRunFuncC){HalSMRunFuncC var;}\
    else if((arg)->type==HalSMVariableType_HalSMLocalFunction){HalSMLocalFunction var;}\
    else if((arg)->type==HalSMVariableType_HalSMCModule){HalSMCModule var;}\
    else if((arg)->type==HalSMVariableType_HalSMModule){HalSMModule var;}\
    else if((arg)->type==HalSMVariableType_HalSMClassC){HalSMClassC var;}\
    else if((arg)->type==HalSMVariableType_HalSMCompiler){HalSMCompiler var;}\
    else if((arg)->type==HalSMVariableType_HalSMCompiler_source){HalSMCompiler* var;}\
    else if((arg)->type==HalSMVariableType_HalSMRunClassC_source){HalSMRunClassC* var;}\
    else if((arg)->type==HalSMVariableType_HalSMRunClass_source){HalSMRunClass* var;}\
    else if((arg)->type==HalSMVariableType_HalSMRunClass){HalSMRunClass var;}\
    else if((arg)->type==HalSMVariableType_HalSMDoubleGet){HalSMDoubleGet var;}\
    else if((arg)->type==HalSMVariableType_HalSMClass){HalSMClass var;}\
    else if((arg)->type==HalSMVariableType_HalSMVar){HalSMVar var;}\
    else if((arg)->type==HalSMVariableType_HalSMPlus){HalSMPlus var;}\
    else if((arg)->type==HalSMVariableType_HalSMMinus){HalSMMinus var;}\
    else if((arg)->type==HalSMVariableType_HalSMMult){HalSMMult var;}\
    else if((arg)->type==HalSMVariableType_HalSMDivide){HalSMDivide var;}\
    else if((arg)->type==HalSMVariableType_HalSMEqual){HalSMEqual var;}\
    else if((arg)->type==HalSMVariableType_HalSMNotEqual){HalSMNotEqual var;}\
    else if((arg)->type==HalSMVariableType_HalSMMore){HalSMMore var;}\
    else if((arg)->type==HalSMVariableType_HalSMLess){HalSMLess var;}\
    else if((arg)->type==HalSMVariableType_HalSMBool){unsigned char var;}\
    else if((arg)->type==HalSMVariableType_HalSMDict){Dict var;}\
    else if((arg)->type==HalSMVariableType_HalSMSetVar){HalSMSetVar var;}\
    else if((arg)->type==HalSMVariableType_HalSMReturn){HalSMReturn var;}\
    else if((arg)->type==HalSMVariableType_HalSMFunctionArray){HalSMFunctionArray var;}\
    else if((arg)->type==HalSMVariableType_unsigned_int){unsigned long long int var;}\
    else if((arg)->type==HalSMVariableType_custom){HalSMVariableCustom var;}\
    else if((arg)->type==HalSMVariableType_HalSMMoreEqual){HalSMMoreEqual var;}\
    else if((arg)->type==HalSMVariableType_HalSMLessEqual){HalSMLessEqual var;}\
    else if((arg)->type==HalSMVariableType_HalSMShift){HalSMShift var;}\
    else if((arg)->type==HalSMVariableType_HalSMPow){HalSMPow var;}\
    else if((arg)->type==HalSMVariableType_HalSMModulo){HalSMModulo var;}\
    else if((arg)->type==HalSMVariableType_HalSMAND){HalSMAND var;}\
    else if((arg)->type==HalSMVariableType_HalSMOR){HalSMOR var;}\
    else if((arg)->type==HalSMVariableType_HalSMNegate){HalSMNegate var;}\
    else if((arg)->type==HalSMVariableType_HalSMNegativeSign){HalSMNegativeSign var;}\
    else if((arg)->type==HalSMVariableType_HalSMGOTO){HalSMGOTO var;}\
    else if((arg)->type==HalSMVariableType_HalSMGOTOSector){HalSMGOTOSector var;}\
    else if((arg)->type==HalSMVariableType_HalSMInlineIf){HalSMInlineIf var;}\
    else if((arg)->type==HalSMVariableType_HalSMInlineElse){HalSMInlineElse var;}\
    else if((arg)->type==HalSMVariableType_HalSMBreak){HalSMBreak var;}\
    else if((arg)->type==HalSMVariableType_HalSMXOR){HalSMXOR var;}\
    else if((arg)->type==HalSMVariableType_HalSMBAND){HalSMBAND var;}\
    else if((arg)->type==HalSMVariableType_HalSMBOR){HalSMBOR var;}\
    else if((arg)->type==HalSMVariableType_HalSMBNOT){HalSMBNOT var;}\
    __typeof__(var) out=*(__typeof__(var)*)(arg)->value;\
    out;\
})

#define HalSMVariable_FromValue(hsmmml,arg) ({\
    __typeof__((arg))* var=(__typeof__((arg))*)(hsmmml)->malloc(sizeof(__typeof__((arg))));\
    *var=(arg);\
    HalSMVariable_init(hsmmml,var,typevar(var));\
})

#define HalSMVariable_FromValueWithType(hsmmml,arg,type) ({\
    type* var=(type*)hsmmml->malloc(sizeof(type));\
    *var=(arg);\
    HalSMVariable_init(hsmmml,var,typevar(var));\
})

#define HalSMVariable_copy(hsmmml,arg) ({\
    __auto_type out=HalSMVariable_GetValue(arg);\
    __typeof__(out)* var=hsmmml->malloc(sizeof(__typeof__(out)));\
    *var=out;\
    HalSMVariable_init(hsmmml,var,(arg)->type);\
})

#define DictForEach(keyOutDictForEach,valueOutDictForEach,dict) \
    HalSMVariable* keyOutDictForEach=(dict)->elements[0]->key;HalSMVariable* valueOutDictForEach=(dict)->elements[0]->value;\
    for (unsigned long long int indexDictForEach=0;indexDictForEach<(dict)->size;indexDictForEach++,keyOutDictForEach=(dict)->elements[indexDictForEach]->key,valueOutDictForEach=(dict)->elements[indexDictForEach]->value)

#define HalSMArrayForEach(elementHalSMArrayForEach,array) \
    HalSMVariable* elementHalSMArrayForEach=(array)->arr[0];\
    for (unsigned long long int indexHalSMArrayForEach=0;indexHalSMArrayForEach<(array)->size;indexHalSMArrayForEach++,elementHalSMArrayForEach=(array)->arr[indexHalSMArrayForEach])

#define MathMin(a,b) ((a)<(b)?(a):(b))
#define MathMax(a,b) ((a)>(b)?(a):(b))
#define MathAbs(a) ((a)<0?-(a):(a))

#define MathCeilPos(a) ((a)-(int)(a)>0?(int)((a)+1):(int)(a))
#define MathCeilNeg(a) ((a)-(int)(a)<0?(int)((a)-1):(int)(a))
#define MathCeil(a) ((a)>0?MathCeilPos((a)):MathCeilNeg((a)))

#endif