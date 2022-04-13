#include "HalSM.h"

HalSMNull null;
HalSMPlus plus;
HalSMMinus minus;
HalSMMult mult;
HalSMDivide divide;
HalSMEqual equal;
HalSMNotEqual notequal;
HalSMMore more;
HalSMLess less;
HalSMArray* arrInt;

HalSMNull* HalSMNull_init(HalSMCompiler* hsmc) {
    HalSMNull* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMNull));
    return out;
}

HalSMError* HalSMError_init(HalSMCompiler* hsmc,unsigned int line,char* error){
    HalSMError* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMError));
    out->line=line;
    out->error=error;
    return out;
}

HalSM* HalSM_init(HalSMArray* externModules,void(*print)(char*),void(*printErrorf)(char*),char*(*inputf)(char*),char*(*readFilef)(char*),char* pathModules,HalSMLoadSharedLibrary* loadsharedlibrary,HalSMStringLibrary* stringlibrary,HalSMMemoryManagmentLibrary* memorymanagmentlibrary,HalSMSystemLibrary* systemlibrary) {
    HalSM* hsm=memorymanagmentlibrary->malloc(sizeof(HalSM));
    arrInt=HalSMArray_init(memorymanagmentlibrary);HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'0',char));HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'1',char));
    HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'2',char));HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'3',char));HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'4',char));
    HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'5',char));HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'6',char));HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'7',char));
    HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'8',char));HalSMArray_add(memorymanagmentlibrary,arrInt,HalSMVariable_FromValueWithType(memorymanagmentlibrary,'9',char));
    string_cpy(hsm->version,"0.0.6 (Pre-Alpha)");
    hsm->externModules=externModules;
    hsm->print=print;
    hsm->printErrorf=printErrorf;
    hsm->inputf=inputf;
    hsm->readFilef=readFilef;
    hsm->pathModules=pathModules;
    hsm->loadsharedlibrary=loadsharedlibrary;
    hsm->stringlibrary=stringlibrary;
    hsm->memorymanagmentlibrary=memorymanagmentlibrary;
    hsm->systemlibrary=systemlibrary;
    return hsm;
}

void HalSM_compile(HalSM* hsm,char* code,char* path)
{
    HalSMCompiler* hsmc=HalSMCompiler_init(
        path,hsm->externModules,hsm->print,hsm->printErrorf,hsm->inputf,hsm->readFilef,hsm->pathModules,hsm->loadsharedlibrary,hsm->stringlibrary,
        hsm->memorymanagmentlibrary,hsm->systemlibrary);
    HalSMCompiler_compile(hsmc,code,0);
}

void HalSM_compile_without_path(HalSM* hsm,char* code)
{
    HalSM_compile(hsm,code,"");
}

HalSMVariable* HalSMCompiler_readFile(HalSMCompiler* hsmc,HalSMArray* args) {
    return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->readFilef(*(char**)args->arr[0]->value));
}

HalSMVariable* HalSMCompiler_input(HalSMCompiler* hsmc,HalSMArray* args) {
    char* text=*(char**)args->arr[0]->value;
    return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->inputf(text));
}

HalSMVariable* HalSMCompiler_reversed(HalSMCompiler* hsmc,HalSMArray* args) {
    HalSMVariable* arr=args->arr[0];
    if (arr->type==HalSMVariableType_HalSMArray) {
        HalSMArray* out=HalSMArray_reverse(hsmc->memorymanagmentlibrary,(HalSMArray*)arr->value);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
    } else if (arr->type==HalSMVariableType_str) {
        char* arrv=*(char**)arr->value;
        unsigned int arrl=string_len(arrv);
        if (arrl==0) {
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"");
        }
        char* out=hsmc->memorymanagmentlibrary->malloc(arrl+1);
        int b=0;
        for (unsigned int i=arrl;i--;) {
            out[b]=arrv[i];
            if (i==0) {break;}
            b++;
        }
        out[arrl]='\0';
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
    }
    HalSMCompiler_ThrowError(hsmc,hsmc->line,"This Variable not list or string");
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMVariable* HalSMCompiler_range(HalSMCompiler* hsmc,HalSMArray* args) {
    if (args->size==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->line,"Range cannot be without numeric args"),HalSMVariableType_HalSMError);}
    double start,end,step;
    if (args->size==1) {
        start=0;
        if (args->arr[0]->type==HalSMVariableType_int) {end=*(int*)args->arr[0]->value;}
        else {end=*(double*)args->arr[0]->value;}
        step=1;
    } else if (args->size==2) {
        if (args->arr[0]->type==HalSMVariableType_int) {start=*(int*)args->arr[0]->value;}
        else {start=*(double*)args->arr[0]->value;}
        if (args->arr[1]->type==HalSMVariableType_int) {end=*(int*)args->arr[1]->value;}
        else {end=*(double*)args->arr[1]->value;}
        step=1;
    } else if (args->size==3) {
        if (args->arr[0]->type==HalSMVariableType_int) {start=*(int*)args->arr[0]->value;}
        else {start=*(double*)args->arr[0]->value;}
        if (args->arr[1]->type==HalSMVariableType_int) {end=*(int*)args->arr[1]->value;}
        else {end=*(double*)args->arr[1]->value;}
        if (args->arr[2]->type==HalSMVariableType_int) {step=*(int*)args->arr[2]->value;}
        else {step=*(double*)args->arr[2]->value;}
    }
    HalSMArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMArray));
    out->size=MathCeilPos((MathAbs((end-start))/MathAbs(step)));
    out->arr=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable*)*out->size);
    double* p=hsmc->memorymanagmentlibrary->malloc(sizeof(double)*out->size);

    HalSMVariable* t=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable)*out->size);
    for (unsigned int i=0;i<out->size;i++) {
        p[i]=start+i*step;
        t[i].type=HalSMVariableType_double;
        t[i].value=&p[i];
        out->arr[i]=&t[i];
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
}

HalSMArray* HalSMCompiler_get_print_text(HalSMCompiler* hsmc,HalSMArray* args) {
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    char* c;
    HalSMVariable* a;
    for (unsigned int i=0;i<args->size;i++) {
        a=args->arr[i];
        if (a->type==HalSMVariableType_int) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->stringlibrary->Decimal2Str(*(int*)a->value)));
        } else if (a->type==HalSMVariableType_unsigned_int) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->stringlibrary->Decimal2Str(*(unsigned int*)a->value)));
        } else if (a->type==HalSMVariableType_double) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->stringlibrary->Double2Str(*(double*)a->value)));
        } else if (a->type==HalSMVariableType_str) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,a);
        } else if (a->type==HalSMVariableType_HalSMNull) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"Null"));
        } else if (a->type==HalSMVariableType_char) {
            c=hsmc->memorymanagmentlibrary->malloc(2);
            c[0]=*(char*)a->value;
            c[1]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,c,HalSMVariableType_str));
            hsmc->memorymanagmentlibrary->free(c);
        } else if (a->type==HalSMVariableType_HalSMArray) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_to_print(hsmc,(HalSMArray*)a->value)));
        } else if (a->type==HalSMVariableType_HalSMRunClassC) {
            c=hsmc->memorymanagmentlibrary->malloc(21+string_len(((HalSMRunClassC*)a->value)->name));
            string_cpy(c,"<Running Class C (");
            string_cat(c,((HalSMRunClassC*)a->value)->name);
            string_cat(c,")>");
            c[string_len(((HalSMRunClassC*)a->value)->name)+20]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,c));
            hsmc->memorymanagmentlibrary->free(c);
        } else if (a->type==HalSMVariableType_HalSMLocalFunction) {
            char* name=((HalSMLocalFunction*)a->value)->name;
            c=hsmc->memorymanagmentlibrary->malloc(19+string_len(name));
            string_cpy(c,"<LocalFunction (");
            string_cat(c,name);
            string_cat(c,")>");
            c[string_len(name)+18]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,c));
            hsmc->memorymanagmentlibrary->free(c);
        } else if (a->type==HalSMVariableType_HalSMFunctionC) {
            char* name=ConcatenateStrings(hsmc->memorymanagmentlibrary,"0x",hsmc->stringlibrary->Decimal2HexStr((int)((HalSMFunctionC*)a->value)->func));
            c=hsmc->memorymanagmentlibrary->malloc(35);
            string_cpy(c,"<Function C at (");
            string_cat(c,name);
            string_cat(c,")>");
            c[string_len(name)+18]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,c));
            hsmc->memorymanagmentlibrary->free(c);
        } else if (a->type==HalSMVariableType_HalSMCModule) {
            char* name=((HalSMCModule*)a->value)->name;
            c=hsmc->memorymanagmentlibrary->malloc(14+string_len(name));
            string_cpy(c,"<Module C (");
            string_cat(c,name);
            string_cat(c,")>");
            c[13+string_len(name)]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,c));
            hsmc->memorymanagmentlibrary->free(c);
        } else {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMVariable_to_str(hsmc->stringlibrary,a)));
        }
    }
    return out;
}

HalSMVariable* HalSMCompiler_print(HalSMCompiler* hsmc,HalSMArray* args) {
    hsmc->print(HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMCompiler_get_print_text(hsmc,args)," "));
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMVariable* HalSMCompiler_exit(HalSMCompiler* hsmc,HalSMArray* args) {
    hsmc->systemlibrary->exit(0);
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}



HalSMCompiler* HalSMCompiler_init(char* path,HalSMArray* externModules,void(*print)(char*),void(*printErrorf)(char*),char*(*inputf)(char*),char*(*readFilef)(char*),char* pathModules,HalSMLoadSharedLibrary* loadsharedlibrary,HalSMStringLibrary* stringlibrary,HalSMMemoryManagmentLibrary* memorymanagmentlibrary,HalSMSystemLibrary* systemlibrary)
{
    HalSMCompiler* hsmc=memorymanagmentlibrary->malloc(sizeof(HalSMCompiler));
    hsmc->print=print;
    hsmc->printErrorf=printErrorf;
    hsmc->inputf=inputf;
    hsmc->readFilef=readFilef;
    hsmc->pathModules=pathModules;
    hsmc->path=path;
    hsmc->externModules=externModules;
    hsmc->loadsharedlibrary=loadsharedlibrary;
    hsmc->stringlibrary=stringlibrary;
    hsmc->memorymanagmentlibrary=memorymanagmentlibrary;
    hsmc->systemlibrary=systemlibrary;
    hsmc->functions=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"readFile"),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc->memorymanagmentlibrary,&HalSMCompiler_readFile),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"input"),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc->memorymanagmentlibrary,&HalSMCompiler_input),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"reversed"),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc->memorymanagmentlibrary,&HalSMCompiler_reversed),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"range"),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc->memorymanagmentlibrary,&HalSMCompiler_range),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"print"),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc->memorymanagmentlibrary,&HalSMCompiler_print),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"exit"),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc->memorymanagmentlibrary,&HalSMCompiler_exit),HalSMVariableType_HalSMFunctionC))
    },6);

    DictElement* sys_modules_arr[0]={};
    hsmc->sys_modules=DictInitWithElements(hsmc->memorymanagmentlibrary,sys_modules_arr,0);
    HalSMCModule* cm;
    for (unsigned int i=0;i<externModules->size;i++)
    {
        cm=(HalSMCModule*)externModules->arr[i]->value;
        PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->sys_modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,cm->name),externModules->arr[i]));
    }
    hsmc->calcVars=HalSMCalculateVars_init();
    hsmc->line=1;
    hsmc->sys_variables=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"copyright"),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"Copyright (c) 2022 Halwarsing.net"))
    },1);
    hsmc->variables=DictCopy(hsmc->memorymanagmentlibrary,hsmc->sys_variables);
    hsmc->modules=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->localFunctions=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->classes=DictInit(hsmc->memorymanagmentlibrary);
    return hsmc;
}

HalSMArray* HalSMCompiler_getLines(HalSMCompiler* hsmc,char* text)
{
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    char* o=hsmc->memorymanagmentlibrary->malloc(0);
    unsigned int s=0;
    char isS='n';
    char i;
    unsigned int lt=string_len(text);
    for (unsigned int d=0;d<lt;d++) {
        i=text[d];
        if (isS!='n') {
            if (i==isS) {
                isS='n';
            }
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(char));
            o[s-1]=i;
        } else if (i=='"' || i=='\'') {
            isS=i;
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(char));
            o[s-1]=i;
        } else if (i=='\n') {
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(char));
            o[s-1]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,o));
            s=0;
            o=hsmc->memorymanagmentlibrary->malloc(0);
        } else if (i==';') {
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(char));
            o[s-1]='\0';
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,o));
            s=0;
            o=hsmc->memorymanagmentlibrary->malloc(0);
        } else {
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(char));
            o[s-1]=i;
        }
    }
    if (s!=0) {
        s++;
        o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(char));
        o[s-1]='\0';
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,o));
    }
    hsmc->memorymanagmentlibrary->free(o);
    return out;
}

void HalSMCompiler_ThrowError(HalSMCompiler* hsmc,int line,char* error)
{
    char* sl=hsmc->stringlibrary->Decimal2Str(line);
    char* out=hsmc->memorymanagmentlibrary->malloc(17+string_len(sl)+string_len(error));
    string_cpy(out,"Error at line ");
    string_cat(out,sl);
    string_cat(out,": ");
    string_cat(out,error);
    out[16+string_len(sl)+string_len(error)]='\0';
    hsmc->printErrorf(out);
}

