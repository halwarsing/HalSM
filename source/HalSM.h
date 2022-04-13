#ifndef HALSM_H
#define HALSM_H
#define NOTHALSMNULLPOINTER ((void*)0)

typedef struct HalSMNull {unsigned char c;} HalSMNull;

typedef struct HalSMError {
    unsigned int line;
    char *error;
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
    HalSMVariableType_HalSMCompiler_source,
    HalSMVariableType_HalSMRunClassC_source,
    HalSMVariableType_HalSMRunClass_source,
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
    HalSMVariableType_HalSMCLElement,
    HalSMVariableType_HalSMDict,
    HalSMVariableType_HalSMSetVar,
    HalSMVariableType_HalSMReturn,
    HalSMVariableType_HalSMFunctionCTypeDef,
    HalSMVariableType_HalSMFunctionArray,
    HalSMVariableType_unsigned_int
} HalSMVariableType;

typedef struct HalSMVariable {
    void* value;
    HalSMVariableType type;
} HalSMVariable;

typedef struct HalSMArray {
    HalSMVariable** arr;
    unsigned int size;
} HalSMArray;

typedef enum HalSMFunctionArrayType {
    HalSMFunctionArrayType_function,
    HalSMFunctionArrayType_array,
    HalSMFunctionArrayType_var
} HalSMFunctionArrayType;

typedef struct HalSMFunctionArray {
    HalSMArray* args;
    HalSMFunctionArrayType type;
} HalSMFunctionArray;

typedef struct DictElement {
    HalSMVariable* key;
    HalSMVariable* value;
} DictElement;

typedef struct DictElementForEach {
    HalSMVariable* key;
    HalSMVariable* value;
    unsigned int index;
} DictElementForEach;

typedef struct Dict {
    unsigned int size;
    DictElement** elements;
} Dict;



typedef struct HalSMFileSystemLibrary {
    char*(*readFile)(char*);
    unsigned char(*isExistsFileOrDir)(char*);
    char*(*getCurrentPath)();
} HalSMFileSystemLibrary;

typedef struct HalSMLoadSharedLibrary {
    void*(*loadLibrary)(char*);
    void*(*getAddressByName)(void*,char*);
    void(*closeLibrary)(void*);
} HalSMLoadSharedLibrary;

typedef struct HalSMStringLibrary {
    char*(*Decimal2Str)(long long);
    char*(*Decimal2HexStr)(long long);
    char*(*Double2Str)(double);
    long long(*ParseDecimal)(char*);
    double(*ParseDouble)(char*);
} HalSMStringLibrary;

typedef struct HalSMMemoryManagmentLibrary {
    void*(*malloc)(unsigned int);
    void*(*calloc)(unsigned int,unsigned int);
    void*(*realloc)(void*,unsigned int);
    void(*free)(void*);
} HalSMMemoryManagmentLibrary;

typedef struct HalSMSystemLibrary {
    void(*exit)(int);
} HalSMSystemLibrary;



