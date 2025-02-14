
/*****************************************************************************
   模块名      : Algorithm type 
   文件名      : algorithmType.h
   相关文件    : 
   文件实现功能: 算法及算法控制数据类型定义
   作者        : 李雪峰
   版本        : V3.6  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2005/03/29  3.6         李雪峰        创建
   2005/04/15  3.6         徐  超      解决了与"mm.h"的冲突
   2005/04/18  3.6         徐  超      解决了使用timing.h时与"basictype.h"的冲突
   2005/04/22  3.6         陆雪忠	   解决关于u32上层定义和底层定义冲突问题
   2005/04/28  3.6		   陆雪忠	   解决关于s32,int32_t, uint32_t上层定义和底层定义冲突问题
******************************************************************************/
#ifndef _ALGORITHM_TYPE_H_
#define _ALGORITHM_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif   /* __cplusplus */

/*推荐使用*/
typedef char            s8;            
typedef short           s16;
typedef long            l32;

typedef unsigned char   u8;   
typedef unsigned short  u16;
typedef unsigned long   u32;
//end Modify

//typedef int BOOL; //xuchao modify
#if !defined(TYPE_BOOL) && !defined(__INCvxTypesh)
typedef int BOOL, *PBOOL;
#define TRUE 1
#define FALSE 0
#define TYPE_BOOL
#endif /* BOOL */

#ifdef _MSC_VER
typedef __int64			s64;
typedef unsigned __int64        u64;
#else 
typedef long long		s64;
typedef unsigned long long      u64;
#endif 

typedef double  d64;

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

//add by luxz
#ifndef NULL
  #ifdef  __cplusplus
    #define NULL    0
  #else
    #define NULL    ((void *)0)
  #endif
#endif

#ifdef _MSC_VER
  #define INLINE __inline 
#else
  #define INLINE inline
#endif
//end add

/*为兼容老版本暂时保留，不推荐使用，保留至6月1号*/
typedef char             int8_t;            
typedef short            int16_t;
//Modify by luxz
//考虑到3.6中，上层定义(int)和底层定义(long)类型不同，导致编译问题
#ifdef _MSC_VER  
typedef int             int32_t;
#else
typedef long             int32_t;
#endif
//End modify

typedef unsigned char    uint8_t;   
typedef unsigned short   uint16_t;

//Modify by luxz
//考虑到3.6中，上层定义(unsigned int)和底层定义(unsigned long)类型不同，导致编译问题
#ifdef _MSC_VER  
typedef unsigned int    uint32_t;
#else  
typedef unsigned long    uint32_t;
#endif
//End modify

#ifdef _MSC_VER
typedef unsigned __int64      uint64_t;
typedef __int64		      int64_t;
#else 
typedef unsigned long long    uint64_t;
typedef long long	      int64_t;
#endif
/*为兼容老版本暂时保留，不推荐使用，保留至6月1号*/ 

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _ALGORITHM_TYPE_H_ */
