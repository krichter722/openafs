/* Generated from ./krb5.asn1 */
/* Do not edit */

#ifndef __krb5_asn1_h__
#define __krb5_asn1_h__

#include <stddef.h>
#include <time.h>

#ifndef __asn1_common_definitions__
#define __asn1_common_definitions__

typedef struct heim_integer {
  size_t length;
  void *data;
  int negative;
} heim_integer;

typedef struct heim_octet_string {
  size_t length;
  void *data;
} heim_octet_string;

typedef char *heim_general_string;

typedef char *heim_utf8_string;

typedef char *heim_printable_string;

typedef char *heim_ia5_string;

typedef struct heim_bmp_string {
  size_t length;
  afs_uint16 *data;
} heim_bmp_string;

typedef struct heim_universal_string {
  size_t length;
  afs_uint32 *data;
} heim_universal_string;

typedef char *heim_visible_string;

typedef struct heim_oid {
  size_t length;
  unsigned *components;
} heim_oid;

typedef struct heim_bit_string {
  size_t length;
  void *data;
} heim_bit_string;

typedef struct heim_octet_string heim_any;
typedef struct heim_octet_string heim_any_set;

#define ASN1_MALLOC_ENCODE(T, B, BL, S, L, R)                  \
  do {                                                         \
    (BL) = length_##T((S));                                    \
    (B) = malloc((BL));                                        \
    if((B) == NULL) {                                          \
      (R) = ENOMEM;                                            \
    } else {                                                   \
      (R) = encode_##T(((unsigned char*)(B)) + (BL) - 1, (BL), \
                       (S), (L));                              \
      if((R) != 0) {                                           \
        free((B));                                             \
        (B) = NULL;                                            \
      }                                                        \
    }                                                          \
  } while (0)


#endif

/*
NAME-TYPE ::= INTEGER {
  KRB5_NT_UNKNOWN(0),
  KRB5_NT_PRINCIPAL(1),
  KRB5_NT_SRV_INST(2),
  KRB5_NT_SRV_HST(3),
  KRB5_NT_SRV_XHST(4),
  KRB5_NT_UID(5),
  KRB5_NT_X500_PRINCIPAL(6),
  KRB5_NT_SMTP_NAME(7),
  KRB5_NT_ENTERPRISE_PRINCIPAL(10),
  KRB5_NT_WELLKNOWN(11),
  KRB5_NT_ENT_PRINCIPAL_AND_ID(-130),
  KRB5_NT_MS_PRINCIPAL(-128),
  KRB5_NT_MS_PRINCIPAL_AND_ID(-129),
  KRB5_NT_NTLM(-1200)
}
*/

typedef enum NAME_TYPE {
  KRB5_NT_UNKNOWN = 0,
  KRB5_NT_PRINCIPAL = 1,
  KRB5_NT_SRV_INST = 2,
  KRB5_NT_SRV_HST = 3,
  KRB5_NT_SRV_XHST = 4,
  KRB5_NT_UID = 5,
  KRB5_NT_X500_PRINCIPAL = 6,
  KRB5_NT_SMTP_NAME = 7,
  KRB5_NT_ENTERPRISE_PRINCIPAL = 10,
  KRB5_NT_WELLKNOWN = 11,
  KRB5_NT_ENT_PRINCIPAL_AND_ID = -130,
  KRB5_NT_MS_PRINCIPAL = -128,
  KRB5_NT_MS_PRINCIPAL_AND_ID = -129,
  KRB5_NT_NTLM = -1200
} NAME_TYPE;

int    decode_NAME_TYPE(const unsigned char *, size_t, NAME_TYPE *, size_t *);
int    encode_NAME_TYPE(unsigned char *, size_t, const NAME_TYPE *, size_t *);
size_t length_NAME_TYPE(const NAME_TYPE *);
int    copy_NAME_TYPE  (const NAME_TYPE *, NAME_TYPE *);
void   free_NAME_TYPE  (NAME_TYPE *);


/*
MESSAGE-TYPE ::= INTEGER {
  krb_as_req(10),
  krb_as_rep(11),
  krb_tgs_req(12),
  krb_tgs_rep(13),
  krb_ap_req(14),
  krb_ap_rep(15),
  krb_safe(20),
  krb_priv(21),
  krb_cred(22),
  krb_error(30)
}
*/

typedef enum MESSAGE_TYPE {
  krb_as_req = 10,
  krb_as_rep = 11,
  krb_tgs_req = 12,
  krb_tgs_rep = 13,
  krb_ap_req = 14,
  krb_ap_rep = 15,
  krb_safe = 20,
  krb_priv = 21,
  krb_cred = 22,
  krb_error = 30
} MESSAGE_TYPE;

int    decode_MESSAGE_TYPE(const unsigned char *, size_t, MESSAGE_TYPE *, size_t *);
int    encode_MESSAGE_TYPE(unsigned char *, size_t, const MESSAGE_TYPE *, size_t *);
size_t length_MESSAGE_TYPE(const MESSAGE_TYPE *);
int    copy_MESSAGE_TYPE  (const MESSAGE_TYPE *, MESSAGE_TYPE *);
void   free_MESSAGE_TYPE  (MESSAGE_TYPE *);


