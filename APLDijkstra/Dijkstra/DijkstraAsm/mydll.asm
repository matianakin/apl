; mydll.asm

ListOfNodes struct
	next dq ?
	prev dq ?
	node db 256
	prevNode db 256
	distance dq ?
	visited dd ?
ListOfNodes ends

ListOfConnections struct
	next dq ?
	prev dq ?
	node1 db 256
	node2 db 256
	distance dq ?
ListOfConnections ends

.data
extern strcmp: PROC
extern strcpy: PROC
extern exit: PROC

extern printf: PROC

thisis db "STARTING NODE",0
incorrect db "Incorrect node provided as a start point",0
sformat db "%s",0

.code
; void inLoop(struct ListOfNodes** headNode, struct ListOfNodes* current, struct ListOfConnections* connectionIterator, char* nameNode)
inLoop PROC
	mov         qword ptr [rsp+32],r9	; nameNode
	mov         qword ptr [rsp+24],r8	; connectionIterator
	mov         qword ptr [rsp+16],rdx	; current
	mov         qword ptr [rsp+8],rcx	; headNode
	push        rbp  
	push        rdi  
	sub         rsp,128h  
	lea         rbp,[rsp+20h]  
	;lea         rcx,[00007FF71B035017h]  
	;call        00007FF71B0213D4  
	mov         rax,qword ptr [rbp+0000000000000120h]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+8],rax ; analyzedNode = *headNode 
	mov         rax,qword ptr [rbp+0000000000000128h]  
	mov         eax,dword ptr [rax+0000000000000210h]  
	mov         rcx,qword ptr [rbp+0000000000000130h]  
	add         eax,dword ptr [rcx+0000000000000210h]  
	mov         dword ptr [rbp+24h],eax  
inLoop59:
	cmp         qword ptr [rbp+8],0  
	je          inLoop8A  
	mov         rax,qword ptr [rbp+8]  
	add         rax,ListOfNodes.node
	mov         rdx,qword ptr [rbp+0000000000000138h]  
	mov         rcx,rax  
	call        strcmp  
	test        eax,eax  
	jne         inLoop7D  
	jmp         inLoop8A  
inLoop7D:
	mov         rax,qword ptr [rbp+8]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+8],rax  
	jmp         inLoop59 
inLoop8A:
	mov         rax,qword ptr [rbp+8]  
	movzx       eax,byte ptr [rax+0000000000000214h]  
	test        eax,eax  
	jne         inLoop0D3  
	mov         rax,qword ptr [rbp+8]  
	mov         ecx,dword ptr [rbp+24h]  
	cmp         dword ptr [rax+0000000000000210h],ecx  
	jle         inLoop0D3  
	mov         rax,qword ptr [rbp+0000000000000128h]  
	add         rax,ListOfNodes.node
	mov         rcx,qword ptr [rbp+8]  
	add         rcx,110h  
	mov         rdx,rax  
	call        strcpy  
	mov         rax,qword ptr [rbp+8]  
	mov         ecx,dword ptr [rbp+24h]  
	mov         dword ptr [rax+0000000000000210h],ecx
inLoop0D3:
	lea         rsp,[rbp+0000000000000108h]  
	pop         rdi  
	pop         rbp  
	ret  
inLoop ENDP

; void linearVersion(struct ListOfNodes** headNode, struct ListOfConnections** headConnection, char* start)
linearVersion PROC
	mov         qword ptr [rsp+24],r8  ; start
	mov         qword ptr [rsp+16],rdx ; headConnection
	mov         qword ptr [rsp+8],rcx  ; headNode
	push        rbp  
	push        rdi 
	sub         rsp,1A8h  
	lea         rbp,[rsp+32]  
	mov         rax,qword ptr [rbp+00000000000001A0h]
	mov         rax,qword ptr [rax]
	mov         qword ptr [rbp+8],rax ; startNode = *headNode
	mov         byte ptr [rbp+36],0  ; finished = false
linearVersion3B:
	cmp         qword ptr [rbp+8],0 ; while (startNode)
	je          EndWhile
	mov         rax,qword ptr [rbp+8]  
	add         rax,16  
	mov         rdx,qword ptr [rbp+00000000000001B0h]  
	mov			rcx,rax
	;mov         rcx,ListOfNodes.node[rax]
	call        strcmp  
	test        eax,eax  ; if (strcmp(startNode->node, start) == 0)
	jne         linearVersion95  
	mov         rax,qword ptr [rbp+8]  
	mov         dword ptr [rax+0000000000000210h],0  
	mov         rax,qword ptr [rbp+8]  
	mov         byte ptr [rax+0000000000000214h],1  
	mov         rax,qword ptr [rbp+8]  
	add         rax,110h  
	lea         rdx,thisis
	mov         rcx,rax  
	call        strcpy  
	jmp         EndWhile  ; break
