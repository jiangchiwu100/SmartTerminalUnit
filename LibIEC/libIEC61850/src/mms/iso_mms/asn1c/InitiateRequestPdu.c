﻿/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#include <asn_internal.h>

#include "InitiateRequestPdu.h"

static asn_TYPE_member_t asn_MBR_InitiateRequestPdu_1[] = {
	{ ATF_POINTER, 1, offsetof(struct InitiateRequestPdu, localDetailCalling),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer32,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"localDetailCalling"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiateRequestPdu, proposedMaxServOutstandingCalling),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer16,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"proposedMaxServOutstandingCalling"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiateRequestPdu, proposedMaxServOutstandingCalled),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer16,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"proposedMaxServOutstandingCalled"
		},
	{ ATF_POINTER, 1, offsetof(struct InitiateRequestPdu, proposedDataStructureNestingLevel),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Integer8,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"proposedDataStructureNestingLevel"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InitiateRequestPdu, mmsInitRequestDetail),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_InitRequestDetail,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"mmsInitRequestDetail"
		},
};
static ber_tlv_tag_t asn_DEF_InitiateRequestPdu_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_InitiateRequestPdu_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* localDetailCalling at 185 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* proposedMaxServOutstandingCalling at 186 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* proposedMaxServOutstandingCalled at 187 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* proposedDataStructureNestingLevel at 188 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* mmsInitRequestDetail at 190 */
};
static asn_SEQUENCE_specifics_t asn_SPC_InitiateRequestPdu_specs_1 = {
	sizeof(struct InitiateRequestPdu),
	offsetof(struct InitiateRequestPdu, _asn_ctx),
	asn_MAP_InitiateRequestPdu_tag2el_1,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_InitiateRequestPdu = {
	"InitiateRequestPdu",
	"InitiateRequestPdu",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_InitiateRequestPdu_tags_1,
	sizeof(asn_DEF_InitiateRequestPdu_tags_1)
		/sizeof(asn_DEF_InitiateRequestPdu_tags_1[0]), /* 1 */
	asn_DEF_InitiateRequestPdu_tags_1,	/* Same as above */
	sizeof(asn_DEF_InitiateRequestPdu_tags_1)
		/sizeof(asn_DEF_InitiateRequestPdu_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_InitiateRequestPdu_1,
	5,	/* Elements count */
	&asn_SPC_InitiateRequestPdu_specs_1	/* Additional specs */
};

