/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.5 */

#ifndef PB_MCU_PB_H_INCLUDED
#define PB_MCU_PB_H_INCLUDED
#include "nanopb/pb.h"

#ifdef __cplusplus
namespace Pufferfish::Application {
#endif

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _VentilationMode {
    VentilationMode_hfnc = 0,
    VentilationMode_pc_ac = 1,
    VentilationMode_vc_ac = 2,
    VentilationMode_niv_pc = 3,
    VentilationMode_niv_ps = 4,
    VentilationMode_psv = 5,
    VentilationMode_prvc = 6
} VentilationMode;

typedef enum _LogEventCode {
    /* Patient alarms */
    LogEventCode_fio2_too_low = 0,
    LogEventCode_fio2_too_high = 1,
    LogEventCode_flow_too_low = 2,
    LogEventCode_flow_too_high = 3,
    LogEventCode_spo2_too_low = 4,
    LogEventCode_spo2_too_high = 5,
    LogEventCode_hr_too_low = 6,
    LogEventCode_hr_too_high = 7,
    /* Control settings */
    LogEventCode_ventilation_operation_changed = 64,
    LogEventCode_ventilation_mode_changed = 65,
    LogEventCode_fio2_setting_changed = 66,
    LogEventCode_flow_setting_changed = 67,
    /* Alarm limits settings */
    LogEventCode_fio2_alarm_limits_changed = 80,
    LogEventCode_flow_alarm_limits_changed = 81,
    LogEventCode_spo2_alarm_limits_changed = 82,
    LogEventCode_hr_alarm_limits_changed = 83,
    /* System settings & alarms */
    LogEventCode_screen_locked = 129,
    LogEventCode_mcu_backend_connection_down = 130, /* mcu lost backend */
    LogEventCode_backend_mcu_connection_down = 131, /* backend lost mcu */
    LogEventCode_backend_frontend_connection_down = 132, /* backend lost frontend */
    /* TODO: if the backend can't send data to the frontend but the frontend can send
 data to the backend, we should make the backend generate a frontend_backend_down
 log event so that there's some record in the event log of a connection problem.
 We can achieve this using a FrontendConnections message type which the frontend sends
 to the backend. */
    LogEventCode_frontend_backend_connection_down = 133, /* frontend lost backend */
    LogEventCode_mcu_backend_connection_up = 134, /* mcu detected backend */
    /* TODO: if the mcu can't send data to the backend but the backend can send
 data to the mcu, we should make the backend generate a backend_mcu_up
 log event once the mcu becomes able to send data again so that the event log
 has a connection_up event to match the connection_down event. */
    LogEventCode_backend_mcu_connection_up = 135, /* backend detected mcu */
    LogEventCode_backend_frontend_connection_up = 136, /* backend detected frontend */
    /* TODO: if the backend can't send data to the frontend but the frontend can send
 data to the backend, we should make the backend generate a frontend_backend_up
 log event once the frontend detects that the backend is able to send data again,
 so that the event log has a connection_up event to match the connection_down event.
 We can achieve this using a FrontendConnections message type which the frontend sends
 to the backend. */
    LogEventCode_frontend_backend_connection_up = 137, /* frontend detected backend */
    LogEventCode_battery_low = 138,
    LogEventCode_battery_critical = 139,
    LogEventCode_charger_disconnected = 140,
    LogEventCode_mcu_started = 141,
    LogEventCode_backend_started = 142,
    LogEventCode_mcu_shutdown = 143,
    LogEventCode_backend_shutdown = 144,
    LogEventCode_sysclock_changed = 145,
    /* Alarm muting/unmuting */
    LogEventCode_alarms_muted_user_software = 146,
    LogEventCode_alarms_muted_user_hardware = 147,
    LogEventCode_alarms_muted_unknown = 148, /* Indicates a software bug; should never occur */
    LogEventCode_alarms_unmuted_user_software = 149,
    LogEventCode_alarms_unmuted_user_hardware = 150,
    LogEventCode_alarms_unmuted_initialization = 151,
    LogEventCode_alarms_unmuted_timeout = 152,
    LogEventCode_alarms_unmuted_mcu_backend_loss = 153,
    LogEventCode_alarms_unmuted_backend_mcu_loss = 154,
    LogEventCode_alarms_unmuted_backend_frontend_loss = 155,
    LogEventCode_alarms_unmuted_frontend_backend_loss = 156,
    LogEventCode_alarms_unmuted_unknown = 157, /* Indicates a software bug; should never occur */
    /* Sensor loss */
    LogEventCode_sfm3019_air_disconnected = 160,
    LogEventCode_sfm3019_o2_disconnected = 161,
    LogEventCode_fdo2_disconnected = 162,
    LogEventCode_nonin_disconnected = 163,
    LogEventCode_nonin_finger_sensor_disconnected = 164,
    LogEventCode_nonin_sensor_alarm = 165,
    LogEventCode_nonin_out_of_track_measurements = 166
} LogEventCode;

typedef enum _LogEventType {
    LogEventType_patient = 0,
    LogEventType_control = 1,
    LogEventType_alarm_limits = 2,
    LogEventType_system = 3
} LogEventType;

typedef enum _AlarmMuteSource {
    AlarmMuteSource_initialization = 0, /* Alarm muting was set on initialization */
    AlarmMuteSource_user_software = 1, /* User took a software action to change alarm muting */
    AlarmMuteSource_user_hardware = 2, /* User took a hardware action to change alarm muting */
    AlarmMuteSource_timeout = 3, /* Alarm mute automatically timed out */
    AlarmMuteSource_mcu_backend_loss = 4, /* Alarm mute was cancelled by mcu due to connection loss */
    AlarmMuteSource_backend_mcu_loss = 5, /* Alarm mute was cancelled by backend due to connection loss */
    AlarmMuteSource_backend_frontend_loss = 6, /* Alarm mute was cancelled by backend due to connection loss */
    AlarmMuteSource_frontend_backend_loss = 7 /* Alarm mute was cancelled by frontend due to connection loss */
} AlarmMuteSource;

/* Struct definitions */
/* Note: NextLogEvents has a custom equality operator in the firmware which must
 be updated if you add/remove/modify the fields of the protobuf definition! */
typedef struct _ActiveLogEvents {
    pb_size_t id_count;
    uint32_t id[32];
} ActiveLogEvents;

typedef struct _AlarmMute {
    bool active;
    /* seq_num is a logical clock and advances in the firmware after each local action
 in the firmware (such as a button-press) or the servicing of each external request. */
    uint32_t seq_num;
    AlarmMuteSource source;
    uint64_t remaining; /* ms */
} AlarmMute;

typedef struct _AlarmMuteRequest {
    bool active;
    /* seq_num is a logical clock which also acts as an idempotency key for requests:
 the firmware only services an AlarmMuteRequest if the request's seq_num is one
 greater than the seq_num in the firmware's copy of AlarmMute. */
    uint32_t seq_num;
    AlarmMuteSource source;
} AlarmMuteRequest;

typedef PB_BYTES_ARRAY_T(64) Announcement_announcement_t;
typedef struct _Announcement {
    uint64_t time; /* ms */
    Announcement_announcement_t announcement;
} Announcement;

typedef struct _BackendConnections {
    bool has_mcu;
    bool has_frontend;
} BackendConnections;

typedef struct _CycleMeasurements {
    uint64_t time; /* ms */
    float vt;
    float rr;
    float peep;
    float pip;
    float ip;
    float ve;
} CycleMeasurements;

typedef struct _ExpectedLogEvent {
    uint32_t id;
    /* session_id is checked by the NextLogEvents sender if the sender's log is
 ephemeral; the sender will ignore any ExpectedLogEvent whose session_id
 doesn't match the sender's session_id, which is announced in NextLogEvents. */
    uint32_t session_id; /* used when the sender's log is ephemeral */
} ExpectedLogEvent;

typedef struct _MCUPowerStatus {
    float power_left;
    bool charging;
} MCUPowerStatus;

typedef struct _Parameters {
    uint64_t time; /* ms */
    bool ventilating;
    VentilationMode mode;
    float fio2;
    float flow;
    float pip;
    float peep;
    float vt;
    float rr;
    float ie;
} Parameters;

typedef struct _ParametersRequest {
    uint64_t time; /* ms */
    bool ventilating;
    VentilationMode mode;
    float fio2;
    float flow;
    float pip;
    float peep;
    float vt;
    float rr;
    float ie;
} ParametersRequest;

typedef struct _Ping {
    uint64_t time; /* ms */
    uint32_t id;
} Ping;

typedef struct _Range {
    int32_t lower;
    int32_t upper;
} Range;

typedef struct _ScreenStatus {
    bool lock;
} ScreenStatus;

typedef struct _SensorMeasurements {
    uint64_t time; /* ms */
    uint32_t cycle;
    float fio2;
    float flow;
    float spo2;
    float hr;
    float paw;
    float volume;
} SensorMeasurements;

/* TODO: AlarmLimits has a max size above 256 bytes, so we need to increase the communication protocol's chunks from a max length of 256 bytes to something more like 512 bytes! */
typedef struct _AlarmLimits {
    uint64_t time; /* ms */
    bool has_fio2;
    Range fio2;
    bool has_flow;
    Range flow;
    bool has_spo2;
    Range spo2;
    bool has_hr;
    Range hr;
    bool has_rr;
    Range rr;
    bool has_pip;
    Range pip;
    bool has_peep;
    Range peep;
    bool has_ip_above_peep;
    Range ip_above_peep;
    bool has_insp_time;
    Range insp_time;
    bool has_paw;
    Range paw;
    bool has_mve;
    Range mve;
    bool has_tv;
    Range tv;
    bool has_etco2;
    Range etco2;
    bool has_apnea;
    Range apnea;
} AlarmLimits;

/* TODO: AlarmLimitsRequest has a max size above 256 bytes, so we need to increase the communication protocol's chunks from a max length of 256 bytes to something more like 512 bytes! */
typedef struct _AlarmLimitsRequest {
    uint64_t time; /* ms */
    bool has_fio2;
    Range fio2;
    bool has_flow;
    Range flow;
    bool has_spo2;
    Range spo2;
    bool has_hr;
    Range hr;
    bool has_rr;
    Range rr;
    bool has_pip;
    Range pip;
    bool has_peep;
    Range peep;
    bool has_ip_above_peep;
    Range ip_above_peep;
    bool has_insp_time;
    Range insp_time;
    bool has_paw;
    Range paw;
    bool has_mve;
    Range mve;
    bool has_tv;
    Range tv;
    bool has_etco2;
    Range etco2;
    bool has_apnea;
    Range apnea;
} AlarmLimitsRequest;

typedef struct _LogEvent {
    uint32_t id;
    uint64_t time; /* ms */
    LogEventCode code;
    LogEventType type;
    bool has_alarm_limits;
    Range alarm_limits;
    /* TODO: rename these to old/new_double */
    double old_float;
    double new_float;
    uint32_t old_uint32;
    uint32_t new_uint32;
    bool old_bool;
    bool new_bool;
    bool has_old_range;
    Range old_range;
    bool has_new_range;
    Range new_range;
    VentilationMode old_mode;
    VentilationMode new_mode;
} LogEvent;

/* Note: NextLogEvents has a custom equality operator in the firmware which must
 be updated if you add/remove/modify the fields of the protobuf definition!
 TODO: NextLogEvents has a max size above 256 bytes, so we need to increase the communication protocol's chunks from a max length of 256 bytes to something more like 512 bytes! */
typedef struct _NextLogEvents {
    uint32_t next_expected;
    uint32_t total;
    uint32_t remaining;
    uint32_t session_id; /* used when the sender's log is ephemeral */
    pb_size_t elements_count;
    LogEvent elements[2];
} NextLogEvents;


/* Helper constants for enums */
#define _VentilationMode_MIN VentilationMode_hfnc
#define _VentilationMode_MAX VentilationMode_prvc
#define _VentilationMode_ARRAYSIZE ((VentilationMode)(VentilationMode_prvc+1))

#define _LogEventCode_MIN LogEventCode_fio2_too_low
#define _LogEventCode_MAX LogEventCode_nonin_out_of_track_measurements
#define _LogEventCode_ARRAYSIZE ((LogEventCode)(LogEventCode_nonin_out_of_track_measurements+1))

#define _LogEventType_MIN LogEventType_patient
#define _LogEventType_MAX LogEventType_system
#define _LogEventType_ARRAYSIZE ((LogEventType)(LogEventType_system+1))

#define _AlarmMuteSource_MIN AlarmMuteSource_initialization
#define _AlarmMuteSource_MAX AlarmMuteSource_frontend_backend_loss
#define _AlarmMuteSource_ARRAYSIZE ((AlarmMuteSource)(AlarmMuteSource_frontend_backend_loss+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define SensorMeasurements_init_default          {0, 0, 0, 0, 0, 0, 0, 0}
#define CycleMeasurements_init_default           {0, 0, 0, 0, 0, 0, 0}
#define Parameters_init_default                  {0, 0, _VentilationMode_MIN, 0, 0, 0, 0, 0, 0, 0}
#define ParametersRequest_init_default           {0, 0, _VentilationMode_MIN, 0, 0, 0, 0, 0, 0, 0}
#define Range_init_default                       {0, 0}
#define AlarmLimits_init_default                 {0, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default}
#define AlarmLimitsRequest_init_default          {0, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default, false, Range_init_default}
#define LogEvent_init_default                    {0, 0, _LogEventCode_MIN, _LogEventType_MIN, false, Range_init_default, 0, 0, 0, 0, 0, 0, false, Range_init_default, false, Range_init_default, _VentilationMode_MIN, _VentilationMode_MIN}
#define ExpectedLogEvent_init_default            {0, 0}
#define NextLogEvents_init_default               {0, 0, 0, 0, 0, {LogEvent_init_default, LogEvent_init_default}}
#define ActiveLogEvents_init_default             {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
#define AlarmMute_init_default                   {0, 0, _AlarmMuteSource_MIN, 0}
#define AlarmMuteRequest_init_default            {0, 0, _AlarmMuteSource_MIN}
#define MCUPowerStatus_init_default              {0, 0}
#define BackendConnections_init_default          {0, 0}
#define ScreenStatus_init_default                {0}
#define Ping_init_default                        {0, 0}
#define Announcement_init_default                {0, {0, {0}}}
#define SensorMeasurements_init_zero             {0, 0, 0, 0, 0, 0, 0, 0}
#define CycleMeasurements_init_zero              {0, 0, 0, 0, 0, 0, 0}
#define Parameters_init_zero                     {0, 0, _VentilationMode_MIN, 0, 0, 0, 0, 0, 0, 0}
#define ParametersRequest_init_zero              {0, 0, _VentilationMode_MIN, 0, 0, 0, 0, 0, 0, 0}
#define Range_init_zero                          {0, 0}
#define AlarmLimits_init_zero                    {0, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero}
#define AlarmLimitsRequest_init_zero             {0, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero, false, Range_init_zero}
#define LogEvent_init_zero                       {0, 0, _LogEventCode_MIN, _LogEventType_MIN, false, Range_init_zero, 0, 0, 0, 0, 0, 0, false, Range_init_zero, false, Range_init_zero, _VentilationMode_MIN, _VentilationMode_MIN}
#define ExpectedLogEvent_init_zero               {0, 0}
#define NextLogEvents_init_zero                  {0, 0, 0, 0, 0, {LogEvent_init_zero, LogEvent_init_zero}}
#define ActiveLogEvents_init_zero                {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
#define AlarmMute_init_zero                      {0, 0, _AlarmMuteSource_MIN, 0}
#define AlarmMuteRequest_init_zero               {0, 0, _AlarmMuteSource_MIN}
#define MCUPowerStatus_init_zero                 {0, 0}
#define BackendConnections_init_zero             {0, 0}
#define ScreenStatus_init_zero                   {0}
#define Ping_init_zero                           {0, 0}
#define Announcement_init_zero                   {0, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define ActiveLogEvents_id_tag                   1
#define AlarmMute_active_tag                     1
#define AlarmMute_seq_num_tag                    2
#define AlarmMute_source_tag                     3
#define AlarmMute_remaining_tag                  4
#define AlarmMuteRequest_active_tag              1
#define AlarmMuteRequest_seq_num_tag             2
#define AlarmMuteRequest_source_tag              3
#define Announcement_time_tag                    1
#define Announcement_announcement_tag            2
#define BackendConnections_has_mcu_tag           1
#define BackendConnections_has_frontend_tag      2
#define CycleMeasurements_time_tag               1
#define CycleMeasurements_vt_tag                 2
#define CycleMeasurements_rr_tag                 3
#define CycleMeasurements_peep_tag               4
#define CycleMeasurements_pip_tag                5
#define CycleMeasurements_ip_tag                 6
#define CycleMeasurements_ve_tag                 7
#define ExpectedLogEvent_id_tag                  1
#define ExpectedLogEvent_session_id_tag          2
#define MCUPowerStatus_power_left_tag            1
#define MCUPowerStatus_charging_tag              2
#define Parameters_time_tag                      1
#define Parameters_ventilating_tag               2
#define Parameters_mode_tag                      3
#define Parameters_fio2_tag                      4
#define Parameters_flow_tag                      5
#define Parameters_pip_tag                       6
#define Parameters_peep_tag                      7
#define Parameters_vt_tag                        8
#define Parameters_rr_tag                        9
#define Parameters_ie_tag                        10
#define ParametersRequest_time_tag               1
#define ParametersRequest_ventilating_tag        2
#define ParametersRequest_mode_tag               3
#define ParametersRequest_fio2_tag               4
#define ParametersRequest_flow_tag               5
#define ParametersRequest_pip_tag                6
#define ParametersRequest_peep_tag               7
#define ParametersRequest_vt_tag                 8
#define ParametersRequest_rr_tag                 9
#define ParametersRequest_ie_tag                 10
#define Ping_time_tag                            1
#define Ping_id_tag                              2
#define Range_lower_tag                          1
#define Range_upper_tag                          2
#define ScreenStatus_lock_tag                    1
#define SensorMeasurements_time_tag              1
#define SensorMeasurements_cycle_tag             2
#define SensorMeasurements_fio2_tag              3
#define SensorMeasurements_flow_tag              4
#define SensorMeasurements_spo2_tag              5
#define SensorMeasurements_hr_tag                6
#define SensorMeasurements_paw_tag               7
#define SensorMeasurements_volume_tag            8
#define AlarmLimits_time_tag                     1
#define AlarmLimits_fio2_tag                     2
#define AlarmLimits_flow_tag                     3
#define AlarmLimits_spo2_tag                     4
#define AlarmLimits_hr_tag                       5
#define AlarmLimits_rr_tag                       6
#define AlarmLimits_pip_tag                      7
#define AlarmLimits_peep_tag                     8
#define AlarmLimits_ip_above_peep_tag            9
#define AlarmLimits_insp_time_tag                10
#define AlarmLimits_paw_tag                      11
#define AlarmLimits_mve_tag                      12
#define AlarmLimits_tv_tag                       13
#define AlarmLimits_etco2_tag                    14
#define AlarmLimits_apnea_tag                    15
#define AlarmLimitsRequest_time_tag              1
#define AlarmLimitsRequest_fio2_tag              2
#define AlarmLimitsRequest_flow_tag              3
#define AlarmLimitsRequest_spo2_tag              4
#define AlarmLimitsRequest_hr_tag                5
#define AlarmLimitsRequest_rr_tag                6
#define AlarmLimitsRequest_pip_tag               7
#define AlarmLimitsRequest_peep_tag              8
#define AlarmLimitsRequest_ip_above_peep_tag     9
#define AlarmLimitsRequest_insp_time_tag         10
#define AlarmLimitsRequest_paw_tag               11
#define AlarmLimitsRequest_mve_tag               12
#define AlarmLimitsRequest_tv_tag                13
#define AlarmLimitsRequest_etco2_tag             14
#define AlarmLimitsRequest_apnea_tag             15
#define LogEvent_id_tag                          1
#define LogEvent_time_tag                        2
#define LogEvent_code_tag                        3
#define LogEvent_type_tag                        4
#define LogEvent_alarm_limits_tag                5
#define LogEvent_old_float_tag                   6
#define LogEvent_new_float_tag                   7
#define LogEvent_old_uint32_tag                  8
#define LogEvent_new_uint32_tag                  9
#define LogEvent_old_bool_tag                    10
#define LogEvent_new_bool_tag                    11
#define LogEvent_old_range_tag                   12
#define LogEvent_new_range_tag                   13
#define LogEvent_old_mode_tag                    14
#define LogEvent_new_mode_tag                    15
#define NextLogEvents_next_expected_tag          1
#define NextLogEvents_total_tag                  2
#define NextLogEvents_remaining_tag              3
#define NextLogEvents_session_id_tag             4
#define NextLogEvents_elements_tag               5

/* Struct field encoding specification for nanopb */
#define SensorMeasurements_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   SINGULAR, UINT32,   cycle,             2) \
X(a, STATIC,   SINGULAR, FLOAT,    fio2,              3) \
X(a, STATIC,   SINGULAR, FLOAT,    flow,              4) \
X(a, STATIC,   SINGULAR, FLOAT,    spo2,              5) \
X(a, STATIC,   SINGULAR, FLOAT,    hr,                6) \
X(a, STATIC,   SINGULAR, FLOAT,    paw,               7) \
X(a, STATIC,   SINGULAR, FLOAT,    volume,            8)
#define SensorMeasurements_CALLBACK NULL
#define SensorMeasurements_DEFAULT NULL

#define CycleMeasurements_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   SINGULAR, FLOAT,    vt,                2) \
X(a, STATIC,   SINGULAR, FLOAT,    rr,                3) \
X(a, STATIC,   SINGULAR, FLOAT,    peep,              4) \
X(a, STATIC,   SINGULAR, FLOAT,    pip,               5) \
X(a, STATIC,   SINGULAR, FLOAT,    ip,                6) \
X(a, STATIC,   SINGULAR, FLOAT,    ve,                7)
#define CycleMeasurements_CALLBACK NULL
#define CycleMeasurements_DEFAULT NULL

#define Parameters_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   SINGULAR, BOOL,     ventilating,       2) \
X(a, STATIC,   SINGULAR, UENUM,    mode,              3) \
X(a, STATIC,   SINGULAR, FLOAT,    fio2,              4) \
X(a, STATIC,   SINGULAR, FLOAT,    flow,              5) \
X(a, STATIC,   SINGULAR, FLOAT,    pip,               6) \
X(a, STATIC,   SINGULAR, FLOAT,    peep,              7) \
X(a, STATIC,   SINGULAR, FLOAT,    vt,                8) \
X(a, STATIC,   SINGULAR, FLOAT,    rr,                9) \
X(a, STATIC,   SINGULAR, FLOAT,    ie,               10)
#define Parameters_CALLBACK NULL
#define Parameters_DEFAULT NULL

#define ParametersRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   SINGULAR, BOOL,     ventilating,       2) \
X(a, STATIC,   SINGULAR, UENUM,    mode,              3) \
X(a, STATIC,   SINGULAR, FLOAT,    fio2,              4) \
X(a, STATIC,   SINGULAR, FLOAT,    flow,              5) \
X(a, STATIC,   SINGULAR, FLOAT,    pip,               6) \
X(a, STATIC,   SINGULAR, FLOAT,    peep,              7) \
X(a, STATIC,   SINGULAR, FLOAT,    vt,                8) \
X(a, STATIC,   SINGULAR, FLOAT,    rr,                9) \
X(a, STATIC,   SINGULAR, FLOAT,    ie,               10)
#define ParametersRequest_CALLBACK NULL
#define ParametersRequest_DEFAULT NULL

#define Range_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    lower,             1) \
X(a, STATIC,   SINGULAR, INT32,    upper,             2)
#define Range_CALLBACK NULL
#define Range_DEFAULT NULL

#define AlarmLimits_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  fio2,              2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  flow,              3) \
X(a, STATIC,   OPTIONAL, MESSAGE,  spo2,              4) \
X(a, STATIC,   OPTIONAL, MESSAGE,  hr,                5) \
X(a, STATIC,   OPTIONAL, MESSAGE,  rr,                6) \
X(a, STATIC,   OPTIONAL, MESSAGE,  pip,               7) \
X(a, STATIC,   OPTIONAL, MESSAGE,  peep,              8) \
X(a, STATIC,   OPTIONAL, MESSAGE,  ip_above_peep,     9) \
X(a, STATIC,   OPTIONAL, MESSAGE,  insp_time,        10) \
X(a, STATIC,   OPTIONAL, MESSAGE,  paw,              11) \
X(a, STATIC,   OPTIONAL, MESSAGE,  mve,              12) \
X(a, STATIC,   OPTIONAL, MESSAGE,  tv,               13) \
X(a, STATIC,   OPTIONAL, MESSAGE,  etco2,            14) \
X(a, STATIC,   OPTIONAL, MESSAGE,  apnea,            15)
#define AlarmLimits_CALLBACK NULL
#define AlarmLimits_DEFAULT NULL
#define AlarmLimits_fio2_MSGTYPE Range
#define AlarmLimits_flow_MSGTYPE Range
#define AlarmLimits_spo2_MSGTYPE Range
#define AlarmLimits_hr_MSGTYPE Range
#define AlarmLimits_rr_MSGTYPE Range
#define AlarmLimits_pip_MSGTYPE Range
#define AlarmLimits_peep_MSGTYPE Range
#define AlarmLimits_ip_above_peep_MSGTYPE Range
#define AlarmLimits_insp_time_MSGTYPE Range
#define AlarmLimits_paw_MSGTYPE Range
#define AlarmLimits_mve_MSGTYPE Range
#define AlarmLimits_tv_MSGTYPE Range
#define AlarmLimits_etco2_MSGTYPE Range
#define AlarmLimits_apnea_MSGTYPE Range

#define AlarmLimitsRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  fio2,              2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  flow,              3) \
X(a, STATIC,   OPTIONAL, MESSAGE,  spo2,              4) \
X(a, STATIC,   OPTIONAL, MESSAGE,  hr,                5) \
X(a, STATIC,   OPTIONAL, MESSAGE,  rr,                6) \
X(a, STATIC,   OPTIONAL, MESSAGE,  pip,               7) \
X(a, STATIC,   OPTIONAL, MESSAGE,  peep,              8) \
X(a, STATIC,   OPTIONAL, MESSAGE,  ip_above_peep,     9) \
X(a, STATIC,   OPTIONAL, MESSAGE,  insp_time,        10) \
X(a, STATIC,   OPTIONAL, MESSAGE,  paw,              11) \
X(a, STATIC,   OPTIONAL, MESSAGE,  mve,              12) \
X(a, STATIC,   OPTIONAL, MESSAGE,  tv,               13) \
X(a, STATIC,   OPTIONAL, MESSAGE,  etco2,            14) \
X(a, STATIC,   OPTIONAL, MESSAGE,  apnea,            15)
#define AlarmLimitsRequest_CALLBACK NULL
#define AlarmLimitsRequest_DEFAULT NULL
#define AlarmLimitsRequest_fio2_MSGTYPE Range
#define AlarmLimitsRequest_flow_MSGTYPE Range
#define AlarmLimitsRequest_spo2_MSGTYPE Range
#define AlarmLimitsRequest_hr_MSGTYPE Range
#define AlarmLimitsRequest_rr_MSGTYPE Range
#define AlarmLimitsRequest_pip_MSGTYPE Range
#define AlarmLimitsRequest_peep_MSGTYPE Range
#define AlarmLimitsRequest_ip_above_peep_MSGTYPE Range
#define AlarmLimitsRequest_insp_time_MSGTYPE Range
#define AlarmLimitsRequest_paw_MSGTYPE Range
#define AlarmLimitsRequest_mve_MSGTYPE Range
#define AlarmLimitsRequest_tv_MSGTYPE Range
#define AlarmLimitsRequest_etco2_MSGTYPE Range
#define AlarmLimitsRequest_apnea_MSGTYPE Range

#define LogEvent_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   id,                1) \
X(a, STATIC,   SINGULAR, UINT64,   time,              2) \
X(a, STATIC,   SINGULAR, UENUM,    code,              3) \
X(a, STATIC,   SINGULAR, UENUM,    type,              4) \
X(a, STATIC,   OPTIONAL, MESSAGE,  alarm_limits,      5) \
X(a, STATIC,   SINGULAR, DOUBLE,   old_float,         6) \
X(a, STATIC,   SINGULAR, DOUBLE,   new_float,         7) \
X(a, STATIC,   SINGULAR, UINT32,   old_uint32,        8) \
X(a, STATIC,   SINGULAR, UINT32,   new_uint32,        9) \
X(a, STATIC,   SINGULAR, BOOL,     old_bool,         10) \
X(a, STATIC,   SINGULAR, BOOL,     new_bool,         11) \
X(a, STATIC,   OPTIONAL, MESSAGE,  old_range,        12) \
X(a, STATIC,   OPTIONAL, MESSAGE,  new_range,        13) \
X(a, STATIC,   SINGULAR, UENUM,    old_mode,         14) \
X(a, STATIC,   SINGULAR, UENUM,    new_mode,         15)
#define LogEvent_CALLBACK NULL
#define LogEvent_DEFAULT NULL
#define LogEvent_alarm_limits_MSGTYPE Range
#define LogEvent_old_range_MSGTYPE Range
#define LogEvent_new_range_MSGTYPE Range

#define ExpectedLogEvent_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   id,                1) \
X(a, STATIC,   SINGULAR, UINT32,   session_id,        2)
#define ExpectedLogEvent_CALLBACK NULL
#define ExpectedLogEvent_DEFAULT NULL

#define NextLogEvents_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   next_expected,     1) \
X(a, STATIC,   SINGULAR, UINT32,   total,             2) \
X(a, STATIC,   SINGULAR, UINT32,   remaining,         3) \
X(a, STATIC,   SINGULAR, UINT32,   session_id,        4) \
X(a, STATIC,   REPEATED, MESSAGE,  elements,          5)
#define NextLogEvents_CALLBACK NULL
#define NextLogEvents_DEFAULT NULL
#define NextLogEvents_elements_MSGTYPE LogEvent

#define ActiveLogEvents_FIELDLIST(X, a) \
X(a, STATIC,   REPEATED, UINT32,   id,                1)
#define ActiveLogEvents_CALLBACK NULL
#define ActiveLogEvents_DEFAULT NULL

#define AlarmMute_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     active,            1) \
X(a, STATIC,   SINGULAR, UINT32,   seq_num,           2) \
X(a, STATIC,   SINGULAR, UENUM,    source,            3) \
X(a, STATIC,   SINGULAR, UINT64,   remaining,         4)
#define AlarmMute_CALLBACK NULL
#define AlarmMute_DEFAULT NULL

#define AlarmMuteRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     active,            1) \
X(a, STATIC,   SINGULAR, UINT32,   seq_num,           2) \
X(a, STATIC,   SINGULAR, UENUM,    source,            3)
#define AlarmMuteRequest_CALLBACK NULL
#define AlarmMuteRequest_DEFAULT NULL

#define MCUPowerStatus_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    power_left,        1) \
X(a, STATIC,   SINGULAR, BOOL,     charging,          2)
#define MCUPowerStatus_CALLBACK NULL
#define MCUPowerStatus_DEFAULT NULL

#define BackendConnections_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     has_mcu,           1) \
X(a, STATIC,   SINGULAR, BOOL,     has_frontend,      2)
#define BackendConnections_CALLBACK NULL
#define BackendConnections_DEFAULT NULL

#define ScreenStatus_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     lock,              1)
#define ScreenStatus_CALLBACK NULL
#define ScreenStatus_DEFAULT NULL

#define Ping_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   SINGULAR, UINT32,   id,                2)
#define Ping_CALLBACK NULL
#define Ping_DEFAULT NULL

#define Announcement_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   time,              1) \
X(a, STATIC,   SINGULAR, BYTES,    announcement,      2)
#define Announcement_CALLBACK NULL
#define Announcement_DEFAULT NULL

extern const pb_msgdesc_t SensorMeasurements_msg;
extern const pb_msgdesc_t CycleMeasurements_msg;
extern const pb_msgdesc_t Parameters_msg;
extern const pb_msgdesc_t ParametersRequest_msg;
extern const pb_msgdesc_t Range_msg;
extern const pb_msgdesc_t AlarmLimits_msg;
extern const pb_msgdesc_t AlarmLimitsRequest_msg;
extern const pb_msgdesc_t LogEvent_msg;
extern const pb_msgdesc_t ExpectedLogEvent_msg;
extern const pb_msgdesc_t NextLogEvents_msg;
extern const pb_msgdesc_t ActiveLogEvents_msg;
extern const pb_msgdesc_t AlarmMute_msg;
extern const pb_msgdesc_t AlarmMuteRequest_msg;
extern const pb_msgdesc_t MCUPowerStatus_msg;
extern const pb_msgdesc_t BackendConnections_msg;
extern const pb_msgdesc_t ScreenStatus_msg;
extern const pb_msgdesc_t Ping_msg;
extern const pb_msgdesc_t Announcement_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define SensorMeasurements_fields &SensorMeasurements_msg
#define CycleMeasurements_fields &CycleMeasurements_msg
#define Parameters_fields &Parameters_msg
#define ParametersRequest_fields &ParametersRequest_msg
#define Range_fields &Range_msg
#define AlarmLimits_fields &AlarmLimits_msg
#define AlarmLimitsRequest_fields &AlarmLimitsRequest_msg
#define LogEvent_fields &LogEvent_msg
#define ExpectedLogEvent_fields &ExpectedLogEvent_msg
#define NextLogEvents_fields &NextLogEvents_msg
#define ActiveLogEvents_fields &ActiveLogEvents_msg
#define AlarmMute_fields &AlarmMute_msg
#define AlarmMuteRequest_fields &AlarmMuteRequest_msg
#define MCUPowerStatus_fields &MCUPowerStatus_msg
#define BackendConnections_fields &BackendConnections_msg
#define ScreenStatus_fields &ScreenStatus_msg
#define Ping_fields &Ping_msg
#define Announcement_fields &Announcement_msg

/* Maximum encoded size of messages (where known) */
#define ActiveLogEvents_size                     192
#define AlarmLimitsRequest_size                  347
#define AlarmLimits_size                         347
#define AlarmMuteRequest_size                    10
#define AlarmMute_size                           21
#define Announcement_size                        77
#define BackendConnections_size                  4
#define CycleMeasurements_size                   41
#define ExpectedLogEvent_size                    12
#define LogEvent_size                            132
#define MCUPowerStatus_size                      7
#define NextLogEvents_size                       294
#define ParametersRequest_size                   50
#define Parameters_size                          50
#define Ping_size                                17
#define Range_size                               22
#define ScreenStatus_size                        2
#define SensorMeasurements_size                  47

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
}  // namespace Pufferfish::Application

