#pragma once

#include <string>
#include <algorithm>

#ifdef ARDUINO
#include "esphome/core/log.h"
#else
#include <iostream>
#define ESP_LOGE(tag, msg) std::cerr << "[" << tag << "] ERROR: " << msg << std::endl
#endif

/**
 * Saves a name associated with an ID to a two-part database string.
 * Sanitizes the name by replacing '=' and ';' with '_'.
 *
 * @param id_str The identifier (e.g., finger ID as string or NFC UID).
 * @param name The name to associate with the ID.
 * @param db1 Reference to the first database string (global).
 * @param db2 Reference to the second database string (global).
 * @param log_tag Tag used for ESP_LOGE if storage is full.
 */
inline void save_name_to_db(const std::string& id_str, std::string name, std::string& db1, std::string& db2, const char* log_tag) {
    if (name.empty()) {
        return;
    }

    // Sanitize name: replace delimiters '=' and ';' with '_'
    std::replace(name.begin(), name.end(), '=', '_');
    std::replace(name.begin(), name.end(), ';', '_');

    // Build the entry string: "ID=Name;"
    std::string entry;
    entry.reserve(id_str.length() + name.length() + 2);
    entry += id_str;
    entry += "=";
    entry += name;
    entry += ";";

    // Storage management with 250 character limit per string (as per yaml logic)
    if (db1.length() + entry.length() <= 250) {
        db1 += entry;
    } else if (db2.length() + entry.length() <= 250) {
        db2 += entry;
    } else {
        ESP_LOGE(log_tag, "Speicher voll!");
    }
}
