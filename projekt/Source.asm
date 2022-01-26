bits 64
default rel

global addVertex
global addConnection
global printVertices
global mainProgram
global uniqueList
global extractCities
global cleaner
global main
global headVertex
global headConnection

extern fclose                                           
extern feof                                             
extern fscanf                                  
extern exit                                             
extern fopen                                            
extern free                                             
extern strcmp                                           
extern printf                                           
extern strcpy                                           
extern malloc
extern _CRT_INIT


SECTION .text                       

addVertex:
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 32                                 
        mov     qword [rbp-18H], rdi                    
        mov     edi, 120                                
        call    malloc                                  
        mov     qword [rbp-8H], rax                     
        mov     rax, qword [rbp-8H]                     
        mov     rdx, qword [rbp-18H]                    
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcpy                                  
        mov     rax, qword [rbp-8H]                     
        add     rax, 50                                 
; Note: Length-changing prefix causes delay on processors
        mov     word [rax], 32                          
        mov     rax, qword [rbp-8H]                     
        mov     dword [rax+64H], 999999                 
        mov     rax, qword [rbp-8H]                     
        mov     byte [rax+68H], 0                       
        mov     rax, qword [rel headVertex]             
        test    rax, rax                                
        jnz     AddNextVertex                              
        mov     rax, qword [rbp-8H]                     
        mov     qword [rel headVertex], rax             
        mov     rax, qword [rel headVertex]             
        mov     qword [rax+70H], 0                      
        jmp     AddFirstVertex                                

AddNextVertex:  mov     rdx, qword [rel headVertex]             
        mov     rax, qword [rbp-8H]                     
        mov     qword [rax+70H], rdx                    
        mov     rax, qword [rbp-8H]                     
        mov     qword [rel headVertex], rax             
AddFirstVertex:  nop                                          
        leave                                           
        ret                                             
; addVertex End of

addConnection:
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 16                                 
        mov     edi, 112                                
        call    malloc                                  
        mov     qword [rbp-8H], rax                     
        mov     rax, qword [rbp-8H]                     
        lea     rsi, [rbp+10H]                          
        mov     rdi, rax                                
        call    strcpy                                  
        mov     rax, qword [rbp-8H]                     
        lea     rdx, [rax+32H]                          
        lea     rax, [rbp+42H]                          
        mov     rsi, rax                                
        mov     rdi, rdx                                
        call    strcpy                                  
        mov     edx, dword [rbp+74H]                    
        mov     rax, qword [rbp-8H]                     
        mov     dword [rax+64H], edx                    
        mov     rax, qword [rel headConnection]         
        test    rax, rax                                
        jnz     AddNextConnection                                
        mov     rax, qword [rbp-8H]                     
        mov     qword [rel headConnection], rax         
        mov     rax, qword [rel headConnection]         
        mov     qword [rax+68H], 0                                                    
        nop
        leave
        ret

AddNextConnection:  mov     rdx, qword [rel headConnection]         
        mov     rax, qword [rbp-8H]                     
        mov     qword [rax+68H], rdx                    
        mov     rax, qword [rbp-8H]                     
        mov     qword [rel headConnection], rax         
        nop                                          
        leave                                           
        ret                                             

printVertices:; begin
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 16                                 
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-8H], rax                     
        jmp     WhileLoop                                

Print:  mov     rax, qword [rbp-8H]                  
        movzx   eax, byte [rax+68H]                     
        movzx   ecx, al                                 
        mov     rax, qword [rbp-8H]                     
        mov     edx, dword [rax+64H]                    
        mov     rax, qword [rbp-8H]                     
        lea     rsi, [rax+32H]                          
        mov     rax, qword [rbp-8H]                     
        mov     r8d, ecx                                
        mov     ecx, edx                                
        mov     rdx, rsi                                
        mov     rsi, rax                                
        lea     rax, [rel Lable043]                     
        mov     rdi, rax                                
        mov     eax, 0                                  
        call    printf                                  
        mov     rax, qword [rbp-8H]                     
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-8H], rax                     
WhileLoop:  cmp     qword [rbp-8H], 0                    
        jnz     Print                                
        nop                                             
        nop                                             
        leave                                           
        ret    

mainProgram:; begin
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 192          rdi                   
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-10H], rax                    
        jmp     HeadVertex                                
        
