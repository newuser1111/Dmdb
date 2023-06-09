#pragma once
#include <unistd.h>
#include <string.h>


#include <string>
#include <queue>

namespace Dmdb {

class DmdbCommand;
class DmdbServerLogger;
class DmdbClientManager;
class DmdbReplicationManager;

struct DmdbClientContactRequiredComponent {
    DmdbServerLogger* _server_logger;
    DmdbClientManager* _client_manager;
    DmdbReplicationManager* _repl_manager;
    bool _is_myself_master;
};

/* We use _client_staus & ClientStatus to get client's status */
enum class ClientStatus{
    CLOSE_AFTER_REPLY = 1
};


class DmdbClientContact
{
public:
    DmdbClientContact(int fd, const std::string &ip, int port);
    ~DmdbClientContact();
    std::string GetClientName();
    int GetClientSocket();
    void AddReplyData2Client(const std::string &replyData);
    void AppendDataToInputBuf(char* data);
    size_t GetInputBufLength();
    const char* GetOutputBuf();
    size_t GetOutputBufLength();
    void ClearRepliedData(size_t repliedLen);
    bool ProcessOneMultiProtocolRequest(size_t &startPos);
    bool ProcessClientRequest();
    void SetChecked();
    bool IsChecked();
    void SetMultiState(bool state);
    void SetStatus(uint32_t status);
    uint32_t GetStatus();
    bool IsMultiState();
    DmdbCommand* PopCommandOfExec();
    std::string GetIp();
    int GetPort();
    size_t GetMultiQueueSize();

private:
    void ClearProcessedData();
    int _client_socket;
    std::string _client_ip;
    int _client_port;
    std::string _client_name;
    std::string _client_input_buffer;
    size_t _process_pos_of_input_buf;
    std::string _client_output_buffer;
    uint32_t _client_status;
    /* DmdbCommand* will be destructed immediately after DmdbCommand executed rather than
     * after ~DmdbClientContact() executed */
    DmdbCommand*  _current_command = nullptr;
    std::queue<DmdbCommand*> _exec_command_queue;
    bool _is_chekced;
    bool _is_multi_state;
};

}