HalSMVariable* HalSMCompiler_isGet(HalSMCompiler* hsmc,char* l,unsigned char ret)
{
    if (!(StringIndexOf(hsmc->memorymanagmentlibrary,l,".")!=-1||(StringIndexOf(hsmc->memorymanagmentlibrary,l,"[")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,l,"]")))) return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* out=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    unsigned int isInd=0;
    HalSMArray* o=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMVariable* module=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    unsigned int indF=0;
    char isCovichki='n';
    unsigned int index=0;
    unsigned int lindex=string_len(l)-1;
    char i;
    HalSMVariable* obuff;
    char* obuffs;
    HalSMArray* argsbuff;
    unsigned ll=string_len(l);
    unsigned int indexa;
    HalSMVariable* cobuff;

    for (unsigned int ii=0;ii<ll;ii++) {
        i=l[ii];
        if (i=='"'||i=='\'') {
            if (i==isCovichki) {
                isCovichki='n';
            } else if (isCovichki=='n') {
                isCovichki=i;
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
        } else if (isCovichki!='n') {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
        } else if (isInd==1&&indF==0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
            if (i==']') {
                isInd=0;
                obuffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
                if (StringIndexOf(hsmc->memorymanagmentlibrary,obuffs,",")!=-1) {
                    o=HalSMArray_split_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,obuffs,0,string_len(obuffs)-1),",");
                    for (unsigned int indexspl=0;indexspl<o->size;indexspl++) {
                        indexa=hsmc->stringlibrary->ParseDecimal(*(char**)o->arr[indexspl]->value);
                        if (out->type==HalSMVariableType_HalSMArray) {
                            HalSMArray* rcls=(HalSMArray*)out->value;
                            if (indexa>=rcls->size) {
                                HalSMCompiler_ThrowError(hsmc,hsmc->line,"Index is equal to or more than size of array");
                            } else {
                                out=rcls->arr[indexa];
                            }
                        } else if (out->type==HalSMVariableType_HalSMRunClassC) {
                            HalSMRunClassC* rcls=(HalSMRunClassC*)out->value;
                            if (DictElementIndexByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))==-1) {
                                HalSMCompiler_ThrowError(hsmc,hsmc->line,"This class does not contain function __getitem__");
                            }
                            argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClassC));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,indexa,int));
                            out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))->value->value)(hsmc,argsbuff);
                            if (out->type==HalSMVariableType_HalSMError){
                                HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)out->value)->error);
                            }
                        } else if (out->type==HalSMVariableType_HalSMRunClass) {
                            HalSMRunClass* rcls=(HalSMRunClass*)out->value;
                            if (DictElementIndexByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))==-1) {
                                HalSMCompiler_ThrowError(hsmc,hsmc->line,"This class does not contain function __getitem__");
                            }
                            argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClass));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,indexa,int));
                            out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))->value->value)(hsmc,argsbuff);
                            if (out->type==HalSMVariableType_HalSMError){
                                HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)out->value)->error);
                            }
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This object is not array");
                        }
                    }
                } else {
                    indexa=hsmc->stringlibrary->ParseDecimal(SubString(hsmc->memorymanagmentlibrary,obuffs,0,string_len(obuffs)-1));
                    if (out->type==HalSMVariableType_HalSMArray) {
                        HalSMArray* rcls=(HalSMArray*)out->value;
                        if (indexa>=rcls->size) {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"Index is equal to or more than size of array");
                        } else {
                            out=rcls->arr[indexa];
                        }
                    } else if (out->type==HalSMVariableType_HalSMRunClassC) {
                        HalSMRunClassC* rcls=(HalSMRunClassC*)out->value;
                        if (DictElementIndexByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))==-1) {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This class does not contain function __getitem__");
                        }
                        argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClassC));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,indexa,int));
                        out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))->value->value)(hsmc,argsbuff);
                        if (out->type==HalSMVariableType_HalSMError){
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)out->value)->error);
                        }
                    } else if (out->type==HalSMVariableType_HalSMRunClass) {
                        HalSMRunClass* rcls=(HalSMRunClass*)out->value;
                        if (DictElementIndexByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))==-1) {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This class does not contain function __getitem__");
                        }
                        argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClass));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,indexa,int));
                        out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__getitem__"))->value->value)(hsmc,argsbuff);
                        if (out->type==HalSMVariableType_HalSMError){
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)out->value)->error);
                        }
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This object is not array");
                    }
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
        } else if (i=='['&&indF==0) {
            isInd=1;
            if (o->size!=0) {
                obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                if (DictElementIndexByKey(hsmc->variables,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc->variables,obuff)->value;
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                }
            }
        } else if (i=='(') {
            if(indF==0) {
                obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                if (out->type==HalSMVariableType_HalSMModule) {
                    if (DictElementIndexByKey(((HalSMModule*)out->value)->lfuncs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(((HalSMModule*)out->value)->lfuncs,obuff)->value;
                    }
                } else if (out->type==HalSMVariableType_HalSMCModule) {
                    if (DictElementIndexByKey(((HalSMCModule*)out->value)->lfuncs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->lfuncs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->vrs,obuff)!=-1&&DictElementFindByKey(((HalSMCModule*)out->value)->vrs,obuff)->value->type==HalSMVariableType_HalSMFunctionC) {
                        module=out;
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->vrs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->classes,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->classes,obuff)->value;
                    }
                } else if (out->type==HalSMVariableType_HalSMRunClass) {
                    if (DictElementIndexByKey(((HalSMRunClass*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClass*)out->value)->funcs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMRunClass*)out->value)->vars,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClass*)out->value)->vars,obuff)->value;
                    }
                } else if (out->type==HalSMVariableType_HalSMClass) {
                    if (DictElementIndexByKey(((HalSMClass*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClass*)out->value)->funcs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMClass*)out->value)->vars,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClass*)out->value)->vars,obuff)->value;
                    }
                } else if (out->type==HalSMVariableType_HalSMRunClassC) {
                    if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,obuff)!=-1&&DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,obuff)->value->type==HalSMVariableType_HalSMFunctionC) {
                        module=out;
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,obuff)->value;
                    }
                } else if (out->type==HalSMVariableType_HalSMClassC) {
                    if (DictElementIndexByKey(((HalSMClassC*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClassC*)out->value)->funcs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMClassC*)out->value)->vrs,obuff)!=-1&&DictElementFindByKey(((HalSMClassC*)out->value)->vrs,obuff)->value->type==HalSMVariableType_HalSMFunctionC) {
                        out=DictElementFindByKey(((HalSMClassC*)out->value)->vrs,obuff)->value;
                    }
                } else if(DictElementIndexByKey(hsmc->functions,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc->functions,obuff)->value;
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,'(',char));
            }
            indF+=1;
        } else if (i==')') {
            if (out->type==HalSMVariableType_HalSMFunctionC&&indF==1) {
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),ret);
                if (module->type==HalSMVariableType_HalSMRunClassC) {HalSMArray_insert(hsmc->memorymanagmentlibrary,args,module,0);}
                if (index==lindex&&ret) {
                    HalSMFunctionArray* hfa=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
                    HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,ob,out);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,args,HalSMVariableType_HalSMArray));
                    hfa->args=ob;
                    hfa->type=HalSMFunctionArrayType_function;
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,hfa,HalSMVariableType_HalSMFunctionArray);
                } else {
                    out=HalSMFunctionC_run(hsmc,(HalSMFunctionC*)out->value,args);
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else if (out->type==HalSMVariableType_HalSMLocalFunction&&indF==1) {
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),ret);
                if (module->type==HalSMVariableType_HalSMRunClass) {HalSMArray_insert(hsmc->memorymanagmentlibrary,args,module,0);}
                if (index==lindex&&ret) {
                    HalSMFunctionArray* hfa=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
                    HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,ob,out);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,args,HalSMVariableType_HalSMArray));
                    hfa->args=ob;
                    hfa->type=HalSMFunctionArrayType_function;
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,hfa,HalSMVariableType_HalSMFunctionArray);
                } else {
                    out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,args);
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else if (out->type==HalSMVariableType_HalSMClassC&&indF==1) {
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),ret);
                if (index==lindex&&ret) {
                    HalSMFunctionArray* hfa=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
                    HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,ob,out);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,args,HalSMVariableType_HalSMArray));
                    hfa->args=ob;
                    hfa->type=HalSMFunctionArrayType_function;
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,hfa,HalSMVariableType_HalSMFunctionArray);
                } else {
                    out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClassC_run(hsmc,(HalSMClassC*)out->value,args),HalSMVariableType_HalSMRunClassC);
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,')',char));
            }
            indF-=1;
        } else if (i=='.'&&indF==0) {
            obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
            if (out->type==HalSMVariableType_HalSMNull) {
                if (DictElementIndexByKey(hsmc->variables,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc->variables,obuff)->value;
                } else if (DictElementIndexByKey(hsmc->classes,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc->classes,obuff)->value;
                } else if (DictElementIndexByKey(hsmc->modules,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc->modules,obuff)->value;
                } else if (HalSMIsInt(hsmc->memorymanagmentlibrary,*(char**)obuff->value)) {
                    out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDoubleGet_init(hsmc->memorymanagmentlibrary,*(char**)obuff->value),HalSMVariableType_HalSMDoubleGet);
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Module or Class Not Found");
                }
            } else if (out->type==HalSMVariableType_HalSMModule) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                    cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                    if (DictElementIndexByKey(((HalSMModule*)out->value)->lfuncs,cobuff)!=-1) {
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMModule*)out->value)->lfuncs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (DictElementIndexByKey(((HalSMModule*)out->value)->classes,cobuff)!=-1) {
                        out=HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)(DictElementFindByKey(((HalSMModule*)out->value)->classes,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0))); 
                    } else if (DictElementIndexByKey(((HalSMModule*)out->value)->vrs,cobuff)!=-1) {
                        out=DictElementFindByKey(((HalSMModule*)out->value)->vrs,cobuff)->value; 
                        if (out->type==HalSMVariableType_HalSMLocalFunction) {
                            out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                        } else if (out->type==HalSMVariableType_HalSMClassC) {
                            //In future
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"Is Not Function or Class");
                        }
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                } else {
                    if (DictElementIndexByKey(((HalSMModule*)out->value)->vrs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMModule*)out->value)->vrs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMModule*)out->value)->lfuncs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMModule*)out->value)->lfuncs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMModule*)out->value)->classes,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMModule*)out->value)->classes,obuff)->value;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                    }
                }
            } else if (out->type==HalSMVariableType_HalSMCModule) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                    cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                    if (DictElementIndexByKey(((HalSMCModule*)out->value)->lfuncs,cobuff)!=-1) {
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMCModule*)out->value)->lfuncs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->classes,cobuff)!=-1) {
                        out=HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)(DictElementFindByKey(((HalSMCModule*)out->value)->classes,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0))); 
                    } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->vrs,cobuff)!=-1) {
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->vrs,cobuff)->value;
                        if (out->type==HalSMVariableType_HalSMFunctionC) {
                            out=HalSMFunctionC_run(hsmc,(HalSMFunctionC*)out->value,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                        }
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                } else {
                    if (DictElementIndexByKey(((HalSMCModule*)out->value)->vrs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->vrs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->lfuncs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->lfuncs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->classes,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMCModule*)out->value)->classes,obuff)->value;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                    }
                }
            } else if (out->type==HalSMVariableType_HalSMClassC) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                    HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"("),0);
                    if (DictElementIndexByKey(((HalSMClassC*)out->value)->funcs,cobuff)!=-1) {
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMClassC*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (DictElementIndexByKey(((HalSMClassC*)out->value)->vrs,cobuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClassC*)out->value)->vrs,cobuff)->value;
                        if (out->type==HalSMVariableType_HalSMLocalFunction) {
                            out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                        }
                    }
                } else {
                    if (DictElementIndexByKey(((HalSMClassC*)out->value)->vrs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClassC*)out->value)->vrs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMClassC*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClassC*)out->value)->funcs,obuff)->value;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function Not Found");
                    }
                }
            } else if (out->type==HalSMVariableType_HalSMRunClassC) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                    HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"("),0);
                    if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)!=-1) {
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,cobuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,cobuff)->value;
                        if (out->type==HalSMVariableType_HalSMLocalFunction) {
                            out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                        }
                    }
                } else {
                    if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,obuff)->value;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function Not Found");
                    }
                }
            } else if (out->type==HalSMVariableType_HalSMRunClass) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                    HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"("),0);
                    if (DictElementIndexByKey(((HalSMRunClass*)out->value)->funcs,cobuff)!=-1) {
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (DictElementIndexByKey(((HalSMRunClass*)out->value)->vars,cobuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClass*)out->value)->vars,cobuff)->value;
                        if (out->type==HalSMVariableType_HalSMLocalFunction) {
                            out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                        }
                    }
                } else {
                    if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,obuff)->value;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function not found");
                    }
                }
            } else if (out->type==HalSMVariableType_HalSMClass) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                    HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"("),0);
                    if (DictElementIndexByKey(((HalSMClass*)out->value)->funcs,cobuff)!=-1) {
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMClass*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (DictElementIndexByKey(((HalSMClass*)out->value)->vars,cobuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClass*)out->value)->vars,cobuff)->value;
                        if (out->type==HalSMVariableType_HalSMLocalFunction) {
                            out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                        }
                    }
                } else {
                    if (DictElementIndexByKey(((HalSMClass*)out->value)->vars,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClass*)out->value)->vars,obuff)->value;
                    } else if (DictElementIndexByKey(((HalSMClass*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(((HalSMClass*)out->value)->funcs,obuff)->value;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function Not Found");
                    }
                }
            }
            o=HalSMArray_init(hsmc->memorymanagmentlibrary);
        } else {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
        }
        index++;
    }

    if (o->size!=0) {
        obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
        if (out->type==HalSMVariableType_HalSMModule) {
            if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                if (DictElementIndexByKey(((HalSMModule*)out->value)->lfuncs,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)(DictElementFindByKey(((HalSMModule*)out->value)->lfuncs,cobuff)->value->value),HalSMVariableType_HalSMLocalFunction));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMModule*)out->value,HalSMVariableType_HalSMModule));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMModule*)out->value)->lfuncs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                } else if (DictElementIndexByKey(((HalSMModule*)out->value)->classes,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMClass*)DictElementFindByKey(((HalSMModule*)out->value)->classes,cobuff)->value->value,HalSMVariableType_HalSMClass));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMModule*)out->value,HalSMVariableType_HalSMModule));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)DictElementFindByKey(((HalSMModule*)out->value)->classes,cobuff)->value->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0)),HalSMVariableType_HalSMArray); 
                } else if (DictElementIndexByKey(((HalSMModule*)out->value)->vrs,cobuff)!=-1) {
                    out=DictElementFindByKey(((HalSMModule*)out->value)->vrs,cobuff)->value; 
                    if (out->type==HalSMVariableType_HalSMLocalFunction) {
                        if (ret) {
                            HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)out->value,HalSMVariableType_HalSMLocalFunction));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMModule*)out->value,HalSMVariableType_HalSMModule));
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                        }
                        return HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else if (out->type==HalSMVariableType_HalSMClassC) {
                        //In future
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            } else {
                if (DictElementIndexByKey(((HalSMModule*)out->value)->vrs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMModule*)out->value)->vrs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMModule*)out->value)->lfuncs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMModule*)out->value)->lfuncs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMModule*)out->value)->classes,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMModule*)out->value)->classes,obuff)->value;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            }
        } else if (out->type==HalSMVariableType_HalSMCModule) {
            if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                if (DictElementIndexByKey(((HalSMCModule*)out->value)->lfuncs,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)DictElementFindByKey(((HalSMCModule*)out->value)->lfuncs,cobuff)->value->value,HalSMVariableType_HalSMLocalFunction));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMCModule*)out->value,HalSMVariableType_HalSMCModule));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMCModule*)out->value)->lfuncs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->classes,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMClass*)DictElementFindByKey(((HalSMCModule*)out->value)->classes,cobuff)->value->value,HalSMVariableType_HalSMClass));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMCModule*)out->value,HalSMVariableType_HalSMCModule));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)DictElementFindByKey(((HalSMCModule*)out->value)->classes,cobuff)->value->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0)),HalSMVariableType_HalSMClass); 
                } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->vrs,cobuff)!=-1) {
                    out=DictElementFindByKey(((HalSMCModule*)out->value)->vrs,cobuff)->value;
                    if (out->type==HalSMVariableType_HalSMFunctionC) {
                        if (ret) {
                            HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)out->value,HalSMVariableType_HalSMLocalFunction));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMCModule*)out->value,HalSMVariableType_HalSMCModule));
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                        }
                        return HalSMFunctionC_run(hsmc,(HalSMFunctionC*)out->value,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            } else {
                if (DictElementIndexByKey(((HalSMCModule*)out->value)->vrs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMCModule*)out->value)->vrs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->lfuncs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMCModule*)out->value)->lfuncs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMCModule*)out->value)->classes,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMCModule*)out->value)->classes,obuff)->value;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            }
        } else if (out->type==HalSMVariableType_HalSMRunClass) {
            if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                if (DictElementIndexByKey(((HalSMRunClass*)out->value)->funcs,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)DictElementFindByKey(((HalSMRunClass*)out->value)->funcs,cobuff)->value->value,HalSMVariableType_HalSMLocalFunction));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMRunClass*)out->value,HalSMVariableType_HalSMRunClass));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                } else if (DictElementIndexByKey(((HalSMRunClass*)out->value)->vars,cobuff)!=-1) {
                    out=DictElementFindByKey(((HalSMRunClass*)out->value)->vars,cobuff)->value;
                    if (out->type==HalSMVariableType_HalSMLocalFunction) {
                        if (ret) {
                            HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)out->value,HalSMVariableType_HalSMLocalFunction));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMRunClass*)out->value,HalSMVariableType_HalSMRunClass));
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                        }
                        return HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                }
            } else {
                if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,obuff)->value;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            }
        } else if (out->type==HalSMVariableType_HalSMClass) {
            if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                if (DictElementIndexByKey(((HalSMClass*)out->value)->funcs,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)DictElementFindByKey(((HalSMClass*)out->value)->funcs,cobuff)->value->value,HalSMVariableType_HalSMLocalFunction));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMClass*)out->value,HalSMVariableType_HalSMClass));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMClass*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                } else if (DictElementIndexByKey(((HalSMClass*)out->value)->vars,cobuff)!=-1) {
                    out=DictElementFindByKey(((HalSMClass*)out->value)->vars,cobuff)->value;
                    if (out->type==HalSMVariableType_HalSMLocalFunction) {
                        if (ret) {
                            HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)out->value,HalSMVariableType_HalSMLocalFunction));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMClass*)out->value,HalSMVariableType_HalSMClass));
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                        }
                        return HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                }
            } else {
                if (DictElementIndexByKey(((HalSMClass*)out->value)->vars,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMClass*)out->value)->vars,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMClass*)out->value)->funcs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMClass*)out->value)->funcs,obuff)->value;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            }
        } else if (out->type==HalSMVariableType_HalSMClassC) {
            if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                if (DictElementIndexByKey(((HalSMClassC*)out->value)->funcs,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)DictElementFindByKey(((HalSMClassC*)out->value)->funcs,cobuff)->value->value,HalSMVariableType_HalSMLocalFunction));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMClassC*)out->value,HalSMVariableType_HalSMClassC));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMClassC*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                } else if (DictElementIndexByKey(((HalSMClassC*)out->value)->vrs,cobuff)!=-1) {
                    out=DictElementFindByKey(((HalSMClassC*)out->value)->vrs,cobuff)->value;
                    if (out->type==HalSMVariableType_HalSMLocalFunction) {
                        if (ret) {
                            HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)out->value,HalSMVariableType_HalSMLocalFunction));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMClassC*)out->value,HalSMVariableType_HalSMClassC));
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                        }
                        return HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                }
            } else {
                if (DictElementIndexByKey(((HalSMClassC*)out->value)->vrs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMClassC*)out->value)->vrs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMClassC*)out->value)->funcs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMClassC*)out->value)->funcs,obuff)->value;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            }
        } else if (out->type==HalSMVariableType_HalSMRunClassC) {
            if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"(")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)obuff->value,")")!=-1) {
                HalSMVariable* cobuff=HalSMArray_get(HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)obuff->value,"("),0);
                if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)!=-1) {
                    if (ret) {
                        HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)->value->value,HalSMVariableType_HalSMLocalFunction));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                        HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMRunClassC*)out->value,HalSMVariableType_HalSMRunClassC));
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                    }
                    return HalSMLocalFunction_run((HalSMLocalFunction*)(DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,cobuff)->value->value),hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,cobuff)!=-1) {
                    out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,cobuff)->value;
                    if (out->type==HalSMVariableType_HalSMLocalFunction) {
                        if (ret) {
                            HalSMArray* ob=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)out->value,HalSMVariableType_HalSMLocalFunction));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,ob,HalSMVariable_init(hsmc->memorymanagmentlibrary,(HalSMRunClassC*)out->value,HalSMVariableType_HalSMRunClassC));
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,ob,HalSMVariableType_HalSMArray);
                        }
                        return HalSMLocalFunction_run((HalSMLocalFunction*)out->value,hsmc,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,*(char**)obuff->value,StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)cobuff->value,"(")+1,string_len(*(char**)obuff->value)-1),0));
                    } else {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function or Class");
                    }
                }
            } else {
                if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->vrs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMRunClassC*)out->value)->vrs,obuff)->value;
                } else if (DictElementIndexByKey(((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
                    out=DictElementFindByKey(((HalSMRunClassC*)out->value)->funcs,obuff)->value;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable or Function or Class Not Found");
                }
            }
        } else if (out->type==HalSMVariableType_HalSMDoubleGet) {
            if (HalSMIsInt(hsmc->memorymanagmentlibrary,*(char**)obuff->value)) {
                char* outfs=hsmc->memorymanagmentlibrary->malloc(string_len(((HalSMDoubleGet*)out->value)->st)+2+string_len(*(char**)obuff->value));
                string_cpy(outfs,((HalSMDoubleGet*)out->value)->st);
                string_cat(outfs,".");
                string_cat(outfs,*(char**)obuff->value);
                outfs[string_len(((HalSMDoubleGet*)out->value)->st)+1+string_len(*(char**)obuff->value)]='\0';
                out=HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,hsmc->stringlibrary->ParseDouble(outfs),double);
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Double");
            }
        }
    }

    return out;
}

HalSMVariable* HalSMCompiler_additionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_str||v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.addStr(hsmc->stringlibrary,hsmc->memorymanagmentlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.addDouble(hsmc->stringlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_int||v1->type==HalSMVariableType_int) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.addInt(hsmc->stringlibrary,v0,v1));
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMVariable* HalSMCompiler_subtractionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if ((v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double)&&v0->type!=HalSMVariableType_str&&v1->type!=HalSMVariableType_str) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.subDouble(hsmc->stringlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_int) {
        if (v1->type==HalSMVariableType_str) {
            v1=HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(*(char**)v1->value),int);
        }
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.subInt(hsmc->stringlibrary,v0,v1));
    } else if (v1->type==HalSMVariableType_int) {
        if (v0->type==HalSMVariableType_str) {
            v0=HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(*(char**)v0->value),int);
        }
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.subInt(hsmc->stringlibrary,v0,v1));
    }
    return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.subStr(hsmc->stringlibrary,hsmc->memorymanagmentlibrary,v0,v1));
}

