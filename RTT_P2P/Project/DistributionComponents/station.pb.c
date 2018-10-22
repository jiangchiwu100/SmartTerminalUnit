/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.1 at Mon Oct 22 20:44:49 2018. */

#include "station.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t node_property_fields[14] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, node_property, id, id, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, type, id, 0),
    PB_FIELD(  3, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, state, type, 0),
    PB_FIELD(  5, UINT32  , REPEATED, STATIC  , OTHER, node_property, neighbourCollect, state, 0),
    PB_FIELD(  6, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, operateType, neighbourCollect, 0),
    PB_FIELD(  7, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, overTimeType, operateType, 0),
    PB_FIELD(  8, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, removalType, overTimeType, 0),
    PB_FIELD(  9, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, insulateType, removalType, 0),
    PB_FIELD( 10, UINT32  , OPTIONAL, STATIC  , OTHER, node_property, faultState, insulateType, 0),
    PB_FIELD( 11, BOOL    , REQUIRED, STATIC  , OTHER, node_property, isFaultEdgeConnected, faultState, 0),
    PB_FIELD( 13, UINT32  , REPEATED, STATIC  , OTHER, node_property, indexArea, isFaultEdgeConnected, 0),
    PB_FIELD( 14, BOOL    , REPEATED, STATIC  , OTHER, node_property, isExitArea, indexArea, 0),
    PB_FIELD( 15, BOOL    , REPEATED, STATIC  , OTHER, node_property, isGather, isExitArea, 0),
    PB_LAST_FIELD
};

const pb_field_t connect_switch_fields[6] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, connect_switch, transferCode, transferCode, 0),
    PB_FIELD(  2, UINT32  , REPEATED, STATIC  , OTHER, connect_switch, path1, transferCode, 0),
    PB_FIELD(  3, UINT32  , REPEATED, STATIC  , OTHER, connect_switch, path2, path1, 0),
    PB_FIELD(  4, UINT32  , OPTIONAL, STATIC  , OTHER, connect_switch, count, path2, 0),
    PB_FIELD(  5, BOOL    , REQUIRED, STATIC  , OTHER, connect_switch, isConnect, count, 0),
    PB_LAST_FIELD
};

const pb_field_t connect_path_fields[4] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, connect_path, id, id, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, connect_path, remainderCapacity, id, 0),
    PB_FIELD(  3, BOOL    , REQUIRED, STATIC  , OTHER, connect_path, isUpdated, remainderCapacity, 0),
    PB_LAST_FIELD
};

const pb_field_t distribution_power_area_fields[9] = {
    PB_FIELD(  1, UINT32  , REPEATED, STATIC  , FIRST, distribution_power_area, areaSwitch, areaSwitch, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, distribution_power_area, upadetedFlag, areaSwitch, 0),
    PB_FIELD(  3, BOOL    , REQUIRED, STATIC  , OTHER, distribution_power_area, isUpadeted, upadetedFlag, 0),
    PB_FIELD(  4, BOOL    , REQUIRED, STATIC  , OTHER, distribution_power_area, isFaultArea, isUpadeted, 0),
    PB_FIELD(  5, BOOL    , REQUIRED, STATIC  , OTHER, distribution_power_area, isExitFaultMessage, isFaultArea, 0),
    PB_FIELD(  6, UINT32  , OPTIONAL, STATIC  , OTHER, distribution_power_area, upadetedFull, isExitFaultMessage, 0),
    PB_FIELD(  7, UINT32  , OPTIONAL, STATIC  , OTHER, distribution_power_area, removalType, upadetedFull, 0),
    PB_FIELD(  8, UINT32  , OPTIONAL, STATIC  , OTHER, distribution_power_area, insulateType, removalType, 0),
    PB_LAST_FIELD
};

const pb_field_t distribution_station_fields[8] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, distribution_station, areaCount, areaCount, 0),
    PB_FIELD(  2, BOOL    , REQUIRED, STATIC  , OTHER, distribution_station, isComplted, areaCount, 0),
    PB_FIELD(  3, BOOL    , REQUIRED, STATIC  , OTHER, distribution_station, isGatherCompleted, isComplted, 0),
    PB_FIELD(  4, BOOL    , REQUIRED, STATIC  , OTHER, distribution_station, isGatherCalculateCompleted, isGatherCompleted, 0),
    PB_FIELD(  5, UINT32  , OPTIONAL, STATIC  , OTHER, distribution_station, switchRef, isGatherCalculateCompleted, 0),
    PB_FIELD(  6, BOOL    , REQUIRED, STATIC  , OTHER, distribution_station, isAlreayExitedFault, switchRef, 0),
    PB_FIELD(  7, BOOL    , REQUIRED, STATIC  , OTHER, distribution_station, isExitedInsulateFailure, isAlreayExitedFault, 0),
    PB_LAST_FIELD
};

