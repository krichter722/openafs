/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 *
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 */

#ifndef _AFS_PROTOTYPES_H_
#define _AFS_PROTOTYPES_H_

/* afs_analyze.c */
extern int afs_Analyze(struct afs_conn *aconn, struct rx_connection *rxconn,
                       afs_int32 acode, struct VenusFid *afid,
                       struct vrequest *areq,
		       int op, afs_int32 locktype, struct cell *cellp);

/* afs_axscache.c */
extern afs_rwlock_t afs_xaxs;
extern struct axscache *afs_SlowFindAxs(struct axscache **cachep,
					afs_int32 id);
extern struct axscache *axs_Alloc(void);
extern void afs_RemoveAxs(struct axscache **headp, struct axscache *axsp);
extern void afs_FreeAllAxs(struct axscache **headp);
extern void shutdown_xscache(void);

/* afs_buffer.c */
extern void shutdown_bufferpackage(void);

/* afs_call.c */
extern int afs_cold_shutdown;
extern char afs_rootVolumeName[64];
extern void afs_shutdown(void);
extern void afs_FlushCBs(void);
extern int afs_CheckInit(void);
extern void afs_shutdown(void);
extern void shutdown_afstest(void);
extern int afs_syscall_call(long parm, long parm2, long parm3,
			    long parm4, long parm5, long parm6);
#if defined(AFS_DARWIN100_ENV)
extern int afs_syscall64_call(user_addr_t parm, user_addr_t parm2,
			      user_addr_t parm3, user_addr_t parm4,
			      user_addr_t parm5, user_addr_t parm6);
#endif


/* afs_callback.c */
extern afs_int32 afs_allCBs;
extern afs_int32 afs_oddCBs;
extern afs_int32 afs_evenCBs;
extern afs_int32 afs_allZaps;
extern afs_int32 afs_oddZaps;
extern afs_int32 afs_evenZaps;
extern afs_int32 afs_connectBacks;
extern unsigned long lastCallBack_vnode;
extern unsigned int lastCallBack_dv;
extern osi_timeval_t lastCallBack_time;
extern struct interfaceAddr afs_cb_interface;

extern int afs_RXCallBackServer(void);
extern int SRXAFSCB_GetCE(struct rx_call *a_call, afs_int32 a_index,
			  struct AFSDBCacheEntry *a_result);
extern int SRXAFSCB_GetCE64(struct rx_call *a_call, afs_int32 a_index,
			    struct AFSDBCacheEntry64 *a_result);
extern int SRXAFSCB_GetLock(struct rx_call *a_call, afs_int32 a_index,
			    struct AFSDBLock *a_result);
extern int SRXAFSCB_CallBack(struct rx_call *a_call,
			     struct AFSCBFids *a_fids,
			     struct AFSCBs *a_callbacks);
extern int SRXAFSCB_Probe(struct rx_call *a_call);
extern int SRXAFSCB_InitCallBackState(struct rx_call *a_call);
extern int SRXAFSCB_XStatsVersion(struct rx_call *a_call,
				  afs_int32 * a_versionP);
extern int SRXAFSCB_GetXStats(struct rx_call *a_call,
			      afs_int32 a_clientVersionNum,
			      afs_int32 a_collectionNumber,
			      afs_int32 * a_srvVersionNumP,
			      afs_int32 * a_timeP, AFSCB_CollData * a_dataP);
extern int afs_RXCallBackServer(void);
extern int shutdown_CB(void);
extern int SRXAFSCB_InitCallBackState2(struct rx_call *a_call,
				       struct interfaceAddr *addr);
extern int SRXAFSCB_WhoAreYou(struct rx_call *a_call,
			      struct interfaceAddr *addr);
extern int SRXAFSCB_InitCallBackState3(struct rx_call *a_call,
				       afsUUID * a_uuid);
extern int SRXAFSCB_ProbeUuid(struct rx_call *a_call, afsUUID * a_uuid);
extern int SRXAFSCB_GetServerPrefs(struct rx_call *a_call, afs_int32 a_index,
				   afs_int32 * a_srvr_addr,
				   afs_int32 * a_srvr_rank);
extern int SRXAFSCB_GetCellServDB(struct rx_call *a_call, afs_int32 a_index,
				  char **a_name, serverList * a_hosts);
extern int SRXAFSCB_GetLocalCell(struct rx_call *a_call, char **a_name);
extern int SRXAFSCB_GetCacheConfig(struct rx_call *a_call,
				   afs_uint32 callerVersion,
				   afs_uint32 * serverVersion,
				   afs_uint32 * configCount,
				   cacheConfig * config);
extern int SRXAFSCB_FetchData(struct rx_call *rxcall, struct AFSFid *Fid,
			      afs_int32 Fd, afs_int64 Position,
			      afs_int64 Length, afs_int64 * TotalLength);
extern int SRXAFSCB_StoreData(struct rx_call *rxcall, struct AFSFid *Fid,
			      afs_int32 Fd, afs_int64 Position,
			      afs_int64 Length, afs_int64 * TotalLength);


