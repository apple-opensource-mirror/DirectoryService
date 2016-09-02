/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*!
 * @header CNiPlugIn
 */

#ifndef __CNiPlugIn_h__
#define __CNiPlugIn_h__		1

#include <netinfo/ni.h>

#include "DirServicesTypes.h"
#include "DirServicesConst.h"
#include "PrivateTypes.h"
#include "CSharedData.h"
#include "PluginData.h"
#include "DSMutexSemaphore.h"

class	CString;

extern DSMutexSemaphore		*gNetInfoMutex;


// -- enum's -------------------------------------

enum eNipThreadSig {
	kNiPlugInThreadSignatrue	=	'NiPi'
};

enum eBuffType {
	kRecrodListType		=	'RecL'
};


extern uInt32		gNodeRefID;

enum {
	kAuthUnknowMethod		= 1220,
	kAuthClearText			= 1221,
	kAuthCrypt				= 1222,
	kAuthSetPasswd			= 1323,
	kAuthSetPasswdAsRoot	= 1224,
	kAuthChangePasswd		= 1225,
	kAuthAPOP				= 1226,
	kAuth2WayRandom			= 1227,
	kAuthNativeClearTextOK	= 1228,
	kAuthNativeNoClearText	= 1229,
	kAuthSMB_NT_Key			= 1230,
	kAuthSMB_LM_Key			= 1231,
	kAuthNativeMethod		= 1232,
	kAuthCRAM_MD5			= 1233
};

typedef struct {
	void	   *fDomain;
	char	   *fDomainName;
	char	   *fRecType;
	char	   *fRecName;
	ni_id		dirID;
	uInt32		offset;
	uInt32		index;
	bool		fDontUseSafeClose;
	uid_t		fUID;
	uid_t		fEffectiveUID;
	char*		fAuthenticatedUserName;
    
    tDirReference fPWSRef;
    tDirNodeReference fPWSNodeRef;
} sNIContextData;

typedef struct {
	uInt32				fAuthPass;
	uInt32				fLimitRecSearch;
	uInt32				fMultiMapIndex;
	uInt32				fRecNameIndex;
	uInt32				fRecTypeIndex;
	uInt32				fAllRecIndex;
	uInt32				fTotalRecCount;
	uInt32				fAttrIndex;
	tDataList		   *fAliasList;
	tDataList		   *fAliasAttribute;
	tDataBuffer		   *fDataBuff;
	void			   *fAuthHndl;
	void			   *fAuthHandlerProc;
	char			   *fAuthAuthorityData;
    tContextData		fPassPlugContinueData;
} sNIContinueData;

typedef sInt32 (*AuthAuthorityHandlerProc) (tDirNodeReference inNodeRef,
											tDataNodePtr inAuthMethod,
											sNIContextData* inContext,
											sNIContinueData** inOutContinueData,
											tDataBufferPtr inAuthData,
											tDataBufferPtr outAuthData,
											bool inAuthOnly,
											char* inAuthAuthorityData );

class	CBuff;
class	CAttributeList;
class	CDataBuff;
class	CNodeRef;


class CNiPlugIn {
public:
				CNiPlugIn					( void );
	virtual	   ~CNiPlugIn					( void );

	sInt32		HandleRequest				( void *inData );
	static void	ContinueDeallocProc			( void *inContinueData );
	static void	ContextDeallocProc			( void* inContextData );
	static sInt32 DoBasicAuth				( tDirNodeReference inNodeRef,
											  tDataNodePtr inAuthMethod, 
											  sNIContextData* inContext, 
											  sNIContinueData** inOutContinueData, 
											  tDataBufferPtr inAuthData, 
											  tDataBufferPtr outAuthData, 
											  bool inAuthOnly,
											  char* inAuthAuthorityData );
	static sInt32 DoLocalWindowsAuth		( tDirNodeReference inNodeRef,
											  tDataNodePtr inAuthMethod, 
											  sNIContextData* inContext, 
											  sNIContinueData** inOutContinueData, 
											  tDataBufferPtr inAuthData, 
											  tDataBufferPtr outAuthData, 
											  bool inAuthOnly, 
											  char* inAuthAuthorityData );	
	static sInt32 DoPasswordServerAuth		( tDirNodeReference inNodeRef,
											  tDataNodePtr inAuthMethod, 
											  sNIContextData* inContext, 
											  sNIContinueData** inOutContinueData, 
											  tDataBufferPtr inAuthData, 
											  tDataBufferPtr outAuthData, 
											  bool inAuthOnly,
											  char* inAuthAuthorityData );	

protected:
	sInt32		OpenDirNode					( sOpenDirNode *inData );
	sInt32		CloseDirNode				( sCloseDirNode *inData );