HalSMVariable* HalSMCompiler_multiplyVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_str&&v1->type==HalSMVariableType_str) {
        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not possible to multiply a string by a string");
    } else if (v0->type==HalSMVariableType_str||v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.mulStr(hsmc->memorymanagmentlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.mulDouble(hsmc->stringlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_int||v1->type==HalSMVariableType_int) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.mulInt(hsmc->stringlibrary,v0,v1));
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMVariable* HalSMCompiler_divideVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_str&&v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,*(char**)v0->value,*(char**)v1->value,""));
    } else if (v0->type==HalSMVariableType_str||v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.divStr(hsmc->stringlibrary,hsmc->memorymanagmentlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.divDouble(hsmc->stringlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_int||v1->type==HalSMVariableType_int) {
        return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,hsmc->calcVars.divInt(hsmc->stringlibrary,v0,v1));
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMVariable* HalSMCompiler_getArgsSetVar(HalSMCompiler* hsmc,char* value)
{
    HalSMArray* out=HalSMCompiler_getArgs(hsmc,value,1);
    int skip=0;
    int lout=out->size;
    int ou=0;

    HalSMVariable* i;
    HalSMVariable* v;
    HalSMArray* temp;
    HalSMFunctionArray* tempf;

    while (ou<lout) {
        if (HalSMArray_get(out,ou)->type==HalSMVariableType_HalSMVar) {
            HalSMArray_set(out,DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)HalSMArray_get(out,ou)->value)->name))->value,ou);
        }
        ou++;
    }

    ou=0;

    while (ou<lout) {
        if (skip>0) {
            skip-=1;
            continue;
        }

        i=HalSMArray_get(out,ou);

        if (i->type==HalSMVariableType_HalSMFunctionArray) {
            tempf=(HalSMFunctionArray*)i->value;
            temp=tempf->args;
            if (tempf->type==HalSMFunctionArrayType_function) {
                if (temp->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                    HalSMArray_set(out,HalSMFunctionC_run(hsmc,(HalSMFunctionC*)temp->arr[0]->value,(HalSMArray*)temp->arr[1]->value),ou);
                } else {
                    HalSMArray_set(out,HalSMLocalFunction_run((HalSMLocalFunction*)temp->arr[0]->value,hsmc,(HalSMArray*)temp->arr[1]->value),ou);
                }
            } else if (tempf->type==HalSMFunctionArrayType_array) {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,*(char**)HalSMArray_get(temp,0)->value,0),HalSMVariableType_HalSMArray),ou);
            }
        } else if (i->type==HalSMVariableType_HalSMMult) {
            HalSMArray_set(out,HalSMCompiler_multiplyVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou-=1;
            lout-=2;
            skip=1;
        } else if (i->type==HalSMVariableType_HalSMDivide) {
            HalSMArray_set(out,HalSMCompiler_divideVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou-=1;
            lout-=2;
            skip=1;
        } else if (i->type==HalSMVariableType_HalSMSetArg) {
            ((HalSMSetArg*)i->value)->value=HalSMArray_get(out,ou+1);
            HalSMArray_set(out,i,ou);
            HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou+1);
            lout-=1;
        }
        ou+=1;
    }

    skip=0;
    lout=out->size;
    ou=0;

    while (ou<lout) {
        if (skip>0) {
            skip-=1;
            continue;
        }

        i=HalSMArray_get(out,ou);

        if (i->type==HalSMVariableType_HalSMPlus) {
            HalSMArray_set(out,HalSMCompiler_additionVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou-=1;
            lout-=2;
            skip=1;
        } else if (i->type==HalSMVariableType_HalSMMinus) {
            HalSMArray_set(out,HalSMCompiler_subtractionVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou-=1;
            lout-=2;
            skip=1;
        } else if (i->type==HalSMVariableType_HalSMEqual) {
            i=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (i->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMVariable_Compare(i,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
            }
        } else if (i->type==HalSMVariableType_HalSMNotEqual) {
            i=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (i->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMVariable_Compare(i,v)==0),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
            }
        } else if (i->type==HalSMVariableType_HalSMMore) {
            i=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (i->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((i->type==HalSMVariableType_int||i->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMCompiler_isMore(i,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"More (>) cannot without numeric args");
            }
        } else if (i->type==HalSMVariableType_HalSMLess) {
            i=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (i->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((i->type==HalSMVariableType_int||i->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMCompiler_isLess(i,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"Less (<) cannot without numeric args");
            }
        }
        ou+=1;
    }
    return HalSMArray_get(out,0);
}

HalSMArray* HalSMCompiler_getArgs(HalSMCompiler* hsmc,char* l,unsigned char tabs)
{
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    char isS='n';
    unsigned int isF=0;
    HalSMArray* o=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned int ind=0;
    unsigned int isA=0;
    unsigned int ll=string_len(l);
    unsigned int lind=ll-1;
    HalSMVariable* isGet;
    unsigned int isNArgs=0;
    unsigned int ignore=0;
    char i;
    HalSMVariable* buffo;
    char* buffs;
    HalSMArray* tempargs;

    for (unsigned int indexi=0;indexi<ll;indexi++) {
        i=l[indexi];
        if (ignore>0) {
            ignore--;continue;
        }

        buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
        buffo=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs);

        if (isNArgs>0&&i=='(') {
            isNArgs++;
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,'(',char));
        } else if (isNArgs>0&&i==')') {
            isNArgs--;
            if (isNArgs==0) {
                HalSMArray_appendArray(hsmc->memorymanagmentlibrary,out,HalSMCompiler_getArgs(hsmc,buffs,tabs));
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,')',char));}
        } else if (isNArgs>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
        } else if (isA>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
            if (i==']') {
                isA--;
                if (isA==0) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionArray_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,buffs,0,o->size-1))},1),HalSMFunctionArrayType_array),HalSMVariableType_HalSMFunctionArray));
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            } else if (i=='[') {isA++;}
        } else if (i=='['&&o->size==0) {isA++;}
        else if (isF>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
            buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
            if (i==')') {
                isF--;
                if (isF==0) {
                    if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)HalSMArray_split_str(hsmc->memorymanagmentlibrary,buffs,"(")->arr[0]->value,".")!=-1) {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMCompiler_isGet(hsmc,buffs,0));
                    } else {
                        if (tabs) {
                            //HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionArray_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs)},1),HalSMFunctionArrayType_function),HalSMVariableType_HalSMFunctionArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMCompiler_isRunFunction(hsmc,1,buffs));
                        } else {
                            HalSMFunctionArray* fc=(HalSMFunctionArray*)HalSMCompiler_isRunFunction(hsmc,0,buffs)->value;
                            tempargs=(HalSMArray*)fc->args->arr[1]->value;
                            if (fc->args->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMFunctionC_run(hsmc,(HalSMFunctionC*)fc->args->arr[0]->value,tempargs));
                            } else if (fc->args->arr[0]->type==HalSMVariableType_HalSMLocalFunction) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMLocalFunction_run((HalSMLocalFunction*)fc->args->arr[0]->value,hsmc,tempargs));
                            } else if (fc->args->arr[0]->type==HalSMVariableType_HalSMClass) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)fc->args->arr[0]->value,hsmc,tempargs),HalSMVariableType_HalSMRunClass));
                            }
                            //Add ClassC RunFunc
                        }
                    }
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            } else if (i=='(') {isF++;}
        } else if (isS!='n') {
            if (i==isS) {
                isS='n';
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,buffo);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));}
        } else if (i=='"'||i=='\'') {
            isS=i;
            if (l[ind+1]==')') {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,buffo);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
        } else if (i=='(') {
            if (isF==0) {
                if (o->size==0) {isNArgs++;}
                else {
                    isF++;
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
                }
            } else {
                isF++;
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
            }
        } else if (i==' ') {

        } else if (i==',') {
            if (o->size!=0) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,buffs,"[")!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,buffs,"]")==-1) {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,',',char));}
                else {
                    if (StringCompare(buffs,"true")) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)1));}
                    else if (StringCompare(buffs,"false")) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0));}
                    else if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                        if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                        else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                    } else if (DictElementIndexByKey(hsmc->localFunctions,buffo)!=-1) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->localFunctions,buffo)->value);}
                    else if (DictElementIndexByKey(hsmc->classes,buffo)!=-1) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->classes,buffo)->value);}
                    else if (DictElementIndexByKey(hsmc->modules,buffo)!=-1) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->modules,buffo)->value);}
                    else {
                        isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                        if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                        else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                        else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                        else if (tabs){HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                        else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                    }
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            }
        } else if (i=='+') {
            if (o->size!=0) {
                if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                    if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                } else {
                    isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                    if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                    else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                    else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                    else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&plus,HalSMVariableType_HalSMPlus));
        } else if (i=='-') {
            if (o->size!=0) {
                if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                    if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                } else {
                    isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                    if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                    else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                    else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                    else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&minus,HalSMVariableType_HalSMMinus));
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
            }
        } else if (i=='*') {
            if (o->size!=0) {
                if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                    if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                } else {
                    isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                    if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                    else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                    else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                    else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&mult,HalSMVariableType_HalSMMult));
        } else if (i=='/') {
            if (o->size!=0) {
                if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                    if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                } else {
                    isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                    if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                    else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                    else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                    else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                }
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&divide,HalSMVariableType_HalSMDivide));
        } else if (i=='=') {
            if (ind+1<=lind&&l[ind+1]=='=') {
                if (o->size!=0) {
                    if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                        if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                        else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                    } else {
                        isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                        if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                        else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                        else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                        else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                        else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                    }
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&equal,HalSMVariableType_HalSMEqual));
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Equal (==) cannot without args");
                }
                ignore=1;
            } else {
                if (o->size!=0) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetArg_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMSetArg));
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,'=',char));}
            }
        } else if (i=='!') {
            if (ind+1<=lind&&l[ind+1]=='=') {
                if (o->size!=0) {
                    if (DictElementIndexByKey(hsmc->variables,buffo)) {
                        if (tabs){HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                        else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                    } else {
                        isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                        if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                        else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                        else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                        else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                        else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&notequal,HalSMVariableType_HalSMNotEqual));
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Not Equal (!=) cannot without args");
                }
                ignore=1;
            }
        } else if (i=='>') {
            if (o->size!=0) {
                if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                    if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                } else {
                    isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                    if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                    else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                    else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                    else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                }
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&more,HalSMVariableType_HalSMMore));
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"More (>) cannot without numeric args");
            }
        }  else if (i=='<') {
            if (o->size!=0) {
                if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
                    if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
                } else {
                    isGet=HalSMCompiler_isGet(hsmc,buffs,0);
                    if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
                    else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
                    else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
                    else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
                    else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
                }
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&less,HalSMVariableType_HalSMLess));
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"Less (<) cannot without numeric args");
            }
        } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));}
        ind++;
    }

    if (o->size!=0) {
        buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
        buffo=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs);
        if (StringCompare(buffs,"true")) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)1));
        } else if (StringCompare(buffs,"false")) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0));
        } else if (DictElementIndexByKey(hsmc->variables,buffo)!=-1) {
            if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
            else {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->variables,buffo)->value);}
        } else if (DictElementIndexByKey(hsmc->localFunctions,buffo)!=-1) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->localFunctions,buffo)->value);
        } else if (DictElementIndexByKey(hsmc->modules,buffo)!=-1) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->modules,buffo)->value);
        }else if (DictElementIndexByKey(hsmc->classes,buffo)!=-1) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc->classes,buffo)->value);
        } else {
            isGet=HalSMCompiler_isGet(hsmc,buffs,0);
            if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(buffs),int));}
            else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(buffs),double));}
            else if (isGet->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
            else if (tabs) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_HalSMVar));}
            else {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");}
        }
    }

    unsigned int skip=0;
    unsigned int lout=out->size;
    unsigned int ou=0;

    HalSMVariable* iv;
    HalSMVariable* v;
    HalSMArray* temp;
    HalSMFunctionArray* tempf;

    while (ou<lout) {
        if (skip>0) {skip--;continue;}

        iv=HalSMArray_get(out,ou);

        if (iv->type==HalSMVariableType_HalSMFunctionArray) {
            if(tabs){skip=1;}
            else {
                tempf=(HalSMFunctionArray*)iv->value;
                temp=tempf->args;
                if (tempf->type==HalSMFunctionArrayType_function) {
                    if (tabs) {
                        HalSMArray* arr=(HalSMArray*)HalSMCompiler_isRunFunction(hsmc,1,*(char**)HalSMArray_get(temp,0)->value)->value;
                        if (arr->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                            HalSMArray_set(arr,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,(HalSMFunctionC*)HalSMArray_get(arr,0)->value,(HalSMArray*)HalSMArray_get(arr,1)->value),HalSMVariableType_HalSMRunFuncC),0);
                        } else if (arr->arr[0]->type==HalSMVariableType_HalSMLocalFunction) {
                            HalSMArray_set(arr,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)HalSMArray_get(arr,0)->value,(HalSMArray*)HalSMArray_get(arr,1)->value),HalSMVariableType_HalSMRunFunc),0);
                        }
                        HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,arr,HalSMVariableType_HalSMArray),ou);
                    } else {
                        v=HalSMCompiler_isRunFunction(hsmc,0,*(char**)HalSMArray_get(temp,0)->value);
                        if (v->type!=HalSMVariableType_HalSMNull) {
                            HalSMArray_set(out,HalSMFunctionC_run(hsmc,(HalSMFunctionC*)HalSMArray_get((HalSMArray*)v->value,0)->value,(HalSMArray*)HalSMArray_get((HalSMArray*)v->value,1)->value),ou);
                        } else if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)HalSMArray_get(temp,0)->value,".")!=-1) {
                            HalSMArray_set(out,HalSMCompiler_isGet(hsmc,*(char**)HalSMArray_get(temp,0)->value,0),ou);
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"Function Not Found");
                        }
                    }
                } else if (tempf->type==HalSMFunctionArrayType_array) {
                    HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,*(char**)HalSMArray_get(temp,0)->value,0),HalSMVariableType_HalSMArray),ou);
                }
            }
        } else if (iv->type==HalSMVariableType_HalSMMult) {
            if (tabs){skip=1;}
            else {
                HalSMArray_set(out,HalSMCompiler_multiplyVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
                temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
                HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
                out=temp;
                ou--;
                lout-=2;
                skip=1;
            }
        } else if (iv->type==HalSMVariableType_HalSMDivide) {
            if (tabs){skip=1;}
            else {
                HalSMArray_set(out,HalSMCompiler_divideVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
                temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
                HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
                out=temp;
                ou--;
                lout-=2;
                skip=1;
            }
        } else if (iv->type==HalSMVariableType_HalSMSetArg) {
            ((HalSMSetArg*)iv->value)->value=HalSMArray_get(out,ou+1);
            HalSMArray_set(out,iv,ou);
            HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou+1);
            lout--;
        }
        ou++;
    }

    skip=0;
    lout=out->size;
    ou=0;

    while (ou<lout) {
        if (skip>0){skip--;continue;}

        iv=HalSMArray_get(out,ou);

        if (iv->type==HalSMVariableType_HalSMPlus) {
            if (tabs){skip=1;}
            else {
                HalSMArray_set(out,HalSMCompiler_additionVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
                temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
                HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
                out=temp;
                ou--;
                lout-=2;
                skip=1;
            }
        } else if (iv->type==HalSMVariableType_HalSMMinus) {
            if (tabs){skip=1;}
            else {
                HalSMArray_set(out,HalSMCompiler_subtractionVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
                temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
                HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
                out=temp;
                ou--;
                lout-=2;
                skip=1;
            }
        } else if (iv->type==HalSMVariableType_HalSMEqual) {
            if (tabs){skip=1;}
            else {
                iv=HalSMArray_get(out,ou-1);
                v=HalSMArray_get(out,ou+1);
                if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
                else {
                    HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMVariable_Compare(iv,v)),ou-1);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    lout-=2;
                }
            }
        } else if (iv->type==HalSMVariableType_HalSMNotEqual) {
            if (tabs){skip=1;}
            else {
                iv=HalSMArray_get(out,ou-1);
                v=HalSMArray_get(out,ou+1);
                if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
                else {
                    HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)(HalSMVariable_Compare(iv,v)==0)),ou-1);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    lout-=2;
                }
            }
        } else if (iv->type==HalSMVariableType_HalSMMore) {
            if (tabs){skip=1;}
            else {
                iv=HalSMArray_get(out,ou-1);
                v=HalSMArray_get(out,ou+1);
                if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
                else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                    HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMCompiler_isMore(iv,v)),ou-1);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    lout-=2;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"More (>) cannot without numeric args");
                }
            }
        } else if (iv->type==HalSMVariableType_HalSMLess) {
            if (tabs){skip=1;}
            else {
                iv=HalSMArray_get(out,ou-1);
                v=HalSMArray_get(out,ou+1);
                if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
                else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                    HalSMArray_set(out,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,HalSMCompiler_isLess(iv,v)),ou-1);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    lout-=2;
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"More (>) cannot without numeric args");
                }
            }
        }
        ou++;
    }
    return out;
}

HalSMVariable* HalSMCompiler_isRunFunction(HalSMCompiler* hsmc,unsigned char tabs,char* l)
{
    if (StringEndsWith(hsmc->memorymanagmentlibrary,l,")")) {
        HalSMArray* array=HalSMArray_init(hsmc->memorymanagmentlibrary);
        HalSMArray* argus=HalSMArray_split_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,l,0,string_len(l)-1),"(");
        HalSMVariable* buffl=HalSMArray_get(argus,0);
        char* args=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,argus,1,argus->size),"(");
        HalSMFunctionArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
        out->type=HalSMFunctionArrayType_function;
        if (DictElementIndexByKey(hsmc->functions,buffl)!=-1) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,DictElementFindByKey(hsmc->functions,buffl)->value);
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,args,tabs),HalSMVariableType_HalSMArray));
            hsmc->memorymanagmentlibrary->free(args);
            out->args=array;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
        } else if (DictElementIndexByKey(hsmc->localFunctions,buffl)!=-1) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,DictElementFindByKey(hsmc->localFunctions,buffl)->value);
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,args,tabs),HalSMVariableType_HalSMArray));
            hsmc->memorymanagmentlibrary->free(args);
            out->args=array;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
        } else if (DictElementIndexByKey(hsmc->classes,buffl)!=-1) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,DictElementFindByKey(hsmc->classes,buffl)->value);
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,args,tabs),HalSMVariableType_HalSMArray));
            hsmc->memorymanagmentlibrary->free(args);
            out->args=array;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
        } else if (DictElementIndexByKey(hsmc->modules,buffl)!=-1) {
            if (DictElementFindByKey(hsmc->modules,buffl)->value->type==HalSMVariableType_HalSMModule) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,array,DictElementFindByKey(((HalSMModule*)DictElementFindByKey(hsmc->modules,buffl)->value->value)->lfuncs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__init__"))->value);
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,array,DictElementFindByKey(((HalSMCModule*)DictElementFindByKey(hsmc->modules,buffl)->value->value)->lfuncs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__init__"))->value);
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,args,tabs),HalSMVariableType_HalSMArray));
            hsmc->memorymanagmentlibrary->free(args);
            out->args=array;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
        } else if (DictElementIndexByKey(hsmc->variables,buffl)!=-1) {
            if (DictElementFindByKey(hsmc->variables,buffl)->value->type==HalSMVariableType_HalSMFunctionC||DictElementFindByKey(hsmc->variables,buffl)->value->type==HalSMVariableType_HalSMLocalFunction) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,array,DictElementFindByKey(hsmc->variables,buffl)->value);
                HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,args,tabs),HalSMVariableType_HalSMArray));
                hsmc->memorymanagmentlibrary->free(args);
                out->args=array;
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function");
            }
        }
        hsmc->memorymanagmentlibrary->free(args);
        HalSMVariable* isGet=HalSMCompiler_isGet(hsmc,l,1);
        if (isGet->type!=HalSMVariableType_HalSMNull) {
            HalSMArray* isga=(HalSMArray*)((HalSMFunctionArray*)isGet->value)->args->arr[1]->value;
            //Edit
            for (unsigned int i=0;i<isga->size;i++) {
                if (isga->arr[i]->type==HalSMVariableType_HalSMVar) {
                    isga->arr[i]=DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)isga->arr[i]->value)->name))->value;
                }
            }
            return isGet;
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

unsigned char HalSMCompiler_isMore(HalSMVariable* a,HalSMVariable* b)
{
    if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){return *(int*)a->value>*(int*)b->value;}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_double){return *(double*)a->value>*(double*)b->value;}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_double){return *(int*)a->value>*(double*)b->value;}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_int){return *(double*)a->value>*(int*)b->value;}
    return 0;
}

unsigned char HalSMCompiler_isLess(HalSMVariable* a,HalSMVariable* b)
{
    if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){return *(int*)a->value<*(int*)b->value;}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_double){return *(double*)a->value<*(double*)b->value;}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_double){return *(int*)a->value<*(double*)b->value;}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_int){return *(double*)a->value<*(int*)b->value;}
    return 0;
}

HalSMVariable* HalSMCompiler_getNameFunction(HalSMCompiler* hsmc,char* l)
{
    HalSMArray* name=HalSMArray_init(hsmc->memorymanagmentlibrary);
    char s;
    unsigned int ll=string_len(l);

    for (unsigned int i=0;i<ll;i++) {
        s=l[i];
        if (s=='(') {
            HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,name)));
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,l,i+1,string_len(l)-1)));
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
        }
        HalSMArray_add(hsmc->memorymanagmentlibrary,name,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,s,char));
    }

    HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Function");
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMVariable* HalSMCompiler_isSetVar(HalSMCompiler* hsmc,char* l)
{
    if (StringStartsWith(hsmc->memorymanagmentlibrary,l,"def ")){return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
    char i;
    HalSMArray* n=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned int ll=string_len(l);

    for (unsigned int ind=0;ind<ll;ind++) {
        i=l[ind];
        if (i=='"'||i=='\''||i=='('||i==')') {return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        else if (i=='='&&n->size!=0) {
            HalSMFunctionArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
            out->args=HalSMArray_init(hsmc->memorymanagmentlibrary);
            out->type=HalSMFunctionArrayType_var;
            HalSMArray_add(hsmc->memorymanagmentlibrary,out->args,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n)));
            HalSMArray_add(hsmc->memorymanagmentlibrary,out->args,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,l,ind+1,string_len(l))));
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
        }
        HalSMArray_add(hsmc->memorymanagmentlibrary,n,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,i));
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMArray* HalSMCompiler_getTabs(HalSMCompiler* hsmc,char* l)
{
    unsigned int i=0;
    while (1) {
        if (StringStartsWith(hsmc->memorymanagmentlibrary,l,"    ")) {
            i++;
            l=SubString(hsmc->memorymanagmentlibrary,l,4,string_len(l));
        } else {break;}
    }
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,i,unsigned int));
    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,l,"\r","")));
    return out;
}

unsigned char HalSMCompiler_isNull(char* text)
{
    char i;
    unsigned int lt=string_len(text);
    for (unsigned int ind=0;ind<lt;ind++) {
        i=text[ind];
        if (i!=' '){return 0;}
    }
    return 1;
}

