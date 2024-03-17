; ModuleID = 'react_v3.cc'
source_filename = "react_v3.cc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%union.pthread_mutex_t = type { %struct.__pthread_mutex_s }
%struct.__pthread_mutex_s = type { i32, i32, i32, i32, i32, i16, i16, %struct.__pthread_internal_list }
%struct.__pthread_internal_list = type { %struct.__pthread_internal_list*, %struct.__pthread_internal_list* }
%struct.sockaddr_in = type { i16, i16, %struct.in_addr, [8 x i8] }
%struct.in_addr = type { i32 }
%struct.sockaddr = type { i16, [14 x i8] }
%struct.epoll_event = type <{ i32, %union.epoll_data }>
%union.epoll_data = type { i8* }
%struct.Context = type { i32, i32 }
%union.pthread_mutexattr_t = type { i32 }

@LOG_MTX = dso_local global %union.pthread_mutex_t zeroinitializer, align 8
@.str = private unnamed_addr constant [12 x i8] c"react_v3.cc\00", align 1
@.str.1 = private unnamed_addr constant [19 x i8] c"initing address...\00", align 1
@.str.2 = private unnamed_addr constant [7 x i8] c"socket\00", align 1
@.str.3 = private unnamed_addr constant [5 x i8] c"bind\00", align 1
@.str.4 = private unnamed_addr constant [26 x i8] c"starting socket listen...\00", align 1
@.str.5 = private unnamed_addr constant [6 x i8] c"accet\00", align 1
@.str.6 = private unnamed_addr constant [30 x i8] c"New client, ip: %s, port: %d\0A\00", align 1
@.str.7 = private unnamed_addr constant [45 x i8] c"Error when add new epoll event(after accept)\00", align 1
@.str.8 = private unnamed_addr constant [5 x i8] c"%s \0A\00", align 1
@.str.9 = private unnamed_addr constant [10 x i8] c"127.0.0.1\00", align 1

; Function Attrs: noinline optnone uwtable
define dso_local %struct.sockaddr_in* @_Z8InitAddrPKci(i8* %0, i32 %1) #0 {
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  %5 = alloca %struct.sockaddr_in*, align 8
  store i8* %0, i8** %3, align 8
  store i32 %1, i32* %4, align 4
  %6 = call noalias nonnull i8* @_Znwm(i64 16) #8
  %7 = bitcast i8* %6 to %struct.sockaddr_in*
  store %struct.sockaddr_in* %7, %struct.sockaddr_in** %5, align 8
  %8 = load %struct.sockaddr_in*, %struct.sockaddr_in** %5, align 8
  %9 = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %8, i32 0, i32 0
  store i16 2, i16* %9, align 4
  %10 = load i32, i32* %4, align 4
  %11 = trunc i32 %10 to i16
  %12 = call zeroext i16 @htons(i16 zeroext %11) #9
  %13 = load %struct.sockaddr_in*, %struct.sockaddr_in** %5, align 8
  %14 = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %13, i32 0, i32 1
  store i16 %12, i16* %14, align 2
  %15 = load i8*, i8** %3, align 8
  %16 = load %struct.sockaddr_in*, %struct.sockaddr_in** %5, align 8
  %17 = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %16, i32 0, i32 2
  %18 = getelementptr inbounds %struct.in_addr, %struct.in_addr* %17, i32 0, i32 0
  %19 = bitcast i32* %18 to i8*
  %20 = call i32 @inet_pton(i32 2, i8* %15, i8* %19) #10
  call void (i32, i8*, i32, i8*, ...) @_Z7log_logiPKciS0_z(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0), i32 36, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i64 0, i64 0))
  %21 = load %struct.sockaddr_in*, %struct.sockaddr_in** %5, align 8
  ret %struct.sockaddr_in* %21
}

; Function Attrs: nobuiltin allocsize(0)
declare dso_local nonnull i8* @_Znwm(i64) #1

; Function Attrs: nounwind readnone
declare dso_local zeroext i16 @htons(i16 zeroext) #2

; Function Attrs: nounwind
declare dso_local i32 @inet_pton(i32, i8*, i8*) #3

declare dso_local void @_Z7log_logiPKciS0_z(i32, i8*, i32, i8*, ...) #4