/*
PADATA-TYPE ::= INTEGER {
  KRB5_PADATA_NONE(0),
  KRB5_PADATA_TGS_REQ(1),
  KRB5_PADATA_AP_REQ(1),
  KRB5_PADATA_ENC_TIMESTAMP(2),
  KRB5_PADATA_PW_SALT(3),
  KRB5_PADATA_ENC_UNIX_TIME(5),
  KRB5_PADATA_SANDIA_SECUREID(6),
  KRB5_PADATA_SESAME(7),
  KRB5_PADATA_OSF_DCE(8),
  KRB5_PADATA_CYBERSAFE_SECUREID(9),
  KRB5_PADATA_AFS3_SALT(10),
  KRB5_PADATA_ETYPE_INFO(11),
  KRB5_PADATA_SAM_CHALLENGE(12),
  KRB5_PADATA_SAM_RESPONSE(13),
  KRB5_PADATA_PK_AS_REQ_19(14),
  KRB5_PADATA_PK_AS_REP_19(15),
  KRB5_PADATA_PK_AS_REQ_WIN(15),
  KRB5_PADATA_PK_AS_REQ(16),
  KRB5_PADATA_PK_AS_REP(17),
  KRB5_PADATA_PA_PK_OCSP_RESPONSE(18),
  KRB5_PADATA_ETYPE_INFO2(19),
  KRB5_PADATA_USE_SPECIFIED_KVNO(20),
  KRB5_PADATA_SVR_REFERRAL_INFO(20),
  KRB5_PADATA_SAM_REDIRECT(21),
  KRB5_PADATA_GET_FROM_TYPED_DATA(22),
  KRB5_PADATA_SAM_ETYPE_INFO(23),
  KRB5_PADATA_SERVER_REFERRAL(25),
  KRB5_PADATA_ALT_PRINC(24),
  KRB5_PADATA_SAM_CHALLENGE2(30),
  KRB5_PADATA_SAM_RESPONSE2(31),
  KRB5_PA_EXTRA_TGT(41),
  KRB5_PADATA_TD_KRB_PRINCIPAL(102),
  KRB5_PADATA_PK_TD_TRUSTED_CERTIFIERS(104),
  KRB5_PADATA_PK_TD_CERTIFICATE_INDEX(105),
  KRB5_PADATA_TD_APP_DEFINED_ERROR(106),
  KRB5_PADATA_TD_REQ_NONCE(107),
  KRB5_PADATA_TD_REQ_SEQ(108),
  KRB5_PADATA_PA_PAC_REQUEST(128),
  KRB5_PADATA_FOR_USER(129),
  KRB5_PADATA_FOR_X509_USER(130),
  KRB5_PADATA_FOR_CHECK_DUPS(131),
  KRB5_PADATA_AS_CHECKSUM(132),
  KRB5_PADATA_PK_AS_09_BINDING(132),
  KRB5_PADATA_CLIENT_CANONICALIZED(133),
  KRB5_PADATA_FX_COOKIE(133),
  KRB5_PADATA_AUTHENTICATION_SET(134),
  KRB5_PADATA_AUTH_SET_SELECTED(135),
  KRB5_PADATA_FX_FAST(136),
  KRB5_PADATA_FX_ERROR(137),
  KRB5_PADATA_ENCRYPTED_CHALLENGE(138),
  KRB5_PADATA_OTP_CHALLENGE(141),
  KRB5_PADATA_OTP_REQUEST(142),
  KBB5_PADATA_OTP_CONFIRM(143),
  KRB5_PADATA_OTP_PIN_CHANGE(144),
  KRB5_PADATA_EPAK_AS_REQ(145),
  KRB5_PADATA_EPAK_AS_REP(146),
  KRB5_PADATA_PKINIT_KX(147),
  KRB5_PADATA_PKU2U_NAME(148),
  KRB5_PADATA_SUPPORTED_ETYPES(165)
}
*/

typedef enum PADATA_TYPE {
  KRB5_PADATA_NONE = 0,
  KRB5_PADATA_TGS_REQ = 1,
  KRB5_PADATA_AP_REQ = 1,
  KRB5_PADATA_ENC_TIMESTAMP = 2,
  KRB5_PADATA_PW_SALT = 3,
  KRB5_PADATA_ENC_UNIX_TIME = 5,
  KRB5_PADATA_SANDIA_SECUREID = 6,
  KRB5_PADATA_SESAME = 7,
  KRB5_PADATA_OSF_DCE = 8,
  KRB5_PADATA_CYBERSAFE_SECUREID = 9,
  KRB5_PADATA_AFS3_SALT = 10,
  KRB5_PADATA_ETYPE_INFO = 11,
  KRB5_PADATA_SAM_CHALLENGE = 12,
  KRB5_PADATA_SAM_RESPONSE = 13,
  KRB5_PADATA_PK_AS_REQ_19 = 14,
  KRB5_PADATA_PK_AS_REP_19 = 15,
  KRB5_PADATA_PK_AS_REQ_WIN = 15,
  KRB5_PADATA_PK_AS_REQ = 16,
  KRB5_PADATA_PK_AS_REP = 17,
  KRB5_PADATA_PA_PK_OCSP_RESPONSE = 18,
  KRB5_PADATA_ETYPE_INFO2 = 19,
  KRB5_PADATA_USE_SPECIFIED_KVNO = 20,
  KRB5_PADATA_SVR_REFERRAL_INFO = 20,
  KRB5_PADATA_SAM_REDIRECT = 21,
  KRB5_PADATA_GET_FROM_TYPED_DATA = 22,
  KRB5_PADATA_SAM_ETYPE_INFO = 23,
  KRB5_PADATA_SERVER_REFERRAL = 25,
  KRB5_PADATA_ALT_PRINC = 24,
  KRB5_PADATA_SAM_CHALLENGE2 = 30,
  KRB5_PADATA_SAM_RESPONSE2 = 31,
  KRB5_PA_EXTRA_TGT = 41,
  KRB5_PADATA_TD_KRB_PRINCIPAL = 102,
  KRB5_PADATA_PK_TD_TRUSTED_CERTIFIERS = 104,
  KRB5_PADATA_PK_TD_CERTIFICATE_INDEX = 105,
  KRB5_PADATA_TD_APP_DEFINED_ERROR = 106,
  KRB5_PADATA_TD_REQ_NONCE = 107,
  KRB5_PADATA_TD_REQ_SEQ = 108,
  KRB5_PADATA_PA_PAC_REQUEST = 128,
  KRB5_PADATA_FOR_USER = 129,
  KRB5_PADATA_FOR_X509_USER = 130,
  KRB5_PADATA_FOR_CHECK_DUPS = 131,
  KRB5_PADATA_AS_CHECKSUM = 132,
  KRB5_PADATA_PK_AS_09_BINDING = 132,
  KRB5_PADATA_CLIENT_CANONICALIZED = 133,
  KRB5_PADATA_FX_COOKIE = 133,
  KRB5_PADATA_AUTHENTICATION_SET = 134,
  KRB5_PADATA_AUTH_SET_SELECTED = 135,
  KRB5_PADATA_FX_FAST = 136,
  KRB5_PADATA_FX_ERROR = 137,
  KRB5_PADATA_ENCRYPTED_CHALLENGE = 138,
  KRB5_PADATA_OTP_CHALLENGE = 141,
  KRB5_PADATA_OTP_REQUEST = 142,
  KBB5_PADATA_OTP_CONFIRM = 143,
  KRB5_PADATA_OTP_PIN_CHANGE = 144,
  KRB5_PADATA_EPAK_AS_REQ = 145,
  KRB5_PADATA_EPAK_AS_REP = 146,
  KRB5_PADATA_PKINIT_KX = 147,
  KRB5_PADATA_PKU2U_NAME = 148,
  KRB5_PADATA_SUPPORTED_ETYPES = 165
} PADATA_TYPE;