HalSMArray* HalSMCompiler_compile(HalSMCompiler* hsmc,char* text,unsigned char isConsole)
{
    hsmc->line=1;
    unsigned char isFunc=0;
    unsigned char isClass=0;
    HalSMVariable* func=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* cls=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* clif=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMArray* tabs=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMVariable* isRunFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* isSetVar;
    char* tabsS;
    unsigned int tabsC;
    unsigned char isNull=0;
    HalSMLocalFunction* f;
    HalSMVariable* fc=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMArray* nameFunc;
    HalSMArray* spliti;
    char* joinSpliti;
    HalSMArray* b;
    HalSMFunctionArray* resRunFunc;
    HalSMVariable* err;
    char* l;
    HalSMArray* allLines=HalSMCompiler_getLines(hsmc,text);
    unsigned int la;

    for (unsigned int indexl=0;indexl<allLines->size;indexl++) {
        l=*(char**)HalSMArray_get(allLines,indexl)->value;
        tabs=HalSMCompiler_getTabs(hsmc,l);
        tabsS=*(char**)HalSMArray_get(tabs,1)->value;
        tabsC=*(int*)HalSMArray_get(tabs,0)->value;

        if (clelem->type!=HalSMVariableType_HalSMNull) {
            if (isClass&&isFunc) {tabsC=tabsC-2;}
            else if (isFunc) {tabsC=tabsC-1;}
        }

        if (!isFunc&&!isClass&&tabsC==0&&clelem->type!=HalSMVariableType_HalSMNull) {
            if (!isNull&&(StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"if ")||StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"while "))) {
                if (clif->type!=HalSMVariableType_HalSMNull){HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)clif->value,clelem);}
                clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
            } else if (!isNull&&(StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"elif ")||StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"else"))) {
                if (clif->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif or Else cannot without If");}
                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)clif->value,clelem);
                clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
            } else if (!isNull) {
                if (clif->type!=HalSMVariableType_HalSMNull) {
                    if (!isFunc&&!isClass) {
                        if (clelem->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)clif->value,clelem);}
                        HalSMVariable* r=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                        HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                        HalSMCLElement* d;
                        for (unsigned int indexclem=0;indexclem<((HalSMArray*)clif->value)->size;indexclem++) {
                            d=(HalSMCLElement*)((HalSMArray*)clif->value)->arr[indexclem]->value;
                            if (d->type==HalSMCLElementType_elif) {
                                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif cannot be without If");}
                                if ((unsigned char*)ifr->value==0) {
                                    err=d->start(d,hsmc);
                                    if (err->type==HalSMVariableType_HalSMError) {HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);}
                                    ifr=err;
                                }
                            } else if (d->type==HalSMCLElementType_else) {
                                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Else cannot be without If");}
                                if ((unsigned char*)ifr->value==0) {
                                    err=d->start(d,hsmc);
                                    if (err->type==HalSMVariableType_HalSMError) {HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);}
                                    ifr=err;
                                } else {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                            } else if (d->type==HalSMCLElementType_if) {
                                err=d->start(d,hsmc);
                                if (err->type==HalSMVariableType_HalSMError) {HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);}
                                ifr=err;
                            }
                        }
                        if (r->type==HalSMVariableType_HalSMError) {HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);}
                    } else if (isFunc&&!isClass) {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,clif);}
                } else if (clelem->type!=HalSMVariableType_HalSMNull) {
                    HalSMCLElement* d=(HalSMCLElement*)clelem->value;
                    err=d->start((HalSMCLElement*)clelem->value,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);
                    }
                }
                clif=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
            }
        }
        isSetVar=HalSMCompiler_isSetVar(hsmc,tabsS);
        isRunFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
        if (isSetVar->type==HalSMVariableType_HalSMNull) {isRunFunc=HalSMCompiler_isRunFunction(hsmc,tabsC>0,tabsS);}
        isNull=HalSMCompiler_isNull(tabsS);

        if (isClass) {
            if (tabsC==0&&!isNull) {
                isClass=0;
                if (isFunc) {PutDictElementToDict(hsmc->memorymanagmentlibrary,(((HalSMClass*)fc->value)->funcs),DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->name),func));}
                isFunc=0;
                if (clelem->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,(((HalSMLocalFunction*)func->value)->func),func);}
                clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                func=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                cls=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
            }
            if (tabsC==1&&!isNull) {
                if (isFunc) {
                    PutDictElementToDict(hsmc->memorymanagmentlibrary,(((HalSMClass*)fc->value)->funcs),DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->name),func));
                    isFunc=0;
                    func=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                }
                if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"def ")) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,":");
                    spliti=HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1);
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,spliti,":");
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,"def ");
                    spliti=HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size);
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,spliti,"def ");
                    nameFunc=(HalSMArray*)HalSMCompiler_getNameFunction(hsmc,joinSpliti)->value;
                    f=HalSMLocalFunction_init(hsmc->memorymanagmentlibrary,*(char**)nameFunc->arr[0]->value,*(char**)nameFunc->arr[1]->value,hsmc->variables);
                    func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                    PutDictElementToDict(hsmc->memorymanagmentlibrary,((HalSMClass*)cls->value)->funcs,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,f->name),func));
                    isFunc=1;
                }
            }
            if (isFunc) {
                if (isRunFunc->type!=HalSMVariableType_HalSMNull) {
                    resRunFunc=(HalSMFunctionArray*)isRunFunc->value;
                    HalSMVariable* ret;
                    if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                        ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,(HalSMFunctionC*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunFuncC);
                    } else if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMLocalFunction) {
                        ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunFunc);
                    }
                    if (clelem->type!=HalSMVariableType_HalSMNull) {
                        b=((HalSMCLElement*)clelem->value)->func;
                        while (1) {
                            if (b->size==0||b->arr[b->size-1]->type!=HalSMVariableType_HalSMCLElement) {break;}
                            b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,b,ret);
                    } else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);}
                } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"for ")) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                    char* vr=*(char**)spliti->arr[1]->value;
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,2,spliti->size)," ");
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,"in ");
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"in ");
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                    if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[0]->value;}
                    else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                    HalSMVariable* args=HalSMCompiler_getArgs(hsmc,joinSpliti,0)->arr[0];
                    if (args->type==HalSMVariableType_str) {
                        HalSMArray* fdf=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        la=string_len(*(char**)args->value);
                        for (unsigned int indexc=0;indexc<la;indexc++) {HalSMArray_add(hsmc->memorymanagmentlibrary,fdf,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(*(char**)args->value)[indexc]));}
                        args=HalSMVariable_init(hsmc->memorymanagmentlibrary,fdf,HalSMVariableType_HalSMArray);
                    } else if (args->type==HalSMVariableType_HalSMArray) {
                        if (((HalSMArray*)args->value)->arr[0]->type==HalSMVariableType_HalSMArray) {args=((HalSMArray*)args->value)->arr[0];}
                    }
                    if (clelem->type==HalSMVariableType_HalSMNull) {
                        clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFor_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,vr),(HalSMArray*)args->value),HalSMVariableType_HalSMCLElement);
                    } else {
                        b=((HalSMCLElement*)clelem->value)->func;
                        while (1) {
                            //Remake
                            if (b->size==0||b->arr[b->size-1]->type!=HalSMVariableType_HalSMCLElement){break;}
                            b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFor_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,vr),(HalSMArray*)args->value),HalSMVariableType_HalSMCLElement));
                    }
                } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"if ")) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                    if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[0]->value;}
                    else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                    HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,clelem->type!=HalSMVariableType_HalSMNull);
                    if (clelem->type==HalSMVariableType_HalSMNull) {
                        clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMIf_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);
                        clif=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray);
                    } else {
                        //Remake
                        b=((HalSMCLElement*)clelem->value)->func;
                        for (unsigned int i=0;i<tabsC-1;i++) {
                            b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMIf_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                    }
                } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"elif ")) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                    if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[1]->value;}
                    else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                    HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,clelem->type!=HalSMVariableType_HalSMNull);
                    if (clelem->type==HalSMVariableType_HalSMNull) {clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElif_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);}
                    else {
                        b=((HalSMCLElement*)clelem->value)->func;
                        for (unsigned int i=0;i<tabsC-1;i++) {
                            b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElif_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                    }
                } else if (StringCompare(tabsS,"else:")) {
                    if (clelem->type==HalSMVariableType_HalSMNull) {
                        //Remake: make error
                        clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElse_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMCLElement);
                    }
                    else {

                        b=((HalSMCLElement*)clelem->value)->func;
                        for (unsigned int i=0;i<tabsC-1;i++) {
                            b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElse_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMCLElement));
                    }
                } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"while ")) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                    if (spliti->size==1) {
                        //Remake
                        joinSpliti=*(char**)spliti->arr[0]->value;
                    } else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                    HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,1);
                    if (clelem->type==HalSMVariableType_HalSMNull) {clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMWhile_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);}
                    else {
                        b=((HalSMCLElement*)clelem->value)->func;
                        for (unsigned int i=0;i<tabsC-1;i++) {
                            b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMWhile_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                    }
                } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"return ")) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,"return ");
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"return ");
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMReturn_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,joinSpliti,1)),HalSMVariableType_HalSMReturn));
                } else if (isSetVar->type!=HalSMVariableType_HalSMNull) {
                    isSetVar=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetVar_init(hsmc->memorymanagmentlibrary,*(char**)((HalSMArray*)isSetVar->value)->arr[0]->value,*(char**)((HalSMArray*)isSetVar->value)->arr[1]->value),HalSMVariableType_HalSMSetVar);
                    if (clelem->type==HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,isSetVar);}
                    else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)(((HalSMLocalFunction*)func->value)->func->arr[((HalSMLocalFunction*)func->value)->func->size-1])->value)->func,isSetVar);}
                }
            }
        }

        if (isFunc&&!isClass) {
            if (tabsC==0&&!isNull) {
                isFunc=0;
                if(clelem->type!=HalSMVariableType_HalSMNull) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,clelem);
                }
                //Remake
                func=HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,0);
            } else if (isRunFunc->type!=HalSMVariableType_HalSMNull) {
                resRunFunc=(HalSMFunctionArray*)isRunFunc->value;
                HalSMVariable* ret;
                if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                    ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,(HalSMFunctionC*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunFuncC);
                } else {
                    ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunFunc);
                }
                if (clelem->type!=HalSMVariableType_HalSMNull) {
                    b=((HalSMCLElement*)clelem->value)->func;
                    while (1) {
                        if (b->size==0||b->arr[b->size-1]->type!=HalSMVariableType_HalSMCLElement) {break;}
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,ret);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"for ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                char* vr=*(char**)spliti->arr[1]->value;
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,2,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,"in ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"in ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[0]->value;}
                else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMVariable* args=HalSMCompiler_getArgs(hsmc,joinSpliti,0)->arr[0];
                if (args->type==HalSMVariableType_str) {
                    HalSMArray* fdf=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    la=string_len(*(char**)args->value);
                    for (unsigned int indexc=0;indexc<la;indexc++) {HalSMArray_add(hsmc->memorymanagmentlibrary,fdf,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(*(char**)args->value)[indexc]));}
                    args=HalSMVariable_init(hsmc->memorymanagmentlibrary,fdf,HalSMVariableType_HalSMArray);
                } else if (args->type==HalSMVariableType_HalSMArray) {
                    if (((HalSMArray*)args->value)->arr[0]->type==HalSMVariableType_HalSMArray) {args=((HalSMArray*)args->value)->arr[0];}
                }
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFor_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,vr),(HalSMArray*)args->value),HalSMVariableType_HalSMCLElement);
                } else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    while (1) {
                        //Remake
                        if (b->size==0||b->arr[b->size-1]->type!=HalSMVariableType_HalSMCLElement){break;}
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFor_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,vr),(HalSMArray*)args->value),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"if ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[0]->value;}
                else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,clelem->type!=HalSMVariableType_HalSMNull);
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMIf_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);
                    clif=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray);
                } else {
                    //Remake
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMIf_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"elif ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[1]->value;}
                else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,clelem->type!=HalSMVariableType_HalSMNull);
                if (clelem->type==HalSMVariableType_HalSMNull) {clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElif_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);}
                else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElif_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringCompare(tabsS,"else:")) {
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    //Remake: make error
                    clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElse_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMCLElement);
                }
                else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElse_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"while ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {
                    //Remake
                    joinSpliti=*(char**)spliti->arr[0]->value;
                } else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,1);
                if (clelem->type==HalSMVariableType_HalSMNull) {clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMWhile_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);}
                else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMWhile_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"return ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,"return ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"return ");
                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMReturn_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,joinSpliti,1)),HalSMVariableType_HalSMReturn));
            } else if (isSetVar->type!=HalSMVariableType_HalSMNull) {
                isSetVar=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetVar_init(hsmc->memorymanagmentlibrary,*(char**)((HalSMArray*)isSetVar->value)->arr[0]->value,*(char**)((HalSMArray*)isSetVar->value)->arr[1]->value),HalSMVariableType_HalSMSetVar);
                if (clelem->type==HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,isSetVar);}
                else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)(((HalSMLocalFunction*)func->value)->func->arr[((HalSMLocalFunction*)func->value)->func->size-1])->value)->func,isSetVar);}
            }
        }

        if (!isFunc&&!isClass) {
            if (StringStartsWith(hsmc->memorymanagmentlibrary,l,"import ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,l,"import ");
                joinSpliti=StringReplace(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"import "),"\n",""),"\r","");
                if (DictElementIndexByKey(hsmc->sys_modules,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,joinSpliti))!=-1) {
                    HalSMVariable* v=DictElementFindByKey(hsmc->sys_modules,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,joinSpliti))->value;
                    if (v->type==HalSMVariableType_str) {
                        if(string_len(*(char**)HalSMCompiler_readFile(hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,hsmc->pathModules,*(char**)v->value))},1))->value)>0) {
                            PutDictElementToDict(hsmc->memorymanagmentlibrary,
                                hsmc->modules,
                                DictElementInit(hsmc->memorymanagmentlibrary,
                                    HalSMVariable_init_str(hsmc->memorymanagmentlibrary,joinSpliti),
                                    HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_loadHalSMModule(hsmc,joinSpliti,ConcatenateStrings(hsmc->memorymanagmentlibrary,hsmc->pathModules,*(char**)v->value)),HalSMVariableType_HalSMModule)
                                )
                            );
                        } else {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"HalSM File is empty or not found");
                        }
                    } else {
                        PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,joinSpliti),v));
                    }
                } else if (string_len(*(char**)HalSMCompiler_readFile(hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,hsmc->path,"\\"),joinSpliti),".halsm"))},1))->value)>0) {
                    PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,joinSpliti),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_loadHalSMModule(hsmc,joinSpliti,ConcatenateStrings(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,hsmc->path,"\\"),joinSpliti),".halsm")),HalSMVariableType_HalSMModule)));
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,ConcatenateStrings(hsmc->memorymanagmentlibrary,"Module with name ",ConcatenateStrings(hsmc->memorymanagmentlibrary,joinSpliti," Not Found")));
                }
            } else if (isRunFunc->type!=HalSMVariableType_HalSMNull) {
                resRunFunc=(HalSMFunctionArray*)isRunFunc->value;
                if (clelem->type!=HalSMVariableType_HalSMNull) {
                    HalSMCLElement* elemn=(HalSMCLElement*)clelem->value;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        elemn=(HalSMCLElement*)elemn->func->arr[elemn->func->size-1]->value;
                    }
                    HalSMVariable* ret;
                    if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                        ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,(HalSMFunctionC*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunFuncC);
                    } else {
                        ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,(HalSMLocalFunction*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunFunc);
                    }
                    elemn->addFunc(hsmc->memorymanagmentlibrary,elemn,ret);
                } else {
                    if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMFunctionC) {
                        err=HalSMFunctionC_run(hsmc,(HalSMFunctionC*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value);
                    } else if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMLocalFunction) {
                        err=HalSMLocalFunction_run((HalSMLocalFunction*)resRunFunc->args->arr[0]->value,hsmc,(HalSMArray*)resRunFunc->args->arr[1]->value);
                    } else if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMClass) {
                        err=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)resRunFunc->args->arr[0]->value,hsmc,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunClass);
                    } else if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMClassC) {
                        err=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClassC_run(hsmc,(HalSMClassC*)resRunFunc->args->arr[0]->value,(HalSMArray*)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMRunClassC);
                    }

                    //Add all types
                    
                    if (err->type==HalSMVariableType_HalSMError) {HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);}
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"def ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,":");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,"def ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"def ");
                nameFunc=(HalSMArray*)HalSMCompiler_getNameFunction(hsmc,joinSpliti)->value;
                f=HalSMLocalFunction_init(hsmc->memorymanagmentlibrary,*(char**)nameFunc->arr[0]->value,*(char**)nameFunc->arr[1]->value,hsmc->variables);
                func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->localFunctions,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,f->name),func));
                isFunc=1;
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"class ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,":");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,"class ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"class ");
                fc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_init(hsmc->memorymanagmentlibrary,joinSpliti,hsmc->variables),HalSMVariableType_HalSMClass);
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->classes,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMClass*)fc->value)->name),fc));
                cls=fc;
                isClass=1;
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"for ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                char* vr=*(char**)spliti->arr[1]->value;
                spliti=HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,2,spliti->size);
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,spliti," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,"in ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"in ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[0]->value;}
                else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMVariable* args=HalSMCompiler_getArgs(hsmc,joinSpliti,0)->arr[0];

                if (args->type==HalSMVariableType_str) {
                    HalSMArray* fdf=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    la=string_len(*(char**)args->value);
                    for (unsigned int indexc=0;indexc<la;indexc++) {HalSMArray_add(hsmc->memorymanagmentlibrary,fdf,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(*(char**)args->value)[indexc]));}
                    args=HalSMVariable_init(hsmc->memorymanagmentlibrary,fdf,HalSMVariableType_HalSMArray);
                } else if (args->type==HalSMVariableType_HalSMArray) {}
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFor_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,vr),(HalSMArray*)args->value),HalSMVariableType_HalSMCLElement);
                } else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFor_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,vr),(HalSMArray*)args->value),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"if ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[0]->value;}
                else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,1);
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMIf_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);
                    clif=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray);
                } else {
                    //Remake
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMIf_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"elif ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {joinSpliti=*(char**)spliti->arr[1]->value;}
                else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,clelem->type!=HalSMVariableType_HalSMNull);
                if (clelem->type==HalSMVariableType_HalSMNull) {clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElif_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);}
                else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElif_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringCompare(tabsS,"else:")) {
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    //Remake: make error
                    clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElse_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMCLElement);
                }
                else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMElse_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMCLElement));
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,"while ")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS," ");
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size)," ");
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,":");
                if (spliti->size==1) {
                    //Remake
                    joinSpliti=*(char**)spliti->arr[0]->value;
                } else {joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),":");}
                HalSMArray* args=HalSMCompiler_getArgs(hsmc,joinSpliti,1);
                if (clelem->type==HalSMVariableType_HalSMNull) {clelem=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMWhile_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement);}
                else {
                    b=((HalSMCLElement*)clelem->value)->func;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        b=((HalSMCLElement*)b->arr[b->size-1]->value)->func;
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,b,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMWhile_init(hsmc->memorymanagmentlibrary,args),HalSMVariableType_HalSMCLElement));
                }
            } else if (isSetVar->type!=HalSMVariableType_HalSMNull) {
                resRunFunc=(HalSMFunctionArray*)isSetVar->value;
                if (clelem->type==HalSMVariableType_HalSMNull) {
                    if (StringIndexOf(hsmc->memorymanagmentlibrary,*(char**)resRunFunc->args->arr[0]->value,"[")!=-1&&StringEndsWith(hsmc->memorymanagmentlibrary,*(char**)resRunFunc->args->arr[0]->value,"]")) {
                        spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)resRunFunc->args->arr[0]->value,"[");
                        if (DictElementIndexByKey(hsmc->variables,spliti->arr[0])==-1) {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                        } else {
                            err=DictElementFindByKey(hsmc->variables,spliti->arr[0])->value;
                            if (err->type!=HalSMVariableType_HalSMArray) {
                                HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Array");
                            } else {
                                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"[");
                                HalSMArray* splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                unsigned int indexarr=1;
                                char temparrc;
                                HalSMArray* curArray=(HalSMArray*)err->value;
                                unsigned int string_lenarrayj=string_len(joinSpliti);
                                for (unsigned int indexj=0;indexj<string_lenarrayj;indexj++) {
                                    temparrc=joinSpliti[indexj];
                                    if (temparrc=='[') {
                                        if (indexarr>0) {
                                            HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,'['));
                                        }
                                        indexarr++;
                                    } else if (temparrc==']') {
                                        if (indexarr==0) {
                                            HalSMCompiler_ThrowError(hsmc,hsmc->line,"The index in array is not completed to the end");
                                        } else if (indexarr==1) {
                                            HalSMArray* argumentsf=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0);
                                            if (indexj==string_lenarrayj-1) {
                                                for (unsigned int indexaf=0;indexaf<argumentsf->size-1;indexaf++) {
                                                    curArray=(HalSMArray*)curArray->arr[*(int*)argumentsf->arr[indexaf]->value]->value;
                                                }
                                                curArray->arr[*(int*)argumentsf->arr[argumentsf->size-1]->value]=HalSMCompiler_getArgs(hsmc,*(char**)resRunFunc->args->arr[1]->value,0)->arr[0];
                                            } else {
                                                for (unsigned int indexaf=0;indexaf<argumentsf->size;indexaf++) {
                                                    curArray=(HalSMArray*)curArray->arr[*(int*)argumentsf->arr[indexaf]->value]->value;
                                                }
                                            }
                                            
                                            splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                        } else {
                                            HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,']'));
                                        }
                                        indexarr--;
                                    } else {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,temparrc));
                                    }
                                }
                            }
                        }
                    } else {
                        PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,resRunFunc->args->arr[0],HalSMCompiler_getArgs(hsmc,*(char**)resRunFunc->args->arr[1]->value,clelem->type!=HalSMVariableType_HalSMNull)->arr[0]));
                    }
                } else {
                    HalSMCLElement* elemn=(HalSMCLElement*)clelem->value;
                    for (unsigned int i=0;i<tabsC-1;i++) {
                        elemn=(HalSMCLElement*)elemn->func->arr[elemn->func->size-1]->value;
                    }
                    elemn->addFunc(hsmc->memorymanagmentlibrary,elemn,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetVar_init(hsmc->memorymanagmentlibrary,*(char**)resRunFunc->args->arr[0]->value,*(char**)resRunFunc->args->arr[1]->value),HalSMVariableType_HalSMSetVar));
                }
            } else {
                if (isConsole==1) {
                    HalSMArray* argumentsi=HalSMCompiler_getArgs(hsmc,tabsS,0);
                    if (argumentsi->size==0) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Unknown Line");
                    } else {
                        HalSMCompiler_print(hsmc,argumentsi);
                    }
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Unknown Line");
                }
            }
        }
        hsmc->line++;
    }
    if (clif->type!=HalSMVariableType_HalSMNull) {
        if (!isFunc && !isClass) {
            HalSMArray* clifa=(HalSMArray*)clif->value;
            if (clelem->type!=HalSMVariableType_HalSMNull) {HalSMArray_add(hsmc->memorymanagmentlibrary,clifa,clelem);}
            HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
            HalSMCLElement* d;
            for (unsigned int indexc=0;indexc<clifa->size;indexc++) {
                d=(HalSMCLElement*)clifa->arr[indexc]->value;
                if (d->type==HalSMCLElementType_elif) {
                    if (ifr->type==HalSMVariableType_HalSMNull) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif cannot be without If");
                    }
                    if (((unsigned char*)ifr->value)==0) {
                        err=d->start(d,hsmc);
                        if (err->type==HalSMVariableType_HalSMError) {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);
                        }
                        ifr=err;
                    }
                } else if (d->type==HalSMCLElementType_else) {
                    if (ifr->type==HalSMVariableType_HalSMNull) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Else cannot be without If");
                    }
                    if (((unsigned char*)ifr->value)==0) {
                        err=d->start(d,hsmc);
                        if (err->type==HalSMVariableType_HalSMError) {
                            HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);
                        }
                        ifr=err;
                    } else {
                        ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
                    }
                } else if (d->type==HalSMCLElementType_if) {
                    err=d->start(d,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);
                    }
                    ifr=err;
                }
            }
        }
    } else if (clelem->type!=HalSMVariableType_HalSMNull) {
        HalSMCLElement* d=(HalSMCLElement*)clelem->value;
        err=d->start((HalSMCLElement*)clelem->value,hsmc);
        if (err->type==HalSMVariableType_HalSMError) {
            HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)err->value)->error);
        }
    }

    HalSMArray* outArr=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMArray_add(hsmc->memorymanagmentlibrary,outArr,HalSMVariable_init(hsmc->memorymanagmentlibrary,DictCopy(hsmc->memorymanagmentlibrary,hsmc->variables),HalSMVariableType_HalSMDict));
    HalSMArray_add(hsmc->memorymanagmentlibrary,outArr,HalSMVariable_init(hsmc->memorymanagmentlibrary,DictCopy(hsmc->memorymanagmentlibrary,hsmc->localFunctions),HalSMVariableType_HalSMDict));
    HalSMArray_add(hsmc->memorymanagmentlibrary,outArr,HalSMVariable_init(hsmc->memorymanagmentlibrary,DictCopy(hsmc->memorymanagmentlibrary,hsmc->classes),HalSMVariableType_HalSMDict));
    HalSMArray_add(hsmc->memorymanagmentlibrary,outArr,HalSMVariable_init(hsmc->memorymanagmentlibrary,DictCopy(hsmc->memorymanagmentlibrary,hsmc->modules),HalSMVariableType_HalSMDict));

    hsmc->variables=DictCopy(hsmc->memorymanagmentlibrary,hsmc->sys_variables);
    hsmc->modules=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->classes=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->localFunctions=DictInit(hsmc->memorymanagmentlibrary);
    return outArr;
}