CompareVertex:  mov     rax, qword [rbp-10H]                 
        mov     rdx, qword [rbp-0B8H]                   
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jnz     Breaking                                
        mov     rax, qword [rbp-10H]                    
        mov     dword [rax+64H], 0                      
        jmp     MainIter                                

Breaking:  mov     rax, qword [rbp-10H]                 
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-10H], rax                    
HeadVertex:  cmp     qword [rbp-10H], 0                   
        jnz     CompareVertex                                
MainIter:  mov     rax, qword [rel headVertex]          
        mov     qword [rbp-18H], rax                    
        jmp     EndMain                                

InnerVertex:  lea     rax, [rbp-70H]                       

        mov     word [rax], 32                          
        mov     dword [rbp-4H], 999999                  
        lea     rax, [rbp-0B0H]                         
; Note: Length-changing prefix causes delay on processors
        mov     word [rax], 32                          
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-20H], rax                    
        jmp     YetAnotherInnerVertex                                

AnotherInnerVertex:  mov     rax, qword [rbp-20H]                 
        movzx   eax, byte [rax+68H]                     
        xor     eax, 01H                                
        test    al, al                                  
        jz      WeightsCalc                                
        mov     rax, qword [rbp-20H]                    
        mov     eax, dword [rax+64H]                    
        cmp     dword [rbp-4H], eax                     
        jle     WeightsCalc                                
        mov     rax, qword [rbp-20H]                    
        mov     eax, dword [rax+64H]                    
        mov     dword [rbp-4H], eax                     
        mov     rdx, qword [rbp-20H]                    
        lea     rax, [rbp-70H]                          
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcpy                                  
WeightsCalc:  mov     rax, qword [rbp-20H]                 
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-20H], rax                    
YetAnotherInnerVertex:  cmp     qword [rbp-20H], 0                   
        jnz     AnotherInnerVertex                                
        lea     rax, [rbp-70H]                          
        lea     rdx, [rel Lable044]                     
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jz      MinimalDistance                                
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-28H], rax                    
        jmp     DistanceMeasure                                

Connection:  mov     rax, qword [rbp-28H]                 
        lea     rdx, [rbp-70H]                          
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jnz     Inspection                                
        mov     rax, qword [rbp-28H]                    
        mov     byte [rax+68H], 1                       
        jmp     MinimalDistance                                   

Inspection:  mov     rax, qword [rbp-28H]                    
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-28H], rax                    
DistanceMeasure:  cmp     qword [rbp-28H], 0                   
        jnz     Connection                                
MinimalDistance:  mov     rax, qword [rel headConnection]         
        mov     qword [rbp-30H], rax                    
        jmp     FinVertex                                

AddVertices:  mov     rax, qword [rbp-30H]                    
        lea     rdx, [rbp-70H]                          
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jz      MoveVertice                                   
        mov     rax, qword [rbp-30H]                    
        lea     rdx, [rax+32H]                          
        lea     rax, [rbp-70H]                          
        mov     rsi, rax                                
        mov     rdi, rdx                                
        call    strcmp                                  
        test    eax, eax                                
        jne     AddCloseRelation                                
MoveVertice:  mov     rax, qword [rbp-30H]                 
        lea     rdx, [rbp-70H]                          
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jnz     MakeConnection                                
        mov     rax, qword [rbp-30H]                    
        lea     rdx, [rax+32H]                          
        lea     rax, [rbp-0B0H]                         
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcpy                                  
MakeConnection:  mov     rax, qword [rbp-30H]                 
        lea     rdx, [rax+32H]                          
        lea     rax, [rbp-70H]                          
        mov     rsi, rax                                
        mov     rdi, rdx                                
        call    strcmp                                  
        test    eax, eax                                
        jnz     GetAnotherConnection                                
        mov     rdx, qword [rbp-30H]                    
        lea     rax, [rbp-0B0H]                         
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcpy                                  
GetAnotherConnection:  mov     rax, qword [rel headVertex]          
        mov     qword [rbp-38H], rax                    
        jmp     Shortcut                                

