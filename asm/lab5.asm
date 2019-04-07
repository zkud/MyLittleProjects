; lab 5: file input / output
.model TINY

.code
org 80h
    cmd_length db ?
    cmd_line db ?
org 100h

begin:
jmp start_place
; ############################################## algorithms
    ; returns in ax count of read bytes, in cx 1 if it's the last possible reading operation
    read proc
        mov ax, 0
        mov ah, 3fh
        mov cx, 50 ; read 50 bytes to read buf
        lea dx, read_buf

        int 21h

        cmp ax, cx
        jb read_the_last_op 

        ; not the last possible read operation
        mov cx, 0
        jmp read_end
    
    read_the_last_op:
        mov cx, 1
    
    read_end:
        ret
    read endp
    
    handle_new_line proc
        cmp letters_count, 2
        ja handle_end

        inc lines_count

        handle_end:
        mov letters_count, 0
        mov cr_occured, 0
        ret
    handle_new_line endp

    scan_buffer proc
        mov si, offset read_buf

        scan_loop:
            lodsb ; load character
            inc letters_count

            ; endline parsing
            cmp cr_occured, 1 ; check if carriage return occured before
            jne check_cr 

            cmp al, 10 ; cmp al with \n
            je handle 
            
            mov cr_occured, 0 ; handle \r + not \n
            jmp skip

        handle: ; handle situation "\r\n" == newline
            call handle_new_line
            jmp skip

        check_cr: ; check if a new char is \r and set a cr flag if it is
            cmp al, 13 ; cmp al with \r
            jne skip
            mov cr_occured, 1

        skip:
        loop scan_loop

        ret
        cr_occured db 0
    scan_buffer endp

    ; gets in bx input file desc
    ; returns in ax count of empty lines
    count_empty_lines proc                   
        push cx
        push dx
        push si

    fill_buffer:
        call read ; fill buffer with file content

        cmp cx, 0 ; if cx = 0, continue, else stop
        je continue 
        jmp end_count
        continue:

        mov cx, buffer_size

        call scan_buffer ; find string ends in buffer

        jmp fill_buffer
    
    end_count:
        mov cx, ax
        cmp cx, 0
        je if_empty_buffer

        call scan_buffer

        if_empty_buffer:

        pop si        
        pop dx        
        pop cx
        mov ax, lines_count
        ret
        letters_count dw 0
        lines_count dw 0
        read_buf db 50 dup('$')
        buffer_size equ 50
    count_empty_lines endp

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
        base equ cx

    Repeat:
        xor	dx,dx      

        div	base ; dx stores residue, ax stores quotient

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
        
; ############################################################ / I / O
; ################################################## support
    ; opens a file with name in ds:dx
    ; cf = 0 if all is ok
    ; returns file desc in bx
    fopen proc
        push ax
        push cx 

        mov ah, 3dh ; use dos int 21: open file
        ; mov al, 00100001b ; open file in read mode,
                          ; allow reading only to others
    
        ; xor cx, cx ; don't use mask
        mov al, 02h
        int 21h

        mov bx, ax ; copy desc to bx
        
        jc error_handling ; if errors occured, handle them
        jmp fopen_end
    error_handling:

        cmp al, 0ch
        jne _not_im
        jmp _incorrect_mode
        _not_im:

        cmp al, 05h
        jne _not_ad
        jmp _access_denied
        _not_ad:

        cmp al, 04h
        jne _not_fc
        jmp _file_count_is_too_big
        _not_fc:

        cmp al, 03h
        jne _not_pnf
        jmp _path_dont_found
        _not_pnf:

        ; error handling
    _file_dont_found:
        puts file_dont_found
        jmp fopen_error_end
    _path_dont_found:
        puts path_dont_found
        jmp fopen_error_end
    _file_count_is_too_big:
        puts file_count_is_too_big
        jmp fopen_error_end
    _access_denied:
        puts access_denied
        jmp fopen_error_end
    _incorrect_mode:
        puts incorrect_mode
        jmp fopen_error_end

    fopen_error_end: ; set carry flag as 1 
        mov cx, 1
        shr cx, 1
    fopen_end:
        pop cx        
        pop ax
        ret
        file_dont_found db "error: file do not found", '$'
        path_dont_found db "error: path do not found", '$'
        file_count_is_too_big db "error: file count is too big", '$'
        access_denied db "error: access denied", '$'
        incorrect_mode db "error: incorrect file mode", '$'
    fopen endp

    ; closes file
    ; input: file desc in bx
    fclose proc
        push ax

        mov ah, 3eh
        int 21h ; just close file

        pop ax
        ret
    fclose endp

    ; loads in ds:dx the first argv value
    get_path proc
        push ax
        push bx
        push cx
        push si
        push di

        mov cl, cmd_length ; check for emptyness
        cmp cl, 1
        jle empty_error
        
        ; find the first param and store it
        mov di, offset cmd_line

        cld

        mov al, ' '
        repz scasb
        dec di ; di stores start of the first param

        mov dx, di
        mov si, di

    scan:
        lodsb
        cmp al, 0dh
        je end_of_scan
        cmp al, 20h ; 20h is a space
        je end_of_scan ; skip chars while not space of end of line
        jmp scan

    end_of_scan:
        dec si 
        mov byte ptr [si], 0 ; form good line end

        jmp get_path_end
    empty_error:
        puts empty_error_text
        mov bx, 1
        shr bx, 1
    get_path_end:
        pop di
        pop si
        pop cx
        pop bx
        pop ax
        ret
    empty_error_text db "input a file path, example: lab.com file.txt", '$'
    get_path endp
; ################################################## / support
; ############################################################################# main

    main proc
        call get_path
        jnc not_main_end 
        jmp main_end
        not_main_end:

        puts processing_message

        ; open a file
        ; file desc will be stored in bx
        call fopen
        jc main_end

        call count_empty_lines

        puts result_message
        ; count of empty lines will be stored in ax        
        ; output it
        call print_decimal

        call fclose
        main_end:
        ret
        processing_message db "start scanning file...", '$'
        result_message db "count of empty lines is:", '$'
    main endp

; ############################################################################# / main
start_place:
    call main

    mov ax, 4C00h
    int 21h 
; ################### additional data
    ; support variables
    newline db 0dh, 0ah, '$'
end begin
