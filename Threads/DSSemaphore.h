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
 * @header DSSemaphore
 * Interface for the DSSemaphore (lock) base class.
 */

#ifndef _DSSemaphore_H_
#define _DSSemaphore_H_

#include <unistd.h>		// for _POSIX_THREADS
#include <pthread.h>	// for pthread_*_t

#include "PrivateTypes.h"

//	DSSemaphore class definition

class DSSemaphore
{
public:
	/**** Typedefs, enums and constants. ****/
	enum eWaitTime {
		kForever = -1,
		kNever = 0
	};
	enum eErr {
		semNoErr = 0,
		semDestroyedErr = 28020,
		semTimedOutErr,
		semNotOwnerErr,
		semAlreadyResetErr,
		semOtherErr
	};

	/**** Instance method protoypes. ****/
	// ctor and dtor.
			DSSemaphore			( sInt32 initialCount = 0 );
	virtual	~DSSemaphore			( void );

	// New methods.
	virtual void		Signal	( void );
	virtual sInt32		Wait	( sInt32 milliSecs = kForever );

protected:
	/**** Instance variables. ****/
	pthread_mutex_t		mConditionLock;
	pthread_cond_t		mSemaphore;
	volatile sInt32		mExcessSignals;
	bool				mDestroying;

private:
	/**** Invalid methods and undefined operations. ****/
	// Copy constructor
							DSSemaphore	( const DSSemaphore & );
	// Assignment
			DSSemaphore &	operator=	( const DSSemaphore & );
} ;

#endif	/* _DSSemaphore_H_ */