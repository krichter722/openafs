/*
 * include/krb5.h
 *
 * Copyright 1989,1990,1995,2001, 2003  by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.	Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 *
 * General definitions for Kerberos version 5.
 */

/*
 * Copyright (C) 1998 by the FundsXpress, INC.
 *
 * All rights reserved.
 *
 * Export of this software from the United States of America may require
 * a specific license from the United States Government.  It is the
 * responsibility of any person or organization contemplating export to
 * obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of FundsXpress. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  FundsXpress makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef KRB5_GENERAL__
#define KRB5_GENERAL__

/* By default, do not expose deprecated interfaces. */
#ifndef KRB5_DEPRECATED
#define KRB5_DEPRECATED 0
#endif
/* Do not expose private interfaces.  Build system will override. */
#ifndef KRB5_PRIVATE
#define KRB5_PRIVATE 0
#endif

#if defined(__MACH__) && defined(__APPLE__)
#	include <TargetConditionals.h>
#    if TARGET_RT_MAC_CFM
#	error "Use KfM 4.0 SDK headers for CFM compilation."
#    endif
#endif

#if defined(_MSDOS) || defined(_WIN32)
#include <win-mac.h>
#endif

#ifndef KRB5_CONFIG__
#ifndef KRB5_CALLCONV
#define KRB5_CALLCONV
#define KRB5_CALLCONV_C
#endif /* !KRB5_CALLCONV */
#endif /* !KRB5_CONFIG__ */

#ifndef KRB5_CALLCONV_WRONG
#define KRB5_CALLCONV_WRONG
#endif

#ifndef THREEPARAMOPEN
#define THREEPARAMOPEN(x,y,z) open(x,y,z)
#endif

#define KRB5_OLD_CRYPTO

#include <stdlib.h>
#include <limits.h>		/* for *_MAX */

#ifndef KRB5INT_BEGIN_DECLS
#if defined(__cplusplus)
#define KRB5INT_BEGIN_DECLS	extern "C" {
#define KRB5INT_END_DECLS	}
#else
#define KRB5INT_BEGIN_DECLS
#define KRB5INT_END_DECLS
#endif
#endif

KRB5INT_BEGIN_DECLS

#if TARGET_OS_MAC
#    pragma options align=mac68k
#endif

/* from profile.h */
struct _profile_t;
/* typedef struct _profile_t *profile_t; */

/*
 * begin wordsize.h
 */

/*
 * Word-size related definition.
 */

typedef	unsigned char	krb5_octet;

#if INT_MAX == 0x7fff
typedef	int	krb5_int16;
typedef	unsigned int	krb5_ui_2;
#elif SHRT_MAX == 0x7fff
typedef	short	krb5_int16;
typedef	unsigned short	krb5_ui_2;
#else
#error undefined 16 bit type
#endif

#if INT_MAX == 0x7fffffffL
typedef	int	krb5_int32;
typedef	unsigned int	krb5_ui_4;
#elif LONG_MAX == 0x7fffffffL
typedef	long	krb5_int32;
typedef	unsigned long	krb5_ui_4;
#elif SHRT_MAX == 0x7fffffffL
typedef	short	krb5_int32;
typedef	unsigned short	krb5_ui_4;
#else
#error: undefined 32 bit type
#endif

#define VALID_INT_BITS	  INT_MAX
#define VALID_UINT_BITS	  UINT_MAX

#define KRB5_INT32_MAX	2147483647
/* this strange form is necessary since - is a unary operator, not a sign
   indicator */
#define KRB5_INT32_MIN	(-KRB5_INT32_MAX-1)

#define KRB5_INT16_MAX 65535
/* this strange form is necessary since - is a unary operator, not a sign
   indicator */
#define KRB5_INT16_MIN	(-KRB5_INT16_MAX-1)

/*
 * end wordsize.h
 */

/*
 * begin "base-defs.h"
 */

/*
 * Basic definitions for Kerberos V5 library
 */

#ifndef FALSE
#define	FALSE	0
#endif
#ifndef TRUE
#define	TRUE	1
#endif

typedef	unsigned int krb5_boolean;
typedef	unsigned int krb5_msgtype;
typedef	unsigned int krb5_kvno;

typedef	krb5_int32 krb5_addrtype;
typedef krb5_int32 krb5_enctype;
typedef krb5_int32 krb5_cksumtype;
typedef krb5_int32 krb5_authdatatype;
typedef krb5_int32 krb5_keyusage;

typedef krb5_int32	krb5_preauthtype; /* This may change, later on */
typedef	krb5_int32	krb5_flags;
typedef krb5_int32	krb5_timestamp;
typedef	krb5_int32	krb5_error_code;
typedef krb5_int32	krb5_deltat;

typedef krb5_error_code	krb5_magic;

typedef struct _krb5_data {
	krb5_magic magic;
	unsigned int length;
	char *data;
} krb5_data;

/*
 * Hack length for crypto library to use the afs_string_to_key It is
 * equivalent to -1 without possible sign extension
 * We also overload for an unset salt type length - which is also -1, but
 * hey, why not....
*/
#define SALT_TYPE_AFS_LENGTH UINT_MAX
#define SALT_TYPE_NO_LENGTH  UINT_MAX

typedef	void * krb5_pointer;
typedef void const * krb5_const_pointer;