int    decode_PADATA_TYPE(const unsigned char *, size_t, PADATA_TYPE *, size_t *);
int    encode_PADATA_TYPE(unsigned char *, size_t, const PADATA_TYPE *, size_t *);
size_t length_PADATA_TYPE(const PADATA_TYPE *);
int    copy_PADATA_TYPE  (const PADATA_TYPE *, PADATA_TYPE *);
void   free_PADATA_TYPE  (PADATA_TYPE *);


/*
AUTHDATA-TYPE ::= INTEGER {
  KRB5_AUTHDATA_IF_RELEVANT(1),
  KRB5_AUTHDATA_INTENDED_FOR_SERVER(2),
  KRB5_AUTHDATA_INTENDED_FOR_APPLICATION_CLASS(3),
  KRB5_AUTHDATA_KDC_ISSUED(4),
  KRB5_AUTHDATA_AND_OR(5),
  KRB5_AUTHDATA_MANDATORY_TICKET_EXTENSIONS(6),
  KRB5_AUTHDATA_IN_TICKET_EXTENSIONS(7),
  KRB5_AUTHDATA_MANDATORY_FOR_KDC(8),
  KRB5_AUTHDATA_INITIAL_VERIFIED_CAS(9),
  KRB5_AUTHDATA_OSF_DCE(64),
  KRB5_AUTHDATA_SESAME(65),
  KRB5_AUTHDATA_OSF_DCE_PKI_CERTID(66),
  KRB5_AUTHDATA_WIN2K_PAC(128),
  KRB5_AUTHDATA_GSS_API_ETYPE_NEGOTIATION(129),
  KRB5_AUTHDATA_SIGNTICKET_OLDER(-17),
  KRB5_AUTHDATA_SIGNTICKET_OLD(142),
  KRB5_AUTHDATA_SIGNTICKET(512)
}
*/

typedef enum AUTHDATA_TYPE {
  KRB5_AUTHDATA_IF_RELEVANT = 1,
  KRB5_AUTHDATA_INTENDED_FOR_SERVER = 2,
  KRB5_AUTHDATA_INTENDED_FOR_APPLICATION_CLASS = 3,
  KRB5_AUTHDATA_KDC_ISSUED = 4,
  KRB5_AUTHDATA_AND_OR = 5,
  KRB5_AUTHDATA_MANDATORY_TICKET_EXTENSIONS = 6,
  KRB5_AUTHDATA_IN_TICKET_EXTENSIONS = 7,
  KRB5_AUTHDATA_MANDATORY_FOR_KDC = 8,
  KRB5_AUTHDATA_INITIAL_VERIFIED_CAS = 9,
  KRB5_AUTHDATA_OSF_DCE = 64,
  KRB5_AUTHDATA_SESAME = 65,
  KRB5_AUTHDATA_OSF_DCE_PKI_CERTID = 66,
  KRB5_AUTHDATA_WIN2K_PAC = 128,
  KRB5_AUTHDATA_GSS_API_ETYPE_NEGOTIATION = 129,
  KRB5_AUTHDATA_SIGNTICKET_OLDER = -17,
  KRB5_AUTHDATA_SIGNTICKET_OLD = 142,
  KRB5_AUTHDATA_SIGNTICKET = 512
} AUTHDATA_TYPE;

int    decode_AUTHDATA_TYPE(const unsigned char *, size_t, AUTHDATA_TYPE *, size_t *);
int    encode_AUTHDATA_TYPE(unsigned char *, size_t, const AUTHDATA_TYPE *, size_t *);
size_t length_AUTHDATA_TYPE(const AUTHDATA_TYPE *);
int    copy_AUTHDATA_TYPE  (const AUTHDATA_TYPE *, AUTHDATA_TYPE *);
void   free_AUTHDATA_TYPE  (AUTHDATA_TYPE *);


/*
CKSUMTYPE ::= INTEGER {
  CKSUMTYPE_NONE(0),
  CKSUMTYPE_CRC32(1),
  CKSUMTYPE_RSA_MD4(2),
  CKSUMTYPE_RSA_MD4_DES(3),
  CKSUMTYPE_DES_MAC(4),
  CKSUMTYPE_DES_MAC_K(5),
  CKSUMTYPE_RSA_MD4_DES_K(6),
  CKSUMTYPE_RSA_MD5(7),
  CKSUMTYPE_RSA_MD5_DES(8),
  CKSUMTYPE_RSA_MD5_DES3(9),
  CKSUMTYPE_SHA1_OTHER(10),
  CKSUMTYPE_HMAC_SHA1_DES3(12),
  CKSUMTYPE_SHA1(14),
  CKSUMTYPE_HMAC_SHA1_96_AES_128(15),
  CKSUMTYPE_HMAC_SHA1_96_AES_256(16),
  CKSUMTYPE_GSSAPI(32771),
  CKSUMTYPE_HMAC_MD5(-138),
  CKSUMTYPE_HMAC_MD5_ENC(-1138)
}
*/