HalSMModule* HalSMCompiler_loadHalSMModule(HalSMCompiler* hsmc,char* name,char* file)
{
    HalSMCompiler* hsm=HalSMCompiler_init(
        file,
        hsmc->externModules,
        hsmc->print,
        hsmc->printErrorf,
        hsmc->inputf,
        hsmc->readFilef,
        hsmc->pathModules,
        hsmc->loadsharedlibrary,
        hsmc->stringlibrary,
        hsmc->memorymanagmentlibrary,
        hsmc->systemlibrary
    );
    HalSMArray* res=HalSMCompiler_compile(hsm,*(char**)HalSMCompiler_readFile(hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,file)},1))->value,0);
    return HalSMModule_init(hsmc->memorymanagmentlibrary,name,(Dict*)res->arr[0]->value,(Dict*)res->arr[1]->value,(Dict*)res->arr[2]->value);
}

HalSMCLElement* HalSMCLElement_init(HalSMMemoryManagmentLibrary* hsmmml,void(*addFunc)(HalSMMemoryManagmentLibrary*,void*,HalSMVariable*),HalSMVariable*(*start)(void*,HalSMCompiler*),HalSMCLElementType type,void* element)
{
    HalSMCLElement* out=hsmmml->malloc(sizeof(HalSMCLElement));
    out->func=HalSMArray_init(hsmmml);
    out->addFunc=addFunc;
    out->start=start;
    out->type=type;
    out->element=element;
    return out;
}

HalSMCLElement* HalSMFor_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* var,HalSMArray* arr)
{
    HalSMFor* out=hsmmml->malloc(sizeof(HalSMFor));
    out->var=var;
    out->arr=arr;
    return HalSMCLElement_init(hsmmml,HalSMFor_addFunc,HalSMFor_start,HalSMCLElementType_for,out);
}

HalSMVariable* HalSMFor_run(HalSMArray* func,HalSMCompiler* hsmc)
{
    HalSMArray* args;
    HalSMVariable* arg;
    unsigned char isFind=0;
    HalSMVariable* err;
    HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* f;
    HalSMArray* spliti;
    char* joinSpliti;
    
    for (unsigned int indexf=0;indexf<func->size;indexf++) {
        f=func->arr[indexf];
        if (f->type==HalSMVariableType_HalSMRunFuncC) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFuncC*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            args->arr[a]=entryValue;
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                    }
                }
            }
            err=HalSMFunctionC_run(hsmc,((HalSMRunFuncC*)f->value)->func,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMRunFunc) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFunc*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            args->arr[a]=entryValue;
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                    }
                }
            }
            err=HalSMLocalFunction_run(((HalSMRunFunc*)f->value)->func,hsmc,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMLocalFunction) {
            err=HalSMLocalFunction_run((HalSMLocalFunction*)f->value,hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]) {HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->variables,HalSMVariableType_HalSMDict)},1));
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMCLElement) {
            HalSMCLElement* clem=(HalSMCLElement*)f->value;
            if (clem->type==HalSMCLElementType_elif) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif cannot be without If");}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else if (clem->type==HalSMCLElementType_else) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Else cannot be without If");}
                if ((unsigned char*)ifr->value) {
                    err=clem->start(clem,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) {return err;}
                    ifr=err;
                } else {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
            } else if (clem->type==HalSMCLElementType_if) {
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else {
                if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
            }
        } else if (f->type==HalSMVariableType_HalSMSetVar) {
            arg=HalSMCompiler_getArgsSetVar(hsmc,((HalSMSetVar*)f->value)->value);
            char* nameVar=((HalSMSetVar*)f->value)->name;
            if (StringIndexOf(hsmc->memorymanagmentlibrary,nameVar,"[")!=-1&&StringEndsWith(hsmc->memorymanagmentlibrary,nameVar,"]")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,nameVar,"[");
                if (DictElementIndexByKey(hsmc->variables,spliti->arr[0])==-1) {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                } else {
                    err=DictElementFindByKey(hsmc->variables,spliti->arr[0])->value;
                    if (err->type!=HalSMVariableType_HalSMArray) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Array");
                    } else {
                        joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"[");
                        HalSMArray* splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        unsigned int indexarr=1;
                        char temparrc;
                        HalSMArray* curArray=(HalSMArray*)err->value;
                        unsigned int string_lenarrayj=string_len(joinSpliti);
                        for (unsigned int indexj=0;indexj<string_lenarrayj;indexj++) {
                            temparrc=joinSpliti[indexj];
                            if (temparrc=='[') {
                                if (indexarr>0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,'['));
                                }
                                indexarr++;
                            } else if (temparrc==']') {
                                if (indexarr==0) {
                                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"The index in array is not completed to the end");
                                } else if (indexarr==1) {
                                    if (indexj==string_lenarrayj-1) {
                                        curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]=arg;
                                    } else {
                                        curArray=(HalSMArray*)curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]->value;
                                    }
                                    
                                    splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,']'));
                                }
                                indexarr--;
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,temparrc));
                            }
                        }
                    }
                }
            } else {
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameVar),arg));
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

void HalSMFor_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func)
{
    HalSMArray_add(hsmmml,((HalSMCLElement*)element)->func,func);
}

HalSMVariable* HalSMFor_start(void* element,HalSMCompiler* hsmc)
{
    HalSMArray* arr;
    HalSMArray* args;
    HalSMCLElement* clem=(HalSMCLElement*)element;
    HalSMFor* elementFor=(HalSMFor*)clem->element;
    if (elementFor->arr->size==0) return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* temp=elementFor->arr->arr[0];
    HalSMVariable* v;
    if (temp->type==HalSMVariableType_HalSMVar) {
        v=DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)temp->value)->name))->value;
        arr=(HalSMArray*)v->value;
    } else if (temp->type==HalSMVariableType_HalSMRunFuncC) {
        arr=HalSMArray_copy(hsmc->memorymanagmentlibrary,elementFor->arr);
        args=HalSMArray_copy(hsmc->memorymanagmentlibrary,(HalSMArray*)arr->arr[1]->value);
        for (unsigned int arg=0;arg<args->size;arg++) {
            v=args->arr[arg];
            if (v->type==HalSMVariableType_HalSMVar){HalSMArray_set(args,DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)v->value)->name))->value,arg);}
        }
        HalSMVariable* arrr=HalSMFunctionC_run(hsmc,((HalSMRunFuncC*)temp->value)->func,args);
        arr=(HalSMArray*)(((HalSMArray*)arrr->value)->arr[0]->value);
    } else if (temp->type==HalSMVariableType_HalSMRunFunc) {
        arr=HalSMArray_copy(hsmc->memorymanagmentlibrary,elementFor->arr);
        args=HalSMArray_copy(hsmc->memorymanagmentlibrary,(HalSMArray*)arr->arr[1]->value);
        for (unsigned int arg=0;arg<args->size;arg++) {
            v=args->arr[arg];
            if (v->type==HalSMVariableType_HalSMVar){HalSMArray_set(args,DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)v->value)->name))->value,arg);}
        }
        HalSMLocalFunction* func=((HalSMRunFunc*)temp->value)->func;
        HalSMVariable* arrr=HalSMLocalFunction_run(func,hsmc,args);
        arr=(HalSMArray*)(((HalSMArray*)arrr->value)->arr[0]->value);
    } else if (temp->type==HalSMVariableType_HalSMLocalFunction) {
        args=HalSMArray_copy(hsmc->memorymanagmentlibrary,(HalSMArray*)elementFor->arr->arr[1]->value);
        for (unsigned int arg=0;arg<args->size;arg++) {
            v=args->arr[arg];
            if (v->type==HalSMVariableType_HalSMVar) {HalSMArray_set(args,DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)v->value)->name))->value,arg);}
        }
        arr=(HalSMArray*)HalSMLocalFunction_run((HalSMLocalFunction*)temp->value,hsmc,args)->value;
    } else if (temp->type==HalSMVariableType_HalSMArray) {
        arr=(HalSMArray*)temp->value;
    } else {
        arr=elementFor->arr;
    }
        
    HalSMVariable* r;
    HalSMArrayForEach(elem,arr) {
        HalSMVariable* copy=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
        copy=elem;
        
        PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(char**)elementFor->var->value),copy));
        r=HalSMFor_run(clem->func,hsmc);
        if (r->type==HalSMVariableType_HalSMError) {return r;}
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMCLElement* HalSMIf_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* arr)
{
    HalSMIf* out=hsmmml->malloc(sizeof(HalSMIf));
    out->arr=arr;
    return HalSMCLElement_init(hsmmml,HalSMIf_addFunc,HalSMIf_start,HalSMCLElementType_if,out);
}

HalSMVariable* HalSMIf_run(HalSMArray* func,HalSMCompiler* hsmc)
{
    HalSMArray* args;
    HalSMVariable* arg;
    unsigned char isFind=0;
    HalSMVariable* err;
    HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* f;
    HalSMArray* spliti;
    char* joinSpliti;

    for (unsigned int indexf=0;indexf<func->size;indexf++) {
        f=func->arr[indexf];
        if (f->type==HalSMVariableType_HalSMRunFuncC) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFuncC*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            HalSMArray_set(args,entryValue,a);
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,0,"Variable Not Found"),HalSMVariableType_HalSMError);
                    }
                }
            }
            err=HalSMFunctionC_run(hsmc,((HalSMRunFuncC*)f->value)->func,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMRunFunc) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFunc*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            HalSMArray_set(args,entryValue,a);
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,0,"Variable Not Found"),HalSMVariableType_HalSMError);
                    }
                }
            }
            err=HalSMLocalFunction_run(((HalSMRunFunc*)f->value)->func,hsmc,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMLocalFunction) {
            err=HalSMLocalFunction_run((HalSMLocalFunction*)f->value,hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->variables,HalSMVariableType_HalSMDict)},1));
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMCLElement) {
            HalSMCLElement* clem=(HalSMCLElement*)f->value;
            if (clem->type==HalSMCLElementType_elif) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif cannot be without If");}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else if (clem->type==HalSMCLElementType_else) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Else cannot be without If");}
                if ((unsigned char*)ifr->value) {
                    err=clem->start(clem,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) {return err;}
                    ifr=err;
                } else {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
            } else if (clem->type==HalSMCLElementType_if) {
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else {
                if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
            }
        } else if (f->type==HalSMVariableType_HalSMSetVar) {
            arg=HalSMCompiler_getArgsSetVar(hsmc,((HalSMSetVar*)f->value)->value);
            char* nameVar=((HalSMSetVar*)f->value)->name;
            if (StringIndexOf(hsmc->memorymanagmentlibrary,nameVar,"[")!=-1&&StringEndsWith(hsmc->memorymanagmentlibrary,nameVar,"]")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,nameVar,"[");
                if (DictElementIndexByKey(hsmc->variables,spliti->arr[0])==-1) {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                } else {
                    err=DictElementFindByKey(hsmc->variables,spliti->arr[0])->value;
                    if (err->type!=HalSMVariableType_HalSMArray) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Array");
                    } else {
                        joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"[");
                        HalSMArray* splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        unsigned int indexarr=1;
                        char temparrc;
                        HalSMArray* curArray=(HalSMArray*)err->value;
                        unsigned int string_lenarrayj=string_len(joinSpliti);
                        for (unsigned int indexj=0;indexj<string_lenarrayj;indexj++) {
                            temparrc=joinSpliti[indexj];
                            if (temparrc=='[') {
                                if (indexarr>0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,'['));
                                }
                                indexarr++;
                            } else if (temparrc==']') {
                                if (indexarr==0) {
                                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"The index in array is not completed to the end");
                                } else if (indexarr==1) {
                                    if (indexj==string_lenarrayj-1) {
                                        curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]=arg;
                                    } else {
                                        curArray=(HalSMArray*)curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]->value;
                                    }
                                    
                                    splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,']'));
                                }
                                indexarr--;
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,temparrc));
                            }
                        }
                    }
                }
            } else {
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameVar),arg));
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

void HalSMIf_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func)
{
    HalSMArray_add(hsmmml,((HalSMCLElement*)element)->func,func);
}

HalSMVariable* HalSMIf_start(void* element,HalSMCompiler* hsmc)
{
    HalSMArray* oa=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMCLElement* clelem=(HalSMCLElement*)element;
    HalSMIf* elementIf=(HalSMIf*)clelem->element;
    HalSMVariable* af;
    for (unsigned int indexaf=0;indexaf<elementIf->arr->size;indexaf++) {
        af=elementIf->arr->arr[indexaf];
        if (af->type==HalSMVariableType_HalSMVar) {HalSMArray_add(hsmc->memorymanagmentlibrary,oa,DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)af->value)->name))->value);}
        else {HalSMArray_add(hsmc->memorymanagmentlibrary,oa,af);}
    }

    unsigned int ind=0;
    unsigned int ignore=0;
    HalSMVariable* v;
    HalSMVariable* i;
    HalSMVariable* a;

    for (unsigned int indexa=0;indexa<oa->size;indexa++) {
        a=oa->arr[indexa];
        if (ignore>0) {
            ignore--;
            ind++;
            continue;
        }

        if (a->type==HalSMVariableType_HalSMEqual) {
            v=oa->arr[ind-1];
            i=oa->arr[ind+1];
            if (HalSMVariable_Compare(v,i)==0) {return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0);}
            ignore++;
        } else if (a->type==HalSMVariableType_HalSMNotEqual) {
            v=oa->arr[ind-1];
            i=oa->arr[ind+1];
            if (HalSMVariable_Compare(v,i)) {return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0);}
            ignore++;
        } else if (a->type==HalSMVariableType_HalSMBool) {
            if (((unsigned char*)a->value)==0) {return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0);}
        } else if (a->type==HalSMVariableType_HalSMMore) {
            v=oa->arr[ind-1];
            i=oa->arr[ind+1];
            if ((i->type==HalSMVariableType_int||i->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                if (HalSMCompiler_isMore(v,i)==0) {return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0);}
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"More (>) cannot without numeric args");
            }
            ignore++;
        } else if (a->type==HalSMVariableType_HalSMLess) {
            v=oa->arr[ind-1];
            i=oa->arr[ind+1];
            if ((i->type==HalSMVariableType_int||i->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                if (HalSMCompiler_isLess(v,i)==0) {return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)0);}
            } else {
                HalSMCompiler_ThrowError(hsmc,hsmc->line,"Less (<) cannot without numeric args");
            }
            ignore++;
        }
        ind++;
    }
    HalSMVariable* r=HalSMIf_run(clelem->func,hsmc);
    if (r->type==HalSMVariableType_HalSMError) {return r;}
    return HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,(unsigned char)1);
}

HalSMCLElement* HalSMElif_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* arr)
{
    HalSMIf* out=hsmmml->malloc(sizeof(HalSMIf));
    out->arr=arr;
    return HalSMCLElement_init(hsmmml,HalSMIf_addFunc,HalSMIf_start,HalSMCLElementType_elif,out);
}

HalSMCLElement* HalSMElse_init(HalSMMemoryManagmentLibrary* hsmmml)
{
    HalSMElse* out=hsmmml->malloc(sizeof(HalSMElse));
    return HalSMCLElement_init(hsmmml,HalSMElse_addFunc,HalSMElse_start,HalSMCLElementType_else,out);
}

HalSMVariable* HalSMElse_run(HalSMArray* func,HalSMCompiler* hsmc)
{
    HalSMArray* args;
    HalSMVariable* arg;
    unsigned char isFind=0;
    HalSMVariable* err;
    HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* f;
    HalSMArray* spliti;
    char* joinSpliti;

    for (unsigned int indexf=0;indexf<func->size;indexf++) {
        f=func->arr[indexf];
        if (f->type==HalSMVariableType_HalSMRunFuncC) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFuncC*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            HalSMArray_set(args,entryValue,a);
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,0,"Variable Not Found"),HalSMVariableType_HalSMError);
                    }
                }
            }
            err=HalSMFunctionC_run(hsmc,((HalSMRunFuncC*)f->value)->func,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMRunFunc) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFunc*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            HalSMArray_set(args,entryValue,a);
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,0,"Variable Not Found"),HalSMVariableType_HalSMError);
                    }
                }
            }
            err=HalSMLocalFunction_run(((HalSMRunFunc*)f->value)->func,hsmc,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMLocalFunction) {
            err=HalSMLocalFunction_run((HalSMLocalFunction*)f->value,hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->variables,HalSMVariableType_HalSMDict)},1));
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMCLElement) {
            HalSMCLElement* clem=(HalSMCLElement*)f->value;
            if (clem->type==HalSMCLElementType_elif) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif cannot be without If");}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else if (clem->type==HalSMCLElementType_else) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Else cannot be without If");}
                if ((unsigned char*)ifr->value) {
                    err=clem->start(clem,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) {return err;}
                    ifr=err;
                } else {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
            } else if (clem->type==HalSMCLElementType_if) {
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else {
                if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
            }
        } else if (f->type==HalSMVariableType_HalSMSetVar) {
            arg=HalSMCompiler_getArgsSetVar(hsmc,((HalSMSetVar*)f->value)->value);
            char* nameVar=((HalSMSetVar*)f->value)->name;
            if (StringIndexOf(hsmc->memorymanagmentlibrary,nameVar,"[")!=-1&&StringEndsWith(hsmc->memorymanagmentlibrary,nameVar,"]")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,nameVar,"[");
                if (DictElementIndexByKey(hsmc->variables,spliti->arr[0])==-1) {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                } else {
                    err=DictElementFindByKey(hsmc->variables,spliti->arr[0])->value;
                    if (err->type!=HalSMVariableType_HalSMArray) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Array");
                    } else {
                        joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"[");
                        HalSMArray* splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        unsigned int indexarr=1;
                        char temparrc;
                        HalSMArray* curArray=(HalSMArray*)err->value;
                        unsigned int string_lenarrayj=string_len(joinSpliti);
                        for (unsigned int indexj=0;indexj<string_lenarrayj;indexj++) {
                            temparrc=joinSpliti[indexj];
                            if (temparrc=='[') {
                                if (indexarr>0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,'['));
                                }
                                indexarr++;
                            } else if (temparrc==']') {
                                if (indexarr==0) {
                                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"The index in array is not completed to the end");
                                } else if (indexarr==1) {
                                    if (indexj==string_lenarrayj-1) {
                                        curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]=arg;
                                    } else {
                                        curArray=(HalSMArray*)curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]->value;
                                    }
                                    
                                    splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,']'));
                                }
                                indexarr--;
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,temparrc));
                            }
                        }
                    }
                }
            } else {
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameVar),arg));
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

