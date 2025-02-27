/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

#include "shared_widgets.hpp"
#include "cseries.hpp"
#include "player.hpp"
#include "preferences.hpp"
#include <algorithm>
#include <vector>

void ChatHistory::append(const ColoredChatEntry& e) {
    m_history.push_back(e);
    if (m_notificationAdapter)
        m_notificationAdapter->contentAdded(e);
}

void ChatHistory::clear() {
    m_history.clear();
    if (m_notificationAdapter)
        m_notificationAdapter->contentCleared();
}

ColorfulChatWidget::~ColorfulChatWidget() {
    if (m_history)
        m_history->setObserver(0);
    delete m_componentWidget;
}

void ColorfulChatWidget::attachHistory(ChatHistory* history) {
    if (m_history)
        m_history->setObserver(0);

    m_history = history;

    m_componentWidget->Clear();
    if (m_history) {
        const vector<ColoredChatEntry>& history_vector = m_history->getHistory();
        for (vector<ColoredChatEntry>::const_iterator it = history_vector.begin(); it != history_vector.end(); ++it) {
            m_componentWidget->Append(*it);
        }

        m_history->setObserver(this);
    }
}

void ColorfulChatWidget::contentAdded(const ColoredChatEntry& e) { m_componentWidget->Append(e); }
