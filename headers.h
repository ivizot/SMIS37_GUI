#ifndef headers_h
#define headers_h

#include <stdint.h>
//#include <Arduino.h>

//#define WORD(hb, lb) ((hb << 8) + lb)

#define TScount 12
#define FScount 12
#define PScount 4
typedef struct __attribute__((packed))
{
    uint16_t T[TScount]; // Temp; 0-100.0 degC -> 0-10000, 0 == offline
    uint16_t F[FScount]; // Flow; 0-60 l/min -> 0-6000, no offline indication
    uint16_t P[PScount]; // Pressure; 0-5V -> 0-1023
    // water module status register
    // 0 online
    uint8_t state;
} TWater; //57 bytes

//#define _MAXT_OUTPUT 6000 // maximum allowed output temp
//#define _MAXT_INPUT 3000  // maximum allowed input temp

//temperature sensors index
#define T_GCOLLECTOR 0 // gyrotron collector [0x28,0x9c,0x7d,0xbe,0x1e,0x19,0x01,0x69]
#define T_GCAVITY 1     // gyrotron cavity [0x28,0xef,0x5a,0xe0,0x20,0x19,0x01,0xe6]
#define T_COIL1 2       // Coil 1 [0x28,0x2f,0x50,0xe1,0x20,0x19,0x01,0x4b]
#define T_COIL2 3       // Coil 2 [0x28,0x77,0xb4,0x5b,0x1c,0x19,0x01,0xee]
#define T_GHVPS 4       // GHVPS [0x28,0xe6,0x69,0x02,0x21,0x19,0x01,0x5f]
#define T_MFPS 5        // MFPS [0x28,0xdf,0x66,0xee,0x20,0x19,0x01,0xbb]
#define T_ANALYZER 6    // Magnetic analyzer [0x28,0x63,0x2b,0x79,0x97,0x18,0x03,0x1d]
#define T_CALORIMETER 7 // Calorimeter [0x,0x,0x,0x,0x,0x,0x,0x]
#define T_CHAMBER 8     // Plasma chamber [0x,0x,0x,0x,0x,0x,0x,0x]
#define T_EXT 9         // Extraction [0x28,0xe8,0xd0,0x79,0x97,0x21,0x03,0x46]
#define T_10 10         // SPARE [0x,0x,0x,0x,0x,0x,0x,0x]
#define T_INPUT 11      // input [0x28,0x56,0x9a,0x09,0x1f,0x19,0x01,0x0e]

//flow sensors index
#define F_GCOLLECTOR 10 // gyrotron collector
#define F_GCAVITY 11    // gyrotron cavity
#define F_COIL1 8       // Coil 1
//#define F_COIL2 8       // Coil 2
#define F_GHVPS 6       // GHVPS
#define F_MFPS 5        // MFPS
#define F_ANALYZER 6    // Magnetic analyzer
#define F_CALORIMETER 7 // Calorimeter
#define F_CHAMBER 8     // Plasma chamber
#define F_EXT 9         // Extraction
//#define F_ 10      //
//#define F_ 11      //

//pressure sensors index
#define P_HIGH 0 // High side
//#define P_LOW 1  // Low side
//#define P_ 2  //
//#define P_ 3     //

// ************************************* Pulse generator data
typedef struct __attribute__((packed))
{
    uint16_t T[8]; // T0 of pulse, *10 us
    uint16_t D[8]; // duration of pulse, *10 us
    uint16_t P;    // pulse period, ms
    uint8_t S;     // channels state (ON/OFF)
    uint32_t N;    // pulse number

    bool TIMER;  // timer state
    uint16_t dT; //time to next pulse, ms
} TPulseGen; //42 bytes
// ************************************* /Pulse generator data

// ************************************* GSPS data
typedef struct __attribute__((packed))
{
    uint16_t I;    // current, mA
    uint16_t Iset; // current setpoint, mA
    uint16_t U;    // voltage, mV
    uint16_t RS;   //ramp speed, mA/sec
    // GSPS Status
    // [0] online/offline
    // [1] output ON/OFF
    // [2] ERROR
    // [3] ramping
    uint8_t state;
} TGSPS; // 9 bytes
//#define GSPS_MIN_CURRENT 10000 // minimum current to enable further devices
// ************************************* /GSPS data

