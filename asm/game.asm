; lab 4: game (backup)
.model TINY
.386
; ************************************************************************
; ***************************** definitions ******************************
; ************************************************************************
point struc
    x db 0
    y db 0
point ends    

.code
org 100h
begin:
jmp start_place
; *****************************************************************************
; *************************** data part ***************************************
; *****************************************************************************
    up_scan_code    = 11h ; W 
    down_scan_code  = 1fh ; S
    left_scan_code  = 1eh ; A
    right_scan_code = 20h ; D

    up   =  0
    down =  1
    left =  2
    right=  3
    head_destination db up

    delay_value dw 4
    food_waiter_start_value = 30
    wall_waiter_start_value = 45
    end_of_game db 0
    food_waiter db 30
    wall_waiter db 45

    snake_len dw 0005h
    snake label word
    head      point<40, 10>
    tail      point<40, 11>
              point<40, 12>
              point<40, 13>
              point<40, 14>
              dw 100 dup (0)
    snake_graph_attrib db 01111111b
    snake_char         db 'p'

    ; help info
    score dw 0
    help db "a - left, s - down, d - right, w - up, eat food, don't touch walls (/)", 0
    score_str db "score:", 0

    wall_len  dw 0000h
    walls     dw 100 dup(0)
    wall_graph_attrib db 01101110b
    wall_char         db '/'

    food_len  dw 0000h
    food      dw 100 dup(0)
    food_graph_attrib db 01001100b
    food_char         db 'f' 

    random_value_max_x = 79
    random_value_max_y = 22

    random_value_x db 0
    random_value_y db 0

    tics dw 0
    min_delay_edge = 1

; *************************************************************************
; ***************************** video *************************************
; *************************************************************************
print_point proc
    pusha

    ; form address es:[bx] = b800h : [y * 80 * 2 + 2 * x]
    mov bx, 0b800h  
    mov es, bx

    mov ax, 80 ; ax = 80 * 2
    xor bx, bx ; bx = y
    mov bl, point_to_print.y

    mul bl     ; ax = 80 * y * 2

    mov bx, ax ; bx = 80 * y * 2

    xor ax, ax
    mov al, point_to_print.x ; ax = x

    add bx, ax     ; bx = 80 * y * 2 + 2 * x
    mov ax, bx
    mov bx, 2
    mul bx
    mov bx, ax

    ; form char
    mov cl, char_to_print
    mov ch, char_attrib

    ; print char at address
    mov word ptr es:bx, cx

    popa
    ret
    point_to_print point<0, 0>
    char_to_print  db 0
    char_attrib    db 0
print_point endp

print_snake proc
    pusha

    mov cx, snake_len
    lea si, snake

print_snake_loop:
    mov ax, [si]
    mov point_to_print, ax

    mov al, snake_char
    mov ah, snake_graph_attrib

    mov char_to_print, al
    mov char_attrib, ah

    call print_point

    add si, 2
    loop print_snake_loop

    popa
    ret
print_snake endp

print_walls proc
    pusha

    mov cx, wall_len
    lea si, walls

    cmp cx, 0
    je print_walls_end
print_wall_loop:
    mov ax, [si]
    mov point_to_print, ax

    mov al, wall_char
    mov ah, wall_graph_attrib

    mov char_to_print, al
    mov char_attrib, ah

    call print_point

    add si, 2
    loop print_wall_loop

print_walls_end:
    popa
    ret
print_walls endp

print_food proc
    pusha

    mov cx, food_len
    lea si, food

    cmp cx, 0
    je print_food_end
print_food_loop:
    mov ax, [si]
    mov point_to_print, ax

    mov al, food_char
    mov ah, food_graph_attrib

    mov char_to_print, al
    mov char_attrib, ah

    call print_point

    add si, 2
    loop print_food_loop

    print_food_end:
    popa
    ret
print_food endp

refresh_screen proc
    call setup_screen

    call print_snake        
    
    call print_walls

    call print_food

    call print_info
    ret
refresh_screen endp

setup_screen proc
    pusha

    mov ah, 0h
    mov al, 03h ; setup standart 80 X 25 video mode
    int 10h

    mov ah, 02h
    xor bx, bx
    mov dx, 1900h
    int 10h 

    popa
    ret
setup_screen endp

setup_end_screen proc
    mov ah, 0h
    mov al, 03h ; setup standart 80 X 25 video mode
    int 10h

    lea bx, score_str
    push 0b800h
    pop es
    mov di, 0

    call putzs

    mov ax, score
    mov number_cords, 12
    call print_decimal
    
    ret
setup_end_screen endp

putzs   proc    near
; DS:BX = ZS ;ZS = Zero-String - строка, оканчивающаяся нулевым (00h) байтом.
; ES:DI = позиция вывода, ES описывает сегмент видеопамяти, DI - смещение в нём.
    push    ax
    push    bx
    push    es
    push    di
    mov ah,1Bh ;В AH будет атрибут вывода - светло-циановые
                ;  символы на синем фоне.
