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

#ifndef __KHIMAIRA_KHERR_H
#define __KHIMAIRA_KHERR_H

/*! \defgroup kherr NetIDMgr Error Reporting

    Error reporting functions provide a mechanism to construct
    meaningful and user friendly error reports for the user.

    Unlike most of the other NetIDMgr API's, the error reporting APIs
    are lightweight and usually do not return an error value.  This is
    mostly because, these functions are called \b after an error
    occurs.

 @{*/
#include<khdefs.h>
#include<khlist.h>

/*! \name Customizable macros
@{ */
#ifndef KHERR_FACILITY
/*! \brief The default facility when reporting errors

    When including this header file, if the KHERR_FACILITY macro is
    defined to be a wide character string, then it will be used as the
    default facility when for the convenience macros.  All of the
    calls to the convenience macros in the source file would then have
    that facility.

    If left undefined, the convenience macros will leave the facility
    value undefined.
 */
#define KHERR_FACILITY NULL
#endif

#ifndef KHERR_FACILITY_ID
/*! \brief The default facility ID when reporting errors

    When including this header file, if the KHERR_FACILITY_ID macro is
    defined to be non-zero, then it will be used as the default
    facility identifier for the convenience macros.  All of the calls
    to the convenience macros in the source file would then have that
    facility identifier.

    The default value of 0 means that the facility is undefined.
 */
#define KHERR_FACILITY_ID 0
#endif

/*! \define KHERR_HMODULE (undefined)
    \brief The default module handle

    When including this header file, if the KHERR_HMODULE macro is
    defined to be an identifier that holds the module handle, then the
    convenience macros that specify a module handle will use it.

    A default value is not defined for KHERR_HMODULE.  Any attempt to
    invoke any of the convenience macros that use it should generate a
    compile time error.
 */
#ifdef _WIN32
#ifndef KHERR_HMODULE
#endif
#endif
/*@}*/

/*! \brief Parameter types
 */
enum kherr_parm_types {
    KEPT_NONE = 0,
    KEPT_INT32 = 1,
    KEPT_UINT32,
    KEPT_INT64,
    KEPT_UINT64,
    KEPT_STRINGC,               /*!< String constant */
    KEPT_STRINGT,               /*!< String.  Will be freed using
                                  free() when the event is freed */
    KEPT_PTR                    /*!< Pointer type. */
};


typedef struct tag_kherr_param {
    khm_octet type;
    khm_ui_8  data;
} kherr_param;

/*! \brief Severity levels

    Larger the value, the less severe it is.
*/
enum tag_kherr_severity {
  KHERR_FATAL = 0,              /*!< Fatal error.*/
  KHERR_ERROR,                  /*!< Non-fatal error.  We'll probably
				  survive.  See the suggested action. */
  KHERR_WARNING,                /*!< Warning. Something almost broke
				  or soon will.  See the suggested
				  action. */
  KHERR_INFO,                   /*!< Informational. Something happened
                                  that we would like you to know
                                  about. */
  KHERR_DEBUG_1 = 64,           /*!< Verbose debug level 1 (high)
				  Events at this severity level are
				  not required to be based on
				  localized strings. */
  KHERR_DEBUG_2 = 65,           /*!< Verbose debug level 2 (medium)
				  Events at this severity level are
				  not required to be based on
				  localized strings. */
  KHERR_DEBUG_3 = 66,           /*!< Verbose debug level 3 (low)
				  Events at this severity level are
				  not required to be based on
				  localized strings. */
  KHERR_RESERVED_BANK = 127,    /*!< Internal use */
  KHERR_NONE = 128              /*!< Nothing interesting has happened
                                  so far */
};

typedef enum tag_kherr_severity kherr_severity;

