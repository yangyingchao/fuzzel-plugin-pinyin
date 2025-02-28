extern "C" {
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

void __attribute__((constructor)) pinyin_init();
const char *fuzzel_plugin_translate(const char *input);
}

#include <algorithm>
#include <cpp-pinyin/G2pglobal.h>
#include <cpp-pinyin/Pinyin.h>
#include <cstring>
#include <filesystem>

std::unique_ptr<Pinyin::Pinyin> instance;

void pinyin_init() {
    std::filesystem::path path = getenv("HOME");
    path /= ".local/share/cpp-pinyin/dict";
    Pinyin::setDictionaryPath(path);
    instance = std::make_unique<Pinyin::Pinyin>();
}

const char *fuzzel_plugin_translate(const char *input) {
    if (std::all_of(input, input + strlen(input),
                    [](char c) { return static_cast<unsigned char>(c) <= 127; })) {
        return input;
    }

    static std::string res;
    res = instance->hanziToPinyin(input, Pinyin::ManTone::Style::NORMAL, Pinyin::Error::Default, false, true)
              .toStdStr();

    return res.c_str();
}