typedef struct HalSMCalculateVars {
    char *version;
    char*(*addStr)(HalSMStringLibrary*,HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    int(*addInt)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    double(*addDouble)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    char*(*subStr)(HalSMStringLibrary*,HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    int(*subInt)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    double(*subDouble)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    char*(*mulStr)(HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    int(*mulInt)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    double(*mulDouble)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    char*(*divStr)(HalSMStringLibrary*,HalSMMemoryManagmentLibrary*,HalSMVariable*,HalSMVariable*);
    int(*divInt)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
    double(*divDouble)(HalSMStringLibrary*,HalSMVariable*,HalSMVariable*);
} HalSMCalculateVars;

typedef struct HalSMCompiler {
    Dict* functions;
    Dict* sys_modules;
    HalSMCalculateVars calcVars;
    HalSMArray* numbers;
    unsigned int line;
    Dict* sys_variables;
    Dict* variables;
    Dict* modules;
    Dict* localFunctions;
    Dict* classes;
    char* path;
    char* pathModules;
    void(*print)(char*);
    void(*printErrorf)(char*);
    char*(*inputf)(char*);
    char*(*readFilef)(char*);
    HalSMArray* externModules;
    HalSMLoadSharedLibrary* loadsharedlibrary;
    HalSMStringLibrary* stringlibrary;
    HalSMMemoryManagmentLibrary* memorymanagmentlibrary;
    HalSMSystemLibrary* systemlibrary;
} HalSMCompiler;

typedef HalSMVariable*(*HalSMFunctionCTypeDef)(HalSMCompiler*,HalSMArray*);

typedef struct HalSM {
    char version[30];
    HalSMArray* externModules;
    char* pathModules;
    void(*print)(char*);
    void(*printErrorf)(char*);
    char*(*inputf)(char*);
    char*(*readFilef)(char*);
    HalSMLoadSharedLibrary* loadsharedlibrary;
    HalSMStringLibrary* stringlibrary;
    HalSMMemoryManagmentLibrary* memorymanagmentlibrary;
    HalSMSystemLibrary* systemlibrary;
} HalSM;

typedef struct HalSMFunctionC {
    HalSMFunctionCTypeDef func;
} HalSMFunctionC;

typedef struct HalSMRunClassC {
    char* name;
    Dict* vrs;
    Dict* funcs;
} HalSMRunClassC;

typedef struct HalSMClassC {
    Dict* vrs;
    Dict* funcs;
    char* name;
    void(*init_runclass)(HalSMRunClassC*);
} HalSMClassC;

typedef struct HalSMCModule {
    Dict* lfuncs;
    Dict* vrs;
    Dict* classes;
    char* name;
} HalSMCModule;

typedef struct HalSMCModule_entry {
    char* name;
    char* description;
    Dict* classes;
    Dict* lfuncs;
    Dict* vrs;
} HalSMCModule_entry;

typedef struct HalSMModule {
    char* name;
    Dict* vrs;
    Dict* lfuncs;
    Dict* classes;
} HalSMModule;

typedef struct HalSMLocalFunction {
    char* name;
    HalSMArray* args;
    HalSMArray* func;
    Dict* vars;
} HalSMLocalFunction;

typedef struct HalSMVar {
    char* name;
} HalSMVar;

typedef struct HalSMPlus {unsigned char c;} HalSMPlus;
typedef struct HalSMMinus {unsigned char c;} HalSMMinus;
typedef struct HalSMMult {unsigned char c;} HalSMMult;
typedef struct HalSMDivide {unsigned char c;} HalSMDivide;

typedef struct HalSMEqual {unsigned char c;} HalSMEqual;
typedef struct HalSMNotEqual {unsigned char c;} HalSMNotEqual;
typedef struct HalSMMore {unsigned char c;} HalSMMore;
typedef struct HalSMLess {unsigned char c;} HalSMLess;

typedef struct HalSMSetArg {
    char *name;
    HalSMVariable* value;
} HalSMSetArg;

typedef struct HalSMRunFuncC {
    HalSMFunctionC* func;
    HalSMArray* args;
} HalSMRunFuncC;

typedef struct HalSMRunFunc {
    HalSMLocalFunction* func;
    HalSMArray* args;
} HalSMRunFunc;

typedef struct HalSMRunClass {
    char* name;
    Dict* funcs;
    Dict* vars;
} HalSMRunClass;

typedef struct HalSMClass {
    char* name;
    Dict* funcs;
    Dict* vars;
} HalSMClass;

typedef struct HalSMDoubleGet {
    char* st;
} HalSMDoubleGet;

typedef struct HalSMSetVar {
    char* name;
    char* value;
} HalSMSetVar;

typedef struct HalSMReturn {
    HalSMArray* value;
} HalSMReturn;



typedef enum HalSMCLElementType {
    HalSMCLElementType_while,
    HalSMCLElementType_elif,
    HalSMCLElementType_if,
    HalSMCLElementType_else,
    HalSMCLElementType_for
} HalSMCLElementType;

typedef struct HalSMCLElement {
    HalSMArray* func;
    void(*addFunc)(HalSMMemoryManagmentLibrary*,void*,HalSMVariable*);
    HalSMVariable*(*start)(void*,HalSMCompiler*);
    HalSMCLElementType type;
    void* element;
} HalSMCLElement;

typedef struct HalSMFor {
    HalSMVariable* var;
    HalSMArray* arr;
} HalSMFor;

typedef struct HalSMIf {
    HalSMArray* arr;
} HalSMIf;

typedef struct HalSMElse {
    unsigned char c;
} HalSMElse;

typedef struct HalSMWhile {
    HalSMArray* arr;
} HalSMWhile;



typedef struct HalSMInteger {
    unsigned char* value;
    unsigned long long size;
} HalSMInteger;



extern HalSMNull null;
extern HalSMPlus plus;
extern HalSMMinus minus;
extern HalSMMult mult;
extern HalSMDivide divide;
extern HalSMEqual equal;
extern HalSMNotEqual notequal;
extern HalSMMore more;
extern HalSMLess less;
extern HalSMArray* arrInt;

HalSMNull* HalSMNull_init(HalSMCompiler* hsmc);
HalSMError* HalSMError_init(HalSMCompiler* hsmc,unsigned int line,char* error);

HalSMArray* HalSMArray_init(HalSMMemoryManagmentLibrary* hsmmml);
HalSMArray* HalSMArray_init_with_elements(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable** arr,unsigned int size);
HalSMArray* HalSMArray_split_str(HalSMMemoryManagmentLibrary* hsmmml,char* str,char* spl);
void HalSMArray_add(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value);
void HalSMArray_set(HalSMArray* harr,HalSMVariable* value,unsigned int index);
void HalSMArray_remove(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned int index);
void HalSMArray_appendArray(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMArray* t);
void HalSMArray_insert(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value,unsigned int index);
HalSMVariable* HalSMArray_get(HalSMArray* harr,unsigned int index);
HalSMArray* HalSMArray_reverse(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);
char* HalSMArray_join_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,char* join);
char* HalSMArray_to_print(HalSMCompiler* hsmc,HalSMArray* harr);
char* HalSMArray_chars_to_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);
HalSMArray* HalSMArray_slice(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned int s,unsigned int e);
unsigned char HalSMArray_compare(HalSMArray* harr,HalSMArray* barr);
HalSMArray* HalSMArray_from_str(HalSMMemoryManagmentLibrary* hsmmml,char* str,unsigned int size);
HalSMArray* HalSMArray_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr);