; Function Attrs: noinline optnone uwtable
define dso_local i32 @_Z14TcpServerStartP11sockaddr_in(%struct.sockaddr_in* %0) #0 {
  %2 = alloca %struct.sockaddr_in*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store %struct.sockaddr_in* %0, %struct.sockaddr_in** %2, align 8
  %6 = call i32 @socket(i32 2, i32 2049, i32 0) #10
  store i32 %6, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  %8 = icmp eq i32 %7, -1
  br i1 %8, label %9, label %10

9:                                                ; preds = %1
  call void @perror(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.2, i64 0, i64 0))
  br label %10

10:                                               ; preds = %9, %1
  %11 = load i32, i32* %3, align 4
  %12 = bitcast i32* %4 to i8*
  %13 = call i32 @setsockopt(i32 %11, i32 1, i32 2, i8* %12, i32 4) #10
  %14 = load i32, i32* %3, align 4
  %15 = load %struct.sockaddr_in*, %struct.sockaddr_in** %2, align 8
  %16 = bitcast %struct.sockaddr_in* %15 to %struct.sockaddr*
  %17 = call i32 @bind(i32 %14, %struct.sockaddr* %16, i32 16) #10
  store i32 %17, i32* %5, align 4
  %18 = load i32, i32* %5, align 4
  %19 = icmp eq i32 %18, -1
  br i1 %19, label %20, label %21

20:                                               ; preds = %10
  call void @perror(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.3, i64 0, i64 0))
  br label %21

21:                                               ; preds = %20, %10
  %22 = load i32, i32* %3, align 4
  %23 = call i32 @listen(i32 %22, i32 128) #10
  call void (i32, i8*, i32, i8*, ...) @_Z7log_logiPKciS0_z(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0), i32 54, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.4, i64 0, i64 0))
  %24 = load i32, i32* %3, align 4
  ret i32 %24
}

; Function Attrs: nounwind
declare dso_local i32 @socket(i32, i32, i32) #3

declare dso_local void @perror(i8*) #4

; Function Attrs: nounwind
declare dso_local i32 @setsockopt(i32, i32, i32, i8*, i32) #3

; Function Attrs: nounwind
declare dso_local i32 @bind(i32, %struct.sockaddr*, i32) #3

; Function Attrs: nounwind
declare dso_local i32 @listen(i32, i32) #3

; Function Attrs: noinline optnone uwtable
define dso_local void @_Z11handleEventP11epoll_eventi(%struct.epoll_event* %0, i32 %1) #0 {
  %3 = alloca %struct.epoll_event*, align 8
  %4 = alloca i32, align 4
  %5 = alloca [1024 x i8], align 16
  %6 = alloca %struct.Context*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca %struct.sockaddr_in, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca %struct.epoll_event, align 1
  %14 = alloca %struct.Context*, align 8
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca [1024 x i8], align 16
  %18 = alloca i32, align 4
  store %struct.epoll_event* %0, %struct.epoll_event** %3, align 8
  store i32 %1, i32* %4, align 4
  %19 = load %struct.epoll_event*, %struct.epoll_event** %3, align 8
  %20 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %19, i32 0, i32 1
  %21 = bitcast %union.epoll_data* %20 to i8**
  %22 = load i8*, i8** %21, align 1
  %23 = bitcast i8* %22 to %struct.Context*
  store %struct.Context* %23, %struct.Context** %6, align 8
  %24 = load %struct.Context*, %struct.Context** %6, align 8
  %25 = getelementptr inbounds %struct.Context, %struct.Context* %24, i32 0, i32 1
  %26 = load i32, i32* %25, align 4
  store i32 %26, i32* %7, align 4
  %27 = load %struct.Context*, %struct.Context** %6, align 8
  %28 = getelementptr inbounds %struct.Context, %struct.Context* %27, i32 0, i32 0
  %29 = load i32, i32* %28, align 4
  store i32 %29, i32* %8, align 4
  %30 = load %struct.epoll_event*, %struct.epoll_event** %3, align 8
  %31 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %30, i32 0, i32 0
  %32 = load i32, i32* %31, align 1
  store i32 %32, i32* %9, align 4
  %33 = load i32, i32* %9, align 4
  %34 = and i32 %33, 1
  %35 = icmp ne i32 %34, 0
  br i1 %35, label %36, label %100

36:                                               ; preds = %2
  %37 = load i32, i32* %8, align 4
  %38 = load i32, i32* %4, align 4
  %39 = icmp eq i32 %37, %38
  br i1 %39, label %40, label %77

