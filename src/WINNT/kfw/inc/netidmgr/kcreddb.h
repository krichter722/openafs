/*
 * Copyright (c) 2005 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* $Id$ */

#ifndef __KHIMAIRA_KCREDDB_H__
#define __KHIMAIRA_KCREDDB_H__

#include<khdefs.h>
#include<time.h>


/*! \defgroup kcdb NetIDMgr Credentials Database */
/*@{*/

/*! \brief Maximum length in characters of short description

    The length includes the terminating \a NULL character.
    */
#define KCDB_MAXCCH_SHORT_DESC  256

/*! \brief Maximum length in bytes of short description

    The length includes the terminating \a NULL character.
    */
#define KCDB_MAXCB_SHORT_DESC   (sizeof(wchar_t) * KCDB_MAXCCH_SHORT_DESC)

/*! \brief Maximum length in characters of long description

    The length includes the terminating \a NULL character.
    */
#define KCDB_MAXCCH_LONG_DESC   8192

/*! \brief Maximum length in characters of long description

    The length includes the terminating \a NULL character.
    */
#define KCDB_MAXCB_LONG_DESC    (sizeof(wchar_t) * KCDB_MAXCCH_LONG_DESC)

/*! \brief Maximum length in characters of name

    The length includes the terminating \a NULL character.
    */
#define KCDB_MAXCCH_NAME        256

/*! \brief Maximum length in bytes of short description

    The length includes the terminating \a NULL character.
    */
#define KCDB_MAXCB_NAME         (sizeof(wchar_t) * KCDB_MAXCCH_NAME)

/*! \brief Automatically determine the number of bytes required

    Can be used in most places where a count of bytes is required.
    For many objects, the number of bytes that are required can be
    determined through context and may be ommited.  In such cases you
    can use the \a KCDB_CBSIZE_AUTO value to specify that the function
    is to determine the size automatically.

    \note Not all functions that take a count of bytes support the \a
        KCDB_CBSIZE_AUTO value.
*/
#define KCDB_CBSIZE_AUTO (-1)

/*!
\defgroup kcdb_ident Identities

Functions, macros etc. for manipulating identities.
*/

/*@{*/

/*! \brief The maximum number of characters (including terminator) that can
           be specified as an identity name */
#define KCDB_IDENT_MAXCCH_NAME 256

/*! \brief The maximum number of bytes that can be specified as an identity
           name */
#define KCDB_IDENT_MAXCB_NAME (sizeof(wchar_t) * KCDB_IDENT_MAXCCH_NAME)

/*! \brief Valid characters in an identity name */
#define KCDB_IDENT_VALID_CHARS L"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._@-/"

/*!
\name Flags for identities */
/*@{*/

/*! \brief Create the identity if it doesn't already exist.
    \note  Only to be used with kcdb_identity_create() */
#define KCDB_IDENT_FLAG_CREATE      0x10000000L

/*! \brief Has configuration information

    Indicates that the identity has persistent configuration
    information associated with it.
 */
#define KCDB_IDENT_FLAG_CONFIG      0x00800000L

/*! \brief Marks the identity as active.

    An active identity is one that is in active use within NetIDMgr.

    \note This flag is readonly and cannot be specified when creating
        or modifying an identity. Once an identity is deleted, it will
        no longer have this flag. */
#define KCDB_IDENT_FLAG_ACTIVE      0x02000000L


/*! \brief The identity has custom attributes assigned
 */
#define KCDB_IDENT_FLAG_ATTRIBS     0x08000000L

/*! \brief This is the default identity.

    At most one identity will have this flag set at any given time.
    To set or reset the flag, use kcdb_identity_set_default() */
#define KCDB_IDENT_FLAG_DEFAULT     0x00000001L

/*! \brief This identity can be searched.

    The meaning of this flag is left to be interpreted by individual
    plugins. */
#define KCDB_IDENT_FLAG_SEARCHABLE  0x00000002L

/*! \brief Hidden identity.

    The identity will not show up in the identity list window.  Once
    the hidden is switched off, the identity (and all associated
    credentials) will re-appear in the window */
#define KCDB_IDENT_FLAG_HIDDEN      0x00000004L