/* afs_cbqueue.c */
extern afs_rwlock_t afs_xcbhash;
extern void afs_QueueCallback(struct vcache *avc, unsigned int atime,
			      struct volume *avp);
extern void afs_CheckCallbacks(unsigned int secs);
extern void afs_FlushCBs(void);
extern void afs_FlushServerCBs(struct server *srvp);
extern int afs_BumpBase(void);
extern void afs_InitCBQueue(int doLockInit);
extern void afs_DequeueCallback(struct vcache *avc);

/* afs_cell.c */
extern afs_int32 afs_cellindex;
extern afs_rwlock_t afs_xcell;
extern afs_rwlock_t afsdb_client_lock;
extern afs_rwlock_t afsdb_req_lock;
extern struct afs_q CellLRU;

extern void afs_CellInit(void);
extern void shutdown_cell(void);
extern int afs_cellname_init(afs_dcache_id_t *inode, int lookupcode);
extern int afs_cellname_write(void);
extern afs_int32 afs_NewCell(char *acellName, afs_int32 * acellHosts,
			     int aflags, char *linkedcname, u_short fsport,
			     u_short vlport, int timeout);
extern afs_int32 afs_SetPrimaryCell(char *acellName);
extern struct cell *afs_GetCell(afs_int32 acell, afs_int32 locktype);
extern struct cell *afs_GetCellStale(afs_int32 acell, afs_int32 locktype);
extern struct cell *afs_GetCellByHandle(void *handle, afs_int32 locktype);
extern struct cell *afs_GetCellByIndex(afs_int32 cellidx, afs_int32 locktype);
extern struct cell *afs_GetCellByName(char *acellName, afs_int32 locktype);
extern struct cell *afs_GetPrimaryCell(afs_int32 locktype);
extern afs_int32 afs_GetPrimaryCellNum(void);
extern int afs_IsPrimaryCellNum(afs_int32 cellnum);
extern int afs_IsPrimaryCell(struct cell *cell);
extern void *afs_TraverseCells(void *(*cb) (struct cell *, void *),
			       void *arg);
extern int afs_CellOrAliasExists(char *aname);
extern int afs_CellNumValid(afs_int32 cellnum);
extern afs_int32 afs_NewCellAlias(char *alias, char *cell);
extern struct cell_alias *afs_GetCellAlias(int index);
extern void afs_PutCellAlias(struct cell_alias *a);
extern int afs_AFSDBHandler(char *acellName, int acellNameLen,
			    afs_int32 * kernelMsg);
extern void afs_LookupAFSDB(char *acellName);
extern void afs_StopAFSDB(void);
extern void afs_RemoveCellEntry(struct server *srvp);

/* afs_chunk.c */
extern afs_int32 afs_FirstCSize;
extern afs_int32 afs_OtherCSize;
extern afs_int32 afs_LogChunk;

/* afs_cell.c */
extern struct cell *afs_GetRealCellByIndex(afs_int32 cellindex,
					   afs_int32 locktype,
					   afs_int32 refresh);

/* afs_conn.c */
extern afs_int32 cryptall;
extern afs_rwlock_t afs_xinterface;
extern afs_rwlock_t afs_xconn;
extern struct afs_conn *afs_Conn(struct VenusFid *afid,
			     struct vrequest *areq,
			     afs_int32 locktype,
                             struct rx_connection **rxconn);
extern struct afs_conn *afs_ConnBySA(struct srvAddr *sap, unsigned short aport,
				 afs_int32 acell, struct unixuser *tu,
				 int force_if_down, afs_int32 create,
				 afs_int32 locktype, afs_int32 replicated,
				 struct rx_connection **rxconn);
extern struct afs_conn *afs_ConnByMHosts(struct server *ahosts[],
				     unsigned short aport, afs_int32 acell,
				     struct vrequest *areq,
				     afs_int32 locktype,
				     afs_int32 replicated,
				     struct rx_connection **rxconn);
extern struct afs_conn *afs_ConnByHost(struct server *aserver,
				   unsigned short aport, afs_int32 acell,
				   struct vrequest *areq, int aforce,
				   afs_int32 locktype,
				   afs_int32 replicated,
				   struct rx_connection **rxconn);
extern void afs_PutConn(struct afs_conn *ac, struct rx_connection *rxconn,
                        afs_int32 locktype);
extern void afs_ReleaseConns(struct sa_conn_vector *tcv);
extern void afs_ReleaseConnsUser(register struct unixuser *au);
extern void ForceNewConnections(struct srvAddr *sap);


/* afs_daemons.c */
extern afs_lock_t afs_xbrs;
extern short afs_brsWaiters;
extern short afs_brsDaemons;
extern struct brequest afs_brs[NBRS];
extern struct afs_osi_WaitHandle AFS_WaitHandler, AFS_CSWaitHandler;
extern afs_int32 afs_gcpags;
extern afs_int32 afs_gcpags_procsize;
extern afs_int32 afs_CheckServerDaemonStarted;
extern afs_int32 afs_probe_interval;
extern afs_int32 afs_preCache;