40:                                               ; preds = %36
  store i32 16, i32* %11, align 4
  %41 = load i32, i32* %4, align 4
  %42 = bitcast %struct.sockaddr_in* %10 to %struct.sockaddr*
  %43 = call i32 @accept4(i32 %41, %struct.sockaddr* %42, i32* %11, i32 2048)
  store i32 %43, i32* %12, align 4
  %44 = load i32, i32* %12, align 4
  %45 = icmp eq i32 %44, -1
  br i1 %45, label %46, label %47

46:                                               ; preds = %40
  call void @perror(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0))
  br label %47

47:                                               ; preds = %46, %40
  %48 = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %10, i32 0, i32 2
  %49 = getelementptr inbounds %struct.in_addr, %struct.in_addr* %48, i32 0, i32 0
  %50 = bitcast i32* %49 to i8*
  %51 = getelementptr inbounds [1024 x i8], [1024 x i8]* %5, i64 0, i64 0
  %52 = call i8* @inet_ntop(i32 2, i8* %50, i8* %51, i32 1024) #10
  %53 = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %10, i32 0, i32 1
  %54 = load i16, i16* %53, align 2
  %55 = call zeroext i16 @ntohs(i16 zeroext %54) #9
  %56 = zext i16 %55 to i32
  call void (i32, i8*, i32, i8*, ...) @_Z7log_logiPKciS0_z(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0), i32 81, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.6, i64 0, i64 0), i8* %52, i32 %56)
  %57 = call noalias nonnull i8* @_Znwm(i64 8) #8
  %58 = bitcast i8* %57 to %struct.Context*
  store %struct.Context* %58, %struct.Context** %14, align 8
  %59 = load i32, i32* %12, align 4
  %60 = load %struct.Context*, %struct.Context** %14, align 8
  %61 = getelementptr inbounds %struct.Context, %struct.Context* %60, i32 0, i32 0
  store i32 %59, i32* %61, align 4
  %62 = load i32, i32* %7, align 4
  %63 = load %struct.Context*, %struct.Context** %14, align 8
  %64 = getelementptr inbounds %struct.Context, %struct.Context* %63, i32 0, i32 1
  store i32 %62, i32* %64, align 4
  %65 = load %struct.Context*, %struct.Context** %14, align 8
  %66 = bitcast %struct.Context* %65 to i8*
  %67 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %13, i32 0, i32 1
  %68 = bitcast %union.epoll_data* %67 to i8**
  store i8* %66, i8** %68, align 1
  %69 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %13, i32 0, i32 0
  store i32 5, i32* %69, align 1
  %70 = load i32, i32* %7, align 4
  %71 = load i32, i32* %12, align 4
  %72 = call i32 @epoll_ctl(i32 %70, i32 1, i32 %71, %struct.epoll_event* %13) #10
  store i32 %72, i32* %15, align 4
  %73 = load i32, i32* %15, align 4
  %74 = icmp eq i32 %73, -1
  br i1 %74, label %75, label %76

75:                                               ; preds = %47
  call void @perror(i8* getelementptr inbounds ([45 x i8], [45 x i8]* @.str.7, i64 0, i64 0))
  br label %76

76:                                               ; preds = %75, %47
  br label %99

77:                                               ; preds = %36
  %78 = load i32, i32* %8, align 4
  store i32 %78, i32* %16, align 4
  br label %79

79:                                               ; preds = %90, %77
  %80 = load i32, i32* %16, align 4
  %81 = getelementptr inbounds [1024 x i8], [1024 x i8]* %17, i64 0, i64 0
  %82 = call i64 @recv(i32 %80, i8* %81, i64 1024, i32 0)
  %83 = trunc i64 %82 to i32
  store i32 %83, i32* %18, align 4
  %84 = icmp ne i32 %83, 0
  br i1 %84, label %85, label %88

85:                                               ; preds = %79
  %86 = load i32, i32* %18, align 4
  %87 = icmp sgt i32 %86, 0
  br label %88

88:                                               ; preds = %85, %79
  %89 = phi i1 [ false, %79 ], [ %87, %85 ]
  br i1 %89, label %90, label %98

90:                                               ; preds = %88
  %91 = getelementptr inbounds [1024 x i8], [1024 x i8]* %17, i64 0, i64 0
  %92 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.8, i64 0, i64 0), i8* %91)
  %93 = load i32, i32* %16, align 4
  %94 = getelementptr inbounds [1024 x i8], [1024 x i8]* %17, i64 0, i64 0
  %95 = getelementptr inbounds [1024 x i8], [1024 x i8]* %17, i64 0, i64 0
  %96 = call i64 @strlen(i8* %95) #11
  %97 = call i64 @send(i32 %93, i8* %94, i64 %96, i32 0)
  br label %79