// ************************************* GFPS data
typedef struct __attribute__((packed))
{
    // 0 LOAD relay
    // 1 interlock IN
    // 2 interlock OUT
    // 3 mode: 0 ramp power, 1 - direct control
    // 4 OVR: current overrange
    // 5 TRIAC fault
    // 6 device online (local state)
    uint8_t state;
    uint16_t Pset; // power setpoint, 10000 = 100.00%
    uint16_t Pint; // interlock out trigger value, 10000 = 100.00%
    uint16_t Pcur; // current power, 10000 = 100.00%
    uint16_t Ramp; // power ramp rate, 10000 = 100% per second
    uint16_t Irms; // current Irms; 1000 = 1 A
    uint16_t A;    // I zero offset; 512
    uint16_t B;    // calibration, mV/A;
    int16_t C;     // zero offset in Irms; 1000 = 1 A;
} TGFPS; // 17 bytes
// ************************************* /GFPS data

// GHVPS data
typedef struct __attribute__((packed))
{
    /*
Charging unit
REG R/W range       Desc
0   R               state and interlocks per-bit
                                    0 Overtemperature
                                    1 Target voltage achieved; READY
                                    2 Charging
                                    3 DEBUG mode
                                    4 MAINS power lost
                                    5 External interlock 
                                    6 Internal interlock
                                    7 Rectifier not ready
1   R               errors per-bit
                                    0 no-load 
                                    1 Too long charge
                                    2 Software Overvoltage
                                    3 Capacitors breakdown
                                    4 IGBT1 short-circuit
                                    5 IGBT2 short-circuit
                                    6 Hardware overvoltage
                                    7 Capacitors short-circuit
                                    8 Rectifier malfunction(or power lost) during charge/maintain
                                    9 Internal interlock triggered during charge/maintain
                                    10 External interlock triggered during charge/maintain
                                    11 Overtemperature during charge/maintain
2   R   0-10000     Output voltage, V
3   R   1-999       Serial number
4   R   0-4000      Output current, mA
5   R/W 500-7500    Output voltage setpoint, V
6   R/W 500-7800    Cutoff voltage, V
7   R/W 1-7         Output power
8   R/W 0-1         Output, on-off
9   R/W 0-1         Error reset
10  R/W 0-1         Control - NOT USED
11  R/W 9800-10200  ADC correction - NOT USED
*/
    uint16_t ZU[12];
    /*
Control unit
REG R/W range       Desc
0   R   0-2048      State and interlocs per-bit:
                        0 Internal interlock
                        1 External interlock
                        2 Demagnetization power is on
                        3 HV switch is ready
                        4 CHVPS is ready for switching on
                        5 Contactor is ON
                        6 ZU is ready
                        7 ZU is charging
                        8 ZU capacitors are ready
                        9 HVswitch is permitted to work
                        10 CHVPS is ready for HVswitch
1   R               Faults per-bit
                        0 Internal interlock engaged after contactor is on
                        1 External interlock engaged after contactor is on
                        2 Demagnetization power lost
                        3 Contactor error
                        4 ZU error
                        5 Fast current protection
                        6 HVswitch ready signal lost
                        7 COMPI1 error
                        8 COMPI2 error
                        9 any of 0-8 happened
2   R/W 0-1         Contactor control
3   W   0-1         Test pulse
4   W   0-1         Error reset
*/
    uint16_t BU[3];
    bool online;
} TGHVPS; //31 bytes

//GIP data
#define GIP_KU 10.2914 // V/ADC; 3 kV = 281 ADC; 4 kV = 392 ADC; 4.4 kV = 427; 2.4 kV = 240 ADC; U[V] = ADC*KU; max ~5000
#define GIP_KI 4.8828  // uA/ADC  1024 ADC = 5V = 5mA; I[uA] = ADC*KI; max 5000
typedef struct __attribute__((packed))
{
    uint16_t U;               // Voltage, V
    uint16_t I;               // current, uA
    uint8_t latchedInterlock; // see TData for details

    // status byte
    // 0 power 0= OFF, 1 = ON
    // 1 low voltage
    // 2 high current -> bad vacuum
    // 3
    uint8_t S;
} TGIP; // 6 bytes

