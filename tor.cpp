#include "tor.h"
#include <jni.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <vector>
#include <string>
#include <android/log.h>

using namespace std;

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "TOR_MANAGER", __VA_ARGS__)

extern "C" int tor_run_main(const tor_main_configuration_t* cfg);
extern "C" tor_main_configuration_t* tor_main_configuration_new(void);
extern "C" int tor_main_configuration_set_command_line(tor_main_configuration_t* cfg, int argc, char* argv[]);

vector<pid_t> active_tor_pids;

const char* c_base_dir = env->GetStringUTFChars(base_dir, nullptr);

string data_dir = string(c_base_dir) + "/tor_data_" + to_string(instance_id);

env->ReleaseStringUTFChars(base_dir, c_base_dir);

pid_t pid = fork();

if (pid < 0) {
    LOGI("Ошибка выполнения fork()");
    return -1;
}

if (pid == 0) {

    string s_port = to_string(socks_port);
    string c_port = to_string(control_port);

    vector<string> args = {
    "tor",
    "--SocksPort", s_port,
    "--ControlPort", c_port,
    "--DataDirectory", data_dir,
    "--RunAsDaemon", "0"
    };

    tor_main_configuration_t* cfg = tor_main_configuration_new();
    tor_main_configuration_set_command_line(cfg, argv.size(), argv.data());

    int res = tor_run_main(cfg);

    _exit(res);
}
else {
    active_tor_pids.push_back(pid);
    LOGI("Динамически запущен Tor с PID: %d на порту %d", pid, socks_port);
    return pid;
}
}
extern "C" JNIEXPORT void JNICALL
Java_com_your_torapp_TorManager_stopInstance(JNIEnv* env, jobject thiz, jint pid) {
    kill(pid, SIGKILL);
}