CopyNames:  mov     rax, qword [rbp-38H]                 
        lea     rdx, [rbp-0B0H]                         
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jnz     SearchPath                                
        mov     rax, qword [rbp-38H]                    
        mov     edx, dword [rax+64H]                    
        mov     rax, qword [rbp-30H]                    
        mov     ecx, dword [rax+64H]                    
        mov     eax, dword [rbp-4H]                     
        add     eax, ecx                                
        cmp     edx, eax                                
        jle     SearchPath                                
        mov     rax, qword [rbp-38H]                    
        lea     rdx, [rax+32H]                          
        lea     rax, [rbp-70H]                          
        mov     rsi, rax                                
        mov     rdi, rdx                                
        call    strcpy                                  
        mov     rax, qword [rbp-30H]                    
        mov     edx, dword [rax+64H]                    
        mov     eax, dword [rbp-4H]                     
        add     edx, eax                                
        mov     rax, qword [rbp-38H]                    
        mov     dword [rax+64H], edx                    
        jmp     AddCloseRelation                                

SearchPath:  mov     rax, qword [rbp-38H]                 
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-38H], rax                    
Shortcut:  cmp     qword [rbp-38H], 0                   
        jnz     CopyNames                                
AddCloseRelation:  mov     rax, qword [rbp-30H]                 
        mov     rax, qword [rax+68H]                    
        mov     qword [rbp-30H], rax                    
FinVertex:  cmp     qword [rbp-30H], 0                   
        jne     AddVertices                                
        mov     rax, qword [rbp-18H]                    
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-18H], rax                    
EndMain:  cmp     qword [rbp-18H], 0                   
        jne     InnerVertex                                
        nop                                             
        nop                                             
        leave                                           
        ret                                             
; mainProgram End of 

uniqueList:
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 32                                 
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-8H], rax                     
        jmp     Lable033                                 

AssignVertex:  mov     rax, qword [rbp-8H]                   
        mov     qword [rbp-10H], rax                    
        jmp     Lable032                                

FreeMemory:  mov     rax, qword [rbp-10H]                 
        mov     rax, qword [rax+70H]                    
        mov     rdx, rax                                
        mov     rax, qword [rbp-8H]                     
        mov     rsi, rdx                                
        mov     rdi, rax                                
        call    strcmp                                  
        test    eax, eax                                
        jnz     Lable031                                
        mov     rax, qword [rbp-10H]                    
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-18H], rax                    
        mov     rax, qword [rbp-10H]                    
        mov     rax, qword [rax+70H]                    
        mov     rdx, qword [rax+70H]                    
        mov     rax, qword [rbp-10H]                    
        mov     qword [rax+70H], rdx                    
        mov     rax, qword [rbp-18H]                    
        mov     rdi, rax                                
        call    free                                    
        mov     qword [rbp-18H], 0                      
        jmp     Lable032                                

Lable031:  mov     rax, qword [rbp-10H]                 
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-10H], rax                    
Lable032:  mov     rax, qword [rbp-10H]                 
        mov     rax, qword [rax+70H]                    
        test    rax, rax                                
        jnz     FreeMemory                                
        mov     rax, qword [rbp-8H]                     
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-8H], rax                     
Lable033:  cmp     qword [rbp-8H], 0                    
        jz      Lable034                                
        mov     rax, qword [rbp-8H]                     
        mov     rax, qword [rax+70H]                    
        test    rax, rax                                
        jne     AssignVertex                                
Lable034:  nop                                          
        leave                                           
        ret                                             
; uniqueList End of

extractCities:;  begin
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 16                                 
        mov     rax, qword [rel headConnection]         
        mov     qword [rbp-8H], rax                     
        jmp     Lable036                                

Lable035:  mov     rax, qword [rbp-8H]                  
        mov     rdi, rax                                
        call    addVertex                               
        mov     rax, qword [rbp-8H]                     
        add     rax, 50                                 
        mov     rdi, rax                                
        call    addVertex                               
        mov     rax, qword [rbp-8H]                     
        mov     rax, qword [rax+68H]                    
        mov     qword [rbp-8H], rax                     
Lable036:  cmp     qword [rbp-8H], 0                    
        jnz     Lable035                                
        nop                                             
        nop                                             
        leave                                           
        ret                                             
; extractCities End of 

cleaner:; begin
        push    rbp                                     
        mov     rbp, rsp                                
        sub     rsp, 32                                 
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-8H], rax                     
        mov     rax, qword [rel headVertex]             
        mov     qword [rbp-18H], rax                    
        jmp     Lable038                                

Lable037:  mov     rax, qword [rbp-8H]                  
        mov     rax, qword [rax+70H]                    
        mov     qword [rbp-18H], rax                    
        mov     rax, qword [rbp-8H]                     
        mov     rdi, rax                                
        call    free                                    
        mov     rax, qword [rbp-18H]                    
        mov     qword [rbp-8H], rax                     
