#pragma once

#include "trait.h"
#include <type_traits>

#ifdef _WIN32
#include <vcruntime.h>
#include <windows.h>
#endif

#include <filesystem>
#include <format>
#include <functional>
#include <string>
#include <thread>
#include <vector>

extern auto temp_file() -> std::string;
extern auto clean() -> void;

// const RUSTC_COLOR_ARGS : &[&str] = &[ "--color", "always" ];
// const RUSTC_EDITION_ARGS : &[&str] = &[ "--edition", "2021" ];
const char  *I_AM_DONE_REGEX = "(?m)^\s*///?\s*I\s+AM\s+NOT\s+DONE";
const size_t THE_CONTEXT     = 2;
// const CLIPPY_CARGO_TOML_PATH : &str = "./exercises/clippy/Cargo.toml";
const char *COMPILER = "clang++";

struct Exercise;


enum class Mode
{
    Compile,
    Test,
    Tidy,
};

struct ExerciseOutput
{
    ExerciseOutput(std::string stdout_, std::string stderr_) : _stdout(std::move(stdout_)), _stderr(std::move(stderr_)) {}
    std::string _stdout;
    std::string _stderr;
};


struct ExerciseList
{
    std::vector<Exercise> exercises;
};



struct ContextLine
{
    std::string line;
    size_t      number;
    bool        important;
};

enum class State
{
    Done,
    Pending,
    // TODO: Pending(Vec<ContextLine>)
};


struct FileHandle : mtd::Drop<FileHandle>
{
    auto drop() -> void override
    {
        clean();
    }
};



struct CompiledExercise : mtd::Runable<CompiledExercise>
{
    CompiledExercise(Exercise *exercise, FileHandle handle) : exercise(exercise), handle(std::move(handle)) {}
    Exercise  *exercise;
    FileHandle handle;

    auto run() -> std::any override;
};


struct Exercise : mtd::Runable<Exercise>
{
    std::string name;
    std::string path;
    Mode        mode;
    std::string hint;

    auto run() -> std::any override;
    auto compile() -> mtd::Result<CompiledExercise, ExerciseOutput>;
};