/*! \brief Invalid identity

    For one reason or another, this identity is invalid.  This flag
    can be set by an identity provider to indicate that this identity
    does not correspond to an actual identity because an external
    entity (such as a KDC) has denied it's existence.

    The absence of this flag does not imply that the identity is
    valid.  The ::KCDB_IDENT_FLAG_VALID bit must be set for that to be
    the case.  If neither flag is set, then the status of the identity
    is not known.
*/
#define KCDB_IDENT_FLAG_INVALID     0x00000008L

/*! \brief Valid identity

    The identity has been validated through an external entity, or
    it's validity implied through the existence of credentials for the
    identity.

    The absence of this flag does not imply that the identity is
    invalid.  The ::KCDB_IDENT_FLAG_INVALID bit must be set for that
    to be the case.  If neither flag is set, then the status of the
    identity is not known.
 */
#define KCDB_IDENT_FLAG_VALID       0x00000010L

/*! \brief Expired identity

    This identity has expired and can not be actively used to obtain
    credentials.  This determination is made based on the input of
    some external entity.  This flag may only be set by an identity
    provider.
*/
#define KCDB_IDENT_FLAG_EXPIRED     0x00000020L

/*! \brief Empty identity

    The identity does not have actual credentials associated with it.
 */
#define KCDB_IDENT_FLAG_EMPTY       0x00000040L

/*! \brief Renewable identity

    The initial credentials associated with this identity are
    renewable.  Thus making the whole identity renewable.
 */
#define KCDB_IDENT_FLAG_RENEWABLE   0x00000080L

/*! \brief Required user interaction

    The identity is in a state which requires user interaction to
    activate.  Currently, the identity may not be in a state where it
    can be used to obtain credentials.

    A typical example of this is when the primary password for an
    identity has expired.
 */
#define KCDB_IDENT_FLAG_INTERACT    0x00000100L

/*! \brief Has expired credentials

    The identity has expired credentials associated with it.
 */
#define KCDB_IDENT_FLAG_CRED_EXP    0x00000200L

/*! \brief Has renewable credentials

    The identity has renewable credentials associated with it.  If the
    initial credentials of the identity are renewable, then identity
    is renewable.  Hence the ::KCDB_IDENT_FLAG_RENEWABLE should also
    be set.
 */
#define KCDB_IDENT_FLAG_CRED_RENEW  0x00000400L

/*! \brief Sticky identity

    Sticky identities are identities that are always visible in the
    credentials display even if no credentials are associated with it.
 */
#define KCDB_IDENT_FLAG_STICKY      0x00000800L

/*! \brief Read/write flags mask.

    A bitmask that correspond to all the read/write flags in the mask.
*/
#define KCDB_IDENT_FLAGMASK_RDWR   0x00000fffL

/*@}*/

/*! \name Identity Provider Data Structures
@{*/

/*! \brief Name transfer structure

    Used when the KCDB is communicating with the identity provider to
    exchange string names of identities.  See individual ::KMSG_IDENT
    message subtypes for the usage of this structure.
 */
typedef struct tag_kcdb_ident_name_xfer {
    const wchar_t * name_src;   /*!< An identity name.  Does not
                                     exceed KCDB_IDENT_MAXCCH_NAME
                                     characters including terminating
                                     NULL. */
    const wchar_t * name_alt;   /*!< An identity name.  Does not
                                     exceed KCDB_IDENT_MAXCCH_NAME
                                     characters including terminating
                                     NULL. */
    wchar_t *       name_dest;  /*!< Pointer to a buffer that is to
                                     receive a response string.  The
                                     size of the buffer in bytes is
                                     specified in \a cb_name_dest. */
    khm_size        cb_name_dest; /*!< Size of buffer pointed to by \a
                                     name_dest in bytes. */
    khm_int32       result;     /*!< Receives a result value, which is
                                     usually an error code defined in
                                     kherror.h, though it is not
                                     always. */
} kcdb_ident_name_xfer;

typedef struct tag_kcdb_ident_info {
    khm_handle      identity;
    khm_int32       fields;

    FILETIME        expiration;
} kcdb_ident_info;

/*@}*/