98:                                               ; preds = %88
  br label %99

99:                                               ; preds = %98, %76
  br label %106

100:                                              ; preds = %2
  %101 = load i32, i32* %9, align 4
  %102 = and i32 %101, 4
  %103 = icmp ne i32 %102, 0
  br i1 %103, label %104, label %105

104:                                              ; preds = %100
  br label %105

105:                                              ; preds = %104, %100
  br label %106

106:                                              ; preds = %105, %99
  ret void
}

declare dso_local i32 @accept4(i32, %struct.sockaddr*, i32*, i32) #4

; Function Attrs: nounwind
declare dso_local i8* @inet_ntop(i32, i8*, i8*, i32) #3

; Function Attrs: nounwind readnone
declare dso_local zeroext i16 @ntohs(i16 zeroext) #2

; Function Attrs: nounwind
declare dso_local i32 @epoll_ctl(i32, i32, i32, %struct.epoll_event*) #3

declare dso_local i64 @recv(i32, i8*, i64, i32) #4

declare dso_local i32 @printf(i8*, ...) #4

declare dso_local i64 @send(i32, i8*, i64, i32) #4

; Function Attrs: nounwind readonly
declare dso_local i64 @strlen(i8*) #5

; Function Attrs: noinline optnone uwtable
define dso_local void @_Z9EventLoopi(i32 %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca %struct.Context*, align 8
  %5 = alloca %struct.epoll_event, align 1
  %6 = alloca [1024 x %struct.epoll_event], align 16
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca %struct.epoll_event*, align 8
  store i32 %0, i32* %2, align 4
  %10 = call i32 @epoll_create(i32 1) #10
  store i32 %10, i32* %3, align 4
  %11 = call noalias nonnull i8* @_Znwm(i64 8) #8
  %12 = bitcast i8* %11 to %struct.Context*
  store %struct.Context* %12, %struct.Context** %4, align 8
  %13 = load i32, i32* %2, align 4
  %14 = load %struct.Context*, %struct.Context** %4, align 8
  %15 = getelementptr inbounds %struct.Context, %struct.Context* %14, i32 0, i32 0
  store i32 %13, i32* %15, align 4
  %16 = load i32, i32* %3, align 4
  %17 = load %struct.Context*, %struct.Context** %4, align 8
  %18 = getelementptr inbounds %struct.Context, %struct.Context* %17, i32 0, i32 1
  store i32 %16, i32* %18, align 4
  %19 = bitcast %struct.Context** %4 to i8*
  %20 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %5, i32 0, i32 1
  %21 = bitcast %union.epoll_data* %20 to i8**
  store i8* %19, i8** %21, align 1
  %22 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %5, i32 0, i32 0
  store i32 5, i32* %22, align 1
  %23 = load i32, i32* %3, align 4
  %24 = load i32, i32* %2, align 4
  %25 = call i32 @epoll_ctl(i32 %23, i32 1, i32 %24, %struct.epoll_event* %5) #10
  br label %26

26:                                               ; preds = %1, %44
  %27 = load i32, i32* %3, align 4
  %28 = getelementptr inbounds [1024 x %struct.epoll_event], [1024 x %struct.epoll_event]* %6, i64 0, i64 0
  %29 = call i32 @epoll_wait(i32 %27, %struct.epoll_event* %28, i32 1024, i32 -1)
  store i32 %29, i32* %7, align 4
  store i32 0, i32* %8, align 4
  br label %30

30:                                               ; preds = %41, %26
  %31 = load i32, i32* %8, align 4
  %32 = load i32, i32* %7, align 4
  %33 = icmp slt i32 %31, %32
  br i1 %33, label %34, label %44

34:                                               ; preds = %30
  %35 = getelementptr inbounds [1024 x %struct.epoll_event], [1024 x %struct.epoll_event]* %6, i64 0, i64 0
  %36 = load i32, i32* %8, align 4
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds %struct.epoll_event, %struct.epoll_event* %35, i64 %37
  store %struct.epoll_event* %38, %struct.epoll_event** %9, align 8
  %39 = load %struct.epoll_event*, %struct.epoll_event** %9, align 8
  %40 = load i32, i32* %2, align 4
  call void @_Z11handleEventP11epoll_eventi(%struct.epoll_event* %39, i32 %40)
  br label %41

41:                                               ; preds = %34
  %42 = load i32, i32* %8, align 4
  %43 = add nsw i32 %42, 1
  store i32 %43, i32* %8, align 4
  br label %30

44:                                               ; preds = %30
  br label %26
}

