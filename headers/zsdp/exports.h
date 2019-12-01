
#ifndef ZSDP_PUBLIC_H
#define ZSDP_PUBLIC_H

#ifdef zsdp_STATIC_DEFINE
#  define ZSDP_PUBLIC
#  define ZSDP_LOCAL
#else
#  ifndef ZSDP_PUBLIC
#    ifdef zsdp_EXPORTS
        /* We are building this library */
#      define ZSDP_PUBLIC 
#    else
        /* We are using this library */
#      define ZSDP_PUBLIC 
#    endif
#  endif

#  ifndef ZSDP_LOCAL
#    define ZSDP_LOCAL 
#  endif
#endif

#ifndef ZSDP_DEPRECATED
#  define ZSDP_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ZSDP_DEPRECATED_EXPORT
#  define ZSDP_DEPRECATED_EXPORT ZSDP_PUBLIC ZSDP_DEPRECATED
#endif

#ifndef ZSDP_DEPRECATED_NO_EXPORT
#  define ZSDP_DEPRECATED_NO_EXPORT ZSDP_LOCAL ZSDP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ZSDP_NO_DEPRECATED
#    define ZSDP_NO_DEPRECATED
#  endif
#endif

#endif /* ZSDP_PUBLIC_H */