/*! \brief Suggestions */
enum tag_kherr_suggestion {
    KHERR_SUGGEST_NONE = 0,     /*!< No suggestions.  */
    KHERR_SUGGEST_ABORT,        /*!< Abort whatever it was you were
			          trying.  It's not gonna work. */
    KHERR_SUGGEST_RETRY,        /*!< Retry.  It might work the second
			          or third time over */
    KHERR_SUGGEST_IGNORE,       /*!< Ignore. It might go away. */
    KHERR_SUGGEST_INTERACT,     /*!< Further user interaction is
                                  necessary to resolve the situation.
                                  The suggest string in the event
                                  should be prompted to the user. */
    KHERR_SUGGEST_OTHER,        /*!< Something else. */
};

typedef enum tag_kherr_suggestion kherr_suggestion;

/*! \brief An event */
typedef struct tag_kherr_event {
    khm_int32   magic;          /*!< Magic number.  Always set to
                                  KHERR_EVENT_MAGIC */
    DWORD       thread_id;      /*!< The thread which reported this
                                  event. */
    const wchar_t * short_desc; /*!< Short description or title
                                   (localized) */
    const wchar_t * facility;   /*!< Facility name of the reporter
                                  (not localized) */
    const wchar_t * location;   /*!< Location.  Usually the function
			          name or such of where the event
			          occured (not localized) */
    const wchar_t * long_desc;  /*!< A long description of what went
			          wrong (localized, formatted) */
    const wchar_t * suggestion; /*!< A suggested way to fix it
			          (localized,formatted) */

    kherr_severity   severity;
                                /*!< Severity level.  One of the
				  severity levels listed in
				  enumeration ::kherr_severity */
    khm_int32   facility_id;    /*!< Left to the application to
				  interpret */
    kherr_suggestion suggestion_id;
                                /*!< One of the suggestion ID's from
				  the enumeration
				  ::kherr_suggestion */

    int         flags;          /*!< Flags. */

    kherr_param p1;             /*!< Parameter 1 for formatting */
    kherr_param p2;             /*!< Parameter 2 for formatting */
    kherr_param p3;             /*!< Parameter 3 for formatting */
    kherr_param p4;             /*!< Parameter 4 for formatting */

    DWORD       time_ticks;     /*!< Time at which event was reported
                                  (as returned by GetTickCount(). */
    FILETIME    time_ft;        /*!< Time at which event was reported.
                                  Current system time as FILETIME. */

#ifdef _WIN32
    HMODULE     h_module;       /*!< Handle to the module which should
                                  resolve any unresolved resources
                                  references above.  */
#endif

    LDCL(struct tag_kherr_event);
} kherr_event;

#define KHERR_EVENT_MAGIC 0x0423e84f

/*! \brief Flags for kherr_event

    Each set of flags that define the type of resource for one value
    is mutually exclusive.
 */
enum kherr_event_flags {
    KHERR_RF_CSTR_SHORT_DESC= 0x00000000,
                                /*!< Short description is a constant
                                  string */
    KHERR_RF_RES_SHORT_DESC = 0x00000001,
                                /*!< Short description is a string
                                  resource */
    KHERR_RF_MSG_SHORT_DESC = 0x00000002,
                                /*!< Short description is a message
                                  resource */
    KHERR_RF_FREE_SHORT_DESC= 0x00000004,
                                /*!< Short description is an allocated
                                  string */
    KHERR_RFMASK_SHORT_DESC = 0x00000007,

    KHERR_RF_CSTR_LONG_DESC = 0x00000000,
                                /*!< Long description is a constant
                                  string */
    KHERR_RF_RES_LONG_DESC  = 0x00000008,
                                /*!< Long description is a string
                                  resource */
    KHERR_RF_MSG_LONG_DESC  = 0x00000010,
                                /*!< Long description is a message
                                  resouce  */
    KHERR_RF_FREE_LONG_DESC = 0x00000020,
                                /*!< Long description is an allocated
                                  string */
    KHERR_RFMASK_LONG_DESC  = 0x00000038,

