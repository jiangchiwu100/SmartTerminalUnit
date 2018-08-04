﻿/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "MMS"
 * 	found in "../mms-extended.asn"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_ParameterSupportOptions_H_
#define	_ParameterSupportOptions_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ParameterSupportOptions {
	ParameterSupportOptions_str1	= 0,
	ParameterSupportOptions_str2	= 1,
	ParameterSupportOptions_vnam	= 2,
	ParameterSupportOptions_valt	= 3,
	ParameterSupportOptions_vadr	= 4,
	ParameterSupportOptions_vsca	= 5,
	ParameterSupportOptions_tpy	= 6,
	ParameterSupportOptions_vlis	= 7,
	ParameterSupportOptions_real	= 8,
	ParameterSupportOptions_cei	= 10
} e_ParameterSupportOptions;

/* ParameterSupportOptions */
typedef BIT_STRING_t	 ParameterSupportOptions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ParameterSupportOptions;
asn_struct_free_f ParameterSupportOptions_free;
asn_struct_print_f ParameterSupportOptions_print;
asn_constr_check_f ParameterSupportOptions_constraint;
ber_type_decoder_f ParameterSupportOptions_decode_ber;
der_type_encoder_f ParameterSupportOptions_encode_der;
xer_type_decoder_f ParameterSupportOptions_decode_xer;
xer_type_encoder_f ParameterSupportOptions_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _ParameterSupportOptions_H_ */