typedef enum CKSUMTYPE {
  CKSUMTYPE_NONE = 0,
  CKSUMTYPE_CRC32 = 1,
  CKSUMTYPE_RSA_MD4 = 2,
  CKSUMTYPE_RSA_MD4_DES = 3,
  CKSUMTYPE_DES_MAC = 4,
  CKSUMTYPE_DES_MAC_K = 5,
  CKSUMTYPE_RSA_MD4_DES_K = 6,
  CKSUMTYPE_RSA_MD5 = 7,
  CKSUMTYPE_RSA_MD5_DES = 8,
  CKSUMTYPE_RSA_MD5_DES3 = 9,
  CKSUMTYPE_SHA1_OTHER = 10,
  CKSUMTYPE_HMAC_SHA1_DES3 = 12,
  CKSUMTYPE_SHA1 = 14,
  CKSUMTYPE_HMAC_SHA1_96_AES_128 = 15,
  CKSUMTYPE_HMAC_SHA1_96_AES_256 = 16,
  CKSUMTYPE_GSSAPI = 32771,
  CKSUMTYPE_HMAC_MD5 = -138,
  CKSUMTYPE_HMAC_MD5_ENC = -1138
} CKSUMTYPE;

int    decode_CKSUMTYPE(const unsigned char *, size_t, CKSUMTYPE *, size_t *);
int    encode_CKSUMTYPE(unsigned char *, size_t, const CKSUMTYPE *, size_t *);
size_t length_CKSUMTYPE(const CKSUMTYPE *);
int    copy_CKSUMTYPE  (const CKSUMTYPE *, CKSUMTYPE *);
void   free_CKSUMTYPE  (CKSUMTYPE *);


/*
ENCTYPE ::= INTEGER {
  ETYPE_NULL(0),
  ETYPE_DES_CBC_CRC(1),
  ETYPE_DES_CBC_MD4(2),
  ETYPE_DES_CBC_MD5(3),
  ETYPE_DES3_CBC_MD5(5),
  ETYPE_OLD_DES3_CBC_SHA1(7),
  ETYPE_SIGN_DSA_GENERATE(8),
  ETYPE_ENCRYPT_RSA_PRIV(9),
  ETYPE_ENCRYPT_RSA_PUB(10),
  ETYPE_DES3_CBC_SHA1(16),
  ETYPE_AES128_CTS_HMAC_SHA1_96(17),
  ETYPE_AES256_CTS_HMAC_SHA1_96(18),
  ETYPE_ARCFOUR_HMAC_MD5(23),
  ETYPE_ARCFOUR_HMAC_MD5_56(24),
  ETYPE_ENCTYPE_PK_CROSS(48),
  ETYPE_ARCFOUR_MD4(-128),
  ETYPE_ARCFOUR_HMAC_OLD(-133),
  ETYPE_ARCFOUR_HMAC_OLD_EXP(-135),
  ETYPE_DES_CBC_NONE(-4096),
  ETYPE_DES3_CBC_NONE(-4097),
  ETYPE_DES_CFB64_NONE(-4098),
  ETYPE_DES_PCBC_NONE(-4099),
  ETYPE_DIGEST_MD5_NONE(-4100),
  ETYPE_CRAM_MD5_NONE(-4101)
}
*/

typedef enum ENCTYPE {
  ETYPE_NULL = 0,
  ETYPE_DES_CBC_CRC = 1,
  ETYPE_DES_CBC_MD4 = 2,
  ETYPE_DES_CBC_MD5 = 3,
  ETYPE_DES3_CBC_MD5 = 5,
  ETYPE_OLD_DES3_CBC_SHA1 = 7,
  ETYPE_SIGN_DSA_GENERATE = 8,
  ETYPE_ENCRYPT_RSA_PRIV = 9,
  ETYPE_ENCRYPT_RSA_PUB = 10,
  ETYPE_DES3_CBC_SHA1 = 16,
  ETYPE_AES128_CTS_HMAC_SHA1_96 = 17,
  ETYPE_AES256_CTS_HMAC_SHA1_96 = 18,
  ETYPE_ARCFOUR_HMAC_MD5 = 23,
  ETYPE_ARCFOUR_HMAC_MD5_56 = 24,
  ETYPE_ENCTYPE_PK_CROSS = 48,
  ETYPE_ARCFOUR_MD4 = -128,
  ETYPE_ARCFOUR_HMAC_OLD = -133,
  ETYPE_ARCFOUR_HMAC_OLD_EXP = -135,
  ETYPE_DES_CBC_NONE = -4096,
  ETYPE_DES3_CBC_NONE = -4097,
  ETYPE_DES_CFB64_NONE = -4098,
  ETYPE_DES_PCBC_NONE = -4099,
  ETYPE_DIGEST_MD5_NONE = -4100,
  ETYPE_CRAM_MD5_NONE = -4101
} ENCTYPE;

int    decode_ENCTYPE(const unsigned char *, size_t, ENCTYPE *, size_t *);
int    encode_ENCTYPE(unsigned char *, size_t, const ENCTYPE *, size_t *);
size_t length_ENCTYPE(const ENCTYPE *);
int    copy_ENCTYPE  (const ENCTYPE *, ENCTYPE *);
void   free_ENCTYPE  (ENCTYPE *);


/*
krb5uint32 ::= INTEGER (0..-1)
*/

typedef unsigned int krb5uint32;

int    decode_krb5uint32(const unsigned char *, size_t, krb5uint32 *, size_t *);
int    encode_krb5uint32(unsigned char *, size_t, const krb5uint32 *, size_t *);
size_t length_krb5uint32(const krb5uint32 *);
int    copy_krb5uint32  (const krb5uint32 *, krb5uint32 *);
void   free_krb5uint32  (krb5uint32 *);


/*
krb5int32 ::= INTEGER (-2147483648..2147483647)
*/

typedef int krb5int32;