void HalSMElse_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func)
{
    HalSMArray_add(hsmmml,((HalSMCLElement*)element)->func,func);
}

HalSMVariable* HalSMElse_start(void* element,HalSMCompiler* hsmc)
{
    HalSMVariable* err=HalSMElse_run(((HalSMCLElement*)element)->func,hsmc);
    if (err->type==HalSMVariableType_HalSMError) {return err;}
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMCLElement* HalSMWhile_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* arr)
{
    HalSMWhile* out=hsmmml->malloc(sizeof(HalSMWhile));
    out->arr=arr;
    return HalSMCLElement_init(hsmmml,HalSMWhile_addFunc,HalSMWhile_start,HalSMCLElementType_while,out);
}

HalSMVariable* HalSMWhile_run(HalSMArray* func,HalSMCompiler* hsmc)
{
    HalSMArray* args;
    HalSMVariable* arg;
    unsigned char isFind=0;
    HalSMVariable* err;
    HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* f;
    HalSMArray* spliti;
    char* joinSpliti;

    for (unsigned int indexf=0;indexf<func->size;indexf++) {
        f=func->arr[indexf];
        if (f->type==HalSMVariableType_HalSMRunFuncC) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFuncC*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            HalSMArray_set(args,entryValue,a);
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,0,"Variable Not Found"),HalSMVariableType_HalSMError);
                    }
                }
            }
            err=HalSMFunctionC_run(hsmc,((HalSMRunFuncC*)f->value)->func,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMRunFunc) {
            args=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFunc*)f->value)->args);
            for (unsigned int a=0;a<args->size;a++) {
                arg=args->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(entryKey,entryValue,hsmc->variables) {
                        if (StringCompare(finalArg->name,*(char**)entryKey->value)) {
                            HalSMArray_set(args,entryValue,a);
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {
                        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,0,"Variable Not Found"),HalSMVariableType_HalSMError);
                    }
                }
            }
            err=HalSMLocalFunction_run(((HalSMRunFunc*)f->value)->func,hsmc,args);
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMLocalFunction) {
            err=HalSMLocalFunction_run((HalSMLocalFunction*)f->value,hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->variables,HalSMVariableType_HalSMDict)},1));
            if (err->type==HalSMVariableType_HalSMError) {return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        } else if (f->type==HalSMVariableType_HalSMCLElement) {
            HalSMCLElement* clem=(HalSMCLElement*)f->value;
            if (clem->type==HalSMCLElementType_elif) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Elif cannot be without If");}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else if (clem->type==HalSMCLElementType_else) {
                if (ifr->type==HalSMVariableType_HalSMNull) {HalSMCompiler_ThrowError(hsmc,hsmc->line,"Else cannot be without If");}
                if ((unsigned char*)ifr->value) {
                    err=clem->start(clem,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) {return err;}
                    ifr=err;
                } else {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
            } else if (clem->type==HalSMCLElementType_if) {
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
                ifr=err;
            } else {
                if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                err=clem->start(clem,hsmc);
                if (err->type==HalSMVariableType_HalSMError) {return err;}
            }
        } else if (f->type==HalSMVariableType_HalSMSetVar) {
            arg=HalSMCompiler_getArgsSetVar(hsmc,((HalSMSetVar*)f->value)->value);
            char* nameVar=((HalSMSetVar*)f->value)->name;
            if (StringIndexOf(hsmc->memorymanagmentlibrary,nameVar,"[")!=-1&&StringEndsWith(hsmc->memorymanagmentlibrary,nameVar,"]")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,nameVar,"[");
                if (DictElementIndexByKey(hsmc->variables,spliti->arr[0])==-1) {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                } else {
                    err=DictElementFindByKey(hsmc->variables,spliti->arr[0])->value;
                    if (err->type!=HalSMVariableType_HalSMArray) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Array");
                    } else {
                        joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"[");
                        HalSMArray* splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        unsigned int indexarr=1;
                        char temparrc;
                        HalSMArray* curArray=(HalSMArray*)err->value;
                        unsigned int string_lenarrayj=string_len(joinSpliti);
                        for (unsigned int indexj=0;indexj<string_lenarrayj;indexj++) {
                            temparrc=joinSpliti[indexj];
                            if (temparrc=='[') {
                                if (indexarr>0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,'['));
                                }
                                indexarr++;
                            } else if (temparrc==']') {
                                if (indexarr==0) {
                                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"The index in array is not completed to the end");
                                } else if (indexarr==1) {
                                    if (indexj==string_lenarrayj-1) {
                                        curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]=arg;
                                    } else {
                                        curArray=(HalSMArray*)curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]->value;
                                    }
                                    
                                    splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,']'));
                                }
                                indexarr--;
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,temparrc));
                            }
                        }
                    }
                }
            } else {
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameVar),arg));
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

void HalSMWhile_addFunc(HalSMMemoryManagmentLibrary* hsmmml,void* element,HalSMVariable* func)
{
    HalSMArray_add(hsmmml,((HalSMCLElement*)element)->func,func);
}

HalSMVariable* HalSMWhile_start(void* element,HalSMCompiler* hsmc)
{
    HalSMArray* oa=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned int ind=0;
    unsigned int ignore=0;
    HalSMVariable* v;
    HalSMVariable* i;
    HalSMCLElement* clelem=(HalSMCLElement*)element;
    HalSMWhile* elementWhile=(HalSMWhile*)clelem->element;
    HalSMVariable* r;
    HalSMVariable* af;
    HalSMVariable* a;
    unsigned int indexaf;
    unsigned int indexa;

    while (1) {
        for (indexaf=0;indexaf<elementWhile->arr->size;indexaf++) {
            af=elementWhile->arr->arr[indexaf];
            if (af->type==HalSMVariableType_HalSMVar) {HalSMArray_add(hsmc->memorymanagmentlibrary,oa,DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)af->value)->name))->value);}
            else {HalSMArray_add(hsmc->memorymanagmentlibrary,oa,af);}
        }

        for (indexa=0;indexa<oa->size;indexa++) {
            a=oa->arr[indexa];
            if (ignore>0) {
                ignore--;
                ind++;
                continue;
            }

            if (a->type==HalSMVariableType_HalSMEqual) {
                v=oa->arr[ind-1];
                i=oa->arr[ind+1];
                if (HalSMVariable_Compare(v,i)==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                ignore++;
            } else if (a->type==HalSMVariableType_HalSMNotEqual) {
                v=oa->arr[ind-1];
                i=oa->arr[ind+1];
                if (HalSMVariable_Compare(v,i)) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                ignore++;
            } else if (a->type==HalSMVariableType_HalSMBool) {
                if (((unsigned char*)a->value)==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
            } else if (a->type==HalSMVariableType_HalSMMore) {
                v=oa->arr[ind-1];
                i=oa->arr[ind+1];
                if ((i->type==HalSMVariableType_int||i->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                    if (HalSMCompiler_isMore(v,i)==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"More (>) cannot without numeric args");
                }
                ignore++;
            } else if (a->type==HalSMVariableType_HalSMLess) {
                v=oa->arr[ind-1];
                i=oa->arr[ind+1];
                if ((i->type==HalSMVariableType_int||i->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double)) {
                    if (HalSMCompiler_isLess(v,i)==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Less (<) cannot without numeric args");
                }
                ignore++;
            }
            ind++;
        }
        ind=0;
        ignore=0;
        r=HalSMWhile_run(clelem->func,hsmc);
        if (r->type==HalSMVariableType_HalSMError) {return r;}
        oa=HalSMArray_init(hsmc->memorymanagmentlibrary);
    }
}

HalSMDoubleGet* HalSMDoubleGet_init(HalSMMemoryManagmentLibrary* hsmmml,char* st)
{
    HalSMDoubleGet* hfg=hsmmml->malloc(sizeof(HalSMDoubleGet));
    hfg->st=st;
    return hfg;
}

HalSMVar* HalSMVar_init(HalSMMemoryManagmentLibrary* hsmmml,char* name)
{
    HalSMVar* out=hsmmml->malloc(sizeof(HalSMVar));
    out->name=name;
    return out;
}

HalSMSetArg* HalSMSetArg_init(HalSMMemoryManagmentLibrary* hsmmml,char* name)
{
    HalSMSetArg* out=hsmmml->malloc(sizeof(HalSMSetArg));
    out->name=name;
    out->value=HalSMVariable_init(hsmmml,&null,HalSMVariableType_HalSMNull);
    return out;
}

HalSMReturn* HalSMReturn_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* val)
{
    HalSMReturn* out=hsmmml->malloc(sizeof(HalSMReturn));
    out->value=val;
    return out;
}

Dict* DictInit(HalSMMemoryManagmentLibrary* hsmmml)
{
    Dict* dict=hsmmml->malloc(sizeof(Dict));
    dict->size=0;
    dict->elements=hsmmml->malloc(0);
    return dict;
}

Dict* DictInitWithElements(HalSMMemoryManagmentLibrary* hsmmml,DictElement* elements[],unsigned int size)
{
    Dict* dict=hsmmml->malloc(sizeof(Dict));
    dict->size=size;
    dict->elements=hsmmml->malloc(sizeof(DictElement*)*size);
    for (unsigned int i=0;i<size;i++) {
        dict->elements[i]=elements[i];
    }
    return dict;
}

DictElement* DictElementInit(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* key,HalSMVariable* value)
{
    DictElement* out=hsmmml->malloc(sizeof(DictElement));
    out->key=key;
    out->value=value;
    return out;
}

DictElement* DictElementFindByIndex(Dict* dict,unsigned int index)
{
    DictElement* temp;
    if (index<dict->size)temp=dict->elements[index];
    return temp;
}

DictElement* DictElementFindByKey(Dict* dict,HalSMVariable* key)
{
    DictElement* empty;
    if (dict->size==0){return empty;}
    for (unsigned int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(dict->elements[i]->key,key)) return dict->elements[i];
    }
    return empty;
}

DictElement* DictElementFindByValue(Dict* dict,HalSMVariable* value)
{
    DictElement* empty;
    if (dict->size==0) {return empty;}
    for (unsigned int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(dict->elements[i]->value,value)) return dict->elements[i];
    }
    return empty;
}

void PutDictElementToDict(HalSMMemoryManagmentLibrary* hsmmml,Dict *dict,DictElement* elem)
{
    int index=DictElementIndexByKey(dict,elem->key);
    if (index==-1) {
        dict->elements=hsmmml->realloc(dict->elements,(1+dict->size)*sizeof(DictElement*));
        dict->elements[dict->size]=elem;
        dict->size=dict->size+1;
    } else {dict->elements[index]=elem;}
}

int DictElementIndexByKey(Dict* dict,HalSMVariable* key)
{
    for (unsigned int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(dict->elements[i]->key,key)) {return i;}
    }
    return -1;
}

int DictElementIndexByValue(Dict* dict,HalSMVariable* value)
{
    for (unsigned int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(dict->elements[i]->value,value)) return i;
    }
    return -1;
}

Dict* DictCopy(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict)
{
    return DictInitWithElements(hsmmml,dict->elements,dict->size);
}

unsigned char DictCompare(Dict* a,Dict* b)
{
    if (a->size!=b->size){return 0;}
    for (unsigned int i=0;i<a->size;i++) {
        if (HalSMVariable_Compare(a->elements[i]->key,b->elements[i]->key)==0||HalSMVariable_Compare(a->elements[i]->value,b->elements[i]->value)==0){return 0;}
    }
    return 1;
}

HalSMVariable* HalSMVariable_init(HalSMMemoryManagmentLibrary* hsmmml,void* value,HalSMVariableType type)
{
    HalSMVariable* out=hsmmml->malloc(sizeof(HalSMVariable));
    out->type=type;
    out->value=value;
    return out;
}

void HalSMVariable_AsVar(void* var,HalSMVariable* arg)
{
    HalSMVariableType type=arg->type;
    void* value=arg->value;
    if(type==HalSMVariableType_int){(*(int*)var)=(*(int*)value);}
    else if(type==HalSMVariableType_char){(*(char*)var)=(*(char*)value);}
    else if(type==HalSMVariableType_double){(*(double*)var)=(*(double*)value);}
    else if(type==HalSMVariableType_void){*((void**)var)=*((void**)value);}
    else if(type==HalSMVariableType_HalSMArray){*((HalSMArray*)var)=*((HalSMArray*)value);}
    else if(type==HalSMVariableType_str){(*(char**)var)=(*(char**)value);}
    else if(type==HalSMVariableType_int_array){*((int**)var)=*((int**)value);}
    else if(type==HalSMVariableType_HalSMFunctionC){(*(HalSMFunctionC*)var)=*(HalSMFunctionC*)value;}
    else if(type==HalSMVariableType_HalSMRunClassC){(*(HalSMRunClassC*)var)=*(HalSMRunClassC*)value;}
    else if(type==HalSMVariableType_HalSMError){(*(HalSMError*)var)=*(HalSMError*)value;}
    else if(type==HalSMVariableType_HalSMNull){(*(HalSMNull*)var)=*(HalSMNull*)value;}
    else if(type==HalSMVariableType_HalSMRunFunc){(*(HalSMRunFunc*)var)=*(HalSMRunFunc*)value;}
    else if(type==HalSMVariableType_HalSMRunFuncC){(*(HalSMRunFuncC*)var)=*(HalSMRunFuncC*)value;}
    else if(type==HalSMVariableType_HalSMLocalFunction){*((HalSMLocalFunction*)var)=*(HalSMLocalFunction*)value;}
    else if(type==HalSMVariableType_HalSMCModule){(*(HalSMCModule*)var)=*(HalSMCModule*)value;}
    else if(type==HalSMVariableType_HalSMModule){(*(HalSMModule*)var)=*(HalSMModule*)value;}
    else if(type==HalSMVariableType_HalSMClassC){(*(HalSMClassC*)var)=*(HalSMClassC*)value;}
    else if(type==HalSMVariableType_HalSMCompiler){(*(HalSMCompiler*)var)=*(HalSMCompiler*)value;}
    else if(type==HalSMVariableType_HalSMCompiler_source){(*(HalSMCompiler**)var)=*(HalSMCompiler**)value;}
    else if(type==HalSMVariableType_HalSMRunClassC_source){(*(HalSMRunClassC**)var)=*(HalSMRunClassC**)value;}
    else if(type==HalSMVariableType_HalSMRunClass_source){(*(HalSMRunClass**)var)=*(HalSMRunClass**)value;}
    else if(type==HalSMVariableType_HalSMRunClass){(*(HalSMRunClass*)var)=*(HalSMRunClass*)value;}
    else if(type==HalSMVariableType_HalSMDoubleGet){(*(HalSMDoubleGet*)var)=*(HalSMDoubleGet*)value;}
    else if(type==HalSMVariableType_HalSMClass){(*(HalSMClass*)var)=*(HalSMClass*)value;}
    else if(type==HalSMVariableType_HalSMVar){(*(HalSMVar*)var)=*(HalSMVar*)value;}
    else if(type==HalSMVariableType_HalSMPlus){(*(HalSMPlus*)var)=*(HalSMPlus*)value;}
    else if(type==HalSMVariableType_HalSMMinus){(*(HalSMMinus*)var)=*(HalSMMinus*)value;}
    else if(type==HalSMVariableType_HalSMMult){(*(HalSMMult*)var)=*(HalSMMult*)value;}
    else if(type==HalSMVariableType_HalSMDivide){(*(HalSMDivide*)var)=*(HalSMDivide*)value;}
    else if(type==HalSMVariableType_HalSMEqual){(*(HalSMEqual*)var)=*(HalSMEqual*)value;}
    else if(type==HalSMVariableType_HalSMNotEqual){(*(HalSMNotEqual*)var)=*(HalSMNotEqual*)value;}
    else if(type==HalSMVariableType_HalSMMore){(*(HalSMMore*)var)=*(HalSMMore*)value;}
    else if(type==HalSMVariableType_HalSMLess){(*(HalSMLess*)var)=*(HalSMLess*)value;}
    else if(type==HalSMVariableType_HalSMBool){(*(unsigned char*)var)=*(unsigned char*)value;}
    else if(type==HalSMVariableType_HalSMCLElement){(*(HalSMCLElement*)var)=*(HalSMCLElement*)value;}
    else if(type==HalSMVariableType_HalSMDict){(*(Dict*)var)=*(Dict*)value;}
    else if(type==HalSMVariableType_HalSMSetVar){(*(HalSMSetVar*)var)=*(HalSMSetVar*)value;}
    else if(type==HalSMVariableType_HalSMReturn){(*(HalSMReturn*)var)=*(HalSMReturn*)value;}
    else if(type==HalSMVariableType_HalSMFunctionCTypeDef){(*(HalSMFunctionCTypeDef*)var)=*(HalSMFunctionCTypeDef*)value;}
    else if(type==HalSMVariableType_HalSMFunctionArray){(*(HalSMFunctionArray*)var)=*(HalSMFunctionArray*)value;}
    else if(type==HalSMVariableType_unsigned_int){(*(unsigned int*)var)=*(unsigned int*)value;}
}

void* HalSMVariable_Read(HalSMVariable* arg){return arg->value;}

HalSMVariable* HalSMVariable_init_str(HalSMMemoryManagmentLibrary* hsmmml,char* str) {
    char** d=hsmmml->malloc(sizeof(char*));
    *d=hsmmml->calloc(string_len(str)+1,sizeof(char));
    stringncpy(*d,str,string_len(str));
    return HalSMVariable_init(hsmmml,d,HalSMVariableType_str);
}

char* HalSMVariable_to_str(HalSMStringLibrary* hsmsl,HalSMVariable* var)
{
    char *out;
    HalSMVariableType type=var->type;
    if(type==HalSMVariableType_str){out=*(char**)var->value;}
    else if(type==HalSMVariableType_int){out=hsmsl->Decimal2Str(*(int*)var->value);}
    else if(type==HalSMVariableType_double){out=hsmsl->Double2Str(*(double*)var->value);}
    return out;
}

