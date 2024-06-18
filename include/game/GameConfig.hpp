#pragma once

#include <IniCM/IniConfManager.hpp>
#include <IniCM/IniConfEntry.hpp>

#include "utils/PathUtils.hpp"

namespace conf {
    IniConfEntry::Integer windowWidth("WindowWidth", "Determines the x resolution of the window", 1000);
    IniConfEntry::Integer windowHeight("WindowHeight", "Determines the y resolution of the window", 1000);

    IniConfEntry::Boolean fullscreen("Fullscreen", "Whether the window should be fullscreen", false);

    void init() {
        IniConfManager manager(PathUtils::absolutePath("settings.ini"));    

        manager.addEntry(&windowWidth);
        manager.addEntry(&windowHeight);
        manager.addEntry(&fullscreen);

        manager.build();
    }
};