/*! \name Identity provider interface functions

    These functions encapsulate safe calls to the current identity
    provider.  While these functions are exported, applications should
    not call these functions directly.  They are provided for use by
    the NetIDMgr core application.
@{*/

/*! \brief Validate an identity name

    The name that is provided will be passed through sets of
    validations.  One set, which doesn't depend on the identity
    provider checks whether the length of the identity name and
    whether there are any invalid characters in the identity name.  If
    the name passes those tests, then the name is passed down to the
    identity provider's name validation handler.

    \retval KHM_ERROR_SUCCESS The name is valid
    \retval KHM_ERROR_TOO_LONG Too many characters in name
    \retval KHM_ERROR_INVALID_NAME There were invalid characters in the name.
    \retval KHM_ERROR_NO_PROVIDER There is no identity provider;
        however the name passed the length and character tests.
    \retval KHM_ERROR_NOT_IMPLEMENTED The identity provider doesn't
        implement a name validation handler; however the name passed
        the length and character tests.

    \see ::KMSG_IDENT_VALIDATE_NAME
 */
KHMEXP khm_int32 KHMAPI
kcdb_identpro_validate_name(const wchar_t * name);

/*! \brief Validate an identity

    The identity itself needs to be validated.  This may involve
    communicating with an external entity.

    \see ::KMSG_IDENT_VALIDATE_IDENTITY
 */
KHMEXP khm_int32 KHMAPI
kcdb_identpro_validate_identity(khm_handle identity);

/*! \brief Canonicalize the name


    \see ::KMSG_IDENT_CANON_NAME
*/
KHMEXP khm_int32 KHMAPI
kcdb_identpro_canon_name(const wchar_t * name_in,
                         wchar_t * name_out,
                         khm_size * cb_name_out);

/*! \brief Compare two identity names

    \see ::KMSG_IDENT_COMPARE_NAME
*/
KHMEXP khm_int32 KHMAPI
kcdb_identpro_compare_name(const wchar_t * name1,
                           const wchar_t * name2);

/*! \brief Set the specified identity as the default

    \see ::KMSG_IDENT_SET_DEFAULT
*/
KHMEXP khm_int32 KHMAPI
kcdb_identpro_set_default(khm_handle identity);

/*! \brief Set the specified identity as searchable

    \see ::KMSG_IDENT_SET_SEARCHABLE
*/
KHMEXP khm_int32 KHMAPI
kcdb_identpro_set_searchable(khm_handle identity,
                             khm_boolean searchable);

/*! \brief Update the specified identity

    \see ::KMSG_IDENT_UPDATE
*/
KHMEXP khm_int32 KHMAPI
kcdb_identpro_update(khm_handle identity);

/*! \brief Obtain the UI callback

    \a rock is actually a pointer to a ::khui_ident_new_creds_cb which
    is to receive the callback.

    \see ::KMSG_IDENT_GET_UI_CALLBACK
 */
KHMEXP khm_int32 KHMAPI
kcdb_identpro_get_ui_cb(void * rock);

/*! \brief Notify an identity provider of the creation of a new identity

    \see ::KMSG_IDENT_NOTIFY_CREATE
*/
KHMEXP khm_int32 KHMAPI
kcdb_identpro_notify_create(khm_handle identity);

/*@}*/

/*! \brief Check if the given name is a valid identity name

    \return TRUE or FALSE to the question, is this valid?
*/
KHMEXP khm_boolean KHMAPI
kcdb_identity_is_valid_name(const wchar_t * name);

/*! \brief Create or open an identity.

    If the KCDB_IDENT_FLAG_CREATE flag is specified in the flags
    parameter a new identity will be created if one does not already
    exist with the given name.  If an identity by that name already
    exists, then the existing identity will be opened. The result
    parameter will receive a held reference to the opened identity.
    Use kcdb_identity_release() to release the handle.

    \param[in] name Name of identity to create
    \param[in] flags If KCDB_IDENT_FLAG_CREATE is specified, then the
        identity will be created if it doesn't already exist.
        Additional flags can be set here which will be assigned to the
        identity if it is created.  Additional flags have no effect if
        an existing identity is opened.
    \param[out] result If the call is successful, this receives a held
        reference to the identity.  The caller should call
        kcdb_identity_release() to release the identity once it is no
        longer needed.
    */
