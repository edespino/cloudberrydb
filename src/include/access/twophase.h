/*-------------------------------------------------------------------------
 *
 * twophase.h
 *	  Two-phase-commit related declarations.
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/access/twophase.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef TWOPHASE_H
#define TWOPHASE_H

#include "access/xact.h"
#include "access/xlogdefs.h"
#include "datatype/timestamp.h"
#include "storage/lock.h"

#include "cdb/cdblocaldistribxact.h"

/*
 * GlobalTransactionData is defined in twophase.c; other places have no
 * business knowing the internal definition.
 */
typedef struct GlobalTransactionData *GlobalTransaction;

/* GUC variable */
extern PGDLLIMPORT int max_prepared_xacts;

extern Size TwoPhaseShmemSize(void);
extern void TwoPhaseShmemInit(void);

extern void AtAbort_Twophase(void);
extern void PostPrepare_Twophase(void);

extern TransactionId TwoPhaseGetXidByVirtualXID(VirtualTransactionId vxid,
												bool *have_more);
extern PGPROC *TwoPhaseGetDummyProc(TransactionId xid, bool lock_held);
extern BackendId TwoPhaseGetDummyBackendId(TransactionId xid, bool lock_held);

extern GlobalTransaction MarkAsPreparing(TransactionId xid,
										 LocalDistribXactData *localDistribXactRef,
										 const char *gid,
										 TimestampTz prepared_at,
										 Oid owner, Oid databaseid);

extern void StartPrepare(GlobalTransaction gxact);
extern void EndPrepare(GlobalTransaction gxact);
extern bool StandbyTransactionIdIsPrepared(TransactionId xid);

extern TransactionId PrescanPreparedTransactions(TransactionId **xids_p,
												 int *nxids_p);
extern void StandbyRecoverPreparedTransactions(void);
extern void RecoverPreparedTransactions(void);

extern void CheckPointTwoPhase(XLogRecPtr redo_horizon);

extern bool FinishPreparedTransaction(const char *gid, bool isCommit, bool raiseErrorIfNotFound);

extern void PrepareRedoAdd(char *buf, XLogRecPtr start_lsn,
						   XLogRecPtr end_lsn, RepOriginId origin_id);
extern void PrepareRedoRemove(TransactionId xid, bool giveWarning);
extern void restoreTwoPhaseData(void);
#endif							/* TWOPHASE_H */