typedef struct krb5_principal_data {
    krb5_magic magic;
    krb5_data realm;
    krb5_data *data;		/* An array of strings */
    krb5_int32 length;
    krb5_int32 type;
} krb5_principal_data;

typedef	krb5_principal_data * krb5_principal;

/*
 * Per V5 spec on definition of principal types
 */

/* Name type not known */
#define KRB5_NT_UNKNOWN		0
/* Just the name of the principal as in DCE, or for users */
#define KRB5_NT_PRINCIPAL	1
/* Service and other unique instance (krbtgt) */
#define KRB5_NT_SRV_INST	2
/* Service with host name as instance (telnet, rcommands) */
#define KRB5_NT_SRV_HST		3
/* Service with host as remaining components */
#define KRB5_NT_SRV_XHST	4
/* Unique ID */
#define KRB5_NT_UID		5

/* constant version thereof: */
typedef const krb5_principal_data *krb5_const_principal;

#define krb5_princ_realm(context, princ) (&(princ)->realm)
#define krb5_princ_set_realm(context, princ,value) ((princ)->realm = *(value))
#define krb5_princ_set_realm_length(context, princ,value) (princ)->realm.length = (value)
#define krb5_princ_set_realm_data(context, princ,value) (princ)->realm.data = (value)
#define	krb5_princ_size(context, princ) (princ)->length
#define	krb5_princ_type(context, princ) (princ)->type
#define	krb5_princ_name(context, princ) (princ)->data
#define	krb5_princ_component(context, princ,i)		\
	    (((i) < krb5_princ_size(context, princ))	\
	     ? (princ)->data + (i)			\
	     : NULL)

/*
 * end "base-defs.h"
 */

/*
 * begin "hostaddr.h"
 */

/* structure for address */
typedef struct _krb5_address {
    krb5_magic magic;
    krb5_addrtype addrtype;
    unsigned int length;
    krb5_octet *contents;
} krb5_address;

/* per Kerberos v5 protocol spec */
#define	ADDRTYPE_INET		0x0002
#define	ADDRTYPE_CHAOS		0x0005
#define	ADDRTYPE_XNS		0x0006
#define	ADDRTYPE_ISO		0x0007
#define ADDRTYPE_DDP		0x0010
#define ADDRTYPE_INET6		0x0018
/* not yet in the spec... */
#define ADDRTYPE_ADDRPORT	0x0100
#define ADDRTYPE_IPPORT		0x0101

/* macros to determine if a type is a local type */
#define ADDRTYPE_IS_LOCAL(addrtype) (addrtype & 0x8000)

/*
 * end "hostaddr.h"
 */


struct _krb5_context;
typedef struct _krb5_context * krb5_context;

struct _krb5_auth_context;
typedef struct _krb5_auth_context * krb5_auth_context;

struct _krb5_cryptosystem_entry;

/*
 * begin "encryption.h"
 */

typedef struct _krb5_keyblock {
    krb5_magic magic;
    krb5_enctype enctype;
    unsigned int length;
    krb5_octet *contents;
} krb5_keyblock;

#ifdef KRB5_OLD_CRYPTO
typedef struct _krb5_encrypt_block {
    krb5_magic magic;
    krb5_enctype crypto_entry;		/* to call krb5_encrypt_size, you need
					   this.  it was a pointer, but it
					   doesn't have to be.  gross. */
    krb5_keyblock *key;
} krb5_encrypt_block;
#endif

typedef struct _krb5_checksum {
    krb5_magic magic;
    krb5_cksumtype checksum_type;	/* checksum type */
    unsigned int length;
    krb5_octet *contents;
} krb5_checksum;

typedef struct _krb5_enc_data {
    krb5_magic magic;
    krb5_enctype enctype;
    krb5_kvno kvno;
    krb5_data ciphertext;
} krb5_enc_data;

/* per Kerberos v5 protocol spec */
#define	ENCTYPE_NULL		0x0000
#define	ENCTYPE_DES_CBC_CRC	0x0001	/* DES cbc mode with CRC-32 */
#define	ENCTYPE_DES_CBC_MD4	0x0002	/* DES cbc mode with RSA-MD4 */
#define	ENCTYPE_DES_CBC_MD5	0x0003	/* DES cbc mode with RSA-MD5 */
#define	ENCTYPE_DES_CBC_RAW	0x0004	/* DES cbc mode raw */
/* XXX deprecated? */
#define	ENCTYPE_DES3_CBC_SHA	0x0005	/* DES-3 cbc mode with NIST-SHA */
#define	ENCTYPE_DES3_CBC_RAW	0x0006	/* DES-3 cbc mode raw */
#define ENCTYPE_DES_HMAC_SHA1	0x0008
#define ENCTYPE_DES3_CBC_SHA1	0x0010
#define ENCTYPE_AES128_CTS_HMAC_SHA1_96	0x0011
#define ENCTYPE_AES256_CTS_HMAC_SHA1_96	0x0012
#define ENCTYPE_ARCFOUR_HMAC	0x0017
#define ENCTYPE_ARCFOUR_HMAC_EXP 0x0018
#define ENCTYPE_UNKNOWN		0x01ff