extern void afs_Daemon(void);
extern struct brequest *afs_BQueue(short aopcode,
				   struct vcache *avc,
				   afs_int32 dontwait, afs_int32 ause,
				   afs_ucred_t *acred,
				   afs_size_t asparm0, afs_size_t asparm1,
				   void *apparm0, void *apparm1,
				   void *apparm2);
extern void afs_SetCheckServerNATmode(int isnat);
extern void afs_CheckServerDaemon(void);
extern int afs_CheckRootVolume(void);
extern void afs_BRelease(struct brequest *ab);
extern int afs_BBusy(void);
extern int afs_BioDaemon(afs_int32 nbiods);
#ifdef AFS_NEW_BKG
extern int afs_BackgroundDaemon(struct afs_uspc_param *uspc, void *param1, void *param2);
#else
extern void afs_BackgroundDaemon(void);
#endif
extern void shutdown_daemons(void);
extern int afs_sgidaemon(void);


/* afs_dcache.c */
extern u_int afs_min_cache;
extern afs_int32 *afs_dvhashTbl;
extern afs_int32 afs_dhashsize;
extern afs_rwlock_t afs_xdcache;
extern afs_size_t afs_vmMappingEnd;
extern afs_int32 afs_blocksUsed;
extern afs_int32 afs_blocksDiscarded;
extern int afs_WaitForCacheDrain;
extern int cacheDiskType;
extern afs_uint32 afs_tpct1, afs_tpct2, splitdcache;
extern unsigned char *afs_indexFlags;
extern struct afs_cacheOps *afs_cacheType;
extern afs_dcache_id_t cacheInode;
extern struct osi_file *afs_cacheInodep;
extern void afs_dcacheInit(int afiles, int ablocks, int aDentries, int achunk,
			   int aflags);
extern int afs_PutDCache(struct dcache *adc);
extern void afs_FlushDCache(struct dcache *adc);
extern void shutdown_dcache(void);
extern void afs_CacheTruncateDaemon(void);
extern afs_int32 afs_fsfragsize;
extern struct dcache *afs_MemGetDSlot(afs_int32 aslot, dslot_state type);
extern struct dcache *afs_GetDCache(struct vcache *avc,
				    afs_size_t abyte,
				    struct vrequest *areq,
				    afs_size_t * aoffset, afs_size_t * alen,
				    int aflags);
extern struct dcache *afs_FindDCache(struct vcache *avc,
				     afs_size_t abyte);
extern void afs_StoreWarn(afs_int32 acode, afs_int32 avolume,
			  afs_int32 aflags);
extern void afs_MaybeWakeupTruncateDaemon(void);
extern void afs_CacheTruncateDaemon(void);
extern void afs_AdjustSize(struct dcache *adc,
			   afs_int32 newSize);
extern int afs_HashOutDCache(struct dcache *adc, int zap);
extern int afs_MaybeFreeDiscardedDCache(void);
extern int afs_RefDCache(struct dcache *adc);
extern void afs_TryToSmush(struct vcache *avc,
			   afs_ucred_t *acred, int sync);
extern void updateV2DC(int lockVc, struct vcache *v, struct dcache *d,
		       int src);
extern void afs_WriteThroughDSlots(void);
extern struct dcache *afs_UFSGetDSlot(afs_int32 aslot, dslot_state type);
extern int afs_WriteDCache(struct dcache *adc, int atime);
extern int afs_wakeup(struct vcache *avc);
extern int afs_InitCacheFile(char *afile, ino_t ainode);
extern int afs_DCacheMissingChunks(struct vcache *avc);
extern struct dcache *afs_ObtainDCacheForWriting(struct vcache *avc,
						 afs_size_t filePos,
						 afs_size_t len,
						 struct vrequest *areq,
						 int noLock);
extern void afs_PopulateDCache(struct vcache *avc, afs_size_t apos,
			       struct vrequest *areq);

/* afs_disconnected.c */

/* afs_dynroot.c */
extern afs_rwlock_t afs_dynrootDirLock;
extern afs_rwlock_t afs_dynSymlinkLock;
extern int afs_IsDynrootFid(struct VenusFid *fid);
extern int afs_IsDynrootMountFid(struct VenusFid *fid);
extern int afs_IsDynrootAnyFid(struct VenusFid *fid);
extern void afs_GetDynrootFid(struct VenusFid *fid);
extern void afs_GetDynrootMountFid(struct VenusFid *fid);
extern int afs_IsDynroot(struct vcache *avc);
extern int afs_IsDynrootMount(struct vcache *avc);
extern int afs_IsDynrootAny(struct vcache *avc);
extern void afs_DynrootInvalidate(void);
extern void afs_GetDynroot(char **dynrootDir, int *dynrootLen,
			   struct AFSFetchStatus *status);
extern void afs_GetDynrootMount(char **dynrootDir, int *dynrootLen,
			   struct AFSFetchStatus *status);
extern void afs_PutDynroot(void);
extern int afs_DynrootNewVnode(struct vcache *avc,
			       struct AFSFetchStatus *status);
extern int afs_InitDynroot(void);
extern int afs_SetDynrootEnable(int enable);
extern int afs_GetDynrootEnable(void);
extern int afs_DynrootVOPRemove(struct vcache *avc, afs_ucred_t *acred,
				char *aname);