KHMEXP khm_int32 KHMAPI
kcdb_identity_create(const wchar_t *name,
                     khm_int32 flags,
                     khm_handle * result);

/*! \brief Mark an identity for deletion.

    The identity will be marked for deletion.  The
    KCDB_IDENT_FLAG_ACTIVE will no longer be present for this
    identity.  Once all references to the identity are released, it
    will be removed from memory.  All associated credentials will also
    be removed. */
KHMEXP khm_int32 KHMAPI
kcdb_identity_delete(khm_handle id);

/*! \brief Set or unset the specified flags in the specified identity.

    Only flags that are in KCDB_IDENT_FLAGMASK_RDWR can be specifed in
    the \a flags parameter or the \a mask parameter.  The flags set in
    the \a mask parameter of the identity will be set to the
    corresponding values in the \a flags parameter.

    If ::KCDB_IDENT_FLAG_INVALID is set using this function, then the
    ::KCDB_IDENT_FLAG_VALID will be automatically reset, and vice
    versa.  Resetting either bit does not undo this change, and will
    leave the identity's validity unspecified.

    Note that setting or resetting certain flags have other semantic
    side-effects:

    - ::KCDB_IDENT_FLAG_DEFAULT : Setting this is equivalent to
      calling kcdb_identity_set_default() with \a id.  Resetting this
      is equivalent to calling kcdb_identity_set_default() with NULL.

    - ::KCDB_IDENT_FLAG_SEARCHABLE : Setting this will result in the
      identity provider getting notified of the change. If the
      identity provider indicates that searchable flag should not be
      set or reset on the identity, then kcdb_identity_set_flags()
      will return an error.

    \note kcdb_identity_set_flags() is not atomic.  Even if the
    function returns a failure code, some flags in the identity may
    have been set.  When calling kcdb_identity_set_flags() always
    check the flags in the identity using kcdb_identity_get_flags() to
    check which flags have been set and which have failed.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_flags(khm_handle id,
                        khm_int32 flags,
                        khm_int32 mask);

/*! \brief Return all the flags for the identity

    The returned flags may include internal flags.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_flags(khm_handle id,
                        khm_int32 * flags);

/*! \brief Return the name of the identity

    \param[out] buffer Buffer to copy the identity name into.  The
        maximum size of an identity name is \a KCDB_IDENT_MAXCB_NAME.
        If \a buffer is \a NULL, then the required size of the buffer
        is returned in \a pcbsize.

    \param[in,out] pcbsize Size of buffer in bytes. */
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_name(khm_handle id,
                       wchar_t * buffer,
                       khm_size * pcbsize);

/*! \brief Set the specified identity as the default.

    Specifying NULL effectively makes none of the identities the
    default.

    \see kcdb_identity_set_flags()
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_default(khm_handle id);

/*! \brief Mark the specified identity as the default.

    This API is reserved for use by identity providers as a means of
    specifying which identity is default.  The difference between
    kcdb_identity_set_default() and kcdb_identity_set_default_int() is
    in semantics.

    - kcdb_identity_set_default() is used to request the KCDB to
      designate the specified identity as the default.  When
      processing the request, the KCDB invokes the identity provider
      to do the necessary work to make the identity the default.

    - kcdb_identity_set_default_int() is used by the identity provider
      to notify the KCDB that the specified identity is the default.
      This does not result in the invocation of any other semantics to
      make the identity the default other than releasing the previous
      defualt identity and making the specified one the default.  As
      an additional side effect, the notification <::KMSG_KCDB,
      ::KMSG_KCDB_IDENT, ::KCDB_OP_NEW_DEFAULT> will also not be sent.
 */
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_default_int(khm_handle id);

/*! \brief Get the default identity

    Obtain a held handle to the default identity if there is one.  The
    handle must be freed using kcdb_identity_release().

    If there is no default identity, then the handle pointed to by \a
    pvid is set to \a NULL and the function returns
    KHM_ERROR_NOT_FOUND. */
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_default(khm_handle * pvid);