linearVersion95:
	mov         rax,qword ptr [rbp+8]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+8],rax  
	cmp         qword ptr [rbp+8],0  
	jne         linearVersion0C5
	lea         rdx,incorrect
	lea         rcx,sformat
	call        printf  
	mov         ecx,1  
	call        exit  ;qword ptr [__imp_exit (07FF746D23308h)]  
linearVersion0C5:
	jmp         linearVersion3B
EndWhile:
	mov         rax,qword ptr [rbp+8]  
	mov         qword ptr [rbp+48h],rax  
linearVersion0D2:
	mov         byte ptr [rbp+24h],1  
	mov         rax,qword ptr [rbp+00000000000001A8h]  ; headConnection
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+68h],rax  
linearVersion0E4:
	cmp         qword ptr [rbp+68h],0  
	je          linearVersion17C  
	mov         rax,qword ptr [rbp+48h]  
	add         rax,10h  
	mov         rcx,qword ptr [rbp+68h]  
	add         rcx,10h  
	mov         rdx,rax  
	call        strcmp  
	test        eax,eax  
	jne         linearVersion12E  
	mov         rax,qword ptr [rbp+68h]  
	add         rax,110h  
	mov         r9,rax  
	mov         r8,qword ptr [rbp+68h]  
	mov         rdx,qword ptr [rbp+48h]  
	mov         rcx,qword ptr [rbp+00000000000001A0h]  
	call        inLoop  
	jmp         linearVersion16C  
linearVersion12E:
	mov         rax,qword ptr [rbp+48h]  
	add         rax,10h  
	mov         rcx,qword ptr [rbp+68h]  
	add         rcx,110h  
	mov         rdx,rax  
	call        strcmp  
	test        eax,eax  
	jne         linearVersion16C  
	mov         rax,qword ptr [rbp+68h]  
	add         rax,10h  
	mov         r9,rax  
	mov         r8,qword ptr [rbp+68h]  
	mov         rdx,qword ptr [rbp+48h]  
	mov         rcx,qword ptr [rbp+00000000000001A0h]  
	call        inLoop  
linearVersion16C:
	mov         rax,qword ptr [rbp+68h]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+68h],rax  
	jmp         linearVersion0E4  
linearVersion17C:
	mov         dword ptr [rbp+0000000000000084h],7FFFFFFFh  
	mov         rax,qword ptr [rbp+00000000000001A0h]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+00000000000000A8h],rax  
linearVersion197:
	cmp         qword ptr [rbp+00000000000000A8h],0  
	je          linearVersion1FD  
	mov         rax,qword ptr [rbp+00000000000000A8h]  
	movzx       eax,byte ptr [rax+0000000000000214h]  
	test        eax,eax  
	jne         linearVersion1EA  
	mov         rax,qword ptr [rbp+00000000000000A8h]  
	mov         ecx,dword ptr [rbp+0000000000000084h]  
	cmp         dword ptr [rax+0000000000000210h],ecx  
	jge         linearVersion1EA  
	mov         rax,qword ptr [rbp+00000000000000A8h]  
	mov         qword ptr [rbp+48h],rax  
	mov         rax,qword ptr [rbp+00000000000000A8h]  
	mov         eax,dword ptr [rax+0000000000000210h]  
	mov         dword ptr [rbp+0000000000000084h],eax  
	mov         byte ptr [rbp+24h],0  
linearVersion1EA:
	mov         rax,qword ptr [rbp+00000000000000A8h]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr [rbp+00000000000000A8h],rax  
	jmp         linearVersion197  
linearVersion1FD:
	mov         rax,qword ptr [rbp+48h]  
	mov         byte ptr [rax+0000000000000214h],1  
	movzx       eax,byte ptr [rbp+24h]  
	test        eax,eax  
	je          linearVersion0D2  
	lea         rsp,[rbp+0000000000000188h]  
	pop         rdi  
	pop         rbp  
	ret  
linearVersion ENDP

END