    KHERR_RF_CSTR_SUGGEST   = 0x00000000,
                                /*!< Suggestion is a constant
                                  string */
    KHERR_RF_RES_SUGGEST    = 0x00000040,
                                /*!< Suggestion is a string
                                  resource */
    KHERR_RF_MSG_SUGGEST    = 0x00000080,
                                /*!< Suggestion is a message
                                  resource */
    KHERR_RF_FREE_SUGGEST   = 0x00000100,
                                /*!< Suggestion is an allocated
                                  string */
    KHERR_RFMASK_SUGGEST    = 0x000001C0,

    KHERR_RF_STR_RESOLVED   = 0x00010000,
                                /*!< The string resources in the event
                                  have been resolved. */
    KHERR_RF_CONTEXT_FOLD   = 0x00020000,
                                /*!< The event is a representation of
                                  a folded context. */

    KHERR_RF_INERT          = 0x00040000,
                                /*!< Inert event.  The event has
                                  already been dealt with and is no
                                  longer considered significant. */
    KHERR_RF_COMMIT         = 0x00080000
				/*!< Committed event.  The commit
				  handlers for this event have already
				  been called. */
};

/*! \brief Serial number for error contexts */
typedef khm_ui_4 kherr_serial;

/*! \brief An error context
*/
typedef struct tag_kherr_context {
    khm_int32      magic;       /*!< Magic number. Always set to
                                  KHERR_CONTEXT_MAGIC */

    kherr_serial   serial;      /*!< Context instance serial number.
                                  Context objects themselves may be
                                  reused for different contexts as
                                  they are freed and reallocated.
                                  However every instance of a context
                                  is guaranteed to have a unique
                                  serial number as specified in this
                                  field.  If an external entity wants
                                  to keep track of the context, it
                                  should keep track of the serial
                                  number as well as the pointer to the
                                  context object. */

    kherr_severity severity;
				/*!< Severity level.  One of the
				  severity levels listed below. This
				  is the severity level of the context
				  and is the maximum severity level of
				  all the events in the queue of
				  events. */

    khm_int32      flags;       /*!< Flags.  Used internally. */
    khm_ui_4       refcount;    /*!< Reference count. Used
                                  internally */

    kherr_event    *desc_event; /*!< Description event. The event that
                                  describes the error context.  This
                                  points to an event that is not in
                                  the event queue. */

    kherr_event    *err_event;  /*!< Significant event.  The last one
				  that caused the severity level to be
				  what it is right now.  This points
				  to an event that is listed in the
				  event queue for this context.*/

    khm_ui_4 progress_num;      /*!< Progress numerator */
    khm_ui_4 progress_denom;    /*!< Progress denominator */

    TDCL(struct tag_kherr_context);
    QDCL(struct tag_kherr_event);
} kherr_context;

#define KHERR_CONTEXT_MAGIC 0x34f3238c

enum kherr_context_flags {
    KHERR_CF_NONE          = 0x00000000,
                                /*!< None. */

    KHERR_CF_DIRTY         = 0x00000001,
                                /*!< Used Internally.  Denotes that
                                  the err_event and severity may need
                                  to be recalculated.  Cannot be set
                                  as an initial flag. */

    KHERR_CF_OWN_PROGRESS  = 0x00000002,
                                /*!< The context maintains its own
                                  progress meter as opposed to one
                                  that is derived from child
                                  contexts. */

    KHERR_CF_UNBOUND       = 0x00000004,
                                /*!< Unbound context.  The context
                                  can't be used to log events.  Call
                                  kherr_push_context() to associate
                                  the context with the global context
                                  hierarchy. Cannot be set as an
                                  initial flag. */

    KHERR_CF_TRANSITIVE    = 0x00000008,
                                /*!< Transitive. The context is
                                  automatically made the current
                                  context for all other threads that
                                  handle messages sent or posted by
                                  threads whose current error context
                                  is this one. */

    KHERR_CFMASK_INITIAL = 0x0000000a,
                                /*!< Allowed initial flags */
};

/*! \brief Maximum length of a string field in characters including terminating NULL
 */
#define KHERR_MAXCCH_STRING 1024

/*! \brief Maximum length of a string field in bytes including terminating NULL
 */