unsigned char HalSMVariable_Compare(HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMVariableType type=v0->type;
    void* var=v0->value;
    void* value=v1->value;
    if(type==HalSMVariableType_int){return *(int*)var==*(int*)value;}
    else if(type==HalSMVariableType_char){return *(char*)var==*(char*)value;}
    else if(type==HalSMVariableType_double){return *(double*)var==*(double*)value;}
    else if(type==HalSMVariableType_void){return *(void**)var==*(void**)value;}
    else if(type==HalSMVariableType_HalSMArray){return HalSMArray_compare((HalSMArray*)var,(HalSMArray*)value);}
    else if(type==HalSMVariableType_str){return StringCompare(*(char**)var,*(char**)value);}
    else if(type==HalSMVariableType_HalSMFunctionC){return ((HalSMFunctionC*)var)->func==((HalSMFunctionC*)value)->func;}
    else if(type==HalSMVariableType_HalSMRunClassC){return StringCompare(((HalSMRunClassC*)var)->name,((HalSMRunClassC*)value)->name)&&DictCompare(((HalSMRunClassC*)var)->vrs,((HalSMRunClassC*)value)->vrs)&&DictCompare(((HalSMRunClassC*)var)->funcs,((HalSMRunClassC*)value)->funcs);}
    else if(type==HalSMVariableType_HalSMError){return ((HalSMError*)var)->line==((HalSMError*)value)->line&&StringCompare(((HalSMError*)var)->error,((HalSMError*)value)->error);}
    else if(type==HalSMVariableType_HalSMNull){return 1;}
    else if(type==HalSMVariableType_HalSMRunFunc){return 1;}
    else if(type==HalSMVariableType_HalSMRunFuncC){return 1;}
    else if(type==HalSMVariableType_HalSMLocalFunction){return StringCompare(((HalSMLocalFunction*)var)->name,((HalSMLocalFunction*)value)->name)&&HalSMArray_compare(((HalSMLocalFunction*)var)->args,((HalSMLocalFunction*)value)->args)&&HalSMArray_compare(((HalSMLocalFunction*)var)->func,((HalSMLocalFunction*)value)->func)&&DictCompare(((HalSMLocalFunction*)var)->vars,((HalSMLocalFunction*)value)->vars);}
    else if(type==HalSMVariableType_HalSMCModule){return DictCompare(((HalSMCModule*)var)->lfuncs,((HalSMCModule*)value)->lfuncs)&&DictCompare(((HalSMCModule*)var)->vrs,((HalSMCModule*)value)->vrs)&&DictCompare(((HalSMCModule*)var)->classes,((HalSMCModule*)value)->classes)&&StringCompare(((HalSMCModule*)var)->name,((HalSMCModule*)value)->name);}
    else if(type==HalSMVariableType_HalSMModule){return StringCompare(((HalSMModule*)var)->name,((HalSMModule*)value)->name)&&DictCompare(((HalSMModule*)var)->vrs,((HalSMModule*)value)->vrs)&&DictCompare(((HalSMModule*)var)->lfuncs,((HalSMModule*)value)->lfuncs)&&DictCompare(((HalSMModule*)var)->classes,((HalSMModule*)value)->classes);}
    else if(type==HalSMVariableType_HalSMClassC){return StringCompare(((HalSMClassC*)var)->name,((HalSMClassC*)value)->name)&&((HalSMClassC*)var)->init_runclass==((HalSMClassC*)value)->init_runclass&&DictCompare(((HalSMClassC*)var)->vrs,((HalSMClassC*)value)->vrs)&&DictCompare(((HalSMClassC*)var)->funcs,((HalSMClassC*)value)->funcs);}
    else if(type==HalSMVariableType_HalSMCompiler){/*finish it later*/}
    else if(type==HalSMVariableType_HalSMRunClass){return StringCompare(((HalSMRunClass*)var)->name,((HalSMRunClass*)value)->name)&&DictCompare(((HalSMRunClass*)var)->funcs,((HalSMRunClass*)value)->funcs)&&DictCompare(((HalSMRunClass*)var)->vars,((HalSMRunClass*)value)->vars);}
    else if(type==HalSMVariableType_HalSMDoubleGet){return StringCompare(((HalSMDoubleGet*)var)->st,((HalSMDoubleGet*)value)->st);}
    else if(type==HalSMVariableType_HalSMClass){return StringCompare(((HalSMClass*)var)->name,((HalSMClass*)value)->name)&&DictCompare(((HalSMClass*)var)->funcs,((HalSMClass*)value)->funcs)&&DictCompare(((HalSMClass*)var)->vars,((HalSMClass*)value)->vars);}
    else if(type==HalSMVariableType_HalSMVar){return StringCompare(((HalSMVar*)var)->name,((HalSMVar*)value)->name);}
    else if(type==HalSMVariableType_HalSMPlus){return 1;}
    else if(type==HalSMVariableType_HalSMMinus){return 1;}
    else if(type==HalSMVariableType_HalSMMult){return 1;}
    else if(type==HalSMVariableType_HalSMDivide){return 1;}
    else if(type==HalSMVariableType_HalSMEqual){return 1;}
    else if(type==HalSMVariableType_HalSMNotEqual){return 1;}
    else if(type==HalSMVariableType_HalSMMore){return 1;}
    else if(type==HalSMVariableType_HalSMLess){return 1;}
    else if(type==HalSMVariableType_HalSMBool){return *(unsigned char*)var==*(unsigned char*)value;}
    else if(type==HalSMVariableType_HalSMCLElement){return ((HalSMCLElement*)var)->type==((HalSMCLElement*)value)->type&&HalSMArray_compare(((HalSMCLElement*)var)->func,((HalSMCLElement*)value)->func)&&((HalSMCLElement*)var)->addFunc==((HalSMCLElement*)value)->addFunc&&((HalSMCLElement*)var)->start==((HalSMCLElement*)value)->start;}
    else if(type==HalSMVariableType_HalSMSetVar){return StringCompare(((HalSMSetVar*)var)->name,((HalSMSetVar*)value)->name)&&StringCompare(((HalSMSetVar*)var)->value,((HalSMSetVar*)value)->value);}
    else if(type==HalSMVariableType_HalSMReturn){return HalSMArray_compare(((HalSMReturn*)var)->value,((HalSMReturn*)value)->value);}
    else if(type==HalSMVariableType_HalSMFunctionArray){return ((HalSMFunctionArray*)var)->type==((HalSMFunctionArray*)value)->type&&HalSMArray_compare(((HalSMFunctionArray*)var)->args,((HalSMFunctionArray*)value)->args);}
    else if(type==HalSMVariableType_unsigned_int){return *(unsigned int*)var==*(unsigned int*)value;}
    return 0;
}

HalSMSetVar* HalSMSetVar_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,char* value)
{
    HalSMSetVar* out=hsmmml->malloc(sizeof(HalSMSetVar));
    out->name=name;
    out->value=value;
    return out;
}

HalSMArray* HalSMArray_init(HalSMMemoryManagmentLibrary* hsmmml)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->size=0;
    out->arr=hsmmml->malloc(0);
    return out;
}

HalSMArray* HalSMArray_split_str(HalSMMemoryManagmentLibrary* hsmmml,char* str,char* spl)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->size=0;
    out->arr=hsmmml->malloc(0);
    unsigned int slen=string_len(str);
    unsigned int plen=string_len(spl);
    if (slen<plen) {
        return HalSMArray_init_with_elements(hsmmml,(HalSMVariable*[]){HalSMVariable_init_str(hsmmml,"")},1);
    }
    unsigned int i=0;
    char *arr;
    char* temp=hsmmml->malloc(0);
    unsigned int d=0;

    while (i<slen) {
        if (i<slen-plen+1) {
            arr=SubString(hsmmml,str,i,i+plen);
            if (StringCompare(arr,spl)) {
                temp=hsmmml->realloc(temp,(d+1)*sizeof(char));
                temp[d]='\0';
                HalSMArray_add(hsmmml,out,HalSMVariable_init_str(hsmmml,temp));
                i+=plen;
                d=0;
                temp=hsmmml->malloc(0);
            } else {
                d++;
                temp=hsmmml->realloc(temp,d*sizeof(char));
                temp[d-1]=str[i];
                i++;
            }
        } else {
            d++;
            temp=hsmmml->realloc(temp,d*sizeof(char));
            temp[d-1]=str[i];
            i++;
        }
    }

    if (string_len(temp)>0) {
        temp=hsmmml->realloc(temp,(d+1)*sizeof(char));
        temp[d]='\0';
        HalSMArray_add(hsmmml,out,HalSMVariable_init_str(hsmmml,temp));
    } else {
        HalSMArray_add(hsmmml,out,HalSMVariable_init_str(hsmmml,""));
    }

    hsmmml->free(arr);
    hsmmml->free(temp);
    return out;
}

int HalSMArray_index(HalSMArray* harr,HalSMVariable* value)
{
    for (unsigned int i=0;i<harr->size;i++)
    {
        if (HalSMVariable_Compare(harr->arr[i],value))return i;
    }
    return -1;
}

void HalSMArray_add(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value)
{
    harr->arr=hsmmml->realloc(harr->arr,sizeof(HalSMVariable*)*(harr->size+1));
    harr->arr[harr->size]=value;
    harr->size=harr->size+1;
}

void HalSMArray_set(HalSMArray* harr,HalSMVariable* value,unsigned int index)
{
    if (index<harr->size) {
        harr->arr[index]=value;
    }
}

void HalSMArray_remove(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned int index)
{
    int b=0;
    HalSMVariable** arr=hsmmml->malloc((harr->size-1)*sizeof(HalSMVariable*));
    for (unsigned int i=0;i<harr->size;i++) {
        if(i!=index){arr[b]=harr->arr[i];b++;}
    }
    harr->size=harr->size-1;
    harr->arr=arr;
}

HalSMArray* HalSMArray_reverse(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    HalSMArray* out=HalSMArray_init(hsmmml);
    for (unsigned int i=harr->size-1;i--;){HalSMArray_add(hsmmml,out,harr->arr[i]);if (i==0) {break;}}
    return out;
}

void HalSMArray_appendArray(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMArray* narr)
{
    harr->arr=hsmmml->realloc(harr->arr,(harr->size+narr->size)*sizeof(HalSMVariable*));
    for (unsigned int i=0;i<narr->size;i++){harr->arr[harr->size+i]=narr->arr[i];}
    harr->size=harr->size+narr->size;
}

void HalSMArray_insert(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value,unsigned int index)
{
    //Error
    if (index>harr->size) {return;}
    HalSMVariable** temp=hsmmml->malloc(sizeof(HalSMVariable*)*(harr->size+1));
    unsigned int i;
    for (i=0;i<index;i++) {
        temp[i]=harr->arr[i];
    }

    for (i=index;i<harr->size;i++) {
        temp[i+1]=harr->arr[i];
    }
    temp[index]=value;
    hsmmml->free(harr->arr);
    harr->arr=temp;
    harr->size=harr->size+1;
}

HalSMVariable* HalSMArray_get(HalSMArray* harr,unsigned int index)
{
    if (harr->size==0||index>=harr->size){HalSMVariable* temp;return temp;}
    return harr->arr[index];
}

void AdditionStrings(HalSMMemoryManagmentLibrary* hsmmml,char** c,char* f,unsigned int sizec,unsigned int sizef)
{
    char* tmp=hsmmml->calloc(sizec+1,sizeof(char));
    memory_cpy(tmp,*c,sizec);
    *c=hsmmml->calloc(sizec+sizef+1,sizeof(char));
    memory_cpy(*c,tmp,sizec);
    memory_cpy(*c+string_len(*c),f,sizef);
    hsmmml->free(tmp);
}

char* HalSMArray_join_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,char* join)
{
    if(harr->size==0) {
        return "";
    } else if (harr->size==1) {
        return *(char**)harr->arr[0]->value;
    }
    unsigned int size=0;
    unsigned int lj=string_len(join);
    char* out=hsmmml->calloc(1,sizeof(char));
    out[0]='\0';
    unsigned int i=0;
    size=0;
    HalSMArrayForEach(o,harr) {
        char* oc=*(char**)o->value;
        AdditionStrings(hsmmml,&out,oc,size,string_len(oc));
        size+=string_len(oc);
        if (i!=harr->size-1) {
            AdditionStrings(hsmmml,&out,join,size,lj);
            size+=lj;
        }
        i++;
    }
    out[size]='\0';
    return out;
}

char* HalSMArray_to_print(HalSMCompiler* hsmc,HalSMArray* harr)
{
    char* joinout=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMCompiler_get_print_text(hsmc,harr),", ");
    char* outs=hsmc->memorymanagmentlibrary->malloc(3+string_len(joinout));
    string_cpy(outs,"[");
    string_cat(outs,joinout);
    string_cat(outs,"]");
    outs[2+string_len(joinout)]='\0';
    return outs;
}

char* HalSMArray_chars_to_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    char* out=hsmmml->malloc(harr->size+1);
    for (unsigned int i=0;i<harr->size;i++) {out[i]=*(char*)harr->arr[i]->value;}
    out[harr->size]='\0';
    return out;
}

HalSMArray* HalSMArray_slice(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned int s,unsigned int e)
{
    if (s>=e){
        //Error
    }
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->arr=hsmmml->malloc((MathMin(e,harr->size)-s)*sizeof(HalSMVariable*));
    out->size=MathMin(e,harr->size)-s;

    unsigned int mm=MathMin(e,harr->size);

    for (unsigned int i=s,d=0;i<mm;i++,d++) {out->arr[d]=harr->arr[i];}
    return out;
}

unsigned char HalSMArray_compare(HalSMArray* harr,HalSMArray* barr)
{
    if (harr->size!=barr->size){return 0;}
    for (unsigned int i=0;i<harr->size;i++) {
        if (HalSMVariable_Compare(harr->arr[i],barr->arr[i])==0){return 0;}
    }
    return 1;
}

HalSMArray* HalSMArray_from_str(HalSMMemoryManagmentLibrary* hsmmml,char* str,unsigned int size)
{
    HalSMArray* out=HalSMArray_init(hsmmml);
    for (unsigned int i=0;i<size;i++) {HalSMArray_add(hsmmml,out,HalSMVariable_FromValue(hsmmml,str[i]));}
    return out;
}

HalSMArray* HalSMArray_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->size=harr->size;
    out->arr=hsmmml->calloc(harr->size,sizeof(HalSMVariable*));
    memory_cpy(out->arr,harr->arr,harr->size*sizeof(HalSMVariable*));
    return out;
}

HalSMArray* HalSMArray_init_with_elements(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable** arr,unsigned int size)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->arr=arr;
    out->size=size;
    return out;
}

HalSMFunctionC* HalSMFunctionC_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMFunctionCTypeDef func)
{
    HalSMFunctionC* out=hsmmml->malloc(sizeof(HalSMFunctionC));
    out->func=func;
    return out;
}

HalSMVariable* HalSMFunctionC_run(HalSMCompiler* hsmc,HalSMFunctionC* hfc,HalSMArray* args) {
    return hfc->func(hsmc,args);
}

void HalSMFunctionC_GetArg(void* var,HalSMArray* args,unsigned int index){HalSMVariable_AsVar(var,args->arr[index]);}

HalSMRunClassC* HalSMRunClassC_init(HalSMMemoryManagmentLibrary* hsmmml,void(*init_runclass)(HalSMRunClassC*),char* name,Dict* vrs,Dict* funcs)
{
    HalSMRunClassC* runclassc=hsmmml->malloc(sizeof(HalSMRunClassC));
    runclassc->name=name;
    runclassc->vrs=vrs;
    runclassc->funcs=funcs;
    init_runclass(runclassc);
    return runclassc;
}

HalSMRunClassC* HalSMRunClassC__init__(HalSMCompiler* hsmc,HalSMRunClassC* runclassc,HalSMArray* args)
{
    int indexInit=DictElementIndexByKey(runclassc->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__init__"));
    if (indexInit!=-1){
        HalSMFunctionC_run(hsmc,(HalSMFunctionC*)DictElementFindByIndex(runclassc->funcs,indexInit)->value->value,args);
    }
    return runclassc;
}

HalSMClassC* HalSMClassC_init(HalSMMemoryManagmentLibrary* hsmmml,void(*init_runclass)(HalSMRunClassC*),char* name)
{
    HalSMClassC* classc=hsmmml->malloc(sizeof(HalSMClassC));
    classc->vrs=DictInit(hsmmml);
    classc->funcs=DictInit(hsmmml);
    classc->name=name;
    classc->init_runclass=init_runclass;
    return classc;
}

HalSMRunClassC* HalSMClassC_run(HalSMCompiler* hsmc,HalSMClassC* classc,HalSMArray* args)
{
    HalSMRunClassC* o=HalSMRunClassC_init(hsmc->memorymanagmentlibrary,classc->init_runclass,classc->name,classc->vrs,classc->funcs);
    HalSMArray_insert(hsmc->memorymanagmentlibrary,args,HalSMVariable_init(hsmc->memorymanagmentlibrary,o,HalSMVariableType_HalSMRunClassC),0);
    return HalSMRunClassC__init__(hsmc,o,args);
}

HalSMClass* HalSMClass_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,Dict* vrs)
{
    HalSMClass* out=hsmmml->malloc(sizeof(HalSMClass));
    out->vars=vrs;
    out->funcs=DictInit(hsmmml);
    out->name=name;
    return out;
}

HalSMRunClass* HalSMClass_run(HalSMClass* class,HalSMCompiler* hsmc,HalSMArray* args)
{
    HalSMRunClass* out=HalSMRunClass_init(hsmc->memorymanagmentlibrary,class->name,class->vars,class->funcs);
    return HalSMRunClass__init__(out,hsmc,args);
}

HalSMRunClass* HalSMRunClass_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,Dict* vrs,Dict* funcs)
{
    HalSMRunClass* out=hsmmml->malloc(sizeof(HalSMRunClass));
    out->name=name;
    out->vars=DictCopy(hsmmml,vrs);
    out->funcs=DictCopy(hsmmml,funcs);
    return out;
}

HalSMRunClass* HalSMRunClass__init__(HalSMRunClass* runclass,HalSMCompiler* hsmc,HalSMArray* args)
{
    if (DictElementIndexByKey(runclass->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__init__"))!=-1) {
        HalSMLocalFunction* func=(HalSMLocalFunction*)DictElementFindByKey(runclass->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,"__init__"))->value->value;
        HalSMLocalFunction_run(func,hsmc,args);
    }
    return runclass;
}

HalSMCModule* HalSMCModule_init(HalSMMemoryManagmentLibrary* hsmmml,char* name)
{
    HalSMCModule* out=hsmmml->malloc(sizeof(HalSMCModule));
    out->lfuncs=DictInit(hsmmml);
    out->vrs=DictInit(hsmmml);
    out->classes=DictInit(hsmmml);
    out->name=name;
    return out;
}

HalSMCModule* HalSMCModule_load(HalSMMemoryManagmentLibrary* hsmmml,HalSMLoadSharedLibrary* loadsharedlibrary,char* path,char* nameModule)
{
    HalSMCModule* out=hsmmml->malloc(sizeof(HalSMCModule));
    void* handle=loadsharedlibrary->loadLibrary(path);
    HalSMCModule_entry* hsmcme=((HalSMCModule_entry*(*)())loadsharedlibrary->getAddressByName(handle,ConcatenateStrings(hsmmml,"HalSMInit_",nameModule)))();
    out->name=hsmcme->name;
    out->lfuncs=hsmcme->lfuncs;
    out->vrs=hsmcme->vrs;
    out->classes=hsmcme->classes;
    //loadsharedlibrary->closeLibrary(path);
    return out;
}

HalSMModule* HalSMModule_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,Dict* vrs,Dict* lfuncs,Dict* classes)
{
    HalSMModule* o=hsmmml->malloc(sizeof(HalSMModule));
    o->lfuncs=lfuncs;
    o->vrs=vrs;
    o->classes=classes;
    o->name=name;
    return o;
}

HalSMRunFuncC* HalSMRunFuncC_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMFunctionC* func,HalSMArray* args)
{
    HalSMRunFuncC* out=hsmmml->malloc(sizeof(HalSMRunFuncC));
    out->func=func;
    out->args=args;
    return out;
}

HalSMRunFunc* HalSMRunFunc_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMLocalFunction* func,HalSMArray* args)
{
    HalSMRunFunc* out=hsmmml->malloc(sizeof(HalSMRunFunc));
    out->func=func;
    out->args=args;
    return out;
}

unsigned char HalSMIsInt(HalSMMemoryManagmentLibrary* hsmmml,char *c)
{
    char b;
    HalSMVariable* var=HalSMVariable_init(hsmmml,&b,HalSMVariableType_char);
    for (unsigned int i=0;i<string_len(c);i++)
    {
        b=c[i];
        if ((b=='-'&&i==0)){continue;}
        else if (HalSMArray_index(arrInt,var)==-1){return 0;}
    }
    return 1;
}

unsigned char HalSMIsDouble(HalSMMemoryManagmentLibrary* hsmmml,char *c)
{
    int ct=0;
    for (unsigned int i=0;i<string_len(c);i++)
    {
        char b=c[i];
        if ((b=='-' && i>0) || HalSMArray_index(arrInt,HalSMVariable_init(hsmmml,&b,HalSMVariableType_char))==-1){return 0;}
        else if(b=='.' && (i==0 || i==string_len(c)-1)){return 0;}
        else if(b=='.' && i>0 && i<string_len(c)){ct+=1;if(ct==2)return 0;}
    }
    if (ct==0){return 0;}
    return 1;
}

HalSMVariable* ParseHalSMVariableInt(HalSMCompiler* hsmc,char* c)
{
    return HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(int)hsmc->stringlibrary->ParseDecimal(c),int);
}

HalSMVariable* ParseHalSMVariableDouble(HalSMCompiler* hsmc,char* c)
{
    return HalSMVariable_FromValueWithType(hsmc->memorymanagmentlibrary,(double)hsmc->stringlibrary->ParseDouble(c),double);
}