	sInt32		GetDirNodeInfo				( sGetDirNodeInfo *inData );
	sInt32		GetAttributeEntry			( sGetAttributeEntry *inData );
	sInt32		GetRecordList				( sGetRecordList *inData );
	sInt32		GetRecordEntry				( sGetRecordEntry *inData );
	sInt32		CreateRecord				( sCreateRecord *inData );
	sInt32		OpenRecord					( sOpenRecord *inData );
	sInt32		GetRecRefInfo				( sGetRecRefInfo *inData );
	sInt32		CloseRecord					( sCloseRecord *inData );
	sInt32		SetRecordName				( sSetRecordName *inData );
	sInt32		SetRecordType				( sSetRecordType *inData );
	sInt32		DeleteRecord				( sDeleteRecord *inData );
	sInt32		AddAttribute				( sAddAttribute *inData );
	sInt32		GetAttributeValue			( sGetAttributeValue *inData );
	sInt32		GetRecAttribInfo			( sGetRecAttribInfo *inData );
	sInt32		GetRecordAttributeValueByID	( sGetRecordAttributeValueByID *inData );
	sInt32		GetRecAttrValueByIndex		( sGetRecordAttributeValueByIndex *inData );
	sInt32		DoAuthentication			( sDoDirNodeAuth *inData );
	sInt32		DoAttributeValueSearch		( sDoAttrValueSearchWithData *inData );
	sInt32		DoPlugInCustomCall			( sDoPlugInCustomCall *inData );
	sInt32		ReleaseContinueData			( sReleaseContinueData *inData );
	sInt32		RemoveAttribute				( sRemoveAttribute *inData );
	sInt32		AddAttributeValue			( sAddAttributeValue *inData );
	sInt32		RemoveAttributeValue		( sRemoveAttributeValue *inData );
	sInt32		SetAttributeValue			( sSetAttributeValue *inData );
	sInt32		CloseAttributeList			( sCloseAttributeList *inData );
	sInt32		CloseAttributeValueList		( sCloseAttributeValueList *inData );

	sInt32		GetAllRecords				(	char *inNativeRecType,
												CAttributeList *inAttrTypeList,
												sNIContinueData *inContinue,
												void *inDomain,
												char *inDomainName,
												bool inAttrOnly,
												CBuff *inBuff,
												uInt32 &outRecCount );
	sInt32		GetTheseRecords				(	char *inConstRecName,
												char *inConstRecType,
												char *inNativeRecType,
												tDirPatternMatch inPattMatch,
												CAttributeList *inAttrTypeList,
												void *inDomain,
												char *inDomainName,
												bool inAttrOnly,
												CBuff *inBuff,
												sNIContinueData *inContinue,
												uInt32 &outRecCount );
	sInt32		GetTheseAttributes			(	CAttributeList *inAttrTypeList,
												ni_proplist *inPropList,
												bool inAttrOnly,
												void *inDomain,
												char *inDomainName,
												sInt32 &outCnt );
	sInt32		FindAllRecords				(	const char *inNI_RecType,
												const char *inDS_RecType,
												const char *inPatt2Match,
												tDirPatternMatch inHow,
												CAttributeList *inAttrTypeList,
												bool inAttrInfoOnly,
												sNIContinueData *inContext,
												void *inDomain,
												char *inDomainName,
												CBuff *inBuff,
												uInt32 &outRecCount );
	sInt32		FindTheseRecords   			(	const char *inNI_RecType,
												const char *inDS_RecType,
												const char *inAttrType,
												const char *inPatt2Match,
												tDirPatternMatch inHow,
												CAttributeList *inAttrTypeList,
												bool inAttrInfoOnly,
												sNIContinueData *inContext,
												void *inDomain,
												char *inDomainName,
												CBuff *inBuff,
												uInt32 &outRecCount );

private:
	sInt32		DoCreateRecord				( void *inDomain, ni_id *inDir, char *inPathName );
	ni_status	DoCreateChild				( void *inDomain, ni_id *inDir, const ni_name inDirName );
	sInt32		DoAddAttribute				( void *domain, ni_id *dir, const ni_name key, ni_namelist values );