#define KHERR_MAXCB_STRING (KHERR_MAXCCH_STRING * sizeof(wchar_t))

/*! \brief Context event

    \see kherr_add_ctx_handler()
*/
enum kherr_ctx_event {
    KHERR_CTX_BEGIN  = 0x0001,  /*!< A new context was created */
    KHERR_CTX_DESCRIBE=0x0002,  /*!< A context was described */
    KHERR_CTX_END    = 0x0004,  /*!< A context was closed */
    KHERR_CTX_ERROR  = 0x0008,  /*!< A context switched to an error
                                  state */
    KHERR_CTX_EVTCOMMIT = 0x0010 /*!< A event was committed into the
				   context */
};

/*! \brief Context event handler

    Context event handlers are invoked when specific events occur with
    respect to an error context.  The ::kherr_ctx_event parameter
    specifies which event occurred using one of the event values
    described in the enumeration.  The error context in which this
    event occurred is specified by the ::kherr_context pointer.

    Note that if the handler needs to keep track of the error context
    for later processing, it also needs to keep track of the \a serial
    field of the error context.  The same context object may be
    reused, but the serial number is guaranteed to be unique.

    \see kherr_add_ctx_handler()
 */
typedef void (KHMAPI * kherr_ctx_handler)(enum kherr_ctx_event,
                                         kherr_context *);

/*! \brief Add a context event handler

    An application can register an event handler that gets notified of
    events that pertain to error contexts.  More than one handler can
    be registered.  The order in which the handlers are called is
    undefined for any specific event.

    These event occur in the context of individual application
    threads.  The handler will be called from within the thread that
    caused the event.  Therefore it is important that the handler is
    both reentrant and returns quickly.

    The events that the handler will be notified of are explained
    below:

    <b>KHERR_CTX_BEGIN</b>: Notification that a new context was
    created.  A pointer to the context will be supplied to the
    handler.  The supplied pointer should not be used to obtain a hold
    on the context, as it will prevent the context from being closed.

    <b>KHERR_CTX_DESCRIBE</b>: The thread called
    kherr_set_desc_event() to set the description of a context.  Once
    again, the pointer should not be used to obtain a hold on the
    context.

    <b>KHERR_CTX_ERROR</b>: The last event that was reported for the
    context was an error event (the severity was was equal or higher
    than KHERR_ERROR).  The pointer may be used to obtain a hold on
    the context.  However, it is the application's resonsibility to
    make sure that the hold is released later.  Otherwise the event
    will never be closed.

    <b>KHERR_CTX_END</b>: Closure.  This event is signalled when the
    last open handle to the context is closed and there is no thread
    that is currently active which has this context in its error
    context stack.  At the time the handler is invoked, the context is
    still intact.  The pointer that is supplied should not be used to
    obtain a handle on the context.

    <b>KHERR_CTX_EVTCOMMIT</b>: An event was committed into the error
    context.  An event is committed when another event is reported
    after the event, or if the context is closed.  Since the last
    event that is reported can still be modified by adding new
    information, the event remains open until it is no longer the last
    event or the context is no longer active.  When this notification
    is received, the last event in the context's event queue is the
    event that was committed.

    \param[in] h Context event handler, of type ::kherr_ctx_handler

    \param[in] filter A combination of ::kherr_ctx_event values
        indication which notifications should be sent to the handler.
        If a \a filter value of zero is provided, all of the events
        will be sent to the handler.

    \param[in] serial The serial number of the error context that
        should be tracked.  If this is zero, all error contexts can
        trigger the handler.
 */
KHMEXP void KHMAPI kherr_add_ctx_handler(kherr_ctx_handler h,
                                         khm_int32 filter,
                                         kherr_serial serial);

/*! \brief Remove a context event handler

    Undoes what was done with kherr_add_ctx_handler()

    \see kherr_add_ctx_handler()
 */
KHMEXP void KHMAPI kherr_remove_ctx_handler(kherr_ctx_handler h,
                                            kherr_serial serial);


