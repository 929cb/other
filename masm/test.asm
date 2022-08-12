;   byte ptr可以指定数据为字节型
;   word ptr可以指定数据为字型




; assume cs:code
; ;从地址20000:1000开始存放0～F
; code segment

;     mov ax, 2000H
;     mov ds, ax
;     mov bx, 1000H

;     mov dl, 0
;     mov cx, 16  ;cx寄存器保存了loop跳转次数

; xunhuan:
;     mov ds:[bx], dl
;     inc dl
;     inc bx
;     loop xunhuan

; code ends

; end


; assume cs:code
; ;计算123*236结果存放在AX中
; code segment

;     mov ax, 0   ;7164H
;     mov cx, 123

; xunhuan:
;     add ax, 236
;     loop xunhuan


;     mov ax, 4C00H
;     int 21H

; code ends

; end



; assume cs:code
; ;求FFFF:0 ~ FFFF:F字节型数据的和存放在dx
; code segment

;     mov dx, 0
;     mov ax, 0FFFFH
;     mov ds, ax
;     mov ax, 0
;     mov bx, 0
;     mov cx, 16

; xunhuan:
;     mov al, ds:[bx]
;     add dx, ax
;     inc bx
;     loop xunhuan

; code ends

; end



; assume cs:code
; ;将内存FFFF:0 ~ FFFF:F内存单元中的数据复制到0:200 ~ 0:20F中
; code segment

;     mov ax, 0FFFFH
;     mov ds, ax
;     mov bx, 0
;     mov cx, 8

; xunhuan:
;     push ds
;     mov dx, ds:[bx]
    
;     mov ax, 20H
;     mov ds, ax
;     mov ds:[bx], dx
;     add bx, 2

;     pop ds
;     loop xunhuan

; code ends
; end


; assume cs:code
; ;向内存0:200 ~ 0:23F依次传输数据0~63(3FH)
; code segment

;     mov ax, 20H
;     mov es, ax
;     mov bx, 0
;     mov cx, 64

; xunhuan:
;     mov es:[bx], bl
;     inc bx
;     loop xunhuan

; code ends
; end


; assume cs:code
; ;将包括loop前的指令复制到内存0:200H开始的地址
; code segment

;     mov ax, cs
;     mov ds, ax

;     mov ax, 20H
;     mov es, ax

;     mov bx, 0
;     mov cx, 23

; xunhuan:
;     mov al, ds:[bx]
;     mov es:[bx], al
;     inc bx
;     loop xunhuan

; code ends
; end



; assume cs:code

; code segment

;         dw 1, 2, 3, 4, 5, 6, 7, 8   ;define data

; start:  mov ax, 0
;         mov bx, 0
;         mov cx, 8

; addNum: add ax, cs:[bx]
;         add bx, 2
;         loop addNum

; code ends

; end start


; assume cs:code, ds:data, ss:stack

; data segment
;     dw 0123H, 0456H, 0789H, 0ABCH, 0DEFH, 0FEDH, 0CBAH, 0987H
; data ends

; stack segment stack
;     ;stack mem  实际占用字节数会向上取16的整数倍
;     dw 0, 0, 0, 0, 0, 0, 0, 0
;     dw 0, 0, 0, 0, 0, 0, 0, 0
; stack ends

; code segment

; start:      mov ax, stack
;             mov ss, ax
;             mov sp, 32

;             mov ax, data
;             mov ds, ax

;             mov bx, 0
;             mov cx, 8

; pushData:   push ds:[bx]
;             add bx, 2
;             loop pushData

;             mov bx, 0
;             mov cx, 8

; popData:    pop ds:[bx]
;             add bx, 2
;             loop popData


;             mov ax, 4C00H
;             int 21H

; code ends

; end start


; assume cs:code,ds:data,ss:stack
; ;将大小写互转
; data segment
;     db 'hEllo'
;     db 'WOrLd'
; data ends

; stack segment stack
;     dw 0,0,0,0
;     dw 0,0,0,0
;     dw 0,0,0,0
;     dw 0,0,0,0
; stack ends

; code segment
;     start:      mov ax, stack
;                 mov ss, ax
;                 mov sp, 32

;                 mov ax, data
;                 mov ds, ax

;                 mov bx, 0
;                 mov cx, 5

;     toUp:       mov al, ds:[bx]
;                 and al, 11011111B
;                 mov ds:[bx], al
;                 inc bx
;                 loop toUp

;                 mov cx, 5

;     toDown:     mov al, ds:[bx]
;                 or al, 32
;                 mov ds:[bx], al
;                 inc bx
;                 loop toDown


;                 mov ax, 4C00H
;                 int 21H
; code ends

;     end start



; assume cs:code,ds:data
; ;copy string
; data segment
;     db 'whlcome to masm!'
;     db '----------------'
; data ends

; code segment
;     start:      mov ax, data
;                 mov ds, ax

;                 mov si, 0
;                 mov di, 16

;                 mov cx, 8

