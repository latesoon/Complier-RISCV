
FSmain:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    rsp,0x8
    1008:	48 8b 05 d9 2f 00 00 	mov    rax,QWORD PTR [rip+0x2fd9]        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   rax,rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   rax
    1016:	48 83 c4 08          	add    rsp,0x8
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <_ZNSirsERi@plt-0x10>:
    1020:	ff 35 5a 2f 00 00    	push   QWORD PTR [rip+0x2f5a]        # 3f80 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 5c 2f 00 00    	jmp    QWORD PTR [rip+0x2f5c]        # 3f88 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

0000000000001030 <_ZNSirsERi@plt>:
    1030:	ff 25 5a 2f 00 00    	jmp    QWORD PTR [rip+0x2f5a]        # 3f90 <_ZNSirsERi@GLIBCXX_3.4>
    1036:	68 00 00 00 00       	push   0x0
    103b:	e9 e0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001040 <__cxa_atexit@plt>:
    1040:	ff 25 52 2f 00 00    	jmp    QWORD PTR [rip+0x2f52]        # 3f98 <__cxa_atexit@GLIBC_2.2.5>
    1046:	68 01 00 00 00       	push   0x1
    104b:	e9 d0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001050 <_ZNSolsEPFRSoS_E@plt>:
    1050:	ff 25 4a 2f 00 00    	jmp    QWORD PTR [rip+0x2f4a]        # 3fa0 <_ZNSolsEPFRSoS_E@GLIBCXX_3.4>
    1056:	68 02 00 00 00       	push   0x2
    105b:	e9 c0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001060 <_ZNSt8ios_base4InitC1Ev@plt>:
    1060:	ff 25 42 2f 00 00    	jmp    QWORD PTR [rip+0x2f42]        # 3fa8 <_ZNSt8ios_base4InitC1Ev@GLIBCXX_3.4>
    1066:	68 03 00 00 00       	push   0x3
    106b:	e9 b0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001070 <_ZNSolsEi@plt>:
    1070:	ff 25 3a 2f 00 00    	jmp    QWORD PTR [rip+0x2f3a]        # 3fb0 <_ZNSolsEi@GLIBCXX_3.4>
    1076:	68 04 00 00 00       	push   0x4
    107b:	e9 a0 ff ff ff       	jmp    1020 <_init+0x20>

Disassembly of section .plt.got:

0000000000001080 <__cxa_finalize@plt>:
    1080:	ff 25 32 2f 00 00    	jmp    QWORD PTR [rip+0x2f32]        # 3fb8 <__cxa_finalize@GLIBC_2.2.5>
    1086:	66 90                	xchg   ax,ax

Disassembly of section .text:

0000000000001090 <__cxx_global_var_init>:
    1090:	55                   	push   rbp
    1091:	48 89 e5             	mov    rbp,rsp
    1094:	48 8d 3d 76 2f 00 00 	lea    rdi,[rip+0x2f76]        # 4011 <_ZStL8__ioinit>
    109b:	e8 c0 ff ff ff       	call   1060 <_ZNSt8ios_base4InitC1Ev@plt>
    10a0:	48 8b 3d 51 2f 00 00 	mov    rdi,QWORD PTR [rip+0x2f51]        # 3ff8 <_ZNSt8ios_base4InitD1Ev@GLIBCXX_3.4>
    10a7:	48 8d 35 63 2f 00 00 	lea    rsi,[rip+0x2f63]        # 4011 <_ZStL8__ioinit>
    10ae:	48 8d 15 53 2f 00 00 	lea    rdx,[rip+0x2f53]        # 4008 <__dso_handle>
    10b5:	e8 86 ff ff ff       	call   1040 <__cxa_atexit@plt>
    10ba:	5d                   	pop    rbp
    10bb:	c3                   	ret    
    10bc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000000010c0 <_GLOBAL__sub_I_main.cpp>:
    10c0:	55                   	push   rbp
    10c1:	48 89 e5             	mov    rbp,rsp
    10c4:	e8 c7 ff ff ff       	call   1090 <__cxx_global_var_init>
    10c9:	5d                   	pop    rbp
    10ca:	c3                   	ret    
    10cb:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