//SPELLMAN data
typedef struct __attribute__((packed))
{
    uint16_t UC_URset; // voltage_ramp_setted = 0; //ms ramp
    uint16_t UC_IRset; // current_ramp_setted = 0;

    uint16_t Uset; // Voltage setpoint [V] //voltage_setpoint_setted
    uint16_t Iset; // Current setpoint [mA] //current_setpoint_setted

    uint16_t Usetreq; // voltage_setpoint_requested
    uint16_t Isetreq; // current_setpoint_requested

    uint16_t OVP; // misc_analog_monitor_readbacks_remote_over_voltage_protection = 0;
    //double misc_analog_monitor_readbacks_spare_arg_2 = 0;
    uint16_t PR;  //  misc_analog_monitor_readbacks_power_reference = 0;
    uint16_t PM;  // misc_analog_monitor_readbacks_power_monitor = 0;
    uint16_t CBT; // double misc_analog_monitor_readbacks_control_board_temperature = 0;
    //double misc_analog_monitor_readbacks_spare_arg_6 = 0;
    //double misc_analog_monitor_readbacks_spare_arg_7 = 0;
    //double misc_analog_monitor_readbacks_spare_arg_8 = 0;

    // 0 power_on
    // 1 hv_ray_on
    // 2 arc
    // 3 interlock_closed
    // 4 over_current
    // 5 over_power
    // 6 over_voltage
    // 7 voltage_control_mode
    uint8_t S1;

    // 0 system_fault
    // 1 regulation_error
    // 2 current_control_mode
    // 3 over_temperature
    // 4 power_control_mode
    // 5 ac_fault
    // 6 local_remote_mode
    // 7 lvps_fault
    uint8_t S2;

    // 0 hv_inhibit
    // 1 timeout
    // 2 online
    // 3
    // 4
    // 5
    // 6
    // 7
    uint8_t S3;

    uint16_t UC_URreq; // request_user_configurations_voltage_rate_ramp = 0; ms
    uint16_t UC_IRreq; // request_user_configurations_current_rate_ramp = 0; ms
    uint16_t FSU;      // request_unit_scaling_full_scale_voltage = 100;
    uint16_t FSI;      // request_unit_scaling_full_scale_current = 120;
    uint16_t UM;       // request_voltage_monitor = 0;
    uint16_t IM;       // double request_current_monitor = 0;

    uint8_t SF;
    // 0 byte request_slave_faults_1 = 0;
    // 1 byte request_slave_faults_2 = 0;
    // 2 byte request_slave_faults_3 = 0;
    // 3 byte request_slave_faults_4 = 0;
    // 4 byte request_slave_faults_5 = 0;
    // 5 byte request_slave_faults_6 = 0;
    // 6 byte request_slave_faults_7 = 0;
    // 7 byte request_slave_faults_8 = 0;
    // NOT USING: byte request_slave_faults_9 = 0;

    uint8_t request_local_remote_mode;
    char error_code_char;     // ???
    uint8_t error_command_byte; // заголовочный байт команды, которая привела к ошибке
} TSPELLMAN; //39 bytes

// interlock OUTPUT bits in PORTC
#define IBO_SPELLMAN 5
#define IBO_GFPS 4
#define IBO_GSPS 3
#define IBO_MFPS 2
#define IBO_GHVPS 1
#define IBO_5 0
#define IBO_6 6
#define IBO_LAMP 7

// interlock INPUT bits in PORTA
#define IBI_GFPS 0  // GFPS power is > MIN
#define IBI_CAGE0 1 // SMIS cage
#define IBI_CAGE1 2 // gyrotron cage
#define IBI_CAGE2 3 // HV cage
#define IBI_CAGE3 4 // HV room

// latched interlocks; if no bits are set, device operation is granted; bits reset by external command only
// LSB  0   LI_WT: water temp
//      1   LI_WF: water flow
//      2   LI_ES: endstops (cages etc)
//      3   LI_DD: dependent device not ready
//      4
//      5
//      6
// MSB  7

#define LI_WT 0b00000001 // water temp error
#define LI_WF 0b00000010 // water flow error
#define LI_ES 0b00000100 // endstops error
#define LI_DD 0b00001000 // linked device not ready

// EEPROM array
typedef struct __attribute__((packed))
{
    uint8_t IP[4];
    uint16_t T[TScount];       // max temp for each sensor
    uint16_t F[FScount];       // min flow for each sensor
    uint16_t P[PScount];       // min pressure for each sensor
    uint16_t GSPS_MIN_CURRENT; // minimum current to enable further devices, mA
    uint16_t GIP_MIN_U;        // minimum GIP voltage, V
    uint16_t GIP_MAX_I;        // max GIP current, uA
} TEEPROM;