int    decode_krb5int32(const unsigned char *, size_t, krb5int32 *, size_t *);
int    encode_krb5int32(unsigned char *, size_t, const krb5int32 *, size_t *);
size_t length_krb5int32(const krb5int32 *);
int    copy_krb5int32  (const krb5int32 *, krb5int32 *);
void   free_krb5int32  (krb5int32 *);


/*
KerberosString ::= GeneralString
*/

typedef heim_general_string KerberosString;

int    decode_KerberosString(const unsigned char *, size_t, KerberosString *, size_t *);
int    encode_KerberosString(unsigned char *, size_t, const KerberosString *, size_t *);
size_t length_KerberosString(const KerberosString *);
int    copy_KerberosString  (const KerberosString *, KerberosString *);
void   free_KerberosString  (KerberosString *);


/*
Realm ::= GeneralString
*/

typedef heim_general_string Realm;

int    decode_Realm(const unsigned char *, size_t, Realm *, size_t *);
int    encode_Realm(unsigned char *, size_t, const Realm *, size_t *);
size_t length_Realm(const Realm *);
int    copy_Realm  (const Realm *, Realm *);
void   free_Realm  (Realm *);


/*
PrincipalName ::= SEQUENCE {
  name-type       [0] NAME-TYPE,
  name-string     [1] SEQUENCE OF GeneralString,
}
*/

typedef struct PrincipalName {
  NAME_TYPE name_type;
  struct PrincipalName_name_string {
    unsigned int len;
    heim_general_string *val;
  } name_string;
} PrincipalName;

int    decode_PrincipalName(const unsigned char *, size_t, PrincipalName *, size_t *);
int    encode_PrincipalName(unsigned char *, size_t, const PrincipalName *, size_t *);
size_t length_PrincipalName(const PrincipalName *);
int    copy_PrincipalName  (const PrincipalName *, PrincipalName *);
void   free_PrincipalName  (PrincipalName *);


/*
Principal ::= SEQUENCE {
  name            [0] PrincipalName,
  realm           [1] Realm,
}
*/

typedef struct Principal {
  PrincipalName name;
  Realm realm;
} Principal;

int    decode_Principal(const unsigned char *, size_t, Principal *, size_t *);
int    encode_Principal(unsigned char *, size_t, const Principal *, size_t *);
size_t length_Principal(const Principal *);
int    copy_Principal  (const Principal *, Principal *);
void   free_Principal  (Principal *);


/*
Principals ::= SEQUENCE OF Principal
*/

typedef struct Principals {
  unsigned int len;
  Principal *val;
} Principals;

int   add_Principals  (Principals *, const Principal *);
int   remove_Principals  (Principals *, unsigned int);
int    decode_Principals(const unsigned char *, size_t, Principals *, size_t *);
int    encode_Principals(unsigned char *, size_t, const Principals *, size_t *);
size_t length_Principals(const Principals *);
int    copy_Principals  (const Principals *, Principals *);
void   free_Principals  (Principals *);


/*
HostAddress ::= SEQUENCE {
  addr-type       [0] krb5int32,
  address         [1] OCTET STRING,
}
*/

typedef struct HostAddress {
  krb5int32 addr_type;
  heim_octet_string address;
} HostAddress;

int    decode_HostAddress(const unsigned char *, size_t, HostAddress *, size_t *);
int    encode_HostAddress(unsigned char *, size_t, const HostAddress *, size_t *);
size_t length_HostAddress(const HostAddress *);
int    copy_HostAddress  (const HostAddress *, HostAddress *);
void   free_HostAddress  (HostAddress *);


/*
HostAddresses ::= SEQUENCE OF HostAddress
*/

typedef struct HostAddresses {
  unsigned int len;
  HostAddress *val;
} HostAddresses;

int    decode_HostAddresses(const unsigned char *, size_t, HostAddresses *, size_t *);
int    encode_HostAddresses(unsigned char *, size_t, const HostAddresses *, size_t *);
size_t length_HostAddresses(const HostAddresses *);
int    copy_HostAddresses  (const HostAddresses *, HostAddresses *);
void   free_HostAddresses  (HostAddresses *);


/*
KerberosTime ::= GeneralizedTime
*/

typedef time_t KerberosTime;

int    decode_KerberosTime(const unsigned char *, size_t, KerberosTime *, size_t *);
int    encode_KerberosTime(unsigned char *, size_t, const KerberosTime *, size_t *);
size_t length_KerberosTime(const KerberosTime *);
int    copy_KerberosTime  (const KerberosTime *, KerberosTime *);
void   free_KerberosTime  (KerberosTime *);


/*
AuthorizationDataElement ::= SEQUENCE {
  ad-type         [0] krb5int32,
  ad-data         [1] OCTET STRING,
}
*/

typedef struct AuthorizationDataElement {
  krb5int32 ad_type;
  heim_octet_string ad_data;
} AuthorizationDataElement;

int    decode_AuthorizationDataElement(const unsigned char *, size_t, AuthorizationDataElement *, size_t *);
int    encode_AuthorizationDataElement(unsigned char *, size_t, const AuthorizationDataElement *, size_t *);
size_t length_AuthorizationDataElement(const AuthorizationDataElement *);
int    copy_AuthorizationDataElement  (const AuthorizationDataElement *, AuthorizationDataElement *);
void   free_AuthorizationDataElement  (AuthorizationDataElement *);


/*
AuthorizationData ::= SEQUENCE OF AuthorizationDataElement
*/

typedef struct AuthorizationData {
  unsigned int len;
  AuthorizationDataElement *val;
} AuthorizationData;

int   add_AuthorizationData  (AuthorizationData *, const AuthorizationDataElement *);
int   remove_AuthorizationData  (AuthorizationData *, unsigned int);
int    decode_AuthorizationData(const unsigned char *, size_t, AuthorizationData *, size_t *);
int    encode_AuthorizationData(unsigned char *, size_t, const AuthorizationData *, size_t *);
size_t length_AuthorizationData(const AuthorizationData *);
int    copy_AuthorizationData  (const AuthorizationData *, AuthorizationData *);
void   free_AuthorizationData  (AuthorizationData *);


