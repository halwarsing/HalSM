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

/*typedef struct TestVarTypeStruct {
    char* test;
} TestVarTypeStruct;

char* TestVarType_toString(HalSMVariable* var) {
    return ((TestVarTypeStruct*)((HalSMVariableCustom*)var->value)->value)->test;
}*/

int main(int argc,char* argv[])
{
    HalStringFormat_init((void*(*)(unsigned long long int))malloc);
    HalSMStringLibrary hsl={HSL_Decimal2Str,HSL_Decimal2HexStr,HSL_Double2Str,HSL_ParseDecimal,HSL_ParseDouble};
    HalSMMemoryManagmentLibrary hmml={HML_malloc,HML_calloc,HML_realloc,HML_free};
    HalSMSystemLibrary hsysl={HSYSL_exit};
    HalSMArray* modules=HalSMArray_init(&hmml);
    HalSMCModule* TestModule=HalSMCModule_init(&hmml,U8H("TestModule"));
    HalSMFileSystemLibrary hfsl={HFSL_isExistsDir,HFSL_isExistsFile,HFSL_getListFiles,HFSL_openFile,HFSL_closeFile,HFSL_read,HFSL_write,HFSL_puts,HFSL_seek,HFSL_tell,HFSL_rewind,HFSL_flush,HFSL_eof};



    /*HalSMVariableTypeCustom testvartype;
    testvartype.name="TestVar";
    testvartype.toString=TestVarType_toString;

    TestVarTypeStruct testvarvalue={"hello halwarsing!!!"};
    
    HalSMVariableCustom testvarcustom;
    testvarcustom.value=&testvarvalue;
    testvarcustom.type=&testvartype;

    PutDictElementToDict(&hmml,TestModule->vrs,DictElementInit(&hmml,HalSMVariable_init_str(&hmml,"testvar"),HalSMVariable_init(&hmml,&testvarcustom,HalSMVariableType_custom)));*/
    
    HalSMCompiler* hsmc=HalSMCompiler_init(U8H(""),HSM_print,HSM_printError,HSM_input,HSM_getVariable,&hsl,&hmml,&hsysl,&hfsl,HSMLoadModule);
    PutDictElementToDict(hsmc,hsmc->sys_modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,TestModule->name),HalSMVariable_init(&hmml,TestModule,HalSMVariableType_HalSMCModule)));

    /*HalSMArray* hsmcc=HalSMCompiler_compile(hsmc,"def test(a):\n g=4\n d=13*g*a\n print(d+g)",0);
    HalSMLocalFunction* lf=(HalSMLocalFunction*)((Dict*)hsmcc->arr[1]->value)->elements[0]->value->value;
    HalSMArray* tempargs=HalSMArray_init(&hmml);
    HalSMArray_add(&hmml,tempargs,HalSMVariable_FromValue(&hmml,5));
    for (unsigned int i=0;i<10;i++) {
        //printf("%d: ",i);
        HalSMLocalFunction_run(lf,hsmc,tempargs);
    }*/

    //HalSM_compile_without_path(hsm,"def test(a,b):\n for(i=a;i>0;i-=1):\n  for(j=0;j<b;j+=1):\n   print(j)\ntest(5,6)");
    //HalSM_compile_without_path(hsm,"i=6\nif(i<5):\n if(i<3):\n  if(i==2):\n   print(i)\n elif(i==4):\n  print('ofig')\n else():\n  print('et')\nelif(i==5):\n print('yes')\nelse():\n print('good')\nprint(\"end\")");
    //HalSM_compile_without_path(hsm,"for(i=0;i<5;i+=1):\n if(i<3):\n  for(j=0;j<2;j++):\n   print(i,j)\n else():\n  for(j=2;j>0;j--):\n   print(i,j)");
    //HalSM_compile_without_path(hsm,"for(i=0;i<5;i+=1):\n if(i<3):\n  for(j=0;j<2;j++):\n   print(i,j)");
    //HalSM_compile_without_path(hsm,"d=[2,2,3]\nprint(++d[0])\nprint(d)");
    /*HalSM_compile_without_path(hsm,"test=2\n"
"print(++((test)+1))\n"
"print(test)\n");*/
    /*HalSM_compile_without_path(hsm,"for(d=0;d<10;d+=1):\n"
    " if(d>2):\n"
    "  print((d++)*10)");*/
    //HalSM_compile_without_path(hsm,"d=5\ndef test(a):\n return a*3+d\nprint(test(d++*3))\nprint(d)");
    //HalSM_compile_without_path(hsm,"a=1\ndo():\n a+=1\n print(a)\nwhile(a<10)\nprint(\"END\")");
    //HalSM_compile_without_path(hsm,"class TestClass:\n def test():\n  a=1\n  do():\n   a+=1\n   print(a)\n  while(a<10)\ntestc=TestClass()\ntestc.test()");
    /*HalSM_compile_without_path(hsm,"class TestClass:\n"
    " def test(self):\n"
    "  self.d=2\n"
    "  print(self.d)\n"
    "  print(self)\n"
    "  return self\n"
    "t=TestClass()\n"
    "temp=t.test()\n"
    "temp.d++\n"
    "print(temp.d)");*/
    //HalSM_compile_without_path(hsm,"d=[2,3,4]\nd[1]=5\nprint(d)");
    /*HalSM_compile_without_path(hsm,"class TestClass:\n"
                                   " def __init__(self):\n"
                                   "  self.d=[3,2.5,\"halwarsing!!!\"]\n"
                                   "  self.testik=self.test\n"
                                   " def test(self):\n"
                                   "  print(self.d)\n"
                                   "t=TestClass()\n"
                                   "t.testik()");*/
    /*HalSM_compile_without_path(hsm,"class TestClass:\n"
                                   " def __init__(self):\n"
                                   "  self.d=6\n"
                                   " def test(self):\n"
                                   "  return self.d\n"
                                   "print(TestClass().test())");*/
    /*HalSM_compile_without_path(hsm,"class TestClass:\n"
                                   " def __init__(self):\n"
                                   "  print(self.test()())\n"
                                   " def test(self):\n"
                                   "  return self.newtest\n"
                                   " def newtest():\n"
                                   "  for(i=0;i<5;i++):\n"
                                   "   print(i)\n"
                                   "  return \"d\"\n"
                                   "print(TestClass().test()())");*/

    /*HalSM_compile_without_path(hsm,"def test(a=[3,4,5],b=[\"hi\",4.0/3])):\n"
                                   " print(++a[2],a,b)\n"
                                   "test()\n");*/

    /*HalSM_compile_without_path(hsm,"class TestClass:\n"
                                   " d=\"hello halwarsing!!!\"\n"
                                   " def test(self,a):\n"
                                   "  print((TestClass.d+\":\"+a)*10)\n"
                                   "t=TestClass()\n"
                                   "for(i=0;i<10;i++):\n"
                                   " t.test(i)");*/

    /*HalSM_compile_without_path(hsm,"d={\n"
"   [1,2,3]: [\n"
"       \"hi\",\n"
"       \"halwarsing\"\n"
"   ]\n"
"}\n"
"print(d[[1,2,3]][0]+d[[1,2,3]][1])");*/
    /*HalSM_compile_without_path(hsm,"def test(a=1,b):\n"
                                   " print(a,b)\n"
                                   "test(b=4,3==3||false)");*/
    /*HalSM_compile_without_path(hsm,"a=3\n"
                                   "print(1046%3)");*/
    //                               "print((3<<10000)*(3<<10000))");
    /*HalSM_compile_without_path(hsm,"def test(a):\n"
                                   " print(a)\n"
                                   " return a\n"
                                   "print(test(false)||test(true))");*/

    /*HalSM_compile_without_path(hsm,"a=4\n"
                                   "--a\n"
                                   "print(a--)\n"
                                   "print(a)");*/

    /*HalSM_compile_without_path(hsm,"from TestModuleHSM import *\n"
                                   "print(testfunc()+\"d\")");*/
    
    //HalSM_compile_without_path(hsm,U8H("print(++(\"test$hi\".replace('$','molodec').substring(0,4).rindex(\"t\")))"));
    //HalSM_compile_without_path(hsm,U8H("print('test$hi'.split('$').slice(-1,2))"));
    //HalSM_compile_without_path(hsm,U8H("print('da')"));


    SetConsoleOutputCP(CP_UTF8);
    //setvbuf(stdout,NULL,_IOFBF,1000);
    //HalSM_compile_without_path(hsm,U8H("print({'heil':'halwarsing!!!'}.appendDict({5:4,4:5}).pop(0))"));
    //printf("%s\n",HU8(HalSMInteger_toString(hsm->memorymanagmentlibrary,HalSMDouble_gcd(hsm->memorymanagmentlibrary,HalSMInteger_parse(hsm->memorymanagmentlibrary,U8H("33")),HalSMInteger_parse(hsm->memorymanagmentlibrary,U8H("17"))),0)));
    char temptesthsm[10000];
    FILE* ftesthsm=fopen("test.hsm","rb");
    fread(temptesthsm,1000,10,ftesthsm);
    fclose(ftesthsm);

    //printf("B: %s\n",HU8(HalSMDouble_toString(hsmc->memorymanagmentlibrary,HalSMDouble_FromFloat(hsmc->memorymanagmentlibrary,4.2f),0)));
    //printf("Hi: %s\n",HU8(HalSMDouble_toString(hsmc->memorymanagmentlibrary,HalSMDouble_FromFloat(hsmc->memorymanagmentlibrary,4.56342342f),0)));
    printf("D: %s\n",HU8(HalSMVariable_to_str(hsmc,HalSMCompiler_compile(hsmc,U8H(temptesthsm),0,&nulld),1)));

    //setlocale(LC_ALL, "UTF8");
    //printf("ПРИВЕТ!!!\n");

    /*HalSMInteger* hi0=HalSMInteger_FromUnsignedInteger(&hmml,500);
    HalSMInteger* hi1=HalSMInteger_FromUnsignedInteger(&hmml,251);
    //HalSMInteger_AddSelf(&hmml,hi1,HalSMInteger_FromUnsignedInteger(&hmml,1));
    //printf("%s\n",HalSMInteger_toString(&hmml,HalSMInteger_Div(&hmml,hi0,hi1),0));
    //printf("%s\n",HalSMInteger_toString(&hmml,HalSMInteger_Div(&hmml,hi0,hi1),0));
    printf("DIV=%s\n",HalSMInteger_toString(&hmml,HalSMInteger_Div(&hmml,hi0,hi1,NULL),0));
    //printf("POW=%s\n",HalSMInteger_toString(&hmml,HalSMInteger_Pow(&hmml,hi0,hi1),0));

    HalSMDouble* do0=HalSMDouble_FromDouble(&hmml,1.5);
    HalSMDouble do1={0,(HalSMInteger){0,malloc(1),1},(HalSMInteger){0,malloc(1),1}};
    do1.a.value[0]=1;
    do1.b.value[0]=128;
    //printf("DBL=%s\n",HalSMDouble_toString(&hmml,&do1,0));
    HalSMDouble_DivSelf(&hmml,do0,&do1);
    printf("HIL=%s\n",HalSMDouble_toString(&hmml,do0,0));*/

    return 0;
}