/*! \brief Report an error

    Creates an event, fills in the details specified in the arguments,
    and adds it to the current error context.

    If the current thread does not have an error context, no reporting
    happens.  However, if any of the supplied strings or parameters
    are marked as allocated, they will be freed before the function
    returns.

    Certain parameters that expect strings can instead be given string
    resources, message resources or allocated strings in addition to
    constant string.  By default, the parameters are expected to be
    constant strings.

    <b>Allocated strings</b>: The application can allocate memory for
    a string.  Since the application is not notified when the event is
    no longer used and freed, it \b must indicate that the string is
    an allocated string by setting the appropriate flag in the \a
    flags parameter.  When the event is no longer used, the memory
    pointed to by the relevant pointer will be freed through a call to
    free().  Not all string parameters take allocated strings.  See
    individual parameter documentation for details.

    <b>String resources</b>: On WIN32, string resources can be passed
    in to kherr_report() using the MAKEINTRESOURCE macro.  However,
    the application \b must specify that the parameter is a string
    resource using the appropriate flag in the \a flags parameter.
    The error reporting engine will expand the string against the
    module handle passed in the \a h_module parameter when the value
    of the string is required.  Not all string parameters take string
    resources.  See individual parameter documentation for details.
    Strings loaded through string resources cannot be longer than
    ::KHERR_MAXCCH_STRING in characters inclusive of terminating NULL.

    <b>Message resources</b>: On WIN32, message resources can be
    passed in to kherr_report() by specifying the message ID where it
    ordinarily expects a pointer to a constant string.  The
    application \b must indicate that the string is a message resource
    by using the appropriate flag in the \a flags parameter.  When the
    value of the string is needed, it is expanded against the module
    handle passed in the \a h_module parameter using the message ID.
    Not all string parameters take message resources.  See individual
    parameter documentation for details.  Note that the facility and
    severity values associated with a message resource are ignored.
    Strings loaded through message resources cannot be longer than
    ::KHERR_MAXCCH_STRING in characters inclusive of terminating NULL.

    <b>Formatted fields</b>: Parameters that are formatted can have
    can have parameter inserts like in printf(). However, specifying
    inserts is different from printf() and follows the conventions
    used in WIN32 API FormatMessage().  This is because for localized
    strings, the order of the parameters in the string may be
    different.  See the documentation for FormatMessage() for details
    on the format string.  The same set of parameters (i.e. \a p1, \a
    p2, \a p3, \a p4) is used for all formatted strings with
    appropriate marshalling for 64 bit types.  The size of the string
    after expansion must not exceed 65536 bytes inclusive of
    terminating NULL.

    \param[in] severity One of ::kherr_severity_level
    \param[in] short_desc Short description or title (localized).  Can
        be a string resource, message resource, allocated string or
        constant string.  The \a flags parameter should indicate the
        type of string used.
    \param[in] facility Facility name of the reporter (not localized)
    \param[in] location Usually the function name or such of where the
        event occured (not localized)
    \param[in] long_desc Long description of event (localized,
        formatted). Can be a string resource, message resource,
        allocated string or constant string.  The \a flags parameter
        should indicate the type of string used.
    \param[in] suggestion Suggested action to correct situation, if
        applicable (localized). Can be a string resource, message
        resource, allocated string or constant string.  The \a flags
        parameter should indicate the type of string used.
    \param[in] facility_id Identifier of facility.  Application
        defined.
    \param[in] suggestion_id One of the suggestion identifiers from
        ::kherr_suggestion_ids
    \param[in] p1 First parameter. Used for formatting.
    \param[in] p2 Second parameter. Used for formatting.
    \param[in] p3 Third parameter. Used for formatting.
    \param[in] p4 Fourth parameter. Used for formatting.
    \param[in] flags Flags.  See ::kherr_report_flags
    \param[in] h_module Handle to a module that resolves any string or
        message resources used for the \a short_description , \a
        long_desc or \a suggestion parameters.  This parameter is only
        available on WIN32.

    \note With the exception of parameters of type KEPT_STRINGT and
        parameters which are flagged for freeing using the \a flags
        parameter, all other string parameters are assumed to be
        pointers to constant strings.  The strings are not copied and
        the pointers are used as is.  Also, no clean-up is performed
        when the event is freed other than that implied by \a flags.
 */
