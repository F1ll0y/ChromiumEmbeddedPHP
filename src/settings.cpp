// Copyright (c) 2012-2014 The PHP Desktop authors. All rights reserved.
// License: New BSD License.
// Website: http://code.google.com/p/phpdesktop/

#include "defines.h"
#include "executable.h"
#include "file_utils.h"
#include "json.h"
#include "log.h"

std::string g_applicationSettingsError = "";

json_value* GetApplicationSettings() {
    static json_value* ret = new json_value();
    static bool settings_fetched = false;
    if (settings_fetched)
        return ret;
    settings_fetched = true;

    std::string settingsFile = GetExecutableDirectory() + "\\settings.json";

    PHP_DESKTOP_LOG_DEBUG << "Fetching settings from settings.json file(" << settingsFile << ")";
    std::string contents = GetFileContents(settingsFile);
    if (contents.empty()) {
        // TODO create default settings.json
        PHP_DESKTOP_LOG_WARNING << "Error while opening the settings.json file";
        g_applicationSettingsError = "Error while opening the settings file: " + settingsFile;
        return ret;
    }

    json_settings settings;
    memset(&settings, 0, sizeof(json_settings));
    char error[256];
    json_value* json_parsed = json_parse_ex(&settings, contents.c_str(),
                                            &error[0]);
    if (json_parsed == 0) {
        PHP_DESKTOP_LOG_WARNING << "Error while parsing settings.json file: " << error;
        g_applicationSettingsError = "Error while parsing the settings file.";
        return ret;
    }
    ret = json_parsed;
    return ret;
}

std::string GetApplicationSettingsError() {
    return g_applicationSettingsError;
}