extern int afs_DynrootVOPSymlink(struct vcache *avc, afs_ucred_t *acred,
				 char *aname, char *atargetName);

/* afs_error.c */
extern void init_et_to_sys_error(void);
extern afs_int32 et_to_sys_error(afs_int32 in);
extern void afs_FinalizeReq(struct vrequest *areq);
extern int afs_CheckCode(afs_int32 acode, struct vrequest *areq, int where);
extern void afs_CopyError(struct vrequest *afrom,
			  struct vrequest *ato);
extern void init_sys_error_to_et(void);

/* afs_exporter.c */
extern struct afs_exporter *root_exported;
extern struct afs_exporter *exporter_find(int type);
extern void shutdown_exporter(void);

/* afs_fetchstore.c */
extern int afs_UFSCacheFetchProc(struct rx_call *, struct osi_file *,
                                 afs_size_t, struct dcache *,
                                 struct vcache *, afs_size_t *,
                                 afs_size_t *, afs_int32);

extern int afs_UFSCacheStoreProc(struct rx_call *, struct osi_file *,
                                 afs_int32, struct vcache *,
                                 int *, afs_size_t *,
                                 afs_size_t *);

/* afs_icl.c */
extern struct afs_icl_set *afs_icl_allSets;
extern int afs_icl_InitLogs(void);
extern void shutdown_icl(void);
extern int afs_icl_CreateLog(char *name, afs_int32 logSize,
			     struct afs_icl_log **outLogpp);
extern int afs_icl_CreateLogWithFlags(char *name, afs_int32 logSize,
				      afs_uint32 flags,
				      struct afs_icl_log **outLogpp);
extern int afs_icl_CopyOut(struct afs_icl_log *logp,
			   afs_int32 * bufferp, afs_int32 * bufSizep,
			   afs_uint32 * cookiep, afs_int32 * flagsp);
extern int afs_icl_GetLogParms(struct afs_icl_log *logp, afs_int32 * maxSizep,
			       afs_int32 * curSizep);
extern int afs_icl_LogHold(struct afs_icl_log *logp);
extern int afs_icl_LogHoldNL(struct afs_icl_log *logp);
extern int afs_icl_LogUse(struct afs_icl_log *logp);
extern int afs_icl_LogFreeUse(struct afs_icl_log *logp);
extern int afs_icl_LogSetSize(struct afs_icl_log *logp,
			      afs_int32 logSize);
extern int afs_icl_ZapLog(struct afs_icl_log *logp);
extern int afs_icl_LogRele(struct afs_icl_log *logp);
extern int afs_icl_LogReleNL(struct afs_icl_log *logp);
extern int afs_icl_ZeroLog(struct afs_icl_log *logp);
extern int afs_icl_LogFree(struct afs_icl_log *logp);
extern struct afs_icl_log *afs_icl_FindLog(char *name);
extern int
  afs_icl_EnumerateLogs(int (*aproc)

			  (char *name, char *arock, struct afs_icl_log * tp),
			char *arock);
extern int afs_icl_CreateSet(char *name, struct afs_icl_log *baseLogp,
			     struct afs_icl_log *fatalLogp,
			     struct afs_icl_set **outSetpp);
extern int afs_icl_CreateSetWithFlags(char *name,
				      struct afs_icl_log *baseLogp,
				      struct afs_icl_log *fatalLogp,
				      afs_uint32 flags,
				      struct afs_icl_set **outSetpp);
extern int afs_icl_SetEnable(struct afs_icl_set *setp, afs_int32 eventID,
			     int setValue);
extern int afs_icl_GetEnable(struct afs_icl_set *setp, afs_int32 eventID,
			     int *getValuep);
extern int afs_icl_ZeroSet(struct afs_icl_set *setp);
extern int
  afs_icl_EnumerateSets(int (*aproc)

			  (char *name, char *arock, struct afs_icl_log * tp),
			char *arock);
extern int afs_icl_AddLogToSet(struct afs_icl_set *setp,
			       struct afs_icl_log *newlogp);
extern int afs_icl_SetSetStat(struct afs_icl_set *setp, int op);
extern int afs_icl_SetHold(struct afs_icl_set *setp);
extern int afs_icl_ZapSet(struct afs_icl_set *setp);
extern int afs_icl_SetRele(struct afs_icl_set *setp);
extern int afs_icl_SetFree(struct afs_icl_set *setp);
extern struct afs_icl_set *afs_icl_FindSet(char *name);

extern int afs_icl_Event4(struct afs_icl_set *setp,
			  afs_int32 eventID, afs_int32 lAndT, long p1,
			  long p2, long p3, long p4);
extern int afs_icl_Event3(struct afs_icl_set *setp,
			  afs_int32 eventID, afs_int32 lAndT, long p1,
			  long p2, long p3);
extern int afs_icl_Event2(struct afs_icl_set *setp,
			  afs_int32 eventID, afs_int32 lAndT, long p1,
			  long p2);
extern int afs_icl_Event1(struct afs_icl_set *setp,
			  afs_int32 eventID, afs_int32 lAndT, long p1);