; Function Attrs: nounwind
declare dso_local i32 @epoll_create(i32) #3

declare dso_local i32 @epoll_wait(i32, %struct.epoll_event*, i32, i32) #4

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main(i32 %0, i8** %1) #6 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %struct.sockaddr_in*, align 8
  %8 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  call void @_Z7InitLogv()
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.9, i64 0, i64 0), i8** %6, align 8
  %9 = load i8*, i8** %6, align 8
  %10 = load i8**, i8*** %5, align 8
  %11 = getelementptr inbounds i8*, i8** %10, i64 1
  %12 = load i8*, i8** %11, align 8
  %13 = call i32 @atoi(i8* %12) #11
  %14 = call %struct.sockaddr_in* @_Z8InitAddrPKci(i8* %9, i32 %13)
  store %struct.sockaddr_in* %14, %struct.sockaddr_in** %7, align 8
  %15 = load %struct.sockaddr_in*, %struct.sockaddr_in** %7, align 8
  %16 = call i32 @_Z14TcpServerStartP11sockaddr_in(%struct.sockaddr_in* %15)
  store i32 %16, i32* %8, align 4
  %17 = load i32, i32* %8, align 4
  call void @_Z9EventLoopi(i32 %17)
  %18 = call i32 @pthread_mutex_destroy(%union.pthread_mutex_t* @LOG_MTX) #10
  ret i32 0
}

; Function Attrs: noinline optnone uwtable
define dso_local void @_Z7InitLogv() #0 {
  call void @_Z13log_set_leveli(i32 0)
  call void @_Z13log_set_quietb(i1 zeroext false)
  %1 = call i32 @pthread_mutex_init(%union.pthread_mutex_t* @LOG_MTX, %union.pthread_mutexattr_t* null) #10
  call void @_Z12log_set_lockPFvbPvES_(void (i1, i8*)* @_Z8log_lockbPv, i8* bitcast (%union.pthread_mutex_t* @LOG_MTX to i8*))
  ret void
}

; Function Attrs: nounwind readonly
declare dso_local i32 @atoi(i8*) #5

; Function Attrs: nounwind
declare dso_local i32 @pthread_mutex_destroy(%union.pthread_mutex_t*) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @_Z8log_lockbPv(i1 zeroext %0, i8* %1) #7 {
  %3 = alloca i8, align 1
  %4 = alloca i8*, align 8
  %5 = alloca %union.pthread_mutex_t*, align 8
  %6 = zext i1 %0 to i8
  store i8 %6, i8* %3, align 1
  store i8* %1, i8** %4, align 8
  %7 = load i8*, i8** %4, align 8
  %8 = bitcast i8* %7 to %union.pthread_mutex_t*
  store %union.pthread_mutex_t* %8, %union.pthread_mutex_t** %5, align 8
  %9 = load i8, i8* %3, align 1
  %10 = trunc i8 %9 to i1
  br i1 %10, label %11, label %14

11:                                               ; preds = %2
  %12 = load %union.pthread_mutex_t*, %union.pthread_mutex_t** %5, align 8
  %13 = call i32 @pthread_mutex_lock(%union.pthread_mutex_t* %12) #10
  br label %17

14:                                               ; preds = %2
  %15 = load %union.pthread_mutex_t*, %union.pthread_mutex_t** %5, align 8
  %16 = call i32 @pthread_mutex_unlock(%union.pthread_mutex_t* %15) #10
  br label %17

17:                                               ; preds = %14, %11
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @pthread_mutex_lock(%union.pthread_mutex_t*) #3

; Function Attrs: nounwind
declare dso_local i32 @pthread_mutex_unlock(%union.pthread_mutex_t*) #3

declare dso_local void @_Z13log_set_leveli(i32) #4

declare dso_local void @_Z13log_set_quietb(i1 zeroext) #4

; Function Attrs: nounwind
declare dso_local i32 @pthread_mutex_init(%union.pthread_mutex_t*, %union.pthread_mutexattr_t*) #3

declare dso_local void @_Z12log_set_lockPFvbPvES_(void (i1, i8*)*, i8*) #4

attributes #0 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin allocsize(0) "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { builtin allocsize(0) }
attributes #9 = { nounwind readnone }
attributes #10 = { nounwind }
attributes #11 = { nounwind readonly }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}
