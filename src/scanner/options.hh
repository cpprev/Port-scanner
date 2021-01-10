#pragma once

#define CONNECT_TIMEOUT 3

namespace scanner
{
    class Options
    {
    public:
        Options() = default;

        int GetTimeout() const { return _scanTimeout; }
        void SetTimeout(int timeout) { _scanTimeout = timeout; }
        bool IsVerboseEnabled() const { return _verbose; }
        void SetVerbose(bool verbose) { _verbose = verbose; }
        bool IsMultithreadingEnabled() const { return _multithreading; }
        void SetMultithreading(bool multithreading) { _multithreading = multithreading; }
    private:
        bool _verbose = false;
        /// Set it to true by default since port scanning is extremely slow without multithreading
        bool _multithreading = true;
        int _scanTimeout = CONNECT_TIMEOUT;
    };
}