HalSM* HalSM_init(HalSMArray* externModules,void(*printf)(char*),void(*printErrorf)(char*),char*(*inputf)(char*),char*(*readFilef)(char*),char* pathModules,HalSMLoadSharedLibrary* loadsharedlibrary,HalSMStringLibrary* stringlibrary,HalSMMemoryManagmentLibrary* memorymanagmentlibrary,HalSMSystemLibrary* systemlibrary);
void HalSM_compile(HalSM* hsm,char* code,char* path);
void HalSM_compile_without_path(HalSM* hsm,char* code);

HalSMCalculateVars HalSMCalculateVars_init();
char* HalSMCalculateVars_addStr(HalSMStringLibrary* hsmsl,HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
char* HalSMCalculateVars_subStr(HalSMStringLibrary* hsmsl,HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
char* HalSMCalculateVars_mulStr(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
char* HalSMCalculateVars_divStr(HalSMStringLibrary* hsmsl,HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1);
int HalSMCalculateVars_addInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
int HalSMCalculateVars_subInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
int HalSMCalculateVars_mulInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
int HalSMCalculateVars_divInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
double HalSMCalculateVars_addDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
double HalSMCalculateVars_subDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
double HalSMCalculateVars_mulDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);
double HalSMCalculateVars_divDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1);

HalSMVariable* HalSMCompiler_readFile(HalSMCompiler* hsmc,HalSMArray* args);
HalSMVariable* HalSMCompiler_input(HalSMCompiler* hsmc,HalSMArray* args);
HalSMVariable* HalSMCompiler_reversed(HalSMCompiler* hsmc,HalSMArray* args);
HalSMVariable* HalSMCompiler_range(HalSMCompiler* hsmc,HalSMArray* args);
HalSMVariable* HalSMCompiler_print(HalSMCompiler* hsmc,HalSMArray* args);
HalSMArray* HalSMCompiler_get_print_text(HalSMCompiler* hsmc,HalSMArray* args);
HalSMCompiler* HalSMCompiler_init(char* path,HalSMArray* externModules,void(*printf)(char*),void(*printErrorf)(char*),char*(*inputf)(char*),char*(*readFilef)(char*),char* pathModules,HalSMLoadSharedLibrary* loadsharedlibrary,HalSMStringLibrary* stringlibrary,HalSMMemoryManagmentLibrary* memorymanagmentlibrary,HalSMSystemLibrary* systemlibrary);
HalSMArray* HalSMCompiler_getLines(HalSMCompiler* hsmc,char* text);
void HalSMCompiler_ThrowError(HalSMCompiler* hsmc,int line,char* error);
HalSMVariable* HalSMCompiler_getNameFunction(HalSMCompiler* hsmc,char* l);
HalSMVariable* HalSMCompiler_isSetVar(HalSMCompiler* hsmc,char* l);
HalSMArray* HalSMCompiler_getTabs(HalSMCompiler* hsmc,char* l);
unsigned char HalSMCompiler_isNull(char* text);
HalSMArray* HalSMCompiler_compile(HalSMCompiler* hsmc,char* text,unsigned char isConsole);
HalSMModule* HalSMCompiler_loadHalSMModule(HalSMCompiler* hsmc,char* name,char* file);

HalSMVariable* HalSMCompiler_isGet(HalSMCompiler* hsmc,char* l,unsigned char ret);
HalSMArray* HalSMCompiler_getArgs(HalSMCompiler* hsmc,char* l,unsigned char tabs);
HalSMVariable* HalSMCompiler_getArgsSetVar(HalSMCompiler* hsmc,char* value);
HalSMVariable* HalSMCompiler_isRunFunction(HalSMCompiler* hsmc,unsigned char tabs,char* l);

HalSMVariable* HalSMCompiler_additionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_subtractionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_multiplyVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);
HalSMVariable* HalSMCompiler_divideVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1);