00000000000010d0 <_start>:
    10d0:	f3 0f 1e fa          	endbr64 
    10d4:	31 ed                	xor    ebp,ebp
    10d6:	49 89 d1             	mov    r9,rdx
    10d9:	5e                   	pop    rsi
    10da:	48 89 e2             	mov    rdx,rsp
    10dd:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
    10e1:	50                   	push   rax
    10e2:	54                   	push   rsp
    10e3:	45 31 c0             	xor    r8d,r8d
    10e6:	31 c9                	xor    ecx,ecx
    10e8:	48 8d 3d d1 00 00 00 	lea    rdi,[rip+0xd1]        # 11c0 <main>
    10ef:	ff 15 d3 2e 00 00    	call   QWORD PTR [rip+0x2ed3]        # 3fc8 <__libc_start_main@GLIBC_2.34>
    10f5:	f4                   	hlt    
    10f6:	66 2e 0f 1f 84 00 00 	cs nop WORD PTR [rax+rax*1+0x0]
    10fd:	00 00 00 

0000000000001100 <deregister_tm_clones>:
    1100:	48 8d 3d 09 2f 00 00 	lea    rdi,[rip+0x2f09]        # 4010 <__TMC_END__>
    1107:	48 8d 05 02 2f 00 00 	lea    rax,[rip+0x2f02]        # 4010 <__TMC_END__>
    110e:	48 39 f8             	cmp    rax,rdi
    1111:	74 15                	je     1128 <deregister_tm_clones+0x28>
    1113:	48 8b 05 be 2e 00 00 	mov    rax,QWORD PTR [rip+0x2ebe]        # 3fd8 <_ITM_deregisterTMCloneTable@Base>
    111a:	48 85 c0             	test   rax,rax
    111d:	74 09                	je     1128 <deregister_tm_clones+0x28>
    111f:	ff e0                	jmp    rax
    1121:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    1128:	c3                   	ret    
    1129:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000001130 <register_tm_clones>:
    1130:	48 8d 3d d9 2e 00 00 	lea    rdi,[rip+0x2ed9]        # 4010 <__TMC_END__>
    1137:	48 8d 35 d2 2e 00 00 	lea    rsi,[rip+0x2ed2]        # 4010 <__TMC_END__>
    113e:	48 29 fe             	sub    rsi,rdi
    1141:	48 89 f0             	mov    rax,rsi
    1144:	48 c1 ee 3f          	shr    rsi,0x3f
    1148:	48 c1 f8 03          	sar    rax,0x3
    114c:	48 01 c6             	add    rsi,rax
    114f:	48 d1 fe             	sar    rsi,1
    1152:	74 14                	je     1168 <register_tm_clones+0x38>
    1154:	48 8b 05 95 2e 00 00 	mov    rax,QWORD PTR [rip+0x2e95]        # 3ff0 <_ITM_registerTMCloneTable@Base>
    115b:	48 85 c0             	test   rax,rax
    115e:	74 08                	je     1168 <register_tm_clones+0x38>
    1160:	ff e0                	jmp    rax
    1162:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
    1168:	c3                   	ret    
    1169:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000001170 <__do_global_dtors_aux>:
    1170:	f3 0f 1e fa          	endbr64 
    1174:	80 3d 95 2e 00 00 00 	cmp    BYTE PTR [rip+0x2e95],0x0        # 4010 <__TMC_END__>
    117b:	75 2b                	jne    11a8 <__do_global_dtors_aux+0x38>
    117d:	55                   	push   rbp
    117e:	48 83 3d 32 2e 00 00 	cmp    QWORD PTR [rip+0x2e32],0x0        # 3fb8 <__cxa_finalize@GLIBC_2.2.5>
    1185:	00 
    1186:	48 89 e5             	mov    rbp,rsp
    1189:	74 0c                	je     1197 <__do_global_dtors_aux+0x27>
    118b:	48 8b 3d 76 2e 00 00 	mov    rdi,QWORD PTR [rip+0x2e76]        # 4008 <__dso_handle>
    1192:	e8 e9 fe ff ff       	call   1080 <__cxa_finalize@plt>
    1197:	e8 64 ff ff ff       	call   1100 <deregister_tm_clones>
    119c:	c6 05 6d 2e 00 00 01 	mov    BYTE PTR [rip+0x2e6d],0x1        # 4010 <__TMC_END__>
    11a3:	5d                   	pop    rbp
    11a4:	c3                   	ret    
    11a5:	0f 1f 00             	nop    DWORD PTR [rax]
    11a8:	c3                   	ret    
    11a9:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

