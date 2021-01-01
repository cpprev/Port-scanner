#pragma once

namespace scanner
{
    enum PORT_STATE
    {
        OPENED,
        CLOSED,
        FILTERED
    };

    /// PORT_STATE to string convertion
    /// \param state
    /// \return
    std::string StateToString(PORT_STATE state);

    /// Returns a color from a state (for logging purposes)
    /// \param state
    /// \return
    std::string StateToColor(PORT_STATE state);

    /// For metrics purposes
    /// \param state
    /// \param countOpened
    /// \param countClosed
    /// \param countFiltered
    void IncrementSummary(PORT_STATE state, int& countOpened , int& countClosed, int& countFiltered);
}