HalSMLocalFunction* HalSMLocalFunction_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,char* args,Dict* vrs);
HalSMVariable* HalSMLocalFunction_run(HalSMLocalFunction* lf,HalSMCompiler* hsmc,HalSMArray* args);

unsigned char HalSMCompiler_isMore(HalSMVariable* a,HalSMVariable* b);
unsigned char HalSMCompiler_isLess(HalSMVariable* a,HalSMVariable* b);

unsigned char HalSMIsInt(HalSMMemoryManagmentLibrary* hsmmml,char *c);
unsigned char HalSMIsDouble(HalSMMemoryManagmentLibrary* hsmmml,char *c);

HalSMDoubleGet* HalSMDoubleGet_init(HalSMMemoryManagmentLibrary* hsmmml,char* st);

HalSMCModule* HalSMCModule_init(HalSMMemoryManagmentLibrary* hsmmml,char* name);
HalSMCModule* HalSMCModule_load(HalSMMemoryManagmentLibrary* hsmmml,HalSMLoadSharedLibrary* loadsharedlibrary,char* path,char* nameModule);

HalSMModule* HalSMModule_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,Dict* vrs,Dict* lfuncs,Dict* classes);

HalSMRunFunc* HalSMRunFunc_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMLocalFunction* func,HalSMArray* args);