;     copyString: mov ax, ds:[si]
;                 mov ds:[di], ax
;                 add si, 2
;                 add di, 2
;                 loop copyString


;                 mov ax, 4C00H
;                 int 21H
; code ends

;     end start



; assume cs:code, ds:data
; ;first to Up
; data segment
;     db '1. file         '
;     db '2. edit         '
;     db '3. search       '
;     db '4. view         '
;     db '5. options      '
;     db '6. help         '
; data ends

; code segment
;     start:      mov ax, data
;                 mov ds, ax
;                 mov bx, 0

;                 mov si, 3
;                 mov cx, 6

;     toUp:       mov al, ds:[si+bx]
;                 and al, 11011111B
;                 mov ds:[si+bx], al
;                 add bx, 16
;                 loop toUp


;                 mov ax, 4C00H
;                 int 21H
; code ends
;     end start



; assume cs:code, ds:data, ss:stack

; data segment
;     db 'ibm             '
;     db 'dec             '
;     db 'dos             '
;     db 'vax             '
; data ends

; stack segment stack
;     dw 0, 0, 0, 0, 0, 0, 0, 0
; stack ends

; code segment 
;     start:      mov ax, data
;                 mov ds, ax
;                 mov bx, 0
;                 mov si, 0

;                 mov ax, stack
;                 mov ss, ax
;                 mov sp, 16

;                 mov cx, 4

;     num:        push cx
;                 mov cx, 3

;     toUp:       mov al, ds:[bx+si]
;                 and al, 0DFH
;                 mov ds:[bx+si], al
;                 inc bx
;                 loop toUp

;                 mov bx, 0
;                 add si, 16
;                 pop cx
;                 loop num


;                 mov ax, 4C00H
;                 int 21H
; code ends
;     end start



; assume cs:code, ds:data, ss:stack

; data segment
;     db 'EDC'            ;公司名
;     db 'Ken Olsen'      ;总裁名
;     dw 137              ;排名      -> 38
;     dw 40               ;收入      -> +70
;     db 'PDP'            ;著名产品   -> 'VAX'
; data ends

; stack segment stack
;     dw 0,0,0,0,0,0,0,0
;     dw 0,0,0,0,0,0,0,0
; stack ends

; code segment
;     start:      mov ax, stack
;                 mov ss, ax
;                 mov sp, 32

;                 mov ax, data
;                 mov ds, ax

;                 mov word ptr ds:[12], 38
;                 add word ptr ds:[14], 70
;                 mov byte ptr ds:[16], 'V'
;                 mov byte ptr ds:[17], 'A'
;                 mov byte ptr ds:[18], 'X'


;                 mov ax, 4C00H
;                 int 21H
; code ends
;     end start



; assume cs:code,ds:data,ss:stack
; ;用div计算data段中的的第一个数据除以第二个数据，商放在第三个存储单元
; data segment
;     dd 100001   ;4字节
;     dw 100
;     dw 0
; data ends

; stack segment stack
;     db 128 dup (0)  ;128个0，每个0占用1个字节
; stack ends

; code segment
;     start:      mov ax, data
;                 mov ds, ax

;                 mov ax, ds:[0]
;                 mov dx, ds:[2]
;                 div word ptr ds:[4]

;                 mov ds:[6], ax


;                 mov ax, 4C00H
;                 int 21H
; code ends
;     end start



; assume cs:code

; code segment
;             mov ax, 4C00H
;             int 21H

;     start:  mov ax, 0
;     s:      nop
;             nop ;什么都不做，但会占用一个字节

;             mov di, OFFSET s    ;计算偏移量
;             mov si, OFFSET s2
;             mov ax, cs:[si] ;   【将s2位置处的两个字节拷贝到s处】【拷贝机器码(编译时计算)】
;             mov cs:[di], ax

;     s0:     jmp short s ;短转移指令 转移范围占一个字节

;     s1:     mov ax, 0
;             int 21H
;             mov ax, 0

;     s2:     jmp short s1    ;   【执行jmp指令时，ip已经+2，跳转到s1处相当于向前跳转了10字节---机器码EBF6(-10)】
;             nop
; code ends
;             ;首先将指令存放到指令缓存区
;             ;再将ip加上当前指令所占字节数
;             ;最后取出指令执行
; end start



assume cs:code,ds:data,ss:stack

data segment
        db 'welcome to XXX!'
data ends

stack segment stack
        dw 8 dup (0)
stack ends

code segment
        start:          mov ax, stack
                        mov ss, ax
                        mov sp, 10H

                        mov ax, data
                        mov ds, ax

                        mov ax, 0B800H
                        mov es, ax

                        mov dh, 1

                        mov bx, 160*10+30*2
                        mov di, 0
                        mov cx, 4

        row:            push cx
                        mov cx, 15

        col:            mov dl, ds:[di]
                        mov es:[bx], dx
                        inc di
                        add bx, 2
                        loop col

                        mov di, 0
                        inc dh
                        pop cx
                        add bx, 160-30
                        loop row

                        mov ax, 4C00H
                        int 21H
code ends

end start