//#define EEPROM_

typedef struct __attribute__((packed))
{
    TWater waterData; //57
    TPulseGen pulseGenData; //42
    TGSPS GSPSData; //9
    TGIP GIPData; //6
    TGFPS GFPSData; //17

    TGHVPS GHVPSData; //31

    TSPELLMAN spellmanData; // 39

    uint8_t GUlast; //average gyrotron voltage in last pulse
    uint8_t GUdesired; // desired gyrotron voltage
    uint8_t GIlast; // average gyrotron current in last pulse
    uint8_t GIdesired; // desired gyrotron current
    uint8_t MIlast; // average coils current in last pulse

    uint8_t interlocksIn;        // input states; 1 = OPEN; 0 = CLOSED
    uint8_t latchedInterlocksIn; // bit is released (set to 0) only by reset command

    uint8_t interlocksOut;           // Output states; 1 = OPEN; 0 = CLOSED
    uint8_t latchedInterlocksOut[8]; // keeps info on why output is not CLOSED

    // water: latched interlocks
    uint16_t waterTemp;    // bit is set when Temp is not nominal (T>Tmax or T==offline)
    uint16_t waterFlow;    // bit is set when Flow is not nominal (F<Fmin)
    uint8_t waterPressure; // bit is set when P is not nominal (P<Pmin)

} TData;

#define FAST_ADC_COUNT 100 // number of samples for fast ADC
#define FAST_ADC_TPP 23.25 // time per point, us
#define GI_PER_ADC 1; // Amp per ADC
#define GU_PER_ADC 1; // kV per ADC
#define MI_PER_ADC 1; // KAmp per ADC
typedef struct __attribute__((packed))
{
    uint8_t GI[FAST_ADC_COUNT];
    uint8_t GU[FAST_ADC_COUNT];
    uint8_t MI[FAST_ADC_COUNT];
} TDataFast;

// ================================ COMMANDS LIST
// all commands are 3 bytes: [C][high(ARG)][low(ARG)]
// uint8_t C is command id;
// uint16_t ARG = WORD(high(ARG),low(ARG)) - command argument; must be zero if no argument is needed
//
// each command produces an answer!
// If no data was requested, then the answer is: [C][low(ARG)][high(ARG)],
// where C is equal to received command, and ARG is the result (always zero at the moment)
//
// Data requesting commands:
// Request: [COMM_QUERY][0][0] - query all data
// Answer:  [COMM_DATA_SLOW][...TData...] - if no pulse since last sending
//          [COMM_DATA_ALL][...TData...][...TDataFast...] - if there was a pulse
//
// Request: [COMM_EEPROM_QUERY][0][0] - query EEPROM settings
// Answer: [COMM_EEPROM_QUERY][...TEEPROM...]

// === pulse generator
#define COMM_PULSEGEN_1 1
#define COMM_PULSEGEN_QUERY 1     // query all data from pulseGen - INTERNAL USE ONLY
#define COMM_PULSEGEN_SET_S 2     // [0][S] set channels state per-bit of S
#define COMM_PULSEGEN_SET_P 3     // [H][L] set timer period to ARG ms
#define COMM_PULSEGEN_SET_TIMER 4 // [0][V] turn timer OFF(V=0) / ON(V=1)
#define COMM_PULSEGEN_PULSE 5     // [0][0] single pulse now
#define COMM_PULSEGEN_SET_TX 6    // [H][L] set CHANNEL #(C-6) TIME to ARG (i.e. channel 0: [6][HIGH][LOW], channel 7: [13][HIGH][LOW])
#define COMM_PULSEGEN_SET_DX 14   // [H][L] set CHANNEL #(C-14) DURATION to ARG (i.e. channel 0: [6][HIGH][LOW], channel 7: [13][HIGH][LOW])
#define COMM_PULSEGEN_2 29
// === /pulse generator

// === GSPS
#define COMM_GSPS_SET_I 30 // [H][L] set target current to ARG mA
#define COMM_GSPS_SET_R 31 // [H][L] set ramp speed to ARG mA/s
// === /GSPS