putzs_1: mov al,[bx]    ; Читаем байт из ZS-строки.
    inc bx      ; Переводим указатель на следующий байт.
    cmp al,0        ; Если байт равен 0,
    je  putzs_end   ; то переходим в конец процедуры.
    mov es:[ di ],ax    ; Иначе - записываем символ вместе с
                ;  атрибутом в видеопамять по заданному
                ;  смещению - цветной символ появится на
                ;  экране.
    add di,2        ; Переводим указатель в видеопамяти на
                ;  позицию следующего символа.
    jmp putzs_1; Повторяем процедуру для следующего байта из ZS-строки.
 
putzs_end: pop di
    pop es
    pop bx
    pop ax
    ret
putzs endp

; prints decimal number to video memory
; input in ax
; cords in number cords
print_decimal proc 
    pusha

    lea	di, buffer_end-1

    mov bx, 10 ; set base of calculation system
    pd_base equ bx
    
    mov cx, 5

Repeat:
    xor	dx,dx 

    div	pd_base ; dx stores residue, ax stores quotient

    xchg ax,dx  
    add	al,'0' 
    mov [di], al  ; add digit to output
    dec di

    xchg ax,dx   
    or	ax,ax  ; check if quotient is zero
    loop Repeat

    push 0b800h
    pop es
    mov bx, di
    mov di, number_cords

    call putzs

    popa
    ret
    buffer_string db 5 dup ('0')
    buffer_end db 0
    number_cords dw 0
print_decimal endp

print_info proc
    pusha

    lea bx, help
    push 0b800h
    pop es
    mov di, 3680
    
    call putzs

    lea bx, score_str
    push 0b800h
    pop es
    mov di, 3840

    call putzs

    mov ax, score
    mov number_cords, 3852
    call print_decimal
    
    popa
    ret
print_info endp

; ****************************************************************************
; ***************************** interrupts ***********************************
; ****************************************************************************
old_keyboard_interrupt dd 0
old_system_timer_interrupt dd 0

new_keyboard_interrupt proc
    pushf
    call cs:old_keyboard_interrupt ; load old keyboard interrupt
    pusha 

    push ds
    push es

    push cs
    pop ds
    ;
    ; if we get scan code and it equal to one of control codes, change destination
    in al, 60h
    xor ah, ah

    ; compare with up
    cmp al, up_scan_code
    jne end_comparison_up

        cmp head_destination, down
        je end_comparison_up

        mov head_destination, up
        jmp end_comparison_right

    end_comparison_up:

    ; compare with down 
    cmp al, down_scan_code
    jne end_comparison_down

        cmp head_destination, up
        je end_comparison_down

        mov head_destination, down
        jmp end_comparison_right

    end_comparison_down:

    ; compare with left
    cmp al, left_scan_code
    jne end_comparison_left

        cmp head_destination, right
        je end_comparison_left

        mov head_destination, left
        jmp end_comparison_right

    end_comparison_left:

    ; compare with right
    cmp al, right_scan_code
    jne end_comparison_right

        cmp head_destination, left
        je end_comparison_right

        mov head_destination, right

    end_comparison_right:

    ;
    pop es
    pop ds

    popa
    iret
new_keyboard_interrupt endp

new_system_timer_interrupt proc
    pushf
    call cs:old_system_timer_interrupt ; load old system_timer interrupt
    pusha 

    push ds
    push es

    push cs
    pop ds
    ;
    cmp random_value_x, 0

    jle reset_random_value_x

    dec random_value_x

    jmp skip_reset_x
    reset_random_value_x:
    mov random_value_x, random_value_max_x
    skip_reset_x:

    cmp random_value_y, 0

    jle reset_random_value_y

    dec random_value_y

    jmp skip_reset_y
    reset_random_value_y:
    mov random_value_y, random_value_max_y
    skip_reset_y:

    cmp tics, 0

    jle skip_dec_tics

    dec tics

    skip_dec_tics:

    ;
    pop es
    pop ds

    popa
    iret
new_system_timer_interrupt endp

real_mode_interrupts_install proc
    mov ah, 35h
    mov al, 09h
    int 21h ; get old keyboard interrupt

    mov word ptr old_keyboard_interrupt, bx
    mov word ptr old_keyboard_interrupt + 2, es ; save old keyboard interrupt

    mov ah, 35h
    mov al, 08h
    int 21h ; get old timer interrupt

    mov word ptr old_system_timer_interrupt, bx
    mov word ptr old_system_timer_interrupt + 2, es

    mov ah, 25h
    mov al, 09h
    lea dx, new_keyboard_interrupt
    int 21h ; set new keyboard interrupt
    
    mov ah, 25h
    mov al, 08h
    mov dx, offset new_system_timer_interrupt
    int 21h ; set new system timer interrupt

    ret
real_mode_interrupts_install endp

delay proc
    pusha

    mov ax, delay_value
    mov tics, ax

    wait_tics:
    cmp tics, 0
    ja wait_tics

    popa
    ret
delay endp

; ******************************************************************************
; *************************** game things **************************************
; ******************************************************************************
exists db 0
; input in ax
check_if_exist proc
    pusha

    mov cx, snake_len
    lea si, snake