extern int afs_icl_Event0(struct afs_icl_set *setp,
			  afs_int32 eventID, afs_int32 lAndT);
extern void afs_icl_AppendRecord(struct afs_icl_log *logp,
				 afs_int32 op, afs_int32 types, long p1,
				 long p2, long p3, long p4);
#if defined(AFS_NBSD_ENV)
extern int Afscall_icl(long opcode, long p1, long p2, long p3, long p4,
		       register_t *retval);
#else
extern int Afscall_icl(long opcode, long p1, long p2, long p3, long p4,
		       long *retval);
#endif
#ifdef AFS_DARWIN100_ENV
extern int Afscall64_icl(int opcode, user_addr_t p1, user_addr_t p2,
		       user_addr_t p3, user_addr_t p4, int *retval);
#endif

/* afs_init.c */
extern struct cm_initparams cm_initParams;
extern int afs_resourceinit_flag;
extern afs_rwlock_t afs_puttofileLock;
extern char *afs_sysname;
extern char *afs_sysnamelist[MAXNUMSYSNAMES];
extern int afs_sysnamecount;
extern int afs_sysnamegen;
extern afs_int32 cacheInfoModTime;
extern int afs_CacheInit(afs_int32 astatSize, afs_int32 afiles,
			 afs_int32 ablocks, afs_int32 aDentries,
			 afs_int32 aVolumes, afs_int32 achunk,
			 afs_int32 aflags, afs_int32 ninodes,
			 afs_int32 nusers, afs_int32 dynamic_vcaches);
extern void afs_ComputeCacheParms(void);
extern void afs_InitFHeader(struct afs_fheader *aheader);
extern int afs_InitCacheInfo(char *afile);
extern int afs_InitVolumeInfo(char *afile);
extern int afs_InitCellInfo(char *afile);
extern int afs_ResourceInit(int preallocs);
extern void shutdown_cache(void);
extern void shutdown_vnodeops(void);
extern void shutdown_AFS(void);
extern int afs_LookupInodeByPath(char *filename, afs_ufs_dcache_id_t *inode,
				 struct vnode **fvpp);

/* afs_lock.c */
extern void Lock_Init(struct afs_lock *lock);
extern void ObtainLock(struct afs_lock *lock, int how,
		       unsigned int src_indicator);
extern void ReleaseLock(struct afs_lock *lock, int how);
extern int Afs_Lock_Trace(int op, struct afs_lock *alock, int type,
			  char *file, int line);
extern void Afs_Lock_Obtain(struct afs_lock *lock, int how);
extern void Afs_Lock_ReleaseR(struct afs_lock *lock);
extern void Afs_Lock_ReleaseW(struct afs_lock *lock);
extern void afs_osi_SleepR(char *addr,
			   struct afs_lock *alock);
extern void afs_osi_SleepW(char *addr,
			   struct afs_lock *alock);
extern void afs_osi_SleepS(char *addr,
			   struct afs_lock *alock);




/* afs_mariner.c */
extern afs_int32 afs_mariner;
extern afs_int32 afs_marinerHost;
extern struct rx_service *afs_server;
extern int afs_AddMarinerName(char *aname,
			      struct vcache *avc);
extern char *afs_GetMariner(struct vcache *avc);
extern void afs_MarinerLogFetch(struct vcache *avc,
				afs_int32 off,
				afs_int32 bytes,
				afs_int32 idx);
extern void afs_MarinerLog(char *astring,
			   struct vcache *avc);
extern void shutdown_mariner(void);

/* afs_fetchstore.c */
extern int afs_CacheStoreVCache(struct dcache **dcList, struct vcache *avc,
				struct vrequest *areq,
				int sync, unsigned int minj,
				unsigned int high, unsigned int moredata,
				afs_hyper_t *anewDV,
                                 afs_size_t *amaxStoredLength);
extern int afs_CacheFetchProc(struct afs_conn *tc, struct rx_connection *rxconn,
                                struct osi_file *fP,
				afs_size_t abase, struct dcache *adc,
				struct vcache *avc, afs_int32 size,
				struct afs_FetchOutput *tsmall)
				AFS_NONNULL((5));

/* afs_memcache.c */
extern int afs_InitMemCache(int blkCount, int blkSize, int flags);
extern int afs_MemCacheClose(struct osi_file *file);
extern void *afs_MemCacheOpen(afs_dcache_id_t *ainode);
extern int afs_MemReadBlk(struct osi_file *fP, int offset,
			  void *dest, int size);
extern int afs_MemReadvBlk(struct memCacheEntry *mceP, int offset,
			   struct iovec *iov, int nio, int size);
extern int afs_MemReadUIO(afs_dcache_id_t *ainode, struct uio *uioP);
extern int afs_MemWriteBlk(struct osi_file *fP, int offset,
			   void *src, int size);
extern int afs_MemWritevBlk(struct memCacheEntry *mceP, int offset,
			    struct iovec *iov, int nio, int size);
extern int afs_MemWriteUIO(struct vcache *, afs_dcache_id_t *, struct uio *);
extern int afs_MemCacheTruncate(struct osi_file *fP,
				int size);
