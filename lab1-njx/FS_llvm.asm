
test.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 10          	sub    $0x10,%rsp
   8:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
   f:	48 8b 3d 00 00 00 00 	mov    0x0(%rip),%rdi        # 16 <main+0x16>
  16:	48 8d 75 f4          	lea    -0xc(%rbp),%rsi
  1a:	e8 00 00 00 00       	call   1f <main+0x1f>
  1f:	c7 45 f8 02 00 00 00 	movl   $0x2,-0x8(%rbp)
  26:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)
  2d:	8b 45 f8             	mov    -0x8(%rbp),%eax
  30:	3b 45 f4             	cmp    -0xc(%rbp),%eax
  33:	0f 8f 18 00 00 00    	jg     51 <main+0x51>
  39:	8b 45 f0             	mov    -0x10(%rbp),%eax
  3c:	0f af 45 f8          	imul   -0x8(%rbp),%eax
  40:	89 45 f0             	mov    %eax,-0x10(%rbp)
  43:	8b 45 f8             	mov    -0x8(%rbp),%eax
  46:	83 c0 01             	add    $0x1,%eax
  49:	89 45 f8             	mov    %eax,-0x8(%rbp)
  4c:	e9 dc ff ff ff       	jmp    2d <main+0x2d>
  51:	8b 75 f0             	mov    -0x10(%rbp),%esi
  54:	48 8b 3d 00 00 00 00 	mov    0x0(%rip),%rdi        # 5b <main+0x5b>
  5b:	e8 00 00 00 00       	call   60 <main+0x60>
  60:	48 89 c7             	mov    %rax,%rdi
  63:	48 8b 35 00 00 00 00 	mov    0x0(%rip),%rsi        # 6a <main+0x6a>
  6a:	e8 00 00 00 00       	call   6f <main+0x6f>
  6f:	8b 45 fc             	mov    -0x4(%rbp),%eax
  72:	48 83 c4 10          	add    $0x10,%rsp
  76:	5d                   	pop    %rbp
  77:	c3                   	ret    

Disassembly of section .text.startup:

0000000000000000 <__cxx_global_var_init>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # b <__cxx_global_var_init+0xb>
   b:	e8 00 00 00 00       	call   10 <__cxx_global_var_init+0x10>
  10:	48 8b 3d 00 00 00 00 	mov    0x0(%rip),%rdi        # 17 <__cxx_global_var_init+0x17>
  17:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 1e <__cxx_global_var_init+0x1e>
  1e:	48 8d 15 00 00 00 00 	lea    0x0(%rip),%rdx        # 25 <__cxx_global_var_init+0x25>
  25:	e8 00 00 00 00       	call   2a <__cxx_global_var_init+0x2a>
  2a:	5d                   	pop    %rbp
  2b:	c3                   	ret    
  2c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000030 <_GLOBAL__sub_I_main.cpp>:
  30:	55                   	push   %rbp
  31:	48 89 e5             	mov    %rsp,%rbp
  34:	e8 c7 ff ff ff       	call   0 <__cxx_global_var_init>
  39:	5d                   	pop    %rbp
  3a:	c3                   	ret    