// === GFPS
#define COMM_GFPS_1 40
#define COMM_GFPS_QUERY 40 // query all data - INTERNAL USE ONLY
#define COMM_GFPS_OUT 41   // [0][V] TURN ON (PARAM=1) / OFF (PARAM=0)
#define COMM_GFPS_P 42     // [H][L] power setpoint, 10000 = 100.00%
#define COMM_GFPS_R 43     // [H][L] power ramp rate, 10000 = 100% per second
#define COMM_GFPS_M 44     // [0][V] Power mode: V=0 - ramp, V=1 - direct
#define COMM_GFPS_TIP 45   // [H][L] interlock trigger value, 10000 = 100.00%
//#define COMM_GFPS_A 46     // [H][L] I zero offset in ADC counts; 512
//#define COMM_GFPS_B 47     // [H][L] calibration, mV/A;
//#define COMM_GFPS_C 48     // [H][L] zero offset in Irms, mA
#define COMM_GFPS_2 49
// === /GFPS

// === GIP
#define COMM_GIP_QUERY 51 // QUERY data, telnet use only
#define COMM_GIP_OUT 50   // [0][V] Power: V=0 - OFF, V=1 - ON
// === /GIP

// === water
#define COMM_WATER_1 60
#define COMM_WATER_QUERY 60 // query all data - INTERNAL USE ONLY
#define COMM_WATER_2 69
// === /water

// === SPELLMAN
#define COMM_SPELLMAN_1 70
#define COMM_SPELLMAN_QUERY 70   // query all data - INTERNAL USE ONLY
#define COMM_SPELLMAN_ON 71      // power on
#define COMM_SPELLMAN_OFF 72     // power off
#define COMM_SPELLMAN_KV_RATE 73 // set KV rate, ms, max 10000
#define COMM_SPELLMAN_MA_RATE 74 // set mA rate, ms, max 10000
#define COMM_SPELLMAN_KV 75      // set VOLTAGE, 100 kV = 10000
#define COMM_SPELLMAN_MA 76      // set CURRENT, 120 mA = 12000
#define COMM_SPELLMAN_RESET 77   // reset faults
#define COMM_SPELLMAN_HV_ON 78   // HV on
#define COMM_SPELLMAN_HV_OFF 79  // HV off
#define COMM_SPELLMAN_2 79
// === /SPELLMAN

// === GHVPS
#define COMM_GHVPS_1 80
#define COMM_GHVPS_QUERY 80     // QUERY data, internal/telnet use only
#define COMM_GHVPS_SET_U 81     // Output voltage setpoint, V
#define COMM_GHVPS_SET_UCUT 82  // Cutoff voltage, V
#define COMM_GHVPS_SET_POW 83   // Output power, 1-7
#define COMM_GHVPS_CONTACTOR 84 // 0-1 Contactor control
#define COMM_GHVPS_CHARGE 85    // Output, on-off
#define COMM_GHVPS_PULSE 86     // single pulse
#define COMM_GHVPS_RESET_BU 87  // 0-1 Error reset
#define COMM_GHVPS_RESET_ZU 88  // 0-1 Error reset
#define COMM_GHVPS_2 89
// === /GHVPS

// === INTERLOCKS
#define COMM_INTERLOCKS_QUERY 100      // query interlocks status - telnet
#define COMM_INTERLOCKS_OVERRIDE 101   // manually set interlocks state 0 - interlocks auto, 1-manual
#define COMM_INTERLOCKS_OVERRIDE_V 102 // manually set interlocks state: LB = interlocks state
#define COMM_INTERLOCKS_RESET 103      // reset latched interlocks
// === /INTERLOCKS

#define COMM_EEPROM_1 200
#define COMM_EEPROM_QUERY 200 // query EEPROM struct
// [COMMAND][HB(arg)][LB(arg)]
// 201..204: IP
// 5..16: TMAX
// 17..28 FMIN
// 29..32 PMIN
// 33 GSPS_MIN_CURRENT
// 34 GIP_MIN_U
// 35 GIP_MAX_I
// ...
// 49
#define COMM_EEPROM_2 249

#define COMM_QUERY 250     // query all data; returns COMM_DATA+TData struct if no pulse; COMM_ALL+TData+TFastData if there was a pulse
#define COMM_DATA_SLOW 251 // response TData
#define COMM_DATA_ALL 252  // response TData+TFastData

#define COMM_RESET 255 // RESET MCU

#endif
