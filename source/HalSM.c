#include <HalSM.h>

HalSMNull hnull;
HalSMVariable null={&hnull,HalSMVariableType_HalSMNull};
HalSMPlus plus;
HalSMMinus minus;
HalSMMult mult;
HalSMDivide divide;
HalSMPow hpow;
HalSMModulo modulo;
HalSMEqual equal;
HalSMNotEqual notequal;
HalSMMore more;
HalSMLess less;
HalSMMoreEqual moreequal;
HalSMLessEqual lessequal;
HalSMAND and;
HalSMOR or;
HalSMNegate negate;
HalSMVariable vnegate={&negate,HalSMVariableType_HalSMNegate};
HalSMNegativeSign negativesign;
HalSMVariable vnegativesign={&negativesign,HalSMVariableType_HalSMNegativeSign};
HalSMShift shift_left={0};
HalSMShift shift_right={1};
HalSMArray* arrInt;
unsigned char boolTrueVal=1;
HalSMVariable boolTrue={&boolTrueVal,HalSMVariableType_HalSMBool};
unsigned char boolFalseVal=0;
HalSMVariable boolFalse={&boolFalseVal,HalSMVariableType_HalSMBool};
HalSMInlineIf inlineif;
HalSMInlineElse inlineelse;
HalSMBreak hbreak;
HalSMReturn hreturn;
HalSMVoid hsmvoid;
HalSMVariable hvoid={&hsmvoid,HalSMVariableType_void};
HalSMXOR hxor;
HalSMBAND hband;
HalSMBOR hbor;
HalSMBNOT hbnot;
HalSMVariable vxor={&hxor,HalSMVariableType_HalSMXOR};
HalSMVariable vband={&hband,HalSMVariableType_HalSMBAND};
HalSMVariable vbor={&hbor,HalSMVariableType_HalSMBOR};
HalSMVariable vbnot={&hbnot,HalSMVariableType_HalSMBNOT};

HalSMInteger HalSMIntegerZero={0,(unsigned char[]){0},1};
HalSMInteger HalSMIntegerOne={0,(unsigned char[]){1},1};
HalSMInteger HalSMIntegerTwo={0,(unsigned char[]){2},1};
HalSMInteger HalSMIntegerThree={0,(unsigned char[]){3},1};
HalSMInteger HalSMIntegerTen={0,(unsigned char[]){10},1};
HalSMInteger HalSMIntegerThousand={0,(unsigned char[]){3,232}};

HalSMDouble HalSMDoubleOne={0,&HalSMIntegerOne,&HalSMIntegerOne};
HalSMDouble HalSMDoubleZero={0,&HalSMIntegerZero,&HalSMIntegerZero};
HalSMDouble HalSMDoubleTwo={0,&HalSMIntegerTwo,&HalSMIntegerOne};
HalSMDouble HalSMDoubleThree={0,&HalSMIntegerThree,&HalSMIntegerOne};
HalSMDouble HalSMDoubleTen={0,&HalSMIntegerTen,&HalSMIntegerOne};

Dict NULLDict={0,NOTHALSMNULLPOINTER};
HalSMArray NULLArray={0,NOTHALSMNULLPOINTER};
HalSMVariable HNULLArray={&NULLArray,HalSMVariableType_HalSMArray};

HalSMNull* HalSMNull_init(HalSMCompiler* hsmc) {
    HalSMNull* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMNull));
    return out;
}

HalSMError* HalSMError_init(HalSMCompiler* hsmc,HalSMErrorIndificator* ind,HalStringFormatChar* error){
    HalSMError* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMError));
    out->ind=ind;
    out->error=error;
    return out;
}

HalSMErrorIndificator* HalSMErrorIndificator_init(HalSMCompiler* hsmc,HalStringFormatChar* name,unsigned long long int line,void* prev)
{
    HalSMErrorIndificator* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMErrorIndificator));
    out->line=line;
    out->name=name;
    out->prev=prev;
    return out;
}

//FILE SYSTEM

HalSMVariable* HalSMCompiler_isExistsPath(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    return HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->isExistsDir(*(HalStringFormatChar**)args->arr[0]->value)||hsmc->filesystemlibrary->isExistsFile(*(HalStringFormatChar**)args->arr[0]->value));
}

HalSMVariable* HalSMCompiler_isExistsFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    return HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->isExistsFile(*(HalStringFormatChar**)args->arr[0]->value));
}

HalSMVariable* HalSMCompiler_isExistsDir(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    return HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->isExistsDir(*(HalStringFormatChar**)args->arr[0]->value));
}

HalSMVariable* HalSMCompiler_getListFiles(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->getListFiles(hsmc,*(HalStringFormatChar**)args->arr[0]->value),HalSMVariableType_HalSMArray);
}

HalSMVariable* HalSMCompiler_openFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==2) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVariableCustom_init(hsmc->memorymanagmentlibrary,(HalSMVariableTypeCustom*)hsmc->sys_custom_variables->elements[0]->value->value,hsmc->filesystemlibrary->openFile(*(HalStringFormatChar**)args->arr[0]->value,*(HalStringFormatChar**)args->arr[1]->value),&NULLDict,&NULLDict),HalSMVariableType_custom);
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_closeFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    hsmc->filesystemlibrary->closeFile(((HalSMVariableCustom*)args->arr[0]->value)->value);
    return &hvoid;
}

HalSMVariable* HalSMCompiler_readFile(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    unsigned long long int count=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
    unsigned long long int size=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[2]->value);
    HalStringFormatChar* out=hsmc->memorymanagmentlibrary->malloc(count*size*sizeof(HalStringFormatChar));
    unsigned long long int ncount=hsmc->filesystemlibrary->read(out,size,count,((HalSMVariableCustom*)args->arr[0]->value)->value);
    if (count>ncount) {
        out=hsmc->memorymanagmentlibrary->realloc(out,ncount*size);
    }
    HalStringFormatChar** o=hsmc->memorymanagmentlibrary->malloc(sizeof(HalStringFormatChar*));
    *o=out;
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,o,HalSMVariableType_str);
}

HalSMVariable* HalSMCompiler_puts(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    hsmc->filesystemlibrary->puts(((HalSMVariableCustom*)args->arr[0]->value)->value,*(HalStringFormatChar**)args->arr[1]->value);
    return &hvoid;
}

HalSMVariable* HalSMCompiler_seek(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    hsmc->filesystemlibrary->seek(((HalSMVariableCustom*)args->arr[0]->value)->value,HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value),HalSMInteger_ToUnsignedInteger((HalSMInteger*)args->arr[2]->value));
    return &hvoid;
}

HalSMVariable* HalSMCompiler_tell(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->tell(((HalSMVariableCustom*)args->arr[0]->value)->value)),HalSMVariableType_int);
}

HalSMVariable* HalSMCompiler_rewind(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    hsmc->filesystemlibrary->rewind(((HalSMVariableCustom*)args->arr[0]->value)->value);
    return &hvoid;
}
HalSMVariable* HalSMCompiler_flush(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    hsmc->filesystemlibrary->flush(((HalSMVariableCustom*)args->arr[0]->value)->value);
    return &hvoid;
}

//FILE SYSTEM

HalSMVariable* HalSMCompiler_input(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    HalStringFormatChar* text=*(HalStringFormatChar**)args->arr[0]->value;
    return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->inputf(text));
}

HalSMVariable* HalSMCompiler_reversed(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    HalSMVariable* arr=args->arr[0];
    if (arr->type==HalSMVariableType_HalSMArray) {
        HalSMArray* out=HalSMArray_reverse(hsmc->memorymanagmentlibrary,(HalSMArray*)arr->value);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
    } else if (arr->type==HalSMVariableType_str) {
        HalStringFormatChar* arrv=*(HalStringFormatChar**)arr->value;
        unsigned long long int arrl=string_len(arrv);
        if (arrl==0) {
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H(""));
        }
        HalStringFormatChar* out=hsmc->memorymanagmentlibrary->malloc((arrl+1)*sizeof(HalStringFormatChar));
        long long int b=0;
        for (unsigned long long int i=arrl;i--;) {
            out[b]=arrv[i];
            if (i==0) {break;}
            b++;
        }
        out[arrl]=ASCIIH('\0');
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
    }
    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("This Variable not list or string")));
    return &hvoid;
}

HalSMVariable* HalSMCompiler_range(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==0) {return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->ind,U8H("Range cannot be without numeric args")),HalSMVariableType_HalSMError);}
    /*double start,end,step;
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
    for (unsigned long long int i=0;i<out->size;i++) {
        p[i]=start+i*step;
        t[i].type=HalSMVariableType_double;
        t[i].value=&p[i];
        out->arr[i]=&t[i];
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);*/
    return &null;
}

HalSMArray* HalSMCompiler_get_print_text(HalSMCompiler* hsmc,HalSMArray* args,unsigned char isToPrint) {
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalStringFormatChar* c;
    HalSMVariable* a;
    for (unsigned long long int i=0;i<args->size;i++) {
        a=args->arr[i];
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMVariable_to_str(hsmc,a,isToPrint)));
    }
    return out;
}

HalSMVariable* HalSMCompiler_print(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    HalSMArray* temp=HalSMCompiler_get_print_text(hsmc,args,0);
    hsmc->print(HalSMArray_join_str(hsmc->memorymanagmentlibrary,temp,U8H(" ")));
    for (unsigned long long int i=0;i<temp->size;i++) {
        hsmc->memorymanagmentlibrary->free(*(HalStringFormatChar**)temp->arr[i]->value);
        hsmc->memorymanagmentlibrary->free(temp->arr[i]->value);
        hsmc->memorymanagmentlibrary->free(temp->arr[i]);
    }
    hsmc->memorymanagmentlibrary->free(temp->arr);
    hsmc->memorymanagmentlibrary->free(temp);
    return &hvoid;
}

HalSMVariable* HalSMCompiler_exit(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    hsmc->systemlibrary->exit(0);
    return &hvoid;
}

HalSMVariable* HalSMCompiler_for(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size!=2||args->arr[0]->type!=HalSMVariableType_str||args->arr[1]->type!=HalSMVariableType_HalSMLocalFunction) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Arguments")),HalSMVariableType_HalSMError);
    }
    HalSMArray* arguments=HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value,U8H(";"));
    if (arguments->size!=3){return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Count Arguments")),HalSMVariableType_HalSMError);}
    HalSMArray* variable=HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)arguments->arr[0]->value,U8H("="));
    HalSMVariable* value;
    
    long long int index=DictElementIndexByKey(hsmc,vars,variable->arr[0]);
    if (index!=-1) {
        value=vars->elements[index]->value;
        HalSMVariable* v=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)variable->arr[1]->value,vars)->arr[0];
        value->value=v->value;
        value->type=v->type;
    } else {
        index=DictElementIndexByKey(hsmc,hsmc->variables,variable->arr[0]);
        if (index!=-1) {
            value=hsmc->variables->elements[index]->value;
            HalSMVariable* v=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)variable->arr[1]->value,vars)->arr[0];
            value->value=v->value;
            value->type=v->type;
        } else if (vars->elements!=NOTHALSMNULLPOINTER) {
            HalSMVariable* v=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)variable->arr[1]->value,vars)->arr[0];
            value=HalSMVariable_init(hsmc->memorymanagmentlibrary,v->value,v->type);
            DictElement* var=DictElementInit(hsmc->memorymanagmentlibrary,variable->arr[0],value);
            PutDictElementToDict(hsmc,vars,var);
        } else {
            HalSMVariable* v=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)variable->arr[1]->value,vars)->arr[0];
            value=HalSMVariable_init(hsmc->memorymanagmentlibrary,v->value,v->type);
            DictElement* var=DictElementInit(hsmc->memorymanagmentlibrary,variable->arr[0],value);
            PutDictElementToDict(hsmc,hsmc->variables,var);
        }
    }
    
    HalSMLocalFunction* lf=args->arr[1]->value;
    lf->name=U8H("for");
    HalSMVariable* err;
    while (1) {
        if (HalSMCompiler_getBool(hsmc,HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)arguments->arr[1]->value,vars)->arr[0])) {
            err=HalSMLocalFunction_run(lf,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
            if (err->type==HalSMVariableType_HalSMError){return err;}
            else if(err->type==HalSMVariableType_HalSMBreak){return &hvoid;}
            else if(err->type==HalSMVariableType_HalSMReturn) {
                if (((HalSMReturn*)err->value)->idLocalFunction==lf->id){return &hvoid;}
                return err;
            }
            //value=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)resRunFunc->args->arr[1]->value,vars)->arr[0];
            HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)arguments->arr[2]->value,vars);
        } else {break;}
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_while(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size!=2||args->arr[0]->type!=HalSMVariableType_str||args->arr[1]->type!=HalSMVariableType_HalSMLocalFunction) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Arguments")),HalSMVariableType_HalSMError);
    }
    HalSMLocalFunction* lf=args->arr[1]->value;
    HalSMVariable* err;
    
    lf->name=U8H("while");

    while (1) {
        if (HalSMCompiler_getBool(hsmc,HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)args->arr[0]->value,vars)->arr[0])) {
            err=HalSMLocalFunction_run(lf,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
            if (err->type==HalSMVariableType_HalSMError){return err;}
            else if(err->type==HalSMVariableType_HalSMBreak){return &hvoid;}
            else if(err->type==HalSMVariableType_HalSMReturn) {
                if (((HalSMReturn*)err->value)->idLocalFunction==lf->id){return &hvoid;}
                return err;
            }
        } else {break;}
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_if(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size>3||args->size<2||args->arr[0]->type!=HalSMVariableType_str||args->arr[1]->type!=HalSMVariableType_HalSMLocalFunction) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Arguments")),HalSMVariableType_HalSMError);
    }
    HalSMLocalFunction* lf=args->arr[1]->value;
    lf->name=U8H("if");
    HalSMVariable* err;
    if (HalSMCompiler_getBool(hsmc,HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)args->arr[0]->value,vars)->arr[0])) {
        err=HalSMLocalFunction_run(lf,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
        if (err->type==HalSMVariableType_HalSMError){return err;}
        else if(err->type==HalSMVariableType_HalSMBreak){return err;}
        else if(err->type==HalSMVariableType_HalSMReturn) {
            if (((HalSMReturn*)err->value)->idLocalFunction==lf->id){return &hvoid;}
            return err;
        }
        return err;
    } else if (args->size==3) {
        HalSMArray* logicFunctions=args->arr[2]->value;
        HalSMArray* logicTemp;
        for (int i=0;i<logicFunctions->size;i++) {
            logicTemp=(HalSMArray*)logicFunctions->arr[i]->value;
            err=HalSMFunctionC_run((HalSMFunctionC*)logicTemp->arr[0]->value,(HalSMArray*)logicTemp->arr[1]->value,vars);
            if (err->type==HalSMVariableType_HalSMBool&&(*(unsigned char*)err->value)==1) {
                break;
            } else if (err->type==HalSMVariableType_HalSMError||err->type==HalSMVariableType_HalSMBreak||err->type==HalSMVariableType_HalSMReturn) {
                return err;
            }
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_elif(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size!=2||args->arr[0]->type!=HalSMVariableType_str||args->arr[1]->type!=HalSMVariableType_HalSMLocalFunction) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Arguments")),HalSMVariableType_HalSMError);
    }
    HalSMLocalFunction* lf=args->arr[1]->value;
    lf->name=U8H("elif");
    HalSMVariable* err;
    if (HalSMCompiler_getBool(hsmc,HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)args->arr[0]->value,vars)->arr[0])) {
        err=HalSMLocalFunction_run(lf,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
        if (err->type==HalSMVariableType_HalSMError){return err;}
        else if(err->type==HalSMVariableType_HalSMBreak){return err;}
        else if(err->type==HalSMVariableType_HalSMReturn) {
            if (((HalSMReturn*)err->value)->idLocalFunction==lf->id){return &boolTrue;}
            return err;
        }
        return &boolTrue;
    }
    return &boolFalse;
}

HalSMVariable* HalSMCompiler_else(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size!=2||args->arr[0]->type!=HalSMVariableType_str||args->arr[1]->type!=HalSMVariableType_HalSMLocalFunction) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Arguments")),HalSMVariableType_HalSMError);
    }
    HalSMLocalFunction* lf=args->arr[1]->value;
    lf->name=U8H("else");
    HalSMVariable* err=HalSMLocalFunction_run(lf,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
    if (err->type==HalSMVariableType_HalSMError){return err;}
    else if(err->type==HalSMVariableType_HalSMBreak){return err;}
    else if(err->type==HalSMVariableType_HalSMReturn) {
        if (((HalSMReturn*)err->value)->idLocalFunction==lf->id){return &boolTrue;}
        return err;
    }
    return &boolTrue;
}

HalSMVariable* HalSMCompiler_do(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size>3||args->size<2||args->arr[0]->type!=HalSMVariableType_str||args->arr[1]->type!=HalSMVariableType_HalSMLocalFunction) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Not Wrong Arguments")),HalSMVariableType_HalSMError);
    }
    HalSMLocalFunction* lf=args->arr[1]->value;
    lf->name=U8H("do");
    HalSMVariable* err=HalSMLocalFunction_run(lf,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
    if (err->type==HalSMVariableType_HalSMError){return err;}
    else if(err->type==HalSMVariableType_HalSMBreak){return err;}
    else if(err->type==HalSMVariableType_HalSMReturn) {
        if (((HalSMReturn*)err->value)->idLocalFunction==lf->id){return &hvoid;}
        return err;
    }
    if (args->size==3) {
        HalSMArray* logicFunctions=args->arr[2]->value;
        HalSMArray* logicTemp;
        HalSMVariable* res;
        if (logicFunctions->size>0) {
            logicTemp=(HalSMArray*)logicFunctions->arr[0]->value;
            HalSMArray* logicArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
            HalSMArray_add(hsmc->memorymanagmentlibrary,logicArgs,((HalSMArray*)logicTemp->arr[1]->value)->arr[0]);
            HalSMArray_add(hsmc->memorymanagmentlibrary,logicArgs,args->arr[1]);
            res=HalSMFunctionC_run((HalSMFunctionC*)logicTemp->arr[0]->value,logicArgs,vars);
            if (res->type==HalSMVariableType_HalSMError) {
                return res;
            } else if(res->type==HalSMVariableType_HalSMBreak){return res;}
            else if(res->type==HalSMVariableType_HalSMReturn){return res;}
        }
    }
    return &hvoid;
}



HalSMCompiler* HalSMCompiler_init(HalStringFormatChar* path,void(*print)(HalStringFormatChar*),void(*printErrorf)(HalStringFormatChar*),HalStringFormatChar*(*inputf)(HalStringFormatChar*),HalSMVariable*(*getVariable)(HalStringFormatChar*),HalSMStringLibrary* stringlibrary,HalSMMemoryManagmentLibrary* memorymanagmentlibrary,HalSMSystemLibrary* systemlibrary,HalSMFileSystemLibrary* filesystemlibrary,HalSMVariable*(*HalSMLoadModule)(HalSMCompiler*,HalStringFormatChar*))
{
    HalSMCompiler* hsmc=memorymanagmentlibrary->malloc(sizeof(HalSMCompiler));
    hsmc->versionName=U8H("0.1.6 (Errors, errors and errors)");
    hsmc->version=16L;
    hsmc->path=path;
    hsmc->print=print;
    hsmc->printErrorf=printErrorf;
    hsmc->inputf=inputf;
    hsmc->getVariable=getVariable;
    hsmc->stringlibrary=stringlibrary;
    hsmc->memorymanagmentlibrary=memorymanagmentlibrary;
    hsmc->systemlibrary=systemlibrary;
    hsmc->filesystemlibrary=filesystemlibrary;
    hsmc->functions=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("input")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_input),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("reversed")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_reversed),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("range")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_range),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("print")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_print),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("exit")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_exit),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("for")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_for),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("while")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_while),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("if")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_if),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("elif")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_elif),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("else")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_else),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("increment")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_increment),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("decrement")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_decrement),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("do")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_do),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("replace")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_replace),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("index")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_index),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("rindex")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_rindex),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("split")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_split),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("substring")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_substring),HalSMVariableType_HalSMFunctionC)),
        
        //ARRAY
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("append")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_append),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("len")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_len),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("clear")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_clear),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("insert")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_insert),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("pop")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_pop),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("remove")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_remove),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("reverse")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_reverse),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("copy")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_copy),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("appendArray")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_appendArray),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("slice")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_slice),HalSMVariableType_HalSMFunctionC)), 

        //DICT
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("appendDict")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_appendDict),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("indexByKey")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_indexByKey),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("indexByValue")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_indexByValue),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("set")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_set),HalSMVariableType_HalSMFunctionC)),

        //Math
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("sqrt")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_sqrt),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("int")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_int),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("double")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_double),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("str")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_str),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("floor")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_floor),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("ceil")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_ceil),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("round")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_round),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("abs")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_abs),HalSMVariableType_HalSMFunctionC)),

        //String
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("join")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_join),HalSMVariableType_HalSMFunctionC)),

        //File
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("fopen")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_openFile),HalSMVariableType_HalSMFunctionC)),

        //Convert
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("array")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_array),HalSMVariableType_HalSMFunctionC)),

        //System
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("dir")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_dir),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("keyByIndex")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_keyByIndex),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("valueByIndex")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_valueByIndex),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("dict")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_dict),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("pow")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_powFunc),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("bin")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_bin),HalSMVariableType_HalSMFunctionC)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("hex")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HalSMCompiler_hex),HalSMVariableType_HalSMFunctionC)),
    },50);

    hsmc->sys_modules=DictInitWithElements(hsmc->memorymanagmentlibrary,hsmc->memorymanagmentlibrary->malloc(0),0);
    hsmc->calcVars=HalSMCalculateVars_init();
    hsmc->line=1;
    hsmc->sys_variables=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("copyright")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("Copyright (C) 2022 Halwarsing.net")))
    },1);
    hsmc->variables=DictCopy(hsmc->memorymanagmentlibrary,hsmc->sys_variables);
    hsmc->modules=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->localFunctions=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->classes=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->groupFunctions=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
        DictElementInit(hsmc->memorymanagmentlibrary,
            HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("if")),
            HalSMVariable_init(hsmc->memorymanagmentlibrary,
                HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){
                    hsmc->functions->elements[8]->value,
                    hsmc->functions->elements[9]->value
                },2),
                HalSMVariableType_HalSMArray
            )
        ),
        DictElementInit(hsmc->memorymanagmentlibrary,
            HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("do")),
            HalSMVariable_init(hsmc->memorymanagmentlibrary,
                HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){
                    hsmc->functions->elements[6]->value,
                    hsmc->functions->elements[5]->value
                },2),
                HalSMVariableType_HalSMArray
            )
        )
    },2);

    hsmc->sys_custom_variables=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("FilePointer")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVariableTypeCustom_init(hsmc->memorymanagmentlibrary,(HalSMVariableTypeCustom){
            U8H("FilePointer"),
            HFP_toString,
            HFP_add,
            HFP_sub,
            HFP_mul,
            HFP_div,
            HFP_pow,
            HFP_modulo,
            HFP_increment,
            HFP_decrement,
            HFP_isEqual,
            HFP_isMore,
            HFP_isLess,
            HFP_getBool,
            HFP_indexVariableByName,
            HFP_indexFunctionByName,
            HFP_getVariableByName,
            HFP_getFunctionByName,
            HFP_getItem,
            HFP_shiftRight,
            HFP_shiftLeft,
            HFP_convert,
            HFP_xor,
            HFP_and,
            HFP_or,
            HFP_bnot,
            DictInit(hsmc->memorymanagmentlibrary),
            DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("close")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_close),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("read")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_read),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("write")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_write),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("eof")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_eof),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("seek")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_seek),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("tell")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_tell),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("rewind")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_rewind),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("flush")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_flush),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("readUTF8")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_readUTF8),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("writeUTF8")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_writeUTF8),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("readHSF")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_readHSF),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("writeHSF")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_writeHSF),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("readUTF16")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_readUTF16),HalSMVariableType_HalSMFunctionC)),
                DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("writeUTF16")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMFunctionC_init(hsmc,&HFP_writeUTF16),HalSMVariableType_HalSMFunctionC)),
            },14)
        }),HalSMVariableType_custom)),
        DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("ByteArray")),HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVariableTypeCustom_init(hsmc->memorymanagmentlibrary,(HalSMVariableTypeCustom){
            U8H("ByteArray"),
            HBA_toString,
            HBA_add,
            HBA_sub,
            HBA_mul,
            HBA_div,
            HBA_pow,
            HBA_modulo,
            HBA_increment,
            HBA_decrement,
            HBA_isEqual,
            HBA_isMore,
            HBA_isLess,
            HBA_getBool,
            HBA_indexVariableByName,
            HBA_indexFunctionByName,
            HBA_getVariableByName,
            HBA_getFunctionByName,
            HBA_getItem,
            HBA_shiftRight,
            HBA_shiftLeft,
            HBA_convert,
            HBA_xor,
            HBA_and,
            HBA_or,
            HBA_bnot,
            DictInit(hsmc->memorymanagmentlibrary),
            DictInit(hsmc->memorymanagmentlibrary)
        }),HalSMVariableType_custom)),
    },2);
    hsmc->GOTOSectors=DictInit(hsmc->memorymanagmentlibrary);
    hsmc->HalSMLoadModule=HalSMLoadModule;
    hsmc->ind=HalSMErrorIndificator_init(hsmc,U8H("MAIN"),0,NOTHALSMNULLPOINTER);
    hsmc->curInd=hsmc->ind;
    return hsmc;
}

unsigned long long int HalSMCompiler_getTabs(HalSMCompiler* hsmc,HalStringFormatChar* l)
{
    long long int s=string_len(l);
    for (int i=0;i<s;i++) {
        if (HSFCharCompare(l[i],ASCIIH(' '))==0)return i;
    }
    return s;
}

unsigned long long int HalSMCompiler_getIndexTabs(unsigned long long int* tabsArr,unsigned long long int tabsS,unsigned long long int tabsC)
{
    unsigned long long int i;
    for (i=0;i<tabsS;i++) {
        if (tabsC<=tabsArr[i])return i;
    }
    return i;
}

HalSMArray* HalSMCompiler_getLines(HalSMCompiler* hsmc,HalStringFormatChar* text)
{
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalStringFormatChar* o=hsmc->memorymanagmentlibrary->malloc(0);
    unsigned long long s=0;
    HalStringFormatChar isS=ASCIIH('n');
    unsigned long long int isF=0,isA=0,isD=0;
    HalStringFormatChar i;
    unsigned long long int lt=string_len(text);
    unsigned long long int tabsC,tabsI,tabsS=0,tabsF=0;
    unsigned long long d;
    unsigned long long int* tabsArr=hsmc->memorymanagmentlibrary->malloc(0);
    unsigned long long int lti=lt-1;
    unsigned char isComment=0;

    for (d=0;d<lt;d++) {
        i=text[d];
        if (HSFCharCompare(i,ASCIIH('\r'))) {

        } else if (HSFCharCompare(isS,ASCIIH('n'))==0) {
            if (HSFCharCompare(i,isS)) {
                isS=ASCIIH('n');
            }
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (isF) {
            if (HSFCharCompare(i,ASCIIH(')'))) {
                isF--;
            } else if (HSFCharCompare(i,ASCIIH('('))) {
                isF++;
            }
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (isA) {
            if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
            } else if (HSFCharCompare(i,ASCIIH('['))) {
                isA++;
            }
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (isD) {
            if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
            } else if (HSFCharCompare(i,ASCIIH('['))) {
                isD++;
            }
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (HSFCharCompare(i,ASCIIH('\n'))||HSFCharCompare(i,ASCIIH(';'))) {
            if (isComment){isComment=0;}
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=ASCIIH('\0');
            if (HalSMCompiler_isNull(o)==0) {
                tabsC=HalSMCompiler_getTabs(hsmc,o);
                tabsI=HalSMCompiler_getIndexTabs(tabsArr,tabsS,tabsC);
                tabsS=tabsI+1;
                tabsArr=hsmc->memorymanagmentlibrary->realloc(tabsArr,tabsS*sizeof(unsigned long long int));
                tabsArr[tabsI]=tabsC;
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,o,tabsC,string_len(o))),HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsI)},2),HalSMVariableType_HalSMArray));
            }
            s=0;
            o=hsmc->memorymanagmentlibrary->malloc(0);
        } else if (isComment) {

        } else if (HSFCharCompare(i,ASCIIH('#'))&&s==0) {
            isComment=1;
        } else if (HSFCharCompare(i,ASCIIH('\\'))) {
            if (d<lti) {
                d++;
                s+=2;
                o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
                o[s-2]=i;
                o[s-1]=text[d+1];
            } else {
                s++;
                o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
                o[s-1]=i;
            }
        } else if (HSFCharCompare(i,ASCIIH('"'))||HSFCharCompare(i,ASCIIH('\''))) {
            isS=i;
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (HSFCharCompare(i,ASCIIH('('))) {
            isF++;
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            isA++;
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            isD++;
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        } else {
            s++;
            o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
            o[s-1]=i;
        }
    }
    if (s>0) {
        s++;
        o=hsmc->memorymanagmentlibrary->realloc(o,s*sizeof(HalStringFormatChar));
        o[s-1]=ASCIIH('\0');
        if (HalSMCompiler_isNull(o)==0) {
            tabsC=HalSMCompiler_getTabs(hsmc,o);
            tabsI=HalSMCompiler_getIndexTabs(tabsArr,tabsS,tabsC);
            if (tabsC<tabsF){tabsI--;}
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,o,tabsC,string_len(o))),HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsI)},2),HalSMVariableType_HalSMArray));
        }
        s=0;
    }
    hsmc->memorymanagmentlibrary->free(o);
    return out;
}

HalStringFormatChar* HalSMCompiler_removeSpaces(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* text)
{
    unsigned long long int l,i,s;
    l=string_len(text);
    if (l==0){return text;}
    HalStringFormatChar* out=hsmmml->malloc(0);
    HalStringFormatChar space=ASCIIH(' ');
    for (i=0,s=0;i<l;i++) {
        if (HSFCharCompare(text[i],space)==0) {
            s++;
            out=hsmmml->realloc(out,s*sizeof(HalStringFormatChar));
            out[s-1]=text[i];
        }
    }
    s++;
    out=hsmmml->realloc(out,s*sizeof(HalStringFormatChar));
    out[s-1]=ASCIIH('\0');
    return out;
}

void HalSMCompiler_ThrowError(HalSMCompiler* hsmc,HalSMError* err)
{
    HalStringFormatChar* sl;
    //=hsmc->stringlibrary->Decimal2Str(line);
    HalStringFormatChar* out=U8H("");
    //string_cpy(out,U8H("Error at line "));
    //string_cat(out,sl);
    HalSMErrorIndificator* ind=err->ind;
    while (1) {
        sl=HalSMInteger_toString(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,ind->line+1),0);
        out=ConcatenateStrings(hsmc->memorymanagmentlibrary,out,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("["),ConcatenateStrings(hsmc->memorymanagmentlibrary,ind->name,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("] at line "),ConcatenateStrings(hsmc->memorymanagmentlibrary,sl,U8H("\n"))))));
        if (ind->prev==NOTHALSMNULLPOINTER){break;}
        ind=(HalSMErrorIndificator*)ind->prev;
    }
    out=ConcatenateStrings(hsmc->memorymanagmentlibrary,out,U8H("Error: "));
    out=ConcatenateStrings(hsmc->memorymanagmentlibrary,out,err->error);
    hsmc->printErrorf(out);
}

unsigned char HalSMCompiler_isGet(HalSMCompiler* hsmc,HalStringFormatChar* l,HalSMVariable* outvar,unsigned char isNewVar,Dict* vars)
{
    unsigned char out;
    HalSMVariable* outv=HalSMCompiler_isGetU(hsmc,l,&out,isNewVar,vars);
    outvar->value=outv->value;
    outvar->type=outv->type;
    return out;
}

HalSMVariable* HalSMCompiler_isGetU(HalSMCompiler* hsmc,HalStringFormatChar* l,unsigned char* isOut,unsigned char isNewVar,Dict* vars)
{
    HalSMVariable* out=&null;
    unsigned long long int isInd=0;
    unsigned long long int isA=0;
    unsigned long long int isD=0;
    HalSMArray* o=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMVariable* module=&null;
    unsigned long long int indF=0;
    HalStringFormatChar isCovichki=ASCIIH('n');
    unsigned long long int index=0;
    unsigned long long int lindex=string_len(l)-1;
    HalStringFormatChar i;
    HalSMVariable* obuff;
    HalStringFormatChar* obuffs;
    HalSMArray* argsbuff;
    unsigned long long int ll=string_len(l);
    unsigned long long int indexa;
    HalSMVariable* cobuff;
    unsigned long long int li=ll-1;
    unsigned char isIncrementDecrement=0;
    unsigned long long int ignore=0;
    unsigned char isNotNull=0;
    HalSMVariable* vartemp=&null;

    for (unsigned long long int ii=0;ii<ll;ii++) {
        i=l[ii];
        if (ignore>0) {
            ignore--;
            continue;
        }
        if (indF>0) {
            if (HSFCharCompare(i,ASCIIH('('))) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('(')));
                indF++;
            } else if (HSFCharCompare(i,ASCIIH(')'))) {
                indF--;
                if (indF==0) {
                    if (out->type==HalSMVariableType_HalSMFunctionC) {
                        HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),vars);
                        if (module->type!=HalSMVariableType_HalSMNull&&module->type!=HalSMVariableType_HalSMModule) {HalSMArray_insert(hsmc->memorymanagmentlibrary,args,module,0);}
                        out=HalSMFunctionC_run((HalSMFunctionC*)out->value,args,vars);
                        if (out->type==HalSMVariableType_HalSMError) {
                            return out;
                        }
                        isNotNull=1;
                        o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    } else if (out->type==HalSMVariableType_HalSMLocalFunction) {
                        HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),vars);
                        if (module->type!=HalSMVariableType_HalSMNull&&module->type!=HalSMVariableType_HalSMModule) {
                            HalSMArray_insert(hsmc->memorymanagmentlibrary,args,module,0);
                        }
                        out=HalSMLocalFunction_run((HalSMLocalFunction*)out->value,args,vars);
                        if (out->type==HalSMVariableType_HalSMError){return out;}
                        isNotNull=1;
                        o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    } else if (out->type==HalSMVariableType_HalSMClassC) {
                        HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),vars);
                        out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClassC_run(hsmc,(HalSMClassC*)out->value,args),HalSMVariableType_HalSMRunClassC);
                        isNotNull=1;
                        o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    } else if (out->type==HalSMVariableType_HalSMClass) {
                        HalSMArray* args=HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),vars);
                        out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_run((HalSMClass*)out->value,hsmc,args),HalSMVariableType_HalSMRunClass);
                        isNotNull=1;
                        o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    } else if (out->type==HalSMVariableType_HalSMNull||out->type==HalSMVariableType_void) {
                        obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                        out=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)obuff->value,vars)->arr[0];
                        isNotNull=1;
                        o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(')')));
                    }
                    module=&null;
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(')')));
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (isInd>0) {
            if (HSFCharCompare(i,ASCIIH(']'))) {
                isInd--;
                if (isInd==0) {
                    obuffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
                    out=HalSMCompiler_getArrayByIndex(hsmc,obuffs,out,&isNotNull,vars);
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                }
            } else {
                if (HSFCharCompare(i,ASCIIH('['))) {
                    isInd++;
                }
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (isA>0) {
            if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
                if (isA==0) {
                    out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),vars),HalSMVariableType_HalSMArray);
                    isNotNull=1;
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            } else if (HSFCharCompare(i,ASCIIH('['))) {
                isA++;
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (isD>0) {
            if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
                if (isD==0) {
                    out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgsDict(hsmc,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o),vars),HalSMVariableType_HalSMDict);
                    isNotNull=1;
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                }
            } else if (HSFCharCompare(i,ASCIIH('{'))) {
                isD++;
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (HSFCharCompare(i,ASCIIH('\\'))) {
            if (ii<li) {
                HalStringFormatChar ichr=l[ii+1];
                ignore=1;
                if (HSFCharCompare(ichr,ASCIIH('a'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x07)));
                } else if (HSFCharCompare(ichr,ASCIIH('b'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x08)));
                } else if (HSFCharCompare(ichr,ASCIIH('e'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x1b)));
                } else if (HSFCharCompare(ichr,ASCIIH('f'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x0c)));
                } else if (HSFCharCompare(ichr,ASCIIH('n'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x0a)));
                } else if (HSFCharCompare(ichr,ASCIIH('r'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x0d)));
                } else if (HSFCharCompare(ichr,ASCIIH('t'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x09)));
                } else if (HSFCharCompare(ichr,ASCIIH('v'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x0b)));
                } else if (HSFCharCompare(ichr,ASCIIH('\\'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x5c)));
                } else if (HSFCharCompare(ichr,ASCIIH('"'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x22)));
                } else if (HSFCharCompare(ichr,ASCIIH('\''))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x27)));
                } else if (HSFCharCompare(ichr,ASCIIH('?'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(0x3f)));
                } else if (HSFCharCompare(ichr,ASCIIH('0'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\0')));
                } else if (HSFCharCompare(ichr,ASCIIH('1'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\1')));
                } else if (HSFCharCompare(ichr,ASCIIH('2'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\2')));
                } else if (HSFCharCompare(ichr,ASCIIH('3'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\3')));
                } else if (HSFCharCompare(ichr,ASCIIH('4'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\4')));
                } else if (HSFCharCompare(ichr,ASCIIH('5'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\5')));
                } else if (HSFCharCompare(ichr,ASCIIH('6'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\6')));
                } else if (HSFCharCompare(ichr,ASCIIH('7'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('\7')));
                } else {
                    ignore=0;
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (HSFCharCompare(i,ASCIIH('"'))||HSFCharCompare(i,ASCIIH('\''))) {
            if (HSFCharCompare(i,isCovichki)) {
                isCovichki=ASCIIH('n');
                out=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                isNotNull=1;
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else if (HSFCharCompare(isCovichki,ASCIIH('n'))) {
                isCovichki=i;
            }
        } else if (HSFCharCompare(isCovichki,ASCIIH('n'))==0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            if (o->size>0) {
                obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                out=HalSMCompiler_isGetValue(hsmc,obuff,isOut,isNewVar,vars,&isNotNull,&module,out,U8H("["));
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                isInd++;
            } else if (isNotNull) {
                isInd++;
            } else {
                isA++;
            }
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            isD++;
        } else if (HSFCharCompare(i,ASCIIH('('))) {
            obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
            if (string_len(*(HalStringFormatChar**)obuff->value)>0) {
                if (out->type==HalSMVariableType_HalSMModule) {
                    if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->variables,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->variables,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->localFunctions,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->localFunctions,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->classes,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->classes,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->functions,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->functions,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->modules,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->modules,obuff)->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class or Module Not Found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMCModule) {
                    if (DictElementIndexByKey(hsmc,((HalSMCModule*)out->value)->lfuncs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMCModule*)out->value)->lfuncs,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMCModule*)out->value)->vrs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMCModule*)out->value)->vrs,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMCModule*)out->value)->classes,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMCModule*)out->value)->classes,obuff)->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class or Module Not Found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMRunClass) {
                    if (DictElementIndexByKey(hsmc,((HalSMRunClass*)out->value)->funcs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMRunClass*)out->value)->funcs,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMRunClass*)out->value)->vars,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMRunClass*)out->value)->vars,obuff)->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function Not Found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMClass) {
                    if (DictElementIndexByKey(hsmc,((HalSMClass*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(hsmc,((HalSMClass*)out->value)->funcs,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMClass*)out->value)->vars,obuff)!=-1) {
                        out=DictElementFindByKey(hsmc,((HalSMClass*)out->value)->vars,obuff)->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function Not Found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMRunClassC) {
                    if (DictElementIndexByKey(hsmc,((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMRunClassC*)out->value)->funcs,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMRunClassC*)out->value)->vrs,obuff)!=-1&&DictElementFindByKey(hsmc,((HalSMRunClassC*)out->value)->vrs,obuff)->value->type==HalSMVariableType_HalSMFunctionC) {
                        module=out;
                        out=DictElementFindByKey(hsmc,((HalSMRunClassC*)out->value)->vrs,obuff)->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function Not Found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMClassC) {
                    if (DictElementIndexByKey(hsmc,((HalSMClassC*)out->value)->funcs,obuff)!=-1) {
                        out=DictElementFindByKey(hsmc,((HalSMClassC*)out->value)->funcs,obuff)->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,((HalSMClassC*)out->value)->vrs,obuff)!=-1&&DictElementFindByKey(hsmc,((HalSMClassC*)out->value)->vrs,obuff)->value->type==HalSMVariableType_HalSMFunctionC) {
                        out=DictElementFindByKey(hsmc,((HalSMClassC*)out->value)->vrs,obuff)->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function Not Found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_custom) {
                    if (((HalSMVariableCustom*)out->value)->type->indexFunctionByName(hsmc,out,*(HalStringFormatChar**)obuff->value)!=-1) {
                        module=out;
                        out=((HalSMVariableCustom*)out->value)->type->getFunctionByName(hsmc,out,*(HalStringFormatChar**)obuff->value);
                        isNotNull=1;
                    } else if (((HalSMVariableCustom*)out->value)->type->indexVariableByName(hsmc,out,*(HalStringFormatChar**)obuff->value)!=-1) {
                        out=((HalSMVariableCustom*)out->value)->type->getVariableByName(hsmc,out,*(HalStringFormatChar**)obuff->value);
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Function or Variable of Custom Variable not found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_str) {
                    HalStringFormatChar* strFunc=*(HalStringFormatChar**)obuff->value;
                    if (HSFCompare(strFunc,U8H("replace"))) {
                        module=out;
                        out=hsmc->functions->elements[13]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("index"))) {
                        module=out;
                        out=hsmc->functions->elements[14]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("rindex"))) {
                        module=out;
                        out=hsmc->functions->elements[15]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("split"))) {
                        module=out;
                        out=hsmc->functions->elements[16]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("substring"))) {
                        module=out;
                        out=hsmc->functions->elements[17]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("len"))) {
                        module=out;
                        out=hsmc->functions->elements[19]->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Function not found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMArray) {
                    HalStringFormatChar* strFunc=*(HalStringFormatChar**)obuff->value;
                    if (HSFCompare(strFunc,U8H("append"))) {
                        module=out;
                        out=hsmc->functions->elements[18]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("len"))) {
                        module=out;
                        out=hsmc->functions->elements[19]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("clear"))) {
                        module=out;
                        out=hsmc->functions->elements[20]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("index"))) {
                        module=out;
                        out=hsmc->functions->elements[14]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("insert"))) {
                        module=out;
                        out=hsmc->functions->elements[21]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("pop"))) {
                        module=out;
                        out=hsmc->functions->elements[22]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("remove"))) {
                        module=out;
                        out=hsmc->functions->elements[23]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("reverse"))) {
                        module=out;
                        out=hsmc->functions->elements[24]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("copy"))) {
                        module=out;
                        out=hsmc->functions->elements[25]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("appendArray"))) {
                        module=out;
                        out=hsmc->functions->elements[26]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("slice"))) {
                        module=out;
                        out=hsmc->functions->elements[27]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("set"))) {
                        module=out;
                        out=hsmc->functions->elements[31]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("join"))) {
                        module=out;
                        out=hsmc->functions->elements[39]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("rindex"))) {
                        module=out;
                        out=hsmc->functions->elements[15]->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Function not found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (out->type==HalSMVariableType_HalSMDict) {
                    HalStringFormatChar* strFunc=*(HalStringFormatChar**)obuff->value;
                    if (HSFCompare(strFunc,U8H("append"))) {
                        module=out;
                        out=hsmc->functions->elements[18]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("appendDict"))) {
                        module=out;
                        out=hsmc->functions->elements[28]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("len"))) {
                        module=out;
                        out=hsmc->functions->elements[19]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("clear"))) {
                        module=out;
                        out=hsmc->functions->elements[20]->value;
                        isNotNull=1; 
                    } else if (HSFCompare(strFunc,U8H("indexByKey"))) {
                        module=out;
                        out=hsmc->functions->elements[29]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("indexByValue"))) {
                        module=out;
                        out=hsmc->functions->elements[30]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("set"))) {
                        module=out;
                        out=hsmc->functions->elements[31]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("pop"))) {
                        module=out;
                        out=hsmc->functions->elements[22]->value;
                        isNotNull=1;
                    } else if (DictElementIndexByKey(hsmc,(Dict*)out->value,obuff)!=-1) {
                        out=DictElementFindByKey(hsmc,(Dict*)out->value,obuff)->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("keyByIndex"))) {
                        module=out;
                        out=hsmc->functions->elements[43]->value;
                        isNotNull=1;
                    } else if (HSFCompare(strFunc,U8H("valueByIndex"))) {
                        module=out;
                        out=hsmc->functions->elements[44]->value;
                        isNotNull=1;
                    } else {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Function not found")));
                        *isOut=0;
                        return &hvoid;
                    }
                } else if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value;
                    isNotNull=1;
                } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc,hsmc->localFunctions,obuff)->value;
                    isNotNull=1;
                } else if (DictElementIndexByKey(hsmc,hsmc->classes,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc,hsmc->classes,obuff)->value;
                    isNotNull=1;
                } else if (DictElementIndexByKey(hsmc,hsmc->variables,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc,hsmc->variables,obuff)->value;
                    isNotNull=1;
                } else if ((vartemp=hsmc->getVariable(*(HalStringFormatChar**)obuff->value))->type!=HalSMVariableType_HalSMNull) {
                    out=vartemp;
                    isNotNull=1;
                    vartemp=&null;
                } else if (DictElementIndexByKey(hsmc,vars,obuff)!=-1) {
                    out=DictElementFindByKey(hsmc,vars,obuff)->value;
                    isNotNull=1;
                } else {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Is not function")));
                    *isOut=0;
                    return &hvoid;
                }
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else if (out->type==HalSMVariableType_HalSMNull||out->type==HalSMVariableType_void) {

            } else {
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            indF++;
        } else if (HSFCharCompare(i,ASCIIH(')'))) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(')')));
        } else if (HSFCharCompare(i,ASCIIH('.'))) {
            obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
            out=HalSMCompiler_isGetValue(hsmc,obuff,isOut,isNewVar,vars,&isNotNull,&module,out,U8H("."));
            o=HalSMArray_init(hsmc->memorymanagmentlibrary);
        /*}   else if (HSFCharCompare(i,ASCIIH('+'))&&ii<li&&HSFCharCompare(l[ii+1],ASCIIH('='))&&indF==0) {
            if (o->size==0) {
                if (out->type==HalSMVariableType_HalSMNull||out->type==HalSMVariableType_void) {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("+= can't be in start of variable")));
                    *isOut=0;
                    return &hvoid;
                } else {
                    HalSMVariable* temp=out;
                    out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                    out->type=temp->type;
                    out->value=temp->value;
                    HalSMVariable* inc=HalSMCompiler_addSelfVariable(hsmc,out);
                    temp->value=inc->value;
                    temp->type=inc->type;
                    isNotNull=1;
                    //halwarsing()+=
                }
            } else {
                obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                //halwarsing.net+=
                
                out=HalSMCompiler_isGetValue(hsmc,obuff,isOut,isNewVar,vars,&isNotNull,&module,out,U8H("+="));
                HalSMVariable* temp=out;
                out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                out->type=temp->type;
                out->value=temp->value;
                HalSMVariable* inc=HalSMCompiler_addSelfVariable(hsmc,out);
                temp->value=inc->value;
                temp->type=inc->type;
                isNotNull=1;
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            ignore=1;*/
        } else if (HSFCharCompare(i,ASCIIH('+'))&&ii<li&&HSFCharCompare(l[ii+1],ASCIIH('+'))&&indF==0) {
            if (o->size==0) {
                if (out->type==HalSMVariableType_HalSMNull||out->type==HalSMVariableType_void) {
                    //++halwarsing
                    isIncrementDecrement=1;
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMVariable* temp=out;
                    out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                    out->type=temp->type;
                    out->value=temp->value;
                    HalSMVariable* inc=HalSMCompiler_incrementVariable(hsmc,out);
                    temp->value=inc->value;
                    temp->type=inc->type;
                    isNotNull=1;
                    //halwarsing()++
                }
            } else {
                obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                //halwarsing.net++
                
                out=HalSMCompiler_isGetValue(hsmc,obuff,isOut,isNewVar,vars,&isNotNull,&module,out,U8H("++"));
                HalSMVariable* temp=out;
                out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                out->type=temp->type;
                out->value=temp->value;
                HalSMVariable* inc=HalSMCompiler_incrementVariable(hsmc,out);
                temp->value=inc->value;
                temp->type=inc->type;
                isNotNull=1;
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            ignore=1;
        } else if (HSFCharCompare(i,ASCIIH('-'))&&ii<li&&HSFCharCompare(l[ii+1],ASCIIH('-'))) {
            if (o->size==0) {
                if (out->type==HalSMVariableType_HalSMNull||out->type==HalSMVariableType_void) {
                    //--halwarsing
                    isIncrementDecrement=2;
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMVariable* temp=out;
                    out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                    out->type=temp->type;
                    out->value=temp->value;
                    HalSMVariable* inc=HalSMCompiler_decrementVariable(hsmc,out);
                    temp->value=inc->value;
                    temp->type=inc->type;
                    isNotNull=1;
                    //halwarsing()--
                }
            } else {
                obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
                //halwarsing.net--
                
                out=HalSMCompiler_isGetValue(hsmc,obuff,isOut,isNewVar,vars,&isNotNull,&module,out,U8H("--"));
                HalSMVariable* temp=out;
                out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                out->type=temp->type;
                out->value=temp->value;
                HalSMVariable* inc=HalSMCompiler_decrementVariable(hsmc,out);
                temp->value=inc->value;
                temp->type=inc->type;
                isNotNull=1;
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            ignore=1;
        } else {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        }
        index++;
    }

    if (o->size!=0) {
        obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o));
        out=HalSMCompiler_isGetValue(hsmc,obuff,isOut,isNewVar,vars,&isNotNull,&module,out,U8H(""));
    }
    
    if (out->type!=HalSMVariableType_HalSMNull&&isIncrementDecrement==1) {
        HalSMVariable* inc=HalSMCompiler_incrementVariable(hsmc,out);
        out->value=inc->value;
        out->type=inc->type;
    } else if (out->type!=HalSMVariableType_HalSMNull&&isIncrementDecrement==2) {
        HalSMVariable* inc=HalSMCompiler_decrementVariable(hsmc,out);
        out->value=inc->value;
        out->type=inc->type;
    }

    if (isNotNull) {
        *isOut=1;
        return out;
    }
    *isOut=0;
    return &hvoid;
}

HalSMVariable* HalSMCompiler_isGetValue(HalSMCompiler* hsmc,HalSMVariable* obuff,unsigned char* isOut,unsigned char isNewVar,Dict* vars,unsigned char* isNotNullOut,HalSMVariable** moduleOut,HalSMVariable* out,HalStringFormatChar* type)
{
    unsigned char isNotNull=*isNotNullOut;
    HalSMVariable* vartemp,*module=*moduleOut;
    HalStringFormatChar* buffs=*(HalStringFormatChar**)obuff->value;
    if (string_len(buffs)==0){return out;}
    if (out->type==HalSMVariableType_HalSMNull||out->type==HalSMVariableType_void) {
        if (DictElementIndexByKey(hsmc,hsmc->variables,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,hsmc->variables,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,vars,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,vars,obuff)->value;
            isNotNull=1;
        } else if ((vartemp=hsmc->getVariable(buffs))->type!=HalSMVariableType_HalSMNull) {
            out=vartemp;
            isNotNull=1;
            vartemp=&null;
        } else if (isNewVar) {
            out=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hnull,HalSMVariableType_HalSMNull);
            if (vars->elements!=NOTHALSMNULLPOINTER) {
                PutDictElementToDict(hsmc,vars,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs),out));
            } else {
                PutDictElementToDict(hsmc,hsmc->variables,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs),out));
            }
            isNotNull=1;
        } else if (HSFCompare(buffs,U8H("true"))) {
            out=HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,1);
            isNotNull=1;
        } else if (HSFCompare(buffs,U8H("false"))) {
            out=HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,0);
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,hsmc->localFunctions,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,hsmc->modules,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,hsmc->modules,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,hsmc->classes,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,hsmc->classes,obuff)->value;
            isNotNull=1;
        } else if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {
            if (HSFCompare(type,U8H("."))) {
                out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDoubleGet_init(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)obuff->value),HalSMVariableType_HalSMDoubleGet);
            } else {
                out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parse(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_int);
            }
            isNotNull=1;
        } else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {
            out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_parse(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_double);
            isNotNull=1;
        } else if (HalSMIsHex(hsmc->memorymanagmentlibrary,buffs)) {
            out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parseHex(hsmc->memorymanagmentlibrary,buffs,1),HalSMVariableType_int);
            isNotNull=1;
        } else if (HalSMIsBin(hsmc->memorymanagmentlibrary,buffs)) {
            out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parseBin(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_int);
            isNotNull=1;
        } else if (HSFCompare(buffs,U8H("NULL"))||HSFCompare(buffs,U8H("null"))){
            out=&null;
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMModule) {
        if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->variables,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->variables,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->localFunctions,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->localFunctions,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->classes,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->classes,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->functions,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->functions,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMModule*)out->value)->hsmc->modules,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMModule*)out->value)->hsmc->modules,obuff)->value;
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class or Module Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMCModule) {
        if (DictElementIndexByKey(hsmc,((HalSMCModule*)out->value)->vrs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMCModule*)out->value)->vrs,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMCModule*)out->value)->lfuncs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMCModule*)out->value)->lfuncs,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMCModule*)out->value)->classes,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMCModule*)out->value)->classes,obuff)->value;
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMRunClass) {
        if (DictElementIndexByKey(hsmc,((HalSMRunClass*)out->value)->vars,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMRunClass*)out->value)->vars,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMRunClass*)out->value)->funcs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMRunClass*)out->value)->funcs,obuff)->value;
            isNotNull=1;
        } else if (isNewVar) {
            module=out;
            out=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hnull,HalSMVariableType_HalSMNull);
            PutDictElementToDict(hsmc,((HalSMRunClass*)module->value)->vars,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)obuff->value),out));
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMClass) {
        if (DictElementIndexByKey(hsmc,((HalSMClass*)out->value)->vars,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMClass*)out->value)->vars,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMClass*)out->value)->funcs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMClass*)out->value)->funcs,obuff)->value;
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMClassC) {
        if (DictElementIndexByKey(hsmc,((HalSMClassC*)out->value)->vrs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMClassC*)out->value)->vrs,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMClassC*)out->value)->funcs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMClassC*)out->value)->funcs,obuff)->value;
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMRunClassC) {
        if (DictElementIndexByKey(hsmc,((HalSMRunClassC*)out->value)->vrs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMRunClassC*)out->value)->vrs,obuff)->value;
            isNotNull=1;
        } else if (DictElementIndexByKey(hsmc,((HalSMRunClassC*)out->value)->funcs,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,((HalSMRunClassC*)out->value)->funcs,obuff)->value;
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function or Class Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMDoubleGet) {
        if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {
            HalStringFormatChar* outfs=hsmc->memorymanagmentlibrary->malloc((string_len(((HalSMDoubleGet*)out->value)->st)+2+string_len(*(HalStringFormatChar**)obuff->value))*sizeof(HalStringFormatChar));
            string_cpy(outfs,((HalSMDoubleGet*)out->value)->st);
            string_cat(outfs,U8H("."));
            string_cat(outfs,*(HalStringFormatChar**)obuff->value);
            outfs[string_len(((HalSMDoubleGet*)out->value)->st)+1+string_len(*(HalStringFormatChar**)obuff->value)]=ASCIIH('\0');
            out=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_parse(hsmc->memorymanagmentlibrary,outfs),HalSMVariableType_double);
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("This is not Double")));
        }
    } else if (out->type==HalSMVariableType_custom) {
        if (((HalSMVariableCustom*)out->value)->type->indexVariableByName(hsmc,out,*(HalStringFormatChar**)obuff->value)!=-1) {
            out=((HalSMVariableCustom*)out->value)->type->getVariableByName(hsmc,out,*(HalStringFormatChar**)obuff->value);
            isNotNull=1;
        } else if (((HalSMVariableCustom*)out->value)->type->indexFunctionByName(hsmc,out,*(HalStringFormatChar**)obuff->value)!=-1) {
            module=out;
            out=((HalSMVariableCustom*)out->value)->type->getFunctionByName(hsmc,out,*(HalStringFormatChar**)obuff->value);
            isNotNull=1;
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable or Function Not Found")));
        }
    } else if (out->type==HalSMVariableType_HalSMDict) {
        if (DictElementIndexByKey(hsmc,(Dict*)out->value,obuff)!=-1) {
            out=DictElementFindByKey(hsmc,(Dict*)out->value,obuff)->value;
            isNotNull=1;
        }
    } else {
        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Unknown Variable")));
    }
    *isNotNullOut=isNotNull;
    *moduleOut=module;
    return out;
}

HalSMVariable* HalSMCompiler_getArrayByIndex(HalSMCompiler* hsmc,HalStringFormatChar* obuffs,HalSMVariable* out,unsigned char* isNotNullOut,Dict* vars)
{
    unsigned char isNotNull=*isNotNullOut;
    unsigned long long int indexa;
    HalSMArray* argsbuff;
    /*HalSMArray* argsbuff,*n;
    n=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned long long int indexa,ll,li;
    ll=string_len(l);
    li=ll-1;
    unsigned long long int isA=0,isF=0,isD=0,isIf=0;
    HalStringFormatChar isS=ASCIIH('n'),i,*obuffs;

    for (unsigned long long int ind=0;ind<ll;ind++) {
        i=l[ind];
        
        if (isA) {
            if (HSFCharCompare(i,ASCIIH('['))) {
                isA++;
            } else if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
            }
        } else if (isF) {
            if (HSFCharCompare(i,ASCIIH('('))) {
                isF++;
            } else if (HSFCharCompare(i,ASCIIH(')'))) {
                isF--;
            }

        } else if (HSFCharCompare(isS,ASCIIH('n'))==0) {
            if (HSFCharCompare(i,isS)){isS=ASCIIH('n');}
        } else if (isD) {
            if (HSFCharCompare(i,ASCIIH('{'))) {
                isD++;
            } else if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
            }
        } else if (HSFCharCompare(i,ASCIIH('\''))||HSFCharCompare(i,ASCIIH('"'))) {
            isS=i;
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            isA++;
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            isD++;
        } else if (HSFCharCompare(i,ASCIIH('('))) {
            isF++;
        } else if (HSFCharCompare(i,ASCIIH('?'))) {
            isIf++;
        } else if (isIf>0&&HSFCharCompare(i,ASCIIH(':'))) {
            isIf--;
        }
        if (ind==li||HSFCharCompare(i,ASCIIH(':'))) {
            obuffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n);
            HalSMArray* argsindex=HalSMCompiler_getArgs(hsmc,obuffs,vars);
            for (unsigned long long int indexspl=0;indexspl<argsindex->size;indexspl++) {
                if (out->type==HalSMVariableType_HalSMArray) {
                    if (argsindex->arr[indexspl]->type!=HalSMVariableType_int) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,U8H("Value from array can only be by integer"));
                    }
                    indexa=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)argsindex->arr[indexspl]->value);
                    HalSMArray* rcls=(HalSMArray*)out->value;
                    if (indexa>=rcls->size) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,U8H("Index is equal to or more than size of array"));
                    } else {
                        out=rcls->arr[indexa];
                        isNotNull=1;
                    }
                } else if (out->type==HalSMVariableType_HalSMRunClassC) {
                    HalSMRunClassC* rcls=(HalSMRunClassC*)out->value;
                    if (DictElementIndexByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))==-1) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,U8H("This class does not contain function __getitem__"));
                    }
                    argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClassC));
                    HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,argsindex->arr[indexspl]);
                    out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))->value->value)(hsmc,argsbuff,vars);
                    if (out->type==HalSMVariableType_HalSMError){
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)out->value)->error);
                    }
                    isNotNull=1;
                } else if (out->type==HalSMVariableType_HalSMRunClass) {
                    HalSMRunClass* rcls=(HalSMRunClass*)out->value;
                    if (DictElementIndexByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))==-1) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,U8H("This class does not contain function __getitem__"));
                    }
                    argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClass));
                    HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,argsindex->arr[indexspl]);
                    out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))->value->value)(hsmc,argsbuff,vars);
                    if (out->type==HalSMVariableType_HalSMError){
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,((HalSMError*)out->value)->error);
                    }
                    isNotNull=1;
                } else if (out->type==HalSMVariableType_custom) {
                    HalSMVariableCustom* hvc=(HalSMVariableCustom*)out->value;
                    out=hvc->type->getItem(hsmc,out,argsindex->arr[indexspl]);
                    isNotNull=1;
                } else if (out->type==HalSMVariableType_HalSMDict) {
                    Dict* rcls=(Dict*)out->value;
                    HalSMVariable* indexv=argsindex->arr[indexspl];
                    if (DictElementIndexByKey(hsmc,rcls,indexv)==-1) {
                        HalSMCompiler_ThrowError(hsmc,hsmc->line,U8H("Not Found Value By Key Dict"));
                    } else {
                        out=DictElementFindByKey(hsmc,rcls,indexv)->value;
                        isNotNull=1;
                    }
                } else {
                    HalSMCompiler_ThrowError(hsmc,hsmc->line,U8H("This object is not array"));
                }
            }
        }
        HalSMArray_add(hsmc->memorymanagmentlibrary,n,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
    }*/
    HalSMArray* argsindex=HalSMCompiler_getArgs(hsmc,obuffs,vars);
    for (unsigned long long int indexspl=0;indexspl<argsindex->size;indexspl++) {
        if (out->type==HalSMVariableType_HalSMArray) {
            if (argsindex->arr[indexspl]->type!=HalSMVariableType_int) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Value from array can only be by integer")));
            }
            indexa=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)argsindex->arr[indexspl]->value);
            HalSMArray* rcls=(HalSMArray*)out->value;
            if (indexa>=rcls->size) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Index is equal to or more than size of array")));
            } else {
                out=rcls->arr[indexa];
                isNotNull=1;
            }
        } else if (out->type==HalSMVariableType_HalSMRunClassC) {
            HalSMRunClassC* rcls=(HalSMRunClassC*)out->value;
            if (DictElementIndexByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))==-1) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("This class does not contain function __getitem__")));
            }
            argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClassC));
            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,argsindex->arr[indexspl]);
            out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))->value->value)(hsmc,argsbuff,vars);
            if (out->type==HalSMVariableType_HalSMError){
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,((HalSMError*)out->value)->error));
            }
            isNotNull=1;
        } else if (out->type==HalSMVariableType_HalSMRunClass) {
            HalSMRunClass* rcls=(HalSMRunClass*)out->value;
            if (DictElementIndexByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))==-1) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("This class does not contain function __getitem__")));
            }
            argsbuff=HalSMArray_init(hsmc->memorymanagmentlibrary);
            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,rcls,HalSMVariableType_HalSMRunClass));
            HalSMArray_add(hsmc->memorymanagmentlibrary,argsbuff,argsindex->arr[indexspl]);
            out=(*(HalSMFunctionCTypeDef*)DictElementFindByKey(hsmc,rcls->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__getitem__")))->value->value)(hsmc,argsbuff,vars);
            if (out->type==HalSMVariableType_HalSMError){
                HalSMCompiler_ThrowError(hsmc,(HalSMError*)out->value);
            }
            isNotNull=1;
        } else if (out->type==HalSMVariableType_custom) {
            HalSMVariableCustom* hvc=(HalSMVariableCustom*)out->value;
            out=hvc->type->getItem(hsmc,out,argsindex->arr[indexspl]);
            isNotNull=1;
        } else if (out->type==HalSMVariableType_HalSMDict) {
            Dict* rcls=(Dict*)out->value;
            HalSMVariable* indexv=argsindex->arr[indexspl];
            if (DictElementIndexByKey(hsmc,rcls,indexv)==-1) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Not Found Value By Key Dict")));
            } else {
                out=DictElementFindByKey(hsmc,rcls,indexv)->value;
                isNotNull=1;
            }
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("This object is not array")));
        }
    }
    *isNotNullOut=isNotNull;
    return out;
}

HalSMVariable* HalSMCompiler_additionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_str||v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.addStr(hsmc,v0,v1));
    } else if (v0->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v0->value)->type->add(hsmc,v0,v1);
    } else if (v1->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v1->value)->type->add(hsmc,v0,v1);
    } else if (v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.addDouble(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_double);
    } else if (v0->type==HalSMVariableType_int||v1->type==HalSMVariableType_int) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.addInt(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_int);
    }
    return &null;
}

HalSMVariable* HalSMCompiler_subtractionVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v0->value)->type->sub(hsmc,v0,v1);
    } else if (v1->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v1->value)->type->sub(hsmc,v0,v1);
    } else if ((v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double)&&v0->type!=HalSMVariableType_str&&v1->type!=HalSMVariableType_str) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.subDouble(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_double);
    } else if (v0->type==HalSMVariableType_int) {
        if (v1->type==HalSMVariableType_str) {
            v1=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parse(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)v1->value),HalSMVariableType_int);
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.subInt(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_int);
    } else if (v1->type==HalSMVariableType_int) {
        if (v0->type==HalSMVariableType_str) {
            v0=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parse(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)v0->value),HalSMVariableType_int);
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.subInt(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_int);
    }
    return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.subStr(hsmc,v0,v1));
}

HalSMVariable* HalSMCompiler_multiplyVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v0->value)->type->mul(hsmc,v0,v1);
    } else if (v1->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v1->value)->type->mul(hsmc,v0,v1);
    } else if (v0->type==HalSMVariableType_str&&v1->type==HalSMVariableType_str) {
        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("This is not possible to multiply a string by a string")));
    } else if (v0->type==HalSMVariableType_str||v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.mulStr(hsmc->memorymanagmentlibrary,v0,v1));
    } else if (v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.mulDouble(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_double);
    } else if (v0->type==HalSMVariableType_int||v1->type==HalSMVariableType_int) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.mulInt(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_int);
    }
    return &null;
}

HalSMVariable* HalSMCompiler_divideVariables(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v0->value)->type->div(hsmc,v0,v1);
    } else if (v1->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v1->value)->type->div(hsmc,v0,v1);
    } else if (v0->type==HalSMVariableType_str&&v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)v0->value,*(HalStringFormatChar**)v1->value,U8H("")));
    } else if (v0->type==HalSMVariableType_str||v1->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.divStr(hsmc,v0,v1));
    } else if (v0->type==HalSMVariableType_double||v1->type==HalSMVariableType_double) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.divDouble(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_double);
    } else if (v0->type==HalSMVariableType_int||v1->type==HalSMVariableType_int) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,hsmc->calcVars.divInt(hsmc->memorymanagmentlibrary,v0,v1),HalSMVariableType_int);
    }
    return &null;
}

HalSMVariable* HalSMCompiler_incrementVariable(HalSMCompiler* hsmc,HalSMVariable* v)
{
    if (v->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v->value)->type->increment(hsmc,v);
    } else if (v->type==HalSMVariableType_int) {
        return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_Add(hsmc->memorymanagmentlibrary,(HalSMInteger*)v->value,&HalSMIntegerOne));
    } else if (v->type==HalSMVariableType_double) {
        return HalSMVariable_init_double(hsmc->memorymanagmentlibrary,HalSMDouble_Add(hsmc->memorymanagmentlibrary,(HalSMDouble*)v->value,&HalSMDoubleOne));
    } else if (v->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.addStr(hsmc,v,v));
    }
    return &null;
}

HalSMVariable* HalSMCompiler_decrementVariable(HalSMCompiler* hsmc,HalSMVariable* v)
{
    if (v->type==HalSMVariableType_custom) {
        return ((HalSMVariableCustom*)v->value)->type->decrement(hsmc,v);
    } else if (v->type==HalSMVariableType_int) {
        return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_Sub(hsmc->memorymanagmentlibrary,(HalSMInteger*)v->value,&HalSMIntegerOne));
    } else if (v->type==HalSMVariableType_double) {
        return HalSMVariable_init_double(hsmc->memorymanagmentlibrary,HalSMDouble_Sub(hsmc->memorymanagmentlibrary,(HalSMDouble*)v->value,&HalSMDoubleOne));
    } else if (v->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,hsmc->calcVars.subStr(hsmc,v,v));
    }
    return &null;
}

HalSMVariable* HalSMCompiler_increment(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2) {
        if (args->arr[0]->type==HalSMVariableType_HalSMVar) {
            HalSMVariable* vartemp=&null;
            if (DictElementIndexByKey(hsmc,hsmc->variables,((HalSMVar*)args->arr[0]->value)->name)!=-1) {
                args->arr[0]=DictElementFindByKey(hsmc,hsmc->variables,((HalSMVar*)args->arr[0]->value)->name)->value;
            } else if ((vartemp=hsmc->getVariable(*(HalStringFormatChar**)((HalSMVar*)args->arr[0]->value)->name->value))->type!=HalSMVariableType_HalSMNull) {
                args->arr[0]=vartemp;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable Not Found")));
            }
        }
        
        HalSMVariable* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
        out->value=args->arr[0]->value;
        out->type=args->arr[0]->type;
        HalSMVariable* res=HalSMCompiler_incrementVariable(hsmc,args->arr[0]);
        args->arr[0]->value=res->value;
        args->arr[0]->type=res->type;
        if ((*(unsigned char*)args->arr[1]->value)==1) {
            return args->arr[0];
        }
        return out;
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_decrement(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_HalSMBool) {
        if (args->arr[0]->type==HalSMVariableType_HalSMVar) {
            HalSMVariable* vartemp=&null;
            if (DictElementIndexByKey(hsmc,hsmc->variables,((HalSMVar*)args->arr[0]->value)->name)!=-1) {
                args->arr[0]=DictElementFindByKey(hsmc,hsmc->variables,((HalSMVar*)args->arr[0]->value)->name)->value;
            } else if ((vartemp=hsmc->getVariable(*(HalStringFormatChar**)((HalSMVar*)args->arr[0]->value)->name->value))->type!=HalSMVariableType_HalSMNull) {
                args->arr[0]=vartemp;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Variable Not Found")));
            }
        }
        
        HalSMVariable* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
        out->value=args->arr[0]->value;
        out->type=args->arr[0]->type;
        HalSMVariable* res=HalSMCompiler_decrementVariable(hsmc,args->arr[0]);
        args->arr[0]->value=res->value;
        args->arr[0]->type=res->type;
        if ((*(unsigned char*)args->arr[1]->value)==1) {
            return args->arr[0];
        }
        return out;
    }
    return &hvoid;
}

void HalSMCompiler_getArgsSetVar(HalSMCompiler* hsmc,HalSMArray* value,HalSMVariable* pointer,Dict* vars)
{
    HalSMArray outc=*value;
    HalSMArray* out=HalSMArray_copy(hsmc->memorymanagmentlibrary,value);
    //HalSMArray* out=value;
    long long int skip=0;
    long long int lout=out->size;
    long long int ou=0;

    HalSMVariable* i;
    HalSMVariable* v;
    HalSMArray* temp;
    HalSMFunctionArray* tempf;

    while (ou<lout) {
        if (HalSMArray_get(out,ou)->type==HalSMVariableType_HalSMVar) {
            unsigned char isOut;
            HalSMArray_set(out,HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)((HalSMVar*)HalSMArray_get(out,ou)->value)->name->value,&isOut,0,vars),ou);
        }
        ou++;
    }

    HalSMCompiler_executeArgs(hsmc,&out,vars);
    pointer->type=out->arr[0]->type;
    pointer->value=out->arr[0]->value;
}

void HalSMCompiler_getArgsFunction(HalSMCompiler* hsmc,HalSMArray* value,HalSMArray* out,Dict* vars) {
    out->size=value->size;
    out->arr=hsmc->memorymanagmentlibrary->realloc(out->arr,sizeof(DictElement*)*value->size);
    for (unsigned long long int i=0;i<value->size;i++)
        out->arr[i]=value->arr[i];
    long long int skip=0;
    long long int lout=out->size;
    long long int ou=0;

    HalSMVariable* i;
    HalSMVariable* v;
    HalSMArray* temp;
    HalSMFunctionArray* tempf;

    while (ou<lout) {
        if (HalSMArray_get(out,ou)->type==HalSMVariableType_HalSMVar) {
            //HalSMArray_set(out,DictElementFindByKey(hsmc,hsmc->variables,((HalSMVar*)HalSMArray_get(out,ou)->value)->name)->value,ou);
            unsigned char isOut;
            HalSMArray_set(out,HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)((HalSMVar*)HalSMArray_get(out,ou)->value)->name->value,&isOut,0,vars),ou);
        }
        ou++;
    }

    HalSMCompiler_executeArgs(hsmc,&out,vars);
}

Dict* HalSMCompiler_getArgsDict(HalSMCompiler* hsmc,HalStringFormatChar* l,Dict* vars) {
    HalStringFormatChar isS=ASCIIH('n');
    unsigned long long int isD=0;
    unsigned long long int isA=0;
    unsigned long long int ll=string_len(l);
    HalStringFormatChar i;
    HalStringFormatChar* buffs;
    HalSMArray* o=HalSMArray_init(hsmc->memorymanagmentlibrary);
    Dict* out=DictInit(hsmc->memorymanagmentlibrary);
    unsigned char isKey=0;
    HalSMVariable* key=&null;
    unsigned long long int li=ll-1;
    unsigned long long int ignore=0;
    for (unsigned long long int indexi=0;indexi<ll;indexi++) {
        i=l[indexi];
        buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
        if (ignore>0) {
            ignore--;
            continue;
        }

        if (HSFCharCompare(isS,ASCIIH('n'))==0) {
            if (HSFCharCompare(i,isS)) {
                isS=ASCIIH('n');
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (isD>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
            } else if (HSFCharCompare(i,ASCIIH('{'))) {
                isD++;
            }
        } else if (isA>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
            } else if (HSFCharCompare(i,ASCIIH('['))) {
                isA++;
            }
        } else if (HSFCharCompare(i,ASCIIH('\\'))) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (indexi<li) {
                HalStringFormatChar ichr=l[indexi+1];
                ignore=1;
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ichr));
            }
        } else if (HSFCharCompare(i,ASCIIH('"'))||HSFCharCompare(i,ASCIIH('\''))) {
            isS=i;
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            isD++;
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            isA++;
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (HSFCharCompare(i,ASCIIH(':'))) {
            if (o->size>0) {
                if (isKey) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                } else {
                    key=HalSMCompiler_getArgs(hsmc,buffs,vars)->arr[0];
                    isKey=1;
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Zero Key Dict")));
                return &NULLDict;
            }
        } else if (HSFCharCompare(i,ASCIIH(','))) {
            if (isKey) {
                if (o->size>0) {
                    PutDictElementToDict(hsmc,out,DictElementInit(hsmc->memorymanagmentlibrary,key,HalSMCompiler_getArgs(hsmc,buffs,vars)->arr[0]));
                    key=&null;
                    isKey=0;
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                } else {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Zero Value Dict")));
                    return &NULLDict;
                }
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Zero Key Dict")));
                return &NULLDict;
            }
        } else if (HSFCharCompare(i,ASCIIH(' '))||HSFCharCompare(i,ASCIIH('\n'))||HSFCharCompare(i,ASCIIH('\t'))||HSFCharCompare(i,ASCIIH('\r'))) {
            
        } else {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        }
    }
    if (o->size>0) {
        if (isKey) {
            buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
            PutDictElementToDict(hsmc,out,DictElementInit(hsmc->memorymanagmentlibrary,key,HalSMCompiler_getArgs(hsmc,buffs,vars)->arr[0]));
            key=&null;
            isKey=0;
            HalSMArray_free(hsmc->memorymanagmentlibrary,o);
        } else {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Zero Value Dict")));
            return &NULLDict;
        }
    }
    return out;
}

void HalSMCompiler_executeArgs(HalSMCompiler* hsmc,HalSMArray** outArr,Dict* vars)
{
    HalSMArray* out=*outArr;
    unsigned long long int skip=0;
    unsigned long long int lout=out->size;
    unsigned long long int ou=0;
    HalSMVariable* iv;
    HalSMVariable* v;
    HalSMArray* temp;
    HalSMFunctionArray* tempf;

    while (ou<lout) {
        if (skip>0) {skip--;continue;}

        iv=HalSMArray_get(out,ou);

        if (iv->type==HalSMVariableType_HalSMFunctionArray) {
            tempf=(HalSMFunctionArray*)iv->value;
            temp=tempf->args;
            if (tempf->type==HalSMFunctionArrayType_function) {
                unsigned char isOut;
                //Question
                v=HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)temp->arr[0]->value,&isOut,0,vars);
                if (isOut==1) {
                    HalSMArray_set(out,v,ou);
                } else {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Function Not Found")));
                }
            } else if (tempf->type==HalSMFunctionArrayType_array) {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)HalSMArray_get(temp,0)->value,vars),HalSMVariableType_HalSMArray),ou);
            } else if (tempf->type==HalSMFunctionArrayType_dict) {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgsDict(hsmc,*(HalStringFormatChar**)HalSMArray_get(temp,0)->value,vars),HalSMVariableType_HalSMDict),ou);
            }
        } else if (iv->type==HalSMVariableType_HalSMMult) {
            HalSMArray_set(out,HalSMCompiler_multiplyVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou--;
            lout-=2;
            skip=1;
        } else if (iv->type==HalSMVariableType_HalSMDivide) {
            HalSMArray_set(out,HalSMCompiler_divideVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou--;
            lout-=2;
            skip=1;
        } else if (iv->type==HalSMVariableType_HalSMShift) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_custom)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMCompiler_shift(hsmc,iv,v,(HalSMShift*)HalSMArray_get(out,ou)->value),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                if (((HalSMShift*)HalSMArray_get(out,ou)->value)->c==0) {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Shift Back (<<) cannot without numeric args")));
                } else {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Shift Forward (>>) cannot without numeric args")));
                }
            }
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
            HalSMArray_set(out,HalSMCompiler_additionVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou--;
            lout-=2;
            skip=1;
        } else if (iv->type==HalSMVariableType_HalSMMinus) {
            HalSMArray_set(out,HalSMCompiler_subtractionVariables(hsmc,HalSMArray_get(out,ou-1),HalSMArray_get(out,ou+1)),ou-1);
            temp=HalSMArray_slice(hsmc->memorymanagmentlibrary,out,0,ou);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,temp,HalSMArray_slice(hsmc->memorymanagmentlibrary,out,ou+2,out->size));
            out=temp;
            ou--;
            lout-=2;
            skip=1;
        } else if (iv->type==HalSMVariableType_HalSMNegativeSign) {
            v=HalSMArray_get(out,ou+1);
            if (v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMCompiler_negate(hsmc,v),ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou+1);
                lout-=1;
            }
        }
        ou++;
    }

    skip=0;
    lout=out->size;
    ou=0;

    while (ou<lout) {
        if (skip>0){skip--;continue;}
        iv=HalSMArray_get(out,ou);

        if (iv->type==HalSMVariableType_HalSMEqual) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
            else {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMVariable_Compare(hsmc,iv,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            }
        } else if (iv->type==HalSMVariableType_HalSMNotEqual) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
            else {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMVariable_Compare(hsmc,iv,v)==0),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            }
        } else if (iv->type==HalSMVariableType_HalSMMore) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
            else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_double||iv->type==HalSMVariableType_custom)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_isMore(hsmc,iv,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("More (>) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMLess) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar){skip=1;}
            else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_double||v->type==HalSMVariableType_custom)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_isLess(hsmc,iv,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Less (<) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMMoreEqual) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_double||iv->type==HalSMVariableType_custom)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_isMore(hsmc,iv,v)||HalSMVariable_Compare(hsmc,iv,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("More Equal (>=) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMLessEqual) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_double||iv->type==HalSMVariableType_custom)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_double||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_isLess(hsmc,iv,v)||HalSMVariable_Compare(hsmc,iv,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Less Equal (<=) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMAND) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_getBool(hsmc,iv)&&HalSMCompiler_getBool(hsmc,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            }
        } else if (iv->type==HalSMVariableType_HalSMOR) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_getBool(hsmc,iv)||HalSMCompiler_getBool(hsmc,v)),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            }
        } else if (iv->type==HalSMVariableType_HalSMNegate) {
            v=HalSMArray_get(out,ou+1);
            if (v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_getBool(hsmc,v)==0),ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou+1);
                lout-=1;
            }
        } else if (iv->type==HalSMVariableType_HalSMInlineIf) {
            if (ou>0&&ou+3<lout&&HalSMArray_get(out,ou+2)->type==HalSMVariableType_HalSMInlineElse) {
                v=HalSMArray_get(out,ou-1);
                if (v->type==HalSMVariableType_HalSMVar) {
                    skip=1;
                } else {
                    if (HalSMCompiler_getBool(hsmc,v)) {
                        HalSMArray_set(out,HalSMArray_get(out,ou+1),ou-1);
                    } else {
                        HalSMArray_set(out,HalSMArray_get(out,ou+3),ou-1);
                    }
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                    lout-=4;
                    ou--;
                }
            }
        } else if (iv->type==HalSMVariableType_HalSMSetArg) {
            if (ou<lout-1) {
                unsigned char isGet=0;
                HalSMVariable* var=HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)((HalSMSetArg*)iv->value)->name->value,&isGet,1,vars);
                if (isGet==1) {
                    var->value=out->arr[ou+1]->value;
                    var->type=out->arr[ou+1]->type;
                    out->arr[ou]=var;
                } else {
                    out->arr[ou]=&hvoid;
                }
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou+1);
            }
            lout--;
        } else if (iv->type==HalSMVariableType_HalSMModulo) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_custom)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMCompiler_modulo(hsmc,iv,v),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->ind,U8H("Modulo (%) cannot without numeric args")));
            }
        }
        ou++;
    }
    

    skip=0;
    lout=out->size;
    ou=0;

    while (ou<lout) {
        if (skip>0){skip--;continue;}
        iv=HalSMArray_get(out,ou);

        if (iv->type==HalSMVariableType_HalSMPow) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if ((iv->type==HalSMVariableType_int||iv->type==HalSMVariableType_custom||iv->type==HalSMVariableType_double)&&(v->type==HalSMVariableType_int||v->type==HalSMVariableType_custom)) {
                HalSMArray_set(out,HalSMCompiler_pow(hsmc,iv,v),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Pow (**) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMXOR) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if (iv->type==HalSMVariableType_int&&iv->type==HalSMVariableType_int) {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_BitXOR(hsmc->memorymanagmentlibrary,(HalSMInteger*)iv->value,(HalSMInteger*)v->value),HalSMVariableType_int),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("XOR (^) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMBAND) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if (iv->type==HalSMVariableType_int&&iv->type==HalSMVariableType_int) {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_BitAND(hsmc->memorymanagmentlibrary,(HalSMInteger*)iv->value,(HalSMInteger*)v->value),HalSMVariableType_int),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("BIT AND (&) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMBOR) {
            iv=HalSMArray_get(out,ou-1);
            v=HalSMArray_get(out,ou+1);
            if (iv->type==HalSMVariableType_HalSMVar||v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else if (iv->type==HalSMVariableType_int&&iv->type==HalSMVariableType_int) {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_BitOR(hsmc->memorymanagmentlibrary,(HalSMInteger*)iv->value,(HalSMInteger*)v->value),HalSMVariableType_int),ou-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou);
                lout-=2;
                ou--;
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("BIT OR (|) cannot without numeric args")));
            }
        } else if (iv->type==HalSMVariableType_HalSMBNOT) {
            v=HalSMArray_get(out,ou+1);
            if (v->type==HalSMVariableType_HalSMVar) {
                skip=1;
            } else {
                HalSMArray_set(out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_BitNOT(hsmc->memorymanagmentlibrary,(HalSMInteger*)v->value),HalSMVariableType_int),ou);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,out,ou+1);
                lout-=1;
            }
        }
        ou++;
    }
    *outArr=out;
}

HalSMArray* HalSMCompiler_getArgs(HalSMCompiler* hsmc,HalStringFormatChar* l,Dict* vars)
{
    HalSMArray* outarr=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalStringFormatChar isS=ASCIIH('n');
    unsigned long long int isF=0;
    HalSMArray* o=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned long long int isA=0;
    unsigned long long int isAG=0;
    unsigned long long int isD=0;
    unsigned long long int ll=string_len(l);
    unsigned long long int lind=ll-1;
    HalSMVariable* isGet;
    unsigned long long int isNArgs=0;
    unsigned long long int ignore=0;
    HalStringFormatChar i;
    HalSMVariable* buffo;
    HalStringFormatChar* buffs;
    HalSMArray* tempargs;
    HalSMVariable* tempret;
    unsigned long long int skip=0;

    HalSMVariable* vartemp=&null;
    unsigned long long int li=ll-1;

    for (unsigned long long int indexi=0;indexi<ll;indexi++) {
        i=l[indexi];
        if (ignore>0) {
            ignore--;
            continue;
        }

        buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
        buffo=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs);

        if (isNArgs>0&&HSFCharCompare(i,ASCIIH('('))) {
            isNArgs++;
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('(')));
        } else if (isNArgs>0&&HSFCharCompare(i,ASCIIH(')'))) {
            isNArgs--;
            if (isNArgs==0) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(')')));
            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(')')));}
        } else if (isNArgs>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (isA>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
            } else if (HSFCharCompare(i,ASCIIH('['))) {isA++;}
        } else if (isAG>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (HSFCharCompare(i,ASCIIH(']'))) {
                isAG--;
            } else if (HSFCharCompare(i,ASCIIH('['))) {isAG++;}
        } else if (isD>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
            } else if (HSFCharCompare(i,ASCIIH('{'))) {
                isD++;
            }
        } else if (HSFCharCompare(i,ASCIIH('\\'))) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            if (indexi<li) {
                HalStringFormatChar ichr=l[indexi+1];
                ignore=1;
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ichr));
            }
        } else if (HSFCharCompare(isS,ASCIIH('n'))==0) {
            if (HSFCharCompare(i,isS)) {
                isS=ASCIIH('n');
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));}
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            if (o->size==0) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                isA++;
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                isAG++;
            }
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            isD++;
        } else if (isF>0) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
            if (HSFCharCompare(i,ASCIIH(')'))) {
                isF--;
            } else if (HSFCharCompare(i,ASCIIH('('))) {isF++;}
        } else if (HSFCharCompare(i,ASCIIH('"'))||HSFCharCompare(i,ASCIIH('\''))) {
            isS=i;
            HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
        } else if (HSFCharCompare(i,ASCIIH('('))) {
            if (isF==0) {
                if (o->size==0) {isNArgs++;HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('(')));}
                else {
                    isF++;
                    HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                }
            } else {
                isF++;
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (HSFCharCompare(i,ASCIIH(' '))||HSFCharCompare(i,ASCIIH('\n'))) {

        } else if (HSFCharCompare(i,ASCIIH(','))) {
            if (o->size!=0) {
                if (StringIndexOf(hsmc->memorymanagmentlibrary,buffs,U8H("["))!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,buffs,U8H("]"))==-1) {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH(',')));}
                else {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            }
            
            HalSMCompiler_executeArgs(hsmc,&out,vars);
            HalSMArray_appendArray(hsmc->memorymanagmentlibrary,outarr,out);
            out=HalSMArray_init(hsmc->memorymanagmentlibrary);
        } else if (HSFCharCompare(i,ASCIIH('+'))) {
            if (o->size!=0&&indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('+'))==0&&HSFCharCompare(*(HalStringFormatChar*)o->arr[0]->value,ASCIIH('+'))==0&&HSFCharCompare(*(HalStringFormatChar*)o->arr[o->size-1]->value,ASCIIH('+'))==0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&plus,HalSMVariableType_HalSMPlus));
            } else if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('+'))) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                ignore=1;
            } else {
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&plus,HalSMVariableType_HalSMPlus));
            }
        } else if (HSFCharCompare(i,ASCIIH('-'))) {
            if (o->size!=0&&indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('-'))==0&&HSFCharCompare(*(HalStringFormatChar*)o->arr[0]->value,ASCIIH('-'))==0&&HSFCharCompare(*(HalStringFormatChar*)o->arr[o->size-1]->value,ASCIIH('-'))==0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&minus,HalSMVariableType_HalSMMinus));
            } else if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('-'))) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
                ignore=1;
            } else if (o->size==0&&indexi<lind) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,&vnegativesign);
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (HSFCharCompare(i,ASCIIH('*'))) {
            if (o->size!=0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('*'))) {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&hpow,HalSMVariableType_HalSMPow));
                    ignore=1;
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&mult,HalSMVariableType_HalSMMult));
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&mult,HalSMVariableType_HalSMMult));
            }
        } else if (HSFCharCompare(i,ASCIIH('/'))) {
            if (o->size!=0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            }
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&divide,HalSMVariableType_HalSMDivide));
        } else if (HSFCharCompare(i,ASCIIH('='))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))){ignore=1;}
                    continue;
                }
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))) {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&equal,HalSMVariableType_HalSMEqual));
                    ignore=1;
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetArg_init(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs)),HalSMVariableType_HalSMSetArg));
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                }
            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,ASCIIH('=')));}
        } else if (HSFCharCompare(i,ASCIIH('!'))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))){ignore=1;}
                    continue;
                }
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))) {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&notequal,HalSMVariableType_HalSMNotEqual));
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    ignore=1;
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,&vnegate);
            }
        } else if (HSFCharCompare(i,ASCIIH('>'))) {
            if (o->size!=0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))||HSFCharCompare(l[indexi+1],ASCIIH('>'))) {
                        ignore=1;
                    }
                    continue;
                }
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))) {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&moreequal,HalSMVariableType_HalSMMoreEqual));
                    ignore=1;
                } else if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('>'))) {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&shift_right,HalSMVariableType_HalSMShift));
                    ignore=1;
                } else {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&more,HalSMVariableType_HalSMMore));
                }
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("More (>) cannot without numeric args")));
            }
        } else if (HSFCharCompare(i,ASCIIH('<'))) {
            if (o->size!=0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))||HSFCharCompare(l[indexi+1],ASCIIH('<'))) {
                        ignore=1;
                    }
                    continue;
                }
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('='))) {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&lessequal,HalSMVariableType_HalSMLessEqual));
                    ignore=1;
                } else if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('<'))) {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&shift_left,HalSMVariableType_HalSMShift));
                    ignore=1;
                } else {
                    HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&less,HalSMVariableType_HalSMLess));
                }
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Less (<) cannot without numeric args")));
            }
        } else if (HSFCharCompare(i,ASCIIH('^'))) {
            if (o->size!=0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&hxor,HalSMVariableType_HalSMXOR));
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Pow (^) cannot without numeric args")));
            }
        } else if (HSFCharCompare(i,ASCIIH('%'))) {
            if (o->size!=0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&modulo,HalSMVariableType_HalSMModulo));
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Modulo (%) cannot without numeric args")));
            }
        } else if (HSFCharCompare(i,ASCIIH('&'))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('&'))){ignore=1;}
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('&'))) {
                    out->arr[out->size-1]=HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_getBool(hsmc,out->arr[out->size-1]));
                    if ((*(unsigned char*)out->arr[out->size-1]->value)==0) {
                        skip++;
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&and,HalSMVariableType_HalSMAND));
                    }
                    ignore=1;
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&hband,HalSMVariableType_HalSMBAND));
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (HSFCharCompare(i,ASCIIH('|'))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('|'))){ignore=1;}
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                if (indexi<lind&&HSFCharCompare(l[indexi+1],ASCIIH('|'))) {
                    HalSMVariable* temppointer=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
                    HalSMCompiler_getArgsSetVar(hsmc,out,temppointer,vars);
                    out=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,HalSMCompiler_getBool(hsmc,temppointer)));
                    if ((*(unsigned char*)out->arr[0]->value)==1) {
                        skip++;
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&or,HalSMVariableType_HalSMOR));
                    }
                    ignore=1;
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&hbor,HalSMVariableType_HalSMBOR));
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
            }
        } else if (HSFCharCompare(i,ASCIIH('?'))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&inlineif,HalSMVariableType_HalSMInlineIf));
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Inline If (?) cannot without args")));
            }
        } else if (HSFCharCompare(i,ASCIIH(':'))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
                HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,&inlineelse,HalSMVariableType_HalSMInlineElse));
                HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                o=HalSMArray_init(hsmc->memorymanagmentlibrary);
            } else {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Inline Else (:) cannot without args")));
            }
        } else if (HSFCharCompare(i,ASCIIH('~'))) {
            if (o->size>0) {
                if (skip>0) {
                    HalSMArray_free(hsmc->memorymanagmentlibrary,o);
                    o=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    continue;
                }
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,&vbnot);
            }
        } else {HalSMArray_add(hsmc->memorymanagmentlibrary,o,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));}
    }

    if (o->size!=0) {
        if (skip==0) {
            buffs=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,o);
            buffo=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,buffs);
            HalSMCompiler_getArgsValue(hsmc,buffo,buffs,out,vars);
        }
        HalSMArray_free(hsmc->memorymanagmentlibrary,o);
    }

    HalSMCompiler_executeArgs(hsmc,&out,vars);
    HalSMArray_appendArray(hsmc->memorymanagmentlibrary,outarr,out);
    return outarr;
}

void HalSMCompiler_getArgsValue(HalSMCompiler* hsmc,HalSMVariable* buffo,HalStringFormatChar* buffs,HalSMArray* out,Dict* vars) {
    HalSMVariable* isGet,*vartemp;
    if (HSFCompare(buffs,U8H("true"))) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,1));
    } else if (HSFCompare(buffs,U8H("false"))) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_bool(hsmc->memorymanagmentlibrary,0));
    } else if (DictElementIndexByKey(hsmc,hsmc->variables,buffo)!=-1) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc,hsmc->variables,buffo)->value);
    } else if (DictElementIndexByKey(hsmc,vars,buffo)!=-1) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc,vars,buffo)->value);
    } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,buffo)!=-1) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc,hsmc->localFunctions,buffo)->value);
    } else if (DictElementIndexByKey(hsmc,hsmc->modules,buffo)!=-1) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc,hsmc->modules,buffo)->value);
    } else if (DictElementIndexByKey(hsmc,hsmc->classes,buffo)!=-1) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,DictElementFindByKey(hsmc,hsmc->classes,buffo)->value);
    } else if ((vartemp=hsmc->getVariable(*(HalStringFormatChar**)buffo->value))->type!=HalSMVariableType_HalSMNull) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,vartemp);
    } else if (HSFCompare(buffs,U8H("NULL"))||HSFCompare(buffs,U8H("null"))) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,&null);
    } else if (HalSMIsInt(hsmc->memorymanagmentlibrary,buffs)) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parse(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_int));
    } else if (HalSMIsDouble(hsmc->memorymanagmentlibrary,buffs)) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_parse(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_double));
    } else if (HalSMIsHex(hsmc->memorymanagmentlibrary,buffs)) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parseHex(hsmc->memorymanagmentlibrary,buffs,1),HalSMVariableType_int));
    } else if (HalSMIsBin(hsmc->memorymanagmentlibrary,buffs)) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parseBin(hsmc->memorymanagmentlibrary,buffs),HalSMVariableType_int));
    } else if (StringStartsWith(hsmc->memorymanagmentlibrary,buffs,U8H("["))&&StringEndsWith(hsmc->memorymanagmentlibrary,buffs,U8H("]"))) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMCompiler_getArgs(hsmc,SubString(hsmc->memorymanagmentlibrary,buffs,1,-1),vars),HalSMVariableType_HalSMArray));
    } else {
        isGet=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMVariable));
        if (HalSMCompiler_isGet(hsmc,buffs,isGet,0,vars)==1) {HalSMArray_add(hsmc->memorymanagmentlibrary,out,isGet);}
        else {HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Variable Not Found")));}
    }
}

unsigned char HalSMCompiler_isMore(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b)
{
    if (a->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)a->value)->type->isMore(hsmc,a,b);}
    else if (b->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)b->value)->type->isMore(hsmc,a,b);}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){return HalSMInteger_isMore((HalSMInteger*)a->value,(HalSMInteger*)b->value);}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_double){return HalSMDouble_isMore(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value,(HalSMDouble*)b->value);}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_double){return HalSMDouble_isMore(hsmc->memorymanagmentlibrary,HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value),(HalSMDouble*)b->value);}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_int){return HalSMDouble_isMore(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value,HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)b->value));}
    return 0;
}

unsigned char HalSMCompiler_isLess(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b)
{
    if (a->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)a->value)->type->isLess(hsmc,a,b);}
    else if (b->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)b->value)->type->isLess(hsmc,a,b);}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){return HalSMInteger_isLess((HalSMInteger*)a->value,(HalSMInteger*)b->value);}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_double){return HalSMDouble_isLess(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value,(HalSMDouble*)b->value);}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_double){return HalSMDouble_isLess(hsmc->memorymanagmentlibrary,HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value),(HalSMDouble*)b->value);}
    else if (a->type==HalSMVariableType_double&&b->type==HalSMVariableType_int){return HalSMDouble_isLess(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value,HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)b->value));}
    return 0;
}

HalSMVariable* HalSMCompiler_shift(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b,HalSMShift* shift)
{
    if (shift->c==0) {
        if (a->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)a->value)->type->shiftLeft(hsmc,a,b);}
        else if (b->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)b->value)->type->shiftLeft(hsmc,a,b);}
        else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_shiftLeft(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value,HalSMInteger_ToUnsignedInteger((HalSMInteger*)b->value)),HalSMVariableType_int);}
    } else if (shift->c==1) {
        if (a->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)a->value)->type->shiftRight(hsmc,a,b);}
        else if (b->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)b->value)->type->shiftRight(hsmc,a,b);}
        else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_shiftRight(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value,HalSMInteger_ToUnsignedInteger((HalSMInteger*)b->value)),HalSMVariableType_int);}
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Wrong arguments for shift operation")),HalSMVariableType_HalSMError);
}

HalSMVariable* HalSMCompiler_pow(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b)
{
    if (a->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)a->value)->type->pow(hsmc,a,b);}
    else if (b->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)b->value)->type->pow(hsmc,a,b);}
    else if (a->type==HalSMVariableType_int) {
        if (b->type==HalSMVariableType_int) {
            if (((HalSMInteger*)b->value)->negative) {
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_Pow(hsmc->memorymanagmentlibrary,HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value),(HalSMInteger*)b->value),HalSMVariableType_double);
            }
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_Pow(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value,(HalSMInteger*)b->value),HalSMVariableType_int);
        } else if (b->type==HalSMVariableType_double) {
            HalSMDouble* c=HalSMDouble_clear(hsmc->memorymanagmentlibrary,(HalSMDouble*)b->value),*v=HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value);
            HalSMDouble_PowSelf(hsmc->memorymanagmentlibrary,v,c->a);
            HalSMDouble_CropSelf(hsmc->memorymanagmentlibrary,v,10);
            HalSMDouble_SquareRootSelf(hsmc->memorymanagmentlibrary,v,c->b,10);
            if (c->negative) {
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_Div(hsmc->memorymanagmentlibrary,&HalSMDoubleOne,v),HalSMVariableType_double);
            }
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,v,HalSMVariableType_double);
        }
    }
    else if (a->type==HalSMVariableType_double) {
        if (b->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_Pow(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value,(HalSMInteger*)b->value),HalSMVariableType_double);
        } else if (b->type==HalSMVariableType_double) {
            HalSMDouble* c=HalSMDouble_clear(hsmc->memorymanagmentlibrary,(HalSMDouble*)b->value),*v=HalSMDouble_copy(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value);
            HalSMDouble_PowSelf(hsmc->memorymanagmentlibrary,v,c->a);
            HalSMDouble_SquareRootSelf(hsmc->memorymanagmentlibrary,v,c->b,10);
            if (c->negative) {
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_Div(hsmc->memorymanagmentlibrary,&HalSMDoubleOne,v),HalSMVariableType_double);
            }
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,v,HalSMVariableType_double);
        }
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Wrong arguments for pow operation")),HalSMVariableType_HalSMError);
}

HalSMVariable* HalSMCompiler_powFunc(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int||args->arr[0]->type==HalSMVariableType_double) {
            return HalSMCompiler_pow(hsmc,args->arr[0],HalSMVariable_init(hsmc->memorymanagmentlibrary,&HalSMIntegerTwo,HalSMVariableType_int));
        }
    } else if (args->size==2) {
        if ((args->arr[0]->type==HalSMVariableType_int||args->arr[0]->type==HalSMVariableType_double)&&(args->arr[1]->type==HalSMVariableType_int||args->arr[1]->type==HalSMVariableType_double)) {
            return HalSMCompiler_pow(hsmc,args->arr[0],args->arr[1]);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_bin(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            HalSMInteger* hi=(HalSMInteger*)args->arr[0]->value;
            if (hi->size==0){return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("0b0"));}
            HalStringFormatChar* out=hsmc->memorymanagmentlibrary->calloc(hi->size*8+3,sizeof(HalStringFormatChar));
            out[0]=ASCIIH('0');
            out[1]=ASCIIH('b');
            HalStringFormatChar* p=&out[2];
            unsigned long long int i;
            i=0;
            if (hi->value[0]<2) {
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else if (hi->value[0]<4) {
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else if (hi->value[0]<8) {
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else if (hi->value[0]<16) {
                *p++=ASCIIH('0'+((hi->value[i]>>3)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else if (hi->value[0]<32) {
                *p++=ASCIIH('0'+((hi->value[i]>>4)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>3)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else if (hi->value[0]<64) {
                *p++=ASCIIH('0'+((hi->value[i]>>5)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>4)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>3)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else if (hi->value[0]<128) {
                *p++=ASCIIH('0'+((hi->value[i]>>6)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>5)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>4)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>3)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            } else {
                *p++=ASCIIH('0'+((hi->value[i]>>7)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>6)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>5)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>4)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>3)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
            }
            
            for (i=1;i<hi->size;i++) {
                *p++=ASCIIH('0'+((hi->value[i]>>7)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>6)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>5)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>4)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>3)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>2)&1));
                *p++=ASCIIH('0'+((hi->value[i]>>1)&1));
                *p++=ASCIIH('0'+(hi->value[i]&1));
                //if (i==0){break;}
            }
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_hex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            HalStringFormatChar* s=HalSMInteger_toString(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value,1);
            if (string_len(s)>1&&HSFCharCompare(s[0],ASCIIH('0'))) {
                unsigned long long int l=string_len(s);
                memory_cpy(s,&s[1],l*sizeof(HalStringFormatChar)-sizeof(HalStringFormatChar));
                s[l-1]=ASCIIH('\0');
            }
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("0x"),s));
        } else if (args->arr[0]->type==HalSMVariableType_double) {
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("0x"),HalSMDouble_toString(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value,1)));
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_modulo(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b)
{
    if (a->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)a->value)->type->modulo(hsmc,a,b);}
    else if (b->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)b->value)->type->modulo(hsmc,a,b);}
    else if (a->type==HalSMVariableType_int&&b->type==HalSMVariableType_int){
        HalSMInteger* rem=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMInteger));
        HalSMInteger_Div(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value,(HalSMInteger*)b->value,rem);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,rem,HalSMVariableType_int);
    }
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Wrong arguments for modulo operation")),HalSMVariableType_HalSMError);
}

unsigned char HalSMCompiler_getBool(HalSMCompiler* hsmc,HalSMVariable* v)
{
    if(v->type==HalSMVariableType_custom){return ((HalSMVariableCustom*)v->value)->type->getBool(hsmc,v);}
    else if(v->type==HalSMVariableType_int){return HalSMInteger_isEqual((HalSMInteger*)v->value,&HalSMIntegerZero)==0;}
    else if(v->type==HalSMVariableType_double){return HalSMDouble_isEqual((HalSMDouble*)v->value,&HalSMDoubleZero)==0;}
    else if(v->type==HalSMVariableType_char){return (*(HalStringFormatChar*)v->value).alphabet>0||(*(HalStringFormatChar*)v->value).symbol>0;}
    else if(v->type==HalSMVariableType_void){return 0;}
    else if(v->type==HalSMVariableType_HalSMArray){return ((HalSMArray*)v->value)->size>0;}
    else if(v->type==HalSMVariableType_str){return string_len(*(HalStringFormatChar**)v->value)>0;}
    else if(v->type==HalSMVariableType_int_array){return 1;}
    else if(v->type==HalSMVariableType_HalSMFunctionC){return 1;}
    else if(v->type==HalSMVariableType_HalSMClassC){return 1;}
    else if(v->type==HalSMVariableType_HalSMRunClassC){return 1;}
    else if(v->type==HalSMVariableType_HalSMSetArg){return 1;}
    else if(v->type==HalSMVariableType_HalSMError){return string_len(((HalSMError*)v->value)->error)>0;}
    else if(v->type==HalSMVariableType_HalSMNull){return 0;}
    else if(v->type==HalSMVariableType_HalSMRunFunc){return 1;}
    else if(v->type==HalSMVariableType_HalSMRunFuncC){return 1;}
    else if(v->type==HalSMVariableType_HalSMLocalFunction){return 1;}
    else if(v->type==HalSMVariableType_HalSMCModule){return 1;}
    else if(v->type==HalSMVariableType_HalSMModule){return 1;}
    else if(v->type==HalSMVariableType_HalSMCompiler){return 1;}
    else if(v->type==HalSMVariableType_HalSMRunClass){return 1;}
    else if(v->type==HalSMVariableType_HalSMDoubleGet){return string_len(((HalSMDoubleGet*)v->value)->st)>0;}
    else if(v->type==HalSMVariableType_HalSMClass){return 1;}
    else if(v->type==HalSMVariableType_HalSMVar){return 1;}
    else if(v->type==HalSMVariableType_HalSMPlus){return 1;}
    else if(v->type==HalSMVariableType_HalSMMinus){return 0;}
    else if(v->type==HalSMVariableType_HalSMMult){return 1;}
    else if(v->type==HalSMVariableType_HalSMDivide){return 1;}
    else if(v->type==HalSMVariableType_HalSMEqual){return 1;}
    else if(v->type==HalSMVariableType_HalSMNotEqual){return 1;}
    else if(v->type==HalSMVariableType_HalSMMore){return 1;}
    else if(v->type==HalSMVariableType_HalSMLess){return 1;}
    else if(v->type==HalSMVariableType_HalSMBool){return *(unsigned char*)v->value;}
    else if(v->type==HalSMVariableType_HalSMDict){return ((Dict*)v->value)->size>0;}
    else if(v->type==HalSMVariableType_HalSMSetVar){return 1;}
    else if(v->type==HalSMVariableType_HalSMReturn){return 1;}
    else if(v->type==HalSMVariableType_HalSMFunctionCTypeDef){return 1;}
    else if(v->type==HalSMVariableType_HalSMFunctionArray){return 1;}
    else if(v->type==HalSMVariableType_unsigned_int){return (*(unsigned long long int*)v->value)!=0;}
    else if(v->type==HalSMVariableType_HalSMMoreEqual){return 1;}
    else if(v->type==HalSMVariableType_HalSMLessEqual){return 1;}
    else if(v->type==HalSMVariableType_HalSMShift){return 1;}
    else if(v->type==HalSMVariableType_HalSMPow){return 1;}
    else if(v->type==HalSMVariableType_HalSMModulo){return 1;}
    else if(v->type==HalSMVariableType_HalSMAND){return 1;}
    else if(v->type==HalSMVariableType_HalSMOR){return 1;}
    else if(v->type==HalSMVariableType_HalSMNegate){return 1;}
    else if(v->type==HalSMVariableType_HalSMNegativeSign){return 1;}
    else if(v->type==HalSMVariableType_HalSMGOTO){return 1;}
    else if(v->type==HalSMVariableType_HalSMGOTOSector){return 1;}
    else if(v->type==HalSMVariableType_HalSMInlineIf){return 1;}
    else if(v->type==HalSMVariableType_HalSMInlineElse){return 1;}
    else if(v->type==HalSMVariableType_HalSMBreak){return 1;}
    else if(v->type==HalSMVariableType_HalSMXOR){return 1;}
    else if(v->type==HalSMVariableType_HalSMBAND){return 1;}
    else if(v->type==HalSMVariableType_HalSMBOR){return 1;}
    else if(v->type==HalSMVariableType_HalSMBNOT){return 1;}
    return 0;
}

HalSMVariable* HalSMCompiler_negate(HalSMCompiler* hsmc,HalSMVariable* v)
{
    if(v->type==HalSMVariableType_int){
        HalSMInteger* out=HalSMInteger_copy(hsmc->memorymanagmentlibrary,(HalSMInteger*)v->value);
        out->negative=out->negative?0:1;
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_int);
    } else if (v->type==HalSMVariableType_double) {
        HalSMDouble* out=HalSMDouble_copy(hsmc->memorymanagmentlibrary,(HalSMDouble*)v->value);
        out->negative=out->negative?0:1;
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_double);
    }
    return v;
}



HalSMVariable* HalSMCompiler_replace(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==3&&args->arr[0]->type==HalSMVariableType_str&&args->arr[1]->type==HalSMVariableType_str&&args->arr[2]->type==HalSMVariableType_str) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value,*(HalStringFormatChar**)args->arr[1]->value,*(HalStringFormatChar**)args->arr[2]->value));
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_index(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2) {
        if (args->arr[0]->type==HalSMVariableType_str&&args->arr[1]->type==HalSMVariableType_str) {
            long long int res=StringIndexOf(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value,*(HalStringFormatChar**)args->arr[1]->value);
            if (res==-1){return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,-1));}
            return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,res));
        } else if (args->arr[0]->type==HalSMVariableType_HalSMArray) {
            signed long long int s=(signed long long int)HalSMArray_index(hsmc,(HalSMArray*)args->arr[0]->value,args->arr[1]);
            return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,s));
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_rindex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2) {
        if (args->arr[0]->type==HalSMVariableType_str&&args->arr[1]->type==HalSMVariableType_str) {
            long long int res=StringLastIndexOf(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value,*(HalStringFormatChar**)args->arr[1]->value);
            if (res==-1) {return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,-1));}
            return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,res));
        } else if (args->arr[0]->type==HalSMVariableType_HalSMArray) {
            long long int res=HalSMArray_right_index(hsmc,(HalSMArray*)args->arr[0]->value,args->arr[1]);
            if (res==-1) {return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,-1));}
            return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,res));
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_split(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_str&&args->arr[1]->type==HalSMVariableType_str) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_split_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value,*(HalStringFormatChar**)args->arr[1]->value),HalSMVariableType_HalSMArray);
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_substring(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==3&&args->arr[0]->type==HalSMVariableType_str&&args->arr[1]->type==HalSMVariableType_int&&args->arr[2]->type==HalSMVariableType_int) {
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubStringHalSMInteger(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value,(HalSMInteger*)args->arr[1]->value,(HalSMInteger*)args->arr[2]->value));
    }
    return &hvoid;
}



HalSMVariable* HalSMCompiler_append(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMArray) {
        HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value,args->arr[1]);
        return args->arr[0];
    } else if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_HalSMDict) {
        PutDictElementToDict(hsmc,(Dict*)args->arr[0]->value,(DictElement*)args->arr[1]->value);
        return args->arr[0];
    } else if (args->size==3&&args->arr[0]->type==HalSMVariableType_HalSMDict) {
        PutDictElementToDict(hsmc,(Dict*)args->arr[0]->value,DictElementInit(hsmc->memorymanagmentlibrary,args->arr[1],args->arr[2]));
        return args->arr[0];
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_clear(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_HalSMArray) {
            ((HalSMArray*)args->arr[0]->value)->arr=hsmc->memorymanagmentlibrary->malloc(0);
            ((HalSMArray*)args->arr[0]->value)->size=0;
            return args->arr[0];
        } else if (args->arr[0]->type==HalSMVariableType_HalSMDict) {
            ((Dict*)args->arr[0]->value)->elements=hsmc->memorymanagmentlibrary->malloc(0);
            ((Dict*)args->arr[0]->value)->size=0;
            return args->arr[0];
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_insert(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==3&&args->arr[0]->type==HalSMVariableType_HalSMArray&&args->arr[2]->type==HalSMVariableType_int) {
        HalSMArray_insert(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value,args->arr[1],HalSMInteger_ToUnsignedInteger((HalSMInteger*)args->arr[2]->value));
        return args->arr[0];
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_pop(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMArray&&args->arr[1]->type==HalSMVariableType_int) {
        unsigned long long int index=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        if (index<((HalSMArray*)args->arr[0]->value)->size) {
            //HalSMVariable* temp=((HalSMArray*)args->arr[0]->value)->arr[index];
            HalSMArray_remove(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value,index);
            //return temp;
        }
        return args->arr[0];
    } else if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_int) {
        unsigned long long int index=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        if (index<((Dict*)args->arr[0]->value)->size) {
            //Dict* temp=DictInitWithElements(hsmc->memorymanagmentlibrary,(DictElement*[]){((Dict*)args->arr[0]->value)->elements[index]},1);
            DictRemove(hsmc->memorymanagmentlibrary,(Dict*)args->arr[0]->value,index);
            //return HalSMVariable_init(hsmc->memorymanagmentlibrary,temp,HalSMVariableType_HalSMDict);
        }
        return args->arr[0];
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_remove(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMArray) {
        HalSMArray* arrarg=(HalSMArray*)args->arr[0]->value;
        for (unsigned long long int i=0;i<arrarg->size;i++) {
            if (HalSMVariable_Compare(hsmc,args->arr[1],arrarg->arr[i])) {
                HalSMArray_remove(hsmc->memorymanagmentlibrary,arrarg,i);
                return args->arr[0];
            }
        }
        return args->arr[0];
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_reverse(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1&&args->arr[0]->type==HalSMVariableType_HalSMArray){return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_reverse(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value),HalSMVariableType_HalSMArray);}
    return &hvoid;
}

HalSMVariable* HalSMCompiler_copy(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1&&args->arr[0]->type==HalSMVariableType_HalSMArray){return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_copy(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value),HalSMVariableType_HalSMArray);}
    return &hvoid;
}

HalSMVariable* HalSMCompiler_appendArray(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMArray&&args->arr[1]->type==HalSMVariableType_HalSMArray){HalSMArray_appendArray(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value,(HalSMArray*)args->arr[1]->value);return args->arr[0];}
    return &hvoid;
}

HalSMVariable* HalSMCompiler_slice(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==3&&args->arr[0]->type==HalSMVariableType_HalSMArray&&args->arr[1]->type==HalSMVariableType_int&&args->arr[2]->type==HalSMVariableType_int) {
        HalSMInteger* start=(HalSMInteger*)args->arr[1]->value;
        HalSMInteger* end=(HalSMInteger*)args->arr[2]->value;
        if (HalSMInteger_isLess(start,&HalSMIntegerZero)) {
            start=HalSMInteger_Add(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,((HalSMArray*)args->arr[0]->value)->size),start);
        }
        
        if (HalSMInteger_isLess(start,&HalSMIntegerZero)) {
            return &null;
        }

        if (HalSMInteger_isLess(end,&HalSMIntegerZero)) {
            end=HalSMInteger_Add(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,((HalSMArray*)args->arr[0]->value)->size),end);
        }
        
        if (HalSMInteger_isLess(end,&HalSMIntegerZero)) {
            return &null;
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value,HalSMInteger_ToUnsignedInteger(start),HalSMInteger_ToUnsignedInteger(end)),HalSMVariableType_HalSMArray);
    }
    return &hvoid;
}



HalSMVariable* HalSMCompiler_len(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size>0) {
        HalSMInteger* out=HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,0);
        unsigned long long int i;
        HalSMVariable* tempk=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__len__"));
        HalSMVariable* f;
        for (i=0;i<args->size;i++) {
            if (args->arr[i]->type==HalSMVariableType_HalSMArray){HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,((HalSMArray*)args->arr[i]->value)->size));}
            else if (args->arr[i]->type==HalSMVariableType_str){HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,string_len(*(HalStringFormatChar**)args->arr[i]->value)));}
            else if (args->arr[i]->type==HalSMVariableType_custom){
                if (((HalSMVariableCustom*)args->arr[i]->value)->type->indexFunctionByName(hsmc,args->arr[i],U8H("__len__"))!=-1) {
                    f=((HalSMVariableCustom*)args->arr[i]->value)->type->getFunctionByName(hsmc,args->arr[i],U8H("__len__"));
                    if (f->type==HalSMVariableType_HalSMLocalFunction) {
                        f=HalSMLocalFunction_run((HalSMLocalFunction*)f->value,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){args->arr[i]},1),vars);
                        if (f->type==HalSMVariableType_HalSMError){return f;}
                    } else if (f->type==HalSMVariableType_HalSMFunctionC) {
                        f=HalSMFunctionC_run((HalSMFunctionC*)args->arr[i]->value,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){args->arr[i]},1),vars);
                        if (f->type==HalSMVariableType_HalSMError){return f;}
                    }
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMClass) {
                if (DictElementIndexByKey(hsmc,((HalSMClass*)args->arr[i]->value)->funcs,tempk)!=-1) {
                    f=HalSMLocalFunction_run((HalSMLocalFunction*)DictElementFindByKey(hsmc,((HalSMClass*)args->arr[i]->value)->funcs,tempk)->value->value,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    } else if (f->type==HalSMVariableType_HalSMError) {
                        return f;
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMClassC) {
                if (DictElementIndexByKey(hsmc,((HalSMClassC*)args->arr[i]->value)->funcs,tempk)!=-1) {
                    f=HalSMFunctionC_run((HalSMFunctionC*)DictElementFindByKey(hsmc,((HalSMClassC*)args->arr[i]->value)->funcs,tempk)->value->value,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    } else if (f->type==HalSMVariableType_HalSMError) {
                        return f;
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMRunClass) {
                if (DictElementIndexByKey(hsmc,((HalSMRunClass*)args->arr[i]->value)->funcs,tempk)!=-1) {
                    f=HalSMLocalFunction_run((HalSMLocalFunction*)DictElementFindByKey(hsmc,((HalSMRunClass*)args->arr[i]->value)->funcs,tempk)->value->value,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){args->arr[i]},1),vars);
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    } else if (f->type==HalSMVariableType_HalSMError) {
                        return f;
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMRunClassC) {
                if (DictElementIndexByKey(hsmc,((HalSMRunClassC*)args->arr[i]->value)->funcs,tempk)!=-1) {
                    f=HalSMFunctionC_run((HalSMFunctionC*)DictElementFindByKey(hsmc,((HalSMRunClassC*)args->arr[i]->value)->funcs,tempk)->value->value,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){args->arr[i]},1),vars);
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    } else if (f->type==HalSMVariableType_HalSMError) {
                        return f;
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMModule) {
                if (DictElementIndexByKey(hsmc,((HalSMModule*)args->arr[i]->value)->hsmc->localFunctions,tempk)!=-1) {
                    f=HalSMLocalFunction_run((HalSMLocalFunction*)DictElementFindByKey(hsmc,((HalSMModule*)args->arr[i]->value)->hsmc->localFunctions,tempk)->value->value,HalSMArray_init(hsmc->memorymanagmentlibrary),vars);
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    } else if (f->type==HalSMVariableType_HalSMError) {
                        return f;
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMCModule) {
                if (DictElementIndexByKey(hsmc,((HalSMCModule*)args->arr[i]->value)->lfuncs,tempk)!=-1) {
                    f=HalSMFunctionC_run((HalSMFunctionC*)DictElementFindByKey(hsmc,((HalSMCModule*)args->arr[i]->value)->lfuncs,tempk)->value->value,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){args->arr[i]},1),vars);
                    if (f->type==HalSMVariableType_int) {
                        HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,(HalSMInteger*)f->value);
                    } else if (f->type==HalSMVariableType_HalSMError) {
                        return f;
                    }
                }
            } else if (args->arr[i]->type==HalSMVariableType_char) {
                HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,&HalSMIntegerOne);
            } else if (args->arr[i]->type==HalSMVariableType_HalSMBool) {
                if ((*(unsigned char*)args->arr[i]->value)==1) {
                    HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,&HalSMIntegerOne);
                }
            } else if (args->arr[i]->type==HalSMVariableType_HalSMDict) {
                HalSMInteger_AddSelf(hsmc->memorymanagmentlibrary,out,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,((Dict*)args->arr[i]->value)->size));
            }
        }
        hsmc->memorymanagmentlibrary->free(*(HalStringFormatChar**)tempk->value);
        hsmc->memorymanagmentlibrary->free(tempk->value);
        hsmc->memorymanagmentlibrary->free(tempk);
        return HalSMVariable_init_int(hsmc->memorymanagmentlibrary,out);
    }
    return &hvoid;
}



HalSMVariable* HalSMCompiler_appendDict(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_HalSMDict){DictAppendDict(hsmc->memorymanagmentlibrary,(Dict*)args->arr[0]->value,(Dict*)args->arr[1]->value);return args->arr[0];}
    return &hvoid;
}

HalSMVariable* HalSMCompiler_indexByKey(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict) {
        long long int r=DictElementIndexByKey(hsmc,(Dict*)args->arr[0]->value,args->arr[1]);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,r<0?HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,-1):HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,(unsigned long long int)r),HalSMVariableType_int);
    }
    return &hvoid;
}
HalSMVariable* HalSMCompiler_indexByValue(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict) {
        long long int r=DictElementIndexByValue(hsmc,(Dict*)args->arr[0]->value,args->arr[1]);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,r<0?HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,(signed int)r):HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,(unsigned long long int)r),HalSMVariableType_int);
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_keyByIndex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_int) {
        long long int ind=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        if (((HalSMInteger*)args->arr[1]->value)->negative){ind=-ind;}
        if (ind<0) {
            ind=((Dict*)args->arr[0]->value)->size+ind;
        }
        if (ind<0||ind>=((Dict*)args->arr[0]->value)->size){return &null;}
        return ((Dict*)args->arr[0]->value)->elements[ind]->key;
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_valueByIndex(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_int) {
        long long int ind=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        if (((HalSMInteger*)args->arr[1]->value)->negative){ind=-ind;}
        if (ind<0) {
            ind=((Dict*)args->arr[0]->value)->size+ind;
        }
        if (ind<0||ind>=((Dict*)args->arr[0]->value)->size){return &null;}
        return ((Dict*)args->arr[0]->value)->elements[ind]->value;
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_set(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==3&&args->arr[0]->type==HalSMVariableType_HalSMArray&&args->arr[1]->type==HalSMVariableType_int) {
        unsigned long long int i=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        if (i<((HalSMArray*)args->arr[0]->value)->size) {
            ((HalSMArray*)args->arr[0]->value)->arr[i]=args->arr[2];
        }
        return args->arr[0];
    } else if (args->size==3&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_int&&args->arr[2]->type==HalSMVariableType_HalSMDict) {
        unsigned long long int i=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        if (i<((Dict*)args->arr[0]->value)->size&&((Dict*)args->arr[2]->value)->size>0) {
            ((Dict*)args->arr[0]->value)->elements[i]=((Dict*)args->arr[2]->value)->elements[0];
        }
        return args->arr[0];
    } else if (args->size==2&&args->arr[0]->type==HalSMVariableType_HalSMDict&&args->arr[1]->type==HalSMVariableType_HalSMDict) {
        if (((Dict*)args->arr[1]->value)->size>0) {
            PutDictElementToDict(hsmc,(Dict*)args->arr[0]->value,((Dict*)args->arr[1]->value)->elements[0]);
        }
        return args->arr[0];
    }
    return &hvoid;
}



HalSMVariable* HalSMCompiler_sqrt(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if ((args->size>0)&&(args->arr[0]->type==HalSMVariableType_int||args->arr[0]->type==HalSMVariableType_double)) {
        HalSMDouble* a;
        HalSMInteger* q=&HalSMIntegerTwo;
        unsigned long long int crop=10;
        if (args->arr[0]->type==HalSMVariableType_int) {
            a=HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value);
        } else {
            a=(HalSMDouble*)args->arr[0]->value;
        }
        if (args->size>1&&args->arr[1]->type==HalSMVariableType_int) {
            q=(HalSMInteger*)args->arr[1]->value;
        }

        if (args->size>2&&args->arr[2]->type==HalSMVariableType_int) {
            crop=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[2]->value);
        }

        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_SquareRoot(hsmc->memorymanagmentlibrary,a,q,crop),HalSMVariableType_double);
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_int(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_copy(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value),HalSMVariableType_int);
        } else if (args->arr[0]->type==HalSMVariableType_double) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromHalSMDouble(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value),HalSMVariableType_int);
        } else if (args->arr[0]->type==HalSMVariableType_str) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parse(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value),HalSMVariableType_int);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_double(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_double) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_copy(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value),HalSMVariableType_double);
        } else if (args->arr[0]->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_FromHalSMInteger(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value),HalSMVariableType_double);
        } else if (args->arr[0]->type==HalSMVariableType_str) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_parse(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value),HalSMVariableType_double);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_str(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size>0&&args->size<3) {
        if (args->arr[0]->type==HalSMVariableType_str) {
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)args->arr[0]->value);
        } else if (args->arr[0]->type==HalSMVariableType_custom) {
            if (HSFCompare(((HalSMVariableCustom*)args->arr[0]->value)->type->name,U8H("ByteArray"))) {
                unsigned int type=0;
                if (args->size==2&&args->arr[1]->type==HalSMVariableType_str) {
                    HalStringFormatChar* stype=*(HalStringFormatChar**)args->arr[1]->value;
                    if (HSFCompare(stype,U8H("utf8"))) {
                        type=1;
                    } else if (HSFCompare(stype,U8H("utf16"))) {
                        type=2;
                    }
                }
                
                HalSMByteArray* ba=(HalSMByteArray*)((HalSMVariableCustom*)args->arr[0]->value)->value;
                HalStringFormatChar* out=NOTHALSMNULLPOINTER;
                if (ba->size==0) {
                    out=U8H("");
                } else if (type==0) {
                    if (ba->size%4==0) {
                        out=(HalStringFormatChar*)ba->bytes;
                    }
                } else if (type==1) {
                    out=U8H((char*)ba->bytes);
                } else if (type==2) {
                    out=U16H((short*)ba->bytes);
                }
                if (out!=NOTHALSMNULLPOINTER) {
                    return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
                }
            }
        }
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMVariable_to_str(hsmc,args->arr[0],0));
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_floor(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_copy(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value),HalSMVariableType_int);
        } else if (args->arr[0]->type==HalSMVariableType_double) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FloorHalSMDouble(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value),HalSMVariableType_int);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_ceil(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_copy(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value),HalSMVariableType_int);
        } else if (args->arr[0]->type==HalSMVariableType_double) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_CeilHalSMDouble(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value),HalSMVariableType_int);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_round(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_copy(hsmc->memorymanagmentlibrary,(HalSMInteger*)args->arr[0]->value),HalSMVariableType_int);
        } else if (args->arr[0]->type==HalSMVariableType_double&&args->arr[1]->type==HalSMVariableType_int) {
            if (HalSMInteger_isEqual((HalSMInteger*)args->arr[1]->value,&HalSMIntegerZero)){return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_CeilHalSMDouble(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value),HalSMVariableType_int);}
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_parse(hsmc->memorymanagmentlibrary,HalSMDouble_toStringRound(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value,HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value))),HalSMVariableType_double);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_abs(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_int) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_copy(hsmc->memorymanagmentlibrary,HalSMInteger_absolute((HalSMInteger*)args->arr[0]->value)),HalSMVariableType_int);
        } else if (args->arr[0]->type==HalSMVariableType_double) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_Abs(hsmc->memorymanagmentlibrary,(HalSMDouble*)args->arr[0]->value),HalSMVariableType_double);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_join(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2) {
        if (args->arr[0]->type==HalSMVariableType_HalSMArray&&args->arr[1]->type==HalSMVariableType_str) {
            HalSMArray* harr=(HalSMArray*)args->arr[0]->value;
            HalStringFormatChar* join=*(HalStringFormatChar**)args->arr[1]->value;
            if(harr->size==0) {
                return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H(""));
            } else if (harr->size==1) {
                return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMVariable_to_str(hsmc,harr->arr[0]->value,1));
            }
            unsigned long long int size=0;
            unsigned long long int lj=string_len(join);
            HalStringFormatChar* out=hsmc->memorymanagmentlibrary->calloc(1,sizeof(HalStringFormatChar));
            out[0]=ASCIIH('\0');
            unsigned long long int i;
            size=0;
            for (i=0;i<harr->size;i++) {
                HalStringFormatChar* oc=HalSMVariable_to_str(hsmc,harr->arr[i],1);
                AdditionStrings(hsmc->memorymanagmentlibrary,&out,oc,size,string_len(oc));
                size+=string_len(oc);
                if (i!=harr->size-1) {
                    AdditionStrings(hsmc->memorymanagmentlibrary,&out,join,size,lj);
                    size+=lj;
                }
            }
            out[size]=ASCIIH('\0');
            return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_array(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_HalSMArray) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_copy(hsmc->memorymanagmentlibrary,(HalSMArray*)args->arr[0]->value),HalSMVariableType_HalSMArray);
        } else if (args->arr[0]->type==HalSMVariableType_custom) {
            HalSMVariableCustom* vc=(HalSMVariableCustom*)args->arr[0]->value;
            return vc->type->convert(hsmc,args->arr[0],&HNULLArray);
        } else if (args->arr[0]->type==HalSMVariableType_str) {
            HalStringFormatChar* str=*(HalStringFormatChar**)args->arr[0]->value;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_from_str(hsmc->memorymanagmentlibrary,str,string_len(str)),HalSMVariableType_HalSMArray);
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_dir(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        HalSMArray* out;
        Dict* d;
        unsigned long long int i;
        if (args->arr[0]->type==HalSMVariableType_str) {
            out=HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("replace")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("index")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("rindex")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("split")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("substring")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("len"))},6);
        } else if (args->arr[0]->type==HalSMVariableType_HalSMArray) {
            out=HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("append")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("len")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("clear")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("index")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("insert")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("pop")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("remove")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("reverse")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("copy")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("appendArray")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("slice")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("set")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("join")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("rindex"))},14);
        } else if (args->arr[0]->type==HalSMVariableType_HalSMDict) {
            out=HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("append")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("appendDict")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("len")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("clear")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("indexByKey")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("indexByValue")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("set")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("pop")),
                                                                                              HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("keyByIndex")),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("valueByIndex"))},10);
        } else if (args->arr[0]->type==HalSMVariableType_custom) {
            out=HalSMArray_init(hsmc->memorymanagmentlibrary);
            d=((HalSMVariableCustom*)args->arr[0]->value)->type->funcs;
            for (i=0;i<d->size;i++) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,d->elements[i]->key);
            }
            d=((HalSMVariableCustom*)args->arr[0]->value)->type->vars;
            for (i=0;i<d->size;i++) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,d->elements[i]->key);
            }
        } else {
            out=HalSMArray_init(hsmc->memorymanagmentlibrary);
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_dict(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        if (args->arr[0]->type==HalSMVariableType_HalSMDict) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,DictCopy(hsmc->memorymanagmentlibrary,(Dict*)args->arr[0]->value),HalSMVariableType_HalSMDict);
        } else if (args->arr[0]->type==HalSMVariableType_str) {
            HalStringFormatChar* temp=*(HalStringFormatChar**)args->arr[0]->value,*tempv;
            if (HSFCharCompare(temp[0],ASCIIH('{'))&&HSFCharCompare(temp[string_len(temp)-1],ASCIIH('}'))) {
                if (string_len(temp)==2) {
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,DictInit(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMDict);
                }
                tempv=hsmc->memorymanagmentlibrary->malloc(string_len(temp)*sizeof(HalStringFormatChar)-sizeof(HalStringFormatChar));
                stringncpy(tempv,&temp[1],string_len(temp)-2);
                Dict* out=HalSMCompiler_getArgsDict(hsmc,tempv,vars);
                hsmc->memorymanagmentlibrary->free(tempv);
                if (out==&NULLDict){return &null;}
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMDict);
            }
        }
    }
    return &hvoid;
}



HalSMVariable* HalSMCompiler_getNameFunction(HalSMCompiler* hsmc,HalStringFormatChar* l)
{
    HalSMArray* name=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalStringFormatChar s;
    unsigned long long int ll=string_len(l);

    for (unsigned long long int i=0;i<ll;i++) {
        s=l[i];
        if (HSFCharCompare(s,ASCIIH('('))) {
            HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,name)));
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,l,i+1,string_len(l)-1)));
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
        }
        HalSMArray_add(hsmc->memorymanagmentlibrary,name,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,s));
    }

    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("This is not Function")));
    return &null;
}

HalSMVariable* HalSMCompiler_isSetVar(HalSMCompiler* hsmc,HalStringFormatChar* l,Dict* vars)
{
    if (StringStartsWith(hsmc->memorymanagmentlibrary,l,U8H("def "))||StringStartsWith(hsmc->memorymanagmentlibrary,l,U8H("return "))||StringEndsWith(hsmc->memorymanagmentlibrary,l,U8H(":"))){return &null;}
    HalStringFormatChar i;
    HalSMArray* n=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned long long int ll=string_len(l);
    /*if (StringIndexOf(hsmc->memorymanagmentlibrary,l,U8H("++"))!=-1||StringIndexOf(hsmc->memorymanagmentlibrary,l,U8H("--"))!=-1) {
        return &null;
    }*/

    unsigned long long int isA=0,isF=0,isD=0;
    HalStringFormatChar isS=ASCIIH('n');

    for (unsigned long long int ind=0;ind<ll;ind++) {
        i=l[ind];
        
        if (isA) {
            if (HSFCharCompare(i,ASCIIH('['))) {
                isA++;
            } else if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
            }
        } else if (isF) {
            if (HSFCharCompare(i,ASCIIH('('))) {
                isF++;
            } else if (HSFCharCompare(i,ASCIIH(')'))) {
                isF--;
            }

        } else if (HSFCharCompare(isS,ASCIIH('n'))==0) {
            if (HSFCharCompare(i,isS)){isS=ASCIIH('n');}
        } else if (isD) {
            if (HSFCharCompare(i,ASCIIH('{'))) {
                isD++;
            } else if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
            }
        } else if (HSFCharCompare(i,ASCIIH('\''))||HSFCharCompare(i,ASCIIH('"'))) {
            isS=i;
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            isA++;
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            isD++;
        } else if (HSFCharCompare(i,ASCIIH('('))) {
            isF++;
        } else if (HSFCharCompare(i,ASCIIH('='))&&n->size>0) {
            if (ind>1&&HSFCharCompare(l[ind-1],ASCIIH('*'))&&HSFCharCompare(l[ind-2],ASCIIH('*'))) {
                HalSMArray_remove(hsmc->memorymanagmentlibrary,n,n->size-1);
                HalSMArray_remove(hsmc->memorymanagmentlibrary,n,n->size-1);
                l=ConcatenateStrings(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n),U8H("**")),SubString(hsmc->memorymanagmentlibrary,l,ind+1,string_len(l)));
            } else if (HSFCharCompare(l[ind-1],ASCIIH('+'))||HSFCharCompare(l[ind-1],ASCIIH('-'))||HSFCharCompare(l[ind-1],ASCIIH('*'))||HSFCharCompare(l[ind-1],ASCIIH('/'))||HSFCharCompare(l[ind-1],ASCIIH('%'))||HSFCharCompare(l[ind-1],ASCIIH('^'))||HSFCharCompare(l[ind-1],ASCIIH('|'))||HSFCharCompare(l[ind-1],ASCIIH('&'))) {
                HalStringFormatChar temp[2];
                temp[0]=l[ind-1];
                temp[1]=ASCIIH('\0');
                HalSMArray_remove(hsmc->memorymanagmentlibrary,n,n->size-1);
                l=ConcatenateStrings(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n),temp),SubString(hsmc->memorymanagmentlibrary,l,ind+1,string_len(l)));
            } else {
                l=SubString(hsmc->memorymanagmentlibrary,l,ind+1,string_len(l));
            }
            HalSMFunctionArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
            out->args=HalSMArray_init(hsmc->memorymanagmentlibrary);
            out->type=HalSMFunctionArrayType_var;
            HalSMArray_add(hsmc->memorymanagmentlibrary,out->args,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n)));
            HalSMArray_add(hsmc->memorymanagmentlibrary,out->args,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,l));
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
        }
        HalSMArray_add(hsmc->memorymanagmentlibrary,n,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
    }
    return &null;
}

unsigned char HalSMCompiler_isNull(HalStringFormatChar* text)
{
    HalStringFormatChar i;
    unsigned long long int lt=string_len(text);
    for (unsigned long long int ind=0;ind<lt;ind++) {
        i=text[ind];
        if (HSFCharCompare(i,ASCIIH(' '))==0&&HSFCharCompare(i,ASCIIH('\r'))==0){return 0;}
    }
    return 1;
}

HalStringFormatChar* HalSMCompiler_getPath(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* a,HalStringFormatChar* b)
{
    if (string_len(a)==0){return b;}
    if (HSFCharCompare(a[string_len(a)-1],ASCIIH('/'))) {
        return ConcatenateStrings(hsmmml,a,b);
    }
    return ConcatenateStrings(hsmmml,a,ConcatenateStrings(hsmmml,U8H("/"),b));
}

HalSMVariable* HalSMCompiler_compileList(HalSMCompiler* hsmc,unsigned char isConsole,HalSMVariable* module,HalSMArray* allLines) {
    unsigned char isFunc=0;
    unsigned char isClass=0;
    HalSMVariable* func=&null;
    HalSMVariable* cls=&null;
    HalSMArray* tabs=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalSMVariable* isSetVar;
    HalStringFormatChar* tabsS;
    unsigned long long int tabsC;
    unsigned char isNull=0;
    HalSMLocalFunction* f;
    HalSMVariable* fc=&null;
    HalSMArray* nameFunc;
    HalSMArray* spliti;
    HalStringFormatChar* joinSpliti;
    HalSMArray* b;
    HalSMFunctionArray* resRunFunc;
    HalSMVariable* err;
    HalSMArray* allLinesTemp;
    HalStringFormatChar* l;
    unsigned long long int la;
    HalSMVariable* funcColon=&null;
    HalSMArray* funcColonArgs;
    unsigned char isFuncColon=0;
    HalSMVariable* groupFunctions=&null;
    HalSMVariable* groupFunc=&null;
    HalSMArray* arrayGroupFunc=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned long long int prevTabsC=0;
    HalSMArray* functions=HalSMArray_init(hsmc->memorymanagmentlibrary);

    //0.1.5
    HalSMVariable* isGOTO=&null;
    unsigned long long lastIndex=allLines->size-1;

    for (;hsmc->indexl<allLines->size;hsmc->indexl++) {
        hsmc->curInd=hsmc->ind;
        allLinesTemp=(HalSMArray*)allLines->arr[hsmc->indexl]->value;
        l=*(HalStringFormatChar**)allLinesTemp->arr[0]->value;
        tabsC=*(unsigned long long int*)allLinesTemp->arr[1]->value;
        tabsS=l;
        isSetVar=HalSMCompiler_isSetVar(hsmc,tabsS,&NULLDict);
        isNull=HalSMCompiler_isNull(tabsS);

        if (tabsC!=prevTabsC&&!isNull) {
            groupFunctions=&null;
            groupFunc=&null;
            long long int sizeArrayGroupFunc=arrayGroupFunc->size;
            for (int indexGF=0;indexGF<sizeArrayGroupFunc;indexGF++) {
                if (*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC||*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value+1==tabsC) {
                    groupFunctions=((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[0];
                    groupFunc=((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[1];
                    break;
                } else if (tabsC<*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value) {
                    HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,arrayGroupFunc->size-1);
                }
            }
        }

        if (isClass) {
            if (tabsC==0&&!isNull) {
                if (isFunc) {
                    isFunc=0;
                    if (funcColon->type!=HalSMVariableType_HalSMNull) {
                        funcColon=&null;
                    }
                    PutDictElementToDict(hsmc,(((HalSMClass*)fc->value)->funcs),DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->name),func));
                    func=&null;

                    if (isFuncColon) {
                        isFuncColon=0;
                    }
                }
                isClass=0;
                isFunc=0;
                func=&null;
                cls=&null;
            } else if (tabsC==1&&!isNull) {
                if (isFunc) {
                    isFunc=0;
                    if (funcColon->type!=HalSMVariableType_HalSMNull) {
                        funcColon=&null;
                    }
                    PutDictElementToDict(hsmc,(((HalSMClass*)fc->value)->funcs),DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->name),func));
                    func=&null;

                    if (isFuncColon) {
                        isFuncColon=0;
                    }
                }

                if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("def "))) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H(":"));
                    spliti=HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1);
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,spliti,U8H(":"));
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,U8H("def "));
                    spliti=HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size);
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,spliti,U8H("def "));
                    nameFunc=(HalSMArray*)HalSMCompiler_getNameFunction(hsmc,joinSpliti)->value;
                    f=HalSMLocalFunction_init(hsmc,*(HalStringFormatChar**)nameFunc->arr[0]->value,*(HalStringFormatChar**)nameFunc->arr[1]->value,module);
                    func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                    PutDictElementToDict(hsmc,((HalSMClass*)cls->value)->funcs,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,f->name),func));
                    isFunc=1;
                } else if (isSetVar->type!=HalSMVariableType_HalSMNull) {
                    resRunFunc=(HalSMFunctionArray*)isSetVar->value;
                    unsigned char isGet=0;
                    HalSMVariable* isGetV=HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)resRunFunc->args->arr[0]->value,&isGet,1,((HalSMClass*)cls->value)->vars);
                    if (isGet) {
                        HalSMVariable* ret=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)resRunFunc->args->arr[1]->value,((HalSMClass*)cls->value)->vars)->arr[0];
                        isGetV->value=ret->value;
                        isGetV->type=ret->type;
                    }
                }
            } else if (isFunc) {
                if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("return "))) {
                    spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H("return "));
                    joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),U8H("return "));
                    HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMReturn_init(hsmc->memorymanagmentlibrary,joinSpliti,((HalSMLocalFunction*)func->value)->id),HalSMVariableType_HalSMReturn);
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                        HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                        if (arrGF->size==0) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                        }
                    } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                        HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                        HalSMVariable* funcElemVar;
                        for (unsigned long long int i=0;i<tabsC-2;i++) {
                            funcElemVar=funcElem->arr[funcElem->size-1];
                            if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                            } else {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                            }
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                    }
                } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("goto "))) {
                    joinSpliti=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H("goto "))->arr[1]->value);
                    HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMGOTO_init(hsmc->memorymanagmentlibrary,joinSpliti),HalSMVariableType_HalSMGOTO);
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                        HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                        if (arrGF->size==0) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                        }
                    } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                        HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                        HalSMVariable* funcElemVar;
                        for (unsigned long long int i=0;i<tabsC-1;i++) {
                            funcElemVar=funcElem->arr[funcElem->size-1];
                            if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                            } else {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                            }
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                    }
                } else if (isSetVar->type!=HalSMVariableType_HalSMNull) {
                    resRunFunc=(HalSMFunctionArray*)isSetVar->value;
                    HalSMVariable* valueVar=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hnull,HalSMVariableType_HalSMNull);
                    HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetVar_init(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)resRunFunc->args->arr[0]->value,*(HalStringFormatChar**)resRunFunc->args->arr[1]->value,valueVar),HalSMVariableType_HalSMSetVar);
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                        if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,resRunFunc->args->arr[0])!=-1) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){resRunFunc->args->arr[0],resRunFunc->args->arr[2]},2),HalSMVariableType_HalSMArray));
                        } else {
                            HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                            if (arrGF->size==0) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                            }
                        }
                    } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                        HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                        HalSMVariable* funcElemVar;
                        for (unsigned long long int i=0;i<tabsC-2;i++) {
                            funcElemVar=funcElem->arr[funcElem->size-1];
                            if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                            } else {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                            }
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                    }
                } else if (HSFCharCompare(tabsS[string_len(tabsS)-1],ASCIIH(':'))) {
                    long long int index=StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("));
                    if (index>0) {
                        HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,index));
                        HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                        if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                            HalSMFunctionC* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                            HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-2)));
                            
                            f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                            if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                                if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC))!=-1) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC),HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                                } else {
                                    err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                    if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                    groupFunctions=&null;
                                    HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,arrayGroupFunc->size-1);
                                }
                            } else if (DictElementIndexByKey(hsmc,hsmc->groupFunctions,obuff)!=-1) {
                                if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                    HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC);
                                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                    if (arrGF->size==0) {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                                    } else {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                                    }
                                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                    HalSMVariable* funcElemVar;
                                    for (unsigned long long int i=0;i<tabsC-2;i++) {
                                        funcElemVar=funcElem->arr[funcElem->size-1];
                                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                        } else {
                                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                        }
                                    }
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                }
                                groupFunctions=DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value;
                                groupFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC),HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray);
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray));
                                HalSMArray_add(hsmc->memorymanagmentlibrary,arrayGroupFunc,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){groupFunctions,groupFunc,HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsC)},3),HalSMVariableType_HalSMArray));
                            } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-2;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                }
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            }
                            isFuncColon=1;
                        } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                            HalSMLocalFunction* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                            HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-2)));

                            f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                            isFuncColon=1;
                            if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-2;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                            } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                                }
                            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));}
                            isFuncColon=1;
                        }
                    } else {
                        HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,string_len(tabsS)-1));
                        HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                        if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                            HalSMVariable* fuvar=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value;
                            HalSMFunctionC* fu=fuvar->value;
                            HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("")));
                            
                            f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                            if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                                if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,fuvar)!=-1) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){fuvar,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                                } else {
                                    //HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                    groupFunctions=&null;
                                    HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,0);
                                }
                            } else if (DictElementIndexByKey(hsmc,hsmc->groupFunctions,obuff)!=-1) {
                                if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                    HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC);
                                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                    if (arrGF->size==0) {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                                    } else {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                                    }
                                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                    HalSMVariable* funcElemVar;
                                    for (unsigned long long int i=0;i<tabsC-1;i++) {
                                        funcElemVar=funcElem->arr[funcElem->size-1];
                                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                        } else {
                                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                        }
                                    }
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                }
                                groupFunctions=DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value;
                                groupFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC),HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray);
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray));
                                HalSMArray_add(hsmc->memorymanagmentlibrary,arrayGroupFunc,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){groupFunctions,groupFunc,HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsC)},3),HalSMVariableType_HalSMArray));
                            } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-1;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                }
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            }
                            isFuncColon=1;
                        } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                            HalSMLocalFunction* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                            HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("")));

                            f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                            isFuncColon=1;
                            if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-1;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                            } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                                }
                            } else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));}
                            isFuncColon=1;
                        } else {
                            HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMGOTOSector_init(hsmc->memorymanagmentlibrary,nameFunction,0,NOTHALSMNULLPOINTER),HalSMVariableType_HalSMGOTOSector);
                            if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                                if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,resRunFunc->args->arr[0])!=-1) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){resRunFunc->args->arr[0],resRunFunc->args->arr[2]},2),HalSMVariableType_HalSMArray));
                                } else {
                                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                    if (arrGF->size==0) {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                                    } else {
                                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                                    }
                                }
                            } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-1;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                            }
                        }
                    }
                } else if (HSFCompare(tabsS,U8H("break"))) {
                    HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hbreak,HalSMVariableType_HalSMBreak);
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                        HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                        if (arrGF->size==0) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                        }
                    } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                        HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                        HalSMVariable* funcElemVar;
                        for (unsigned long long int i=0;i<tabsC-1;i++) {
                            funcElemVar=funcElem->arr[funcElem->size-1];
                            if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                            } else {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                            }
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                    }
                } else {
                    if (groupFunc->type!=HalSMVariableType_HalSMNull) {
                        if (HSFCharCompare(tabsS[string_len(tabsS)-1],ASCIIH(')'))&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("))!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("["))==-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("]"))==-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("."))==-1) {
                            long long int index=StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("));
                            HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,index));
                            HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                            if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1&&groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                                HalSMVariable* fuvar=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value;
                                HalSMFunctionC* fu=fuvar->value;
                                HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-1)));
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module),HalSMVariableType_HalSMLocalFunction));
                                if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,fuvar)!=-1) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){fuvar,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                                    groupFunc=&null; 
                                    goto NEXT_HALSM_COMPILE_LOOP;
                                }
                            }
                        }
                        if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                            groupFunc=&null;
                        }
                    }
                    HalSMVariable* ret;
                    HalSMArray* array=HalSMArray_init(hsmc->memorymanagmentlibrary);
                    HalSMFunctionArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
                    out->type=HalSMFunctionArrayType_function;
                    HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,l),HalSMVariableType_HalSMVar));
                    out->args=array;
                    ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                        HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                        if (arrGF->size==0) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,ret);
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                        }
                    } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                        HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                        HalSMVariable* funcElemVar;
                        for (unsigned long long int i=0;i<tabsC-2;i++) {
                            funcElemVar=funcElem->arr[funcElem->size-1];
                            if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                            } else {
                                funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                            }
                        }
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                    }
                }
            }
        }
        
        if (isFunc&&!isClass) {
            if (tabsC==0&&!isNull) {
                isFunc=0;
                if (funcColon->type!=HalSMVariableType_HalSMNull) {
                    if (funcColon->type==HalSMVariableType_HalSMFunctionC) {
                        if (groupFunctions->type==HalSMVariableType_HalSMNull) {
                            err=HalSMFunctionC_run((HalSMFunctionC*)funcColon->value,funcColonArgs,&NULLDict);
                            if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                            hsmc->memorymanagmentlibrary->free(funcColon);
                        }
                    } else if (funcColon->type==HalSMVariableType_HalSMLocalFunction) {
                        err=HalSMLocalFunction_run((HalSMLocalFunction*)funcColon->value,funcColonArgs,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                        hsmc->memorymanagmentlibrary->free(funcColon);
                    }
                    funcColon=&null;
                }
                func=&null;

                if (isFuncColon) {
                    isFuncColon=0;
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("return "))) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H("return "));
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),U8H("return "));
                HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMReturn_init(hsmc->memorymanagmentlibrary,joinSpliti,((HalSMLocalFunction*)func->value)->id),HalSMVariableType_HalSMReturn);
                if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                    if (arrGF->size==0) {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                    }
                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                    HalSMVariable* funcElemVar;
                    for (unsigned long long int i=0;i<tabsC-1;i++) {
                        funcElemVar=funcElem->arr[funcElem->size-1];
                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                        } else {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                        }
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("goto "))) {
                joinSpliti=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H("goto "))->arr[1]->value);
                HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMGOTO_init(hsmc->memorymanagmentlibrary,joinSpliti),HalSMVariableType_HalSMGOTO);
                if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                    if (arrGF->size==0) {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                    }
                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                    HalSMVariable* funcElemVar;
                    for (unsigned long long int i=0;i<tabsC-1;i++) {
                        funcElemVar=funcElem->arr[funcElem->size-1];
                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                        } else {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                        }
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                }
            } else if (isSetVar->type!=HalSMVariableType_HalSMNull) {
                resRunFunc=(HalSMFunctionArray*)isSetVar->value;
                HalSMVariable* valueVar=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hnull,HalSMVariableType_HalSMNull);
                HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetVar_init(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)resRunFunc->args->arr[0]->value,*(HalStringFormatChar**)resRunFunc->args->arr[1]->value,valueVar),HalSMVariableType_HalSMSetVar);
                if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                    if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,resRunFunc->args->arr[0])!=-1) {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){resRunFunc->args->arr[0],resRunFunc->args->arr[2]},2),HalSMVariableType_HalSMArray));
                    } else {
                        HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                        if (arrGF->size==0) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                        }
                    }
                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                    HalSMVariable* funcElemVar;
                    for (unsigned long long int i=0;i<tabsC-1;i++) {
                        funcElemVar=funcElem->arr[funcElem->size-1];
                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                        } else {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                        }
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                }
            } else if (HSFCharCompare(tabsS[string_len(tabsS)-1],ASCIIH(':'))) {
                long long int index=StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("));
                if (index>0) {
                    HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,index));
                    HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                    if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                        HalSMVariable* fuvar=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value;
                        HalSMFunctionC* fu=fuvar->value;
                        HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-2)));
                        
                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                        if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,fuvar)!=-1) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){fuvar,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                            } else {
                                //HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                groupFunctions=&null;
                                HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,0);
                            }
                        } else if (DictElementIndexByKey(hsmc,hsmc->groupFunctions,obuff)!=-1) {
                            if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC);
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                                }
                            } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-1;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            }
                            groupFunctions=DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value;
                            groupFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC),HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,arrayGroupFunc,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){groupFunctions,groupFunc,HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsC)},3),HalSMVariableType_HalSMArray));
                        } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                            HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                            HalSMVariable* funcElemVar;
                            for (unsigned long long int i=0;i<tabsC-1;i++) {
                                funcElemVar=funcElem->arr[funcElem->size-1];
                                if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                } else {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                }
                            }
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                        } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                            if (arrGF->size==0) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            }
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                        }
                        isFuncColon=1;
                    } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                        HalSMLocalFunction* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                        HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-2)));

                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                        isFuncColon=1;
                        if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                            HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                            HalSMVariable* funcElemVar;
                            for (unsigned long long int i=0;i<tabsC-1;i++) {
                                funcElemVar=funcElem->arr[funcElem->size-1];
                                if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                } else {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                }
                            }
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                        } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                            if (arrGF->size==0) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                            }
                        } else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));}
                        isFuncColon=1;
                    }
                } else {
                    HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,string_len(tabsS)-1));
                    HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                    if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                        HalSMVariable* fuvar=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value;
                        HalSMFunctionC* fu=fuvar->value;
                        HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("")));
                        
                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                        if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,fuvar)!=-1) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){fuvar,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                            } else {
                                //HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                                groupFunctions=&null;
                                HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,0);
                            }
                        } else if (DictElementIndexByKey(hsmc,hsmc->groupFunctions,obuff)!=-1) {
                            if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                                HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC);
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                                }
                            } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                                HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                                HalSMVariable* funcElemVar;
                                for (unsigned long long int i=0;i<tabsC-1;i++) {
                                    funcElemVar=funcElem->arr[funcElem->size-1];
                                    if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                    } else {
                                        funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                    }
                                }
                                HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            }
                            groupFunctions=DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value;
                            groupFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC),HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,arrayGroupFunc,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){groupFunctions,groupFunc,HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsC)},3),HalSMVariableType_HalSMArray));
                        } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                            HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                            HalSMVariable* funcElemVar;
                            for (unsigned long long int i=0;i<tabsC-1;i++) {
                                funcElemVar=funcElem->arr[funcElem->size-1];
                                if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                } else {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                }
                            }
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                        } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                            if (arrGF->size==0) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                            }
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFuncC));
                        }
                        isFuncColon=1;
                    } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                        HalSMLocalFunction* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                        HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("")));

                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction));
                        isFuncColon=1;
                        if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                            HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                            HalSMVariable* funcElemVar;
                            for (unsigned long long int i=0;i<tabsC-1;i++) {
                                funcElemVar=funcElem->arr[funcElem->size-1];
                                if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                } else {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                }
                            }
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                        } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                            if (arrGF->size==0) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                            } else {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));
                            }
                        } else {HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFunc_init(hsmc->memorymanagmentlibrary,fu,funcArgs),HalSMVariableType_HalSMRunFunc));}
                        isFuncColon=1;
                    } else {
                        HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMGOTOSector_init(hsmc->memorymanagmentlibrary,nameFunction,0,NOTHALSMNULLPOINTER),HalSMVariableType_HalSMGOTOSector);
                        if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,resRunFunc->args->arr[0])!=-1) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){resRunFunc->args->arr[0],resRunFunc->args->arr[2]},2),HalSMVariableType_HalSMArray));
                            } else {
                                HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                                if (arrGF->size==0) {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                                } else {
                                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                                }
                            }
                        } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                            HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                            HalSMVariable* funcElemVar;
                            for (unsigned long long int i=0;i<tabsC-1;i++) {
                                funcElemVar=funcElem->arr[funcElem->size-1];
                                if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                                } else {
                                    funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                                }
                            }
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                        } else {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                        }
                    }
                }
            } else if (HSFCompare(tabsS,U8H("break"))) {
                HalSMVariable* ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hbreak,HalSMVariableType_HalSMBreak);
                if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                    if (arrGF->size==0) {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value))->func,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                    }
                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                    HalSMVariable* funcElemVar;
                    for (unsigned long long int i=0;i<tabsC-1;i++) {
                        funcElemVar=funcElem->arr[funcElem->size-1];
                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                        } else {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                        }
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                }
            } else {
                if (groupFunc->type!=HalSMVariableType_HalSMNull) {
                    if (HSFCharCompare(tabsS[string_len(tabsS)-1],ASCIIH(')'))&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("))!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("["))==-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("]"))==-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("."))==-1) {
                        long long int index=StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("));
                        HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,index));
                        HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                        if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1&&groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                            HalSMVariable* fuvar=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value;
                            HalSMFunctionC* fu=fuvar->value;
                            HalSMArray* funcArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-1)));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module),HalSMVariableType_HalSMLocalFunction));
                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,fuvar)!=-1) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){fuvar,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                                groupFunc=&null; 
                                goto NEXT_HALSM_COMPILE_LOOP;
                            }
                        }
                    }
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value==tabsC) {
                        groupFunc=&null;
                    }
                }
                HalSMVariable* ret;
                HalSMArray* array=HalSMArray_init(hsmc->memorymanagmentlibrary);
                HalSMFunctionArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionArray));
                out->type=HalSMFunctionArrayType_function;
                HalSMArray_add(hsmc->memorymanagmentlibrary,array,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,l),HalSMVariableType_HalSMVar));
                out->args=array;
                ret=HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMFunctionArray);
                if (groupFunctions->type!=HalSMVariableType_HalSMNull&&*(unsigned long long int*)((HalSMArray*)arrayGroupFunc->arr[arrayGroupFunc->size-1]->value)->arr[2]->value<tabsC) {
                    HalSMArray* arrGF=(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value;
                    if (arrGF->size==0) {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[1]->value)->func,ret);
                    } else {
                        HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)((HalSMArray*)((HalSMArray*)arrGF->arr[arrGF->size-1]->value)->arr[1]->value)->arr[1]->value)->func,ret);
                    }
                } else if (funcColon->type!=HalSMVariableType_HalSMNull||isFuncColon) {
                    HalSMArray* funcElem=((HalSMLocalFunction*)func->value)->func;
                    HalSMVariable* funcElemVar;
                    for (unsigned long long int i=0;i<tabsC-1;i++) {
                        funcElemVar=funcElem->arr[funcElem->size-1];
                        if (funcElemVar->type==HalSMVariableType_HalSMRunFuncC) {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFuncC*)funcElemVar->value)->args->arr[1]->value)->func;
                        } else {
                            funcElem=((HalSMLocalFunction*)((HalSMRunFunc*)funcElemVar->value)->args->arr[1]->value)->func;
                        }
                    }
                    HalSMArray_add(hsmc->memorymanagmentlibrary,funcElem,ret);
                } else {
                    HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,ret);
                }
            }
        }

        if (!isFunc&&!isClass) {
            if (isGOTO->type==HalSMVariableType_HalSMNull&&StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("import "))) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,l,U8H("import "));
                HalStringFormatChar* pathModule=StringReplace(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),U8H("import ")),U8H("\n"),U8H("")),U8H("\r"),U8H(""));
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,pathModule,U8H("/"));
                HalSMVariable* nameModule=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)spliti->arr[spliti->size-1]->value);
                HalSMVariable* module=hsmc->HalSMLoadModule(hsmc,*(HalStringFormatChar**)nameModule->value);
                if (module->type!=HalSMVariableType_HalSMCModule&&module->type!=HalSMVariableType_HalSMModule) {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("Module with name "),ConcatenateStrings(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)nameModule->value,U8H(" Not Found")))));
                    return &hvoid;
                } else {
                    PutDictElementToDict(hsmc,hsmc->modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)nameModule->value),module));
                }
            } else if (isGOTO->type==HalSMVariableType_HalSMNull&&StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("from "))&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H(" import "))!=-1) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,l,U8H("from "));
                joinSpliti=StringReplace(hsmc->memorymanagmentlibrary,StringReplace(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)spliti->arr[1]->value,U8H("\n"),U8H("")),U8H("\r"),U8H(""));
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,U8H(" import "));
                HalStringFormatChar* pathModule=*(HalStringFormatChar**)spliti->arr[0]->value;
                HalSMVariable* nameImport=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)spliti->arr[1]->value);
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,pathModule,U8H("/"));
                HalSMVariable* nameModule=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)spliti->arr[spliti->size-1]->value);
                if (DictElementIndexByKey(hsmc,hsmc->modules,nameModule)==-1) {
                    HalSMVariable* module=hsmc->HalSMLoadModule(hsmc,*(HalStringFormatChar**)nameModule->value);
                    if (module->type!=HalSMVariableType_HalSMCModule&&module->type!=HalSMVariableType_HalSMModule) {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("Module with name "),ConcatenateStrings(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)nameModule->value,U8H(" Not Found")))));
                        return &hvoid;
                    } else {
                        PutDictElementToDict(hsmc,hsmc->modules,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)nameModule->value),module));
                    }
                }

                if (DictElementIndexByKey(hsmc,hsmc->modules,nameModule)!=-1) {
                    HalSMVariable* module=DictElementFindByKey(hsmc,hsmc->modules,nameModule)->value;
                    if (module->type==HalSMVariableType_HalSMCModule) {
                        HalSMCModule* cmodule=(HalSMCModule*)module->value;
                        if (HSFCompare(*(HalStringFormatChar**)nameImport->value,U8H("*"))) {
                            unsigned long long int a;
                            for (a=0;a<cmodule->classes->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->classes,cmodule->classes->elements[a]);
                            }
                            for (a=0;a<cmodule->lfuncs->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->functions,cmodule->lfuncs->elements[a]);
                            }
                            for (a=0;a<cmodule->vrs->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->variables,cmodule->vrs->elements[a]);
                            }
                        } else if (DictElementIndexByKey(hsmc,cmodule->classes,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->classes,DictElementFindByKey(hsmc,cmodule->classes,nameImport));
                        } else if (DictElementIndexByKey(hsmc,cmodule->lfuncs,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->functions,DictElementFindByKey(hsmc,cmodule->lfuncs,nameImport));
                        } else if (DictElementIndexByKey(hsmc,cmodule->vrs,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->variables,DictElementFindByKey(hsmc,cmodule->vrs,nameImport));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("Not found element "),ConcatenateStrings(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)nameImport->value,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H(" from CModule "),cmodule->name)))));
                        }
                    } else if (module->type==HalSMVariableType_HalSMModule) {
                        HalSMModule* mmodule=(HalSMModule*)module->value;
                        if (HSFCompare(*(HalStringFormatChar**)nameImport->value,U8H("*"))) {
                            unsigned long long int a;
                            for (a=0;a<mmodule->hsmc->classes->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->classes,mmodule->hsmc->classes->elements[a]);
                            }
                            for (a=0;a<mmodule->hsmc->localFunctions->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->localFunctions,mmodule->hsmc->localFunctions->elements[a]);
                            }
                            for (a=0;a<mmodule->hsmc->variables->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->variables,mmodule->hsmc->variables->elements[a]);
                            }
                            for (a=0;a<mmodule->hsmc->functions->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->functions,mmodule->hsmc->functions->elements[a]);
                            }
                            for (a=0;a<mmodule->hsmc->modules->size;a++) {
                                PutDictElementToDict(hsmc,hsmc->modules,mmodule->hsmc->modules->elements[a]);
                            }
                        } else if (DictElementIndexByKey(hsmc,mmodule->hsmc->classes,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->classes,DictElementFindByKey(hsmc,mmodule->hsmc->classes,nameImport));
                        } else if (DictElementIndexByKey(hsmc,mmodule->hsmc->localFunctions,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->localFunctions,DictElementFindByKey(hsmc,mmodule->hsmc->localFunctions,nameImport));
                        } else if (DictElementIndexByKey(hsmc,mmodule->hsmc->variables,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->variables,DictElementFindByKey(hsmc,mmodule->hsmc->variables,nameImport));
                        } else if (DictElementIndexByKey(hsmc,mmodule->hsmc->functions,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->functions,DictElementFindByKey(hsmc,mmodule->hsmc->functions,nameImport));
                        } else if (DictElementIndexByKey(hsmc,mmodule->hsmc->modules,nameImport)!=-1) {
                            PutDictElementToDict(hsmc,hsmc->modules,DictElementFindByKey(hsmc,mmodule->hsmc->modules,nameImport));
                        } else {
                            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("Not found element "),ConcatenateStrings(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)nameImport->value,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H(" from Module "),mmodule->name)))));
                        }
                    }
                }
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("def "))) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H(":"));
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),U8H(":"));
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,U8H("def "));
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),U8H("def "));
                nameFunc=(HalSMArray*)HalSMCompiler_getNameFunction(hsmc,joinSpliti)->value;
                f=HalSMLocalFunction_init(hsmc,*(HalStringFormatChar**)nameFunc->arr[0]->value,*(HalStringFormatChar**)nameFunc->arr[1]->value,module);
                func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                PutDictElementToDict(hsmc,hsmc->localFunctions,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,f->name),func));
                isFunc=1;
            } else if (StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("class "))) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H(":"));
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,0,spliti->size-1),U8H(":"));
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,joinSpliti,U8H("class "));
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),U8H("class "));
                fc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMClass_init(hsmc->memorymanagmentlibrary,joinSpliti,hsmc->variables),HalSMVariableType_HalSMClass);
                PutDictElementToDict(hsmc,hsmc->classes,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,((HalSMClass*)fc->value)->name),fc));
                cls=fc;
                isClass=1;
            } else if (isGOTO->type==HalSMVariableType_HalSMNull&&StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("goto "))) {
                joinSpliti=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H("goto "))->arr[1]->value);
                err=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,joinSpliti);
                if (DictElementIndexByKey(hsmc,hsmc->GOTOSectors,err)!=-1) {
                    //GOTO Function
                    hsmc->indexl=((HalSMGOTOSector*)DictElementFindByKey(hsmc,hsmc->GOTOSectors,err)->value->value)->index;
                } else {
                    isGOTO=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMGOTO_init(hsmc->memorymanagmentlibrary,joinSpliti),HalSMVariableType_HalSMGOTO);
                }
            } else if (isGOTO->type==HalSMVariableType_HalSMNull&&StringStartsWith(hsmc->memorymanagmentlibrary,tabsS,U8H("return "))) {
                spliti=HalSMArray_split_str(hsmc->memorymanagmentlibrary,tabsS,U8H("return "));
                joinSpliti=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMArray_slice(hsmc->memorymanagmentlibrary,spliti,1,spliti->size),U8H("return "));
                HalSMArray* outarr=HalSMCompiler_getArgs(hsmc,joinSpliti,&NULLDict);
                if (outarr->size==0) {
                    HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Return not wrong value")));
                    return &hvoid;
                } else {
                    return outarr->arr[0];
                }
            } else if (isGOTO->type==HalSMVariableType_HalSMNull&&isSetVar->type!=HalSMVariableType_HalSMNull) {
                resRunFunc=(HalSMFunctionArray*)isSetVar->value;
                unsigned char isGet=0;
                HalSMVariable* isGetV=HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)resRunFunc->args->arr[0]->value,&isGet,1,&NULLDict);
                if (isGet) {
                    HalSMVariable* ret=HalSMCompiler_getArgs(hsmc,*(HalStringFormatChar**)resRunFunc->args->arr[1]->value,&NULLDict)->arr[0];
                    isGetV->value=ret->value;
                    isGetV->type=ret->type;
                }
            } else if (HSFCharCompare(tabsS[string_len(tabsS)-1],ASCIIH(':'))) {
                long long int index=StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("));
                if (index>-1) {
                    HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,index));
                    HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                    if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                        HalSMFunctionC* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                        funcColonArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-2)));
                        funcColon=HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC);
                        
                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,func);
                        isFunc=1;
                        if (DictElementIndexByKey(hsmc,hsmc->groupFunctions,obuff)!=-1) {
                            if (groupFunc->type!=HalSMVariableType_HalSMNull) {
                                err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                                groupFunc=&null;
                            }
                            
                            groupFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){funcColon,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray);
                            //groupFunctions=HalSMVariable_init(hsmc->memorymanagmentlibrary,DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value->value,HalSMVariableType_HalSMFunctionC);
                            //groupFunctions=HalSMVariable_init(hsmc->memorymanagmentlibrary,DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value->value,HalSMVariableType_HalSMArray);
                            groupFunctions=DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value;
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,arrayGroupFunc,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){groupFunctions,groupFunc,HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsC)},3),HalSMVariableType_HalSMArray));
                        } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,funcColon)!=-1) {
                                //Sdes' problema
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){funcColon,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                            } else {
                                err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,funcColon->value,funcColonArgs),HalSMVariableType_HalSMRunFuncC));
                                groupFunctions=&null;
                                HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,0);
                            }
                        }
                    } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                        HalSMLocalFunction* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                        funcColonArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-2)));
                        funcColon=HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMLocalFunction);

                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,func);
                        isFunc=1;
                    }
                } else {
                    HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,string_len(tabsS)-1));
                    HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                    if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1) {
                        HalSMFunctionC* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                        funcColonArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("")));
                        funcColon=HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC);
                        
                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,func);
                        isFunc=1;
                        if (DictElementIndexByKey(hsmc,hsmc->groupFunctions,obuff)!=-1) {
                            if (groupFunc->type!=HalSMVariableType_HalSMNull) {
                                err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                                groupFunc=&null;
                            }
                            
                            groupFunc=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){funcColon,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray);
                            //groupFunctions=HalSMVariable_init(hsmc->memorymanagmentlibrary,DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value->value,HalSMVariableType_HalSMFunctionC);
                            //groupFunctions=HalSMVariable_init(hsmc->memorymanagmentlibrary,DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value->value,HalSMVariableType_HalSMArray);
                            groupFunctions=DictElementFindByKey(hsmc,hsmc->groupFunctions,obuff)->value;
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init(hsmc->memorymanagmentlibrary),HalSMVariableType_HalSMArray));
                            HalSMArray_add(hsmc->memorymanagmentlibrary,arrayGroupFunc,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){groupFunctions,groupFunc,HalSMVariable_init_unsigned_int(hsmc->memorymanagmentlibrary,tabsC)},3),HalSMVariableType_HalSMArray));
                        } else if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,funcColon)!=-1) {
                                //Sdes' problema
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){funcColon,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                            } else {
                                err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                                HalSMArray_add(hsmc->memorymanagmentlibrary,((HalSMLocalFunction*)func->value)->func,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMRunFuncC_init(hsmc->memorymanagmentlibrary,funcColon->value,funcColonArgs),HalSMVariableType_HalSMRunFuncC));
                                groupFunctions=&null;
                                HalSMArray_remove(hsmc->memorymanagmentlibrary,arrayGroupFunc,0);
                            }
                        }
                    } else if (DictElementIndexByKey(hsmc,hsmc->localFunctions,obuff)!=-1) {
                        HalSMLocalFunction* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                        funcColonArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("")));
                        funcColon=HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMLocalFunction);

                        f=HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module);
                        func=HalSMVariable_init(hsmc->memorymanagmentlibrary,f,HalSMVariableType_HalSMLocalFunction);
                        HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,func);
                        isFunc=1;
                    } else {
                        if (isGOTO->type==HalSMVariableType_HalSMGOTO&&HSFCompare(nameFunction,((HalSMGOTO*)isGOTO->value)->sector)) {
                            HalSMVariable_free(hsmc->memorymanagmentlibrary,isGOTO);
                            isGOTO=&null;
                        }
                        PutDictElementToDict(hsmc,hsmc->GOTOSectors,DictElementInit(hsmc->memorymanagmentlibrary,obuff,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMGOTOSector_init(hsmc->memorymanagmentlibrary,nameFunction,hsmc->indexl,NOTHALSMNULLPOINTER),HalSMVariableType_HalSMGOTOSector)));
                    }
                }
            } else if (isGOTO->type==HalSMVariableType_HalSMNull) {
                if (groupFunc->type!=HalSMVariableType_HalSMNull) {
                    if (HSFCharCompare(tabsS[string_len(tabsS)-1],ASCIIH(')'))&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("))!=-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("["))==-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("]"))==-1&&StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("."))==-1) {
                        long long int index=StringIndexOf(hsmc->memorymanagmentlibrary,tabsS,U8H("("));
                        HalStringFormatChar* nameFunction=HalSMCompiler_removeSpaces(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,0,index));
                        HalSMVariable* obuff=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,nameFunction);
                        if (DictElementIndexByKey(hsmc,hsmc->functions,obuff)!=-1&&groupFunctions->type!=HalSMVariableType_HalSMNull) {
                            HalSMFunctionC* fu=DictElementFindByKey(hsmc,hsmc->functions,obuff)->value->value;
                            funcColonArgs=HalSMArray_init(hsmc->memorymanagmentlibrary);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,tabsS,index+1,string_len(tabsS)-1)));
                            funcColon=HalSMVariable_init(hsmc->memorymanagmentlibrary,fu,HalSMVariableType_HalSMFunctionC);
                            HalSMArray_add(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMLocalFunction_init(hsmc,U8H(""),U8H(""),module),HalSMVariableType_HalSMLocalFunction));

                            if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,funcColon)!=-1) {
                                HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){funcColon,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                                funcColon=&null;

                                err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                                if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                                groupFunc=&null;
                                groupFunctions=&null;
                                goto NEXT_HALSM_COMPILE_LOOP;
                            }

                            funcColon=&null;

                            err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                            if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                            groupFunc=&null;
                            groupFunctions=&null;
                        }

                        err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                        groupFunc=&null;
                    } else {
                        err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                        groupFunc=&null;
                    }
                }
                if (isConsole==1) {
                    HalSMArray* argumentsi=HalSMCompiler_getArgs(hsmc,tabsS,&NULLDict);
                    if (argumentsi->size==0) {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Unknown Line")));
                        return &hvoid;
                    } else if (argumentsi->arr[0]->type!=HalSMVariableType_void) {
                        hsmc->print(HalSMVariable_to_str(hsmc,argumentsi->arr[0],1));
                    }
                } else {
                    unsigned char isOut=0;
                    HalSMVariable* err=HalSMCompiler_isGetU(hsmc,l,&isOut,0,&NULLDict);
                    if (err->type==HalSMVariableType_HalSMError) {
                        HalSMCompiler_ThrowError(hsmc,(HalSMError*)err->value);
                        return &hvoid;
                    } else if (isOut==0) {
                        HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Unknown Line")));
                        return &hvoid;
                    }
                }
            }
        }

        NEXT_HALSM_COMPILE_LOOP:
        hsmc->line++;
        hsmc->ind->line=hsmc->line;
        if (hsmc->indexl==lastIndex) {
            if (isFunc&&tabsC>0) {
                isFunc=0;
                if (funcColon->type!=HalSMVariableType_HalSMNull) {
                    err=&null;
                    if (funcColon->type==HalSMVariableType_HalSMFunctionC) {
                        err=HalSMFunctionC_run((HalSMFunctionC*)funcColon->value,funcColonArgs,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                        hsmc->memorymanagmentlibrary->free(funcColon);
                    } else if (funcColon->type==HalSMVariableType_HalSMLocalFunction) {
                        err=HalSMLocalFunction_run((HalSMLocalFunction*)funcColon->value,funcColonArgs,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                        hsmc->memorymanagmentlibrary->free(funcColon);
                    }
                    if(err->type==HalSMVariableType_HalSMGOTOSector) {
                        tabsC=0;
                        prevTabsC=0;
                        hsmc->indexl=((HalSMGOTOSector*)err->value)->index;
                        HalSMArray_clear(hsmc->memorymanagmentlibrary,arrayGroupFunc);
                        HalSMArray_clear(hsmc->memorymanagmentlibrary,funcColonArgs);
                        groupFunc=&null;
                        groupFunctions=&null;
                    }
                    funcColon=&null;
                }
                func=&null;

                if (isFuncColon) {
                    isFuncColon=0;
                }
            } else if (funcColon->type!=HalSMVariableType_HalSMNull) {
                if (funcColon->type==HalSMVariableType_HalSMFunctionC) {
                    if (groupFunctions->type!=HalSMVariableType_HalSMNull) {
                        if (HalSMArray_find(hsmc,(HalSMArray*)groupFunctions->value,funcColon)!=-1) {
                            HalSMArray_add(hsmc->memorymanagmentlibrary,(HalSMArray*)((HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value)->arr[2]->value,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){funcColon,HalSMVariable_init(hsmc->memorymanagmentlibrary,funcColonArgs,HalSMVariableType_HalSMArray)},2),HalSMVariableType_HalSMArray));
                        }
                        err=HalSMFunctionC_run((HalSMFunctionC*)((HalSMArray*)groupFunc->value)->arr[0]->value,(HalSMArray*)((HalSMArray*)groupFunc->value)->arr[1]->value,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                    } else {
                        err=HalSMFunctionC_run((HalSMFunctionC*)funcColon->value,funcColonArgs,&NULLDict);
                        if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                    }
                } else if (funcColon->type==HalSMVariableType_HalSMLocalFunction) {
                    err=HalSMLocalFunction_run((HalSMLocalFunction*)funcColon->value,funcColonArgs,&NULLDict);
                    if (err->type==HalSMVariableType_HalSMError){return &hvoid;}
                }
                hsmc->memorymanagmentlibrary->free(funcColon);
                funcColon=&null;
            }
        }
        if (!isNull) {
            prevTabsC=tabsC;
        }
    }
    return &hvoid;
}

HalSMVariable* HalSMCompiler_compile(HalSMCompiler* hsmc,HalStringFormatChar* text,unsigned char isConsole,HalSMVariable* module)
{
    hsmc->line=1;
    HalSMArray* allLines=HalSMCompiler_getLines(hsmc,text);
    hsmc->indexl=0;
    hsmc->countLocalFunctions=0;
    hsmc->isRun=1;
    HalSMVariable* out=HalSMCompiler_compileList(hsmc,isConsole,module,allLines);
    hsmc->isRun=0;
    return out;
}

HalSMDoubleGet* HalSMDoubleGet_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* st)
{
    HalSMDoubleGet* hfg=hsmmml->malloc(sizeof(HalSMDoubleGet));
    hfg->st=st;
    return hfg;
}

HalSMVar* HalSMVar_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name)
{
    HalSMVar* out=hsmmml->malloc(sizeof(HalSMVar));
    out->name=HalSMVariable_init_str(hsmmml,name);
    return out;
}

HalSMSetArg* HalSMSetArg_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* name)
{
    HalSMSetArg* out=hsmmml->malloc(sizeof(HalSMSetArg));
    out->name=name;
    out->value=HalSMVariable_init(hsmmml,&null,HalSMVariableType_HalSMNull);
    return out;
}

HalSMSetArg* HalSMSetArg_initWithValue(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* name,HalSMVariable* value)
{
    HalSMSetArg* out=hsmmml->malloc(sizeof(HalSMSetArg));
    out->name=name;
    out->value=value;
    return out;
}

HalSMReturn* HalSMReturn_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* val,unsigned long long idLocalFunction)
{
    HalSMReturn* out=hsmmml->malloc(sizeof(HalSMReturn));
    out->value=val;
    out->idLocalFunction=idLocalFunction;
    return out;
}


//DICT


Dict* DictInit(HalSMMemoryManagmentLibrary* hsmmml)
{
    Dict* dict=hsmmml->malloc(sizeof(Dict));
    dict->size=0;
    dict->elements=hsmmml->malloc(0);
    return dict;
}

Dict* DictInitWithElements(HalSMMemoryManagmentLibrary* hsmmml,DictElement* elements[],unsigned long long int size)
{
    Dict* dict=hsmmml->malloc(sizeof(Dict));
    dict->size=size;
    dict->elements=hsmmml->malloc(sizeof(DictElement*)*size);
    for (unsigned long long int i=0;i<size;i++) {
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

DictElement* DictElementFindByIndex(Dict* dict,unsigned long long int index)
{
    DictElement* temp;
    if (index<dict->size)temp=dict->elements[index];
    return temp;
}

DictElement* DictElementFindByKey(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* key)
{
    DictElement* empty;
    if (dict->size==0){return empty;}
    for (unsigned long long int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(hsmc,dict->elements[i]->key,key)) return dict->elements[i];
    }
    return empty;
}

DictElement* DictElementFindByValue(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* value)
{
    DictElement* empty;
    if (dict->size==0) {return empty;}
    for (unsigned long long int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(hsmc,dict->elements[i]->value,value)) return dict->elements[i];
    }
    return empty;
}

DictElement* DictElementCopy(HalSMMemoryManagmentLibrary* hsmmml,DictElement* elem)
{
    return DictElementInit(hsmmml,HalSMVariable_init(hsmmml,elem->key->value,elem->key->type),HalSMVariable_init(hsmmml,elem->value->value,elem->value->type));
}

void PutDictElementToDict(HalSMCompiler* hsmc,Dict *dict,DictElement* elem)
{
    long long int index=DictElementIndexByKey(hsmc,dict,elem->key);
    if (index==-1) {
        dict->elements=hsmc->memorymanagmentlibrary->realloc(dict->elements,(dict->size+1)*sizeof(DictElement*));
        dict->elements[dict->size]=elem;
        dict->size=dict->size+1;
    } else {
        dict->elements[index]->value->value=elem->value->value;
        dict->elements[index]->value->type=elem->value->type;
        //hsmmml->free(elem);
    }
}

long long int DictElementIndexByKey(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* key)
{
    if (dict->size==0)return -1;
    DictElement** p=&dict->elements[0];
    for (unsigned long long int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(hsmc,(*p++)->key,key)) {return i;}
    }
    return -1;
}

long long int DictElementIndexByValue(HalSMCompiler* hsmc,Dict* dict,HalSMVariable* value)
{
    if (dict->size==0)return -1;
    DictElement** p=&dict->elements[0];
    for (unsigned long long int i=0;i<dict->size;i++)
    {
        if (HalSMVariable_Compare(hsmc,(*p++)->value,value)) {return i;}
    }
    return -1;
}

Dict* DictCopy(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict)
{
    return DictInitWithElements(hsmmml,dict->elements,dict->size);
}

unsigned char DictCompare(HalSMCompiler* hsmc,Dict* a,Dict* b)
{
    if (a->size!=b->size){return 0;}
    for (unsigned long long int i=0;i<a->size;i++) {
        if (HalSMVariable_Compare(hsmc,a->elements[i]->key,b->elements[i]->key)==0||HalSMVariable_Compare(hsmc,a->elements[i]->value,b->elements[i]->value)==0){return 0;}
    }
    return 1;
}

void DictFree(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict)
{
    for (unsigned long long int i=0;i<dict->size;i++)
        hsmmml->free(dict->elements[i]);
    hsmmml->free(dict->elements);
    hsmmml->free(dict);
}

void DictClear(HalSMMemoryManagmentLibrary* hsmmml,Dict* dict)
{
    for (unsigned long long int i=0;i<dict->size;i++)
        hsmmml->free(dict->elements[i]);
    dict->elements=hsmmml->realloc(dict->elements,0);
    dict->size=0;
}

HalStringFormatChar* Dict_to_print(HalSMCompiler* hsmc,Dict* harr)
{
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    if (harr->size>0) {
        DictElement** p=&harr->elements[0];
        HalStringFormatChar* keystr;
        HalStringFormatChar* valuestr;
        for (unsigned long long int i=0;i<harr->size;i++,p++) {
            keystr=HalSMVariable_to_str(hsmc,(*p)->key,1);
            valuestr=HalSMVariable_to_str(hsmc,(*p)->value,1);
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,ConcatenateStrings(hsmc->memorymanagmentlibrary,keystr,ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H(":"),valuestr))));
        }
    }
    HalStringFormatChar* joinout=HalSMArray_join_str(hsmc->memorymanagmentlibrary,out,U8H(", "));
    unsigned long long int l=string_len(joinout);
    HalStringFormatChar* outs=hsmc->memorymanagmentlibrary->realloc(joinout,(l+3)*sizeof(HalStringFormatChar));
    HalStringFormatChar* po=&outs[l];
    HalStringFormatChar* pj=&outs[l-1];
    for (unsigned long long int i=0;i<l;i++,po--,pj--){*po=*pj;}
    outs[0]=(HalStringFormatChar){0,97,0};
    outs[l+1]=(HalStringFormatChar){0,99,0};
    outs[l+2]=(HalStringFormatChar){0,0,0};
    return outs;
}

void DictAppendDict(HalSMMemoryManagmentLibrary* hsmmml,Dict* a,Dict* b)
{
    a->elements=hsmmml->realloc(a->elements,(a->size+b->size)*sizeof(DictElement*));
    DictElement** p=&a->elements[a->size];
    DictElement** pn=&b->elements[0];
    for (unsigned long long int i=0;i<b->size;i++){*p++=*pn++;}
    a->size=a->size+b->size;
}

void DictRemove(HalSMMemoryManagmentLibrary* hsmmml,Dict* a,unsigned long long int index)
{
    DictElement** arr=hsmmml->malloc((a->size-1)*sizeof(DictElement*));   
    DictElement** ap=arr;
    DictElement** ae=a->elements;
    for (unsigned long long int i=0;i<a->size;i++) {
        if(i!=index){*ap++=*ae++;}
    }
    a->size=a->size-1;
    hsmmml->free(a->elements);
    a->elements=arr;
}


//HalSMVariable


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
    if(type==HalSMVariableType_int){(*(HalSMInteger*)var)=*(HalSMInteger*)value;}
    else if(type==HalSMVariableType_char){(*(HalStringFormatChar*)var)=(*(HalStringFormatChar*)value);}
    else if(type==HalSMVariableType_double){(*(HalSMDouble*)var)=(*(HalSMDouble*)value);}
    else if(type==HalSMVariableType_void){(*(HalSMVoid*)var)=*(HalSMVoid*)value;}
    else if(type==HalSMVariableType_HalSMArray){*((HalSMArray*)var)=*((HalSMArray*)value);}
    else if(type==HalSMVariableType_str){(*(HalStringFormatChar**)var)=(*(HalStringFormatChar**)value);}
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
    else if(type==HalSMVariableType_HalSMDict){(*(Dict*)var)=*(Dict*)value;}
    else if(type==HalSMVariableType_HalSMSetVar){(*(HalSMSetVar*)var)=*(HalSMSetVar*)value;}
    else if(type==HalSMVariableType_HalSMReturn){(*(HalSMReturn*)var)=*(HalSMReturn*)value;}
    else if(type==HalSMVariableType_HalSMFunctionCTypeDef){(*(HalSMFunctionCTypeDef*)var)=*(HalSMFunctionCTypeDef*)value;}
    else if(type==HalSMVariableType_HalSMFunctionArray){(*(HalSMFunctionArray*)var)=*(HalSMFunctionArray*)value;}
    else if(type==HalSMVariableType_unsigned_int){(*(unsigned long long int*)var)=*(unsigned long long int*)value;}
    else if(type==HalSMVariableType_custom){(*(HalSMVariableCustom*)var)=*(HalSMVariableCustom*)value;}
    else if(type==HalSMVariableType_HalSMMoreEqual){(*(HalSMMoreEqual*)var)=*(HalSMMoreEqual*)value;}
    else if(type==HalSMVariableType_HalSMLessEqual){(*(HalSMLessEqual*)var)=*(HalSMLessEqual*)value;}
    else if(type==HalSMVariableType_HalSMShift){(*(HalSMShift*)var)=*(HalSMShift*)value;}
    else if(type==HalSMVariableType_HalSMPow){(*(HalSMPow*)var)=*(HalSMPow*)value;}
    else if(type==HalSMVariableType_HalSMModulo){(*(HalSMModulo*)var)=*(HalSMModulo*)value;}
    else if(type==HalSMVariableType_HalSMAND){(*(HalSMAND*)var)=*(HalSMAND*)value;}
    else if(type==HalSMVariableType_HalSMOR){(*(HalSMOR*)var)=*(HalSMOR*)value;}
    else if(type==HalSMVariableType_HalSMNegate){(*(HalSMNegate*)var)=*(HalSMNegate*)value;}
    else if(type==HalSMVariableType_HalSMNegativeSign){(*(HalSMNegativeSign*)var)=*(HalSMNegativeSign*)value;}
    else if(type==HalSMVariableType_HalSMGOTO){(*(HalSMGOTO*)var)=*(HalSMGOTO*)value;}
    else if(type==HalSMVariableType_HalSMGOTOSector){(*(HalSMGOTOSector*)var)=*(HalSMGOTOSector*)value;}
    else if(type==HalSMVariableType_HalSMInlineIf){(*(HalSMInlineIf*)var)=*(HalSMInlineIf*)value;}
    else if(type==HalSMVariableType_HalSMInlineElse){(*(HalSMInlineElse*)var)=*(HalSMInlineElse*)value;}
    else if(type==HalSMVariableType_HalSMBreak){(*(HalSMBreak*)var)=*(HalSMBreak*)value;}
    else if(type==HalSMVariableType_HalSMXOR){(*(HalSMXOR*)var)=*(HalSMXOR*)value;}
    else if(type==HalSMVariableType_HalSMBAND){(*(HalSMBAND*)var)=*(HalSMBAND*)value;}
    else if(type==HalSMVariableType_HalSMBOR){(*(HalSMBOR*)var)=*(HalSMBOR*)value;}
    else if(type==HalSMVariableType_HalSMBNOT){(*(HalSMBNOT*)var)=*(HalSMBNOT*)value;}
}

HalSMVariable* HalSMVariable_init_str(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str) {
    HalStringFormatChar** d=hsmmml->malloc(sizeof(HalStringFormatChar*));
    *d=hsmmml->calloc(string_len(str)+1,sizeof(HalStringFormatChar));
    stringncpy(*d,str,string_len(str));
    return HalSMVariable_init(hsmmml,d,HalSMVariableType_str);
}

HalSMVariable* HalSMVariable_init_int(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* n) {
    return HalSMVariable_init(hsmmml,n,HalSMVariableType_int);
}

HalSMVariable* HalSMVariable_init_double(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* d) {
    return HalSMVariable_init(hsmmml,d,HalSMVariableType_double);
}

HalSMVariable* HalSMVariable_init_bool(HalSMMemoryManagmentLibrary* hsmmml,unsigned char b) {
    unsigned char* c=hsmmml->malloc(sizeof(unsigned char));
    *c=b;
    return HalSMVariable_init(hsmmml,c,HalSMVariableType_HalSMBool);
}

HalSMVariable* HalSMVariable_init_unsigned_int(HalSMMemoryManagmentLibrary* hsmmml,unsigned long long int n) {
    unsigned long long int* i=hsmmml->malloc(sizeof(unsigned long long int));
    *i=n;
    return HalSMVariable_init(hsmmml,i,HalSMVariableType_unsigned_int);
}

HalSMVariable* HalSMVariable_init_char(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar c) {
    HalStringFormatChar* out=hsmmml->malloc(sizeof(HalStringFormatChar));
    *out=c;
    return HalSMVariable_init(hsmmml,out,HalSMVariableType_char);
}

HalStringFormatChar* HalSMVariable_to_str(HalSMCompiler* hsmc,HalSMVariable* a,unsigned char isToPrint)
{
    HalStringFormatChar* out;
    HalStringFormatChar* c;
    if (a->type==HalSMVariableType_int) {
        out=HalSMInteger_toString(hsmc->memorymanagmentlibrary,(HalSMInteger*)a->value,0);
    } else if (a->type==HalSMVariableType_unsigned_int) {
        out=HalSMInteger_toString(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,*(unsigned long long int*)a->value),0);
    } else if (a->type==HalSMVariableType_double) {
        out=HalSMDouble_toString(hsmc->memorymanagmentlibrary,(HalSMDouble*)a->value,0);
    } else if (a->type==HalSMVariableType_str) {
        if (isToPrint) {
            unsigned long long int l=string_len(*(HalStringFormatChar**)a->value);
            c=hsmc->memorymanagmentlibrary->malloc((l+3)*sizeof(HalStringFormatChar));
            c[0]=ASCIIH('"');
            c[1]=ASCIIH('\0');
            string_cat(c,*(HalStringFormatChar**)a->value);
            c[l+1]=ASCIIH('"');
            c[l+2]=ASCIIH('\0');
            out=c;
        } else {
            unsigned long long int l=string_len(*(HalStringFormatChar**)a->value);
            c=hsmc->memorymanagmentlibrary->malloc((l+1)*sizeof(HalStringFormatChar));
            string_cpy(c,*(HalStringFormatChar**)a->value);
            out=c;
        }
    } else if (a->type==HalSMVariableType_HalSMNull) {
        out=U8H("Null");
    } else if (a->type==HalSMVariableType_void) {
        out=U8H("(void)");
    } else if (a->type==HalSMVariableType_char) {
        c=hsmc->memorymanagmentlibrary->malloc(2*sizeof(HalStringFormatChar));
        c[0]=*(HalStringFormatChar*)a->value;
        c[1]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMArray) {
        out=HalSMArray_to_print(hsmc,(HalSMArray*)a->value);
    } else if (a->type==HalSMVariableType_HalSMRunClassC) {
        c=hsmc->memorymanagmentlibrary->malloc((21+string_len(((HalSMRunClassC*)a->value)->name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Running Class C ("));
        string_cat(c,((HalSMRunClassC*)a->value)->name);
        string_cat(c,U8H(")>"));
        c[string_len(((HalSMRunClassC*)a->value)->name)+20]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMLocalFunction) {
        HalStringFormatChar* name=((HalSMLocalFunction*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((19+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<LocalFunction ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[string_len(name)+18]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMFunctionC) {
        HalStringFormatChar* name=ConcatenateStrings(hsmc->memorymanagmentlibrary,U8H("0x"),hsmc->stringlibrary->Decimal2HexStr((unsigned long long int)((HalSMFunctionC*)a->value)->func));
        c=hsmc->memorymanagmentlibrary->malloc(35*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Function C at ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[string_len(name)+18]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMCModule) {
        HalStringFormatChar* name=((HalSMCModule*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((14+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Module C ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[13+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMModule) {
        HalStringFormatChar* name=((HalSMModule*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((12+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Module ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[13+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMClassC) {
        HalStringFormatChar* name=((HalSMClassC*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((13+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Class C ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[12+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMRunClassC) {
        HalStringFormatChar* name=((HalSMRunClassC*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((16+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<RunClass C ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[15+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMBool) {
        out=(*(unsigned char*)a->value)?U8H("true"):U8H("false");
    } else if (a->type==HalSMVariableType_HalSMRunClass) {
        HalStringFormatChar* name=((HalSMRunClass*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((16+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<RunClass ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[13+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMClass) {
        HalStringFormatChar* name=((HalSMClass*)a->value)->name;
        c=hsmc->memorymanagmentlibrary->malloc((13+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Class ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[10+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMVar) {
        HalStringFormatChar* name=*(HalStringFormatChar**)((HalSMVar*)a->value)->name->value;
        c=hsmc->memorymanagmentlibrary->malloc((13+string_len(name))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<HalSMVar ("));
        string_cat(c,name);
        string_cat(c,U8H(")>"));
        c[14+string_len(name)]=ASCIIH('\0');
        out=c;
    } else if (a->type==HalSMVariableType_HalSMDict) {
        out=Dict_to_print(hsmc,(Dict*)a->value);
    } else if (a->type==HalSMVariableType_custom) {
        out=((HalSMVariableCustom*)a->value)->type->toString(hsmc,a);
    } else if (a->type==HalSMVariableType_HalSMError) {
        HalStringFormatChar* error=((HalSMError*)a->value)->error;
        HalStringFormatChar* line=HalSMInteger_toString(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,((HalSMError*)a->value)->ind->line),0);
        c=hsmc->memorymanagmentlibrary->malloc((21+string_len(error)+string_len(line))*sizeof(HalStringFormatChar));
        string_cpy(c,U8H("<Error line="));
        string_cat(c,line);
        string_cat(c,U8H("; error="));
        string_cat(c,error);
        string_cat(c,U8H(">"));
        c[22+string_len(error)+string_len(line)]=ASCIIH('\0');
        out=c;
    } else {
        out=U8H("<Unknown Type>");
    }
    return out;
}

unsigned char HalSMVariable_Compare(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMVariableType type=v0->type;
    if (type!=v1->type)return 0;
    void* var=v0->value;
    void* value=v1->value;
    if(type==HalSMVariableType_int){return HalSMInteger_isEqual((HalSMInteger*)var,(HalSMInteger*)value);}
    else if(type==HalSMVariableType_char){return HSFCharCompare(*(HalStringFormatChar*)var,*(HalStringFormatChar*)value);}
    else if(type==HalSMVariableType_double){return HalSMDouble_isEqual((HalSMDouble*)var,(HalSMDouble*)value);}
    else if(type==HalSMVariableType_void){return 1;}
    else if(type==HalSMVariableType_HalSMArray){return HalSMArray_compare(hsmc,(HalSMArray*)var,(HalSMArray*)value);}
    else if(type==HalSMVariableType_str){return HSFCompare(*(HalStringFormatChar**)var,*(HalStringFormatChar**)value);}
    else if(type==HalSMVariableType_HalSMFunctionC){return ((HalSMFunctionC*)var)->func==((HalSMFunctionC*)value)->func;}
    else if(type==HalSMVariableType_HalSMRunClassC){return HSFCompare(((HalSMRunClassC*)var)->name,((HalSMRunClassC*)value)->name)&&DictCompare(hsmc,((HalSMRunClassC*)var)->vrs,((HalSMRunClassC*)value)->vrs)&&DictCompare(hsmc,((HalSMRunClassC*)var)->funcs,((HalSMRunClassC*)value)->funcs);}
    else if(type==HalSMVariableType_HalSMError){return ((HalSMError*)var)->ind==((HalSMError*)value)->ind&&HSFCompare(((HalSMError*)var)->error,((HalSMError*)value)->error);}
    else if(type==HalSMVariableType_HalSMNull){return 1;}
    else if(type==HalSMVariableType_HalSMRunFunc){return 1;}
    else if(type==HalSMVariableType_HalSMRunFuncC){return 1;}
    else if(type==HalSMVariableType_HalSMLocalFunction){return HSFCompare(((HalSMLocalFunction*)var)->name,((HalSMLocalFunction*)value)->name)&&HalSMArray_compare(hsmc,((HalSMLocalFunction*)var)->args,((HalSMLocalFunction*)value)->args)&&HalSMArray_compare(hsmc,((HalSMLocalFunction*)var)->func,((HalSMLocalFunction*)value)->func)&&DictCompare(hsmc,((HalSMLocalFunction*)var)->vars,((HalSMLocalFunction*)value)->vars);}
    else if(type==HalSMVariableType_HalSMCModule){return DictCompare(hsmc,((HalSMCModule*)var)->lfuncs,((HalSMCModule*)value)->lfuncs)&&DictCompare(hsmc,((HalSMCModule*)var)->vrs,((HalSMCModule*)value)->vrs)&&DictCompare(hsmc,((HalSMCModule*)var)->classes,((HalSMCModule*)value)->classes)&&HSFCompare(((HalSMCModule*)var)->name,((HalSMCModule*)value)->name);}
    else if(type==HalSMVariableType_HalSMModule){return HSFCompare(((HalSMModule*)var)->name,((HalSMModule*)value)->name)&&((HalSMModule*)var)->hsmc==((HalSMModule*)value)->hsmc;}
    else if(type==HalSMVariableType_HalSMClassC){return HSFCompare(((HalSMClassC*)var)->name,((HalSMClassC*)value)->name)&&DictCompare(hsmc,((HalSMClassC*)var)->vrs,((HalSMClassC*)value)->vrs)&&DictCompare(hsmc,((HalSMClassC*)var)->funcs,((HalSMClassC*)value)->funcs);}
    else if(type==HalSMVariableType_HalSMCompiler){return v0->value==v1->value;}
    else if(type==HalSMVariableType_HalSMRunClass){return HSFCompare(((HalSMRunClass*)var)->name,((HalSMRunClass*)value)->name)&&DictCompare(hsmc,((HalSMRunClass*)var)->funcs,((HalSMRunClass*)value)->funcs)&&DictCompare(hsmc,((HalSMRunClass*)var)->vars,((HalSMRunClass*)value)->vars);}
    else if(type==HalSMVariableType_HalSMDoubleGet){return HSFCompare(((HalSMDoubleGet*)var)->st,((HalSMDoubleGet*)value)->st);}
    else if(type==HalSMVariableType_HalSMClass){return HSFCompare(((HalSMClass*)var)->name,((HalSMClass*)value)->name)&&DictCompare(hsmc,((HalSMClass*)var)->funcs,((HalSMClass*)value)->funcs)&&DictCompare(hsmc,((HalSMClass*)var)->vars,((HalSMClass*)value)->vars);}
    else if(type==HalSMVariableType_HalSMVar){return HSFCompare(*(HalStringFormatChar**)((HalSMVar*)var)->name->value,*(HalStringFormatChar**)((HalSMVar*)value)->name->value);}
    else if(type==HalSMVariableType_HalSMPlus){return 1;}
    else if(type==HalSMVariableType_HalSMMinus){return 1;}
    else if(type==HalSMVariableType_HalSMMult){return 1;}
    else if(type==HalSMVariableType_HalSMDivide){return 1;}
    else if(type==HalSMVariableType_HalSMEqual){return 1;}
    else if(type==HalSMVariableType_HalSMNotEqual){return 1;}
    else if(type==HalSMVariableType_HalSMMore){return 1;}
    else if(type==HalSMVariableType_HalSMLess){return 1;}
    else if(type==HalSMVariableType_HalSMBool){return *(unsigned char*)var==*(unsigned char*)value;}
    else if(type==HalSMVariableType_HalSMSetVar){return HSFCompare(*(HalStringFormatChar**)((HalSMSetVar*)var)->name->value,*(HalStringFormatChar**)((HalSMSetVar*)value)->name->value)&&HSFCompare(((HalSMSetVar*)var)->value,((HalSMSetVar*)value)->value);}
    else if(type==HalSMVariableType_HalSMReturn){return HSFCompare(((HalSMReturn*)var)->value,((HalSMReturn*)value)->value);}
    else if(type==HalSMVariableType_HalSMFunctionArray){return ((HalSMFunctionArray*)var)->type==((HalSMFunctionArray*)value)->type&&HalSMArray_compare(hsmc,((HalSMFunctionArray*)var)->args,((HalSMFunctionArray*)value)->args);}
    else if(type==HalSMVariableType_unsigned_int){return *(unsigned long long int*)var==*(unsigned long long int*)value;}
    else if(type==HalSMVariableType_custom){return ((HalSMVariableCustom*)var)->type->isEqual(hsmc,var,value);}
    else if(type==HalSMVariableType_HalSMMoreEqual){return 1;}
    else if(type==HalSMVariableType_HalSMLessEqual){return 1;}
    else if(type==HalSMVariableType_HalSMShift){return ((HalSMShift*)var)->c==((HalSMShift*)value)->c;}
    else if(type==HalSMVariableType_HalSMPow){return 1;}
    else if(type==HalSMVariableType_HalSMModulo){return 1;}
    else if(type==HalSMVariableType_HalSMAND){return 1;}
    else if(type==HalSMVariableType_HalSMOR){return 1;}
    else if(type==HalSMVariableType_HalSMNegate){return 1;}
    else if(type==HalSMVariableType_HalSMNegativeSign){return 1;}
    else if(type==HalSMVariableType_HalSMGOTO){return HSFCompare(((HalSMGOTO*)var)->sector,((HalSMGOTO*)value)->sector);}
    else if(type==HalSMVariableType_HalSMGOTOSector){return HSFCompare(((HalSMGOTOSector*)var)->sector,((HalSMGOTOSector*)value)->sector)&&((HalSMGOTOSector*)var)->index==((HalSMGOTOSector*)value)->index;}
    else if(type==HalSMVariableType_HalSMInlineIf){return 1;}
    else if(type==HalSMVariableType_HalSMInlineElse){return 1;}
    else if(type==HalSMVariableType_HalSMBreak){return 1;}
    else if(type==HalSMVariableType_HalSMXOR){return 1;}
    else if(type==HalSMVariableType_HalSMBAND){return 1;}
    else if(type==HalSMVariableType_HalSMBOR){return 1;}
    else if(type==HalSMVariableType_HalSMBNOT){return 1;}
    return 0;
}

void HalSMVariable_free(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* arg)
{
    if (arg->type==HalSMVariableType_HalSMArray) {
        HalSMArray_free(hsmmml,(HalSMArray*)arg->value);
    } else {
        hsmmml->free(arg->value);
    }
    hsmmml->free(arg);
}

HalSMVariableCustom* HalSMVariableCustom_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariableTypeCustom* type,void* value,Dict* vars,Dict* funcs)
{
    HalSMVariableCustom* out=hsmmml->malloc(sizeof(HalSMVariableCustom));
    out->type=type;
    out->value=value;
    out->vars=vars;
    out->funcs=funcs;
    return out;
}

HalSMVariableTypeCustom* HalSMVariableTypeCustom_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariableTypeCustom type)
{
    HalSMVariableTypeCustom* out=hsmmml->malloc(sizeof(HalSMVariableTypeCustom));
    *out=type;
    return out;
}

HalSMSetVar* HalSMSetVar_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,HalStringFormatChar* value,HalSMVariable* pointer)
{
    HalSMSetVar* out=hsmmml->malloc(sizeof(HalSMSetVar));
    out->name=HalSMVariable_init_str(hsmmml,name);
    out->value=value;
    out->pointer=pointer;
    return out;
}

HalSMArray* HalSMArray_init(HalSMMemoryManagmentLibrary* hsmmml)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->size=0;
    out->arr=hsmmml->malloc(0);
    return out;
}

HalSMArray* HalSMArray_split_str(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str,HalStringFormatChar* spl)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->size=0;
    out->arr=hsmmml->malloc(0);
    unsigned long long int slen=string_len(str);
    unsigned long long int plen=string_len(spl);
    if (slen<plen) {
        return HalSMArray_init_with_elements(hsmmml,(HalSMVariable*[]){HalSMVariable_init_str(hsmmml,U8H(""))},1);
    }
    unsigned long long int i=0;
    HalStringFormatChar* arr;
    HalStringFormatChar* temp=hsmmml->malloc(0);
    unsigned long long int d=0;

    while (i<slen) {
        if (i<slen-plen+1) {
            arr=SubString(hsmmml,str,i,i+plen);
            if (HSFCompare(arr,spl)) {
                temp=hsmmml->realloc(temp,(d+1)*sizeof(HalStringFormatChar));
                temp[d]=ASCIIH('\0');
                HalSMArray_add(hsmmml,out,HalSMVariable_init_str(hsmmml,temp));
                i+=plen;
                d=0;
                temp=hsmmml->malloc(0);
            } else {
                d++;
                temp=hsmmml->realloc(temp,d*sizeof(HalStringFormatChar));
                temp[d-1]=str[i];
                i++;
            }
        } else {
            d++;
            temp=hsmmml->realloc(temp,d*sizeof(HalStringFormatChar));
            temp[d-1]=str[i];
            i++;
        }
    }

    if (string_len(temp)>0) {
        temp=hsmmml->realloc(temp,(d+1)*sizeof(HalStringFormatChar));
        temp[d]=ASCIIH('\0');
        HalSMArray_add(hsmmml,out,HalSMVariable_init_str(hsmmml,temp));
    } else {
        HalSMArray_add(hsmmml,out,HalSMVariable_init_str(hsmmml,U8H("")));
    }

    hsmmml->free(arr);
    hsmmml->free(temp);
    return out;
}

long long int HalSMArray_index(HalSMCompiler* hsmc,HalSMArray* harr,HalSMVariable* value)
{
    for (unsigned long long int i=0;i<harr->size;i++)
    {
        if (HalSMVariable_Compare(hsmc,harr->arr[i],value))return i;
    }
    return -1;
}

long long int HalSMArray_right_index(HalSMCompiler* hsmc,HalSMArray* harr,HalSMVariable* value)
{
    for (unsigned long long int i=harr->size-1;;i++) {
        if (HalSMVariable_Compare(hsmc,harr->arr[i],value))return i;
        if (i==0){break;}
    }
    return -1;
}

void HalSMArray_add(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value)
{
    HalSMVariable** old=harr->arr;
    HalSMVariable** oldp=harr->arr;
    harr->arr=hsmmml->malloc((harr->size+1)*sizeof(HalSMVariable*));
    HalSMVariable** p=harr->arr;
    for (unsigned long long int i=0;i<harr->size;i++){*p++=*oldp++;}
    harr->arr[harr->size++]=value;
    hsmmml->free(old);
}

void HalSMArray_set(HalSMArray* harr,HalSMVariable* value,unsigned long long int index)
{
    if (index<harr->size) {
        harr->arr[index]=value;
    }
}

void HalSMArray_remove(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned long long int index)
{
    HalSMVariable** arr=hsmmml->malloc((harr->size-1)*sizeof(HalSMVariable*));
    HalSMVariable** ap=arr;
    HalSMVariable** aa=harr->arr;
    for (unsigned long long int i=0;i<harr->size;i++) {
        if(i!=index){*ap++=*aa++;}
    }
    harr->size=harr->size-1;
    hsmmml->free(harr->arr);
    harr->arr=arr;
}

HalSMArray* HalSMArray_reverse(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    HalSMArray* out=HalSMArray_init(hsmmml);
    for (unsigned long long int i=harr->size-1;i--;){HalSMArray_add(hsmmml,out,harr->arr[i]);if (i==0) {break;}}
    return out;
}

void HalSMArray_appendArray(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMArray* narr)
{
    harr->arr=hsmmml->realloc(harr->arr,(harr->size+narr->size)*sizeof(HalSMVariable*));
    HalSMVariable** p=&harr->arr[harr->size];
    HalSMVariable** pn=&narr->arr[0];
    for (unsigned long long int i=0;i<narr->size;i++){*p++=*pn++;}
    harr->size=harr->size+narr->size;
}

void HalSMArray_insert(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalSMVariable* value,unsigned long long int index)
{
    //Error
    if (index>harr->size) {return;}
    HalSMVariable** temp=hsmmml->malloc(sizeof(HalSMVariable*)*(harr->size+1));
    unsigned long long int i;
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

HalSMVariable* HalSMArray_get(HalSMArray* harr,unsigned long long int index)
{
    if (harr->size==0||index>=harr->size){HalSMVariable* temp;return temp;}
    return harr->arr[index];
}

void AdditionStrings(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar** c,HalStringFormatChar* f,unsigned long long int sizec,unsigned long long int sizef)
{
    HalStringFormatChar* tmp=hsmmml->calloc(sizec+1,sizeof(HalStringFormatChar));
    memory_cpy(tmp,*c,sizec*sizeof(HalStringFormatChar));
    *c=hsmmml->calloc(sizec+sizef+1,sizeof(HalStringFormatChar));
    memory_cpy(*c,tmp,sizec*sizeof(HalStringFormatChar));
    memory_cpy(*c+string_len(*c),f,sizef*sizeof(HalStringFormatChar));
    hsmmml->free(tmp);
}

HalStringFormatChar* HalSMArray_join_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,HalStringFormatChar* join)
{
    if(harr->size==0) {
        return U8H("");
    } else if (harr->size==1) {
        return *(HalStringFormatChar**)harr->arr[0]->value;
    }
    unsigned long long int size=0;
    unsigned long long int lj=string_len(join);
    HalStringFormatChar* out=hsmmml->calloc(1,sizeof(HalStringFormatChar));
    out[0]=ASCIIH('\0');
    unsigned long long int i;
    size=0;
    HalSMVariable* o;
    for (i=0;i<harr->size;i++) {
        o=harr->arr[i];
        HalStringFormatChar* oc=*(HalStringFormatChar**)o->value;
        AdditionStrings(hsmmml,&out,oc,size,string_len(oc));
        size+=string_len(oc);
        if (i!=harr->size-1) {
            AdditionStrings(hsmmml,&out,join,size,lj);
            size+=lj;
        }
    }
    out[size]=ASCIIH('\0');
    return out;
}

HalStringFormatChar* HalSMArray_to_print(HalSMCompiler* hsmc,HalSMArray* harr)
{
    HalStringFormatChar* joinout=HalSMArray_join_str(hsmc->memorymanagmentlibrary,HalSMCompiler_get_print_text(hsmc,harr,1),U8H(", "));
    unsigned long long int l=string_len(joinout);
    HalStringFormatChar* outs=hsmc->memorymanagmentlibrary->malloc((3+l)*sizeof(HalStringFormatChar));
    string_cpy(outs,U8H("["));
    string_cat(outs,joinout);
    string_cat(outs,U8H("]"));
    outs[l+2]=ASCIIH('\0');
    return outs;
}

HalStringFormatChar* HalSMArray_chars_to_str(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    HalStringFormatChar* out=hsmmml->malloc((harr->size+1)*sizeof(HalStringFormatChar));
    for (unsigned long long int i=0;i<harr->size;i++) {out[i]=*(HalStringFormatChar*)harr->arr[i]->value;}
    out[harr->size]=ASCIIH('\0');
    return out;
}

HalSMArray* HalSMArray_slice(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr,unsigned long long int s,unsigned long long int e)
{
    if (s>=e){
        //Error
    }
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->arr=hsmmml->malloc((MathMin(e,harr->size)-s)*sizeof(HalSMVariable*));
    out->size=MathMin(e,harr->size)-s;

    unsigned long long int mm=MathMin(e,harr->size);

    for (unsigned long long int i=s,d=0;i<mm;i++,d++) {out->arr[d]=harr->arr[i];}
    return out;
}

unsigned char HalSMArray_compare(HalSMCompiler* hsmc,HalSMArray* harr,HalSMArray* barr)
{
    if (harr->size!=barr->size){return 0;}
    for (unsigned long long int i=0;i<harr->size;i++) {
        if (HalSMVariable_Compare(hsmc,harr->arr[i],barr->arr[i])==0){return 0;}
    }
    return 1;
}

HalSMArray* HalSMArray_from_str(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str,unsigned long long int size)
{
    HalSMArray* out=HalSMArray_init(hsmmml);
    for (unsigned long long int i=0;i<size;i++) {HalSMArray_add(hsmmml,out,HalSMVariable_init_char(hsmmml,str[i]));}
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

void HalSMArray_free(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    for (unsigned long long int i=0;i<harr->size;i++) {
        HalSMVariable_free(hsmmml,harr->arr[i]);
    }
    hsmmml->free(harr->arr);
    hsmmml->free(harr);
}

long long int HalSMArray_find(HalSMCompiler* hsmc,HalSMArray* harr,HalSMVariable* value)
{
    HalSMVariable** p=harr->arr;
    for (unsigned long long int i=0;i<harr->size;i++) {
        if (HalSMVariable_Compare(hsmc,*p++,value)){return i;}
    }
    return -1;
}

void HalSMArray_clear(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* harr)
{
    harr->arr=hsmmml->realloc(harr->arr,0);
    harr->size=0;
}

HalSMArray* HalSMArray_init_with_elements(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable** arr,unsigned long long int size)
{
    HalSMArray* out=hsmmml->malloc(sizeof(HalSMArray));
    out->arr=hsmmml->malloc(size*sizeof(HalSMVariable*));
    memory_cpy(out->arr,arr,size*sizeof(HalSMVariable*));
    out->size=size;
    return out;
}

HalSMFunctionC* HalSMFunctionC_init(HalSMCompiler* hsmc,HalSMFunctionCTypeDef func)
{
    HalSMFunctionC* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionC));
    out->func=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionCTypeDef));
    *out->func=func;
    out->type=0;
    out->data=NOTHALSMNULLPOINTER;
    out->hsmc=hsmc;
    return out;
}

HalSMFunctionC* HalSMFunctionC_initWithData(void* data,HalSMCompiler* hsmc,HalSMFunctionCTypeDefWithData func)
{
    HalSMFunctionC* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionC));
    out->func=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMFunctionCTypeDefWithData));
    *out->func=func;
    out->type=1;
    out->data=data;
    out->hsmc=hsmc;
    return out;
}

HalSMVariable* HalSMFunctionC_run(HalSMFunctionC* hfc,HalSMArray* args,Dict* vars) {
    if (hfc->type==1){return (*(HalSMFunctionCTypeDefWithData*)hfc->func)(hfc->data,hfc->hsmc,args,vars);}
    return (*(HalSMFunctionCTypeDef*)hfc->func)(hfc->hsmc,args,vars);
}

HalSMRunClassC* HalSMRunClassC_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,Dict* vrs,Dict* funcs)
{
    HalSMRunClassC* runclassc=hsmmml->malloc(sizeof(HalSMRunClassC));
    runclassc->name=name;
    runclassc->vrs=vrs;
    runclassc->funcs=funcs;
    return runclassc;
}

HalSMRunClassC* HalSMRunClassC__init__(HalSMCompiler* hsmc,HalSMRunClassC* runclassc,HalSMArray* args)
{
    long long int indexInit=DictElementIndexByKey(hsmc,runclassc->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__init__")));
    if (indexInit!=-1){
        HalSMFunctionC_run((HalSMFunctionC*)DictElementFindByIndex(runclassc->funcs,indexInit)->value->value,args,&NULLDict);
    }
    return runclassc;
}

HalSMClassC* HalSMClassC_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name)
{
    HalSMClassC* classc=hsmmml->malloc(sizeof(HalSMClassC));
    classc->vrs=DictInit(hsmmml);
    classc->funcs=DictInit(hsmmml);
    classc->name=name;
    return classc;
}

HalSMRunClassC* HalSMClassC_run(HalSMCompiler* hsmc,HalSMClassC* classc,HalSMArray* args)
{
    HalSMRunClassC* o=HalSMRunClassC_init(hsmc->memorymanagmentlibrary,classc->name,classc->vrs,classc->funcs);
    HalSMArray_insert(hsmc->memorymanagmentlibrary,args,HalSMVariable_init(hsmc->memorymanagmentlibrary,o,HalSMVariableType_HalSMRunClassC),0);
    return HalSMRunClassC__init__(hsmc,o,args);
}

HalSMClass* HalSMClass_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,Dict* vrs)
{
    HalSMClass* out=hsmmml->malloc(sizeof(HalSMClass));
    //out->vars=vrs;
    out->vars=DictInit(hsmmml);
    out->funcs=DictInit(hsmmml);
    out->name=name;
    return out;
}

HalSMRunClass* HalSMClass_run(HalSMClass* clas,HalSMCompiler* hsmc,HalSMArray* args)
{
    HalSMRunClass* out=HalSMRunClass_init(hsmc->memorymanagmentlibrary,clas->name,clas->vars,clas->funcs);
    return HalSMRunClass__init__(out,hsmc,args);
}

HalSMRunClass* HalSMRunClass_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,Dict* vrs,Dict* funcs)
{
    HalSMRunClass* out=hsmmml->malloc(sizeof(HalSMRunClass));
    out->name=name;
    out->vars=DictCopy(hsmmml,vrs);
    out->funcs=DictCopy(hsmmml,funcs);
    return out;
}

HalSMRunClass* HalSMRunClass__init__(HalSMRunClass* runclass,HalSMCompiler* hsmc,HalSMArray* args)
{
    if (DictElementIndexByKey(hsmc,runclass->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__init__")))!=-1) {
        HalSMLocalFunction* func=(HalSMLocalFunction*)DictElementFindByKey(hsmc,runclass->funcs,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U8H("__init__")))->value->value;
        HalSMArray_insert(hsmc->memorymanagmentlibrary,args,HalSMVariable_init(hsmc->memorymanagmentlibrary,runclass,HalSMVariableType_HalSMRunClass),0);
        HalSMLocalFunction_run(func,args,&NULLDict);
    }
    return runclass;
}

HalSMCModule* HalSMCModule_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name)
{
    HalSMCModule* out=hsmmml->malloc(sizeof(HalSMCModule));
    out->lfuncs=DictInit(hsmmml);
    out->vrs=DictInit(hsmmml);
    out->classes=DictInit(hsmmml);
    out->name=name;
    return out;
}

HalSMCModule* HalSMCModule_load(HalSMCompiler* hsmc,HalSMCModule_entry* entry)
{
    HalSMCModule* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMCModule));
    out->lfuncs=entry->lfuncs;
    out->vrs=entry->vrs;
    out->classes=entry->classes;
    out->name=entry->name;
    return out;
}

HalSMModule* HalSMModule_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* name,HalSMCompiler* hsmcmodule)
{
    HalSMModule* o=hsmmml->malloc(sizeof(HalSMModule));
    o->hsmc=hsmcmodule;
    o->name=name;
    return o;
}

HalSMRunFuncC* HalSMRunFuncC_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMFunctionC* func,HalSMArray* args)
{
    HalSMRunFuncC* out=hsmmml->malloc(sizeof(HalSMRunFuncC));
    out->func=func;
    out->args=args;
    out->pointer=HalSMArray_init(hsmmml);
    return out;
}

HalSMRunFunc* HalSMRunFunc_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMLocalFunction* func,HalSMArray* args)
{
    HalSMRunFunc* out=hsmmml->malloc(sizeof(HalSMRunFunc));
    out->func=func;
    out->args=args;
    out->pointer=HalSMArray_init(hsmmml);
    return out;
}

unsigned char HalSMIsInt(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c)
{
    HalStringFormatChar b;
    unsigned long long int l=string_len(c);
    for (unsigned long long int i=0;i<l;i++)
    {
        b=c[i];
        if (HSFCharCompare(b,ASCIIH('-'))&&i==0){continue;}
        else if (HSFCharCompare(b,ASCIIH('0'))==0&&HSFCharCompare(b,ASCIIH('1'))==0&&HSFCharCompare(b,ASCIIH('2'))==0&&HSFCharCompare(b,ASCIIH('3'))==0&&HSFCharCompare(b,ASCIIH('4'))==0&&HSFCharCompare(b,ASCIIH('5'))==0&&HSFCharCompare(b,ASCIIH('6'))==0&&HSFCharCompare(b,ASCIIH('7'))==0&&HSFCharCompare(b,ASCIIH('8'))==0&&HSFCharCompare(b,ASCIIH('9'))==0){return 0;}
    }
    return 1;
}

unsigned char HalSMIsDouble(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c)
{
    HalStringFormatChar b;
    long long int ct=0;
    unsigned long long int l=string_len(c);
    for (unsigned long long int i=0;i<l;i++)
    {
        b=c[i];
        if (HSFCharCompare(b,ASCIIH('-'))&&i>0||(HSFCharCompare(b,ASCIIH('0'))==0&&HSFCharCompare(b,ASCIIH('1'))==0&&HSFCharCompare(b,ASCIIH('2'))==0&&HSFCharCompare(b,ASCIIH('3'))==0&&HSFCharCompare(b,ASCIIH('4'))==0&&HSFCharCompare(b,ASCIIH('5'))==0&&HSFCharCompare(b,ASCIIH('6'))==0&&HSFCharCompare(b,ASCIIH('7'))==0&&HSFCharCompare(b,ASCIIH('8'))==0&&HSFCharCompare(b,ASCIIH('9'))==0)){return 0;}
        else if(HSFCharCompare(b,ASCIIH('.')) && (i==0 || i==string_len(c)-1)){return 0;}
        else if(HSFCharCompare(b,ASCIIH('.')) && i>0 && i<string_len(c)){ct+=1;if(ct==2)return 0;}
    }
    if (ct==0){return 0;}
    return 1;
}

unsigned char HalSMIsHex(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c)
{
    HalStringFormatChar b;
    unsigned long long int l=string_len(c);
    if (l<3){return 0;}
    HalStringFormatChar* p=c;
    if (HSFCharCompare(*p,ASCIIH('-'))){p++;l--;}
    if (HSFCharCompare(p[0],ASCIIH('0'))==0||HSFCharCompare(p[1],ASCIIH('x'))==0){return 0;}
    p+=2;
    l-=2;

    for (unsigned long long int i=0;i<l;i++,p++)
    {
        b=*p;
        if (HSFCharCompare(b,ASCIIH('0'))==0&&HSFCharCompare(b,ASCIIH('1'))==0&&HSFCharCompare(b,ASCIIH('2'))==0&&HSFCharCompare(b,ASCIIH('3'))==0&&HSFCharCompare(b,ASCIIH('4'))==0&&HSFCharCompare(b,ASCIIH('5'))==0&&HSFCharCompare(b,ASCIIH('6'))==0&&HSFCharCompare(b,ASCIIH('7'))==0&&HSFCharCompare(b,ASCIIH('8'))==0&&HSFCharCompare(b,ASCIIH('9'))==0&&HSFCharCompare(b,ASCIIH('a'))==0&&HSFCharCompare(b,ASCIIH('b'))==0&&HSFCharCompare(b,ASCIIH('c'))==0&&HSFCharCompare(b,ASCIIH('d'))==0&&HSFCharCompare(b,ASCIIH('e'))==0&&HSFCharCompare(b,ASCIIH('f'))==0&&HSFCharCompare(b,ASCIIH('A'))==0&&HSFCharCompare(b,ASCIIH('B'))==0&&HSFCharCompare(b,ASCIIH('C'))==0&&HSFCharCompare(b,ASCIIH('D'))==0&&HSFCharCompare(b,ASCIIH('E'))==0&&HSFCharCompare(b,ASCIIH('F'))==0){return 0;}
    }
    return 1;
}

unsigned char HalSMIsBin(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c)
{
    HalStringFormatChar b;
    unsigned long long int l=string_len(c);
    if (l<3){return 0;}
    HalStringFormatChar* p=c;
    if (HSFCharCompare(*p,ASCIIH('-'))){p++;l--;}
    if (HSFCharCompare(p[0],ASCIIH('0'))==0||HSFCharCompare(p[1],ASCIIH('b'))==0){return 0;}
    p+=2;
    l-=2;

    for (unsigned long long int i=0;i<l;i++,p++)
    {
        b=*p;
        if (HSFCharCompare(b,ASCIIH('0'))==0&&HSFCharCompare(b,ASCIIH('1'))==0){return 0;}
    }
    return 1;
}

HalSMVariable* ParseHalSMVariableInt(HalSMCompiler* hsmc,HalStringFormatChar* c)
{
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_parse(hsmc->memorymanagmentlibrary,c),HalSMVariableType_int);
}

HalSMVariable* ParseHalSMVariableDouble(HalSMCompiler* hsmc,HalStringFormatChar* c)
{
    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMDouble_parse(hsmc->memorymanagmentlibrary,c),HalSMVariableType_double);
}

HalSMVariable* HalSMLocalFunction_run(HalSMLocalFunction* lf,HalSMArray* args,Dict* vars)
{
    unsigned long long int ia=0;
    HalSMVariable* v;
    Dict* vrs;
    HalSMCompiler* hsmc=lf->hsmc;
    HalSMErrorIndificator* ind=HalSMErrorIndificator_init(hsmc,lf->name,0,hsmc->curInd);
    hsmc->curInd=ind;
    if (vars->size>0) {
        vrs=DictInitWithElements(hsmc->memorymanagmentlibrary,vars->elements,vars->size);
    } else {
        vrs=DictInit(hsmc->memorymanagmentlibrary);
    }
    DictAppendDict(hsmc->memorymanagmentlibrary,vrs,lf->vars);

    if (lf->module->type==HalSMVariableType_HalSMModule) {
        hsmc=((HalSMModule*)lf->module->value)->hsmc;
    }

    for (unsigned long long int arg=0;arg<args->size;arg++)
    {
        v=args->arr[arg];
        if (v->type==HalSMVariableType_HalSMSetArg)
        {
            PutDictElementToDict(hsmc,vrs,DictElementInit(hsmc->memorymanagmentlibrary,((HalSMSetArg*)v->value)->name,((HalSMSetArg*)v->value)->value));
            ia++;
            continue;
        }
        if (lf->args->size<=arg-ia) {
            HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Arguments more than parameters")));
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Arguments more than parameters")),HalSMVariableType_HalSMError);
        }
        if (lf->args->arr[arg-ia]->type==HalSMVariableType_HalSMVar) {
            PutDictElementToDict(hsmc,vrs,DictElementInit(hsmc->memorymanagmentlibrary,((HalSMVar*)lf->args->arr[arg-ia]->value)->name,v));
        } else if (lf->args->arr[arg-ia]->type==HalSMVariableType_HalSMSetArg) {
            PutDictElementToDict(hsmc,vrs,DictElementInit(hsmc->memorymanagmentlibrary,((HalSMSetArg*)lf->args->arr[arg-ia]->value)->name,v));
        } else {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Unknown Type Argument")),HalSMVariableType_HalSMError);
        }
    }
    HalSMVariable* ad;
    HalSMSetArg* argi;
    HalSMVar* argv;
    HalStringFormatChar e=ASCIIH('=');
    unsigned long long int a;
    for (a=0;a<lf->args->size;a++)
    {
        if (lf->args->arr[a]->type==HalSMVariableType_HalSMSetArg) {
            argi=(HalSMSetArg*)lf->args->arr[a]->value;
            ad=argi->name;
            if (DictElementIndexByKey(hsmc,vrs,ad)!=-1){continue;}
            /*HalSMArray temparrvar={1,(HalSMVariable*[]){argi->value}};
            HalSMVariable* tempvarp=HalSMVariable_init(hsmc->memorymanagmentlibrary,&hnull,HalSMVariableType_HalSMNull);
            HalSMCompiler_getArgsSetVar(hsmc,&temparrvar,tempvarp,vrs);*/
            unsigned char isOut=0;
            PutDictElementToDict(hsmc,vrs,DictElementInit(hsmc->memorymanagmentlibrary,ad,HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)argi->value->value,&isOut,0,vars)));
            if (isOut==0) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Wrong set arg value")));
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Wrong set arg value")),HalSMVariableType_HalSMError);
            }
        } else if (lf->args->arr[a]->type==HalSMVariableType_HalSMVar) {
            argv=(HalSMVar*)lf->args->arr[a]->value;
            if (DictElementIndexByKey(hsmc,vrs,argv->name)!=-1) {
                continue;
            } else {
                HalSMError* err=HalSMError_init(hsmc,hsmc->curInd,U8H("Not enough args"));
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,err,HalSMVariableType_HalSMError);
            }
        }
    }

    /*if (lf->module->type!=HalSMVariableType_HalSMNull) {
        HalSMModule* module=(HalSMModule*)lf->module->value;
        for (a=0;a<module->hsmc->classes->size;a++) {
            PutDictElementToDict(hsmc,vrs,module->hsmc->classes->elements[a]);
        }

        for (a=0;a<module->hsmc->localFunctions->size;a++) {
            PutDictElementToDict(hsmc,vrs,module->hsmc->localFunctions->elements[a]);
        }

        for (a=0;a<module->hsmc->variables->size;a++) {
            PutDictElementToDict(hsmc,vrs,module->hsmc->variables->elements[a]);
        }
    }*/

    HalSMArray* arguss,*retargs;
    HalSMVariable* arg;
    unsigned char isFind;
    HalSMVariable* err;
    HalSMVariable* ifr=&null;
    HalSMVariable* f;
    HalSMArray* spliti;
    HalSMFunctionArray* resRunFunc;
    HalStringFormatChar* joinSpliti;
    HalSMVariable* isGOTO=&null;
    HalSMGOTOSector* tempSector;
    unsigned char isGet;
    for (unsigned long long int fi=0;fi<lf->func->size;fi++)
    {
        f=lf->func->arr[fi];
        if (f->type==HalSMVariableType_HalSMGOTOSector) {
            if (isGOTO->type==HalSMVariableType_HalSMGOTO&&HSFCompare(((HalSMGOTOSector*)f->value)->sector,((HalSMGOTO*)isGOTO->value)->sector)) {
                isGOTO=&null;
            }
            tempSector=(HalSMGOTOSector*)f->value;
            tempSector->index=fi;
            tempSector->lf=lf;
            PutDictElementToDict(hsmc,vrs,DictElementInit(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,tempSector->sector),f));
        } else if (isGOTO->type!=HalSMVariableType_HalSMNull) {

        } else if (f->type==HalSMVariableType_HalSMRunFuncC) {
            arguss=((HalSMRunFuncC*)f->value)->args;
            HalSMCompiler_getArgsFunction(hsmc,arguss,((HalSMRunFuncC*)f->value)->pointer,vrs);
            err=HalSMFunctionC_run(((HalSMRunFuncC*)f->value)->func,((HalSMRunFuncC*)f->value)->pointer,vrs);
            if(err->type==HalSMVariableType_HalSMError){return err;}
            else if(err->type==HalSMVariableType_HalSMGOTOSector) {
                tempSector=(HalSMGOTOSector*)err->value;
                if (tempSector->lf!=lf){return err;}
                fi=tempSector->index;
            } else if (err->type==HalSMVariableType_HalSMBreak){return err;}
            else if(err->type==HalSMVariableType_HalSMReturn) {
                if (((HalSMReturn*)err->value)->idLocalFunction==lf->id) {
                    retargs=HalSMCompiler_getArgs(hsmc,((HalSMReturn*)err->value)->value,vrs);
                    if (retargs->size==1) {
                        return retargs->arr[0];
                    }
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,retargs,HalSMVariableType_HalSMArray);
                }
                return err;
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=&null;
        } else if(f->type==HalSMVariableType_HalSMRunFunc) {
            arguss=((HalSMRunFunc*)f->value)->args;
            HalSMCompiler_getArgsFunction(hsmc,arguss,((HalSMRunFunc*)f->value)->pointer,vrs);
            err=HalSMLocalFunction_run(((HalSMRunFunc*)f->value)->func,((HalSMRunFunc*)f->value)->pointer,vrs);
            if(err->type==HalSMVariableType_HalSMError){return err;}
            else if(err->type==HalSMVariableType_HalSMGOTOSector) {
                tempSector=(HalSMGOTOSector*)err->value;
                if (tempSector->lf!=lf){return err;}
                fi=tempSector->index;
            } else if (err->type==HalSMVariableType_HalSMBreak){return err;}
            else if(err->type==HalSMVariableType_HalSMReturn) {
                if (((HalSMReturn*)err->value)->idLocalFunction==lf->id) {
                    retargs=HalSMCompiler_getArgs(hsmc,((HalSMReturn*)err->value)->value,vrs);
                    if (retargs->size==1) {
                        return retargs->arr[0];
                    }
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,retargs,HalSMVariableType_HalSMArray);
                }
                return err;
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=&null;
        } else if (f->type==HalSMVariableType_HalSMLocalFunction) {
            err=HalSMLocalFunction_run(((HalSMLocalFunction*)f->value),HalSMArray_init_with_elements(hsmc->memorymanagmentlibrary,(HalSMVariable*[]){HalSMVariable_init(hsmc->memorymanagmentlibrary,vrs,HalSMVariableType_HalSMDict)},1),vrs);
            if(err->type==HalSMVariableType_HalSMError){return err;}
            else if(err->type==HalSMVariableType_HalSMGOTOSector) {
                tempSector=(HalSMGOTOSector*)err->value;
                if (tempSector->lf!=lf){return err;}
                fi=tempSector->index;
            } else if (err->type==HalSMVariableType_HalSMBreak){return err;}
            else if(err->type==HalSMVariableType_HalSMReturn) {
                if (((HalSMReturn*)err->value)->idLocalFunction==lf->id) {
                    retargs=HalSMCompiler_getArgs(hsmc,((HalSMReturn*)err->value)->value,vrs);
                    if (retargs->size==1) {
                        return retargs->arr[0];
                    }
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,retargs,HalSMVariableType_HalSMArray);
                }
                return err;
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=&null;
        } else if (f->type==HalSMVariableType_HalSMSetVar) {
            //HalSMCompiler_getArgsSetVar(hsmc,((HalSMSetVar*)f->value)->value,((HalSMSetVar*)f->value)->pointer,vrs);
            ((HalSMSetVar*)f->value)->pointer=HalSMCompiler_getArgs(hsmc,((HalSMSetVar*)f->value)->value,vrs)->arr[0];
            arg=((HalSMSetVar*)f->value)->pointer;
            HalStringFormatChar* nameVar=*(HalStringFormatChar**)((HalSMSetVar*)f->value)->name->value;
            HalSMVariable* isGetV=HalSMCompiler_isGetU(hsmc,nameVar,&isGet,1,vrs);
            if (isGet) {
                isGetV->value=arg->value;
                isGetV->type=arg->type;
            }
            if (ifr->type!=HalSMVariableType_HalSMNull) ifr=&null;
        } else if (f->type==HalSMVariableType_HalSMFunctionArray) {
            resRunFunc=(HalSMFunctionArray*)f->value;
            if (resRunFunc->args->arr[0]->type==HalSMVariableType_HalSMVar) {
                unsigned char isOut=0;
                err=HalSMCompiler_isGetU(hsmc,*(HalStringFormatChar**)((HalSMVar*)resRunFunc->args->arr[0]->value)->name->value,&isOut,0,vrs);
                if (isOut) {
                    if (err->type==HalSMVariableType_HalSMError){return err;}
                    else if(err->type==HalSMVariableType_HalSMGOTOSector) {
                        tempSector=(HalSMGOTOSector*)err->value;
                        if (tempSector->lf!=lf){return err;}
                        fi=tempSector->index;
                    } else if (err->type==HalSMVariableType_HalSMBreak){return err;}
                    else if(err->type==HalSMVariableType_HalSMReturn) {
                        if (((HalSMReturn*)err->value)->idLocalFunction==lf->id) {
                            retargs=HalSMCompiler_getArgs(hsmc,((HalSMReturn*)err->value)->value,vrs);
                            if (retargs->size==1) {
                                return retargs->arr[0];
                            }
                            return HalSMVariable_init(hsmc->memorymanagmentlibrary,retargs,HalSMVariableType_HalSMArray);
                        }
                        return err;
                    }
                } else {
                    return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMError_init(hsmc,hsmc->curInd,U8H("Variable Not Found")),HalSMVariableType_HalSMError);
                }
            }
        } else if (f->type==HalSMVariableType_HalSMGOTO) {
            HalSMGOTO* gt=(HalSMGOTO*)f->value;
            err=HalSMVariable_init_str(hsmc->memorymanagmentlibrary,gt->sector);
            HalSMVariable* ret;
            if (DictElementIndexByKey(hsmc,vrs,err)!=-1&&(ret=DictElementFindByKey(hsmc,vrs,err)->value)->type==HalSMVariableType_HalSMGOTOSector) {
                tempSector=(HalSMGOTOSector*)ret->value;
                if (tempSector->lf!=lf){return ret;}
                fi=tempSector->index;
            } else if (DictElementIndexByKey(hsmc,hsmc->GOTOSectors,err)!=-1) {
                //GOTO Function
                return DictElementFindByKey(hsmc,hsmc->GOTOSectors,err)->value;
            } else {
                isGOTO=f;
            }
        } else if (f->type==HalSMVariableType_HalSMBreak) {
            return f;
        } else if (f->type==HalSMVariableType_HalSMReturn) {
            if (((HalSMReturn*)f->value)->idLocalFunction==lf->id){
                retargs=HalSMCompiler_getArgs(hsmc,((HalSMReturn*)f->value)->value,vrs);
                if (retargs->size==1) {
                    return retargs->arr[0];
                }
                return HalSMVariable_init(hsmc->memorymanagmentlibrary,retargs,HalSMVariableType_HalSMArray);
            }
            return f;
        }
        ind->line++;
        hsmc->curInd=ind;
    }
    END_LOCAL_FUNCTION:
    return &hvoid;
}

HalSMArray* HalSMLocalFunction_getArgs(HalSMCompiler* hsmc,HalStringFormatChar* l)
{
    HalStringFormatChar i;
    HalSMArray* n=HalSMArray_init(hsmc->memorymanagmentlibrary);
    unsigned long long int ll=string_len(l);

    unsigned long long int isA=0,isF=0,isD=0;
    HalStringFormatChar isS=ASCIIH('n');
    HalSMArray* out=HalSMArray_init(hsmc->memorymanagmentlibrary);
    HalStringFormatChar* isSetArg=NOTHALSMNULLPOINTER;

    for (unsigned long long int ind=0;ind<ll;ind++) {
        i=l[ind];
        
        if (isA) {
            if (HSFCharCompare(i,ASCIIH('['))) {
                isA++;
            } else if (HSFCharCompare(i,ASCIIH(']'))) {
                isA--;
            }
        } else if (isF) {
            if (HSFCharCompare(i,ASCIIH('('))) {
                isF++;
            } else if (HSFCharCompare(i,ASCIIH(')'))) {
                isF--;
            }

        } else if (HSFCharCompare(isS,ASCIIH('n'))==0) {
            if (HSFCharCompare(i,isS)){isS=ASCIIH('n');}
        } else if (isD) {
            if (HSFCharCompare(i,ASCIIH('{'))) {
                isD++;
            } else if (HSFCharCompare(i,ASCIIH('}'))) {
                isD--;
            }
        } else if (HSFCharCompare(i,ASCIIH('\''))||HSFCharCompare(i,ASCIIH('"'))) {
            isS=i;
        } else if (HSFCharCompare(i,ASCIIH('['))) {
            isA++;
        } else if (HSFCharCompare(i,ASCIIH('{'))) {
            isD++;
        } else if (HSFCharCompare(i,ASCIIH('('))) {
            isF++;
        } else if (HSFCharCompare(i,ASCIIH('='))&&n->size>0) {
            if (HSFCharCompare(l[ind-1],ASCIIH('+'))==0&&HSFCharCompare(l[ind-1],ASCIIH('-'))==0&&HSFCharCompare(l[ind-1],ASCIIH('*'))==0&&HSFCharCompare(l[ind-1],ASCIIH('/'))==0) {
                isSetArg=HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n);
                HalSMArray_clear(hsmc->memorymanagmentlibrary,n);
                continue;
            }
        } else if (HSFCharCompare(i,ASCIIH(','))) {
            if (n->size==0) {
                HalSMCompiler_ThrowError(hsmc,HalSMError_init(hsmc,hsmc->curInd,U8H("Wrong parameters in local function")));
                return out;
            }
            if (isSetArg==NOTHALSMNULLPOINTER) {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n)),HalSMVariableType_HalSMVar));
            } else {
                HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetArg_initWithValue(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n)),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,isSetArg)),HalSMVariableType_HalSMSetArg));
                isSetArg=NOTHALSMNULLPOINTER;
            }
            HalSMArray_clear(hsmc->memorymanagmentlibrary,n);
            continue;
        }
        HalSMArray_add(hsmc->memorymanagmentlibrary,n,HalSMVariable_init_char(hsmc->memorymanagmentlibrary,i));
    }
    if (n->size>0) {
        if (isSetArg==NOTHALSMNULLPOINTER) {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVar_init(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n)),HalSMVariableType_HalSMVar));
        } else {
            HalSMArray_add(hsmc->memorymanagmentlibrary,out,HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMSetArg_initWithValue(hsmc->memorymanagmentlibrary,HalSMVariable_init_str(hsmc->memorymanagmentlibrary,HalSMArray_chars_to_str(hsmc->memorymanagmentlibrary,n)),HalSMVariable_init_str(hsmc->memorymanagmentlibrary,isSetArg)),HalSMVariableType_HalSMSetArg));
            isSetArg=NOTHALSMNULLPOINTER;
        }
        HalSMArray_clear(hsmc->memorymanagmentlibrary,n);
    }
    return out;
}

HalSMLocalFunction* HalSMLocalFunction_init(HalSMCompiler* hsmc,HalStringFormatChar* name,HalStringFormatChar* args,HalSMVariable* module)
{
    HalSMLocalFunction* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMLocalFunction));
    out->hsmc=hsmc;
    out->name=name;
    out->id=hsmc->countLocalFunctions++;
    out->args=HalSMLocalFunction_getArgs(hsmc,args);
    out->func=HalSMArray_init(hsmc->memorymanagmentlibrary);
    out->vars=DictInit(hsmc->memorymanagmentlibrary);
    out->module=module;
    return out;
}

long long int StringIndexOf(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f) {
    unsigned long long int fl=string_len(f);
    unsigned long long int cl=string_len(c);
    if (fl==0||cl==0||cl<fl){return -1;}
    HalStringFormatChar* fa=hsmmml->malloc((fl+1)*sizeof(HalStringFormatChar));
    fa[fl]=ASCIIH('\0');
    unsigned long long int d=cl-fl+1;
    for (unsigned long long int i=0;i<d;i++) {
        memory_cpy(fa,&c[i],fl*sizeof(HalStringFormatChar));
        if (HSFCompare(fa,f)){return i;}
    }
    return -1;
}

long long int StringLastIndexOf(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f) {
    unsigned long long int fl=string_len(f);
    unsigned long long int cl=string_len(c);
    if (fl==0||cl==0||fl>=cl){return -1;}
    long long int out=-1;
    HalStringFormatChar* fa=hsmmml->malloc((fl+1)*sizeof(HalStringFormatChar));
    fa[fl]=ASCIIH('\0');
    unsigned long long int d=cl-fl+1;
    for (unsigned long long int i=0;i<d;i++) {
        memory_cpy(fa,&c[i],fl*sizeof(HalStringFormatChar));
        if (HSFCompare(fa,f)){out=i;}
    }
    return out;
}

unsigned char StringEndsWith(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f)
{
    if (string_len(c)==0||string_len(f)==0) {
        return 0;
    } else if (string_len(c)==string_len(f)) {
        return HSFCompare(c,f);
    } else if (string_len(c)<string_len(f)) {
        return 0;
    }
    return HSFCompare(SubString(hsmmml,c,string_len(c)-string_len(f),string_len(c)),f);
}

unsigned char StringStartsWith(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f)
{
    if (string_len(c)==0||string_len(f)==0) {
        return 0;
    } else if (string_len(c)==string_len(f)) {
        return HSFCompare(c,f);
    } else if (string_len(c)<string_len(f)) {
        return 0;
    }
    return HSFCompare(SubString(hsmmml,c,0,string_len(f)),f);
}

HalStringFormatChar* SubString(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,int start,int end) {
    long long int cl=string_len(c);
    if (start>=cl){
        //PRINT ERROR
        return U8H("");
    }
    else if (cl==0||end>cl){return U8H("");}
    if (end<0){end=cl+end;}
    if (start<0){start=cl+start;}
    if (end<0){return U8H("");}
    if (start<0){return U8H("");}
    unsigned long long int i,lo;
    lo=MathMin(end,cl)-start;
    HalStringFormatChar* out=hsmmml->malloc((lo+1)*sizeof(HalStringFormatChar));
    //memory_cpy(out,&c[start],lo*sizeof(HalStringFormatChar));
    HalStringFormatChar* cp=&c[start];
    HalStringFormatChar* outp=out;
    for (i=0;i<lo;i++) {
        *outp++=*cp++;
    }
    out[lo]=ASCIIH('\0');
    return out;
}

HalStringFormatChar* SubStringHalSMInteger(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalSMInteger* start,HalSMInteger* end) {
    HalSMInteger* cl=string_len_HalSMInteger(hsmmml,c);
    if (HalSMInteger_isMore(start,cl)||HalSMInteger_isEqual(start,cl)) {
        return U8H("");
    } else if (HalSMInteger_isEqual(cl,&HalSMIntegerZero)||HalSMInteger_isMore(end,cl)){return U8H("");}
    unsigned long long int lo=HalSMInteger_ToUnsignedLongLongInteger(HalSMInteger_Sub(hsmmml,HalSMInteger_isLess(end,cl)?end:cl,start));
    HalStringFormatChar* out=hsmmml->malloc((lo+1)*sizeof(HalStringFormatChar));
    memory_cpy(out,&c[HalSMInteger_ToUnsignedInteger(start)],lo*sizeof(HalStringFormatChar));
    out[lo]=ASCIIH('\0');
    return out;
}

HalStringFormatChar* ConcatenateStrings(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f) {
    if (string_len(c)==0&&string_len(f)==0){return U8H("");}
    HalStringFormatChar* out=hsmmml->malloc((string_len(c)+string_len(f)+1)*sizeof(HalStringFormatChar));
    string_cpy(out,c);
    string_cat(out,f);
    out[string_len(c)+string_len(f)]=ASCIIH('\0');
    return out;
}

HalStringFormatChar* StringReplace(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* c,HalStringFormatChar* f,HalStringFormatChar* r) {
    unsigned long long int lf=string_len(f);
    unsigned long long int lr=string_len(r);
    unsigned long long int cl=string_len(c);
    if(cl<lf||lf==0){return c;}
    else if(cl==0){return U8H("");}
    else if(c==f){return r;}
    HalStringFormatChar* out=hsmmml->malloc(0);
    unsigned long long int i=0;
    unsigned long long int size=0;
    unsigned long long int d=cl-lf+1;
    while(i<d){
        if(HSFCompare(SubString(hsmmml,c,i,i+lf),f)){
            if(lr>0){
                out=hsmmml->realloc(out,(size+lr)*sizeof(HalStringFormatChar));
                for (unsigned long long int j=0;j<lr;j++)
                    out[size+j]=r[j];
                size+=lr;
            }
            i+=lf;
        } else {
            out=hsmmml->realloc(out,(size+1)*sizeof(HalStringFormatChar));
            out[size]=c[i];
            size++;
            i++;
        }
    }
    out=hsmmml->realloc(out,(size+1)*sizeof(HalStringFormatChar));
    out[size]=ASCIIH('\0');
    return out;
}

HalSMCalculateVars HalSMCalculateVars_init()
{
    HalSMCalculateVars out;
    out.version=U8H("0.0.1");
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

HalStringFormatChar* HalSMCalculateVars_addStr(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    return ConcatenateStrings(hsmc->memorymanagmentlibrary,HalSMVariable_to_str(hsmc,v0,0),HalSMVariable_to_str(hsmc,v1,0));
}

HalStringFormatChar* HalSMCalculateVars_subStr(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    HalStringFormatChar* v0s=HalSMVariable_to_str(hsmc,v0,0);
    HalStringFormatChar* v1s=HalSMVariable_to_str(hsmc,v1,0);
    if (string_len(v0s)<string_len(v1s)){return v0s;};
    long long int v=StringLastIndexOf(hsmc->memorymanagmentlibrary,v0s,v1s);
    if (v==-1){return v0s;}
    return ConcatenateStrings(hsmc->memorymanagmentlibrary,SubString(hsmc->memorymanagmentlibrary,v0s,0,v),SubString(hsmc->memorymanagmentlibrary,v0s,v+string_len(v1s),string_len(v0s)));
}

HalStringFormatChar* HalSMCalculateVars_mulStr(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v0->type==HalSMVariableType_int) {
        HalSMVariable* temp=v0;
        v0=v1;
        v1=temp;
    }
    if (v1->type!=HalSMVariableType_int||v0->type!=HalSMVariableType_str){return U8H("");}
    HalStringFormatChar* st=*(HalStringFormatChar**)v0->value;
    HalSMInteger* ch=(HalSMInteger*)v1->value;
    if (HalSMInteger_isEqual(ch,&HalSMIntegerZero)){return U8H("");}
    else if (HalSMInteger_isEqual(ch,&HalSMIntegerOne)){return st;}
    unsigned long long int lst=string_len(st);
    unsigned long long int l=HalSMInteger_ToUnsignedLongLongInteger(ch)*lst,i;
    unsigned long long int size=lst*l;
    HalStringFormatChar* out=hsmmml->malloc((size+1)*sizeof(HalStringFormatChar));
    //out[size]=ASCIIH('\0');
    HalStringFormatChar* p=out;
    for (i=0;i<l;i++,p++) {
        *p=st[i%lst];
        //out[size]=ASCIIH('\0');
    }
    *p=ASCIIH('\0');
    return out;
}

HalStringFormatChar* HalSMCalculateVars_divStr(HalSMCompiler* hsmc,HalSMVariable* v0,HalSMVariable* v1)
{
    if (v1->type==HalSMVariableType_str){return StringReplace(hsmc->memorymanagmentlibrary,*(HalStringFormatChar**)v0->value,*(HalStringFormatChar**)v1->value,U8H(""));}
    HalStringFormatChar* s0=HalSMVariable_to_str(hsmc,v0,0);
    HalSMInteger* i1=(HalSMInteger*)v1->value;
    unsigned long long int ls0=string_len(s0);
    if(ls0==0||HalSMInteger_isEqual(i1,&HalSMIntegerZero)){return U8H("");}
    else if(HalSMInteger_isEqual(i1,&HalSMIntegerOne)){return s0;}
    HalSMInteger remainder;
    HalSMInteger* louti=HalSMInteger_Div(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,string_len(s0)),i1,&remainder);
    unsigned long long int lout=HalSMInteger_ToUnsignedLongLongInteger(louti);
    HalStringFormatChar* out=hsmc->memorymanagmentlibrary->malloc((lout+1)*sizeof(HalStringFormatChar));
    for (unsigned long long int i=0;i<lout;i++) {
        out[i]=s0[i];
    }
    out[lout]=ASCIIH('\0');
    return out;
}

HalSMInteger* HalSMCalculateVars_addInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMInteger* i0;
    if (v0->type==HalSMVariableType_str) {
        i0=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else {
        i0=(HalSMInteger*)v0->value;
    }

    HalSMInteger* i1;
    if (v1->type==HalSMVariableType_str) {
        i1=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else {
        i1=(HalSMInteger*)v1->value;
    }
    return HalSMInteger_Add(hsmmml,i0,i1);
}

HalSMInteger* HalSMCalculateVars_subInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMInteger* i0;
    if (v0->type==HalSMVariableType_str) {
        i0=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else {
        i0=(HalSMInteger*)v0->value;
    }

    HalSMInteger* i1;
    if (v1->type==HalSMVariableType_str) {
        i1=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else {
        i1=(HalSMInteger*)v1->value;
    }
    return HalSMInteger_Sub(hsmmml,i0,i1);
}

HalSMInteger* HalSMCalculateVars_mulInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMInteger* i0;
    if (v0->type==HalSMVariableType_str) {
        i0=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else {
        i0=(HalSMInteger*)v0->value;
    }

    HalSMInteger* i1;
    if (v1->type==HalSMVariableType_str) {
        i1=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else {
        i1=(HalSMInteger*)v1->value;
    }
    return HalSMInteger_Mul(hsmmml,i0,i1);
}

HalSMInteger* HalSMCalculateVars_divInt(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMInteger* i0;
    if (v0->type==HalSMVariableType_str) {
        i0=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else {
        i0=(HalSMInteger*)v0->value;
    }

    HalSMInteger* i1;
    if (v1->type==HalSMVariableType_str) {
        i1=HalSMInteger_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else {
        i1=(HalSMInteger*)v1->value;
    }
    HalSMInteger remainder;
    return HalSMInteger_Div(hsmmml,i0,i1,&remainder);
}

HalSMDouble* HalSMCalculateVars_addDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMDouble* f0;
    if (v0->type==HalSMVariableType_str) {
        f0=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else if (v0->type==HalSMVariableType_int) {
        f0=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v0->value);
    } else {
        f0=(HalSMDouble*)v0->value;
    }

    HalSMDouble* f1;
    if (v1->type==HalSMVariableType_str) {
        f1=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else if (v1->type==HalSMVariableType_int) {
        f1=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v1->value);
    } else {
        f1=(HalSMDouble*)v1->value;
    }
    return HalSMDouble_Add(hsmmml,f0,f1);
}

HalSMDouble* HalSMCalculateVars_subDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMDouble* f0;
    if (v0->type==HalSMVariableType_str) {
        f0=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else if (v0->type==HalSMVariableType_int) {
        f0=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v0->value);
    } else {
        f0=(HalSMDouble*)v0->value;
    }

    HalSMDouble* f1;
    if (v1->type==HalSMVariableType_str) {
        f1=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else if (v1->type==HalSMVariableType_int) {
        f1=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v1->value);
    } else {
        f1=(HalSMDouble*)v1->value;
    }
    return HalSMDouble_Sub(hsmmml,f0,f1);
}
HalSMDouble* HalSMCalculateVars_mulDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMDouble* f0;
    if (v0->type==HalSMVariableType_str) {
        f0=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else if (v0->type==HalSMVariableType_int) {
        f0=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v0->value);
    } else {
        f0=(HalSMDouble*)v0->value;
    }

    HalSMDouble* f1;
    if (v1->type==HalSMVariableType_str) {
        f1=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else if (v1->type==HalSMVariableType_int) {
        f1=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v1->value);
    } else {
        f1=(HalSMDouble*)v1->value;
    }
    return HalSMDouble_Mul(hsmmml,f0,f1);
}
HalSMDouble* HalSMCalculateVars_divDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMVariable* v0,HalSMVariable* v1)
{
    HalSMDouble* f0;
    if (v0->type==HalSMVariableType_str) {
        f0=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v0->value);
    } else if (v0->type==HalSMVariableType_int) {
        f0=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v0->value);
    } else {
        f0=(HalSMDouble*)v0->value;
    }

    HalSMDouble* f1;
    if (v1->type==HalSMVariableType_str) {
        f1=HalSMDouble_parse(hsmmml,*(HalStringFormatChar**)v1->value);
    } else if (v1->type==HalSMVariableType_int) {
        f1=HalSMDouble_FromHalSMInteger(hsmmml,(HalSMInteger*)v1->value);
    } else {
        f1=(HalSMDouble*)v1->value;
    }
    return HalSMDouble_Div(hsmmml,f0,f1);
}

HalSMFunctionArray* HalSMFunctionArray_init(HalSMMemoryManagmentLibrary* hsmmml,HalSMArray* args,HalSMFunctionArrayType type)
{
    HalSMFunctionArray* out=hsmmml->malloc(sizeof(HalSMFunctionArray));
    out->args=args;
    out->type=type;
    return out;
}

//GOTO

HalSMGOTO* HalSMGOTO_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* sector)
{
    HalSMGOTO* out=hsmmml->malloc(sizeof(HalSMGOTO));
    out->sector=sector;
    return out;
}

HalSMGOTOSector* HalSMGOTOSector_init(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* sector,unsigned long long int index,HalSMLocalFunction* lf)
{
    HalSMGOTOSector* out=hsmmml->malloc(sizeof(HalSMGOTOSector));
    out->sector=sector;
    out->index=index;
    out->lf=lf;
    return out;
}

//String

unsigned long long int string_len(HalStringFormatChar* s)
{
    unsigned long long int out=0;
    while (HSFCharCompare(*s++,ASCIIH('\0'))==0) {
        out++;
    }
    return out;
}

HalSMInteger* string_len_HalSMInteger(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* s)
{
    HalSMInteger* out=HalSMInteger_init(hsmmml,0,hsmmml->malloc(0),0);
    while (HSFCharCompare(*s,ASCIIH('\0'))==0) {
        HalSMInteger_AddSelf(hsmmml,out,&HalSMIntegerOne);
        s++;
    }
    return out;
}

HalStringFormatChar* string_cat(HalStringFormatChar* s1,HalStringFormatChar* s2)
{
    if ((s1==NOTHALSMNULLPOINTER)&&(s2==NOTHALSMNULLPOINTER))return NOTHALSMNULLPOINTER;
    HalStringFormatChar* ptr=s1;
    while (HSFCharCompare(*ptr,ASCIIH('\0'))==0) ptr++;
    while (HSFCharCompare(*s2,ASCIIH('\0'))==0) *ptr++=*s2++;
    *ptr=ASCIIH('\0');
    return s1;
}

HalStringFormatChar* string_cpy(HalStringFormatChar* s1,HalStringFormatChar* s2)
{
    if ((s1==NOTHALSMNULLPOINTER)&&(s2==NOTHALSMNULLPOINTER))return NOTHALSMNULLPOINTER;
    
    HalStringFormatChar* ptr=s1;
    while (HSFCharCompare(*s2,ASCIIH('\0'))==0)
    {
        *s1=*s2;
        s1++;
        s2++;
    }
    *s1=ASCIIH('\0');
    return ptr;
}

HalStringFormatChar* stringncpy(HalStringFormatChar* s1,HalStringFormatChar* s2,unsigned long long int n)
{
    if ((s1==NOTHALSMNULLPOINTER)&&(s2==NOTHALSMNULLPOINTER))return NOTHALSMNULLPOINTER;
    HalStringFormatChar* ptr=s1;

    while (HSFCharCompare(*s2,ASCIIH('\0'))==0&&n--)
    {
        *s1=*s2;
        s1++;
        s2++;
    }

    *s1=ASCIIH('\0');
    return ptr;
}

void* memory_cpy(void* dst,const void* src,unsigned long long int n)
{
    char* bdst=(char*)dst;
    char* bsrc=(char*)src;
    if ((bdst!=NOTHALSMNULLPOINTER)&&(bsrc!=NOTHALSMNULLPOINTER)) {
        while (n)
        {
            *(bdst++)=*(bsrc++);
            --n;
        }
    }
    return dst;
}

//HalSMInteger

HalSMInteger* HalSMInteger_init(HalSMMemoryManagmentLibrary* hsmmml,unsigned char negative,unsigned char* value,unsigned long long size)
{
    HalSMInteger* out=hsmmml->malloc(sizeof(HalSMInteger));
    out->negative=negative;
    out->value=value;
    out->size=size;
    return out;
}

HalSMInteger* HalSMInteger_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    HalSMInteger* out=hsmmml->malloc(sizeof(HalSMInteger));
    out->negative=a->negative;
    out->value=hsmmml->malloc(a->size);
    memory_cpy(out->value,a->value,a->size);
    out->size=a->size;
    return out;
}

HalSMInteger* HalSMInteger_FromUnsignedInteger(HalSMMemoryManagmentLibrary* hsmmml,unsigned int value)
{
    unsigned char* v=hsmmml->malloc(4);
    v[0]=(value>>24)&0xFF;
    v[1]=(value>>16)&0xFF;
    v[2]=(value>>8)&0xFF;
    v[3]=value&0xFF;
    return HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v,4));
}

HalSMInteger* HalSMInteger_FromSignedInteger(HalSMMemoryManagmentLibrary* hsmmml,signed int value)
{
    unsigned char* v=hsmmml->malloc(4);
    unsigned char negate=value<0?1:0;
    if (negate) {
        value=-value;
    }
    v[0]=(value>>24)&0xFF;
    v[1]=(value>>16)&0xFF;
    v[2]=(value>>8)&0xFF;
    v[3]=value&0xFF;
    return HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,negate,v,4));
}

HalSMInteger* HalSMInteger_FromUnsignedLongLongInteger(HalSMMemoryManagmentLibrary* hsmmml,unsigned long long int value)
{
    unsigned char* v=hsmmml->malloc(8);
    v[0]=(value>>56)&0xFF;
    v[1]=(value>>48)&0xFF;
    v[2]=(value>>40)&0xFF;
    v[3]=(value>>32)&0xFF;
    v[4]=(value>>24)&0xFF;
    v[5]=(value>>16)&0xFF;
    v[6]=(value>>8)&0xFF;
    v[7]=value&0xFF;
    return HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v,8));
}

unsigned int HalSMInteger_ToUnsignedInteger(HalSMInteger* value)
{
    unsigned char out[4]={0,0,0,0};
    unsigned long long int l=value->size>4?4:value->size;
    for (unsigned long long int i=0;i<value->size;i++) {
        out[value->size-1-i]=value->value[i];
    }
    return *(unsigned int*)out;
}

unsigned long long int HalSMInteger_ToUnsignedLongLongInteger(HalSMInteger* value)
{
    unsigned char out[8]={0,0,0,0};
    unsigned long long int l=value->size>8?8:value->size;
    for (unsigned long long int i=0;i<value->size;i++) {
        out[value->size-1-i]=value->value[i];
    }
    return *(unsigned long long int*)out;
}

HalSMInteger* HalSMInteger_parse(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str)
{
    HalSMInteger* out=HalSMInteger_init(hsmmml,0,hsmmml->malloc(0),0);
    unsigned char neg = HSFCharCompare(str[0],ASCIIH('-'));
    long long int i = neg ? 1 : 0;
    HalSMInteger t={0,(unsigned char[]){0},1};
    while (str[i].symbol>='0'&&str[i].symbol<='9')
    {
        HalSMInteger_MulSelf(hsmmml,out,&HalSMIntegerTen);
        t.value[0]=str[i].symbol-'0';
        HalSMInteger_AddSelf(hsmmml,out,&t);  
        i++;
    }
    if ( neg ) {
        out->negative=1;
    }
    return HalSMInteger_clear(hsmmml,out);
}

HalSMInteger* HalSMInteger_parseHex(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str,unsigned char isZeroX)
{
    unsigned long long int l=string_len(str);
    if (l<3){return &HalSMIntegerZero;}
    unsigned char neg=HSFCharCompare(str[0],ASCIIH('-'));
    unsigned long long int i;
    unsigned char v=0;
    HalStringFormatChar* p=str;
    if (neg){p++;l--;}
    if (isZeroX&&(HSFCharCompare(p[0],ASCIIH('0'))==0||HSFCharCompare(p[1],ASCIIH('x'))==0)){return &HalSMIntegerZero;}
    if (isZeroX){p+=2;l-=2;}
    HalSMInteger* out=HalSMInteger_init(hsmmml,0,hsmmml->malloc((l+1)/2),(l+1)/2);
    i=0;
    if (l%2==1) {
        if (p->symbol>=65&&p->symbol<=70) {
            v=p->symbol-65+10;
        } else if (p->symbol>='0'&&p->symbol<='9') {
            v=p->symbol-'0';
        }
        out->value[i/2]=v;
        p++;
        i=2;
        l++;
    }
    for (;i<l;i++,p++) {
        if (p->symbol>=65&&p->symbol<=70) {
            v=p->symbol-65+10;
        } else if (p->symbol>='0'&&p->symbol<='9') {
            v=p->symbol-'0';
        }
        if (i%2==1) {
            out->value[i/2]|=v;
        } else {
            out->value[i/2]=v<<4;
        }
    }
    return HalSMInteger_clear(hsmmml,out);
}

HalSMInteger* HalSMInteger_parseBin(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str)
{
    unsigned long long int l=string_len(str);
    if (l<3){return &HalSMIntegerZero;}
    unsigned char neg=HSFCharCompare(str[0],ASCIIH('-'));
    unsigned long long int i;
    unsigned char v=0;
    HalStringFormatChar* p=str;
    if (neg){p++;l--;}
    if (HSFCharCompare(p[0],ASCIIH('0'))==0||HSFCharCompare(p[1],ASCIIH('b'))==0){return &HalSMIntegerZero;}
    p+=2;
    l-=2;
    HalSMInteger* out=HalSMInteger_init(hsmmml,0,hsmmml->calloc((l+7)/8,sizeof(unsigned char)),(l+7)/8);
    i=0;
    unsigned long long int ind=0;
    if (l%8>0) {
        for (;i<l%8;i++,p++) {
            v=p->symbol-'0';
            out->value[0]|=v<<((l%8)-1-i);
        }
        //l++;
        l=l-i;
        i=0;
        ind=1;
    }
    for (;i<l;i++,p++) {
        v=p->symbol-'0';
        out->value[ind+i/8]|=v<<(7-(i%8));
    }
    return HalSMInteger_clear(hsmmml,out);
}

void HalSMInteger_AddSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    if (a->negative&&b->negative==0) {
        a->negative=0;
        HalSMInteger_SubSelf(hsmmml,a,b);
        if (a->negative) {
            a->negative=0;
        } else {
            a->negative=1;
        }
        HalSMInteger_clear(hsmmml,a);
        return;
    } else if (b->negative&&a->negative==0) {
        b->negative=0;
        HalSMInteger_SubSelf(hsmmml,a,b);
        b->negative=1;
        HalSMInteger_clear(hsmmml,a);
        return;
    }
    unsigned long long int i,j,c;
    unsigned short tempr;
    unsigned char tempn=0,tempb;
    unsigned char* temps;
    unsigned long long int cm=a->size;
    if (a->size<b->size) {
        a->value=hsmmml->realloc(a->value,b->size);
        for (i=a->size-1,j=b->size-1;;i--,j--) {
            a->value[j]=a->value[i];
            //a->value[i]=0;
            if (i==0){break;}
        }
        
        j=b->size-a->size;
        for (i=0;i<j;i++) {
            a->value[i]=0;
        }
        a->size=b->size;
        cm=b->size;
    }
    for (i=a->size-1,j=b->size,c=0;c<cm;i--,c++) {
        if (j==0) {
            tempb=0;
        } else {
            j--;
            tempb=b->value[j];
        }
        tempr=a->value[i]+tempb+tempn;
        temps=(unsigned char*)&tempr;
        a->value[i]=temps[0];
        tempn=temps[1];
    }

    if (tempn!=0) {
        a->value=hsmmml->realloc(a->value,a->size+1);
        for (i=a->size-1;;i--) {
            a->value[i+1]=a->value[i];
            if (i==0) {break;}
        }
        a->value[0]=tempn;
        a->size=a->size+1;
    }
    HalSMInteger_clear(hsmmml,a);
}

HalSMInteger* HalSMInteger_Add(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    unsigned char* temp=hsmmml->malloc(a->size);
    memory_cpy(temp,a->value,a->size);
    HalSMInteger* out=HalSMInteger_init(hsmmml,a->negative,temp,a->size);
    HalSMInteger_AddSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_SubSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    if (HalSMInteger_isEqual(a,b)) {
        hsmmml->free(a->value);
        a->value=hsmmml->calloc(1,sizeof(unsigned char));
        a->size=1;
        a->negative=0;
        return;
    } else if (a->negative==1) {
        HalSMInteger* tempa=HalSMInteger_init(hsmmml,0,a->value,a->size);
        HalSMInteger* tempb=HalSMInteger_init(hsmmml,0,b->value,b->size);
        if (b->negative==1) {
            if (HalSMInteger_isLess(tempa,tempb)) {
                tempa=HalSMInteger_Sub(hsmmml,tempb,tempa);
                hsmmml->free(a->value);
                a->value=tempa->value;
                a->size=tempa->size;
                a->negative=tempa->negative;
                return;
            } else if (HalSMInteger_isEqual(tempa,tempb)) {
                hsmmml->free(a->value);
                a->value=hsmmml->calloc(1,sizeof(unsigned char));
                a->size=1;
                a->negative=0;
                return;
            }
            tempa=HalSMInteger_Sub(hsmmml,tempa,tempb);
            hsmmml->free(a->value);
            a->value=tempa->value;
            a->size=tempa->size;
            a->negative=1;
            return;
        }
        HalSMInteger* out=HalSMInteger_Add(hsmmml,tempa,tempb);
        hsmmml->free(a->value);
        a->value=out->value;
        a->size=out->size;
        a->negative=1;
        return;
    } else if (b->negative==1) {
        HalSMInteger_AddSelf(hsmmml,a,HalSMInteger_init(hsmmml,0,b->value,b->size));
        return;
    } else if (HalSMInteger_isLess(a,b)) {
        HalSMInteger* out=HalSMInteger_Sub(hsmmml,b,a);
        hsmmml->free(a->value);
        a->value=out->value;
        a->size=out->size;
        a->negative=1;
        return;
    }

    unsigned long long int i,j,c;
    short tempr;
    unsigned char tempn=0,tempb;
    unsigned char* temps;
    unsigned long long int cm=a->size;
    if (a->size<b->size) {
        a->value=hsmmml->realloc(a->value,b->size);
        for (i=a->size-1,j=b->size-1;;i--,j--) {
            a->value[j]=a->value[i];
            //a->value[i]=0;
            if (i==0){break;}
        }
        j=b->size-a->size;
        for (i=0;i<j;i++) {
            a->value[i]=0;
        }
        a->size=b->size;
        cm=b->size;
    }
    for (i=a->size-1,j=b->size,c=0;c<cm;i--,c++) {
        if (j==0) {
            tempb=0;
        } else {
            j--;
            tempb=b->value[j];
        }
        tempr=a->value[i]-tempb-tempn;
        if (tempr<0){tempn=1;tempr=(short)(tempr+0x100);}
        else{tempn=0;}
        a->value[i]=(unsigned char)tempr;
    }

    if (tempn!=0) {
        a->value=hsmmml->realloc(a->value,a->size+1);
        for (i=a->size-1;;i--) {
            a->value[i+1]=a->value[i];
            if (i==0) {break;}
        }
        a->value[0]=tempn;
        a->size=a->size+1;
    }
    HalSMInteger_clear(hsmmml,a);
}

HalSMInteger* HalSMInteger_Sub(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    unsigned char* temp=hsmmml->malloc(a->size);
    memory_cpy(temp,a->value,a->size);
    HalSMInteger* out=HalSMInteger_init(hsmmml,a->negative,temp,a->size);
    HalSMInteger_SubSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_MulSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{   
    if (a->negative==1&&b->negative==1) {a->negative=0;}
    else if (b->negative==1) {a->negative=1;}
    unsigned int* product=hsmmml->calloc(a->size+b->size,sizeof(unsigned int));
    long long int b_i,a_i,k,g;
    int carry,base;
    base=256;
    k=0;
    long long int p=a->size;
    for (b_i=b->size;b_i>0;b_i--) {
        carry=0;
        for (a_i=a->size;a_i>0;a_i--) {
            k=a_i+b_i-1;
            product[k]+=carry+a->value[a_i-1]*b->value[b_i-1];
            carry=product[k]/base;
            product[k]=product[k]%base;
        }
        product[b_i-1]+=carry;
    }
    a->size+=b->size;
    a->value=hsmmml->realloc(a->value,a->size);
    for (unsigned long long int i=0;i<a->size;i++) {
        a->value[i]=product[i];
    }
    hsmmml->free(product);
    HalSMInteger_clear(hsmmml,a);
}

HalSMInteger* HalSMInteger_Mul(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_MulSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_DivSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b,HalSMInteger* remainder)
{
    unsigned char isBNegative=b->negative;
    unsigned char isANegative=a->negative;
    b->negative=0;
    a->negative=0;
    long long int i,bit;
    unsigned char* quotient;
    HalSMInteger* R=HalSMInteger_init(hsmmml,0,hsmmml->calloc(1,1),1);
    HalSMInteger* Q=HalSMInteger_init(hsmmml,0,hsmmml->calloc(1,1),1);

    unsigned long long int loops=0;
    for (i=a->size*8-1;;i--) {
        HalSMInteger_shiftLeftSelf(hsmmml,R,1);
        HalSMInteger_setBitSelf(hsmmml,R,0,0,HalSMInteger_getBit(a,(i-(i%8))/8,i%8));
        if (HalSMInteger_isLess(R,b)==0) {
            HalSMInteger_SubSelf(hsmmml,R,b);
            HalSMInteger_setBitSelf(hsmmml,Q,(i-(i%8))/8,i%8,1);
        }
        if (i==0) {
            break;
        }
    }

    if (remainder==NOTHALSMNULLPOINTER){hsmmml->free(a->value);}
    else {
        remainder->negative=R->negative;
        remainder->size=R->size;
        remainder->value=R->value;
        HalSMInteger_clear(hsmmml,remainder);
    }
    a->size=Q->size;
    a->value=Q->value;
    if ((isANegative==0&&isBNegative)||(isBNegative==0&&isANegative)) {
        a->negative=1;
    }
    HalSMInteger_clear(hsmmml,a);
}

HalSMInteger* HalSMInteger_Div(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b,HalSMInteger* remainder)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_DivSelf(hsmmml,out,b,remainder);
    return out;
}

HalSMInteger* HalSMInteger_negate(HalSMInteger* a)
{
    a->negative=a->negative==1?0:1;
    return a;
}

HalSMInteger* HalSMInteger_absolute(HalSMInteger* a)
{
    a->negative=0;
    return a;
}

unsigned char HalSMInteger_isMore(HalSMInteger* a,HalSMInteger* b)
{
    if (a->size<b->size||(a->size==1&&b->size==1&&a->value[0]==b->value[0])||(a->negative==1&&b->negative==0)) {return 0;}
    else if (a->size>b->size||(a->negative==0&&b->negative==1)){return 1;}
    unsigned long long i;
    if (a->negative==1) {
        for (i=0;i<a->size;i++) {
            if (a->value[i]>b->value[i]) {return 0;}
            else if (a->value[i]<b->value[i]) {return 1;}
        }
    } else {
        for (i=0;i<a->size;i++) {
            if (a->value[i]<b->value[i]) {return 0;}
            else if (a->value[i]>b->value[i]) {return 1;}
        }
    }
    return 0;
}

unsigned char HalSMInteger_isLess(HalSMInteger* a,HalSMInteger* b)
{
    if (a->size>b->size||(a->size==1&&b->size==1&&a->value[0]==b->value[0])||(a->negative==0&&b->negative==1)) {return 0;}
    else if (a->size<b->size||(a->negative==1&&b->negative==0)){return 1;}
    unsigned long long i;
    if (a->negative==1) {
        for (i=0;i<a->size;i++) {
            if (a->value[i]<b->value[i]) {return 0;}
            else if (a->value[i]>b->value[i]) {return 1;}
        }
    } else {
        for (i=0;i<a->size;i++) {
            if (a->value[i]>b->value[i]) {return 0;}
            else if (a->value[i]<b->value[i]) {return 1;}
        }
    }
    return 0;
}

unsigned char HalSMInteger_isEqual(HalSMInteger* a,HalSMInteger* b)
{
    if (a->size!=b->size||a->negative!=b->negative) {return 0;}
    unsigned long long i;
    for (i=0;i<a->size;i++) {
        if (a->value[i]!=b->value[i]) {return 0;}
    }
    return 1;
}

HalSMInteger* HalSMInteger_getValueWithoutNull(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    if (a->size==0) {return a;}
    else if (a->size==1) {return a;}
    
    unsigned long long int i,cn;
    cn=0;
    for (i=0;i<a->size;i++) {
        if (a->value[i]==0)cn++;
        else break;
    }
    if (cn==0)return a;
    if (a->size-cn==0) {
        a->size=1;
    } else {
        a->size=a->size-cn;
    }
    memory_cpy(a->value,&a->value[cn],a->size);
    a->value=hsmmml->realloc(a->value,a->size);
    return a;
}

char* HalSMInteger_Byte2Bits(HalSMMemoryManagmentLibrary* hsmmml,unsigned char byte)
{
    char* out=hsmmml->malloc(9);
    out[0]=(byte>>7&1)+'0';
    out[1]=((byte>>6)&1)+'0';
    out[2]=((byte>>5)&1)+'0';
    out[3]=((byte>>4)&1)+'0';
    out[4]=((byte>>3)&1)+'0';
    out[5]=((byte>>2)&1)+'0';
    out[6]=((byte>>1)&1)+'0';
    out[7]=(byte&1)+'0';
    out[8]='\0';
    return out;
}

HalStringFormatChar* HalSMInteger_Bytes2Bits(HalSMMemoryManagmentLibrary* hsmmml,unsigned char* bytes,unsigned long long size)
{
    if (size==0) {return U8H("");}
    else if (size==1) {return U8H(HalSMInteger_Byte2Bits(hsmmml,bytes[0]));}
    HalStringFormatChar* out=hsmmml->malloc((size*8+1)*sizeof(HalStringFormatChar));
    string_cpy(out,U8H(HalSMInteger_Byte2Bits(hsmmml,bytes[0])));
    for (unsigned long long i=1;i<size;i++) {
        string_cat(out,U8H(HalSMInteger_Byte2Bits(hsmmml,bytes[i])));
    }
    out[size*8].alphabet=0;
    out[size*8].symbol=0;
    out[size*8].parameters=0;
    return out;
}

HalStringFormatChar* SumStringValues(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* s1,HalStringFormatChar* s2)
{
    char* result=hsmmml->malloc(0);
    unsigned long long resultl=0;
    HalStringFormatChar* str1=s1,*str2=s2;
    if (string_len(str1)>string_len(str2)) {
        HalStringFormatChar* temp=str1;
        str1=str2;
        str2=temp;
    }

    while (string_len(str1)<string_len(str2)) {
        str1=ConcatenateStrings(hsmmml,U8H("0"),str1);
    }
    unsigned long long len=string_len(str1);
    unsigned char carry=0;
    while (len)
    {
        len--;
        long long int d1=str1[len].symbol-'0';
        long long int d2=str2[len].symbol-'0';

        long long int sum=d1+d2+carry;
        carry=sum>9;
        if (carry)
        {
            sum-=10;
        }
        result=hsmmml->realloc(result,++resultl);
        result[resultl-1]='0'+sum;
    }

    if (carry)
    {
        result=hsmmml->realloc(result,++resultl);
        result[resultl-1]='1';
    }

    result=hsmmml->realloc(result,resultl+1);
    result[resultl]='\0';

    unsigned long long i;
    char* out=hsmmml->malloc(resultl+1);
    for (i=0;i<resultl;i++) {
        out[resultl-1-i]=result[i];
    }
    out[resultl]='\0';
    return U8H(out);
}

HalStringFormatChar* ShiftLeftString(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* s,unsigned long long N)
{
    HalStringFormatChar* result=s;
    while (N>0) {
        result=SumStringValues(hsmmml,result,result);
        N--;
    }
    return result;
}

HalStringFormatChar* byteToString(HalSMMemoryManagmentLibrary* hsmmml,unsigned char byte) {
    char* out;
    if (byte>99) {
        out=hsmmml->calloc(4,sizeof(char));
        out[0]='0'+(byte/100);
        out[1]='0'+((byte%100)/10);
        out[2]='0'+(byte%10);
        return U8H(out);
    } else if (byte>9) {
        out=hsmmml->calloc(3,sizeof(char));
        out[0]='0'+(byte/10);
        out[1]='0'+(byte%10);
        return U8H(out);
    }
    out=hsmmml->calloc(2,sizeof(char));
    out[0]='0'+byte;
    return U8H(out);
}

unsigned char hi_div10(unsigned char* hex,unsigned long long size) {
    unsigned char rem=0;
    unsigned char n;
    unsigned long long i;
    for (i=size-1;;i--) {
        n=rem*256+hex[i];
        hex[i]=n/10;
        rem=n%10;
        if (i==0)break;
    }
    return rem;
}

unsigned char hi_isNotNull(unsigned char* hex,unsigned long long size) {
    unsigned long long i;
    for (i=0;i<size;i++) {
        if (hex[i]>0)return 1;
    }
    return 0;
}

HalStringFormatChar* HalSMInteger_toString(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,unsigned char isHex)
{
    if (isHex==1) {
        HalStringFormatChar* out=hsmmml->malloc((2*a->size+1)*sizeof(HalStringFormatChar));
        unsigned long long i,l;
        for (i=0;i<a->size;i++) {
            HalSMInteger_Byte2Hex(&out[i*2],a->value[i]);
        }
        out[2*a->size]=ASCIIH('\0');
        return out;
    }

    HalStringFormatChar* hex=HalSMInteger_toString(hsmmml,a,1);
    unsigned long long int i=0,l,d,decl=1;
    char c;
    unsigned char* dec=hsmmml->calloc(1,sizeof(HalStringFormatChar));
    l=string_len(hex);
    if (l>0&&HSFCharCompare(hex[0],ASCIIH('0'))) {
        i=1;
    }
    
    //Make check ascii
    while (i<l) {
        c=UTF8HalStringFormatCharacters[HalUTF8_indexByHalStringFormatChar(hex[i])].utf8c;
        long long int carry=(c<='9'?c-'0':c-'a'+10);
        for (d=0;d<decl;d++) {
            long long int val=dec[d]*16+carry;
            dec[d]=val%10;
            carry=val/10;
        }

        while (carry>0) {
            dec=hsmmml->realloc(dec,(++decl)*sizeof(HalStringFormatChar));
            dec[decl-1]=carry%10;
            carry/=10;
        }
        i++;
    }

    HalStringFormatChar* out;
    HalStringFormatChar* outp;
    if (a->negative&&(decl>1||dec[0]>0)) {
        out=hsmmml->malloc((decl+2)*sizeof(HalStringFormatChar));
        out[0]=ASCIIH('-');
        out[decl+1]=ASCIIH('\0');
        outp=&out[decl];
    } else {
        out=hsmmml->malloc((decl+1)*sizeof(HalStringFormatChar));
        out[decl]=ASCIIH('\0');
        outp=&out[decl-1];
    }
    for (i=0;i<decl;i++,outp--) {
        *outp=ASCIIH('0'+dec[i]);
    }
    return out;
}

HalStringFormatChar* HalSMInteger_Byte2Hex(HalStringFormatChar* out,unsigned char x)
{
    unsigned char a=(x>>4)&0xF;
    unsigned char b=x&0xF;
    out[0]=U8CH(a>=10?'a'+a-10:'0'+a);
    out[1]=U8CH(b>=10?'a'+b-10:'0'+b);
    return out;
}

HalStringFormatChar* HalSMInteger_toStringBytes(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    unsigned long long i,curInd;
    HalStringFormatChar* out=hsmmml->malloc(3*a->size+1);
    out[3*a->size]=ASCIIH('\0');
    HalStringFormatChar* temp=hsmmml->calloc(2,sizeof(char));
    for (i=0;i<a->size;i++)
    {
        curInd=i*3;
        out[curInd]=ASCIIH('\\');
        temp=HalSMInteger_Byte2Hex(temp,a->value[i]);
        if (HSFCharCompare(temp[1],ASCIIH('\0'))) {
            out[curInd+1]=ASCIIH('0');
            out[curInd+2]=temp[0];
        } else {
            out[curInd+1]=temp[0];
            out[curInd+2]=temp[1];
        }
        temp[0]=ASCIIH('\0');
        temp[1]=ASCIIH('\0');
    }
    return out;
}

unsigned char HalSMInteger_getBit(HalSMInteger* a,long long int byte,unsigned char bit) {
    if (byte>=a->size){return 0;}
    return (a->value[a->size-1-byte]>>bit)&1;
}

void HalSMInteger_setBitSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int byte,unsigned char bit,unsigned char value) {
    if (byte>=a->size){
        a->value=hsmmml->realloc(a->value,byte+1);
        unsigned long long int i,j;
        for (i=a->size-1,j=byte;;i--,j--) {
            a->value[j]=a->value[i];
            if (i==0){break;}
        }
        j=byte+1-a->size;
        for (i=0;i<j;i++) {
            a->value[i]=0;
        }
        a->size=byte+1;
    }
    if (value==0) {
        a->value[a->size-1-byte]&=~(1<<bit);
        return;
    }
    a->value[a->size-1-byte]|=(1<<bit);
}

HalSMInteger* HalSMInteger_setBit(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int byte,unsigned char bit,unsigned char value)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_setBitSelf(hsmmml,out,byte,bit,value);
    return out;
}

unsigned char reverseBits(unsigned char n)
{
    return (65280>>n)&0xff;
}

void HalSMInteger_shiftRightSelf(HalSMInteger* a,long long int bit) {
    if (bit==0)return;
    long long int i,b;
    unsigned char carry,savedCarry;
    for (b=0;b<bit;b++)
    {
        carry=0;
        for (i=0;i<a->size;i++)
        {
            savedCarry=(a->value[i]&1)?0x80:0;
            a->value[i]=carry|(a->value[i]>>1);
            carry=savedCarry;
        }
    }
}

HalSMInteger* HalSMInteger_shiftRight(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int bit) {
    unsigned char* temp=hsmmml->malloc(a->size);
    memory_cpy(temp,a->value,a->size);
    HalSMInteger* out=HalSMInteger_init(hsmmml,a->negative,temp,a->size);
    HalSMInteger_shiftRightSelf(out,bit);
    return out;
}

void HalSMInteger_shiftLeftSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int bit) {
    if (bit==0)return;
    long long int i,b;
    unsigned char carry,savedCarry;
    unsigned long long int newSize=bit/8+1;
    unsigned char* temp=hsmmml->malloc(a->size+newSize);
    for (i=0;i<a->size;i++){temp[i+newSize]=a->value[i];}
    hsmmml->free(a->value);
    a->value=temp;
    for (i=0;i<newSize;i++)a->value[i]=0;
    a->size=a->size+newSize;
    for (b=0;b<bit;b++)
    {
        carry=0;
        for (i=a->size-1;i>=0;i--)
        {
            savedCarry=a->value[i]>>7;
            a->value[i]=(a->value[i]<<1)|carry;
            carry=savedCarry;
        }
    }
    HalSMInteger_clear(hsmmml,a);
}

HalSMInteger* HalSMInteger_shiftLeft(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,long long int bit) {
    unsigned char* temp=hsmmml->malloc(a->size);
    memory_cpy(temp,a->value,a->size);
    HalSMInteger* out=HalSMInteger_init(hsmmml,a->negative,temp,a->size);
    HalSMInteger_shiftLeftSelf(hsmmml,out,bit);
    return out;
}

HalSMInteger* HalSMInteger_clear(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    long long int i,b,mx;
    b=0;
    mx=a->size-1;
    for (i=0;i<mx;i++) {
        if (a->value[i]==0)b++;
        else break;
    }
    if (b==0)return a;
    unsigned char* temp;
    temp=hsmmml->malloc(a->size-b);
    memory_cpy(temp,&a->value[b],a->size-b);
    hsmmml->free(a->value);
    a->value=temp;
    a->size=a->size-b;
    return a;
}

void HalSMInteger_PowSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    HalSMInteger nb;
    HalSMInteger* base=HalSMInteger_copy(hsmmml,a);
    a->value=hsmmml->malloc(1);
    a->value[0]=1;
    a->size=1;
    HalSMInteger* exp=HalSMInteger_copy(hsmmml,b);
    unsigned char isNegative=a->negative;
    while (hi_isNotNull(exp->value,exp->size)) {
        if (HalSMInteger_getBit(exp,0,0)){HalSMInteger_MulSelf(hsmmml,a,base);}
        HalSMInteger_MulSelf(hsmmml,base,base);
        HalSMInteger_shiftRightSelf(exp,1);
    }
    HalSMInteger remainder;
    if (isNegative) {
        HalSMInteger_Div(hsmmml,b,&HalSMIntegerTwo,&remainder);
        a->negative=1;
        if (HalSMInteger_isEqual(&remainder,&HalSMIntegerZero)) {
            a->negative=0;
        }
    }
}

HalSMInteger* HalSMInteger_Pow(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    unsigned char* temp=hsmmml->malloc(a->size);
    memory_cpy(temp,a->value,a->size);
    HalSMInteger* out=HalSMInteger_init(hsmmml,a->negative,temp,a->size);
    HalSMInteger_PowSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_Free(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    hsmmml->free(a->value);
    hsmmml->free(a);
}

unsigned char HalSMInteger_ByteCTZ(unsigned char byte) {
    if (((byte>>7)&1)==1) {
        return 0;
    } else if (((byte>>6)&1)==1) {
        return 1;
    } else if (((byte>>5)&1)==1) {
        return 2;
    } else if (((byte>>4)&1)==1) {
        return 3;
    } else if (((byte>>3)&1)==1) {
        return 4;
    } else if (((byte>>2)&1)==1) {
        return 5;
    } else if (((byte>>1)&1)==1) {
        return 6;
    } else if (((byte>>0)&1)==1) {
        return 7;
    }
    return 8;
}

long long int HalSMInteger_ctz(HalSMInteger* a)
{
    if (a->size==0){return 0;}
    long long int out=0,i;
    for (i=a->size-1;;i--) {
        if (a->value[i]==0) {
            out+=8;
        } else {
            out+=HalSMInteger_ByteCTZ(a->value[i]);
            break;
        }
        if (i==0) {
            break;
        }
    }
    return out;
}

void HalSMInteger_BitORSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    if (b->size==0&&a->size==0) {return;}
    unsigned long long int i,cm,j,c;
    unsigned char tempb;
    cm=a->size;
    if (a->size<b->size) {
        a->value=hsmmml->realloc(a->value,b->size);
        for (i=a->size-1,j=b->size-1;;i--,j--) {
            a->value[j]=a->value[i];
            //a->value[i]=0;
            if (i==0){break;}
        }
        j=b->size-a->size;
        for (i=0;i<j;i++) {
            a->value[i]=0;
        }
        a->size=b->size;
        cm=b->size;
    }
    for (i=a->size-1,j=b->size,c=0;c<cm;i--,c++) {
        if (j==0) {
            tempb=0;
        } else {
            j--;
            tempb=b->value[j];
        }
        a->value[i]=a->value[i]|tempb;
    }
}

HalSMInteger* HalSMInteger_BitOR(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_BitORSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_BitXORSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    if (b->size==0&&a->size==0) {return;}
    unsigned long long int i,cm,j,c;
    unsigned char tempb;
    cm=a->size;
    if (a->size<b->size) {
        a->value=hsmmml->realloc(a->value,b->size);
        for (i=a->size-1,j=b->size-1;;i--,j--) {
            a->value[j]=a->value[i];
            //a->value[i]=0;
            if (i==0){break;}
        }
        j=b->size-a->size;
        for (i=0;i<j;i++) {
            a->value[i]=0;
        }
        a->size=b->size;
        cm=b->size;
    }
    for (i=a->size-1,j=b->size,c=0;c<cm;i--,c++) {
        if (j==0) {
            tempb=0;
        } else {
            j--;
            tempb=b->value[j];
        }
        a->value[i]=a->value[i]^tempb;
    }
}

HalSMInteger* HalSMInteger_BitXOR(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_BitXORSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_BitANDSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    if (b->size==0&&a->size==0) {return;}
    unsigned long long int i,cm,j,c;
    unsigned char tempb;
    cm=a->size;
    if (a->size<b->size) {
        a->value=hsmmml->realloc(a->value,b->size);
        for (i=a->size-1,j=b->size-1;;i--,j--) {
            a->value[j]=a->value[i];
            //a->value[i]=0;
            if (i==0){break;}
        }
        j=b->size-a->size;
        for (i=0;i<j;i++) {
            a->value[i]=0;
        }
        a->size=b->size;
        cm=b->size;
    }
    for (i=a->size-1,j=b->size,c=0;c<cm;i--,c++) {
        if (j==0) {
            tempb=0;
        } else {
            j--;
            tempb=b->value[j];
        }
        a->value[i]=a->value[i]&tempb;
    }
}

HalSMInteger* HalSMInteger_BitAND(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_BitANDSelf(hsmmml,out,b);
    return out;
}

void HalSMInteger_BitNOTSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    if (a->size==0) {return;}
    unsigned long long int i;
    for (i=0;i<a->size;i++) {
        a->value[i]=~a->value[i];
    }
}

HalSMInteger* HalSMInteger_BitNOT(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a)
{
    HalSMInteger* out=HalSMInteger_copy(hsmmml,a);
    HalSMInteger_BitNOTSelf(hsmmml,out);
    return out;
}

HalSMInteger* HalSMInteger_FromHalSMDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    HalSMInteger remainder;
    HalSMInteger* out=HalSMInteger_Div(hsmmml,a->a,a->b,&remainder);
    if (HalSMInteger_isLess(&remainder,HalSMInteger_Div(hsmmml,a->b,&HalSMIntegerTwo,NOTHALSMNULLPOINTER))==0) {
        HalSMInteger_AddSelf(hsmmml,out,&HalSMIntegerOne);
    }
    return out;
}

HalSMInteger* HalSMInteger_FloorHalSMDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    HalSMInteger* out=HalSMInteger_Div(hsmmml,a->a,a->b,NOTHALSMNULLPOINTER);
    out->negative=a->negative;
    return out;
}

HalSMInteger* HalSMInteger_CeilHalSMDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    HalSMInteger remainder;
    HalSMInteger* out=HalSMInteger_Div(hsmmml,a->a,a->b,&remainder);
    if (HalSMInteger_isMore(&remainder,&HalSMIntegerZero)) {
        HalSMInteger_AddSelf(hsmmml,out,&HalSMIntegerOne);
    }
    out->negative=a->negative;
    return out;
}

//HalSMInteger

//HalSMDouble

HalSMDouble* HalSMDouble_init(HalSMMemoryManagmentLibrary* hsmmml,unsigned char negative,HalSMInteger* a,HalSMInteger* b)
{
    
    HalSMDouble* out=hsmmml->malloc(sizeof(HalSMDouble));
    out->negative=negative;
    out->a=a;
    out->b=b;
    return out;
}

HalSMDouble* HalSMDouble_copy(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    HalSMDouble* out=hsmmml->malloc(sizeof(HalSMDouble));
    out->negative=a->negative;

    out->a=HalSMInteger_copy(hsmmml,a->a);

    out->b=HalSMInteger_copy(hsmmml,a->b);
    return out;
}

HalSMDouble* HalSMDouble_FromUnsignedInteger(HalSMMemoryManagmentLibrary* hsmmml,unsigned int value)
{
    unsigned char* v=hsmmml->malloc(4);
    v[0]=(value>>24)&0xFF;
    v[1]=(value>>16)&0xFF;
    v[2]=(value>>8)&0xFF;
    v[3]=value&0xFF;
    
    unsigned char* v1=hsmmml->malloc(1);
    v1[0]=1;
    return HalSMDouble_init(hsmmml,0,HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v,4)),HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v1,1)));
}

HalSMDouble* HalSMDouble_FromSignedInteger(HalSMMemoryManagmentLibrary* hsmmml,signed int value)
{
    unsigned char* v=hsmmml->malloc(4);
    unsigned char negate=value<0?1:0;
    if (negate) {
        value=-value;
    }
    v[0]=(value>>24)&0xFF;
    v[1]=(value>>16)&0xFF;
    v[2]=(value>>8)&0xFF;
    v[3]=value&0xFF;
    
    unsigned char* v1=hsmmml->malloc(1);
    v1[0]=1;
    return HalSMDouble_init(hsmmml,negate,HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v,4)),HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v1,1)));
}

HalSMDouble* HalSMDouble_FromFloat(HalSMMemoryManagmentLibrary* hsmmml,float value)
{
    unsigned char negate=value<0?1:0;
    value=negate?-value:value;

    //precision
    unsigned int c=1000000;
    float b=(value-((unsigned int)value))*c;
    unsigned int d=((unsigned int)value)*c+(unsigned int)(b+0.5);

    while (d%10==0) {
        d/=10;
        c/=10;
    }

    unsigned char* v=hsmmml->malloc(4);
    v[0]=(d>>24)&0xFF;
    v[1]=(d>>16)&0xFF;
    v[2]=(d>>8)&0xFF;
    v[3]=d&0xFF;

    unsigned char* v1=hsmmml->malloc(4);
    v1[0]=(c>>24)&0xFF;
    v1[1]=(c>>16)&0xFF;
    v1[2]=(c>>8)&0xFF;
    v1[3]=c&0xFF;
    return HalSMDouble_clear(hsmmml,HalSMDouble_init(hsmmml,negate,HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v,4)),HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v1,4))));
}

HalSMDouble* HalSMDouble_FromDouble(HalSMMemoryManagmentLibrary* hsmmml,double value)
{
    unsigned char negate=value<0?1:0;
    value=negate?-value:value;

    unsigned int c=100000000;
    double b=(value-((unsigned int)value))*c;
    unsigned int d=((unsigned int)value)*c+(unsigned int)(b+0.5);

    while (d%10==0) {
        d/=10;
        c/=10;
    }

    unsigned char* v=hsmmml->malloc(4);
    v[0]=(d>>24)&0xFF;
    v[1]=(d>>16)&0xFF;
    v[2]=(d>>8)&0xFF;
    v[3]=d&0xFF;

    unsigned char* v1=hsmmml->malloc(4);
    v1[0]=(c>>24)&0xFF;
    v1[1]=(c>>16)&0xFF;
    v1[2]=(c>>8)&0xFF;
    v1[3]=c&0xFF;
    return HalSMDouble_clear(hsmmml,HalSMDouble_init(hsmmml,negate,HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v,4)),HalSMInteger_clear(hsmmml,HalSMInteger_init(hsmmml,0,v1,4))));
}

HalSMDouble* HalSMDouble_FromHalSMInteger(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* value)
{
    unsigned char* v1=hsmmml->malloc(1);
    v1[0]=1;
    value=HalSMInteger_copy(hsmmml,value);
    unsigned char isNeg=value->negative;
    value->negative=0;
    return HalSMDouble_init(hsmmml,isNeg,value,HalSMInteger_init(hsmmml,0,v1,1));

}

HalSMDouble* HalSMDouble_parse(HalSMMemoryManagmentLibrary* hsmmml,HalStringFormatChar* str)
{
    HalSMDouble* rez = HalSMDouble_FromUnsignedInteger(hsmmml,0), *fact = HalSMDouble_FromUnsignedInteger(hsmmml,1);
    HalSMDouble HalSMDoubleTen={0,&HalSMIntegerTen,&HalSMIntegerOne};
    HalSMInteger HalSMDoubledf={0,(unsigned char[]){0,0,0,1},4};
    HalSMInteger HalSMDoubleds={0,(unsigned char[]){0,0,0,1},4};
    HalSMDouble HalSMDoubled={0,&HalSMDoubledf,&HalSMDoubleds};
    if (HSFCharCompare(*str,ASCIIH('-'))){
        str++;
        fact->negative=1;
    }
    //OPTIMIZE HSF
    for (int point_seen = 0;HSFCharCompare(*str,ASCIIH('\0'))==0; str++){
        if (HSFCharCompare(*str,ASCIIH('.'))){
            point_seen = 1; 
            continue;
        }
        long long int d = str->symbol - '0';
        if (d >= 0 && d <= 9){
            if (point_seen){HalSMDouble_DivSelf(hsmmml,fact,&HalSMDoubleTen);}
            HalSMDouble_MulSelf(hsmmml,rez,&HalSMDoubleTen);
            HalSMDoubled.a->value[3]=d;
            HalSMDouble_AddSelf(hsmmml,rez,&HalSMDoubled);
        } else {break;}
    }
    return HalSMDouble_clear(hsmmml,HalSMDouble_Mul(hsmmml,rez,fact));
}

unsigned char HalSMDouble_isMore(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMDouble* r=HalSMDouble_Sub(hsmmml,a,b);
    if (r->negative==1||HalSMDouble_isEqual(r,&HalSMDoubleZero)){return 0;}
    return 1;
}

unsigned char HalSMDouble_isLess(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMDouble* r=HalSMDouble_Sub(hsmmml,a,b);
    if (r->negative==0||HalSMDouble_isEqual(r,&HalSMDoubleZero)){HalSMDouble_Free(hsmmml,r);return 0;}
    return 1;
}

unsigned char HalSMDouble_isEqual(HalSMDouble* a,HalSMDouble* b)
{
    /*HalSMInteger* av0=HalSMInteger_clear(hsmmml,a->a);
    HalSMInteger* av1=HalSMInteger_clear(hsmmml,a->b);
    HalSMInteger* bv0=HalSMInteger_clear(hsmmml,b->a);
    HalSMInteger* bv1=HalSMInteger_clear(hsmmml,b->b);*/
    //if (a->negative!=b->negative||av0->size!=bv0->size||av1->size!=bv1->size) {return 0;}
    if (a->negative!=b->negative){return 0;}
    /*unsigned long long i;
    for (i=0;i<av0->size;i++) {
        if (av0->value[i]!=bv0->value[i]) {return 0;}
    }
    for (i=0;i<av1->size;i++) {
        if (av1->value[i]!=bv1->value[i]) {return 0;}
    }
    return 1;*/
    if (HalSMInteger_isEqual(a->a,b->a)==0||HalSMInteger_isEqual(a->b,b->b)==0){return 0;}
    return 1;
}

void HalSMDouble_AddSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    if (a->negative&&b->negative==0) {
        a->negative=0;
        HalSMDouble_SubSelf(hsmmml,a,b);
        if (a->a->size==0||a->b->size==0||(a->a->size==1&&a->a->value[0]==0)||(a->b->size==1&&a->b->value[0]==0)) {
            a->negative=0;
            return;
        }
        a->negative=a->negative?0:1;
        return;
    } else if (b->negative&&a->negative==0) {
        b->negative=0;
        HalSMDouble_SubSelf(hsmmml,a,b);
        b->negative=1;
        return;
    }

    HalSMInteger_MulSelf(hsmmml,a->a,b->b);
    HalSMInteger_AddSelf(hsmmml,a->a,HalSMInteger_Mul(hsmmml,b->a,a->b));
    HalSMInteger_MulSelf(hsmmml,a->b,b->b);
    //HalSMDouble_clear(hsmmml,a);
}

HalSMDouble* HalSMDouble_Add(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_AddSelf(hsmmml,out,b);
    return out;
}

void HalSMDouble_SubSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    if (b->negative&&a->negative==0) {
        b->negative=0;
        HalSMDouble_AddSelf(hsmmml,a,b);
        b->negative=1;
        return;
    } else if (a->negative&&b->negative==0) {
        a->negative=0;
        HalSMDouble_AddSelf(hsmmml,a,b);
        a->negative=1;
        return;
    }

    HalSMInteger_MulSelf(hsmmml,a->a,b->b);
    HalSMInteger* bb=HalSMInteger_Mul(hsmmml,b->a,a->b);
    HalSMInteger_SubSelf(hsmmml,a->a,bb);
    HalSMInteger_Free(hsmmml,bb);
    HalSMInteger_MulSelf(hsmmml,a->b,b->b);
    if (a->negative&&b->negative) {
        a->negative=a->a->negative?0:1;
    } else {
        a->negative=a->a->negative;
    }
    a->a->negative=0;
    //HalSMDouble_clear(hsmmml,a);
}

HalSMDouble* HalSMDouble_Sub(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_SubSelf(hsmmml,out,b);
    return out;
}

void HalSMDouble_MulSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMInteger_MulSelf(hsmmml,a->a,b->a);
    HalSMInteger_MulSelf(hsmmml,a->b,b->b);
    if (a->negative&&b->negative)a->negative=0;
    else if (b->negative)a->negative=1;
    //HalSMDouble_clear(hsmmml,a);
}

HalSMDouble* HalSMDouble_Mul(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_MulSelf(hsmmml,out,b);
    return out;
}

void HalSMDouble_DivSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMInteger_MulSelf(hsmmml,a->a,b->b);
    HalSMInteger_MulSelf(hsmmml,a->b,b->a);
    if (a->negative&&b->negative)a->negative=0;
    else if (b->negative)a->negative=1;
    //HalSMDouble_clear(hsmmml,a);
}

HalSMDouble* HalSMDouble_Div(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMDouble* b)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_DivSelf(hsmmml,out,b);
    return out;
}

HalStringFormatChar* HalSMDouble_toString(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,unsigned char isHex)
{
    HalSMInteger* remainder=hsmmml->malloc(sizeof(HalSMInteger));
    HalSMInteger* quotient=HalSMInteger_Div(hsmmml,a->a,a->b,remainder);
    HalSMInteger NOLL={0,(unsigned char[]){0},1};
    HalSMInteger DESYAT={0,(unsigned char[]){10},1};
    HalSMInteger ODIN={0,(unsigned char[]){1},1};
    HalStringFormatChar* f=HalSMInteger_toString(hsmmml,quotient,0);
    HalSMInteger* res=HalSMInteger_FromUnsignedInteger(hsmmml,0);
    unsigned long long int i=0;
    unsigned long long int cn=0;
    unsigned char isNotStart=1;
    while (HalSMInteger_isMore(remainder,&HalSMIntegerZero)) {
        if (HalSMInteger_isMore(remainder,a->b)){
            HalSMInteger_MulSelf(hsmmml,res,&HalSMIntegerTen);
            HalSMInteger_AddSelf(hsmmml,res,HalSMInteger_Div(hsmmml,remainder,a->b,remainder));
            isNotStart=0;
        } else if (HalSMInteger_isEqual(remainder,a->b)){
            HalSMInteger_MulSelf(hsmmml,res,&HalSMIntegerTen);
            HalSMInteger_AddSelf(hsmmml,res,&HalSMIntegerOne);
            break;
        } else if (isNotStart) {
            cn++;
        } else {
            HalSMInteger_MulSelf(hsmmml,res,&HalSMIntegerTen);
        }
        HalSMInteger_MulSelf(hsmmml,remainder,&HalSMIntegerTen);
        if (i==100)break;
        i++;
    }
    HalStringFormatChar* out;
    if (cn>1) {
        HalStringFormatChar* t=HalSMInteger_toString(hsmmml,res,0);
        out=hsmmml->malloc((cn+string_len(t)+1)*sizeof(HalStringFormatChar));
        memory_cpy(&out[cn-1],t,(string_len(t)+1)*sizeof(HalStringFormatChar));
        while (cn-->1) {
            out[cn-1]=ASCIIH('0');
        }
    } else {
        out=HalSMInteger_toString(hsmmml,res,0);
    }
    out=ConcatenateStrings(hsmmml,f,ConcatenateStrings(hsmmml,U8H("."),out));
    return (a->negative)?ConcatenateStrings(hsmmml,U8H("-"),out):out;
}

HalStringFormatChar* HalSMDouble_toStringRound(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,unsigned long long int round)
{
    HalSMInteger* remainder=hsmmml->malloc(sizeof(HalSMInteger));
    HalSMInteger* quotient=HalSMInteger_Div(hsmmml,a->a,a->b,remainder);
    HalSMInteger NOLL={0,(unsigned char[]){0},1};
    HalSMInteger DESYAT={0,(unsigned char[]){10},1};
    HalSMInteger ODIN={0,(unsigned char[]){1},1};
    HalStringFormatChar* f=HalSMInteger_toString(hsmmml,quotient,0);
    HalSMInteger* res=HalSMInteger_FromUnsignedInteger(hsmmml,0);
    unsigned long long int i=0;
    unsigned long long int cn=0;
    unsigned char isNotStart=1;
    while (HalSMInteger_isMore(remainder,&HalSMIntegerZero)) {
        if (HalSMInteger_isMore(remainder,a->b)){
            HalSMInteger_MulSelf(hsmmml,res,&HalSMIntegerTen);
            HalSMInteger_AddSelf(hsmmml,res,HalSMInteger_Div(hsmmml,remainder,a->b,remainder));
            isNotStart=0;
        } else if (HalSMInteger_isEqual(remainder,a->b)){
            HalSMInteger_MulSelf(hsmmml,res,&HalSMIntegerTen);
            HalSMInteger_AddSelf(hsmmml,res,&HalSMIntegerOne);
            break;
        } else if (isNotStart) {
            cn++;
        } else {
            HalSMInteger_MulSelf(hsmmml,res,&HalSMIntegerTen);
        }
        HalSMInteger_MulSelf(hsmmml,remainder,&HalSMIntegerTen);
        if (i==round)break;
        i++;
    }
    HalStringFormatChar* out;
    if (cn>1) {
        HalStringFormatChar* t=HalSMInteger_toString(hsmmml,res,0);
        out=hsmmml->malloc((cn+string_len(t)+1)*sizeof(HalStringFormatChar));
        memory_cpy(&out[cn-1],t,(string_len(t)+1)*sizeof(HalStringFormatChar));
        while (cn-->1) {
            out[cn-1]=ASCIIH('0');
        }
    } else {
        out=HalSMInteger_toString(hsmmml,res,0);
    }
    out=ConcatenateStrings(hsmmml,f,ConcatenateStrings(hsmmml,U8H("."),out));
    return (a->negative)?ConcatenateStrings(hsmmml,U8H("-"),out):out;
}

double HalSMDouble_ToDouble(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    int i;
    HalSMInteger* ai=HalSMInteger_copy(hsmmml,a->a);
    HalSMInteger remainder;
    HalSMInteger_DivSelf(hsmmml,ai,a->b,&remainder);
    double out=HalSMInteger_ToUnsignedInteger(ai);
    for (i=10;i<=10000000;i*=10) {
        ai=HalSMInteger_copy(hsmmml,&remainder);
        if (HalSMInteger_isEqual(ai,&HalSMIntegerZero)){break;}
        HalSMInteger_MulSelf(hsmmml,ai,&HalSMIntegerTen);
        HalSMInteger_DivSelf(hsmmml,ai,a->b,&remainder);
        out+=((double)HalSMInteger_ToUnsignedInteger(ai))/i;
    }
    if (a->negative) {
        out=-out;
    }
    return out;
}

float HalSMDouble_ToFloat(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    int i;
    HalSMInteger* ai=HalSMInteger_copy(hsmmml,a->a);
    HalSMInteger remainder;
    HalSMInteger_DivSelf(hsmmml,ai,a->b,&remainder);
    float out=HalSMInteger_ToUnsignedInteger(ai);
    for (i=10;i<=10000000;i*=10) {
        ai=HalSMInteger_copy(hsmmml,&remainder);
        if (HalSMInteger_isEqual(ai,&HalSMIntegerZero)){break;}
        HalSMInteger_MulSelf(hsmmml,ai,&HalSMIntegerTen);
        HalSMInteger_DivSelf(hsmmml,ai,a->b,&remainder);
        out+=((float)HalSMInteger_ToUnsignedInteger(ai))/i;
    }
    if (a->negative) {
        out=-out;
    }
    return out;
}

HalSMInteger* stepsr(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* k,HalSMInteger* a,HalSMInteger* x)
{
    HalSMInteger* temp=HalSMInteger_Sub(hsmmml,k,&HalSMIntegerOne);
    return HalSMInteger_Div(hsmmml,HalSMInteger_Add(hsmmml,HalSMInteger_Mul(hsmmml,temp,x),HalSMInteger_Pow(hsmmml,HalSMInteger_Div(hsmmml,a,x,NOTHALSMNULLPOINTER),temp)),k,NOTHALSMNULLPOINTER);
}

HalSMInteger* iPow(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* e)
{
    HalSMInteger* r=HalSMInteger_copy(hsmmml,&HalSMIntegerOne);
    if (HalSMInteger_isEqual(e,&HalSMIntegerZero)){return r;}
    while (HalSMInteger_isEqual(e,&HalSMIntegerZero)==0) {
        if (HalSMInteger_getBit(e,0,0)==1){HalSMInteger_MulSelf(hsmmml,r,a);}
        HalSMInteger_shiftRightSelf(e,1);
        HalSMInteger_clear(hsmmml,e);
        HalSMInteger_MulSelf(hsmmml,a,a);
    }
    return r;
}

HalSMInteger* HalSMInteger_SquareRoot(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* k)
{
    if (HalSMInteger_isLess(a,&HalSMIntegerOne)) {
        return a;
    }
    
    /*HalSMInteger* x0,*x1;
    x0=HalSMInteger_Div(hsmmml,a,&HalSMIntegerTwo,NOTHALSMNULLPOINTER);
    x1=HalSMInteger_Add(hsmmml,x0,HalSMInteger_Div(hsmmml,a,x0,NOTHALSMNULLPOINTER));
    HalSMInteger_DivSelf(hsmmml,x1,&HalSMIntegerTwo,NOTHALSMNULLPOINTER);
    while (HalSMInteger_isLess(x1,x0)) {
        x0=x1;
        x1=HalSMInteger_Add(hsmmml,x0,HalSMInteger_Div(hsmmml,a,x0,NOTHALSMNULLPOINTER));
        HalSMInteger_DivSelf(hsmmml,x1,&HalSMIntegerTwo,NOTHALSMNULLPOINTER);
    }
    return x0;*/
    HalSMInteger* u,*s,*t,*d;
    u=HalSMInteger_copy(hsmmml,a);
    s=HalSMInteger_Add(hsmmml,u,&HalSMIntegerOne);
    while (HalSMInteger_isLess(u,s)) {
        s=u;
        d=HalSMInteger_Sub(hsmmml,k,&HalSMIntegerOne);
        t=HalSMInteger_Add(hsmmml,HalSMInteger_Mul(hsmmml,d,s),HalSMInteger_Div(hsmmml,a,HalSMInteger_Pow(hsmmml,s,d),NOTHALSMNULLPOINTER));
        u=HalSMInteger_Div(hsmmml,t,k,NOTHALSMNULLPOINTER);
    }
    return s;
}

HalSMDouble* HalSMDouble_toDesyatki(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    while (HalSMDouble_isMore(hsmmml,a,&HalSMDoubleOne)) {
        HalSMDouble_DivSelf(hsmmml,a,&HalSMDoubleTen);
    }
    return a;
}

/*HalSMDouble* stepdsr(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* x,HalSMDouble* a,HalSMDouble* v,HalSMInteger* z,HalSMDouble* c)
{
    HalSMDouble* p,*d,*m,*e,*l;
    p=HalSMDouble_Pow(hsmmml,x,z);
    HalSMDouble_CropSelf(hsmmml,p,10);
    d=HalSMDouble_Div(hsmmml,a,p);
    HalSMDouble_CropSelf(hsmmml,d,10);
    HalSMDouble_Free(hsmmml,p);
    m=HalSMDouble_Mul(hsmmml,v,x);
    HalSMDouble_CropSelf(hsmmml,m,10);
    e=HalSMDouble_Add(hsmmml,m,d);
    HalSMDouble_CropSelf(hsmmml,e,10);
    HalSMDouble_Free(hsmmml,d);
    HalSMDouble_Free(hsmmml,m);
    l=HalSMDouble_Mul(hsmmml,c,e);
    HalSMDouble_CropSelf(hsmmml,l,10);
    HalSMDouble_Free(hsmmml,e);
    return l;
}*/

void HalSMDouble_SquareRootSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* x,HalSMInteger* n,unsigned long long int crop)
{
    //HalSquareRootFindAlgorithm
    //root=HSRFA(isqrt(x),x-isqrt(x))
    //HALWARSING ROOT DIVIDER = 3
    //1. Find integer square root of x = iroot
    //2. a=x - iroot^2
    //3. b=a/30
    //4. c=subisqrt
    //5. while precision
    //6.  c=c+b
    //7.  a=x-(iroot+c)^2
    //8.  b=a/3
    //9. return iroot+c
    /*HalSMInteger remainder;
    HalSMInteger* iroot=HalSMInteger_SquareRoot(hsmmml,HalSMInteger_Div(hsmmml,x->a,x->b,&remainder));
    HalSMDouble* a=HalSMDouble_Sub(hsmmml,x,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,iroot,iroot)));
    HalSMDouble* b=HalSMDouble_Div(hsmmml,a,HalSMDouble_parse(hsmmml,U8H("3")));
    HalSMDouble* c=HalSMDouble_FromUnsignedInteger(hsmmml,0);
    HalSMDouble* root=HalSMDouble_FromHalSMInteger(hsmmml,iroot);
    HalSMDouble* temp;
    for (int i=0;i<30;i++) {
        HalSMDouble_AddSelf(hsmmml,c,b);
        HalSMDouble_Free(hsmmml,b);
        temp=HalSMDouble_Add(hsmmml,root,HalSMDouble_toDesyatki(hsmmml,HalSMDouble_copy(hsmmml,c)));
        a=HalSMDouble_Sub(hsmmml,x,HalSMDouble_Mul(hsmmml,temp,temp));
        HalSMDouble_Free(hsmmml,temp);
        b=HalSMDouble_Div(hsmmml,a,&HalSMDoubleThree);
        HalSMDouble_Free(hsmmml,a);
    }
    *x=*HalSMDouble_Add(hsmmml,root,HalSMDouble_toDesyatki(hsmmml,HalSMDouble_copy(hsmmml,c)));*/
    /*HalSMInteger* na,*nb,*qa,*qb,*da;
    da=HalSMInteger_Div(hsmmml,a->a,a->b,NOTHALSMNULLPOINTER);
    HalSMInteger* i=HalSMInteger_copy(hsmmml,q);
    na=HalSMInteger_copy(hsmmml,da);
    while (HalSMInteger_isMore(i,&HalSMIntegerOne)) {
        na=HalSMInteger_SquareRoot(hsmmml,na);
        HalSMInteger_SubSelf(hsmmml,i,&HalSMIntegerOne);
    }
    nb=HalSMInteger_Add(hsmmml,na,&HalSMIntegerOne);
    qa=HalSMInteger_copy(hsmmml,na);
    qb=HalSMInteger_copy(hsmmml,nb);
    i=HalSMInteger_copy(hsmmml,q);
    while (HalSMInteger_isMore(i,&HalSMIntegerOne)) {
        HalSMInteger_MulSelf(hsmmml,qa,qa);
        HalSMInteger_MulSelf(hsmmml,qb,qb);
        HalSMInteger_SubSelf(hsmmml,i,&HalSMIntegerOne);
    }
        printf("G: %s\n",HU8(HalSMInteger_toString(hsmmml,da,0)));
        printf("G: %s\n",HU8(HalSMInteger_toString(hsmmml,qa,0)));
        printf("G: %s\n",HU8(HalSMInteger_toString(hsmmml,qb,0)));
    if (HalSMInteger_isLess(HalSMInteger_Sub(hsmmml,da,qa),HalSMInteger_absolute(HalSMInteger_Sub(hsmmml,qb,da)))) {
        //A
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,qa));
        HalSMDouble_DivSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,na,&HalSMIntegerTwo)));
        HalSMDouble_AddSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,na));
    } else {
        //B
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,qb));
        HalSMDouble_DivSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,nb,&HalSMIntegerTwo)));
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,nb));
        a->negative=0;
    }*/

    /*HalSMInteger* na,*nb,*qa,*qb,*da;
    da=HalSMInteger_Div(hsmmml,a->a,a->b,NOTHALSMNULLPOINTER);
    na=HalSMInteger_SquareRoot(hsmmml,da);
    nb=HalSMInteger_Add(hsmmml,na,&HalSMIntegerOne);
    qa=HalSMInteger_Mul(hsmmml,na,na);
    qb=HalSMInteger_Mul(hsmmml,nb,nb);
    if (HalSMInteger_isLess(HalSMInteger_Sub(hsmmml,da,qa),HalSMInteger_absolute(HalSMInteger_Sub(hsmmml,qb,da)))) {
        //A
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,qa));
        HalSMDouble_DivSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,na,&HalSMIntegerTwo)));
        HalSMDouble_AddSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,na));
    } else {
        //B
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,qb));
        HalSMDouble_DivSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,nb,&HalSMIntegerTwo)));
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,nb));
        a->negative=0;
    }*/

    /*HalSMInteger* na,*nb,*qa,*qb,*da;
    unsigned char isNegative=0;
    if (a->negative){isNegative=1;a->negative=0;}
    da=HalSMInteger_Div(hsmmml,a->a,a->b,NOTHALSMNULLPOINTER);
    na=HalSMInteger_copy(hsmmml,da);
    na=HalSMInteger_SquareRoot(hsmmml,na,q);
    nb=HalSMInteger_Add(hsmmml,na,&HalSMIntegerOne);
    qa=HalSMInteger_Pow(hsmmml,na,q);
    qb=HalSMInteger_Pow(hsmmml,nb,q);

    if (HalSMInteger_isLess(HalSMInteger_Sub(hsmmml,da,qa),HalSMInteger_absolute(HalSMInteger_Sub(hsmmml,qb,da)))) {
        //A
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,qa));
        HalSMDouble_DivSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,q,HalSMInteger_Pow(hsmmml,na,HalSMInteger_Sub(hsmmml,q,&HalSMIntegerOne)))));
        HalSMDouble_AddSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,na));
    } else {
        //B
        HalSMDouble_SubSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,qb));
        HalSMDouble_DivSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,HalSMInteger_Mul(hsmmml,q,HalSMInteger_Pow(hsmmml,nb,HalSMInteger_Sub(hsmmml,q,&HalSMIntegerOne)))));
        HalSMDouble_AddSelf(hsmmml,a,HalSMDouble_FromHalSMInteger(hsmmml,nb));
        a->negative=0;
    }
    HalSMInteger rem;
    HalSMInteger_Div(hsmmml,q,&HalSMIntegerTwo,&rem);
    if (isNegative&&HalSMInteger_isEqual(&rem,&HalSMIntegerOne)) {
        a->negative=1;
    }*/
    /*unsigned long long int i;
    HalSMDouble* x=HalSMDouble_copy(hsmmml,&HalSMDoubleOne),*g,*v,*c;
    HalSMInteger* z;
    g=HalSMDouble_FromHalSMInteger(hsmmml,n);
    v=HalSMDouble_Sub(hsmmml,g,&HalSMDoubleOne);
    c=HalSMDouble_Div(hsmmml,&HalSMDoubleOne,g);
    z=HalSMInteger_Sub(hsmmml,n,&HalSMIntegerOne);
    for (i=0;i<200;i++) {
        x=stepdsr(hsmmml,x,a,v,z,c);
    }
    a->a=x->a;
    a->b=x->b;*/
    HalSMDouble* high,*low,*mid,*p;
    high=HalSMDouble_copy(hsmmml,&HalSMDoubleOne);
    while (HalSMDouble_isMore(hsmmml,HalSMDouble_Pow(hsmmml,high,n),x)==0) {
        HalSMInteger_MulSelf(hsmmml,high->a,&HalSMIntegerTwo);
    }
    low=HalSMDouble_Div(hsmmml,high,&HalSMDoubleTwo);
    while (HalSMDouble_isLess(hsmmml,low,high)) {
        mid=HalSMDouble_Div(hsmmml,HalSMDouble_Add(hsmmml,low,high),&HalSMDoubleTwo);
        HalSMDouble_CropSelf(hsmmml,mid,crop);
        p=HalSMDouble_Pow(hsmmml,mid,n);
        HalSMDouble_CropSelf(hsmmml,p,crop);
        if (HalSMDouble_isLess(hsmmml,low,mid)&&HalSMDouble_isLess(hsmmml,p,x)) {
            low=mid;
        } else if (HalSMDouble_isMore(hsmmml,high,mid)&&HalSMDouble_isMore(hsmmml,p,x)) {
            high=mid;
        } else {
            *x=*mid;
            return;
        }
    }
    *x=*mid;
    HalSMDouble_AddSelf(hsmmml,x,&HalSMDoubleOne);
}

HalSMDouble* HalSMDouble_SquareRoot(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* q,unsigned long long int crop)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_SquareRootSelf(hsmmml,out,q,crop);
    return out;
}

void HalSMDouble_AbsSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    a->negative=0;
}

HalSMDouble* HalSMDouble_Abs(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_AbsSelf(hsmmml,out);
    return out;
}

void HalSMDouble_Free(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    hsmmml->free(a->a->value);
    hsmmml->free(a->a);
    hsmmml->free(a->b->value);
    hsmmml->free(a->b);
    hsmmml->free(a);
}

void HalSMDouble_PowSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b)
{
    unsigned char isNegative=0;
    if (b->negative==1) {
        isNegative=1;
        b->negative=0;
    }
    HalSMInteger_PowSelf(hsmmml,a->a,b);
    HalSMInteger_PowSelf(hsmmml,a->b,b);
    
    HalSMInteger remainder;
    if (a->negative) {
        HalSMInteger_Div(hsmmml,b,&HalSMIntegerTwo,&remainder);
        if (HalSMInteger_isEqual(&remainder,&HalSMIntegerZero)) {
            a->negative=0;
        }
    }
    if (isNegative) {
        b->negative=1;
        HalSMDouble* temp=HalSMDouble_Div(hsmmml,&HalSMDoubleOne,a);
        a->a=temp->a;
        a->negative=temp->negative;
        a->b=temp->b;
    }
}

HalSMDouble* HalSMDouble_Pow(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_PowSelf(hsmmml,out,b);
    return out;
}

void HalSMDouble_RoundSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b)
{
    
}

HalSMDouble* HalSMDouble_Round(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,HalSMInteger* b)
{
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_RoundSelf(hsmmml,out,b);
    return out;
}

void HalSMDouble_CropSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a,unsigned long long int crop)
{
    HalStringFormatChar* s=HalSMDouble_toString(hsmmml,a,0);
    unsigned long long int index=StringIndexOf(hsmmml,s,U8H(".")),l,m;
    l=string_len(s);
    m=l-index-1;
    if (m>crop) {
        s[index+crop+1]=ASCIIH('\0');
        HalSMDouble* out=HalSMDouble_parse(hsmmml,s);
        a->a=out->a;
        a->b=out->b;
    }
    hsmmml->free(s);
}

HalSMInteger* HalSMDouble_gcdSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* a,HalSMInteger* b)
{
    if (HalSMInteger_isEqual(a,&HalSMIntegerZero)) {
        return b;
    }
    if (HalSMInteger_isEqual(b,&HalSMIntegerZero)) {
        return a;
    }

    HalSMInteger* t;
    unsigned long long int k;
    for (k=0;HalSMInteger_isEqual(HalSMInteger_BitAND(hsmmml,HalSMInteger_BitOR(hsmmml,a,b),&HalSMIntegerOne),&HalSMIntegerZero);k++) {
        HalSMInteger_shiftRightSelf(a,1);
        HalSMInteger_shiftRightSelf(b,1);
    }

    while (HalSMInteger_isEqual(HalSMInteger_BitAND(hsmmml,a,&HalSMIntegerOne),&HalSMIntegerZero)) {
        HalSMInteger_shiftRightSelf(a,1);
    }

    do {
        while (HalSMInteger_isEqual(HalSMInteger_BitAND(hsmmml,b,&HalSMIntegerOne),&HalSMIntegerZero)) {
            HalSMInteger_shiftRightSelf(b,1);
        }

        if (HalSMInteger_isMore(a,b)) {
            t=a;
            a=b;
            b=t;
        }
        HalSMInteger_SubSelf(hsmmml,b,a);
    } while (HalSMInteger_isEqual(b,&HalSMIntegerZero)==0);
    HalSMInteger_shiftLeftSelf(hsmmml,a,k);
    return a;
}

HalSMInteger* HalSMDouble_gcd(HalSMMemoryManagmentLibrary* hsmmml,HalSMInteger* u,HalSMInteger* v)
{
    return HalSMDouble_gcdSelf(hsmmml,HalSMInteger_copy(hsmmml,u),HalSMInteger_copy(hsmmml,v));
}

void HalSMDouble_clearSelf(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    //return;
    HalSMInteger* gcd=HalSMDouble_gcd(hsmmml,a->a,a->b);
    if (HalSMInteger_isEqual(gcd,&HalSMIntegerZero)){return;}
    HalSMInteger_DivSelf(hsmmml,a->a,gcd,NOTHALSMNULLPOINTER);
    HalSMInteger_DivSelf(hsmmml,a->b,gcd,NOTHALSMNULLPOINTER);
}

HalSMDouble* HalSMDouble_clear(HalSMMemoryManagmentLibrary* hsmmml,HalSMDouble* a)
{
    //return a;
    HalSMDouble* out=HalSMDouble_copy(hsmmml,a);
    HalSMDouble_clearSelf(hsmmml,out);
    return out;
}

//HalSMDouble

//HalSM File Pointer

HalStringFormatChar* HFP_toString(HalSMCompiler* hsmc,HalSMVariable* v){return U8H("<HalSMFilePointer>");}
HalSMVariable* HFP_add(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HFP_sub(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HFP_mul(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HFP_div(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HFP_pow(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HFP_modulo(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HFP_increment(HalSMCompiler* hsmc,HalSMVariable* v){return v;}
HalSMVariable* HFP_decrement(HalSMCompiler* hsmc,HalSMVariable* v){return v;}
unsigned char HFP_isEqual(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){{return a->value==b->value;}}
unsigned char HFP_isMore(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return 0;}
unsigned char HFP_isLess(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return 0;}
unsigned char HFP_getBool(HalSMCompiler* hsmc,HalSMVariable* v){return 1;}
long long int HFP_indexVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    if (HSFCompare(name,U8H("SEEK_SET"))) {
        return 0;
    } else if (HSFCompare(name,U8H("SEEK_CUR"))) {
        return 1;
    } else if (HSFCompare(name,U8H("SEEK_END"))) {
        return 2;
    }
    return -1;
}
long long int HFP_indexFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    if (HSFCompare(name,U8H("close"))) {
        return 0;
    } else if (HSFCompare(name,U8H("read"))) {
        return 1;
    } else if (HSFCompare(name,U8H("write"))) {
        return 2;
    } else if (HSFCompare(name,U8H("eof"))) {
        return 3;
    } else if (HSFCompare(name,U8H("seek"))) {
        return 4;
    } else if (HSFCompare(name,U8H("tell"))) {
        return 5;
    } else if (HSFCompare(name,U8H("rewind"))) {
        return 6;
    } else if (HSFCompare(name,U8H("flush"))) {
        return 7;
    } else if (HSFCompare(name,U8H("readUTF8"))) {
        return 8;
    } else if (HSFCompare(name,U8H("writeUTF8"))) {
        return 9;
    } else if (HSFCompare(name,U8H("readHSF"))) {
        return 10;
    } else if (HSFCompare(name,U8H("writeHSF"))) {
        return 11;
    } else if (HSFCompare(name,U8H("readUTF16"))) {
        return 12;
    } else if (HSFCompare(name,U8H("writeUTF16"))) {
        return 13;
    }
    return -1;
}
HalSMVariable* HFP_getVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    if (HSFCompare(name,U8H("SEEK_SET"))) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,0),HalSMVariableType_int);
    } else if (HSFCompare(name,U8H("SEEK_CUR"))) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,1),HalSMVariableType_int);
    } else if (HSFCompare(name,U8H("SEEK_END"))) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,2),HalSMVariableType_int);
    }
    return &hvoid;
}
HalSMVariable* HFP_getFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    if (HSFCompare(name,U8H("close"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[0]->value;
    } else if (HSFCompare(name,U8H("read"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[1]->value;
    } else if (HSFCompare(name,U8H("write"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[2]->value;
    } else if (HSFCompare(name,U8H("eof"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[3]->value;
    } else if (HSFCompare(name,U8H("seek"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[4]->value;
    } else if (HSFCompare(name,U8H("tell"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[5]->value;
    } else if (HSFCompare(name,U8H("rewind"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[6]->value;
    } else if (HSFCompare(name,U8H("flush"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[7]->value;
    } else if (HSFCompare(name,U8H("readUTF8"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[8]->value;
    } else if (HSFCompare(name,U8H("writeUTF8"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[9]->value;
    } else if (HSFCompare(name,U8H("readHSF"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[10]->value;
    } else if (HSFCompare(name,U8H("writeHSF"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[11]->value;
    } else if (HSFCompare(name,U8H("readUTF16"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[12]->value;
    } else if (HSFCompare(name,U8H("writeUTF16"))) {
        return ((HalSMVariableCustom*)v->value)->type->funcs->elements[13]->value;
    }
    return &hvoid;
}

HalSMVariable* HFP_getItem(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_shiftRight(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_shiftLeft(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_convert(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_xor(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_and(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_or(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HFP_bnot(HalSMCompiler* hsmc,HalSMVariable* a){return &hvoid;}

HalSMVariable* HFP_close(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        hsmc->filesystemlibrary->closeFile(((HalSMVariableCustom*)args->arr[0]->value)->value);
    }
    return &hvoid;
}

HalSMVariable* HFP_read(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==1) {
        HalSMInteger* out=HalSMInteger_init(hsmc->memorymanagmentlibrary,0,hsmc->memorymanagmentlibrary->malloc(1),1);
        hsmc->filesystemlibrary->read(out->value,1,1,((HalSMVariableCustom*)args->arr[0]->value)->value);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_int);
    } else if (args->size==2&&args->arr[1]->type==HalSMVariableType_int) {
        HalSMByteArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMByteArray));
        unsigned long long int size=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        out->bytes=hsmc->memorymanagmentlibrary->malloc(size);
        out->size=size;
        hsmc->filesystemlibrary->read(out->bytes,1,size,((HalSMVariableCustom*)args->arr[0]->value)->value);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVariableCustom_init(hsmc->memorymanagmentlibrary,(HalSMVariableTypeCustom*)hsmc->sys_custom_variables->elements[1]->value->value,out,&NULLDict,&NULLDict),HalSMVariableType_custom);
    } else if (args->size==3&&args->arr[1]->type==HalSMVariableType_int&&args->arr[2]->type==HalSMVariableType_int) {
        HalSMByteArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMByteArray));
        unsigned long long int count,size;
        size=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value);
        count=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[2]->value);
        out->bytes=hsmc->memorymanagmentlibrary->malloc(count*size);
        out->size=size;
        hsmc->filesystemlibrary->read(out->bytes,size,count,((HalSMVariableCustom*)args->arr[0]->value)->value);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMVariableCustom_init(hsmc->memorymanagmentlibrary,(HalSMVariableTypeCustom*)hsmc->sys_custom_variables->elements[1]->value->value,out,&NULLDict,&NULLDict),HalSMVariableType_custom);
    }
    return &hvoid;
}

HalSMVariable* HFP_write(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==2) {
        if (args->arr[1]->type==HalSMVariableType_int) {
            HalSMInteger* out=(HalSMInteger*)args->arr[1]->value;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->write(out->value,1,out->size,((HalSMVariableCustom*)args->arr[0]->value)->value)),HalSMVariableType_int);
        } else if (args->arr[1]->type==HalSMVariableType_custom&&HSFCompare(((HalSMVariableCustom*)args->arr[1]->value)->type->name,U8H("ByteArray"))) {
            HalSMByteArray* out=(HalSMByteArray*)args->arr[1]->value;
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->write(out->bytes,1,out->size,((HalSMVariableCustom*)args->arr[0]->value)->value)),HalSMVariableType_int);
        }
    }
    return &hvoid;
}

HalSMVariable* HFP_eof(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==1) {
        int e=hsmc->filesystemlibrary->eof(((HalSMVariableCustom*)args->arr[0]->value)->value);
        HalSMInteger* out;
        if (e<0) {
            out=HalSMInteger_FromSignedInteger(hsmc->memorymanagmentlibrary,(signed int)e);
        } else {
            out=HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,(unsigned int)e);
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_int);
    }
    return &hvoid;
}

HalSMVariable* HFP_seek(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_int) {
        hsmc->filesystemlibrary->seek(((HalSMVariableCustom*)args->arr[0]->value)->value,HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value),0);
    } else if (args->size==3&&args->arr[1]->type==HalSMVariableType_int&&args->arr[2]->type==HalSMVariableType_int) {
        hsmc->filesystemlibrary->seek(((HalSMVariableCustom*)args->arr[0]->value)->value,HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value),HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[2]->value));
    }
    return &hvoid;
}

HalSMVariable* HFP_tell(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==1) {
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->tell(((HalSMVariableCustom*)args->arr[0]->value)->value)),HalSMVariableType_int);
    }
    return &hvoid;
}

HalSMVariable* HFP_rewind(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==1) {
        hsmc->filesystemlibrary->rewind(((HalSMVariableCustom*)args->arr[0]->value)->value);
    }
    return &hvoid;
}

HalSMVariable* HFP_flush(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==1) {
        hsmc->filesystemlibrary->flush(((HalSMVariableCustom*)args->arr[0]->value)->value);
    }
    return &hvoid;
}

HalSMVariable* HFP_readUTF8(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars) {
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_int) {
        unsigned long long int l=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value),i;
        unsigned char bytes[4];
        void* p=((HalSMVariableCustom*)args->arr[0]->value)->value;
        HalStringFormatChar* out=hsmc->memorymanagmentlibrary->malloc(0);
        for (i=0;i<l;i++) {
            hsmc->filesystemlibrary->read(bytes,1,1,p);
            if (hsmc->filesystemlibrary->eof(p)) {break;}
            out=hsmc->memorymanagmentlibrary->realloc(out,i*sizeof(HalStringFormatChar)+sizeof(HalStringFormatChar));
            if (utf8_is_single_byte(bytes)) {
                if (bytes[0]==0) {break;}
                out[i]=U8CH(bytes[0]);
            } else if (utf8_is_double_byte(bytes)) {
                hsmc->filesystemlibrary->read(&bytes[1],1,1,p);
                out[i]=U8CH((bytes[0]<<8)|bytes[1]);
            } else if (utf8_is_triple_byte(bytes)) {
                hsmc->filesystemlibrary->read(&bytes[1],2,1,p);
                out[i]=U8CH(bytes[2]+(bytes[1]<<8)+(bytes[0]<<16));
            } else if (utf8_is_quadruple_byte(bytes)) {
                hsmc->filesystemlibrary->read(&bytes[1],3,1,p);
                out[i]=U8CH(bytes[3]+(bytes[2]<<8)+(bytes[1]<<16)+(bytes[0]<<24));
            }
        }
        out=hsmc->memorymanagmentlibrary->realloc(out,i*sizeof(HalStringFormatChar)+sizeof(HalStringFormatChar));
        out[i]=ASCIIH('\0');
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
    }
    return &hvoid;
}

HalSMVariable* HFP_writeUTF8(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_str) {
        char* str=HU8(*(HalStringFormatChar**)args->arr[1]->value);
        void* p=((HalSMVariableCustom*)args->arr[0]->value)->value;
        unsigned long long int l=0;
        char* pstr;
        for (pstr=str;;pstr++) {
            if (*pstr=='\0'){l=pstr-str;break;}
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->write(str,1,l,p)),HalSMVariableType_int);
    }
    return &hvoid;
}

HalSMVariable* HFP_readHSF(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_int) {
        unsigned long long int l=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value),i;
        unsigned char bytes[4];
        void* p=((HalSMVariableCustom*)args->arr[0]->value)->value;
        HalStringFormatChar* out=hsmc->memorymanagmentlibrary->malloc(0);
        for (i=0;i<l;i++) {
            hsmc->filesystemlibrary->read(bytes,4,1,p);
            if (hsmc->filesystemlibrary->eof(p)){break;}
            if (*(unsigned int*)bytes==0){break;}
            out=hsmc->memorymanagmentlibrary->realloc(out,i*sizeof(HalStringFormatChar)+sizeof(HalStringFormatChar));
            unsigned int hi=*(unsigned int*)bytes;
            out[i]=*(HalStringFormatChar*)&hi;
        }
        out=hsmc->memorymanagmentlibrary->realloc(out,i*sizeof(HalStringFormatChar)+sizeof(HalStringFormatChar));
        out[i]=ASCIIH('\0');
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,out);
    }
    return &hvoid;
}

HalSMVariable* HFP_writeHSF(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_str) {
        HalStringFormatChar* str=*(HalStringFormatChar**)args->arr[1]->value;
        unsigned long long int l=string_len(str),i;
        unsigned int v;
        void* p=((HalSMVariableCustom*)args->arr[0]->value)->value;
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,hsmc->filesystemlibrary->write((unsigned char*)str,4,l,p)),HalSMVariableType_int);
    }
    return &hvoid;
}


HalSMVariable* HFP_readUTF16(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_int) {
        unsigned long long int l=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)args->arr[1]->value),i;
        unsigned short v=0;
        void* p=((HalSMVariableCustom*)args->arr[0]->value)->value;
        short* out=hsmc->memorymanagmentlibrary->malloc(0);
        hsmc->filesystemlibrary->read(&v,2,1,p);
        if (v!=0xfeff){return &hvoid;}
        for (i=0;i<l;i++) {
            hsmc->filesystemlibrary->read(&v,2,1,p);
            if (hsmc->filesystemlibrary->eof(p)){break;}
            if (v==0){break;}
            out=hsmc->memorymanagmentlibrary->realloc(out,i*sizeof(short)+sizeof(short));
            out[i]=v;
        }
        out=hsmc->memorymanagmentlibrary->realloc(out,i*sizeof(short)+sizeof(short));
        out[i]=0x00;
        return HalSMVariable_init_str(hsmc->memorymanagmentlibrary,U16H(out));
    }
    return &hvoid;
}

HalSMVariable* HFP_writeUTF16(HalSMCompiler* hsmc,HalSMArray* args,Dict* vars)
{
    if (args->size==2&&args->arr[1]->type==HalSMVariableType_str) {
        short* str=HU16(*(HalStringFormatChar**)args->arr[1]->value);
        void* p=((HalSMVariableCustom*)args->arr[0]->value)->value;
        unsigned long long int l=0;
        short* pstr;
        for (pstr=str;;pstr++) {
            if (*pstr==0){l=pstr-str;break;}
        }
        short utf16=0xfeff;
        unsigned long long int out=hsmc->filesystemlibrary->write(&utf16,1,2,p)+hsmc->filesystemlibrary->write(str,2,l,p);
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedLongLongInteger(hsmc->memorymanagmentlibrary,out),HalSMVariableType_int);
    }
    return &hvoid;
}

//HalSM Byte Array

HalStringFormatChar* HBA_toString(HalSMCompiler* hsmc,HalSMVariable* v) {
    HalSMByteArray* ba=(HalSMByteArray*)((HalSMVariableCustom*)v->value)->value;
    HalStringFormatChar* join=U8H(", ");
    if(ba->size==0) {
        return U8H("");
    } else if (ba->size==1) {
        return HalSMInteger_toString(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,ba->bytes[0]),0);
    }
    unsigned long long int size;
    unsigned long long int lj=string_len(join);
    HalStringFormatChar* out=hsmc->memorymanagmentlibrary->calloc(2,sizeof(HalStringFormatChar));
    out[0]=ASCIIH('[');
    out[1]=ASCIIH('\0');
    unsigned long long int i;
    size=1;
    for (i=0;i<ba->size;i++) {
        HalStringFormatChar* oc=HalSMInteger_toString(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,ba->bytes[i]),0);
        AdditionStrings(hsmc->memorymanagmentlibrary,&out,oc,size,string_len(oc));
        size+=string_len(oc);
        if (i!=ba->size-1) {
            AdditionStrings(hsmc->memorymanagmentlibrary,&out,join,size,lj);
            size+=lj;
        }
    }
    AdditionStrings(hsmc->memorymanagmentlibrary,&out,U8H("]"),size,1);
    out[size+1]=ASCIIH('\0');
    return out;
}
HalSMVariable* HBA_add(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HBA_sub(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HBA_mul(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HBA_div(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HBA_pow(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HBA_modulo(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return a;}
HalSMVariable* HBA_increment(HalSMCompiler* hsmc,HalSMVariable* v){return v;}
HalSMVariable* HBA_decrement(HalSMCompiler* hsmc,HalSMVariable* v){return v;}
unsigned char HBA_isEqual(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){{return a->value==b->value;}}
unsigned char HBA_isMore(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return 0;}
unsigned char HBA_isLess(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return 0;}
unsigned char HBA_getBool(HalSMCompiler* hsmc,HalSMVariable* v){return 1;}
long long int HBA_indexVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    return -1;
}
long long int HBA_indexFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    return -1;
}
HalSMVariable* HBA_getVariableByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    return &hvoid;
}
HalSMVariable* HBA_getFunctionByName(HalSMCompiler* hsmc,HalSMVariable* v,HalStringFormatChar* name) {
    return &hvoid;
}
HalSMVariable* HBA_getItem(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b) {
    HalSMByteArray* ba=(HalSMByteArray*)((HalSMVariableCustom*)a->value)->value;
    if (b->type==HalSMVariableType_int&&((HalSMInteger*)b->value)->size<9) {
        unsigned long long int ind=HalSMInteger_ToUnsignedLongLongInteger((HalSMInteger*)b->value);
        if (ind<ba->size) {
            return HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,ba->bytes[ind]),HalSMVariableType_int);
        }
    }
    return &hvoid;
}
HalSMVariable* HBA_shiftRight(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HBA_shiftLeft(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HBA_convert(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b) {
    if (b->type==HalSMVariableType_HalSMArray) {
        HalSMByteArray* ba=(HalSMByteArray*)((HalSMVariableCustom*)a->value)->value;
        HalSMArray* out=hsmc->memorymanagmentlibrary->malloc(sizeof(HalSMArray));
        out->size=ba->size;
        out->arr=hsmc->memorymanagmentlibrary->malloc(ba->size*sizeof(HalSMVariable));
        unsigned long long int i;
        for (i=0;i<ba->size;i++) {
            out->arr[i]=HalSMVariable_init(hsmc->memorymanagmentlibrary,HalSMInteger_FromUnsignedInteger(hsmc->memorymanagmentlibrary,ba->bytes[i]),HalSMVariableType_int);
        }
        return HalSMVariable_init(hsmc->memorymanagmentlibrary,out,HalSMVariableType_HalSMArray);
    }
    return &hvoid;
}

HalSMVariable* HBA_xor(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HBA_and(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HBA_or(HalSMCompiler* hsmc,HalSMVariable* a,HalSMVariable* b){return &hvoid;}
HalSMVariable* HBA_bnot(HalSMCompiler* hsmc,HalSMVariable* a){return &hvoid;}