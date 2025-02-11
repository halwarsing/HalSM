#include <HalSM.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <Windows.h>
#include <locale.h>

HalSMNull hnulld;
HalSMVariable nulld={&hnulld,HalSMVariableType_HalSMNull};

//HALSM

//HalSM Memory Library

void* HML_malloc(unsigned long long int size) {
    return malloc(size);
}

void* HML_calloc(unsigned long long int n,unsigned long long int size) {
    return calloc(n,size);
}

void* HML_realloc(void* p,unsigned long long int nsize) {
    return realloc(p,nsize);
}

void HML_free(void* p) {free(p);}

//HalSM Memory Library

//HalSM String Library

HalStringFormatChar* HSL_Decimal2Str(long long d)
{
    char* out=calloc(21,sizeof(char));
    sprintf(out,"%lld",d);
    return U8H(out);
}

HalStringFormatChar* HSL_Decimal2HexStr(long long d)
{
    char* out=calloc(21,sizeof(char));
    sprintf(out,"%x",d);
    return U8H(out);
}

HalStringFormatChar* HSL_Double2Str(double d)
{
    char* out=calloc(256,sizeof(char));
    sprintf(out,"%d",d);
    return U8H(out);
}

long long HSL_ParseDecimal(HalStringFormatChar* str)
{
    return strtoll(HSF2ASCII(str),NULL,10);
}

double HSL_ParseDouble(HalStringFormatChar* str)
{
    return strtod(HSF2ASCII(str),NULL);
}

//HalSM String Library

//HalSM System Library

void HSYSL_exit(int code){exit(code);}

//HalSM System Library

void HSM_print(HalStringFormatChar* str){printf("%s\n",HU8(str));}

void HSM_printError(HalStringFormatChar* str){printf("%s\n",HU8(str));}

HalStringFormatChar* HSM_input(HalStringFormatChar* str){printf("%s",HU8(str));char* out=calloc(100,sizeof(char));fgets(out,100,stdin);return U8H(out);}

HalSMVariable* HSM_getVariable(HalStringFormatChar* name){return &nulld;}

//HalSM File System Library

void* HFSL_openFile(HalStringFormatChar* path,HalStringFormatChar* mode){return fopen(HU8(path),HU8(mode));}

void HFSL_closeFile(void* p){fclose(p);}

unsigned long long int HFSL_read(void* out,unsigned long long int size,unsigned long long int count,void* p){return fread(out,size,count,p);}

unsigned long long int HFSL_write(void* in,unsigned long long int size,unsigned long long int count,void* p) {
    return fwrite(in,size,count,p);
}

void HFSL_puts(void* p,HalStringFormatChar* str){fputs((char*)str,p);}

void HFSL_seek(void* p,unsigned long long int offset,unsigned long long int origin){fseek(p,offset,origin);}

unsigned long long int HFSL_tell(void* p){return ftell(p);}

void HFSL_rewind(void* p){rewind(p);}

void HFSL_flush(void* p){fflush(p);}

int HFSL_eof(void* p){return feof(p);}

unsigned char HFSL_isExistsDir(HalStringFormatChar* path){
    struct stat sb;
    if (stat(HU8(path), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 1;
    }
    return 0;
}

unsigned char HFSL_isExistsFile(HalStringFormatChar* path){
    struct stat sb;
    if (stat(HU8(path), &sb) == 0 && S_ISDIR(sb.st_mode)==0) {
        return 1;
    }
    return 0;
}

HalSMArray* HFSL_getListFiles(HalSMCompiler* hsmc,HalStringFormatChar* path){return HalSMArray_init(hsmc->memorymanagmentlibrary);}

HalSMVariable* HSMLoadModule(HalSMCompiler* hsmc,HalStringFormatChar* name){return &nulld;}

int main(int argc,char* argv[])
{
    HalStringFormat_init((void*(*)(unsigned long long int))malloc);
    HalSMStringLibrary hsl={HSL_Decimal2Str,HSL_Decimal2HexStr,HSL_Double2Str,HSL_ParseDecimal,HSL_ParseDouble};
    HalSMMemoryManagmentLibrary hmml={HML_malloc,HML_calloc,HML_realloc,HML_free};
    HalSMSystemLibrary hsysl={HSYSL_exit};
    HalSMArray* modules=HalSMArray_init(&hmml);
    HalSMCModule* TestModule=HalSMCModule_init(&hmml,U8H("TestModule"));
    HalSMFileSystemLibrary hfsl={HFSL_isExistsDir,HFSL_isExistsFile,HFSL_getListFiles,HFSL_openFile,HFSL_closeFile,HFSL_read,HFSL_write,HFSL_puts,HFSL_seek,HFSL_tell,HFSL_rewind,HFSL_flush,HFSL_eof};

    HalSMCompiler* hsmc=HalSMCompiler_init(U8H(""),HSM_print,HSM_printError,HSM_input,HSM_getVariable,&hsl,&hmml,&hsysl,&hfsl,HSMLoadModule);
    PutDictElementToDict(hsmc,hsmc->sys_modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,TestModule->name),HalSMVariable_init(&hmml,TestModule,HalSMVariableType_HalSMCModule)));

    SetConsoleOutputCP(CP_UTF8);
    
    char temptesthsm[10000];
    FILE* ftesthsm=fopen("test.hsm","rb");
    fread(temptesthsm,1000,10,ftesthsm);
    fclose(ftesthsm);
    printf("D: %s\n",HU8(HalSMVariable_to_str(hsmc,HalSMCompiler_compile(hsmc,U8H(temptesthsm),0,&nulld),1)));


    return 0;
}