/* Message descriptors for nanopb */
namespace nanopb {

// using namespace is the simplest way to make the message declarations available in this section
using namespace Pufferfish::Application;

template <>
struct MessageDescriptor<SensorMeasurements> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 8;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &SensorMeasurements_msg;
    }
};
template <>
struct MessageDescriptor<CycleMeasurements> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 7;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &CycleMeasurements_msg;
    }
};
template <>
struct MessageDescriptor<Parameters> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 10;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &Parameters_msg;
    }
};
template <>
struct MessageDescriptor<ParametersRequest> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 10;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &ParametersRequest_msg;
    }
};
template <>
struct MessageDescriptor<Range> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &Range_msg;
    }
};
template <>
struct MessageDescriptor<AlarmLimits> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 15;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &AlarmLimits_msg;
    }
};
template <>
struct MessageDescriptor<AlarmLimitsRequest> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 15;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &AlarmLimitsRequest_msg;
    }
};
template <>
struct MessageDescriptor<LogEvent> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 15;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &LogEvent_msg;
    }
};
template <>
struct MessageDescriptor<ExpectedLogEvent> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &ExpectedLogEvent_msg;
    }
};
template <>
struct MessageDescriptor<NextLogEvents> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 5;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &NextLogEvents_msg;
    }
};
template <>
struct MessageDescriptor<ActiveLogEvents> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 1;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &ActiveLogEvents_msg;
    }
};
template <>
struct MessageDescriptor<AlarmMute> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 4;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &AlarmMute_msg;
    }
};
template <>
struct MessageDescriptor<AlarmMuteRequest> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 3;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &AlarmMuteRequest_msg;
    }
};
template <>
struct MessageDescriptor<MCUPowerStatus> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &MCUPowerStatus_msg;
    }
};
template <>
struct MessageDescriptor<BackendConnections> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &BackendConnections_msg;
    }
};
template <>
struct MessageDescriptor<ScreenStatus> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 1;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &ScreenStatus_msg;
    }
};
template <>
struct MessageDescriptor<Ping> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &Ping_msg;
    }
};
template <>
struct MessageDescriptor<Announcement> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static PB_INLINE_CONSTEXPR const pb_msgdesc_t* fields() {
        return &Announcement_msg;
    }
};
}  // namespace nanopb

#endif  /* __cplusplus */


#endif
