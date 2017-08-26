.source code.txt
.class public test
.super java/lang/Object

.method public <init>()V
aload_0
invokenonvirtual java/lang/Object/<init>()V
return
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 100
.limit stack 100
iconst_0
istore 1
fconst_0
fstore 2
.line 1
iconst_0
istore 3
.line 2
L_0:
ldc 5
istore 3
.line 3
L_1:
iload 3
ldc 5
if_icmpeq L_2
goto L_12
.line 4
L_2:
.line 5
ldc 100
istore 3
.line 6
L_3:
iconst_0
istore 4
.line 7
L_4:
ldc 0
istore 4
L_5:
iload 4
ldc 9
if_icmplt L_7
goto L_8
L_6:
iload 4
ldc 1
iadd
istore 4
goto L_5
L_7:
.line 8
iload 4
istore 1
getstatic      java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 9
goto L_6
.line 10
L_8:
L_9:
iload 4
ldc 0
if_icmpge L_10
goto L_13
L_10:
.line 11
iload 4
istore 1
getstatic      java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 12
L_11:
iload 4
ldc 1
isub
istore 4
.line 13
goto L_9
.line 14
goto L_13
.line 15
.line 16
L_12:
.line 17
ldc 9
istore 3
.line 18
.line 19
L_13:
iload 3
istore 1
getstatic      java/lang/System/out Ljava/io/PrintStream;
iload 1
invokevirtual java/io/PrintStream/println(I)V
.line 20
L_14:
return
.end method