/*
APOptions ::= BIT STRING {
  reserved(0),
  use-session-key(1),
  mutual-required(2)
}
*/

typedef struct APOptions {
  unsigned int reserved:1;
  unsigned int use_session_key:1;
  unsigned int mutual_required:1;
  unsigned int _unused3:1;
  unsigned int _unused4:1;
  unsigned int _unused5:1;
  unsigned int _unused6:1;
  unsigned int _unused7:1;
  unsigned int _unused8:1;
  unsigned int _unused9:1;
  unsigned int _unused10:1;
  unsigned int _unused11:1;
  unsigned int _unused12:1;
  unsigned int _unused13:1;
  unsigned int _unused14:1;
  unsigned int _unused15:1;
  unsigned int _unused16:1;
  unsigned int _unused17:1;
  unsigned int _unused18:1;
  unsigned int _unused19:1;
  unsigned int _unused20:1;
  unsigned int _unused21:1;
  unsigned int _unused22:1;
  unsigned int _unused23:1;
  unsigned int _unused24:1;
  unsigned int _unused25:1;
  unsigned int _unused26:1;
  unsigned int _unused27:1;
  unsigned int _unused28:1;
  unsigned int _unused29:1;
  unsigned int _unused30:1;
  unsigned int _unused31:1;
} APOptions;


unsigned APOptions2int(APOptions);
APOptions int2APOptions(unsigned);
int    decode_APOptions(const unsigned char *, size_t, APOptions *, size_t *);
int    encode_APOptions(unsigned char *, size_t, const APOptions *, size_t *);
size_t length_APOptions(const APOptions *);
int    copy_APOptions  (const APOptions *, APOptions *);
void   free_APOptions  (APOptions *);


/*
TicketFlags ::= BIT STRING {
  reserved(0),
  forwardable(1),
  forwarded(2),
  proxiable(3),
  proxy(4),
  may-postdate(5),
  postdated(6),
  invalid(7),
  renewable(8),
  initial(9),
  pre-authent(10),
  hw-authent(11),
  transited-policy-checked(12),
  ok-as-delegate(13),
  anonymous(14)
}
*/

typedef struct TicketFlags {
  unsigned int reserved:1;
  unsigned int forwardable:1;
  unsigned int forwarded:1;
  unsigned int proxiable:1;
  unsigned int proxy:1;
  unsigned int may_postdate:1;
  unsigned int postdated:1;
  unsigned int invalid:1;
  unsigned int renewable:1;
  unsigned int initial:1;
  unsigned int pre_authent:1;
  unsigned int hw_authent:1;
  unsigned int transited_policy_checked:1;
  unsigned int ok_as_delegate:1;
  unsigned int anonymous:1;
  unsigned int _unused15:1;
  unsigned int _unused16:1;
  unsigned int _unused17:1;
  unsigned int _unused18:1;
  unsigned int _unused19:1;
  unsigned int _unused20:1;
  unsigned int _unused21:1;
  unsigned int _unused22:1;
  unsigned int _unused23:1;
  unsigned int _unused24:1;
  unsigned int _unused25:1;
  unsigned int _unused26:1;
  unsigned int _unused27:1;
  unsigned int _unused28:1;
  unsigned int _unused29:1;
  unsigned int _unused30:1;
  unsigned int _unused31:1;
} TicketFlags;


unsigned TicketFlags2int(TicketFlags);
TicketFlags int2TicketFlags(unsigned);
int    decode_TicketFlags(const unsigned char *, size_t, TicketFlags *, size_t *);
int    encode_TicketFlags(unsigned char *, size_t, const TicketFlags *, size_t *);
size_t length_TicketFlags(const TicketFlags *);
int    copy_TicketFlags  (const TicketFlags *, TicketFlags *);
void   free_TicketFlags  (TicketFlags *);


/*
KDCOptions ::= BIT STRING {
  reserved(0),
  forwardable(1),
  forwarded(2),
  proxiable(3),
  proxy(4),
  allow-postdate(5),
  postdated(6),
  renewable(8),
  request-anonymous(14),
  canonicalize(15),
  constrained-delegation(16),
  disable-transited-check(26),
  renewable-ok(27),
  enc-tkt-in-skey(28),
  renew(30),
  validate(31)
}
*/

typedef struct KDCOptions {
  unsigned int reserved:1;
  unsigned int forwardable:1;
  unsigned int forwarded:1;
  unsigned int proxiable:1;
  unsigned int proxy:1;
  unsigned int allow_postdate:1;
  unsigned int postdated:1;
  unsigned int _unused7:1;
  unsigned int renewable:1;
  unsigned int _unused9:1;
  unsigned int _unused10:1;
  unsigned int _unused11:1;
  unsigned int _unused12:1;
  unsigned int _unused13:1;
  unsigned int request_anonymous:1;
  unsigned int canonicalize:1;
  unsigned int constrained_delegation:1;
  unsigned int _unused17:1;
  unsigned int _unused18:1;
  unsigned int _unused19:1;
  unsigned int _unused20:1;
  unsigned int _unused21:1;
  unsigned int _unused22:1;
  unsigned int _unused23:1;
  unsigned int _unused24:1;
  unsigned int _unused25:1;
  unsigned int disable_transited_check:1;
  unsigned int renewable_ok:1;
  unsigned int enc_tkt_in_skey:1;
  unsigned int _unused29:1;
  unsigned int renew:1;
  unsigned int validate:1;
} KDCOptions;


unsigned KDCOptions2int(KDCOptions);
KDCOptions int2KDCOptions(unsigned);
int    decode_KDCOptions(const unsigned char *, size_t, KDCOptions *, size_t *);
int    encode_KDCOptions(unsigned char *, size_t, const KDCOptions *, size_t *);
size_t length_KDCOptions(const KDCOptions *);
int    copy_KDCOptions  (const KDCOptions *, KDCOptions *);
void   free_KDCOptions  (KDCOptions *);


