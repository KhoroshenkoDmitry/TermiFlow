#include <string>
#include <vector>
#include "test_framework.hpp"
#include "theme_manager.hpp"

// Helper function for theme validation
bool isValidTheme(const std::string& theme) {
    return theme == "light" || theme == "dark" || theme == "auto";
}

// Test valid theme names
TEST(Theme_ValidThemeNamesAccepted) {
    ASSERT_TRUE(isValidTheme("light"));
    ASSERT_TRUE(isValidTheme("dark"));
    ASSERT_TRUE(isValidTheme("auto"));
}

// Test invalid theme names rejected
TEST(Theme_InvalidThemeNamesRejected) {
    ASSERT_FALSE(isValidTheme("invalid"));
    ASSERT_FALSE(isValidTheme(""));
    ASSERT_FALSE(isValidTheme("LIGHT"));  // case-sensitive
}

// Test theme parsing edge cases
TEST(Theme_CaseSensitivity) {
    auto checkTheme = [](const std::string& theme) {
        return theme == "light" || theme == "dark";
    };
    
    ASSERT_FALSE(checkTheme("Light"));
    ASSERT_FALSE(checkTheme("DARK"));
    ASSERT_TRUE(checkTheme("light"));
}

// Regression test for #80: an unsupported theme must report failure to the caller
// so the invalid value is not saved and no success message is shown
TEST(Theme_ChangeThemeReportsInvalidThemeAsFailure) {
    ASSERT_FALSE(changeTheme("invalid"));
    ASSERT_FALSE(changeTheme(""));
    ASSERT_FALSE(changeTheme("LIGHT"));  // case-sensitive
}