HalSMVariable* HalSMLocalFunction_run(HalSMLocalFunction* lf,HalSMCompiler* hsmc,HalSMArray* args)
{
    unsigned int ia=0;
    HalSMVariable* v;
    Dict* vrs=DictInitWithElements(hsmc->memorymanagmentlibrary,lf->vars->elements,lf->vars->size);
    for (unsigned int arg=0;arg<args->size;arg++)
    {
        v=args->arr[arg];
        if (v->type==HalSMVariableType_HalSMSetArg)
        {
            PutDictElementToDict(hsmc->memorymanagmentlibrary,lf->vars,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init(hsmc->memorymanagmentlibrary,((HalSMSetArg*)v->value)->name,HalSMVariableType_str),((HalSMSetArg*)v->value)->value));
            ia++;
            continue;
        }
        PutDictElementToDict(hsmc->memorymanagmentlibrary,vrs,DictElementInit(hsmc->memorymanagmentlibrary,HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(char**)lf->args->arr[arg-ia]->value,"=")->arr[0],v));
    }
    HalSMVariable* ad;
    char* argi;
    char e='=';
    
    for (unsigned int a=0;a<lf->args->size;a++)
    {
        argi=*(char**)lf->args->arr[a]->value;
        ad=HalSMArray_split_str(hsmc->memorymanagmentlibrary,argi,"=")->arr[0];
        if (DictElementIndexByKey(vrs,ad)!=-1){continue;}
        if (HalSMArray_index(HalSMArray_from_str(hsmc->memorymanagmentlibrary,argi,string_len(argi)),HalSMVariable_init(hsmc->memorymanagmentlibrary,&e,HalSMVariableType_char))!=-1) {
            PutDictElementToDict(hsmc->memorymanagmentlibrary,vrs,DictElementInit(hsmc->memorymanagmentlibrary,ad,HalSMCompiler_getArgs(hsmc,*(char**)HalSMArray_split_str(hsmc->memorymanagmentlibrary,argi,"=")->arr[1]->value,0)->arr[0]));
        } else {
            //Add calculation line number
            HalSMError* err=HalSMError_init(hsmc,hsmc->line,"Not enough args");
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,err,HalSMVariableType_HalSMError);
        }
    }

    HalSMArray* arguss;
    HalSMVariable* arg;
    unsigned char isFind;
    HalSMVariable* err;
    HalSMVariable* ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
    HalSMVariable* f;
    HalSMArray* spliti;
    char* joinSpliti;

    for (unsigned int fi=0;fi<lf->func->size;fi++)
    {
        f=lf->func->arr[fi];
        if(f->type==HalSMVariableType_HalSMRunFuncC) {
            arguss=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFuncC*)f->value)->args);
            for (unsigned int a=0;a<arguss->size;a++) {
                arg=arguss->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(key,val,vrs) {
                        if (StringCompare(finalArg->name,*(char**)key->value)) {
                            arguss->arr[a]=val;
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->line,"Variable Not Found"),HalSMVariableType_HalSMError);}
                }
            }
            err=HalSMFunctionC_run(hsmc,((HalSMRunFuncC*)f->value)->func,arguss);
            if(err->type==HalSMVariableType_HalSMError){return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
        } else if(f->type==HalSMVariableType_HalSMRunFunc) {
            arguss=HalSMArray_copy(hsmc->memorymanagmentlibrary,((HalSMRunFunc*)f->value)->args);
            for (unsigned int a=0;a<arguss->size;a++) {
                arg=arguss->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    isFind=0;
                    HalSMVar* finalArg=(HalSMVar*)arg->value;
                    DictForEach(key,val,vrs) {
                        if (StringCompare(finalArg->name,*(char**)key->value)) {
                            arguss->arr[a]=val;
                            isFind=1;
                            break;
                        }
                    }
                    if (isFind==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->line,"Variable Not Found"),HalSMVariableType_HalSMError);}
                }
            }
            err=HalSMLocalFunction_run(((HalSMRunFunc*)f->value)->func,hsmc,arguss);
            if(err->type==HalSMVariableType_HalSMError){return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
        } else if(f->type==HalSMVariableType_HalSMLocalFunction) {
            err=HalSMLocalFunction_run(((HalSMLocalFunction*)f->value),hsmc,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,vrs,HalSMVariableType_HalSMDict)},1));
            if(err->type==HalSMVariableType_HalSMError){return err;}
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
        } else if (f->type==HalSMVariableType_HalSMCLElement) {
            HalSMCLElement* clt=(HalSMCLElement*)f->value;
            if (clt->type==HalSMCLElementType_elif) {
                if (ifr->type==HalSMVariableType_HalSMNull) return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->line,"Elif cannot be without If"),HalSMVariableType_HalSMError);
                err=clt->start(clt->element,hsmc);
                if (err->type==HalSMVariableType_HalSMError) return err;
                ifr=err;
            } else if (clt->type==HalSMCLElementType_else) {
                if (ifr->type==HalSMVariableType_HalSMNull) return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->line,"Else cannot be without If"),HalSMVariableType_HalSMError);
                if ((*(unsigned char*)ifr->value)==1) {
                    err=clt->start(clt->element,hsmc);
                    if (err->type==HalSMVariableType_HalSMError) return err;
                    ifr=err;
                } else {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
            } else if (clt->type==HalSMCLElementType_if) {
                err=clt->start(clt->element,hsmc);
                if (err->type==HalSMVariableType_HalSMError) return err;
                ifr=err;
            } else {
                if (ifr->type!=HalSMVariableType_HalSMNull) {ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);}
                err=clt->start(clt->element,hsmc);
                if (err->type==HalSMVariableType_HalSMError) return err;
            }
        } else if (f->type==HalSMVariableType_HalSMSetVar) {
            arg=HalSMCompiler_getArgsSetVar(hsmc,((HalSMSetVar*)f->value)->value);
            char* nameVar=((HalSMSetVar*)f->value)->name;
            if (StringIndexOf(hsmc->memorymanagmentlibrary,nameVar,"[")!=-1&&StringEndsWith(hsmc->memorymanagmentlibrary,nameVar,"]")) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,nameVar,"[");
                if (DictElementIndexByKey(hsmc->variables,spliti->arr[0])==-1) {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"Variable Not Found");
                } else {
                    err=DictElementFindByKey(hsmc->variables,spliti->arr[0])->value;
                    if (err->type!=HalSMVariableType_HalSMArray) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,"This is not Array");
                    } else {
                        joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),"[");
                        HalSMArray* splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        unsigned int indexarr=1;
                        char temparrc;
                        HalSMArray* curArray=(HalSMArray*)err->value;
                        unsigned int string_lenarrayj=string_len(joinSpliti);
                        for (unsigned int indexj=0;indexj<string_lenarrayj;indexj++) {
                            temparrc=joinSpliti[indexj];
                            if (temparrc=='[') {
                                if (indexarr>0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,'['));
                                }
                                indexarr++;
                            } else if (temparrc==']') {
                                if (indexarr==0) {
                                    HalSMCompiler_ThrowError(hsmc,hsmc->line,"The index in array is not completed to the end");
                                } else if (indexarr==1) {
                                    if (indexj==string_lenarrayj-1) {
                                        curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]=arg;
                                    } else {
                                        curArray=(HalSMArray*)curArray->arr[*(int*)HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,splitin),0)->arr[0]->value]->value;
                                    }
                                    
                                    splitin=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,']'));
                                }
                                indexarr--;
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,splitin,HalSMVariable_FromValue(hsmc->memorymanagmentlibrary,temparrc));
                            }
                        }
                    }
                }
            } else {
                PutDictElementToDict(hsmc->memorymanagmentlibrary,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameVar),arg));
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
        } else if (f->type==HalSMVariableType_HalSMReturn) {
            arguss=((HalSMReturn*)f->value)->value;
            for (unsigned int a=0;a<arguss->size;a++) {
                arg=arguss->arr[a];
                if (arg->type==HalSMVariableType_HalSMVar) {
                    if (DictElementIndexByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)arg->value)->name))!=-1) {
                        arguss->arr[a]=DictElementFindByKey(hsmc->variables,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMVar*)arg->value)->name))->value;
                    } else {return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->line,"Variable Not Found"),HalSMVariableType_HalSMError);}
                }
            }
            return arguss->arr[0];
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,&null,HalSMVariableType_HalSMNull);
}

HalSMLocalFunction* HalSMLocalFunction_init(HalSMMemoryManagmentLibrary* hsmmml,char* name,char* args,Dict* vrs)
{
    HalSMLocalFunction* out=hsmmml->malloc(sizeof(HalSMLocalFunction));
    out->name=name;
    out->args=HalSMArray_split_str(hsmmml,args,",");
    if(out->args->size>0 && string_len(*(char**)out->args->arr[0]->value)==0){out->args=HalSMArray_init(hsmmml);}
    out->func=HalSMArray_init(hsmmml);
    out->vars=vrs;
    return out;
}

unsigned char StringCompare(char* c,char* f) {
    if (string_len(c)!=string_len(f)){return 0;}
    unsigned int lc=string_len(c);
    for (unsigned int i=0;i<lc;i++) {
        if (c[i]!=f[i]){return 0;}
    }
    return 1;
}

int StringIndexOf(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f) {
    unsigned int fl=string_len(f);
    unsigned int cl=string_len(c);
    if (fl==0||cl==0||fl==0||cl<fl){return -1;}
    char *fa=hsmmml->malloc(fl+1);
    fa[fl]='\0';
    unsigned int d=cl-fl+1;
    for (unsigned int i=0;i<d;i++) {
        memory_cpy(fa,&c[i],fl);
        if (StringCompare(fa,f)){return i;}
    }
    return -1;
}

int StringLastIndexOf(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f) {
    unsigned int fl=string_len(f);
    unsigned int cl=string_len(c);
    if (fl==0||cl==0||fl==0||fl>=cl){return -1;}
    int out=-1;
    char *fa=hsmmml->malloc(fl+1);
    fa[fl]='\0';
    unsigned int d=cl-fl+1;
    for (unsigned int i=0;i<d;i++) {
        memory_cpy(fa,&c[i],fl);
        if (StringCompare(fa,f)){out=i;}
    }
    return out;
}

unsigned char StringEndsWith(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f)
{
    if (string_len(c)==0||string_len(f)==0) {
        return 0;
    } else if (string_len(c)==string_len(f)) {
        return StringCompare(c,f);
    } else if (string_len(c)<string_len(f)) {
        return 0;
    }
    return StringCompare(SubString(hsmmml,c,string_len(c)-string_len(f),string_len(c)),f);
}

unsigned char StringStartsWith(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f)
{
    if (string_len(c)==0||string_len(f)==0) {
        return 0;
    } else if (string_len(c)==string_len(f)) {
        return StringCompare(c,f);
    } else if (string_len(c)<string_len(f)) {
        return 0;
    }
    return StringCompare(SubString(hsmmml,c,0,string_len(f)),f);
}

char* SubString(HalSMMemoryManagmentLibrary* hsmmml,char* c,int start,int end) {
    int cl=string_len(c);
    if (start>=cl){
        //PRINT ERROR
        return "";
    }
    else if (cl==0||end>cl){return "";}
    int lo=MathMin(end,cl)-start;
    char* out=hsmmml->malloc(lo+1);
    memory_cpy(out,&c[start],lo);
    out[lo]='\0';
    return out;
}

char* ConcatenateStrings(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f) {
    if (string_len(c)==0&&string_len(f)==0){return "";}
    char* out=hsmmml->malloc(string_len(c)+string_len(f)+1);
    string_cpy(out,c);
    string_cat(out,f);
    out[string_len(c)+string_len(f)]='\0';
    return out;
}

char* StringReplace(HalSMMemoryManagmentLibrary* hsmmml,char* c,char* f,char* r) {
    unsigned int lf=string_len(f);
    unsigned int lr=string_len(r);
    unsigned int cl=string_len(c);
    if(cl<lf||lf==0){return c;}
    else if(cl==0){return "";}
    else if(c==f){return r;}
    char* out=hsmmml->malloc(0);
    unsigned int i=0;
    unsigned int size=0;
    unsigned int d=cl-lf+1;
    while(i<d){
        if(StringCompare(SubString(hsmmml,c,i,i+lf),f)){
            if(lr>0){
                out=hsmmml->realloc(out,(size+lr));
                for (unsigned int j=0;j<lr;j++)
                    out[size+j]=r[j];
                size+=lr;
            }
            i+=lf;
        } else {
            out=hsmmml->realloc(out,(size+1));
            out[size]=c[i];
            size++;
            i++;
        }
    }
    out=hsmmml->realloc(out,size+1);
    out[size]='\0';
    return out;
}

HalSMCalculateVars HalSMCalculateVars_init()
{
    HalSMCalculateVars out;
    out.version="0.0.1";
    out.addStr=HalSMCalculateVars_addStr;
    out.subStr=HalSMCalculateVars_subStr;
    out.mulStr=HalSMCalculateVars_mulStr;
    out.divStr=HalSMCalculateVars_divStr;
    out.addInt=HalSMCalculateVars_addInt;
    out.subInt=HalSMCalculateVars_subInt;
    out.mulInt=HalSMCalculateVars_mulInt;
    out.divInt=HalSMCalculateVars_divInt;
    out.addDouble=HalSMCalculateVars_addDouble;
    out.subDouble=HalSMCalculateVars_subDouble;
    out.mulDouble=HalSMCalculateVars_mulDouble;
    out.divDouble=HalSMCalculateVars_divDouble;
    return out;
}

char* HalSMCalculateVars_addStr(HalSMStringLibrary* hsmsl,HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    return ConcatenateStrings(hsmmml,HalSMVariable_to_str(hsmsl,v0),HalSMVariable_to_str(hsmsl,v1));
}

char* HalSMCalculateVars_subStr(HalSMStringLibrary* hsmsl,HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    char* v0s=HalSMVariable_to_str(hsmsl,v0);
    char* v1s=HalSMVariable_to_str(hsmsl,v1);
    if (string_len(v0s)<string_len(v1s)){return v0s;};
    int v=StringLastIndexOf(hsmmml,v0s,v1s);
    if (v==-1){return v0s;}
    return ConcatenateStrings(hsmmml,SubString(hsmmml,v0s,0,v),SubString(hsmmml,v0s,v+string_len(v1s),string_len(v0s)));
}

char* HalSMCalculateVars_mulStr(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    char* st=*(char**)v0->value;
    int ch;
    HalSMVariable_AsVarAuto(&ch,v1);
    if (ch==0){return "";}
    else if (ch==1){return st;}
    char* out=hsmmml->malloc(string_len(st)*ch+1);
    string_cpy(out,st);
    for (int i=0;i<ch-1;i++) {
        string_cat(out,st);
    }
    out[string_len(st)*ch]='\0';
    return out;
}

char* HalSMCalculateVars_divStr(HalSMStringLibrary* hsmsl,HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v1->type==HalSMVariableType_str){return StringReplace(hsmmml,*(char**)v0->value,*(char**)v1->value,"");}
    char* s0=HalSMVariable_to_str(hsmsl,v0);
    int i1;
    HalSMVariable_AsVarAuto(&i1,v1);
    int ls0=string_len(s0);
    if(ls0==0||i1==0){return "";}
    else if(i1==1){return s0;}
    int lout=string_len(s0)/i1;
    char* out=hsmmml->malloc(lout+1);
    for (int i=0;i<lout;i++) {
        out[i]=s0[i];
    }
    out[lout]='\0';
    return out;
}

int HalSMCalculateVars_addInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    int i0;
    if (v0->type==HalSMVariableType_str) {
        i0=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v0));
    } else {
        i0=*(int*)v0->value;
    }

    int i1;
    if (v1->type==HalSMVariableType_str) {
        i1=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v1));
    } else {
        i1=*(int*)v1->value;
    }
    return i0+i1;
}

int HalSMCalculateVars_subInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    int i0;
    if (v0->type==HalSMVariableType_str) {
        i0=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v0));
    } else {
        i0=*(int*)v0->value;
    }

    int i1;
    if (v1->type==HalSMVariableType_str) {
        i1=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v1));
    } else {
        i1=*(int*)v1->value;
    }
    return i0-i1;
}

int HalSMCalculateVars_mulInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    int i0;
    if (v0->type==HalSMVariableType_str) {
        i0=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v0));
    } else {
        i0=*(int*)v0->value;
    }

    int i1;
    if (v1->type==HalSMVariableType_str) {
        i1=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v1));
    } else {
        i1=*(int*)v1->value;
    }
    return i0*i1;
}

int HalSMCalculateVars_divInt(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    int i0;
    if (v0->type==HalSMVariableType_str) {
        i0=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v0));
    } else {
        i0=*(int*)v0->value;
    }

    int i1;
    if (v1->type==HalSMVariableType_str) {
        i1=(int)hsmsl->ParseDecimal(HalSMVariable_to_str(hsmsl,v1));
    } else {
        i1=*(int*)v1->value;
    }
    return i0/i1;
}

double HalSMCalculateVars_addDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    double f0;
    if (v0->type==HalSMVariableType_str) {
        f0=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v0));
    } else if (v0->type==HalSMVariableType_int) {
        f0=(double)(*(int*)v0->value);
    } else {
        f0=*(double*)v0->value;
    }

    double f1;
    if (v1->type==HalSMVariableType_str) {
        f1=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v1));
    } else if (v1->type==HalSMVariableType_int) {
        f1=(double)(*(int*)v1->value);
    } else {
        f1=*(double*)v1->value;
    }
    return f0+f1;
}

double HalSMCalculateVars_subDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    double f0;
    if (v0->type==HalSMVariableType_str) {
        f0=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v0));
    } else if (v0->type==HalSMVariableType_int) {
        f0=(double)(*(int*)v0->value);
    } else {
        f0=*(double*)v0->value;
    }

    double f1;
    if (v1->type==HalSMVariableType_str) {
        f1=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v1));
    } else if (v1->type==HalSMVariableType_int) {
        f1=(double)(*(int*)v1->value);
    } else {
        f1=*(double*)v1->value;
    }
    return f0-f1;
}
double HalSMCalculateVars_mulDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    double f0;
    if (v0->type==HalSMVariableType_str) {
        f0=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v0));
    } else if (v0->type==HalSMVariableType_int) {
        f0=(double)(*(int*)v0->value);
    } else {
        f0=*(double*)v0->value;
    }

    double f1;
    if (v1->type==HalSMVariableType_str) {
        f1=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v1));
    } else if (v1->type==HalSMVariableType_int) {
        f1=(double)(*(int*)v1->value);
    } else {
        f1=*(double*)v1->value;
    }
    return f0*f1;
}
double HalSMCalculateVars_divDouble(HalSMStringLibrary* hsmsl,HalSMVariable* v0,HalSMVariable* v1)
{
    double f0;
    if (v0->type==HalSMVariableType_str) {
        f0=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v0));
    } else if (v0->type==HalSMVariableType_int) {
        f0=(double)(*(int*)v0->value);
    } else {
        f0=*(double*)v0->value;
    }

    double f1;
    if (v1->type==HalSMVariableType_str) {
        f1=(double)hsmsl->ParseDouble(HalSMVariable_to_str(hsmsl,v1));
    } else if (v1->type==HalSMVariableType_int) {
        f1=(double)(*(int*)v1->value);
    } else {
        f1=*(double*)v1->value;
    }
    return f0/f1;
}



HalSMInteger HalSMInteger_FromSignedInteger(signed int value)
{
    HalSMInteger out;
    /*out.size=value<128&&value>-129?1:(value<32768&&value>-32769?2:(value<8388608&&value>-8388609?3:4));
    out.value=hsmc->memorymanagmentlibrary->malloc(out.size);
    unsigned char* temp=(unsigned char*)&value;
    for (unsigned int i=out.size-1;i<out.size;i++){
        out.value[i]=temp[i];
    }*/
    return out;
}

HalSMInteger HalSMInteger_Add(HalSMInteger a,HalSMInteger b)
{
    HalSMInteger out;
    return out;
}

HalSMInteger HalSMInteger_Sub(HalSMInteger a,HalSMInteger b)
{
    HalSMInteger out;
    return out;
}

char* HalSMInteger_toString(HalSMInteger a)
{
    //char* out=calloc(a.size*2+1,sizeof(char));
    return "";
}

HalSMFunctionArray* HalSMFunctionArray_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* args,HalSMFunctionArrayType type)
{
    HalSMFunctionArray* out=hsmmml->malloc(sizeof(HalSMFunctionArray));
    out->args=args;
    out->type=type;
    return out;
}

unsigned int string_len(char* s)
{
    unsigned int out=0;
    while (*s!='\0') {
        out++;
        s++;
    }
    return out;
}

char* string_cat(char* s1,const char* s2)
{
    if ((s1==NOTHALSMNULLPOINTER)&&(s2==NOTHALSMNULLPOINTER))return NOTHALSMNULLPOINTER;
    char* ptr=s1;
    while (*ptr!='\0') ptr++;
    while (*s2!='\0') *ptr++=*s2++;
    *ptr='\0';
    return s1;
}

char* string_cpy(char* s1,const char* s2)
{
    if ((s1==NOTHALSMNULLPOINTER)&&(s2==NOTHALSMNULLPOINTER))return NOTHALSMNULLPOINTER;
    
    char* ptr=s1;
    while (*s2!='\0')
    {
        *s1=*s2;
        s1++;
        s2++;
    }
    *s1='\0';
    return ptr;
}

char* stringncpy(char* s1,const char* s2,unsigned int n)
{
    if ((s1==NOTHALSMNULLPOINTER)&&(s2==NOTHALSMNULLPOINTER))return NOTHALSMNULLPOINTER;
    char* ptr=s1;

    while (*s2&&n--)
    {
        *s1=*s2;
        s1++;
        s2++;
    }

    *s1='\0';
    return ptr;
}

void* memory_cpy(void* dst,const void* src,unsigned int n)
{
    char* bdst=(char*)dst;
    const char* bsrc=(const char*)src;
    if ((bdst!=NOTHALSMNULLPOINTER)&&(bsrc!=NOTHALSMNULLPOINTER)) {
        while (n)
        {
            *(bdst++)=*(bsrc++);
            --n;
        }
    }
    return dst;
}