/*! \brief Get the configuration space for the identity.

    If the configuration space for the identity does not exist and the
    flags parameter does not specify ::KHM_FLAG_CREATE, then the
    function will return a failure code as specified in
    ::khc_open_space().  Depending on whether or not a configuration
    space was found, the ::KCDB_IDENT_FLAG_CONFIG flag will be set or
    reset for the identity.

    \param[in] id Identity for which the configuraiton space is requested

    \param[in] flags Flags used when calling khc_open_space().  If \a
        flags specifies KHM_FLAG_CREATE, then the configuration space
        is created.

    \param[out] result The resulting handle.  If the call is
        successful, this receives a handle to the configuration space.
        Use khc_close_space() to close the handle.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_config(khm_handle id,
                         khm_int32 flags,
                         khm_handle * result);

/*! \brief Hold a reference to an identity.

    A reference to an identity (a handle) is only valid while it is
    held.  \note Once the handle is released, it can not be
    revalidated by calling kcdb_identity_hold().  Doing so would lead
    to unpredictable consequences. */
KHMEXP khm_int32 KHMAPI
kcdb_identity_hold(khm_handle id);

/*! \brief Release a reference to an identity.
    \see kcdb_identity_hold() */
KHMEXP khm_int32 KHMAPI
kcdb_identity_release(khm_handle id);

/*! \brief Set the identity provider subscription

    If there was a previous subscription, that subscription will be
    automatically deleted.

    \param[in] sub New identity provider subscription
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_provider(khm_handle sub);

/*! \brief Set the primary credentials type

    The primary credentials type is designated by the identity
    provider.  As such, this function should only be called by an
    identity provider.
 */
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_type(khm_int32 cred_type);

/*! \brief Retrieve the identity provider subscription

    \param[out] sub Receives the current identity provider
        subscription.  Set to NULL if only the existence of an
        identity provider needs to be checked.

    \retval KHM_ERROR_SUCCESS An identity provider exists.  If \a sub
        was not NULL, the subscription has been copied there.

    \retval KHM_ERROR_NOT_FOUND There is currently no registered
        identity provider.  If \a sub was not NULL, the handle it
        points to has been set to NULL.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_provider(khm_handle * sub);

/*! \brief Retrieve the identity provider credentials type

    This is the credentials type that the identity provider has
    designated as the primary credentials type.
 */
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_type(khm_int32 * ptype);

/*! \brief Returns TRUE if the two identities are equal

    Also returns TRUE if both identities are NULL.
 */
KHMEXP khm_boolean KHMAPI
kcdb_identity_is_equal(khm_handle identity1,
                       khm_handle identity2);

