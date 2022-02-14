; mydll.asm

ListOfNodes STRUCT
	next QWORD ?
	prev QWORD ?
	node BYTE 256
	prevNode BYTE 256
	distance QWORD ?
	visited	DWORD ?
ListOfNodes ENDS

ListOfConnections STRUCT
	next QWORD ?
	prev QWORD ?
	node1 BYTE 256
	node2 BYTE 256
	distance QWORD ?
ListOfConnections ENDS

.data
szMarker db "STARTING NODE",0
szErrorMsg db "Incorrect node provided as a start point",0
szFormat db "%s",0

EXTERN exit: PROC
EXTERN printf: PROC
EXTERN strcmp: PROC
EXTERN strcpy: PROC

.code
; void inLoop(struct ListOfNodes** headNode, struct ListOfNodes* current, struct ListOfConnections* connectionIterator, char* nameNode)
inLoop PROC
	LOCAL analyzedNode: QWORD
	LOCAL road: QWORD

	;     stack layout
	; ======================
	; [ road			   ] v-- local variables --v
	; [ analyzedNode	   ]
	; [					   ] <-- curr stack frame
	; [ ret addr		   ] <-- inLoop ret addr
	; [ headNode		   ] v-- func arguments --v
	; [ current			   ] 
	; [ connectionIterator ]
	; [ nameNode		   ]
	; [					   ] <-- old stack frame	

	; prologue
	mov         qword ptr [rbp+16],rcx	; headNode
	mov         qword ptr [rbp+24],rdx	; current
	mov         qword ptr [rbp+32],r8	; connectionIterator
	mov         qword ptr [rbp+40],r9	; nameNode

	; analyzedNode = *headNode
	mov         rax,qword ptr [rbp+16]
	mov         rax,qword ptr [rax]  
	mov         qword ptr analyzedNode,rax

	; road = current->distance + connectionIterator->distance
	mov         rax,qword ptr [rbp+24]
	mov         eax,dword ptr [rax+528]  
	mov         rcx,qword ptr [rbp+32]  
	add         eax,dword ptr [rcx+528]  
	mov         dword ptr road,eax  

	; while (analyzedNode)
WhileCondition:
	cmp         qword ptr analyzedNode,0  
	je          WhileLeave  

	; if (strcmp(analyzedNode->node, nameNode) == 0)
	mov         rax,qword ptr analyzedNode
	add         rax,ListOfNodes.node
	mov         rdx,qword ptr [rbp+40]  
	mov         rcx,rax  
	call        strcmp  
	test        eax,eax  
	je          WhileLeave

	; analyzedNode = analyzedNode->next
	mov         rax,qword ptr analyzedNode
	mov         rax,qword ptr [rax]  
	mov         qword ptr analyzedNode,rax  
	jmp         WhileCondition 

WhileLeave:
	; if (!analyzedNode->visited && analyzedNode->distance > road)
	mov         rax,qword ptr analyzedNode
	movzx       eax,byte ptr [rax+532]
	test        eax,eax  
	jne         FuncEnd  
	mov         rax,qword ptr analyzedNode 
	mov         ecx,dword ptr road  
	cmp         dword ptr [rax+528],ecx  
	jle         FuncEnd  

	; strcpy(analyzedNode->prevNode, current->node)
	mov         rax,qword ptr [rbp+24]  
	add         rax,ListOfNodes.node
	mov         rcx,qword ptr analyzedNode 
	add         rcx,272
	mov         rdx,rax  
	call        strcpy

	; analyzedNode->distance = road
	mov         rax,qword ptr analyzedNode 
	mov         ecx,dword ptr road
	mov         dword ptr [rax+528],ecx

FuncEnd:
	ret  
inLoop ENDP

; void linearVersion(struct ListOfNodes** headNode, struct ListOfConnections** headConnection, char* start)
linearVersion PROC
	LOCAL startNode: QWORD
	LOCAL finished: QWORD
	LOCAL current: QWORD
	LOCAL connectionIterator: QWORD
	LOCAL tempNode: QWORD
	LOCAL tempDist: QWORD

	;     stack layout
	; ======================
	; [ current			   ] v-- local variables --v
	; [ finished		   ]
	; [ startNode		   ]
	; [					   ] <-- curr stack frame
	; [ ret addr		   ] <-- inLoop ret addr
	; [ headNode		   ] v-- func arguments --v
	; [ headConnection	   ] 
	; [ start			   ]
	; [					   ] <-- old stack frame	

	; prologue
	mov         qword ptr [rbp+32],r8  ; start
	mov         qword ptr [rbp+24],rdx ; headConnection
	mov         qword ptr [rbp+16],rcx ; headNode

	; startNode = *headNode
	mov         rax,qword ptr [rbp+16]
	mov         rax,qword ptr [rax]
	mov         qword ptr startNode,rax

	; finished = false
	mov         byte ptr finished,0

	; while (startNode)
