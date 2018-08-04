﻿/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#include <asn_internal.h>

#include "AccessResult.h"

static asn_TYPE_member_t asn_MBR_AccessResult_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.failure),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DataAccessError,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"failure"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.array),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DataSequence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"array"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.structure),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DataSequence,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"structure"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.boolean),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"boolean"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.bitstring),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"bitstring"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.integer),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"integer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.Unsigned),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"unsigned"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.floatingpoint),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FloatingPoint,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"floatingpoint"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.octetstring),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"octetstring"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.visiblestring),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VisibleString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"visiblestring"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.generalizedtime),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeneralizedTime,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"generalizedtime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.binarytime),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TimeOfDay,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"binarytime"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.bcd),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"bcd"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.booleanArray),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"booleanArray"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.mMSString),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MMSString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"mMSString"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AccessResult, choice.utctime),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UtcTime,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"utctime"
		},
};
static asn_TYPE_tag2member_t asn_MAP_AccessResult_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* failure at 687 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* array at 689 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* structure at 690 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* boolean at 691 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* bitstring at 692 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* integer at 693 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* unsigned at 694 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* floatingpoint at 695 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 8, 0, 0 }, /* octetstring at 697 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 9, 0, 0 }, /* visiblestring at 698 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 10, 0, 0 }, /* generalizedtime at 699 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 11, 0, 0 }, /* binarytime at 700 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 12, 0, 0 }, /* bcd at 701 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 13, 0, 0 }, /* booleanArray at 702 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 14, 0, 0 }, /* mMSString at 705 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 15, 0, 0 } /* utctime at 709 */
};
static asn_CHOICE_specifics_t asn_SPC_AccessResult_specs_1 = {
	sizeof(struct AccessResult),
	offsetof(struct AccessResult, _asn_ctx),
	offsetof(struct AccessResult, present),
	sizeof(((struct AccessResult *)0)->present),
	asn_MAP_AccessResult_tag2el_1,
	16,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_AccessResult = {
	"AccessResult",
	"AccessResult",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_AccessResult_1,
	16,	/* Elements count */
	&asn_SPC_AccessResult_specs_1	/* Additional specs */
};

