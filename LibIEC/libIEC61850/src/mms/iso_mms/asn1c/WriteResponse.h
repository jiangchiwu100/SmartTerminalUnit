﻿/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_WriteResponse_H_
#define	_WriteResponse_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "DataAccessError.h"
#include <NULL.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum WriteResponse__Member_PR {
	WriteResponse__Member_PR_NOTHING,	/* No components present */
	WriteResponse__Member_PR_failure,
	WriteResponse__Member_PR_success
} WriteResponse__Member_PR;

struct WriteResponse__Member {
		WriteResponse__Member_PR present;
		union WriteResponse__Member_u {
			DataAccessError_t	 failure;
			NULL_t	 success;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	};

/* WriteResponse */
typedef struct WriteResponse {
	A_SEQUENCE_OF(struct WriteResponse__Member) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} WriteResponse_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_WriteResponse;

#ifdef __cplusplus
}
#endif

#endif	/* _WriteResponse_H_ */
