//
// Created by Arjo Nagelhout on 20/03/2024.
//

#ifndef SHAPEREALITY_APPLICATION_INFO_H
#define SHAPEREALITY_APPLICATION_INFO_H

#include <filesystem>

#ifdef __APPLE__

#include "TargetConditionals.h"

#endif

namespace common
{
    // global directories:

    // log (on macOS ~/Library/Logs/com.organization_name.app_name/)
    // user data (global settings and cached data)

    enum class Platform
    {
        Windows = 0,
        macOS,
        iOS,
        tvOS,
        visionOS,
        Android,
        Linux
    };

    class ApplicationInfo
    {
    public:

        // platform checking using constexpr instead of a plethora of macros
        constexpr static Platform platform = Platform::
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        Windows
#elif __APPLE__
#if TARGET_OS_MAC
        macOS
#elif TARGET_OS_IPHONE
        iOS
#elif TARGET_OS_TV
        tvOS
#elif TARGET_OS_VISION
        visionOS
#endif
#elif __ANDROID__
        Android
#elif __linux__
        Linux
#elif __unix__
        Linux
#elif defined (_POSIX_VERSION)
        Linux
#endif
        ;

        constexpr static std::string_view extension = "com";

        constexpr static std::string_view applicationName = "Engine";

        constexpr static std::string_view organizationName = "ShapeReality";

        // runtime

        [[nodiscard]] static std::string_view applicationIdentifier();

        // converts ~ to the absolute path (only useful on desktop operating systems)
        [[nodiscard]] static std::filesystem::path userHomeDirectory();

        // e.g. on macOS: the loggingDirectory is ~/Library/Logging/com.organizationName.applicationName/
        [[nodiscard]] static std::filesystem::path loggingDirectory();

        // e.g. on Android: the dataDirectory is the app bundle path
        [[nodiscard]] static std::filesystem::path dataDirectory();

        // e.g. on macOS: the persistentDataDirectory is Library/ApplicationData/com.organizationName.applicationName/
        [[nodiscard]] static std::filesystem::path persistentDataDirectory();
    };
}

#endif //SHAPEREALITY_APPLICATION_INFO_H
