; lab 6: iinterrrupts
.model TINY
.386
.code
org 80h
    cmd_length db ?
    cmd_line db ?
org 100h

begin:
jmp start_place
; ############################################## algorithms
; ################################################## / algorithms    
; ################################################## support
    prologue macro
        push bp
        mov bp, sp
        endm
    
    epilogue macro
        mov sp, bp
        pop bp
        endm

    ; saves common registers
    save_regs macro
        push ax
        push bx
        push cx
        push dx
        push ds
        push si
        push es
        push di
        endm


    ; loads common registers
    load_regs macro
        pop di
        pop es
        pop si
        pop ds
        pop dx
        pop cx
        pop bx
        pop ax
        endm

; ################################################## / support
; ################################################################### I / O
    ; input : al - char to output
    putc macro
        save_regs

        mov dl, al
        mov ah, 02h
        int 21h

        load_regs
        endm

    ; input : string - string adress
    print_string macro string
        save_regs

        mov ah, 9
        mov dx, offset string
        int 21h

        load_regs
        endm
    
    puts macro string
        save_regs

        print_string string
        print_string newline

        load_regs
        endm

    ; char of number in al, ah
    print_number proc far
        push cx

        mov cl, al
        add cl, '0'
        mov ch, attrib 

        mov word ptr es:bx, cx
        add bx, 2

        mov cl, ah
        add cl, '0'
        mov ch, attrib 

        mov word ptr es:bx, cx
        add bx, 2

        pop cx
        ret
        attrib db 050h
    print_number endp

    print_divider proc far
        push cx

        mov cl, ':'
        mov ch, attrib 

        mov word ptr es:bx, cx
        add bx, 2

        pop cx
        ret
        div_attrib db 050h
    print_divider endp

    ; prints time in x, y
    print_time proc far
        save_regs

        ; set up start cords
        mov bx, 0b800h  
        mov es, bx

        mov ax, 80 ; ax = 80 * 2
        xor bx, bx ; bx = y
        mov bl, time_y

        mul bl     ; ax = 80 * y * 2

        mov bx, ax ; bx = 80 * y * 2

        xor ax, ax
        mov al, time_x ; ax = x

        add bx, ax     ; bx = 80 * y * 2 + 2 * x
        ; form address es:[bx] = b800h : [time_y * 80 * 2 + 2 * time_x]
        mov ax, bx
        mov bx, 2
        mul bx
        mov bx, ax


        ; print time directly in videobuffer
        mov ah, 2ch
        int 21h ; get system time ch hour, cl minute, dh second

        xor ax, ax
        mov al, ch 
        div base

         
        call print_number
        call print_divider

        xor ax, ax
        mov al, cl
        div base

        call print_number
        call print_divider

        xor ax, ax
        mov al, dh
        div base

        call print_number

        load_regs

        ret
        size_of_char db 2
        base db 10
    print_time endp
        