HalSMRunFuncC* HalSMRunFuncC_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMFunctionC* func,HalSMArray* args);

HalSMClassC* HalSMClassC_init(HalSMMemoryManagmentLibrary* hsmmml,void(*init_runclass)(HalSMRunClassC*),char* name);
HalSMRunClassC* HalSMClassC_run(HalSMCompiler* hsmc,HalSMClassC* classc,HalSMArray* args);

HalSMClass* HalSMClass_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,Dict* vrs);
HalSMRunClass* HalSMClass_run(HalSMClass* class,HalSMCompiler* hsmc,HalSMArray* args);

HalSMRunClass* HalSMRunClass_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,Dict* vrs,Dict* funcs);
HalSMRunClass* HalSMRunClass__init__(HalSMRunClass* runclass,HalSMCompiler* hsmc,HalSMArray* args);

HalSMFunctionC* HalSMFunctionC_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMFunctionCTypeDef func);
HalSMVariable* HalSMFunctionC_run(HalSMCompiler* hsmc,HalSMFunctionC* hfc,HalSMArray* args);
void HalSMFunctionC_GetArg(void* var,HalSMArray* args,unsigned int index);

HalSMRunClassC* HalSMRunClassC_init(HalSMMemoryManagmentLibrary* hsmmml,void(*init_runclass)(HalSMRunClassC*),char* name,Dict* vrs,Dict* funcs);
HalSMRunClassC* HalSMRunClassC__init__(HalSMCompiler* hsmc,HalSMRunClassC* runclassc,HalSMArray* args);

HalSMVar* HalSMVar_init(HalSMMemoryManagmentLibrary* hsmmml,char* name);

HalSMSetArg* HalSMSetArg_init(HalSMMemoryManagmentLibrary* hsmmml,char* name);

HalSMReturn* HalSMReturn_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* val);

Dict* DictInit(HalSMMemoryManagmentLibrary* hsmmml);
Dict* DictInitWithElements(HalSMMemoryManagmentLibrary* hsmmml,DictElement* elements[],unsigned int size);
DictElement* DictElementInit(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* key,HalSMVariable* value);
DictElement* DictElementFindByKey(Dict* dict,HalSMVariable* key);
DictElement* DictElementFindByValue(Dict* dict,HalSMVariable* value);
DictElement* DictElementFindByIndex(Dict* dict,unsigned int index);
int DictElementIndexByKey(Dict* dict,HalSMVariable* key);
int DictElementIndexByValue(Dict* dict,HalSMVariable* value);
void PutDictElementToDict(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict,DictElement* elem);
Dict* DictCopy(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict);
unsigned char DictCompare(Dict* a,Dict* b);

HalSMVariable* HalSMVariable_init(HalSMMemoryManagmentLibrary* hsmmml,void* value,HalSMVariableType type);
void HalSMVariable_AsVar(void* var,HalSMVariable* arg);
void* HalSMVariable_Read(HalSMVariable* arg);
HalSMVariable* HalSMVariable_init_str(HalSMMemoryManagmentLibrary* hsmmml,char* str);
char* HalSMVariable_to_str(HalSMStringLibrary* hsmsl,HalSMVariable* var);
unsigned char HalSMVariable_Compare(HalSMVariable* v0,HalSMVariable* v1);

HalSMSetVar* HalSMSetVar_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,char* value);

HalSMVariable* ParseHalSMVariableInt(HalSMCompiler* hsmc,char* c);
HalSMVariable* ParseHalSMVariableDouble(HalSMCompiler* hsmc,char* c);

int StringIndexOf(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f);
int StringLastIndexOf(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f);
unsigned char StringCompare(char* c,char* f);
char* SubString(HalSMMemoryManagmentLibrary* hsmmml,char* c,int start,int end);
char* ConcatenateStrings(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f);
char* StringReplace(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f,char* r);
unsigned char StringEndsWith(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f);
unsigned char StringStartsWith(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f);
void AdditionStrings(HalSMMemoryManagmentLibrary* hsmmml,char** c,char* f,unsigned int sizec,unsigned int sizef);