/*! \brief Set an attribute in an identity by attribute id

    \param[in] buffer A pointer to a buffer containing the data to
        assign to the attribute.  Setting \a buffer to NULL has the
        effect of removing any data that is already assigned to the
        attribute.  If \a buffer is non-NULL, then \a cbbuf should
        specify the number of bytes in \a buffer.

    \param[in] cbbuf Number of bytes of data in \a buffer.  The
        individual data type handlers may copy in less than this many
        bytes in to the credential.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_attr(khm_handle identity,
                       khm_int32 attr_id,
                       void * buffer,
                       khm_size cbbuf);

/*! \brief Set an attribute in an identity by name

    The attribute name has to be a KCDB registered attribute or
    property.

    \param[in] cbbuf Number of bytes of data in \a buffer.  The
        individual data type handlers may copy in less than this many
        bytes in to the credential.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_set_attrib(khm_handle identity,
                         wchar_t * attr_name,
                         void * buffer,
                         khm_size cbbuf);

/*! \brief Get an attribute from an identity by attribute id.

    \param[in] buffer The buffer that is to receive the attribute
        value.  Set this to NULL if only the required buffer size is
        to be returned.

    \param[in,out] cbbuf The number of bytes available in \a buffer.
        If \a buffer is not sufficient, returns KHM_ERROR_TOO_LONG and
        sets this to the required buffer size.

    \param[out] attr_type Receives the data type of the attribute.
        Set this to NULL if the type is not required.

    \note Set both \a buffer and \a cbbuf to NULL if only the
        existence of the attribute is to be checked.  If the attribute
        exists in this identity then the function will return
        KHM_ERROR_SUCCESS, otherwise it returns KHM_ERROR_NOT_FOUND.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_attr(khm_handle identity,
                       khm_int32 attr_id,
                       khm_int32 * attr_type,
                       void * buffer,
                       khm_size * pcbbuf);

/*! \brief Get an attribute from an identity by name.

    \param[in] buffer The buffer that is to receive the attribute
        value.  Set this to NULL if only the required buffer size is
        to be returned.

    \param[in,out] cbbuf The number of bytes available in \a buffer.
        If \a buffer is not sufficient, returns KHM_ERROR_TOO_LONG and
        sets this to the required buffer size.

    \note Set both \a buffer and \a cbbuf to NULL if only the
        existence of the attribute is to be checked.  If the attribute
        exists in this identity then the function will return
        KHM_ERROR_SUCCESS, otherwise it returns KHM_ERROR_NOT_FOUND.
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_attrib(khm_handle identity,
                         wchar_t * attr_name,
                         khm_int32 * attr_type,
                         void * buffer,
                         khm_size * pcbbuf);

/*! \brief Get the string representation of an identity attribute.

    A shortcut function which generates the string representation of
    an identity attribute directly.

    \param[in] identity A handle to an identity

    \param[in] attr_id The attribute to retrieve

    \param[out] buffer A pointer to a string buffer which receives the
        string form of the attribute.  Set this to NULL if you only
        want to determine the size of the required buffer.

    \param[in,out] pcbbuf A pointer to a #khm_int32 that, on entry,
        holds the size of the buffer pointed to by \a buffer, and on
        exit, receives the actual number of bytes that were copied.

    \param[in] flags Flags for the string conversion. Can be set to
        one of KCDB_TS_LONG or KCDB_TS_SHORT.  The default is
        KCDB_TS_LONG.

    \retval KHM_ERROR_SUCCESS Success
    \retval KHM_ERROR_NOT_FOUND The given attribute was either invalid
        or was not defined for this identity
    \retval KHM_ERROR_INVALID_PARAM One or more parameters were invalid
    \retval KHM_ERROR_TOO_LONG Either \a buffer was NULL or the
        supplied buffer was insufficient
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_attr_string(khm_handle identity,
                              khm_int32 attr_id,
                              wchar_t * buffer,
                              khm_size * pcbbuf,
                              khm_int32 flags);

/*! \brief Get the string representation of an identity attribute by name.

    A shortcut function which generates the string representation of
    an identity attribute directly.

    \param[in] identity A handle to an identity

    \param[in] attrib The name of the attribute to retrieve

    \param[out] buffer A pointer to a string buffer which receives the
        string form of the attribute.  Set this to NULL if you only
        want to determine the size of the required buffer.

    \param[in,out] pcbbuf A pointer to a #khm_int32 that, on entry,
        holds the size of the buffer pointed to by \a buffer, and on
        exit, receives the actual number of bytes that were copied.

    \param[in] flags Flags for the string conversion. Can be set to
        one of KCDB_TS_LONG or KCDB_TS_SHORT.  The default is
        KCDB_TS_LONG.

    \see kcdb_identity_get_attr_string()
*/
KHMEXP khm_int32 KHMAPI
kcdb_identity_get_attrib_string(khm_handle identity,
                                wchar_t * attr_name,
                                wchar_t * buffer,
                                khm_size * pcbbuf,
                                khm_int32 flags);