#define	CKSUMTYPE_CRC32		0x0001
#define	CKSUMTYPE_RSA_MD4	0x0002
#define	CKSUMTYPE_RSA_MD4_DES	0x0003
#define	CKSUMTYPE_DESCBC	0x0004
/* des-mac-k */
/* rsa-md4-des-k */
#define	CKSUMTYPE_RSA_MD5	0x0007
#define	CKSUMTYPE_RSA_MD5_DES	0x0008
#define CKSUMTYPE_NIST_SHA	0x0009
#define CKSUMTYPE_HMAC_SHA1_DES3	0x000c
#define CKSUMTYPE_HMAC_SHA1_96_AES128	0x000f
#define CKSUMTYPE_HMAC_SHA1_96_AES256	0x0010
#define CKSUMTYPE_HMAC_MD5_ARCFOUR -138 /*Microsoft md5 hmac cksumtype*/

/* The following are entropy source designations. Whenever
 * krb5_C_random_add_entropy is called, one of these source  ids is passed
 * in.  This  allows the library  to better estimate bits of
 * entropy in the sample and to keep track of what sources of entropy have
 * contributed enough entropy.  Sources marked internal MUST NOT be
 * used by applications outside the Kerberos library
*/

enum {
  KRB5_C_RANDSOURCE_OLDAPI = 0, /*calls to krb5_C_RANDOM_SEED (INTERNAL)*/
  KRB5_C_RANDSOURCE_OSRAND = 1, /* /dev/random or equivalent (internal)*/
  KRB5_C_RANDSOURCE_TRUSTEDPARTY = 2, /* From KDC or other trusted party*/
  /*This source should be used carefully; data in this category
   * should be from a third party trusted to give random bits
   * For example keys issued by the KDC in the application server.
   */
  KRB5_C_RANDSOURCE_TIMING = 3, /* Timing of operations*/
  KRB5_C_RANDSOURCE_EXTERNAL_PROTOCOL = 4, /*Protocol data possibly from attacker*/
  KRB5_C_RANDSOURCE_MAX = 5 /*Do not use; maximum source ID*/
};

#ifndef krb5_roundup
/* round x up to nearest multiple of y */
#define krb5_roundup(x, y) ((((x) + (y) - 1)/(y))*(y))
#endif /* roundup */

/* macro function definitions to help clean up code */

#if 1
#define krb5_x(ptr,args) ((ptr)?((*(ptr)) args):(abort(),1))
#define krb5_xc(ptr,args) ((ptr)?((*(ptr)) args):(abort(),(char*)0))
#else
#define krb5_x(ptr,args) ((*(ptr)) args)
#define krb5_xc(ptr,args) ((*(ptr)) args)
#endif

krb5_error_code KRB5_CALLCONV
    krb5_c_encrypt
    (krb5_context context, const krb5_keyblock *key,
		    krb5_keyusage usage, const krb5_data *cipher_state,
		    const krb5_data *input, krb5_enc_data *output);

krb5_error_code KRB5_CALLCONV
    krb5_c_decrypt
    (krb5_context context, const krb5_keyblock *key,
		    krb5_keyusage usage, const krb5_data *cipher_state,
		    const krb5_enc_data *input, krb5_data *output);

krb5_error_code KRB5_CALLCONV
    krb5_c_encrypt_length
    (krb5_context context, krb5_enctype enctype,
		    size_t inputlen, size_t *length);

krb5_error_code KRB5_CALLCONV
    krb5_c_block_size
    (krb5_context context, krb5_enctype enctype,
		    size_t *blocksize);

krb5_error_code KRB5_CALLCONV
	krb5_c_init_state
(krb5_context context,
const krb5_keyblock *key, krb5_keyusage usage,
krb5_data *new_state);

krb5_error_code KRB5_CALLCONV
	krb5_c_free_state
(krb5_context context, const krb5_keyblock *key, krb5_data *state);

krb5_error_code KRB5_CALLCONV
    krb5_c_prf (krb5_context, const krb5_keyblock *,
		krb5_data *in, krb5_data *out);

krb5_error_code KRB5_CALLCONV
    krb5_c_prf_length (krb5_context, krb5_enctype, size_t *outlen);

krb5_error_code KRB5_CALLCONV
    krb5_c_make_random_key
    (krb5_context context, krb5_enctype enctype,
		    krb5_keyblock *k5_random_key);

/* Register a new entropy sample  with the PRNG. may cause
* the PRNG to be reseeded, although this is not guaranteed.  See previous randsource definitions
* for information on how each source should be used.
*/
krb5_error_code KRB5_CALLCONV
	krb5_c_random_add_entropy
(krb5_context context, unsigned int  randsource_id, const krb5_data *data);


krb5_error_code KRB5_CALLCONV
    krb5_c_random_make_octets
    (krb5_context context, krb5_data *data);

/*
* Collect entropy from the OS if possible. strong requests that as strong
* of a source of entropy  as available be used.  Setting strong may
* increase the probability of blocking and should not  be used for normal
* applications.  Good uses include seeding the PRNG for kadmind
* and realm setup.
* If successful is non-null, then successful is set to 1 if the OS provided
* entropy else zero.
*/
krb5_error_code KRB5_CALLCONV
krb5_c_random_os_entropy
(krb5_context context, int strong, int *success);

/*deprecated*/ krb5_error_code KRB5_CALLCONV
    krb5_c_random_seed
    (krb5_context context, krb5_data *data);