KHMEXP kherr_event * KHMAPI kherr_report(
    enum kherr_severity severity,
    const wchar_t * short_desc,
    const wchar_t * facility,
    const wchar_t * location,
    const wchar_t * long_desC,
    const wchar_t * suggestion,
    khm_int32 facility_id,
    enum kherr_suggestion suggestion_id,
    kherr_param p1,
    kherr_param p2,
    kherr_param p3,
    kherr_param p4,
    khm_int32 flags
#ifdef _WIN32
    ,HMODULE  h_module
#endif
);

/*! \brief Report a formatted message

    The format string \a long_desc_fmt should be a string constant and
    the format specifiers follow that of \a sprintf.  This creates an
    event with the long description set to the expansion of the format
    string against the arguments.
 */
KHMEXP kherr_event * __cdecl
kherr_reportf_ex(enum kherr_severity severity,
                 const wchar_t * facility,
                 khm_int32 facility_id,
#ifdef _WIN32
                 HMODULE hModule,
#endif
                 const wchar_t * long_desc_fmt,
                 ...);
#define _reportf_ex kherr_reportf_ex

/*! \brief Report a formatted message

    The format string \a long_desc_fmt should be a string constant and
    the format specifiers follow that of \a sprintf.  This creates an
    event with the long description set to the expansion of the format
    string against the arguments.
 */
KHMEXP kherr_event * __cdecl
kherr_reportf(const wchar_t * long_desc_fmt,
              ...);
#define _reportf kherr_reportf

/*! \brief Create a parameter out of a transient string

    A parameter is created by duplicating the string that is passed
    into the function.  If the string exceeds KHERR_MAXCCH_STRING,
    then only the first part of the string that fits within the limit
    is duplicated.

    The resulign ::kherr_param must be passed in to kherr_report().
    The event logging framework will free the duplicated string once
    the data is no longer required.
 */
KHMEXP kherr_param kherr_dup_string(const wchar_t * s);

__inline KHMEXP kherr_param
kherr_val(khm_octet ptype, khm_ui_8 pvalue) {
    kherr_param p;

    p.type = ptype;
    p.data = pvalue;

    return p;
}

#define _int32(i)   kherr_val(KEPT_INT32, (khm_ui_8) i)
#define _uint32(ui) kherr_val(KEPT_UINT32, (khm_ui_8) ui)
#define _int64(i)   kherr_val(KEPT_INT64, (khm_ui_8) i)
#define _uint64(ui) kherr_val(KEPT_UINT64, (khm_ui_8) ui)
#define _cstr(cs)   kherr_val(KEPT_STRINGC, (khm_ui_8) cs)
#define _tstr(ts)   kherr_val(KEPT_STRINGT, (khm_ui_8) ts)
#define _cptr(p)    kherr_val(KEPT_PTR, (khm_ui_8) p)
#define _vnull()    kherr_val(KEPT_NONE, 0)
#define _dupstr(s)  kherr_dup_string(s)

/* convenience macros for calling kherr_report */
#ifdef KHERR_HMODULE

#define _report_cs0(severity, long_description)                 \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, _vnull(), _vnull(), _vnull(), _vnull(), 0, KHERR_HMODULE)

#define _report_cs1(severity, long_description, p1)             \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, _vnull(), _vnull(), _vnull(), 0, KHERR_HMODULE)

#define _report_cs2(severity, long_description, p1, p2)         \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, p2, _vnull(), _vnull(), 0, KHERR_HMODULE)

#define _report_cs3(severity, long_description, p1, p2, p3)     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, _vnull(), 0, KHERR_HMODULE)

#define _report_cs4(severity, long_description, p1, p2, p3, p4) \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, p4, 0, KHERR_HMODULE)

#else

