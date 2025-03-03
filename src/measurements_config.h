#ifndef MEASUREMENTS_CONFIG_H
#define MEASUREMENTS_CONFIG_H

#include "measurements.h"

// Definiere die Messwerte
Measurement measurements[] = {
    {32100, 1, "battery/voltage", 0.01, 300000, 0, 0.0, "", false, 1, false, 0},
    {32101, 1, "battery/current", 0.01, 300000, 0, 0.0, "", true, 1, false, 0},
    {32102, 2, "battery/power", 1.0, 10000, 0, 0.0, "", true, 0, false, 0},
    {32104, 1, "battery/soc", 1.0, 60000, 0, 0.0, "", false, 0, false, 0},
    {32105, 1, "battery/energy", 0.001, 30000, 0, 0.0, "", false, 3, false, 0},
    {32200, 1, "ac/voltage", 0.1, 300000, 0, 0.0, "", false, 1, false, 0},
    {32201, 1, "ac/current", 0.01, 300000, 0, 0.0, "", true, 1, false, 0},
    {32202, 2, "ac/power", 1.0, 10000, 0, 0.0, "", true, 0, false, 0},
    {32300, 1, "ac_offgrid/voltage", 0.01, 300000, 0, 0.0, "", false, 1, false, 0},
    {32301, 1, "ac_offgrid/current", 0.01, 300000, 0, 0.0, "", true, 1, false, 0},
    {32302, 2, "ac_offgrid/power", 1.0, 10000, 0, 0.0, "", true, 0, false, 0},
    {33000, 2, "total_charging_energy", 0.01, 300000, 0, 0.0, "", false, 2, false, 0},
    {33002, 2, "total_discharging_energy", 0.01, 300000, 0, 0.0, "", false, 2, false, 0},
    {33004, 2, "daily_charging_energy", 0.01, 300000, 0, 0.0, "", false, 2, false, 0},
    {33006, 2, "daily_discharging_energy", 0.01, 300000, 0, 0.0, "", false, 2, false, 0},
    {33008, 2, "monthly_charging_energy", 0.01, 300000, 0, 0.0, "", false, 2, false, 0},
    {33010, 2, "monthly_discharging_energy", 0.01, 300000, 0, 0.0, "", false, 2, false, 0},
    {35000, 1, "internal_temperature", 0.1, 600000, 0, 0.0, "", true, 1, false, 0},
    {35001, 1, "internal_mos1_temperature", 0.1, 600000, 0, 0.0, "", true, 1, false, 0},
    {35002, 1, "internal_mos2_temperature", 0.1, 600000, 0, 0.0, "", true, 1, false, 0},
    {35010, 1, "max_cell_temperature", 0.1, 600000, 0, 0.0, "", true, 1, false, 0},
    {35011, 1, "min_cell_temperature", 0.1, 600000, 0, 0.0, "", true, 1, false, 0},
    {35100, 1, "inverter_state", 1.0, 5000, 0, 0.0, "", false, 0, false, 0, {{0, "sleep"}, {1, "standby"}, {2, "charge"}, {3, "discharge"}, {4, "backup mode"}, {5, "OTA upgrade"}}},
    {35110, 1, "charge_voltage_limit", 0.1, 300000, 0, 0.0, "", false, 1, false, 0},
    {35111, 1, "charge_current_limit", 0.1, 10000, 0, 0.0, "", false, 1, false, 0},
    {35112, 1, "discharge_current_limit", 0.1, 10000, 0, 0.0, "", false, 1, false, 0},
    {42000, 1, "rs485_control_mode", 1.0, 10000, 0, 0.0, "", false, 0, false, 0, {{21947, "deaktiviert"}, {21930, "aktiviert"}}},
    {42010, 1, "force_charge_discharge_mode", 1.0, 10000, 0, 0.0, "", false, 0, false, 0},
    {42011, 1, "charge_to_soc", 1.0, 10000, 0, 0.0, "", false, 0, false, 0},
    {42020, 1, "forcible_charge_power", 1.0, 10000, 0, 0.0, "", false, 0, false, 0},
    {42021, 1, "forcible_discharge_power", 1.0, 10000, 0, 0.0, "", false, 0, false, 0},
    {44000, 1, "charging_cutoff_capacity", 0.1, 60000, 0, 0.0, "", false, 1, false, 0}, // Prozentwert
    {44001, 1, "discharging_cutoff_capacity", 0.1, 60000, 0, 0.0, "", false, 1, false, 0}, // Prozentwert
    {44002, 1, "max_charge_power", 1.0, 10000, 0, 0.0, "", false, 0, false, 0}, // Watt
    {44003, 1, "max_discharge_power", 1.0, 10000, 0, 0.0, "", false, 0, false, 0}, // Watt
    {31000, 10, "device_name", 1.0, 300000, 0, 0.0, "", false, 0, true, 20} // Textwert
};

const size_t numMeasurements = sizeof(measurements) / sizeof(measurements[0]);

#endif // MEASUREMENTS_CONFIG_H