krb5_error_code KRB5_CALLCONV
    krb5_c_string_to_key
    (krb5_context context, krb5_enctype enctype,
		    const krb5_data *string, const krb5_data *salt,
		    krb5_keyblock *key);
krb5_error_code KRB5_CALLCONV
krb5_c_string_to_key_with_params(krb5_context context,
				 krb5_enctype enctype,
				 const krb5_data *string,
				 const krb5_data *salt,
				 const krb5_data *params,
				 krb5_keyblock *key);

krb5_error_code KRB5_CALLCONV
    krb5_c_enctype_compare
    (krb5_context context, krb5_enctype e1, krb5_enctype e2,
		    krb5_boolean *similar);

krb5_error_code KRB5_CALLCONV
    krb5_c_make_checksum
    (krb5_context context, krb5_cksumtype cksumtype,
		    const krb5_keyblock *key, krb5_keyusage usage,
		    const krb5_data *input, krb5_checksum *cksum);

krb5_error_code KRB5_CALLCONV
    krb5_c_verify_checksum
    (krb5_context context,
		    const krb5_keyblock *key, krb5_keyusage usage,
		    const krb5_data *data,
		    const krb5_checksum *cksum,
		    krb5_boolean *valid);

krb5_error_code KRB5_CALLCONV
    krb5_c_checksum_length
    (krb5_context context, krb5_cksumtype cksumtype,
		    size_t *length);

krb5_error_code KRB5_CALLCONV
    krb5_c_keyed_checksum_types
    (krb5_context context, krb5_enctype enctype,
		    unsigned int *count, krb5_cksumtype **cksumtypes);

#define KRB5_KEYUSAGE_AS_REQ_PA_ENC_TS		1
#define KRB5_KEYUSAGE_KDC_REP_TICKET		2
#define KRB5_KEYUSAGE_AS_REP_ENCPART		3
#define KRB5_KEYUSAGE_TGS_REQ_AD_SESSKEY	4
#define KRB5_KEYUSAGE_TGS_REQ_AD_SUBKEY		5
#define KRB5_KEYUSAGE_TGS_REQ_AUTH_CKSUM	6
#define KRB5_KEYUSAGE_TGS_REQ_AUTH		7
#define KRB5_KEYUSAGE_TGS_REP_ENCPART_SESSKEY	8
#define KRB5_KEYUSAGE_TGS_REP_ENCPART_SUBKEY	9
#define KRB5_KEYUSAGE_AP_REQ_AUTH_CKSUM		10
#define KRB5_KEYUSAGE_AP_REQ_AUTH		11
#define KRB5_KEYUSAGE_AP_REP_ENCPART		12
#define KRB5_KEYUSAGE_KRB_PRIV_ENCPART		13
#define KRB5_KEYUSAGE_KRB_CRED_ENCPART		14
#define KRB5_KEYUSAGE_KRB_SAFE_CKSUM		15
#define KRB5_KEYUSAGE_APP_DATA_ENCRYPT		16
#define KRB5_KEYUSAGE_APP_DATA_CKSUM		17
#define KRB5_KEYUSAGE_KRB_ERROR_CKSUM		18
#define KRB5_KEYUSAGE_AD_KDCISSUED_CKSUM	19
#define KRB5_KEYUSAGE_AD_MTE			20
#define KRB5_KEYUSAGE_AD_ITE			21

/* XXX need to register these */

#define KRB5_KEYUSAGE_GSS_TOK_MIC		22
#define KRB5_KEYUSAGE_GSS_TOK_WRAP_INTEG	23
#define KRB5_KEYUSAGE_GSS_TOK_WRAP_PRIV		24

/* Defined in hardware preauth draft */

#define KRB5_KEYUSAGE_PA_SAM_CHALLENGE_CKSUM	25
#define KRB5_KEYUSAGE_PA_SAM_CHALLENGE_TRACKID	26
#define KRB5_KEYUSAGE_PA_SAM_RESPONSE		27

krb5_boolean KRB5_CALLCONV krb5_c_valid_enctype
	(krb5_enctype ktype);
krb5_boolean KRB5_CALLCONV krb5_c_valid_cksumtype
	(krb5_cksumtype ctype);
krb5_boolean KRB5_CALLCONV krb5_c_is_coll_proof_cksum
	(krb5_cksumtype ctype);
krb5_boolean KRB5_CALLCONV krb5_c_is_keyed_cksum
	(krb5_cksumtype ctype);

#if KRB5_PRIVATE
/* Use the above four instead.  */
krb5_boolean KRB5_CALLCONV valid_enctype
	(krb5_enctype ktype);
krb5_boolean KRB5_CALLCONV valid_cksumtype
	(krb5_cksumtype ctype);
krb5_boolean KRB5_CALLCONV is_coll_proof_cksum
	(krb5_cksumtype ctype);
krb5_boolean KRB5_CALLCONV is_keyed_cksum
	(krb5_cksumtype ctype);
#endif

#ifdef KRB5_OLD_CRYPTO
/*
 * old cryptosystem routine prototypes.  These are now layered
 * on top of the functions above.
 */
krb5_error_code KRB5_CALLCONV krb5_encrypt
	(krb5_context context,
		krb5_const_pointer inptr,
		krb5_pointer outptr,
		size_t size,
		krb5_encrypt_block * eblock,
		krb5_pointer ivec);