While1Condition:
	cmp         qword ptr startNode,0
	je          While1Leave

	; if (strcmp(startNode->node, start) == 0)
	mov         rax,qword ptr startNode
	add         rax,16  
	mov         rdx,qword ptr [rbp+32]  
	mov			rcx,rax
	call        strcmp  
	test        eax,eax
	jne         While1AdvanceToNextNode

	; startNode->distance = 0
	mov         rax,qword ptr startNode
	mov         dword ptr [rax+528],0

	; startNode->visited = true
	mov         rax,qword ptr startNode
	mov         byte ptr [rax+532],1

	; strcpy(startNode->prevNode, "<---  This is the starting vector")
	mov         rax,qword ptr startNode
	add         rax,110h  
	lea         rdx,szMarker
	mov         rcx,rax  
	call        strcpy
	
	; break
	jmp         While1Leave  ; break

While1AdvanceToNextNode:
	; startNode = startNode->next
	mov         rax,qword ptr startNode  
	mov         rax,qword ptr [rax]  
	mov         qword ptr startNode,rax

	; if (!startNode)
	cmp         qword ptr startNode,0  
	jne         While1Continue

	; printf("%s", "Incorrect node provided as a start point\n")
	lea         rdx,szErrorMsg
	lea         rcx,szFormat
	call        printf

	; exit(1)
	mov         ecx,1  
	call        exit

While1Continue:
	jmp         While1Condition

While1Leave:
	; current = startNode
	mov         rax,qword ptr startNode  
	mov         qword ptr current,rax  

	; do {

DoWhile1:
	; finished = true
	mov         byte ptr finished,1

	; connectionIterator = *headConnection
	mov         rax,qword ptr [rbp+24]
	mov         rax,qword ptr [rax]  
	mov         qword ptr connectionIterator,rax
	
	; while (connectionIterator)

While2Condition:
	cmp         qword ptr connectionIterator,0  
	je          DoWhile1Continue1

	; if(strcmp(connectionIterator->node1, current->node) == 0)
	mov         rax,qword ptr current
	add         rax,10h  
	mov         rcx,qword ptr connectionIterator 
	add         rcx,10h  
	mov         rdx,rax  
	call        strcmp  
	test        eax,eax  
	jne         While2ElseIf

	; inLoop(headNode, current, connectionIterator, connectionIterator->node2)
	mov         rax,qword ptr connectionIterator
	add         rax,110h  
	mov         r9,rax
	mov         r8,qword ptr connectionIterator  
	mov         rdx,qword ptr current
	mov         rcx,qword ptr [rbp+16]
	sub			rsp,32
	call        inLoop
	
	; end of this while
	jmp         While2Continue  

	; else if(strcmp(connectionIterator->node2, current->node) == 0)
While2ElseIf:
	mov         rax,qword ptr current  
	add         rax,10h  
	mov         rcx,qword ptr connectionIterator
	add         rcx,110h  
	mov         rdx,rax  
	call        strcmp  
	test        eax,eax  
	jne         While2Continue

	; inLoop(headNode, current, connectionIterator, connectionIterator->node1)
	mov         rax,qword ptr connectionIterator 
	add         rax,10h  
	mov         r9,rax  
	mov         r8,qword ptr connectionIterator
	mov         rdx,qword ptr current  
	mov         rcx,qword ptr [rbp+16]
	sub			rsp,32
	call        inLoop

While2Continue:
	; connectionIterator = connectionIterator->next
	mov         rax,qword ptr connectionIterator
	mov         rax,qword ptr [rax]
	mov         qword ptr connectionIterator,rax  
	jmp         While2Condition

DoWhile1Continue1:
	; tempDist = INT_MAX
	mov         dword ptr tempDist,7FFFFFFFh

	; tempNode = *headNode
	mov         rax,qword ptr [rbp+16]  
	mov         rax,qword ptr [rax]  
	mov         qword ptr tempNode,rax

	; while (tempNode)
While3Condition:
	cmp         qword ptr tempNode,0  
	je          DoWhile1Continue2  

	; if (!tempNode->visited && tempNode->distance < tempDist)
	mov         rax,qword ptr tempNode
	movzx       eax,byte ptr [rax+532]
	test        eax,eax  
	jne         While3AdvanceToTheNextNode  
	mov         rax,qword ptr tempNode  
	mov         ecx,dword ptr tempDist  
	cmp         dword ptr [rax+528],ecx  
	jge         While3AdvanceToTheNextNode  

	; current = tempNode
	mov         rax,qword ptr tempNode 
	mov         qword ptr current,rax

	; tempDist = tempNode->distance
	mov         rax,qword ptr tempNode
	mov         eax,dword ptr [rax+528]  
	mov         dword ptr tempDist,eax
	
	; finished = false
	mov         byte ptr finished,0

While3AdvanceToTheNextNode:
	; tempNode = tempNode->next
	mov         rax,qword ptr tempNode 
	mov         rax,qword ptr [rax]  
	mov         qword ptr tempNode,rax  
	jmp         While3Condition

DoWhile1Continue2:
	; current->visited = true
	mov         rax,qword ptr current
	mov         byte ptr [rax+532],1
	
	; } while (!finished)
	movzx       eax,byte ptr finished  
	test        eax,eax  
	je          DoWhile1

	ret  
linearVersion ENDP

END