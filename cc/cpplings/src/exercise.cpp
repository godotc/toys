#include "exercise.h"
#include <command.h>
#include <cstdlib>
#include <optional>
#include <stdio.h>




auto temp_file() -> std::string
{
    unsigned int thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
#ifdef _WIN32
    auto process_id = GetCurrentProcessId();
#else
    auto process_id = getpid();
#endif
    return std::format("./temp_{}_{}", process_id, thread_id);
}

auto clean() -> void
{
    auto _ignored = std::filesystem::remove(temp_file());
}

auto Exercise::compile() -> mtd::Result<CompiledExercise, ExerciseOutput>
{
    Command cmd;

    switch (mode) {
    case Mode::Compile:
        cmd = Command::New(COMPILER)
                  .args({path, "-o", temp_file()})
                  .exec();
        break;
    // TODO: test mode(with gtest add the unit test file in test dir? or in the same dir of file)
    case Mode::Test:
    case Mode::Tidy:
        break;
    };


    if (cmd.status.success()) {
        return {CompiledExercise(this, {})};
    }
    else {
        return {ExerciseOutput(
            cmd.stdout_,
            cmd.stderr_)};
    }
}

auto Exercise::run() -> std::any
{
}

auto CompiledExercise::run() -> std::any
{
    if (exercise) {
        return exercise->run();
    }
    return printf("Execerise is nullptr!\n");
}
