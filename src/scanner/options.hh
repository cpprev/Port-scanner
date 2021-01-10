#pragma once

#define CONNECT_TIMEOUT 3

namespace scanner
{
    class Options
    {
    public:
        Options() = default;
        Options(bool verbose, int scanTimeout) : _verbose(verbose), _scanTimeout(scanTimeout) {}

        int GetTimeout() const { return _scanTimeout; }
        void SetTimeout(int timeout) { _scanTimeout = timeout; }
        bool IsVerboseEnabled() const { return _verbose; }
        void SetVerbose(bool verbose) { _verbose = verbose; }
    private:
        bool _verbose = false;
        int _scanTimeout = CONNECT_TIMEOUT;
    };
}