search_snake_loop:
    mov bx, [si]

    cmp ax, bx
    je found

    add si, 2
    loop search_snake_loop

    mov cx, food_len
    lea si, food

    cmp cx, 0
    je try_find_walls
search_food_loop:
    mov bx, [si]

    cmp ax, bx
    je found

    add si, 2
    loop search_food_loop

try_find_walls:
    mov cx, wall_len
    lea si, walls

    cmp cx, 0
    je skip_walls_search
search_wall_loop:
    mov bx, [si]

    cmp ax, bx
    je found

    add si, 2
    loop search_wall_loop

skip_walls_search:

    jmp not_found
    found:
    mov exists, 1
    jmp end_search
    not_found:
    mov exists, 0
    end_search:
    popa
    ret
check_if_exist endp

generate_food proc
    pusha

    ; generate food
    generate_f_again:
    mov al, random_value_x    
    mov ah, random_value_y

    call check_if_exist
    cmp exists, 1
    je generate_f_again

    ; add to food array 
    inc food_len
    lea di, food
    add di, food_len
    add di, food_len
    sub di, 2

    mov [di], ax
    
    popa
    ret
generate_food endp

generate_wall proc
    pusha

    ; generate food
    generate_w_again:
    mov al, random_value_x    
    mov ah, random_value_y

    call check_if_exist
    cmp exists, 1
    je generate_w_again

    ; add to food array 
    inc wall_len
    lea di, walls
    add di, wall_len
    add di, wall_len
    sub di, 2

    mov [di], ax

    popa
    ret
generate_wall endp

shift_snake proc
    pusha

    ; check if game should end
    mov cx, wall_len
    lea si, walls

    cmp cx, 0
    jle check_tail
wall_check_loop:
    mov ax, [si]
    cmp ax, head
    je end_game
skip_next_wall:
    add si, 2
    loop wall_check_loop

    check_tail:
    mov cx, snake_len
    dec cx
    lea si, snake
    add si, 2

tail_check_loop:
    mov ax, [si]
    cmp ax, head
    je end_game
    add si, 2
    loop tail_check_loop

    ; eat food
    mov cx, food_len
    lea si, food

    cmp cx, 0
    jle shift_tail
food_check_loop:
    mov ax, [si]
    cmp ax, head
    je eat_food
skip_next_food:
    add si, 2
    loop food_check_loop

jmp shift_tail
eat_food:
    inc snake_len
    inc score

    cmp delay_value, min_delay_edge
    jle skip_delay_dec
    dec delay_value

    skip_delay_dec:

    cmp food_len, 1
    jle dec_food

    lea di, food
    add di, food_len
    add di, food_len
    sub di, 2

    mov ax, [di]
    mov [si], ax

    dec_food:
    dec food_len

shift_tail:
    ; shift tail
    mov cx, snake_len
    dec cx

    lea di, snake
    add di, snake_len
    add di, snake_len
    sub di, 2
    lea si, snake
    add si, snake_len
    add si, snake_len
    sub si, 4

shift_loop:
    mov ax, [si]
    mov [di], ax ; snake [i] = snake [i - 1]

    sub si, 2
    sub di, 2

    loop shift_loop

    ; shift head
    cmp head_destination, up
    jne skip_up   ; handle up

    ; if head.y is 0, make it 22, else make it head.y - 1
    cmp head.y, 0
    jne dec_up 

    mov head.y, 22
    jmp skip_up
    dec_up:
    dec head.y

    skip_up:
    cmp head_destination, down
    jne skip_down ; handle down

    cmp head.y, 22
    jle inc_down

    mov head.y, 0
    jmp skip_down
    inc_down:
    inc head.y

    skip_down:
    cmp head_destination, left
    jne skip_left ; handle left

    cmp head.x, 0
    jne dec_left

    mov head.x, 78
    jmp skip_left
    dec_left:
    dec head.x
    
    skip_left:    ; handle right
    cmp head_destination, right
    jne skip_right

    cmp head.x, 78
    jle inc_right

    mov head.x, 0
    jmp end_shift

    inc_right:
    inc head.x

    skip_right:

    jmp end_shift
end_game:
    mov end_of_game, 1
end_shift:
    popa
    ret
shift_snake endp

; ************************************************************************
; ************************* main program *********************************
; ************************************************************************
    main proc
        call setup_screen
        call real_mode_interrupts_install

    game_loop:
        ; shift snake and check end conditions
        call shift_snake

        ; generate food every food_waiter_start_value times
        cmp food_waiter, 0                
        jne skip_food_gen

        call generate_food
        mov food_waiter, food_waiter_start_value

    skip_food_gen:
        dec food_waiter

        ; generate wall every wall_waiter_start_value times
        cmp wall_waiter, 0                
        jne skip_wall_gen

        call generate_wall
        mov wall_waiter, wall_waiter_start_value

    skip_wall_gen:
        dec wall_waiter

        call refresh_screen

        ; wait delay_value time
        call delay

        cmp end_of_game, 1
        jne game_loop

        call setup_end_screen

        ret
    main endp

start_place:
    call main

    mov ax, 3100h
    mov dx, (start_place - begin + 10fh) / 16
    int 21h 
end begin