HalSMInteger HalSMInteger_init(unsigned char* value,unsigned long long size);
HalSMInteger HalSMInteger_FromSignedInteger(signed int value);
HalSMInteger HalSMInteger_Add(HalSMInteger a,HalSMInteger b);
HalSMInteger HalSMInteger_Sub(HalSMInteger a,HalSMInteger b);
char* HalSMInteger_toString(HalSMInteger a);

HalSMCLElement* HalSMCLElement_init(HalSMMemoryManagmentLibrary* hsmmml,void(*addFunc)(HalSMMemoryManagmentLibrary*,void*,HalSMVariable*),HalSMVariable*(*start)(void*,HalSMCompiler*),HalSMCLElementType type,void* element);

HalSMCLElement* HalSMFor_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* var,HalSMArray* arr);
HalSMVariable* HalSMFor_run(HalSMArray* func,HalSMCompiler* hsmc);
void HalSMFor_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func);
HalSMVariable* HalSMFor_start(void* element,HalSMCompiler* hsmc);

HalSMCLElement* HalSMIf_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* arr);
HalSMVariable* HalSMIf_run(HalSMArray* func,HalSMCompiler* hsmc);
void HalSMIf_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func);
HalSMVariable* HalSMIf_start(void* element,HalSMCompiler* hsmc);

HalSMCLElement* HalSMElif_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* arr);

HalSMCLElement* HalSMElse_init(HalSMMemoryManagmentLibrary* hsmmml);
HalSMVariable* HalSMElse_run(HalSMArray* func,HalSMCompiler* hsmc);
void HalSMElse_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func);
HalSMVariable* HalSMElse_start(void* element,HalSMCompiler* hsmc);

HalSMCLElement* HalSMWhile_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* arr);
HalSMVariable* HalSMWhile_run(HalSMArray* func,HalSMCompiler* hsmc);
void HalSMWhile_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func);
HalSMVariable* HalSMWhile_start(void* element,HalSMCompiler* hsmc);

HalSMFunctionArray* HalSMFunctionArray_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* args,HalSMFunctionArrayType type);

unsigned int string_len(char* s);
char* string_cat(char* s1,const char* s2);
char* string_cpy(char* s1,const char* s2);
char* stringncpy(char* s1,const char* s2,unsigned int n);

void* memory_cpy(void* dst,const void* src,unsigned int n);