extern void shutdown_memcache(void);


/* afs_nfsclnt.c */
extern struct afs_exporter *afs_nfsexported;
extern struct afs_exporter *afs_nfsexporter;
extern void afs_nfsclient_init(void);
extern int afs_nfsclient_reqhandler(struct afs_exporter *exporter,
				    afs_ucred_t **cred,
				    afs_uint32 host, afs_int32 *pagparam,
				    struct afs_exporter **outexporter);
extern void shutdown_nfsclnt(void);

/* afs_osi.c */
extern afs_lock_t afs_ftf;
extern void afs_osi_Invisible(void);
extern void afs_osi_Visible(void);
extern void afs_osi_RxkRegister(void);
extern void afs_osi_MaskSignals(void);
extern void afs_osi_UnmaskRxkSignals(void);
extern void afs_osi_MaskUserLoop(void);
extern void osi_Init(void);
extern void afs_osi_MaskSignals(void);
extern void afs_osi_UnmaskRxkSignals(void);
extern void afs_osi_RxkRegister(void);
extern void afs_osi_Invisible(void);
extern void shutdown_osi(void);
extern void shutdown_osinet(void);
extern void shutdown_osisleep(void);
extern int afs_osi_suser(void *credp);
extern void afs_osi_TraverseProcTable(void);
#if defined(KERNEL) && !defined(UKERNEL)
extern const afs_ucred_t *afs_osi_proc2cred(afs_proc_t * pr);
#endif

/* afs_osi_alloc.c */
#ifndef AFS_PRIVATE_OSI_ALLOCSPACES
extern afs_lock_t osi_fsplock;
extern afs_lock_t osi_flplock;
#endif

extern void *afs_osi_Alloc_debug(size_t x, char *func, int line);
#ifndef afs_osi_Free
extern void afs_osi_Free(void *x, size_t asize);
#endif
#if !defined(AFS_OBSD44_ENV)
extern void afs_osi_FreeStr(char *x);
#endif
extern void osi_FreeLargeSpace(void *adata);
extern void osi_FreeSmallSpace(void *adata);
extern void *osi_AllocLargeSpace(size_t size);
extern void *osi_AllocSmallSpace(size_t size);
extern void shutdown_osinet(void);

/* afs_osi_pag.c */
#if defined(AFS_SUN5_ENV)
extern int afs_setpag(afs_ucred_t **credpp);
#elif defined(AFS_FBSD_ENV)
extern int afs_setpag(struct thread *td, void *args);
#elif defined(AFS_NBSD_ENV)
extern int afs_setpag(afs_proc_t *p, const void *args, register_t *retval);
#elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
extern int afs_setpag(afs_proc_t *p, void *args, int *retval);
#else
extern int afs_setpag(void);
#endif

extern afs_uint32 genpag(void);
extern afs_uint32 getpag(void);
#if defined(AFS_FBSD_ENV)
extern int AddPag(struct thread *td, afs_int32 aval, afs_ucred_t **credpp);
#elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
extern int AddPag(afs_proc_t *p, afs_int32 aval, afs_ucred_t **credpp);
#else
extern int AddPag(afs_int32 aval, afs_ucred_t **credpp);
#endif
extern int afs_InitReq(struct vrequest *av, afs_ucred_t *acred);
extern int afs_CreateReq(struct vrequest **avpp, afs_ucred_t *acred);
extern void afs_DestroyReq(struct vrequest *av);
extern afs_uint32 afs_get_pag_from_groups(gid_t g0a, gid_t g1a);
extern void afs_get_groups_from_pag(afs_uint32 pag, gid_t * g0p, gid_t * g1p);
extern afs_int32 PagInCred(afs_ucred_t *cred);

/* afs_osi_uio.c */
#if !defined(AFS_DARWIN80_ENV)
extern int afsio_copy(struct uio *ainuio, struct uio *aoutuio,
		      struct iovec *aoutvec);
extern int afsio_trim(struct uio *auio, afs_int32 asize);
extern void afsio_free(struct uio *auio);
#endif

extern struct uio* afsio_partialcopy(struct uio *auio, size_t size);
extern int afsio_skip(struct uio *auio, afs_int32 asize);

/* afs_osi_vm.c */
extern int osi_Active(struct vcache *avc);
extern void osi_FlushPages(struct vcache *avc,
			   afs_ucred_t *credp);
extern void osi_FlushText_really(struct vcache *vp);
extern int osi_VMDirty_p(struct vcache *avc);
#ifndef UKERNEL
extern void osi_ReleaseVM(struct vcache *avc, afs_ucred_t *acred);
#endif


/* LINUX/osi_fetchstore.c */
#ifdef AFS_LINUX26_ENV
extern int afs_linux_storeproc(struct storeOps *, void *, struct dcache *,
			       int *, afs_size_t *);
#endif

/* ARCH/osi_crypto.c */
extern int osi_readRandom(void *, afs_size_t);

/* ARCH/osi_misc.c */
extern void afs_osi_SetTime(osi_timeval_t * atv);