krb5_error_code KRB5_CALLCONV krb5_decrypt
	(krb5_context context,
		krb5_const_pointer inptr,
		krb5_pointer outptr,
		size_t size,
		krb5_encrypt_block * eblock,
		krb5_pointer ivec);
krb5_error_code KRB5_CALLCONV krb5_process_key
	(krb5_context context,
		krb5_encrypt_block * eblock,
		const krb5_keyblock * key);
krb5_error_code KRB5_CALLCONV krb5_finish_key
	(krb5_context context,
		krb5_encrypt_block * eblock);
krb5_error_code KRB5_CALLCONV krb5_string_to_key
	(krb5_context context,
		const krb5_encrypt_block * eblock,
		krb5_keyblock * keyblock,
		const krb5_data * data,
		const krb5_data * salt);
krb5_error_code KRB5_CALLCONV krb5_init_random_key
	(krb5_context context,
		const krb5_encrypt_block * eblock,
		const krb5_keyblock * keyblock,
		krb5_pointer * ptr);
krb5_error_code KRB5_CALLCONV krb5_finish_random_key
	(krb5_context context,
		const krb5_encrypt_block * eblock,
		krb5_pointer * ptr);
krb5_error_code KRB5_CALLCONV krb5_random_key
	(krb5_context context,
		const krb5_encrypt_block * eblock,
		krb5_pointer ptr,
		krb5_keyblock ** keyblock);
krb5_enctype KRB5_CALLCONV krb5_eblock_enctype
	(krb5_context context,
		const krb5_encrypt_block * eblock);
krb5_error_code KRB5_CALLCONV krb5_use_enctype
	(krb5_context context,
		krb5_encrypt_block * eblock,
		krb5_enctype enctype);
size_t KRB5_CALLCONV krb5_encrypt_size
	(size_t length,
		krb5_enctype crypto);
size_t KRB5_CALLCONV krb5_checksum_size
	(krb5_context context,
		krb5_cksumtype ctype);
krb5_error_code KRB5_CALLCONV krb5_calculate_checksum
	(krb5_context context,
		krb5_cksumtype ctype,
		krb5_const_pointer in, size_t in_length,
		krb5_const_pointer seed, size_t seed_length,
		krb5_checksum * outcksum);
krb5_error_code KRB5_CALLCONV krb5_verify_checksum
	(krb5_context context,
		krb5_cksumtype ctype,
		const krb5_checksum * cksum,
		krb5_const_pointer in, size_t in_length,
		krb5_const_pointer seed, size_t seed_length);

#if KRB5_PRIVATE
krb5_error_code KRB5_CALLCONV krb5_random_confounder
	(size_t, krb5_pointer);

krb5_error_code krb5_encrypt_data
	(krb5_context context, krb5_keyblock *key,
		krb5_pointer ivec, krb5_data *data,
		krb5_enc_data *enc_data);

krb5_error_code krb5_decrypt_data
	(krb5_context context, krb5_keyblock *key,
		krb5_pointer ivec, krb5_enc_data *data,
		krb5_data *enc_data);
#endif

#endif /* KRB5_OLD_CRYPTO */

/*
 * end "encryption.h"
 */

/*
 * begin "fieldbits.h"
 */

/* kdc_options for kdc_request */
/* options is 32 bits; each host is responsible to put the 4 bytes
   representing these bits into net order before transmission */
/* #define	KDC_OPT_RESERVED	0x80000000 */
#define	KDC_OPT_FORWARDABLE		0x40000000
#define	KDC_OPT_FORWARDED		0x20000000
#define	KDC_OPT_PROXIABLE		0x10000000
#define	KDC_OPT_PROXY			0x08000000
#define	KDC_OPT_ALLOW_POSTDATE		0x04000000
#define	KDC_OPT_POSTDATED		0x02000000
/* #define	KDC_OPT_UNUSED		0x01000000 */
#define	KDC_OPT_RENEWABLE		0x00800000
/* #define	KDC_OPT_UNUSED		0x00400000 */
/* #define	KDC_OPT_RESERVED	0x00200000 */
/* #define	KDC_OPT_RESERVED	0x00100000 */
/* #define	KDC_OPT_RESERVED	0x00080000 */
/* #define	KDC_OPT_RESERVED	0x00040000 */
#define	KDC_OPT_REQUEST_ANONYMOUS	0x00020000
/* #define	KDC_OPT_RESERVED	0x00010000 */
/* #define	KDC_OPT_RESERVED	0x00008000 */
/* #define	KDC_OPT_RESERVED	0x00004000 */
/* #define	KDC_OPT_RESERVED	0x00002000 */
/* #define	KDC_OPT_RESERVED	0x00001000 */
/* #define	KDC_OPT_RESERVED	0x00000800 */
/* #define	KDC_OPT_RESERVED	0x00000400 */
/* #define	KDC_OPT_RESERVED	0x00000200 */
/* #define	KDC_OPT_RESERVED	0x00000100 */
/* #define	KDC_OPT_RESERVED	0x00000080 */
/* #define	KDC_OPT_RESERVED	0x00000040 */
#define	KDC_OPT_DISABLE_TRANSITED_CHECK	0x00000020
#define	KDC_OPT_RENEWABLE_OK		0x00000010
#define	KDC_OPT_ENC_TKT_IN_SKEY		0x00000008
/* #define	KDC_OPT_UNUSED		0x00000004 */
#define	KDC_OPT_RENEW			0x00000002
#define	KDC_OPT_VALIDATE		0x00000001