#define typevar(x) _Generic((x),char*:HalSMVariableType_char,void*:HalSMVariableType_void,int*:HalSMVariableType_int,int**:HalSMVariableType_int_array,\
double*:HalSMVariableType_double,HalSMArray*:HalSMVariableType_HalSMArray,char**:HalSMVariableType_str,HalSMFunctionC*:HalSMVariableType_HalSMFunctionC,\
HalSMRunClassC*:HalSMVariableType_HalSMRunClassC,HalSMSetArg*:HalSMVariableType_HalSMSetArg,HalSMError*:HalSMVariableType_HalSMError,\
HalSMNull*:HalSMVariableType_HalSMNull,HalSMRunFunc*:HalSMVariableType_HalSMRunFunc,HalSMLocalFunction*:HalSMVariableType_HalSMLocalFunction,\
HalSMCModule*:HalSMVariableType_HalSMCModule,HalSMModule*:HalSMVariableType_HalSMModule,HalSMClassC*:HalSMVariableType_HalSMClassC,\
HalSMCompiler*:HalSMVariableType_HalSMCompiler,HalSMCompiler**:HalSMVariableType_HalSMCompiler_source,\
HalSMRunClassC**:HalSMVariableType_HalSMRunClassC_source,HalSMRunClass**:HalSMVariableType_HalSMRunClass_source,\
HalSMRunClass*:HalSMVariableType_HalSMRunClass,HalSMDoubleGet*:HalSMVariableType_HalSMDoubleGet,\
HalSMClass*:HalSMVariableType_HalSMClass,HalSMVar*:HalSMVariableType_HalSMVar,HalSMPlus*:HalSMVariableType_HalSMPlus,\
HalSMMinus*:HalSMVariableType_HalSMMinus,HalSMMult*:HalSMVariableType_HalSMMult,HalSMDivide*:HalSMVariableType_HalSMDivide,\
HalSMEqual*:HalSMVariableType_HalSMEqual,HalSMNotEqual*:HalSMVariableType_HalSMNotEqual,HalSMMore*:HalSMVariableType_HalSMMore,\
HalSMLess*:HalSMVariableType_HalSMLess,unsigned char*:HalSMVariableType_HalSMBool,HalSMCLElement*:HalSMVariableType_HalSMCLElement,\
Dict*:HalSMVariableType_HalSMDict,HalSMReturn*:HalSMVariableType_HalSMReturn,HalSMSetVar*:HalSMVariableType_HalSMSetVar,\
HalSMFunctionCTypeDef*:HalSMVariableType_HalSMFunctionCTypeDef,HalSMFunctionArray*:HalSMVariableType_HalSMFunctionArray,\
unsigned int*:HalSMVariableType_unsigned_int,HalSMRunFuncC*:HalSMVariableType_HalSMRunFuncC)


#define HalSMVariable_auto(val) (HalSMVariable_init(val,typevar(val)))
#define HalSMVariable_AsVarAuto(var,arg) *var=*(__typeof__(*var)*)arg->value;
#define HalSMVariable_GetValue(arg) ({\
    void* var;\
    if(arg.type==HalSMVariableType_str){char* var;}\
    else if(arg.type==HalSMVariableType_int){int var;}\
    else if(arg.type==HalSMVariableType_char){char var;}\
    else if(arg.type==HalSMVariableType_double){double var;}\
    else if(arg.type==HalSMVariableType_HalSMArray){HalSMArray var;}\
    else if(arg.type==HalSMVariableType_int_array){int* var;}\
    else if(arg.type==HalSMVariableType_HalSMFunctionC){HalSMFunctionC var;}\
    else if(arg.type==HalSMVariableType_HalSMRunClassC){HalSMRunClassC var;}\
    else if(arg.type==HalSMVariableType_HalSMSetArg){HalSMSetArg var;}\
    else if(arg.type==HalSMVariableType_HalSMError){HalSMError var;}\
    else if(arg.type==HalSMVariableType_HalSMNull){HalSMNull var;}\
    else if(arg.type==HalSMVariableType_HalSMRunFunc){HalSMRunFunc var;}\
    else if(arg.type==HalSMVariableType_HalSMRunFuncC){HalSMRuNFuncC var;}\
    else if(arg.type==HalSMVariableType_HalSMLocalFunction){HalSMLocalFunction var;}\
    else if(arg.type==HalSMVariableType_HalSMCModule){HalSMCModule var;}\
    else if(arg.type==HalSMVariableType_HalSMModule){HalSMModule var;}\
    else if(arg.type==HalSMVariableType_HalSMClassC){HalSMClassC var;}\
    else if(arg.type==HalSMVariableType_HalSMCompiler){HalSMCompiler var;}\
    else if(arg.type==HalSMVariableType_HalSMCompiler_source){HalSMCompiler* var;}\
    else if(arg.type==HalSMVariableType_HalSMRunClassC_source){HalSMRunClassC* var;}\
    else if(arg.type==HalSMVariableType_HalSMRunClass_source){HalSMRunClass* var;}\
    else if(arg.type==HalSMVariableType_HalSMRunClass){HalSMRunClass var;}\
    else if(arg.type==HalSMVariableType_HalSMDoubleGet){HalSMDoubleGet var;}\
    else if(arg.type==HalSMVariableType_HalSMClass){HalSMClass var;}\
    else if(arg.type==HalSMVariableType_HalSMVar){HalSMVar var;}\
    else if(arg.type==HalSMVariableType_HalSMPlus){HalSMPlus var;}\
    else if(arg.type==HalSMVariableType_HalSMMinus){HalSMMinus var;}\
    else if(arg.type==HalSMVariableType_HalSMMult){HalSMMult var;}\
    else if(arg.type==HalSMVariableType_HalSMDivide){HalSMDivide var;}\
    else if(arg.type==HalSMVariableType_HalSMEqual){HalSMEqual var;}\
    else if(arg.type==HalSMVariableType_HalSMNotEqual){HalSMNotEqual var;}\
    else if(arg.type==HalSMVariableType_HalSMMore){HalSMMore var;}\
    else if(arg.type==HalSMVariableType_HalSMLess){HalSMLess var;}\
    else if(arg.type==HalSMVariableType_HalSMBool){unsigned char var;}\
    else if(arg.type==HalSMVariableType_HalSMCLElement){HalSMCLElement var;}\
    else if(arg.type==HalSMVariableType_HalSMDict){Dict var;}\
    else if(arg.type==HalSMVariableType_HalSMSetVar){HalSMSetVar var;}\
    else if(arg.type==HalSMVariableType_HalSMReturn){HalSMReturn var;}\
    else if(arg.type==HalSMVariableType_HalSMFunctionArray){HalSMFunctionArray var;}\
    else if(arg.type==HalSMVariableType_unsigned_int){unsigned int var;}\
    __typeof__(var) out=*(__typeof__(var)*)arg.value;\
    out;\
})