/* LINUX/osi_misc.c */
#ifdef AFS_LINUX20_ENV
#ifdef AFS_LINUX24_ENV
extern int osi_lookupname(char *aname, uio_seg_t seg, int followlink,
			  struct dentry **dpp);
extern int osi_InitCacheInfo(char *aname);
extern int osi_rdwr(struct osi_file *osifile, struct uio *uiop, int rw);
extern void setup_uio(struct uio *uiop, struct iovec *iovecp, const char *buf,
		      afs_offs_t pos, int count, uio_flag_t flag,
		      uio_seg_t seg);
extern int uiomove(char *dp, int length, uio_flag_t rw, struct uio *uiop);
extern void osi_linux_free_inode_pages(void);
#endif
extern void osi_linux_mask(void);
extern void osi_linux_unmask(void);
extern int setpag(cred_t ** cr, afs_uint32 pagvalue, afs_uint32 * newpag,
		  int change_parent);
#endif


/* ARCH/osi_sleep.c */
extern void afs_osi_InitWaitHandle(struct afs_osi_WaitHandle *achandle);
extern void afs_osi_CancelWait(struct afs_osi_WaitHandle *achandle);
extern int afs_osi_Wait(afs_int32 ams, struct afs_osi_WaitHandle *ahandle,
			int aintok);
extern int afs_osi_TimedSleep(void *event, afs_int32 ams, int aintok);
#ifndef afs_osi_Wakeup
extern int afs_osi_Wakeup(void *event);
#endif
#ifndef afs_osi_Sleep
extern void afs_osi_Sleep(void *event);
#endif
#ifndef afs_osi_SleepSig
extern int afs_osi_SleepSig(void *event);
#endif


/* ARCH/osi_inode.c */
#ifdef AFS_SUN5_ENV
extern int afs_syscall_icreate(dev_t, long, long, long, long, long,
		               rval_t *, afs_ucred_t *);
extern int afs_syscall_iopen(dev_t, int, int, rval_t *, afs_ucred_t *);
extern int afs_syscall_iincdec(dev_t, int, int, int, rval_t *,
			       afs_ucred_t *);
#elif defined(AFS_SGI65_ENV)
extern int afs_syscall_icreate(afs_uint32, afs_uint32, afs_uint32, afs_uint32, afs_uint32, afs_uint32, rval_t *);
extern int afs_syscall_iopen(int, ino_t, int, rval_t *);
extern int afs_syscall_iincdec(int, int, int, int);
#elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
#if defined(AFS_NBSD_ENV)
extern int afs_syscall_icreate(long, long, long, long, long, long, register_t *);
extern int afs_syscall_iopen(int dev, int inode, int usrmod, register_t *);
#else
extern int afs_syscall_icreate(long, long, long, long, long, long, long*);
extern int afs_syscall_iopen(int dev, int inode, int usrmod, long *retval);
#endif
extern int afs_syscall_iincdec(int dev, int inode, int inode_p1, int amount);
#else
extern int afs_syscall_icreate(long, long, long, long, long, long);
extern int afs_syscall_iopen(int, int, int);
extern int afs_syscall_iincdec(int, int, int, int);
#endif

/* ARCH/osi_file.c */
extern int afs_osicred_initialized;
extern void *osi_UFSOpen(afs_dcache_id_t *ainode);
#if defined(AFS_LINUX22_ENV)
extern void osi_get_fh(struct dentry *dp, afs_ufs_dcache_id_t *ainode);
#endif
extern int afs_osi_Stat(struct osi_file *afile,
			struct osi_stat *astat);
extern int osi_UFSClose(struct osi_file *afile);
extern int osi_UFSTruncate(struct osi_file *afile, afs_int32 asize);
extern void osi_DisableAtimes(struct vnode *avp);
extern int afs_osi_Read(struct osi_file *afile, int offset,
			void *aptr, afs_int32 asize);
extern int afs_osi_Write(struct osi_file *afile, afs_int32 offset,
			 void *aptr, afs_int32 asize);
extern int afs_osi_MapStrategy(int (*aproc) (struct buf * bp),
			       struct buf *bp);
extern void shutdown_osifile(void);


/* ARCH/osi_groups.c */
#if defined(UKERNEL)
extern int usr_setpag(afs_ucred_t **cred, afs_uint32 pagvalue,
		      afs_uint32 * newpag, int change_parent);
#else
# if defined AFS_XBSD_ENV
#  if !defined(AFS_DFBSD_ENV)
#   if defined(AFS_FBSD_ENV)
extern int setpag(struct thread *td, struct ucred **cred, afs_uint32 pagvalue,
		  afs_uint32 * newpag, int change_parent);

#   elif defined(AFS_NBSD_ENV)
extern int setpag(afs_proc_t *proc, afs_ucred_t **cred, afs_uint32 pagvalue,
		  afs_uint32 * newpag, int change_parent);
#   else
extern int setpag(afs_proc_t *proc, struct ucred **cred, afs_uint32 pagvalue,
		  afs_uint32 * newpag, int change_parent);
#   endif /* AFS_FBSD_ENV */
#  endif /* ! AFS_DFBSD_ENV */
# endif /* AFS_XBSD_ENV */
#endif /* UKERNEL */