; ############################################################ / I / O
; ################################################## support
    ; string to integer 
    ; get input askii string (c like) in es:si
    ; output in al
    ; if input is incorrect, set cf = 1
    parse_decimal proc
        push bx

        mov ax, 0
        mov bx, 0
        mov parse_overflow, 0

        get_char:
            lodsb

            cmp al, 0
            je end_parsing_loop

            cmp al, ' '
            je get_char

            cmp al, '0'
            jb parse_decimal_error 

            cmp al, '9'
            ja parse_decimal_error

            sub al, '0' 
            add bx, ax   ; bx = ax - '0' + bx * 10

            lodsb ; crutch
            cmp al, 0
            je end_parsing_loop


            dec si

            mov ax, bx ; ax = bx
            mov bx, 10
            mul bl      ; ax = bx * 10
            jo  parse_decimal_error

            mov bx, ax ; bx = bx * 10
            jmp get_char

                    
        end_parsing_loop:
        mov ax, bx

        cmp ax, limit
        jge parse_decimal_error
        
        jmp parse_decimal_end
        parse_decimal_error:
            mov parse_overflow, 1
        parse_decimal_end:

        pop bx

        ret
        limit dw 0fffh
        parse_overflow db 0
    parse_decimal endp

    ; prints decimal number to stdout
    ; input in ax
    print_decimal proc 
        push ax
        push cx
        push dx 
        push di

        std ; prepare string buffer to writing
        lea	di, buffer_end-1

        mov cx, 10 ; set base of calculation system
        pd_base equ cx

    Repeat:
        xor	dx,dx      

        div	pd_base ; dx stores residue, ax stores quotient

        xchg ax,dx  
        add	al,'0' 
        stosb  ; add digit to output

        xchg ax,dx   
        or	ax,ax  ; check if quotient is zero
        jne	Repeat         

        mov	ah,9
        lea	dx,[di+1] ; output string (di + 1 is address of string start)
        int	21h

        pop di
        pop dx
        pop cx
        pop ax

        ret
        buffer_string db 5 dup ('$')
        buffer_end db '$'
    print_decimal endp

    get_argv proc
        pusha

        mov get_argv_error, 0

        cld

        mov cl, cmd_length ; check for emptyness
        cmp cl, 1
        jle incorrect_error
        
        mov cx, -1 ; set up start position, make cx work line infinity
        mov di, offset cmd_line

    find_param:
        mov al, ' '
        repz scasb
        dec di ; skip spaces

        push di
        inc word ptr argc ; save parameter
        
        mov si, di

    scan_params: ; get char, compare it to ends, handle ends
        lodsb 

        cmp al, 0dh
        je params_ended

        cmp al, 20h
        je end_scan_params

        cmp al, '0'
        jl incorrect_error

        cmp al, '9'
        jg incorrect_error
        jmp scan_params
    end_scan_params:

        dec si
        mov byte ptr [si], 0 ; set end of parameter as \0

        mov di, si

        inc di

        jmp short find_param

    params_ended:
        dec si
        mov byte ptr [si], 0

        ; parameters handling
        cmp argc, 2 
        jne incorrect_error
         
    parse_paramoters:
        dec argc
        pop si
        mov limit, 25
        call parse_decimal
        
        cmp parse_overflow, 1
        je overflow

        mov time_y, al

        dec argc
        pop si
        mov limit, 72
        call parse_decimal

        cmp parse_overflow, 1
        je overflow

        mov time_x, al

        jmp get_argv_end
    incorrect_error:
        mov cx, argc

        free_loop:
            pop bx
        loop free_loop

        puts empty_error_text
        mov get_argv_error, 1

        jmp get_argv_end
    overflow:
        mov cx, argc

        free_loop_2:
            pop bx
        loop free_loop_2

        puts overflow_text
        mov get_argv_error, 1

    get_argv_end:
        
        popa
        ret
    argc dw 0
    empty_error_text db "input a cords of clock, example: 20 20", '$'
    overflow_text db "your args are too big", '$'
    debug db "debug", '$'
    get_argv_error db 0
    get_argv endp
; ################################################## / support
; ################################################## interrupts
old_timer_int dd ?
    timer_int proc
        pushf
        call cs: old_timer_int
        pusha

        push ds
        push es
        push cs
        pop ds ; setup ds
        ;
        call print_time
        ;
        pop es
        pop ds
        popa
        iret
    timer_int endp

    timer_install proc
        ; get old interrupt
        mov ah, 35h
        mov al, 08h
        int 21h

        mov word ptr old_timer_int, bx
        mov word ptr old_timer_int + 2, es

        ; set new interrupt
        mov ah, 25h
        mov al, 08h
        lea dx, timer_int
        int 21h
        ret
    timer_install endp
; ################################################## /interrupts
; ############################################################################# main

    main proc
        call get_argv
        cmp get_argv_error, 1
        jne not_main_end 
        jmp main_end
        not_main_end:

        puts processing_message

        puts x_res
        xor ax, ax
        mov al, time_x
        call print_decimal
        print_string newline

        puts y_res
        xor ax, ax
        mov al, time_y
        call print_decimal
        print_string newline

        puts result_message

        call timer_install

        main_end:
        ret
        processing_message db "setup clock...", '$'
        result_message db "successful", '$'
    main endp

; ############################################################################# / main
start_place:
    call main

    mov ax, 3100h
    mov dx, (start_place - begin + 10fh) / 16
    int 21h 
; ################### additional data
    time_x db 0
    time_y db 0

    ; support variables
    newline db 0dh, 0ah, '$'
    x_res db "your x cords:$"
    y_res db "your y cords:$"
end begin