	static bool		IsWriteAuthRequest		( uInt32 uiAuthMethod );
	static sInt32	ReadWindowsHash			( const char *inUserName, unsigned char outHashes[32] );
	static sInt32	WriteWindowsHash		( const char *inUserName, unsigned char inHashes[32] );
    
    static sInt32	RepackBufferForPWServer	(	tDataBufferPtr inBuff,
                                                const char *inUserID,
                                                unsigned long inUserIDNodeNum,
                                                tDataBufferPtr *outBuff );
    
    static sInt32	PWOpenDirNode			( tDirNodeReference fDSRef, char *inNodeName, tDirNodeReference *outNodeRef );
    
	static sInt32	DoSetPassword			( sNIContextData *inContext, tDataBuffer *inAuthData );
	static sInt32	DoSetPasswordAsRoot		( sNIContextData *inContext, tDataBuffer *inAuthData );
	static sInt32	DoChangePassword		( sNIContextData *inContext, tDataBuffer *inAuthData );
	static sInt32	DoNodeNativeAuth		( sNIContextData *inContext, tDataBuffer *inAuthData, bool inAuthOnly );
	static sInt32	DoUnixCryptAuth			( sNIContextData *inContext, tDataBuffer *inAuthData, bool inAuthOnly );
	static sInt32	DoTimSMBAuth			( sNIContextData *inContext, tDataBuffer *inAuthData, uInt32 inWhichOne );
	static sInt32	DoTimMultiPassAuth		( tDirNodeReference inNodeRef, tDataNodePtr inAuthMethod,
											  sNIContextData *inContext, 
											  sNIContinueData** inOutContinueData, 
											  tDataBufferPtr inAuthData, tDataBufferPtr outAuthData, 
											  bool inAuthOnly );
	static sInt32	ValidateDigest			( sNIContextData *inContext, tDataBuffer *inAuthData, uInt32 inAuthMethod );
	static sInt32	AuthOpen				( sNIContextData *inContext, const char * inUserName, 
											  const char * inPassword );
	sInt32		VerifyPatternMatch			( const tDirPatternMatch inPatternMatch );

	static sInt32	IsValidRecordName		( const char *inRecName, const char *inRecType, 
											  void *inDomain, ni_id &outDirID );
	static sInt32	IsValidRealname			( char *inRealname, void *inDomain, char **outRecName );
	static bool		UserIsAdmin				( const char *inUserName, void *inDomain );
	static char*	GetUserNameForUID 		( uid_t inUserID, void *inDomain );
	static sInt32	GetUserNameFromAuthBuffer	( tDataBufferPtr inAuthData, unsigned long inUserNameIndex, char **outUserName );
	static sInt32	ParseAuthAuthority		( const char * inAuthAuthority, char **outVersion,
											  char **outAuthTag, char **outAuthData );
	AuthAuthorityHandlerProc GetAuthAuthorityHandler ( const char* inTag );
	static char*	BuildRecordNamePath		( const char *inRecName, const char *inRecType );
	static sInt32	GetAuthMethod			( tDataNode *inData, uInt32 *outAuthMethod );

	char*		MapRecToNetInfoType			( const char *inRecType );
	char*		MapNetInfoRecToDSType		( const char *inRecType );
	char*		MapAttrToNetInfoType		( const char *inAttrType );
	static char*	MapNetInfoAttrToDSType	( const char *inAttrType );

	static CString*	GetAuthTypeStr			( const char *inNativeAuthStr );
	static CString*	GetAuthString			( tDataNode *inData );

	static bool		IsTimRunning			( void );
	static sInt32	MapNetInfoErrors		( sInt32 inNiError );
	static sInt32	MapAuthResult			( sInt32 inAuthResult );
	uInt32		CalcCRC						( const char *inStr );

	static bool		DoesThisMatch			( const char *inString, const char *inPatt, tDirPatternMatch inHow );
	static char*	BuildRegExp				( const char *inString );

	void		MakeGood					( char *inStr, char *outStr );

	sNIContextData*	MakeContextData			( void );
    static sInt32	CleanContextData		( sNIContextData *inContext );
	static char* 	BuildDomainPathFromName	( char* inDomainName );

private:
	//KW have been extremely lucky up until now using these member vars in thread capable class
	//ie. the netinfo mutex has helped us do this in all but one function ie. GetRecRefInfo fixed now
	CDataBuff	   *fRecData;
	CDataBuff	   *fAttrData;
	CDataBuff	   *fTmpData;

};

#endif