#define _report_cs0(severity, long_description)                 \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, _vnull(), _vnull(), _vnull(), _vnull(), 0, NULL)

#define _report_cs1(severity, long_description, p1)             \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, _vnull(), _vnull(), _vnull(), 0, NULL)

#define _report_cs2(severity, long_description, p1, p2)         \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, p2, _vnull(), _vnull(), 0, NULL)

#define _report_cs3(severity, long_description, p1, p2, p3)     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, _vnull(), 0, NULL)

#define _report_cs4(severity, long_description, p1, p2, p3, p4) \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_description), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, p4, 0, NULL)
#endif /* !defined(KHERR_HMODULE) */

#ifdef _WIN32
#define _report_sr0(severity, long_desc_id)                     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, MAKEINTRESOURCE(long_desc_id), NULL, KHERR_FACILITY_ID, 0, _vnull(), _vnull(), _vnull(), _vnull(), KHERR_RF_RES_LONG_DESC, KHERR_HMODULE)

#define _report_sr1(severity, long_desc_id, p1)                 \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, MAKEINTRESOURCE(long_desc_id), NULL, KHERR_FACILITY_ID, 0, p1, _vnull(), _vnull(), _vnull(), KHERR_RF_RES_LONG_DESC, KHERR_HMODULE)

#define _report_sr2(severity, long_desc_id, p1, p2)             \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, MAKEINTRESOURCE(long_desc_id), NULL, KHERR_FACILITY_ID, 0, p1, p2, _vnull(), _vnull(), KHERR_RF_RES_LONG_DESC, KHERR_HMODULE)

#define _report_sr3(severity, long_desc_id, p1, p2, p3)         \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, MAKEINTRESOURCE(long_desc_id), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, _vnull(), KHERR_RF_RES_LONG_DESC, KHERR_HMODULE)

#define _report_sr4(severity, long_desc_id, p1, p2, p3, p4)     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, MAKEINTRESOURCE(long_desc_id), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, p4, KHERR_RF_RES_LONG_DESC, KHERR_HMODULE)
#endif

#ifdef _WIN32
#define _report_mr0(severity, long_desc_msg_id)                     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (wchar_t *)(long_desc_msg_id), NULL, KHERR_FACILITY_ID, 0, _vnull(), _vnull(), _vnull(), _vnull(), KHERR_RF_MSG_LONG_DESC, KHERR_HMODULE)

#define _report_mr1(severity, long_desc_msg_id, p1)                 \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (wchar_t *)(long_desc_msg_id), NULL, KHERR_FACILITY_ID, 0, p1, _vnull(), _vnull(), _vnull(), KHERR_RF_MSG_LONG_DESC, KHERR_HMODULE)

#define _report_mr2(severity, long_desc_msg_id, p1, p2)             \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (wchar_t *)(long_desc_msg_id), NULL, KHERR_FACILITY_ID, 0, p1, p2, _vnull(), _vnull(), KHERR_RF_MSG_LONG_DESC, KHERR_HMODULE)

#define _report_mr3(severity, long_desc_msg_id, p1, p2, p3)         \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (wchar_t *)(long_desc_msg_id), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, _vnull(), KHERR_RF_MSG_LONG_DESC, KHERR_HMODULE)

#define _report_mr4(severity, long_desc_msg_id, p1, p2, p3, p4)     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (wchar_t *)(long_desc_msg_id), NULL, KHERR_FACILITY_ID, 0, p1, p2, p3, p4, KHERR_RF_MSG_LONG_DESC, KHERR_HMODULE)
#endif

#define _report_ts0(severity, long_desc_ptr)                     \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_desc_ptr), NULL, KHERR_FACILITY_ID, 0, _vnull(), _vnull(), _vnull(), _vnull(), KHERR_RF_FREE_LONG_DESC, NULL)

#define _report_ts1(severity, long_desc_ptr, p1)                 \
    kherr_report((severity), NULL, KHERR_FACILITY, NULL, (long_desc_ptr), NULL, KHERR_F