/*
LR-TYPE ::= INTEGER {
  LR_NONE(0),
  LR_INITIAL_TGT(1),
  LR_INITIAL(2),
  LR_ISSUE_USE_TGT(3),
  LR_RENEWAL(4),
  LR_REQUEST(5),
  LR_PW_EXPTIME(6),
  LR_ACCT_EXPTIME(7)
}
*/

typedef enum LR_TYPE {
  LR_NONE = 0,
  LR_INITIAL_TGT = 1,
  LR_INITIAL = 2,
  LR_ISSUE_USE_TGT = 3,
  LR_RENEWAL = 4,
  LR_REQUEST = 5,
  LR_PW_EXPTIME = 6,
  LR_ACCT_EXPTIME = 7
} LR_TYPE;

int    decode_LR_TYPE(const unsigned char *, size_t, LR_TYPE *, size_t *);
int    encode_LR_TYPE(unsigned char *, size_t, const LR_TYPE *, size_t *);
size_t length_LR_TYPE(const LR_TYPE *);
int    copy_LR_TYPE  (const LR_TYPE *, LR_TYPE *);
void   free_LR_TYPE  (LR_TYPE *);


/*
LastReq ::= SEQUENCE OF SEQUENCE {
  lr-type         [0] LR-TYPE,
  lr-value        [1] KerberosTime,
}
*/

typedef struct LastReq {
  unsigned int len;
  struct LastReq_val {
    LR_TYPE lr_type;
    KerberosTime lr_value;
  } *val;
} LastReq;

int    decode_LastReq(const unsigned char *, size_t, LastReq *, size_t *);
int    encode_LastReq(unsigned char *, size_t, const LastReq *, size_t *);
size_t length_LastReq(const LastReq *);
int    copy_LastReq  (const LastReq *, LastReq *);
void   free_LastReq  (LastReq *);


/*
EncryptedData ::= SEQUENCE {
  etype           [0] ENCTYPE,
  kvno            [1] krb5int32 OPTIONAL,
  cipher          [2] OCTET STRING,
}
*/

typedef struct EncryptedData {
  ENCTYPE etype;
  krb5int32 *kvno;
  heim_octet_string cipher;
} EncryptedData;

int    decode_EncryptedData(const unsigned char *, size_t, EncryptedData *, size_t *);
int    encode_EncryptedData(unsigned char *, size_t, const EncryptedData *, size_t *);
size_t length_EncryptedData(const EncryptedData *);
int    copy_EncryptedData  (const EncryptedData *, EncryptedData *);
void   free_EncryptedData  (EncryptedData *);


/*
EncryptionKey ::= SEQUENCE {
  keytype         [0] krb5int32,
  keyvalue        [1] OCTET STRING,
}
*/

typedef struct EncryptionKey {
  krb5int32 keytype;
  heim_octet_string keyvalue;
} EncryptionKey;

int    decode_EncryptionKey(const unsigned char *, size_t, EncryptionKey *, size_t *);
int    encode_EncryptionKey(unsigned char *, size_t, const EncryptionKey *, size_t *);
size_t length_EncryptionKey(const EncryptionKey *);
int    copy_EncryptionKey  (const EncryptionKey *, EncryptionKey *);
void   free_EncryptionKey  (EncryptionKey *);


/*
TransitedEncoding ::= SEQUENCE {
  tr-type         [0] krb5int32,
  contents        [1] OCTET STRING,
}
*/

typedef struct TransitedEncoding {
  krb5int32 tr_type;
  heim_octet_string contents;
} TransitedEncoding;

int    decode_TransitedEncoding(const unsigned char *, size_t, TransitedEncoding *, size_t *);
int    encode_TransitedEncoding(unsigned char *, size_t, const TransitedEncoding *, size_t *);
size_t length_TransitedEncoding(const TransitedEncoding *);
int    copy_TransitedEncoding  (const TransitedEncoding *, TransitedEncoding *);
void   free_TransitedEncoding  (TransitedEncoding *);


/*
Ticket ::= [APPLICATION 1] SEQUENCE {
  tkt-vno         [0] krb5int32,
  realm           [1] Realm,
  sname           [2] PrincipalName,
  enc-part        [3] EncryptedData,
}
*/

typedef struct Ticket {
  krb5int32 tkt_vno;
  Realm realm;
  PrincipalName sname;
  EncryptedData enc_part;
} Ticket;

int    decode_Ticket(const unsigned char *, size_t, Ticket *, size_t *);
int    encode_Ticket(unsigned char *, size_t, const Ticket *, size_t *);
size_t length_Ticket(const Ticket *);
int    copy_Ticket  (const Ticket *, Ticket *);
void   free_Ticket  (Ticket *);


/*
EncTicketPart ::= [APPLICATION 3] SEQUENCE {
  flags                [0] TicketFlags,
  key                  [1] EncryptionKey,
  crealm               [2] Realm,
  cname                [3] PrincipalName,
  transited            [4] TransitedEncoding,
  authtime             [5] KerberosTime,
  starttime            [6] KerberosTime OPTIONAL,
  endtime              [7] KerberosTime,
  renew-till           [8] KerberosTime OPTIONAL,
  caddr                [9] HostAddresses OPTIONAL,
  authorization-data   [10] AuthorizationData OPTIONAL,
}
*/

typedef struct EncTicketPart {
  TicketFlags flags;
  EncryptionKey key;
  Realm crealm;
  PrincipalName cname;
  TransitedEncoding transited;
  KerberosTime authtime;
  KerberosTime *starttime;
  KerberosTime endtime;
  KerberosTime *renew_till;
  HostAddresses *caddr;
  AuthorizationData *authorization_data;
} EncTicketPart;

int    decode_EncTicketPart(const unsigned char *, size_t, EncTicketPart *, size_t *);
int    encode_EncTicketPart(unsigned char *, size_t, const EncTicketPart *, size_t *);
size_t length_EncTicketPart(const EncTicketPart *);
int    copy_EncTicketPart  (const EncTicketPart *, EncTicketPart *);
void   free_EncTicketPart  (EncTicketPart *);