const pb_field_t faultdeal_handle_fields[20] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, faultdeal_handle, switchPropertyID, switchPropertyID, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, state, switchPropertyID, 0),
    PB_FIELD(  3, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, lastState, state, 0),
    PB_FIELD(  4, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, nextState, lastState, 0),
    PB_FIELD(  5, BOOL    , REQUIRED, STATIC  , OTHER, faultdeal_handle, isRun, nextState, 0),
    PB_FIELD(  6, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, step, isRun, 0),
    PB_FIELD(  7, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, startTime, step, 0),
    PB_FIELD(  8, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, limitTime, startTime, 0),
    PB_FIELD(  9, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, t1, limitTime, 0),
    PB_FIELD( 10, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, t2, t1, 0),
    PB_FIELD( 11, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, t3, t2, 0),
    PB_FIELD( 12, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, t4, t3, 0),
    PB_FIELD( 13, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, t5, t4, 0),
    PB_FIELD( 14, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, checkOpenTime, t5, 0),
    PB_FIELD( 15, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, checkBackupTime, checkOpenTime, 0),
    PB_FIELD( 16, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, rejectTime, checkBackupTime, 0),
    PB_FIELD( 17, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, reciveRemovalSuccessTime, rejectTime, 0),
    PB_FIELD( 18, UINT32  , OPTIONAL, STATIC  , OTHER, faultdeal_handle, reciveConnectTime, reciveRemovalSuccessTime, 0),
    PB_FIELD( 19, BOOL    , REQUIRED, STATIC  , OTHER, faultdeal_handle, isCheckPass, reciveConnectTime, 0),
    PB_LAST_FIELD
};

const pb_field_t StationMessage_fields[10] = {
    PB_FIELD(  1, UINT32  , REPEATED, STATIC  , FIRST, StationMessage, idCollect, idCollect, 0),
    PB_FIELD(  2, BOOL    , REQUIRED, STATIC  , OTHER, StationMessage, isGainComplted, idCollect, 0),
    PB_FIELD(  3, MESSAGE , REPEATED, STATIC  , OTHER, StationMessage, node, isGainComplted, &node_property_fields),
    PB_FIELD(  4, MESSAGE , OPTIONAL, STATIC  , OTHER, StationMessage, connect, node, &connect_switch_fields),
    PB_FIELD(  5, MESSAGE , REPEATED, STATIC  , OTHER, StationMessage, con_path, connect, &connect_path_fields),
    PB_FIELD(  6, MESSAGE , REPEATED, STATIC  , OTHER, StationMessage, power_area, con_path, &distribution_power_area_fields),
    PB_FIELD(  7, MESSAGE , OPTIONAL, STATIC  , OTHER, StationMessage, distribution, power_area, &distribution_station_fields),
    PB_FIELD(  8, MESSAGE , OPTIONAL, STATIC  , OTHER, StationMessage, fault_handle, distribution, &faultdeal_handle_fields),
    PB_FIELD(  9, UINT32  , OPTIONAL, STATIC  , OTHER, StationMessage, id, fault_handle, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(StationMessage, node[0]) < 65536 && pb_membersize(StationMessage, connect) < 65536 && pb_membersize(StationMessage, con_path[0]) < 65536 && pb_membersize(StationMessage, power_area[0]) < 65536 && pb_membersize(StationMessage, distribution) < 65536 && pb_membersize(StationMessage, fault_handle) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_node_property_connect_switch_connect_path_distribution_power_area_distribution_station_faultdeal_handle_StationMessage)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(StationMessage, node[0]) < 256 && pb_membersize(StationMessage, connect) < 256 && pb_membersize(StationMessage, con_path[0]) < 256 && pb_membersize(StationMessage, power_area[0]) < 256 && pb_membersize(StationMessage, distribution) < 256 && pb_membersize(StationMessage, fault_handle) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_node_property_connect_switch_connect_path_distribution_power_area_distribution_station_faultdeal_handle_StationMessage)
#endif


/* @@protoc_insertion_point(eof) */
