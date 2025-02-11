#include <HalSM.h>
#include <stdio.h>


void HalSMMemory_init(HalSMMemory* out,HalSMMemoryManagmentLibrary* mml) {
    out->free=0;
    out->size=0;
    out->table=(HalSMMemoryCell*)mml->malloc(0);
    out->mml=mml;
    out->firstFree=NOTHALSMNULLPOINTER;
}

void HalSMMemory_setCell(HalSMMemory* memory,ui64 nc,void* addr) {
    if(nc==HSMEM_ERR_ADDR)return;
    //printf("MEMORY SET CELL: %p;%lld START\n",addr,nc);
    memory->table[nc]=(HalSMMemoryCell){
        .addr=addr,
        .isFree=0,
        .nextFree=NOTHALSMNULLPOINTER,
        .prevFree=NOTHALSMNULLPOINTER
    };
    
    //printf("MEMORY SET CELL: %p;%lld END\n",addr,nc);
}

void HalSMMemory_setFreeCell(HalSMMemory* memory,void* addr) {
    HalSMMemoryCell* z;
    z=memory->firstFree;
    if(memory->firstFree==z) {
        if(z->nextFree!=NOTHALSMNULLPOINTER) {
            memory->firstFree=z->nextFree;
        } else {
            memory->firstFree=NOTHALSMNULLPOINTER;
        }
    }

    if(z->prevFree!=NOTHALSMNULLPOINTER&&z->nextFree!=NOTHALSMNULLPOINTER) {
        z->prevFree->nextFree=z->nextFree;
        z->nextFree->prevFree=z->prevFree;
    }
    
    //printf("MEMORY SET FREECELL: %p START\n",addr);
    z->isFree=0;
    z->addr=addr;
    z->nextFree=NOTHALSMNULLPOINTER;
    z->prevFree=NOTHALSMNULLPOINTER;
    memory->free--;
    //printf("MEMORY SET FREECELL: %p END\n",addr);
}

void HalSMMemory_addNewCell(HalSMMemory* memory,void* addr) {
    if(memory->firstFree!=NOTHALSMNULLPOINTER) {
        HalSMMemory_setFreeCell(memory,addr);
        return;
    }
    //printf("MEMORY ADD CELL: %p START\n",addr);
    memory->table=memory->mml->realloc(memory->table,sizeof(HalSMMemoryCell)+sizeof(HalSMMemoryCell)*memory->size);
    memory->table[memory->size++]=(HalSMMemoryCell){
        .addr=addr,
        .isFree=0,
        .nextFree=NOTHALSMNULLPOINTER,
        .prevFree=NOTHALSMNULLPOINTER
    };
    //printf("MEMORY ADD CELL: %p END\n",addr);
}

ui64 HalSMMemory_getNCByAddr(HalSMMemory* memory,void* addr) {
    ui64 i;
    HalSMMemoryCell* p,cell;
    p=memory->table;
    for(i=0;i<memory->size;i++,p++) {
        cell=*p;
        if(cell.addr==addr)return i;
    }
    return HSMEM_ERR_ADDR;
}

void HalSMMemory_freeCell(HalSMMemory* memory,ui64 nc) {
    if(nc==HSMEM_ERR_ADDR)return;
    HalSMMemoryCell* z;
    z=&memory->table[nc];
    //printf("MEMORY FREE CELL: %lld START\n",nc);
    z->addr=NOTHALSMNULLPOINTER;
    z->nextFree=NOTHALSMNULLPOINTER;
    z->isFree=1;
    memory->free++;
    
    if(memory->firstFree==NOTHALSMNULLPOINTER){
        z->prevFree=NOTHALSMNULLPOINTER;
        memory->firstFree=&memory->table[nc];
        return;
    }
    HalSMMemoryCell* cell=memory->firstFree;
    while(cell->nextFree!=NOTHALSMNULLPOINTER) {
        cell=cell->nextFree;
    }
    z->prevFree=cell;
    
    //printf("MEMORY FREE CELL: %lld END\n",nc);
}

void HalSMMemory_freeSys(HalSMMemory* memory) {
    ui64 i;
    HalSMMemoryCell* p,cell;
    p=memory->table;
    for(i=0;i<memory->size;i++,p++) {
        cell=*p;
        if(cell.isFree==0){
            memory->mml->free(cell.addr);
        }
    }
    memory->mml->free(memory->table);
    memory->table=memory->mml->malloc(0);
    memory->size=0;
    memory->free=0;
    memory->firstFree=NOTHALSMNULLPOINTER;
}

void* HalSMMemory_malloc(HalSMMemory* memory,ui64 size) {
    void* p=memory->mml->malloc(size);
    //printf("malloc %lld:%p\n",size,p);
    HalSMMemory_addNewCell(memory,p);
    return p;
}

void* HalSMMemory_realloc(HalSMMemory* memory,void* p,ui64 newsize) {
    ui64 nc=HalSMMemory_getNCByAddr(memory,p);
    void* o=memory->mml->realloc(p,newsize);
    //printf("realloc %lld:%p\n",newsize,o);
    HalSMMemory_setCell(memory,nc,o);
    return o;
}

void HalSMMemory_free(HalSMMemory* memory,void* p) {
    //printf("free %p\n",p);
    HalSMMemory_freeCell(memory,HalSMMemory_getNCByAddr(memory,p));
    return memory->mml->free(p);
}

void* HalSMMemory_calloc(HalSMMemory* memory,ui64 count,ui64 size) {
    void* p=memory->mml->calloc(count,size);
    //printf("calloc %lld;%lld:%p\n",count,size,p);
    HalSMMemory_addNewCell(memory,p);
    return p;
}