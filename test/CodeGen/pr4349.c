// RUN: %clang_cc1 %s -emit-llvm -o - | FileCheck %s
// PR 4349

union reg
{
    unsigned char b[2][2];
    unsigned short w[2];
    unsigned int d;
};
struct cpu
{
    union reg pc;
};
extern struct cpu cpu;
struct svar
{
    void *ptr;
};
// CHECK: @svars1 = global [1 x %struct.svar] [%struct.svar { i8* bitcast (i32* getelementptr inbounds (%struct.cpu, %struct.cpu* @cpu, i32 0, i32 0, i32 0) to i8*) }]
struct svar svars1[] =
{
    { &((cpu.pc).w[0]) }
};
// CHECK: @svars2 = global [1 x %struct.svar] [%struct.svar { i8* getelementptr (i8, i8* bitcast (i32* getelementptr inbounds (%struct.cpu, %struct.cpu* @cpu, i32 0, i32 0, i32 0) to i8*), i64 1) }]
struct svar svars2[] =
{
    { &((cpu.pc).b[0][1]) }
};
// CHECK: @svars3 = global [1 x %struct.svar] [%struct.svar { i8* getelementptr (i8, i8* bitcast (i32* getelementptr inbounds (%struct.cpu, %struct.cpu* @cpu, i32 0, i32 0, i32 0) to i8*), i64 2) }]
struct svar svars3[] =
{
    { &((cpu.pc).w[1]) }
};
// CHECK: @svars4 = global [1 x %struct.svar] [%struct.svar { i8* getelementptr (i8, i8* bitcast (i32* getelementptr inbounds (%struct.cpu, %struct.cpu* @cpu, i32 0, i32 0, i32 0) to i8*), i64 3) }]
struct svar svars4[] =
{
    { &((cpu.pc).b[1][1]) }
};