#define HalSMVariable_FromValue(hsmmml,arg) ({\
    __typeof__(arg)* var=(__typeof__(arg)*)hsmmml->malloc(sizeof(__typeof__(arg)));\
    *var=arg;\
    HalSMVariable_init(hsmmml,var,typevar(var));\
})

#define HalSMVariable_FromValueWithType(hsmmml,arg,type) ({\
    type* var=(type*)hsmmml->malloc(sizeof(type));\
    *var=arg;\
    HalSMVariable_init(hsmmml,var,typevar(var));\
})

#define HalSMVariable_copy(hsmmml,arg) ({\
    __auto_type out=HalSMVariable_GetValue(arg);\
    __typeof__(out)* var=hsmmml->malloc(sizeof(__typeof__(out)));\
    *var=out;\
    HalSMVariable_init(hsmmml,var,arg.type);\
})

#define DictForEach(keyOutDictForEach,valueOutDictForEach,dict) \
    HalSMVariable* keyOutDictForEach=dict->elements[0]->key;HalSMVariable* valueOutDictForEach=dict->elements[0]->value;\
    for (unsigned int indexDictForEach=0;indexDictForEach<dict->size;indexDictForEach++,keyOutDictForEach=dict->elements[indexDictForEach]->key,valueOutDictForEach=dict->elements[indexDictForEach]->value)

#define HalSMArrayForEach(elementHalSMArrayForEach,array) \
    HalSMVariable* elementHalSMArrayForEach=array->arr[0];\
    for (unsigned int indexHalSMArrayForEach=0;indexHalSMArrayForEach<array->size;indexHalSMArrayForEach++,elementHalSMArrayForEach=array->arr[indexHalSMArrayForEach])

#define MathMin(a,b) (a<b?a:b)
#define MathMax(a,b) (a>b?a:b)
#define MathAbs(a) (a<0?-a:a)

#define MathCeilPos(a) (a-(int)a>0?(int)(a+1):(int)a)
#define MathCeilNeg(a) (a-(int)a<0?(int)(a-1):(int)a)
#define MathCeil(a) (a>0?MathCeilPos(a):MathCeilNeg(a))

#endif