/*
 * Mask of ticket flags in the TGT which should be converted into KDC
 * options when using the TGT to get derivitive tickets.
 *
 *  New mask = KDC_OPT_FORWARDABLE | KDC_OPT_PROXIABLE |
 *	       KDC_OPT_ALLOW_POSTDATE | KDC_OPT_RENEWABLE
 */
#define KDC_TKT_COMMON_MASK		0x54800000

/* definitions for ap_options fields */
/* ap_options are 32 bits; each host is responsible to put the 4 bytes
   representing these bits into net order before transmission */
#define	AP_OPTS_RESERVED		0x80000000
#define	AP_OPTS_USE_SESSION_KEY		0x40000000
#define	AP_OPTS_MUTUAL_REQUIRED		0x20000000
/* #define	AP_OPTS_RESERVED	0x10000000 */
/* #define	AP_OPTS_RESERVED	0x08000000 */
/* #define	AP_OPTS_RESERVED	0x04000000 */
/* #define	AP_OPTS_RESERVED	0x02000000 */
/* #define	AP_OPTS_RESERVED	0x01000000 */
/* #define	AP_OPTS_RESERVED	0x00800000 */
/* #define	AP_OPTS_RESERVED	0x00400000 */
/* #define	AP_OPTS_RESERVED	0x00200000 */
/* #define	AP_OPTS_RESERVED	0x00100000 */
/* #define	AP_OPTS_RESERVED	0x00080000 */
/* #define	AP_OPTS_RESERVED	0x00040000 */
/* #define	AP_OPTS_RESERVED	0x00020000 */
/* #define	AP_OPTS_RESERVED	0x00010000 */
/* #define	AP_OPTS_RESERVED	0x00008000 */
/* #define	AP_OPTS_RESERVED	0x00004000 */
/* #define	AP_OPTS_RESERVED	0x00002000 */
/* #define	AP_OPTS_RESERVED	0x00001000 */
/* #define	AP_OPTS_RESERVED	0x00000800 */
/* #define	AP_OPTS_RESERVED	0x00000400 */
/* #define	AP_OPTS_RESERVED	0x00000200 */
/* #define	AP_OPTS_RESERVED	0x00000100 */
/* #define	AP_OPTS_RESERVED	0x00000080 */
/* #define	AP_OPTS_RESERVED	0x00000040 */
/* #define	AP_OPTS_RESERVED	0x00000020 */
/* #define	AP_OPTS_RESERVED	0x00000010 */
/* #define	AP_OPTS_RESERVED	0x00000008 */
/* #define	AP_OPTS_RESERVED	0x00000004 */
/* #define	AP_OPTS_RESERVED	0x00000002 */
#define AP_OPTS_USE_SUBKEY	0x00000001

#define AP_OPTS_WIRE_MASK	0xfffffff0

/* definitions for ad_type fields. */
#define	AD_TYPE_RESERVED	0x8000
#define	AD_TYPE_EXTERNAL	0x4000
#define	AD_TYPE_REGISTERED	0x2000

#define AD_TYPE_FIELD_TYPE_MASK	0x1fff

/* Ticket flags */
/* flags are 32 bits; each host is responsible to put the 4 bytes
   representing these bits into net order before transmission */
/* #define	TKT_FLG_RESERVED	0x80000000 */
#define	TKT_FLG_FORWARDABLE		0x40000000
#define	TKT_FLG_FORWARDED		0x20000000
#define	TKT_FLG_PROXIABLE		0x10000000
#define	TKT_FLG_PROXY			0x08000000
#define	TKT_FLG_MAY_POSTDATE		0x04000000
#define	TKT_FLG_POSTDATED		0x02000000
#define	TKT_FLG_INVALID			0x01000000
#define	TKT_FLG_RENEWABLE		0x00800000
#define	TKT_FLG_INITIAL			0x00400000
#define	TKT_FLG_PRE_AUTH		0x00200000
#define	TKT_FLG_HW_AUTH			0x00100000
#define	TKT_FLG_TRANSIT_POLICY_CHECKED	0x00080000
#define	TKT_FLG_OK_AS_DELEGATE		0x00040000
#define	TKT_FLG_ANONYMOUS		0x00020000
/* #define	TKT_FLG_RESERVED	0x00010000 */
/* #define	TKT_FLG_RESERVED	0x00008000 */
/* #define	TKT_FLG_RESERVED	0x00004000 */
/* #define	TKT_FLG_RESERVED	0x00002000 */
/* #define	TKT_FLG_RESERVED	0x00001000 */
/* #define	TKT_FLG_RESERVED	0x00000800 */
/* #define	TKT_FLG_RESERVED	0x00000400 */
/* #define	TKT_FLG_RESERVED	0x00000200 */
/* #define	TKT_FLG_RESERVED	0x00000100 */
/* #define	TKT_FLG_RESERVED	0x00000080 */
/* #define	TKT_FLG_RESERVED	0x00000040 */
/* #define	TKT_FLG_RESERVED	0x00000020 */
/* #define	TKT_FLG_RESERVED	0x00000010 */
/* #define	TKT_FLG_RESERVED	0x00000008 */
/* #define	TKT_FLG_RESERVED	0x00000004 */
/* #define	TKT_FLG_RESERVED	0x00000002 */
/* #define	TKT_FLG_RESERVED	0x00000001 */

