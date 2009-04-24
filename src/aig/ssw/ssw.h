/**CFile****************************************************************

  FileName    [ssw.h] 

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Inductive prover with constraints.]

  Synopsis    [External declarations.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - September 1, 2008.]

  Revision    [$Id: ssw.h,v 1.00 2008/09/01 00:00:00 alanmi Exp $]

***********************************************************************/

#ifndef __SSW_H__
#define __SSW_H__

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" { 
#endif

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

// choicing parameters
typedef struct Ssw_Pars_t_ Ssw_Pars_t;
struct Ssw_Pars_t_
{
    int              nPartSize;     // size of the partition
    int              nOverSize;     // size of the overlap between partitions
    int              nFramesK;      // the induction depth
    int              nFramesAddSim; // the number of additional frames to simulate
    int              nConstrs;      // treat the last nConstrs POs as seq constraints
    int              nMaxLevs;      // the max number of levels of nodes to consider
    int              nBTLimit;      // conflict limit at a node
    int              nBTLimitGlobal;// conflict limit for multiple runs
    int              nMinDomSize;   // min clock domain considered for optimization
    int              nItersStop;    // stop after the given number of iterations
    int              fDumpSRInit;   // dumps speculative reduction
    int              nResimDelta;   // the number of nodes to resimulate
    int              fPolarFlip;    // uses polarity adjustment
    int              fLatchCorr;    // perform register correspondence
    int              fSemiFormal;   // enable semiformal filtering
    int              fUniqueness;   // enable uniqueness constraints
    int              fDynamic;      // enable dynamic addition of constraints
    int              fLocalSim;     // enable local simulation simulation
    int              fPartSigCorr;  // uses partial signal correspondence
    int              nIsleDist;     // extends islands by the given distance
    int              fScorrGia;     // new signal correspondence implementation
    int              fUseCSat;      // new SAT solver using when fScorrGia is selected
    int              fVerbose;      // verbose stats
    int              fFlopVerbose;  // verbose printout of redundant flops
    // optimized latch correspondence
    int              fLatchCorrOpt; // perform register correspondence (optimized)
    int              nSatVarMax;    // max number of SAT vars before recycling SAT solver (optimized latch corr only)
    int              nRecycleCalls; // calls to perform before recycling SAT solver (optimized latch corr only)
    // optimized signal correspondence
    int              nSatVarMax2;   // max number of SAT vars before recycling SAT solver (optimized latch corr only)
    int              nRecycleCalls2;// calls to perform before recycling SAT solver (optimized latch corr only)
    // internal parameters
    int              nIters;        // the number of iterations performed
    int              nConflicts;    // the total number of conflicts performed
};

// sequential counter-example
typedef struct Ssw_Cex_t_   Ssw_Cex_t;
struct Ssw_Cex_t_
{
    int              iPo;               // the zero-based number of PO, for which verification failed
    int              iFrame;            // the zero-based number of the time-frame, for which verificaiton failed
    int              nRegs;             // the number of registers in the miter 
    int              nPis;              // the number of primary inputs in the miter
    int              nBits;             // the number of words of bit data used
    unsigned         pData[0];          // the cex bit data (the number of bits: nRegs + (iFrame+1) * nPis)
};

typedef struct Ssw_Sml_t_ Ssw_Sml_t; // sequential simulation manager

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/*=== sswBmc.c ==========================================================*/
extern int           Ssw_BmcDynamic( Aig_Man_t * pAig, int nFramesMax, int nConfLimit, int fVerbose, int * piFrame );
/*=== sswCore.c ==========================================================*/
extern void          Ssw_ManSetDefaultParams( Ssw_Pars_t * p );
extern void          Ssw_ManSetDefaultParamsLcorr( Ssw_Pars_t * p );
extern Aig_Man_t *   Ssw_SignalCorrespondence( Aig_Man_t * pAig, Ssw_Pars_t * pPars );
extern Aig_Man_t *   Ssw_LatchCorrespondence( Aig_Man_t * pAig, Ssw_Pars_t * pPars );
/*=== sswIslands.c ==========================================================*/
extern int           Ssw_SecWithSimilarityPairs( Aig_Man_t * p0, Aig_Man_t * p1, Vec_Int_t * vPairs, Ssw_Pars_t * pPars );
extern int           Ssw_SecWithSimilarity( Aig_Man_t * p0, Aig_Man_t * p1, Ssw_Pars_t * pPars );
/*=== sswMiter.c ===================================================*/
extern int           Ssw_SecSpecialMiter( Aig_Man_t * p0, Aig_Man_t * p1, int nFrames, int fVerbose );
/*=== sswPart.c ==========================================================*/
extern Aig_Man_t *   Ssw_SignalCorrespondencePart( Aig_Man_t * pAig, Ssw_Pars_t * pPars );
/*=== sswPairs.c ===================================================*/
extern int           Ssw_MiterStatus( Aig_Man_t * p, int fVerbose );
extern int           Ssw_SecWithPairs( Aig_Man_t * pAig1, Aig_Man_t * pAig2, Vec_Int_t * vIds1, Vec_Int_t * vIds2, Ssw_Pars_t * pPars );
extern int           Ssw_SecGeneral( Aig_Man_t * pAig1, Aig_Man_t * pAig2, Ssw_Pars_t * pPars );
extern int           Ssw_SecGeneralMiter( Aig_Man_t * pMiter, Ssw_Pars_t * pPars );
/*=== sswSim.c ===================================================*/
extern Ssw_Sml_t *   Ssw_SmlSimulateComb( Aig_Man_t * pAig, int nWords );
extern Ssw_Sml_t *   Ssw_SmlSimulateSeq( Aig_Man_t * pAig, int nPref, int nFrames, int nWords );
extern void          Ssw_SmlUnnormalize( Ssw_Sml_t * p );
extern void          Ssw_SmlStop( Ssw_Sml_t * p );
extern int           Ssw_SmlNumFrames( Ssw_Sml_t * p );
extern int           Ssw_SmlNumWordsTotal( Ssw_Sml_t * p );
extern unsigned *    Ssw_SmlSimInfo( Ssw_Sml_t * p, Aig_Obj_t * pObj );
extern int           Ssw_SmlObjsAreEqualWord( Ssw_Sml_t * p, Aig_Obj_t * pObj0, Aig_Obj_t * pObj1 );
extern Ssw_Cex_t *   Ssw_SmlAllocCounterExample( int nRegs, int nRealPis, int nFrames );
extern void          Ssw_SmlFreeCounterExample( Ssw_Cex_t * pCex );
extern int           Ssw_SmlRunCounterExample( Aig_Man_t * pAig, Ssw_Cex_t * p );
extern int           Ssw_SmlFindOutputCounterExample( Aig_Man_t * pAig, Ssw_Cex_t * p );
extern Ssw_Cex_t *   Ssw_SmlDupCounterExample( Ssw_Cex_t * p, int nRegsNew );

#ifdef __cplusplus
}
#endif

#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