/*! \brief Enumerate identities

    Enumerates all the active identities that match the criteria
    specified using \a and_flags and \a eq_flags.  The condition is
    applied to all active identities as follows:

    \code
    (identity->flags & and_flags) == (eq_flags & and_flags)
    \endcode

    Essentially, if a flag is set in \a and_flags, then that flag in
    the identity should equal the setting in \a eq_flags.

    \param[in] and_flags See above

    \param[in] eq_flags See above

    \param[out] name_buf Buffer to receive the list of identity names.
        Can be NULL if only the required size of the buffer or the
        number of matching identities is required.  The list is
        returned as a multi string.

    \param[in,out] pcb_buf Number of bytes in buffer pointed to by \a
        name_buf on entry.  On exit, will receive the number of bytes
        copied.  Can be NULL only if \a name_buf is also NULL.  If \a
        name_buf is NULL or if \a pcb_buf indicates that the buffer is
        insufficient, this will receive the number of bytes required
        and the return value of the function will be
        KHM_ERROR_TOO_LONG

    \param[out] pn_idents Receives the number of identities that match
        the given criteria.

    \retval KHM_ERROR_SUCCESS If \a name_buf was valid, the buffer now
        contains a multi string of identities that matched.  If \a
        pn_idents was valid, it contains the number of identities
        matched.

    \retval KHM_ERROR_TOO_LONG No buffer was supplied or the supplied
        buffer was insufficient.  If \a pn_idents was valid, it
        contains the number of identities.

    \retval KHM_ERROR_INVALID_PARAM None of the parameters \a name_buf,
        \a pcb_buf and \a pn_idents were supplied, or \a pcb_buf was
        NULL when \a name_buf was not.

    \note Calling this function to obtain the required size of the
        buffer and then calling it with a that sized buffer is not
        guaranteed to work since the list of identities may change
        between the two calls.
  */
KHMEXP khm_int32 KHMAPI
kcdb_identity_enum(khm_int32 and_flags,
                   khm_int32 eq_flags,
                   wchar_t * name_buf,
                   khm_size * pcb_buf,
                   khm_size * pn_idents);

/*! \brief Refresh identity attributes based on root credential set

    Several flags in an identity are dependent on the credentials that
    are associated with it in the root credential set.  In addition,
    other flags in an identity depend on external factors that need to
    be verfied once in a while.  This API goes through the root
    credential set as well as consulting the identity provider to
    update an identity.

    \see kcdb_identity_refresh()
 */
KHMEXP khm_int32 KHMAPI
kcdb_identity_refresh(khm_handle vid);

/*! \brief Refresh all identities

    Equivalent to calling kcdb_identity_refresh() for all active
    identities.

    \see kcdb_identityt_refresh()
 */
KHMEXP khm_int32 KHMAPI
kcdb_identity_refresh_all(void);

/* KSMG_KCDB_IDENT notifications are structured as follows:
   type=KMSG_KCDB
   subtype=KMSG_KCDB_IDENT
   uparam=one of KCDB_OP_*
   blob=handle to identity in question */

/*@}*/


/*********************************************************************/


/*!
\defgroup kcdb_creds Credential sets and individual credentials

@{
*/


/*! \brief Credentials process function

    This function is called for each credential in a credential set
    when supplied to kcdb_credset_apply().  It should return
    KHM_ERROR_SUCCESS to continue the operation, or any other value to
    terminate the processing.

    \see kcdb_credset_apply()
*/
typedef khm_int32
(KHMAPI *kcdb_cred_apply_func)(khm_handle cred,
                               void * rock);

/*! \brief Credentials filter function.

    Should return non-zero if the credential passed as \a cred is to
    be "accepted".  The precise consequence of a non-zero return value
    is determined by the individual function that this call back is
    passed into.

    This function should not call any other function which may modify
    \a cred.

    \see kcdb_credset_collect_filtered()
    \see kcdb_credset_extract_filtered()
*/
typedef khm_int32
(KHMAPI *kcdb_cred_filter_func)(khm_handle cred,
                                khm_int32 flags,
                                void * rock);

/*! \brief Credentials compare function.

    Asserts a weak ordering on the credentials that are passed in as
    \a cred1 and \a cred2.  It should return:

    - a negative value if \a cred1 < \a cred2
    - zero if \a cred1 == \a cred2
    - a postive value if \a cred1 > \a cred2
    \see kcdb_credset_sort()
    \see ::kcdb_credtype
*/
typedef khm_int32
(KHMAPI *kcdb_cred_comp_func)(khm_handle cred1,
                              khm_handle cred2,
                              void * rock);

/*! \defgroup kcdb_credset Credential sets */
/*@{*/

/*! \brief Create a credential set.

    Credential sets are temporary containers for credentials.  These
    can be used by plug-ins to store credentials while they are being
    enumerated from an external source.  Once all the credentials have
    been collected into the credential set, the plug-in may call
    kcdb_credset_collect() to collect the credentials into the root
    credential store.

    The user interface will only display credentials that are in the
    root credential store.  No 