Lable038:  cmp     qword [rbp-8H], 0                    
        jnz     Lable037                                
        mov     qword [rel headVertex], 0               
        mov     rax, qword [rel headVertex]             
        mov     rdi, rax                                
        call    free                                    
        mov     rax, qword [rel headConnection]         
        mov     qword [rbp-10H], rax                    
        mov     rax, qword [rel headConnection]         
        mov     qword [rbp-20H], rax                    
        jmp     Lable040                                

Lable039:  mov     rax, qword [rbp-10H]                 
        mov     rax, qword [rax+68H]                    
        mov     qword [rbp-20H], rax                    
        mov     rax, qword [rbp-10H]                    
        mov     rdi, rax                                
        call    free                                    
        mov     rax, qword [rbp-20H]                    
        mov     qword [rbp-10H], rax                    
Lable040:  cmp     qword [rbp-10H], 0                   
        jnz     Lable039                                
        mov     qword [rel headConnection], 0           
        mov     rax, qword [rel headConnection]         
        mov     rdi, rax                                
        call    free                                    
        nop                                             
        leave                                           
        ret                                             
; cleaner End of 

main:
        push    rbp                                     
        mov     rbp, rsp                                
        add     rsp, -128
        
        ; important!!
        call    _CRT_INIT                              
        lea     rax, [rel Lable045]                     
        mov     rsi, rax                                
        lea     rax, [rel Lable046]                     
        mov     rdi, rax                                
        call    fopen                                   
        mov     qword [rbp-8H], rax                     
        cmp     qword [rbp-8H], 0                       
        jne     Lable042                                   
        lea     rax, [rel Lable047]                        
        mov     rdi, rax                                
        mov     eax, 0                                  
        call    printf                                  
        mov     edi, 1                                  
        call    exit                                    
Lable041:  lea     rax, [rbp-80H]                       
        lea     rsi, [rax+64H]                          
        lea     rax, [rbp-80H]                          
        lea     rcx, [rax+32H]                          
        lea     rdx, [rbp-80H]                          
        mov     rax, qword [rbp-8H]                     
        mov     r8, rsi                                 
        lea     rsi, [rel Lable048]                     
        mov     rdi, rax                                
        mov     eax, 0                                  
        call    fscanf                         
        push    qword [rbp-18H]                         
        push    qword [rbp-20H]                         
        push    qword [rbp-28H]                         
        push    qword [rbp-30H]                         
        push    qword [rbp-38H]                         
        push    qword [rbp-40H]                         
        push    qword [rbp-48H]                         
        push    qword [rbp-50H]                         
        push    qword [rbp-58H]                         
        push    qword [rbp-60H]                         
        push    qword [rbp-68H]                         
        push    qword [rbp-70H]                         
        push    qword [rbp-78H]                         
        push    qword [rbp-80H]                         
        call    addConnection                           
        add     rsp, 112                                
Lable042:  mov     rax, qword [rbp-8H]                  
        mov     rdi, rax                                
        call    feof                                    
        test    eax, eax                                
        jz      Lable041                                
        mov     rax, qword [rbp-8H]                     
        mov     rdi, rax                                
        call    fclose                                  
        mov     eax, 0                                  
        call    extractCities                           
        mov     eax, 0                                  
        call    uniqueList                              
        lea     rax, [rel Lable049]                     
        mov     rdi, rax                                
        call    mainProgram                             
        mov     eax, 0                                  
        call    printVertices                           
        mov     eax, 0                                  
        call    cleaner                                 
        mov     eax, 0                                  
        leave                                           
        ret                                             
; main End of 


SECTION .data                          ; section number 2, data


SECTION .bss                           ; section number 3, bss

headVertex:                                             ; qword
        resq    1                                       ; 0000

headConnection:                                         ; qword
        resq    1                                       ; 0008


SECTION .rodata                        ; section number 4, const

Lable043:                                                  
        db "%s - %s - %d - %d", 0AH, 00H                            

Lable044:                                                  
        db 20H, 00H                                    

Lable045:                                                  
        db "rb", 00H                                

Lable046:                                                  
        db "test.txt", 00H                                          

Lable047:                                                  
        db "Error!", 00H            

Lable048:                                                  
        db "%s %s %d", 0AH, 00H         
Lable049:                                                  
        db "Bydgoszcz", 00H                                     