/*
Checksum ::= SEQUENCE {
  cksumtype       [0] CKSUMTYPE,
  checksum        [1] OCTET STRING,
}
*/

typedef struct Checksum {
  CKSUMTYPE cksumtype;
  heim_octet_string checksum;
} Checksum;

int    decode_Checksum(const unsigned char *, size_t, Checksum *, size_t *);
int    encode_Checksum(unsigned char *, size_t, const Checksum *, size_t *);
size_t length_Checksum(const Checksum *);
int    copy_Checksum  (const Checksum *, Checksum *);
void   free_Checksum  (Checksum *);


/*
Authenticator ::= [APPLICATION 2] SEQUENCE {
  authenticator-vno    [0] krb5int32,
  crealm               [1] Realm,
  cname                [2] PrincipalName,
  cksum                [3] Checksum OPTIONAL,
  cusec                [4] krb5int32,
  ctime                [5] KerberosTime,
  subkey               [6] EncryptionKey OPTIONAL,
  seq-number           [7] krb5uint32 OPTIONAL,
  authorization-data   [8] AuthorizationData OPTIONAL,
}
*/

typedef struct Authenticator {
  krb5int32 authenticator_vno;
  Realm crealm;
  PrincipalName cname;
  Checksum *cksum;
  krb5int32 cusec;
  KerberosTime ctime;
  EncryptionKey *subkey;
  krb5uint32 *seq_number;
  AuthorizationData *authorization_data;
} Authenticator;

int    decode_Authenticator(const unsigned char *, size_t, Authenticator *, size_t *);
int    encode_Authenticator(unsigned char *, size_t, const Authenticator *, size_t *);
size_t length_Authenticator(const Authenticator *);
int    copy_Authenticator  (const Authenticator *, Authenticator *);
void   free_Authenticator  (Authenticator *);


/*
PA-DATA ::= SEQUENCE {
  padata-type     [1] PADATA-TYPE,
  padata-value    [2] OCTET STRING,
}
*/

typedef struct PA_DATA {
  PADATA_TYPE padata_type;
  heim_octet_string padata_value;
} PA_DATA;

int    decode_PA_DATA(const unsigned char *, size_t, PA_DATA *, size_t *);
int    encode_PA_DATA(unsigned char *, size_t, const PA_DATA *, size_t *);
size_t length_PA_DATA(const PA_DATA *);
int    copy_PA_DATA  (const PA_DATA *, PA_DATA *);
void   free_PA_DATA  (PA_DATA *);


/*
ETYPE-INFO-ENTRY ::= SEQUENCE {
  etype           [0] ENCTYPE,
  salt            [1] OCTET STRING OPTIONAL,
  salttype        [2] krb5int32 OPTIONAL,
}
*/

typedef struct ETYPE_INFO_ENTRY {
  ENCTYPE etype;
  heim_octet_string *salt;
  krb5int32 *salttype;
} ETYPE_INFO_ENTRY;

int    decode_ETYPE_INFO_ENTRY(const unsigned char *, size_t, ETYPE_INFO_ENTRY *, size_t *);
int    encode_ETYPE_INFO_ENTRY(unsigned char *, size_t, const ETYPE_INFO_ENTRY *, size_t *);
size_t length_ETYPE_INFO_ENTRY(const ETYPE_INFO_ENTRY *);
int    copy_ETYPE_INFO_ENTRY  (const ETYPE_INFO_ENTRY *, ETYPE_INFO_ENTRY *);
void   free_ETYPE_INFO_ENTRY  (ETYPE_INFO_ENTRY *);


/*
ETYPE-INFO ::= SEQUENCE OF ETYPE-INFO-ENTRY
*/

typedef struct ETYPE_INFO {
  unsigned int len;
  ETYPE_INFO_ENTRY *val;
} ETYPE_INFO;

int   add_ETYPE_INFO  (ETYPE_INFO *, const ETYPE_INFO_ENTRY *);
int   remove_ETYPE_INFO  (ETYPE_INFO *, unsigned int);
int    decode_ETYPE_INFO(const unsigned char *, size_t, ETYPE_INFO *, size_t *);
int    encode_ETYPE_INFO(unsigned char *, size_t, const ETYPE_INFO *, size_t *);
size_t length_ETYPE_INFO(const ETYPE_INFO *);
int    copy_ETYPE_INFO  (const ETYPE_INFO *, ETYPE_INFO *);
void   free_ETYPE_INFO  (ETYPE_INFO *);


/*
ETYPE-INFO2-ENTRY ::= SEQUENCE {
  etype           [0] ENCTYPE,
  salt            [1] KerberosString OPTIONAL,
  s2kparams       [2] OCTET STRING OPTIONAL,
}
*/

typedef struct ETYPE_INFO2_ENTRY {
  ENCTYPE etype;
  KerberosString *salt;
  heim_octet_string *s2kparams;
} ETYPE_INFO2_ENTRY;

int    decode_ETYPE_INFO2_ENTRY(const unsigned char *, size_t, ETYPE_INFO2_ENTRY *, size_t *);
int    encode_ETYPE_INFO2_ENTRY(unsigned char *, size_t, const ETYPE_INFO2_ENTRY *, size_t *);
size_t length_ETYPE_INFO2_ENTRY(const ETYPE_INFO2_ENTRY *);
int    copy_ETYPE_INFO2_ENTRY  (const ETYPE_INFO2_ENTRY *, ETYPE_INFO2_ENTRY *);
void   free_ETYPE_INFO2_ENTRY  (ETYPE_INFO2_ENTRY *);


/*
ETYPE-INFO2 ::= SEQUENCE OF ETYPE-INFO2-ENTRY
*/

typedef struct ETYPE_INFO2 {
  unsigned int len;
  ETYPE_INFO2_ENTRY *val;
} ETYPE_INFO2;

int   add_ETYPE_INFO2  (ETYPE_INFO2 *, const ETYPE_INFO2_ENTRY *);
int   remove_ETYPE_INFO2  (ETYPE_INFO2 *, un