/* definitions for lr_type fields. */
#define	LR_TYPE_THIS_SERVER_ONLY	0x8000

#define LR_TYPE_INTERPRETATION_MASK	0x7fff

/* definitions for ad_type fields. */
#define	AD_TYPE_EXTERNAL	0x4000
#define	AD_TYPE_REGISTERED	0x2000

#define AD_TYPE_FIELD_TYPE_MASK	0x1fff
#define AD_TYPE_INTERNAL_MASK	0x3fff

/* definitions for msec direction bit for KRB_SAFE, KRB_PRIV */
#define	MSEC_DIRBIT		0x8000
#define	MSEC_VAL_MASK		0x7fff

/*
 * end "fieldbits.h"
 */

/*
 * begin "proto.h"
 */

/* Protocol version number */
#define	KRB5_PVNO	5

/* Message types */

#define	KRB5_AS_REQ	((krb5_msgtype)10) /* Req for initial authentication */
#define	KRB5_AS_REP	((krb5_msgtype)11) /* Response to KRB_AS_REQ request */
#define	KRB5_TGS_REQ	((krb5_msgtype)12) /* TGS request to server */
#define	KRB5_TGS_REP	((krb5_msgtype)13) /* Response to KRB_TGS_REQ req */
#define	KRB5_AP_REQ	((krb5_msgtype)14) /* application request to server */
#define	KRB5_AP_REP	((krb5_msgtype)15) /* Response to KRB_AP_REQ_MUTUAL */
#define	KRB5_SAFE	((krb5_msgtype)20) /* Safe application message */
#define	KRB5_PRIV	((krb5_msgtype)21) /* Private application message */
#define	KRB5_CRED	((krb5_msgtype)22) /* Credential forwarding message */
#define	KRB5_ERROR	((krb5_msgtype)30) /* Error response */

/* LastReq types */
#define KRB5_LRQ_NONE			0
#define KRB5_LRQ_ALL_LAST_TGT		1
#define KRB5_LRQ_ONE_LAST_TGT		(-1)
#define KRB5_LRQ_ALL_LAST_INITIAL	2
#define KRB5_LRQ_ONE_LAST_INITIAL	(-2)
#define KRB5_LRQ_ALL_LAST_TGT_ISSUED	3
#define KRB5_LRQ_ONE_LAST_TGT_ISSUED	(-3)
#define KRB5_LRQ_ALL_LAST_RENEWAL	4
#define KRB5_LRQ_ONE_LAST_RENEWAL	(-4)
#define KRB5_LRQ_ALL_LAST_REQ		5
#define KRB5_LRQ_ONE_LAST_REQ		(-5)
#define KRB5_LRQ_ALL_PW_EXPTIME		6
#define KRB5_LRQ_ONE_PW_EXPTIME		(-6)

/* PADATA types */
#define KRB5_PADATA_NONE		0
#define	KRB5_PADATA_AP_REQ		1
#define	KRB5_PADATA_TGS_REQ		KRB5_PADATA_AP_REQ
#define KRB5_PADATA_ENC_TIMESTAMP	2
#define	KRB5_PADATA_PW_SALT		3
#if 0				/* Not used */
#define KRB5_PADATA_ENC_ENCKEY		4  /* Key encrypted within itself */
#endif
#define KRB5_PADATA_ENC_UNIX_TIME	5  /* timestamp encrypted in key */
#define KRB5_PADATA_ENC_SANDIA_SECURID	6  /* SecurId passcode */
#define KRB5_PADATA_SESAME		7  /* Sesame project */
#define KRB5_PADATA_OSF_DCE		8  /* OSF DCE */
#define KRB5_CYBERSAFE_SECUREID		9  /* Cybersafe */
#define	KRB5_PADATA_AFS3_SALT		10 /* Cygnus */
#define KRB5_PADATA_ETYPE_INFO		11 /* Etype info for preauth */
#define KRB5_PADATA_SAM_CHALLENGE	12 /* draft challenge system */
#define KRB5_PADATA_SAM_RESPONSE	13 /* draft challenge system response */
#define KRB5_PADATA_PK_AS_REQ		14 /* PKINIT */
#define KRB5_PADATA_PK_AS_REP		15 /* PKINIT */
#define KRB5_PADATA_ETYPE_INFO2 19
#define KRB5_PADATA_SAM_CHALLENGE_2	30 /* draft challenge system, updated */
#define KRB5_PADATA_SAM_RESPONSE_2	31 /* draft challenge system, updated */

#define	KRB5_SAM_USE_SAD_AS_KEY		0x80000000
#define	KRB5_SAM_SEND_ENCRYPTED_SAD	0x40000000
#define	KRB5_SAM_MUST_PK_ENCRYPT_SAD	0x20000000 /* currently must be zero */

/* Reserved for SPX pre-authentication. */
#define KRB5_PADATA_DASS		16

/* Transited encoding types */
#define	KRB5_DOMAIN_X500_COMPRESS	1