#if defined(AFS_LINUX26_ENV)
extern afs_int32 osi_get_group_pag(afs_ucred_t *cred);
#endif



/* ARCH/osi_vm.c */
extern int osi_VM_FlushVCache(struct vcache *avc);
extern void osi_VM_StoreAllSegments(struct vcache *avc);
extern void osi_VM_TryToSmush(struct vcache *avc, afs_ucred_t *acred,
			      int sync);
extern void osi_VM_FlushPages(struct vcache *avc, afs_ucred_t *credp);
#if !defined(AFS_NBSD_ENV)
extern void osi_VM_Truncate(struct vcache *avc, int alen, afs_ucred_t *acred);
#else
extern void osi_VM_Truncate(struct vcache *avc, voff_t alen, afs_ucred_t *acred);
#endif
extern void osi_VM_TryReclaim(struct vcache *avc, int *slept);
extern void osi_VM_NukePages(struct vnode *vp, off_t offset, off_t size);
extern int osi_VM_Setup(struct vcache *avc, int force);

#ifdef AFS_SUN5_ENV
extern int osi_VM_GetDownD(struct vcache *avc, struct dcache *adc);
extern int osi_VM_MultiPageConflict(struct vcache *avc, struct dcache *adc);
extern void osi_VM_PreTruncate(struct vcache *avc, int alen,
			       afs_ucred_t *acred);
#endif


/* ARCH/osi_vnodeops.c */
extern struct vnodeops Afs_vnodeops;
extern int afs_inactive(struct vcache *avc, afs_ucred_t *acred);

/* afs_osifile.c */

/* afs_osi_pag.c */
extern afs_uint32 pag_epoch;
extern afs_uint32 pagCounter;

/* OS/osi_vfsops.c */
#if defined(AFS_XBSD_ENV) || defined(AFS_DARWIN_ENV)
extern struct mount *afs_globalVFS;
#else
extern struct vfs *afs_globalVFS;
#endif
extern struct vcache *afs_globalVp;
#ifdef AFS_LINUX20_ENV
extern void vcache2inode(struct vcache *avc);
extern void vcache2fakeinode(struct vcache *rootvp, struct vcache *mpvp);
#endif

#ifdef UKERNEL
extern int afs_mount(struct vfs *afsp, char *path, void *data);
extern int afs_root(OSI_VFS_DECL(afsp), struct vnode **avpp);
extern int afs_unmount(struct vfs *afsp);
#endif

/* afs_pag_call.c */
extern afs_int32 afs_nfs_server_addr;
extern void afspag_Init(afs_int32 nfs_server_addr);
extern void afspag_Shutdown(void);

/* afs_pag_cred.c */
extern afs_rwlock_t afs_xpagcell;
extern afs_rwlock_t afs_xpagsys;
extern int afspag_PUnlog(char *ain, afs_int32 ainSize,
			 afs_ucred_t **acred);
extern int afspag_PSetTokens(char *ain, afs_int32 ainSize,
			     afs_ucred_t **acred);
extern int afspag_PSetSysName(char *ain, afs_int32 ainSize,
                              afs_ucred_t **acred);

/* afs_pioctl.c */
extern struct VenusFid afs_rootFid;
extern afs_int32 afs_waitForever;
extern short afs_waitForeverCount;
extern afs_int32 afs_showflags;
extern int afs_defaultAsynchrony;
#if defined(AFS_DARWIN100_ENV)
extern int afs_syscall64_pioctl(user_addr_t path, unsigned int com,
				user_addr_t cmarg, int follow, \
				afs_ucred_t *credp);
#endif
#ifdef AFS_SUN5_ENV
extern int afs_syscall_pioctl(char *path, unsigned int com, caddr_t cmarg,
			      int follow, rval_t *rvp, afs_ucred_t *credp);
#elif defined(AFS_DARWIN_ENV) || defined(AFS_XBSD_ENV)
extern int afs_syscall_pioctl(char *path, unsigned int com, caddr_t cmarg,
			      int follow, afs_ucred_t *credp);
#else
extern int afs_syscall_pioctl(char *path, unsigned int com, caddr_t cmarg,
			      int follow);
#endif
extern int HandleIoctl(struct vcache *avc, afs_int32 acom,
		       struct afs_ioctl *adata);


/* afs_segments.c */
extern int afs_StoreAllSegments(struct vcache *avc,
				struct vrequest *areq, int sync);
extern int afs_InvalidateAllSegments(struct vcache *avc);
extern int afs_ExtendSegments(struct vcache *avc,
			      afs_size_t alen, struct vrequest *areq);
extern int afs_TruncateAllSegments(struct vcache *avc,
				   afs_size_t alen, struct vrequest *areq,
				   afs_ucred_t *acred);

/* afs_server.c */
extern afs_rwlock_t afs_xsrvAddr;
extern afs_rwlock_t afs_xserver;
extern afs_rwlock_t afs_icl_lock;
extern struct srvAddr *afs_srvAddrs[NSERVERS];
extern struct server *afs_servers[NSERVERS];
extern int afs_to