00000000000011b0 <frame_dummy>:
    11b0:	f3 0f 1e fa          	endbr64 
    11b4:	e9 77 ff ff ff       	jmp    1130 <register_tm_clones>
    11b9:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

00000000000011c0 <main>:
    11c0:	55                   	push   rbp
    11c1:	48 89 e5             	mov    rbp,rsp
    11c4:	48 83 ec 10          	sub    rsp,0x10
    11c8:	c7 45 fc 00 00 00 00 	mov    DWORD PTR [rbp-0x4],0x0
    11cf:	48 8b 3d 0a 2e 00 00 	mov    rdi,QWORD PTR [rip+0x2e0a]        # 3fe0 <_ZSt3cin@GLIBCXX_3.4>
    11d6:	48 8d 75 f4          	lea    rsi,[rbp-0xc]
    11da:	e8 51 fe ff ff       	call   1030 <_ZNSirsERi@plt>
    11df:	c7 45 f8 02 00 00 00 	mov    DWORD PTR [rbp-0x8],0x2
    11e6:	c7 45 f0 01 00 00 00 	mov    DWORD PTR [rbp-0x10],0x1
    11ed:	8b 45 f8             	mov    eax,DWORD PTR [rbp-0x8]
    11f0:	3b 45 f4             	cmp    eax,DWORD PTR [rbp-0xc]
    11f3:	0f 8f 18 00 00 00    	jg     1211 <main+0x51>
    11f9:	8b 45 f0             	mov    eax,DWORD PTR [rbp-0x10]
    11fc:	0f af 45 f8          	imul   eax,DWORD PTR [rbp-0x8]
    1200:	89 45 f0             	mov    DWORD PTR [rbp-0x10],eax
    1203:	8b 45 f8             	mov    eax,DWORD PTR [rbp-0x8]
    1206:	83 c0 01             	add    eax,0x1
    1209:	89 45 f8             	mov    DWORD PTR [rbp-0x8],eax
    120c:	e9 dc ff ff ff       	jmp    11ed <main+0x2d>
    1211:	8b 75 f0             	mov    esi,DWORD PTR [rbp-0x10]
    1214:	48 8b 3d b5 2d 00 00 	mov    rdi,QWORD PTR [rip+0x2db5]        # 3fd0 <_ZSt4cout@GLIBCXX_3.4>
    121b:	e8 50 fe ff ff       	call   1070 <_ZNSolsEi@plt>
    1220:	48 89 c7             	mov    rdi,rax
    1223:	48 8b 35 96 2d 00 00 	mov    rsi,QWORD PTR [rip+0x2d96]        # 3fc0 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    122a:	e8 21 fe ff ff       	call   1050 <_ZNSolsEPFRSoS_E@plt>
    122f:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
    1232:	48 83 c4 10          	add    rsp,0x10
    1236:	5d                   	pop    rbp
    1237:	c3                   	ret    

Disassembly of section .fini:

0000000000001238 <_fini>:
    1238:	f3 0f 1e fa          	endbr64 
    123c:	48 83 ec 08          	sub    rsp,0x8
    1240:	48 83 c4 08          	add    rsp,0x8
    1244:	c3                   	ret    