/* alternate authentication types */
#define	KRB5_ALTAUTH_ATT_CHALLENGE_RESPONSE	64

/* authorization data types */
#define	KRB5_AUTHDATA_OSF_DCE	64
#define KRB5_AUTHDATA_SESAME	65

/* password change constants */

#define KRB5_KPASSWD_SUCCESS		0
#define KRB5_KPASSWD_MALFORMED		1
#define KRB5_KPASSWD_HARDERROR		2
#define KRB5_KPASSWD_AUTHERROR		3
#define KRB5_KPASSWD_SOFTERROR		4
/* These are Microsoft's extensions in RFC 3244, and it looks like
   they'll become standardized, possibly with other additions.  */
#define KRB5_KPASSWD_ACCESSDENIED	5	/* unused */
#define KRB5_KPASSWD_BAD_VERSION	6
#define KRB5_KPASSWD_INITIAL_FLAG_NEEDED 7	/* unused */

/*
 * end "proto.h"
 */

/* Time set */
typedef struct _krb5_ticket_times {
    krb5_timestamp authtime; /* XXX ? should ktime in KDC_REP == authtime
				in ticket? otherwise client can't get this */
    krb5_timestamp starttime;		/* optional in ticket, if not present,
					   use authtime */
    krb5_timestamp endtime;
    krb5_timestamp renew_till;
} krb5_ticket_times;

/* structure for auth data */
typedef struct _krb5_authdata {
    krb5_magic magic;
    krb5_authdatatype ad_type;
    unsigned int length;
    krb5_octet *contents;
} krb5_authdata;

/* structure for transited encoding */
typedef struct _krb5_transited {
    krb5_magic magic;
    krb5_octet tr_type;
    krb5_data tr_contents;
} krb5_transited;

typedef struct _krb5_enc_tkt_part {
    krb5_magic magic;
    /* to-be-encrypted portion */
    krb5_flags flags;			/* flags */
    krb5_keyblock *session;		/* session key: includes enctype */
    krb5_principal client;		/* client name/realm */
    krb5_transited transited;		/* list of transited realms */
    krb5_ticket_times times;		/* auth, start, end, renew_till */
    krb5_address **caddrs;	/* array of ptrs to addresses */
    krb5_authdata **authorization_data; /* auth data */
} krb5_enc_tkt_part;

typedef struct _krb5_ticket {
    krb5_magic magic;
    /* cleartext portion */
    krb5_principal server;		/* server name/realm */
    krb5_enc_data enc_part;		/* encryption type, kvno, encrypted
					   encoding */
    krb5_enc_tkt_part *enc_part2;	/* ptr to decrypted version, if
					   available */
} krb5_ticket;

/* the unencrypted version */
typedef struct _krb5_authenticator {
    krb5_magic magic;
    krb5_principal client;		/* client name/realm */
    krb5_checksum *checksum;	/* checksum, includes type, optional */
    krb5_int32 cusec;			/* client usec portion */
    krb5_timestamp ctime;		/* client sec portion */
    krb5_keyblock *subkey;		/* true session key, optional */
    krb5_ui_4 seq_number;		/* sequence #, optional */
    krb5_authdata **authorization_data; /* New add by Ari, auth data */
} krb5_authenticator;

typedef struct _krb5_tkt_authent {
    krb5_magic magic;
    krb5_ticket *ticket;
    krb5_authenticator *authenticator;
    krb5_flags ap_options;
} krb5_tkt_authent;

/* credentials:	 Ticket, session key, etc. */
typedef struct _krb5_creds {
    krb5_magic magic;
    krb5_principal client;		/* client's principal identifier */
    krb5_principal server;		/* server's principal identifier */
    krb5_keyblock keyblock;		/* session encryption key info */
    krb5_ticket_times times;		/* lifetime info */
    krb5_boolean is_skey;		/* true if ticket is encrypted in
					   another ticket's skey */
    krb5_flags ticket_flags;		/* flags in ticket */
    krb5_address **addresses;	/* addrs in ticket */
    krb5_data ticket;			/* ticket string itself */
    krb5_data second_ticket;		/* second ticket, if related to
					   ticket (via DUPLICATE-SKEY or
					   ENC-TKT-IN-SKEY) */
    krb5_authdata **authdata;	/* authorization data */
} krb5_creds;

/* Last request fields */
typedef struct _krb5_last_req_entry {
    krb5_magic magic;
    krb5_int32 lr_type;
    krb5_timestamp value;
} krb5_last_req_entry;

/* pre-authentication data */
typedef struct _krb5_pa_data {
    krb5_magic magic;
    krb5_preauthtype  pa_type;
    unsigned int length;
    krb5_octet *contents;
} krb5_pa_data;

typedef struct _krb5_kdc_req {
    krb5_magic magic;
    krb5_msgtype msg_type;		/* AS_REQ or TGS_REQ? */
    krb5_pa_data **padata;	/* e.g. encoded AP_REQ */
    /* real body */
    krb5_flags kdc_options;		/* requested options */
    krb5_principal client;		/* includes realm; optional */
    krb5_principal server;		/* includes realm (only used if no
					   client) */
    krb5_timestamp from;		/* requested starttime */
    krb5_timestamp till;		/* requested endtime */
    krb5_timestamp rtime;		/* (optional) requested renew_till */
    krb5_int32 nonce;			/* nonce to