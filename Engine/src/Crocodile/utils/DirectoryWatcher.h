#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>



namespace fs = std::filesystem;

namespace Crocodile
{

    class DirectoryWatcher {

    public:
        DirectoryWatcher(const std::string& path_to_watch, std::chrono::duration<int, std::milli> delay)
            : path_to_watch(path_to_watch), delay(delay)
        {
            for (const auto& file : fs::recursive_directory_iterator(path_to_watch))
                paths_[file.path().string()] = fs::last_write_time(file);
        }

        void start(const std::function<void(std::string)>& action) {
            running_ = true;
            watcher_thread = std::thread([this, action]() {
                while (running_) {
                    std::this_thread::sleep_for(delay);
                    for (const auto& file : fs::recursive_directory_iterator(path_to_watch)) {
                        auto current_file_last_write_time = fs::last_write_time(file);
                        if (contains(file.path().string()))
                        {
                            if (paths_[file.path().string()] != current_file_last_write_time)
                            {
                                paths_[file.path().string()] = current_file_last_write_time;
                                action(file.path().string());
                            }
                        }
                        else
                        {
                            paths_[file.path().string()] = current_file_last_write_time;
                        }
                    }
                }
                });
        }

        void stop() {
            running_ = false;
            if (watcher_thread.joinable()) {
                watcher_thread.join();
            }
        }

    private:
        std::unordered_map<std::string, fs::file_time_type> paths_;
        std::string path_to_watch;
        std::chrono::duration<int, std::milli> delay;
        std::atomic<bool> running_{ false };
        std::thread watcher_thread;

        bool contains(const std::string& key) const {
            return paths_.find(key) != paths_.end();
        }
    };

}
